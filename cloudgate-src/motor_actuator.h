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
    }

    ~MyActuator() {
        if (md.logFile.is_open()) {
            md.logFile.close();
        }
    }

    // Static methods
    static void connectGateway();  // Connect to the gateway
    static void configureMotors();  // Configure all motors on bus
    static void startMotion();  // Start tracking to target motor velocity
    static void disableMotors();  // Close the actuator stream

    // Instance methods
    void setMotorPos(int position);

private:
    int CANid;
    static unsigned int g_GtwyHandle;
    static int MemberQuantity;

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
    } md;
};

#endif // MOTOR_ACTUATOR_H
