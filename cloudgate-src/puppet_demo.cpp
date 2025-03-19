#include "motor_actuator.h"

#include "inipp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

#include <map>
#include <array>
#include <atomic>

// Define a structure to hold a joint position
struct Point {
    float x;
    float y;
};

// Define a structure to hold the buffered keypoint state
struct JointState {
    static constexpr size_t BUFFER_SIZE = 10; // Fixed buffer size
    std::array<Point, BUFFER_SIZE> buffer;    // Fixed-size circular buffer
    std::atomic<size_t> write_index{0};       // Index for writing
    std::atomic<size_t> read_index{0};        // Index for reading

    MyActuator leftMotor;
    MyActuator rightMotor;

    // Constructor to initialize motors
    JointState() {
        // Default initialization
    }

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
std::map<int, JointState> joint_states;
// Track the end of the log file
std::atomic<bool> done(false);


// Function prototypes
std::pair<float, float> puppet2motor(float X, float Y);
void update_keypoint(int id, float x, float y);
void loadPuppetConfig(const std::string& config_path);


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
    // leftMotor.returnToZero();
    // rightMotor.returnToZero();
    // MyActuator::startMotion();

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
        for (const auto& [id, state] : joint_states) {
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
    // // Calculate distance from motor to target position, subtract d0, convert to counts
    // float a1 = -sqrt(
    //     pow((X - leftMotor.getXpos()), 2.0) + 
    //     pow((Y - leftMotor.getYpos()), 2.0)
    // ) * cpm + leftMotor.getD0();  // counts

    // float a2 = -sqrt(
    //     pow((X - rightMotor.getXpos()), 2.0) + 
    //     pow((Y - rightMotor.getYpos()), 2.0)
    // ) * cpm + rightMotor.getD0();  // counts

    // return std::make_pair(a1, a2);
    return std::make_pair(X, Y); // Placeholder for actual transformation
}


// Update the keypoint state with new position
void update_keypoint(int id, float x, float y) {
    auto& state = joint_states[id];
    state.push_back({x, y});  // Lock-free write
}


// Load puppet configuration
void loadPuppetConfig(const std::string& config_path) {
    try {
        // Create inipp parser
        inipp::Ini<char> ini;
        
        // Open the INI file
        std::ifstream file(config_path);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open config file: " << config_path << std::endl;
            return;
        }
        
        // Parse the file
        ini.parse(file);
        file.close();
        
        // Interpolate variables if needed
        ini.interpolate();
        
        // Process each joint section
        for (const auto& section : ini.sections) {
            // Check if this is a joint section
            if (section.first.substr(0, 6) == "joint_") {
                try {
                    // Extract joint ID
                    int joint_id = std::stoi(section.first.substr(6));
                    std::cout << "Processing joint " << joint_id << std::endl;
                    
                    // Get or create joint state
                    auto& joint_state = joint_states[joint_id];
                    
                    // Parse joint parameters
                    for (const auto& item : section.second) {
                        const std::string& key = item.first;
                        const std::string& value_str = item.second;
                        
                        // Parse left motor parameters
                        if (key == "left_motor_can_id") {
                            int can_id;
                            inipp::extract(value_str, can_id);
                            joint_state.leftMotor.setCANid(can_id);
                            joint_state.leftMotor.setGroupID(joint_id + 100);
                        } 
                        else if (key == "left_motor_x_pos" || key == "left_motor_xpos") {
                            float x_pos;
                            inipp::extract(value_str, x_pos);
                            joint_state.leftMotor.setXpos(x_pos);
                        } 
                        else if (key == "left_motor_y_pos" || key == "left_motor_ypos") {
                            float y_pos;
                            inipp::extract(value_str, y_pos);
                            joint_state.leftMotor.setYpos(y_pos);
                        }
                        
                        // Parse right motor parameters
                        else if (key == "right_motor_can_id") {
                            int can_id;
                            inipp::extract(value_str, can_id);
                            joint_state.rightMotor.setCANid(can_id);
                            joint_state.rightMotor.setGroupID(joint_id + 100);
                        } 
                        else if (key == "right_motor_x_pos" || key == "right_motor_xpos") {
                            float x_pos;
                            inipp::extract(value_str, x_pos);
                            joint_state.rightMotor.setXpos(x_pos);
                        } 
                        else if (key == "right_motor_y_pos" || key == "right_motor_ypos") {
                            float y_pos;
                            inipp::extract(value_str, y_pos);
                            joint_state.rightMotor.setYpos(y_pos);
                        }
                        
                        // Parse home position if available
                        else if (key == "home") {
                            std::string home_str = value_str;
                            // Parse comma-separated home position (x,y)
                            size_t comma_pos = home_str.find(',');
                            if (comma_pos != std::string::npos) {
                                try {
                                    float home_x = std::stof(home_str.substr(0, comma_pos));
                                    float home_y = std::stof(home_str.substr(comma_pos + 1));
                                    // Store home position in the joint state or use it as needed
                                    joint_state.push_back({home_x, home_y});
                                    std::cout << "  Home position: (" << home_x << ", " << home_y << ")" << std::endl;
                                } catch (const std::exception& e) {
                                    std::cerr << "Error parsing home position for joint " << joint_id 
                                              << ": " << e.what() << std::endl;
                                }
                            }
                        }
                    }
                    
                    // Calculate D0 for left motor
                    float left_d0 = sqrt(
                        pow(joint_state.leftMotor.getXpos(), 2.0) + 
                        pow(joint_state.leftMotor.getYpos(), 2.0)
                    ) * cpm;
                    joint_state.leftMotor.setD0(left_d0);
                    
                    // Calculate D0 for right motor
                    float right_d0 = sqrt(
                        pow(joint_state.rightMotor.getXpos(), 2.0) + 
                        pow(joint_state.rightMotor.getYpos(), 2.0)
                    ) * cpm;
                    joint_state.rightMotor.setD0(right_d0);
                    
                    std::cout << "Configured joint " << joint_id << ":" << std::endl;
                    std::cout << "  Left motor: CANid=" << joint_state.leftMotor.getCANid()
                              << ", X=" << joint_state.leftMotor.getXpos()
                              << ", Y=" << joint_state.leftMotor.getYpos()
                              << ", D0=" << joint_state.leftMotor.getD0() << std::endl;
                    std::cout << "  Right motor: CANid=" << joint_state.rightMotor.getCANid()
                              << ", X=" << joint_state.rightMotor.getXpos()
                              << ", Y=" << joint_state.rightMotor.getYpos()
                              << ", D0=" << joint_state.rightMotor.getD0() << std::endl;
                    
                } catch (const std::exception& e) {
                    std::cerr << "Error processing joint section " << section.first << ": " << e.what() << std::endl;
                }
            }
        }
        
        std::cout << "Puppet configuration loaded successfully from " << config_path << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading puppet configuration: " << e.what() << std::endl;
    }
}


// Main function
int main() {
    // Path to the config file
    std::string config_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\puppet_config.ini";

    // Path to the log file  
    std::string logfile_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints_xy.csv";

    // Connect to the gateway and configure the motors
    MyActuator::connectGateway();

    // Load the puppet configuration
    loadPuppetConfig(config_path);

    // Configure the motors
    MyActuator::configureMotors();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // // Start the monitoring and processing threads
    // std::thread monitor_thread(monitorLogFile, logfile_path);
    // std::thread process_thread(processSetpoints);

    // monitor_thread.join();
    // process_thread.join();

    return 0;
}
