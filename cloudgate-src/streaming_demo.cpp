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
    int last_position = -1;
    auto start_time = std::chrono::steady_clock::now();

    std::cout << "Monitoring log file: " << logfile_path << std::endl;

    if (!logfile.is_open()) {
        std::cerr << "Failed to open log file: " << logfile_path << std::endl;
        return;
    }

    float timestamp = 0; // Initialize timestamp in seconds
    float position;
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

            // Parse the line into two float variables
            if (!(iss >> timestamp >> comma >> position) || comma != ',') {
                // Handle parsing error
                std::cerr << "Error parsing line: " << line << std::endl;
                continue;
            } else {
                // std::cout << "Parsed timestamp: " << timestamp << ", position: " << position << std::endl;
                
                // Lock the mutex to safely access the queue and push the new position
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    setpoint_queue.push(position);
                }
                
                // Notify the processing thread
                queue_cv.notify_one();
                last_position = position;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Check every 10ms (40Hz)
    }

    logfile.close();
}

void processSetpoints() {
    while (true) {
        // Lock the mutex and wait until the queue is not empty
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_cv.wait(lock, [] { return !setpoint_queue.empty(); });

        // Get the most recent position from the queue
        int position;
        while (!setpoint_queue.empty()) {
            position = setpoint_queue.front();
            setpoint_queue.pop();
        }
        lock.unlock(); // Unlock the mutex

        // Move the actuator to the new position
        moveActuatorToPosition(5, position);

        // std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Check every 25ms (40Hz)
    }
}

int main() {
    std::string logfile_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints.csv";

    configureMotor();

    std::thread monitor_thread(monitorLogFile, logfile_path);
    std::thread process_thread(processSetpoints);

    monitor_thread.join();
    process_thread.join();

    return 0;
}
