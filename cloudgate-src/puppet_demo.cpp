#include "motor_actuator.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

#include <queue>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <boost/circular_buffer.hpp>
#include <atomic>

// Define a structure to hold a joint position
struct Point {
    float x;
    float y;
};

// Define a structure to hold the buffered keypoint state
struct KeypointState {
    boost::circular_buffer<Point> buffer{10}; // Last 10 positions
    std::atomic<float> latest_x{0};
    std::atomic<float> latest_y{0};
};

std::queue<float> setpoint_queue;
std::unordered_map<int, KeypointState> keypoint_states;

std::mutex queue_mutex;
std::condition_variable queue_cv;
std::atomic<bool> done(false); // Add this line to track end of logfile

// Initialize motor objects
MyActuator leftMotor(11);
MyActuator rightMotor(10);
float cpm = 3200 / 0.24;  // counts/m = counts/rev / m/rev

// Function prototypes
std::pair<float, float> puppet2motor(float X, float Y);
void update_keypoint(int id, float x, float y);

// Function to monitor the log file and push the setpoints to the queue
void monitorLogFile(const std::string& logfile_path) {
    std::ifstream logfile(logfile_path);
    std::ofstream setpoint_log("c:\\Users\\tighe\\uirobot-fw\\utils\\setpoint_log.csv");
    std::string line;
    auto start_time = std::chrono::steady_clock::now();

    std::cout << "Monitoring log file: " << logfile_path << std::endl;

    if (!logfile.is_open()) {
        std::cerr << "Failed to open log file: " << logfile_path << std::endl;
        return;
    }

    if (!setpoint_log.is_open()) {
        std::cerr << "Failed to open setpoint log file: ../utils/setpoint_log.csv" << std::endl;
        return;
    }

    float timestamp = 0; // Initialize timestamp in seconds
    std::vector<float> positions;
    char comma;

    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count()/1000.0; // Convert to seconds

        if (elapsed_time > timestamp) {           
            // Read the next line from the log file 
            if (!std::getline(logfile, line)) {
                // If end of file is reached, print a message and exit the loop
                std::cout << "End of log file reached, exiting monitoring thread." << std::endl;
                done = true; // Set the flag to true
                queue_cv.notify_one(); // Notify the processing thread
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
                // Lock the mutex to safely access the queue and push the new positions
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);

                    // Push the motor positions to the queue
                    // setpoint_queue.push(motor_positions.first);
                    // setpoint_queue.push(motor_positions.second);
                    update_keypoint(0, positions[0], positions[1]);
                    
                    // Log the timestamp and motor positions to the setpoint log file
                    auto motor_positions = puppet2motor(positions[0], positions[1]);
                    setpoint_log << timestamp << "," << motor_positions.first << "," << motor_positions.second << std::endl;
                }
                
                // Notify the processing thread
                queue_cv.notify_one();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Check every 10ms (40Hz)
    }

    // Close the log files
    std::cout << "Closing log files." << std::endl;
    logfile.close();
    setpoint_log.close();
    MyActuator::closeAllLogFiles();
    
    // Return puppet to zero position
    std::cout << "Returning to zero position." << std::endl;
    leftMotor.returnToZero();
    rightMotor.returnToZero();
    MyActuator::startMotion();

    std::cout << "Waiting for return to zero." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    // Disable the motors and terminate the program
    std::cout << "Disabling motors." << std::endl;
    MyActuator::disableMotors();
    return;
}

// Function to push setpoints to the actuators
void processSetpoints() {
    while (true) {
        // Lock the mutex and wait until the keypoint_states is not empty or done is true
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_cv.wait(lock, [] { return !keypoint_states.empty() || done; });

        if (done) {
            break; // Exit the loop if done is true and the keypoint_states is empty
        }

        // Create a local copy of the keypoint states
        std::unordered_map<int, std::vector<Point>> local_keypoint_states;

        // Copy the keypoint states to the local variable
        for (const auto& [id, state] : keypoint_states) {
            local_keypoint_states[id] = std::vector<Point>(state.buffer.begin(), state.buffer.end());
        }

        lock.unlock(); // Unlock the mutex

        // Process the local copy of the keypoint states
        for (const auto& [id, points] : local_keypoint_states) {
            // Check if the buffer is not empty
            if (!points.empty()) {
                // Get the latest x and y values from the local copy of the circular buffer
                const Point& latest_point = points.back();
                float latest_x = latest_point.x;
                float latest_y = latest_point.y;

                // Move the actuator to the new position
                auto motor_positions = puppet2motor(latest_x, latest_y);
                leftMotor.setMotorPos(motor_positions.first);
                rightMotor.setMotorPos(motor_positions.second);

                MyActuator::startMotion();

                // // Simulate actuator movement
                // std::cout << "id: " << id << ", a1: " << motor_positions.first << ", a2: " << motor_positions.second << std::endl;
                // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
}

// Transform the XY coordinates in meters to actuator positions in counts
std::pair<float, float> puppet2motor(float X, float Y) {
    // Calculate distance from motor to target position, subtract d0, convert to counts
    float a1 = -sqrt(
        pow((X - leftMotor.getXpos()), 2.0) + 
        pow((Y - leftMotor.getYpos()), 2.0)
    ) * cpm + leftMotor.getD0();  // counts

    float a2 = -sqrt(
        pow((X - rightMotor.getXpos()), 2.0) + 
        pow((Y - rightMotor.getYpos()), 2.0)
    ) * cpm + rightMotor.getD0();  // counts

    return std::make_pair(a1, a2);
}

// Update the keypoint state with new position
void update_keypoint(int id, float x, float y) {
    auto& state = keypoint_states[id];
    state.buffer.push_back({x, y});
    state.latest_x.store(x, std::memory_order_relaxed);
    state.latest_y.store(y, std::memory_order_relaxed);
}

int main() {
    // Path to the log file  
    std::string logfile_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints_xy.csv";

    // Set the initial positions of the motors
    leftMotor.setXpos(-0.96);  // meters
    leftMotor.setYpos(1.56);  // meters
    leftMotor.setD0(sqrt(pow(leftMotor.getXpos(), 2.0) + pow(leftMotor.getYpos(), 2.0)) * cpm);
    std::cout << "leftMotor D0: " << leftMotor.getD0() << std::endl;
    rightMotor.setXpos(1.03);
    rightMotor.setYpos(1.56);
    rightMotor.setD0(sqrt(pow(rightMotor.getXpos(), 2.0) + pow(rightMotor.getYpos(), 2.0)) * cpm);
    std::cout << "rightMotor D0: " << rightMotor.getD0() << std::endl;

    // Connect to the gateway and configure the motors
    MyActuator::connectGateway();
    MyActuator::configureMotors();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Start the monitoring and processing threads
    std::thread monitor_thread(monitorLogFile, logfile_path);
    std::thread process_thread(processSetpoints);

    monitor_thread.join();
    process_thread.join();

    return 0;
}
