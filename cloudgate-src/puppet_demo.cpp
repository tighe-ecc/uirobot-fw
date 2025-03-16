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
#include <atomic>

std::queue<float> setpoint_queue;
std::mutex queue_mutex;
std::condition_variable queue_cv;
std::atomic<bool> done(false); // Add this line to track end of logfile
    
// Initialize motor objects
MyActuator leftMotor(10);
MyActuator rightMotor(11);
float cpm = 3200 / 0.24;  // counts/m = counts/rev / m/rev

// Function to transform the XY coordinates in meters to actuator positions in counts
std::pair<float, float> puppet2motor(float X, float Y);

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

                    // Convert the puppet positions to motor positions
                    auto motor_positions = puppet2motor(positions[0], positions[1]);
                    
                    // Push the motor positions to the queue
                    setpoint_queue.push(motor_positions.first);
                    setpoint_queue.push(motor_positions.second);

                    // Log the timestamp and motor positions to the setpoint log file
                    setpoint_log << timestamp << "," << motor_positions.first << "," << motor_positions.second << std::endl;
                }
                
                // Notify the processing thread
                queue_cv.notify_one();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Check every 10ms (40Hz)
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Close the log files
    logfile.close();
    setpoint_log.close();
    MyActuator::closeAllLogFiles();
    
    // Return puppet to zero position
    leftMotor.returnToZero();
    rightMotor.returnToZero();
    MyActuator::startMotion();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Disable the motors and terminate the program
    MyActuator::disableMotors();
    return;
}

// Function to push setpoints to the actuators
void processSetpoints() {
    while (true) {
        // Lock the mutex and wait until the queue is not empty or done is true
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_cv.wait(lock, [] { return !setpoint_queue.empty() || done; });

        if (done && setpoint_queue.empty()) {
            break; // Exit the loop if done is true and the queue is empty
        }

        // Get the most recent positions from the queue
        std::vector<float> positions;
        while (!setpoint_queue.empty()) {
            positions.push_back(static_cast<float>(setpoint_queue.front()));
            setpoint_queue.pop();
        }
        lock.unlock(); // Unlock the mutex

        // Move the actuator to the new position
        leftMotor.setMotorPos(positions[0]);
        rightMotor.setMotorPos(positions[1]);

        MyActuator::startMotion();

        // // Simulate actuator movement
        // std::cout << "a1: " << positions[0] << ", a2: " << positions[1] << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

int main() {
    // Path to the log file  
    std::string logfile_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints_xy.csv";

    // Set the initial positions of the motors
    leftMotor.setXpos(-0.546);  // meters
    leftMotor.setYpos(1);  // meters
    leftMotor.setD0(sqrt(pow(leftMotor.getXpos(), 2.0) + pow(leftMotor.getYpos(), 2.0)) * cpm);
    std::cout << "leftMotor D0: " << leftMotor.getD0() << std::endl;
    rightMotor.setXpos(0.483);
    rightMotor.setYpos(1.2);
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
