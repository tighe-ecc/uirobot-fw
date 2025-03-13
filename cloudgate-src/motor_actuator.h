#ifndef MOTOR_ACTUATOR_H
#define MOTOR_ACTUATOR_H

#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"

#include <iostream>
#include <thread>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

class MyActuator {
public:
    MyActuator(int CANid) : CANid(CANid), md{} {
        md.firstRun = true;
        md.err = 0;
        md.unRxData = 0;
        md.lastVelocity = 0;
        md.integralError = 0;
        md.lastPositionError = 0;
        md.startTime = std::chrono::steady_clock::now();
        md.lastTime = std::chrono::steady_clock::now();
        instances.push_back(this);
    }

    ~MyActuator() {
        // Remove this instance from the instances vector
        auto it = std::remove(instances.begin(), instances.end(), this);
        instances.erase(it, instances.end());
    }

    // Static methods
    static void connectGateway();  // Connect to the gateway
    static void configureMotors();  // Configure all motors on bus
    static void startMotion();  // Start tracking to target motor velocity
    static void disableMotors();  // Close the actuator stream
    static void closeAllLogFiles();  // Close all open log files
    
    // Instance methods
    void setMotorPos(int position);  // Set the target position for the motor
    void returnToZero();  // Return the motor to zero position

    // Getter and Setter for xpos
    float getXpos() const {
        return md.xpos;
    }

    void setXpos(float xpos) {
        md.xpos = xpos;
    }

    // Getter and Setter for ypos
    float getYpos() const {
        return md.ypos;
    }

    void setYpos(float ypos) {
        md.ypos = ypos;
    }

    // Getter and Setter for d0
    float getD0() const {
        return md.d0;
    }

    void setD0(float d0) {
        md.d0 = d0;
    }
    
private:
    int CANid;
    static unsigned int g_GtwyHandle;
    static int MemberQuantity;
    static std::vector<MyActuator*> instances;  // Vector to keep track of all instances

    struct MotionData {
        bool firstRun;
        ERRO err;
        unint unRxData;
        int RxPr;
        int RxVelo;
        int lastVelocity;
        MOTION_STATUS_OBJ MotionStatus;
        int Velo;
        int Prel;
        int Pabs;
        float integralError;
        float lastPositionError;
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point lastTime;
        std::ofstream logFile;
        float xpos;  // x position of motor
        float ypos;  // y position of motor 
        float d0;  // distance from puppet CRF origin in counts
    } md;
};

#endif // MOTOR_ACTUATOR_H
