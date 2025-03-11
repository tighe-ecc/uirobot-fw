#include "motor_actuator.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sstream>

std::queue<int> setpoint_queue;
std::mutex queue_mutex;
std::condition_variable queue_cv;

// void moveActuatorToPosition(int position) {
//     // Placeholder function to move the actuator
//     std::cout << "Moving actuator to position: " << position << std::endl;
// }

void monitorLogFile(const std::string& logfile_path) {
    std::ifstream logfile(logfile_path);
    std::string line;
    auto start_time = std::chrono::steady_clock::now();

    std::cout << "Monitoring log file: " << logfile_path << std::endl;

    if (!logfile.is_open()) {
        std::cerr << "Failed to open log file: " << logfile_path << std::endl;
        return;
    }

    float timestamp = 0; // Initialize timestamp in seconds
    std::vector<float> positions;
    char comma;

    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count()/1000.0; // Convert to seconds

        // std::cout << "Elapsed time: " << elapsed_time << "s, Timestamp: " << timestamp << "s, Difference: " << (elapsed_time - timestamp) << "s" << std::endl;

        if (elapsed_time > timestamp) {           
            // Read the next line from the log file 
            if (!std::getline(logfile, line)) {
                // If end of file is reached, print a message and exit the loop
                std::cout << "End of log file reached, exiting monitoring thread." << std::endl;
                break;
            }
            std::istringstream iss(line);

            // Parse the line into timestamp and positions
            if (!(iss >> timestamp >> comma) || comma != ',') {
                // Handle parsing error
                std::cerr << "Error parsing line: " << line << std::endl;
                continue;
            }

            positions.clear();
            float position;
            while (iss >> position) {
                positions.push_back(position);
                if (iss.peek() == ',') {
                    iss.ignore();
                }
            }

            if (positions.empty()) {
                // Handle parsing error
                std::cerr << "Error parsing positions in line: " << line << std::endl;
                continue;
            } else {
                // std::cout << "Parsed timestamp: " << timestamp << ", positions: ";
                // for (const auto& pos : positions) std::cout << pos << " ";
                // std::cout << std::endl;
                
                // Lock the mutex to safely access the queue and push the new positions
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    for (const auto& pos : positions) {
                        setpoint_queue.push(pos);
                    }
                }
                
                // Notify the processing thread
                queue_cv.notify_one();
            }
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Check every 10ms (40Hz)
    }

    logfile.close();
    closeActuatorStream(5);
    closeActuatorStream(11);
}

void processSetpoints() {
    while (true) {
        // Lock the mutex and wait until the queue is not empty
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_cv.wait(lock, [] { return !setpoint_queue.empty(); });

        // Get the most recent positions from the queue
        std::vector<float> positions;
        while (!setpoint_queue.empty()) {
            positions.push_back(setpoint_queue.front());
            setpoint_queue.pop();
        }
        lock.unlock(); // Unlock the mutex

        // Move the actuator to the new position
        moveActuatorToPosition(5, positions[0]);
        // moveActuatorToPosition(11, positions[1]);

        startActuatorMotion();

        // std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Check every 25ms (40Hz)
    }
}

int main() {
    std::string logfile_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints.csv";

    // Configure the motor
    configureMotors();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Start the monitoring and processing threads
    std::thread monitor_thread(monitorLogFile, logfile_path);
    std::thread process_thread(processSetpoints);

    monitor_thread.join();
    process_thread.join();

    return 0;
}
