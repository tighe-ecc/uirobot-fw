#include "motor_actuator.h"
#include "joint-osc-listener.h"

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

#define ADDRESS "0.0.0.0"
#define PORT 7600

const bool debug = false;  // Enable debug mode

// Define a structure to hold a joint position
struct Point {
    float x;
    float y;
};

// Define a structure to hold the buffered keypoint state
struct JointState {
    static constexpr size_t BUFFER_SIZE = 10;   // Fixed buffer size
    std::array<Point, BUFFER_SIZE> buffer;      // Fixed-size circular buffer
    std::atomic<size_t> write_index{0};         // Index for writing
    std::atomic<size_t> read_index{0};          // Index for reading (not used)

    std::array<float, 2> home;                  // Home position [x, y]

    MyActuator leftMotor;                       // Left motor actuator
    MyActuator rightMotor;                      // Right motor actuator

    // Constructor to initialize motors and home position
    JointState()
        : leftMotor("leftMotor"),  // Initialize leftMotor with a name
          rightMotor("rightMotor"), // Initialize rightMotor with a name
          home{0.0f, 0.0f}          // Default home position (0, 0)
    {
        // Additional initialization if needed
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
std::pair<float, float> puppet2motor(float X, float Y, const JointState& joint);
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
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() / 1000.0; // Convert to seconds

        if (elapsed_time > timestamp) {
            // Read the next line from the log file
            if (!std::getline(logfile, line)) {
            // If end of file is reached, print a message and exit the loop
            std::cout << "End of log file reached, exiting monitoring thread." << std::endl;
            done = true; // Set the flag to true
            break;
            }
            std::istringstream iss(line);

            // Parse the line into timestamp, joint_id, and positions
            int joint_id;
            if (!(iss >> timestamp >> comma) || comma != ',' || !(iss >> joint_id >> comma) || comma != ',') {
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

            if (positions.size() < 2) {
            // Handle parsing error
            std::cerr << "Error parsing positions in line: " << line << std::endl;
            continue;
            } else {
            // Push the motor positions to the queue
            update_keypoint(joint_id, positions[0], positions[1]);

            // Log the timestamp, joint_id, and motor positions to the setpoint log file
            auto motor_positions = puppet2motor(positions[0], positions[1], joint_states[joint_id]);
            setpoint_log << timestamp << "," << joint_id << "," << motor_positions.first << "," << motor_positions.second << std::endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(25)); // 40Hz
    }

    // Close the log files
    std::cout << "Closing log files." << std::endl;
    logfile.close();
    setpoint_log.close();
    return;
}


// Function to push setpoints to the actuators
void processSetpoints() {
    while (!done) {
        // Process each keypoint state
        for (auto& [id, state] : joint_states) {
            if (debug) { std::cout << "Processing joint id: " << id << std::endl; }
            
            // Lock-free read of the latest point
            Point latest_point = state.get_latest();

            if (debug) {
                std::cout << "id: " << id << ", x: " << latest_point.x << ", y: " << latest_point.y << std::endl;
            }

            // Transform the XY coordinates to motor positions
            auto motor_positions = puppet2motor(latest_point.x, latest_point.y, state);

            // Set motor positions
            state.leftMotor.setMotorPos(motor_positions.first);
            int leftVel = state.leftMotor.getLastVelocity();
            state.rightMotor.setMotorPos(motor_positions.second);
            int rightVel = state.rightMotor.getLastVelocity();
            
            if (debug) {
                // Simulate actuator movement
                std::cout << "id: " << id << ", a1: " << motor_positions.first << ", a2: " << motor_positions.second << std::endl;
                std::cout << "id: " << id << ", leftVel: " << leftVel << ", rightVel: " << rightVel << std::endl;
                std::cout << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            // Start the motion
            MyActuator::startMotion();
        }
        
        if (debug) { std::cout << std::endl; }
    }
    
    // Return puppet to zero position
    std::cout << "Returning to zero position." << std::endl;
    for (auto& [id, state] : joint_states) {
        state.leftMotor.returnToZero();
        state.rightMotor.returnToZero();
    }
    MyActuator::startMotion();
    
    std::cout << "Waiting for return to zero." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    MyActuator::closeAllLogFiles();
    
    // Disable the motors and terminate the program
    std::cout << "Disabling motors." << std::endl;
    MyActuator::disableMotors();
}


// Transform the XY coordinates in meters to actuator positions in counts
std::pair<float, float> puppet2motor(float X, float Y, const JointState& joint) {
    // Calculate distance from motor to target position, subtract d0, convert to counts
    float a1 = -sqrt(
        pow((X - joint.leftMotor.getXpos()), 2.0) + 
        pow((Y - joint.leftMotor.getYpos()), 2.0)
    ) * cpm + joint.leftMotor.getD0();  // counts

    float a2 = -sqrt(
        pow((X - joint.rightMotor.getXpos()), 2.0) + 
        pow((Y - joint.rightMotor.getYpos()), 2.0)
    ) * cpm + joint.rightMotor.getD0();  // counts

    return std::make_pair(a1, a2);
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
                                    // Store home position in the home tuple of joint state
                                    joint_state.home = {home_x, home_y};
                                    // Populate the buffer with the home position
                                    for (size_t i = 0; i < JointState::BUFFER_SIZE; ++i) {
                                        joint_state.buffer[i] = {home_x, home_y};
                                    }
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
                        pow((joint_state.home[0] - joint_state.leftMotor.getXpos()), 2.0) + 
                        pow((joint_state.home[1] - joint_state.leftMotor.getYpos()), 2.0)
                    ) * cpm;
                    joint_state.leftMotor.setD0(left_d0);

                    if (debug) {
                    std::cout << "Joint ID: " << joint_id << std::endl;
                    std::cout << "  Left motor D0: " << left_d0 << std::endl;
                    }
                    
                    // Calculate D0 for right motor
                    float right_d0 = sqrt(
                        pow((joint_state.home[0] - joint_state.rightMotor.getXpos()), 2.0) + 
                        pow((joint_state.home[1] - joint_state.rightMotor.getYpos()), 2.0)
                    ) * cpm;
                    joint_state.rightMotor.setD0(right_d0);

                    if (debug) {
                    std::cout << "  Left motor D0: " << right_d0 << std::endl;
                    
                    std::cout << "Configured joint " << joint_id << ":" << std::endl;
                    std::cout << "  Left motor: CANid=" << joint_state.leftMotor.getCANid()
                              << ", X=" << joint_state.leftMotor.getXpos()
                              << ", Y=" << joint_state.leftMotor.getYpos()
                              << ", D0=" << joint_state.leftMotor.getD0() << std::endl;
                    std::cout << "  Right motor: CANid=" << joint_state.rightMotor.getCANid()
                              << ", X=" << joint_state.rightMotor.getXpos()
                              << ", Y=" << joint_state.rightMotor.getYpos()
                              << ", D0=" << joint_state.rightMotor.getD0() << std::endl;
                    }
                    
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

void oscResponder(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6, float x7, float y7) {
    // This function can be used to handle OSC messages
    // and update the joint states accordingly.
    // For now, it's just a placeholder.
    // std::cout << "Received OSC message with coordinates: "
    //           << "(" << x1 << ", " << y1 << "), "
    //           << "(" << x2 << ", " << y2 << "), "
    //           << "(" << x3 << ", " << y3 << "), "
    //           << "(" << x4 << ", " << y4 << "), "
    //           << "(" << x5 << ", " << y5 << "), "
    //           << "(" << x6 << ", " << y6 << "), "
    //           << "(" << x7 << ", " << y7 << ")" 
    //           << std::endl;

    std::array<std::array<float, 2>, 7> coordinates = {{
        {x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}, {x5, y5}, {x6, y6}, {x7, y7}
    }};

    // for (size_t i = 0; i < coordinates.size(); ++i) {
    //     update_keypoint(i, coordinates[i][0], coordinates[i][1]);
    // }

    update_keypoint(0, x1, y1);
    update_keypoint(3, x4, y4);
    update_keypoint(6, x7, y7);
}


// Main function
int main() {
    // Path to the config file
    std::string config_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\config.ini";

    // Path to the log file  
    std::string logfile_path = "c:\\Users\\tighe\\uirobot-fw\\cloudgate\\setpoints_xy.csv";

    // Connect to the gateway and configure the motors
    MyActuator::connectGateway();

    // Load the puppet configuration
    loadPuppetConfig(config_path);

    // Configure the motors
    MyActuator::configureMotors();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Start the monitoring and processing threads
    // std::thread monitor_thread(monitorLogFile, logfile_path);
    std::thread process_thread(processSetpoints);

    // monitor_thread.join();
    // process_thread.join();

    JointListener listener;
    listener.callback = oscResponder;
    UdpListeningReceiveSocket s(IpEndpointName(ADDRESS, PORT), &listener);
    std::cout << "listening on " << ADDRESS << ":" << PORT << std::endl;
    s.RunUntilSigInt();

    return 0;
}
