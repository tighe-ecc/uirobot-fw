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
#include <map>
#include <boost/circular_buffer.hpp>
#include <array>
#include <atomic>

// Define a structure to hold a joint position
struct Point {
    float x;
    float y;
};

// Define a structure to hold the buffered keypoint state
struct KeypointState {
    static constexpr size_t BUFFER_SIZE = 10; // Fixed buffer size
    std::array<Point, BUFFER_SIZE> buffer;    // Fixed-size circular buffer
    std::atomic<size_t> write_index{0};       // Index for writing
    std::atomic<size_t> read_index{0};        // Index for reading

    // Add a new point to the buffer (lock-free)
    void push_back(const Point& point) {
        size_t index = write_index.fetch_add(1, std::memory_order_relaxed) % BUFFER_SIZE;
        buffer[index] = point;
    }

    // Get the latest point from the buffer (lock-free)
    Point get_latest() const {
        size_t index = (write_index.load(std::memory_order_relaxed) - 1) % BUFFER_SIZE;
        return buffer[index];
    }
};

// Define a queue to hold the setpoints for the motors
std::map<int, KeypointState> keypoint_states;
std::atomic<bool> done(false);  // Add this line to track end of logfile

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
                // Push the motor positions to the queue
                update_keypoint(0, positions[0], positions[1]);
                
                // Log the timestamp and motor positions to the setpoint log file
                auto motor_positions = puppet2motor(positions[0], positions[1]);
                setpoint_log << timestamp << "," << motor_positions.first << "," << motor_positions.second << std::endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 40Hz
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
    while (!done) {
        // Process each keypoint state
        for (const auto& [id, state] : keypoint_states) {
            // Lock-free read of the latest point
            Point latest_point = state.get_latest();

            // Transform the XY coordinates to motor positions
            auto motor_positions = puppet2motor(latest_point.x, latest_point.y);

            // // Set motor positions
            // leftMotor.setMotorPos(motor_positions.first);
            // rightMotor.setMotorPos(motor_positions.second);

            // // Start motor motion
            // MyActuator::startMotion();
            
            // Simulate actuator movement
            std::cout << "id: " << id << ", a1: " << motor_positions.first << ", a2: " << motor_positions.second << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
    state.push_back({x, y});  // Lock-free write
}

int main() {
    // Path to the log file  
    std::string logfile_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints_xy.csv";

    // Set the initial positions of the motors
    leftMotor.setXpos(-0.96);  // m
    leftMotor.setYpos(1.56);  // m
    leftMotor.setD0(sqrt(pow(leftMotor.getXpos(), 2.0) + pow(leftMotor.getYpos(), 2.0)) * cpm);
    // std::cout << "leftMotor D0: " << leftMotor.getD0() << std::endl;
    rightMotor.setXpos(1.03);  // m
    rightMotor.setYpos(1.56);  // m
    rightMotor.setD0(sqrt(pow(rightMotor.getXpos(), 2.0) + pow(rightMotor.getYpos(), 2.0)) * cpm);
    // std::cout << "rightMotor D0: " << rightMotor.getD0() << std::endl;

    // // Connect to the gateway and configure the motors
    // MyActuator::connectGateway();
    // MyActuator::configureMotors();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Start the monitoring and processing threads
    std::thread monitor_thread(monitorLogFile, logfile_path);
    std::thread process_thread(processSetpoints);

    monitor_thread.join();
    process_thread.join();

    return 0;
}
