// sandbox.cpp
// This file initializes a gateway, finds all devices on that gateway, and iteratively lowers the winch on each one waiting for the user's prompt to continue. This allows the user to configure the puppet.
//
// Author: tighe
// Date: 2025-03-08

#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"
#include <thread>

#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include "motor_actuator.h"

// Gateway handle
unint g_GtwyHandle;

void move_actuator_to_position(int position) {
  static bool firstRun = true;
  if (firstRun) {
    firstRun = false;

    ERRO err = 0;
    unint unRxData = 0;
    uchar unRxIODcanid = 0;
    int RxPr;  // Position feedback
    int RxVelo;  // Velocity feedback

    MOTION_STATUS_OBJ MotionStatus;
    int Velo;
    int Prel;
    int Pabs;

    // Enable
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 1, &unRxData);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return; }

    // Set origin
    err = SdkSetOrigin(g_GtwyHandle, CANid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetOrigin Fail!\n"; return; }

    std::ofstream logFile("position_log.csv");
    logFile << "ClockTime,ActualPosition,SetpointVelocity\n";

    auto startTime = std::chrono::steady_clock::now();
    auto lastTime = std::chrono::steady_clock::now();
  }

  while (true) {
    err = SdkGetMotionStatus(g_GtwyHandle, CANid, &MotionStatus, &Velo, &Prel, &Pabs);
    if (err != 0) {
      std::cout << "CANid:" << CANid << "  SdkGetMotionStatus Fail!\n";
      logFile.close();
      return;
    }

    // Record the time elapsed since the last iteration
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    lastTime = currentTime;

    // Calculate the velocity based on the change in position
    int deltaPosition = position - Pabs;
    int setpointVelocity = deltaPosition / elapsedTime * 1000; // Assuming elapsedTime interval in ms

    err = SdkSetJogMxn(g_GtwyHandle, CANid, setpointVelocity, &RxVelo);
    if (err != 0) {
      std::cout << "CANid:" << CANid << "  SdkSetJogMxn Fail!\n";
      logFile.close();
      return;
    }

    err = SdkSetBeginMxn(g_GtwyHandle, CANid);
    if (err != 0) {
      std::cout << "CANid:" << CANid << "  SdkSetBeginMxn Fail!\n";
      logFile.close();
      return;
    }

    // Log the setpoint and actual positions
    logFile << elapsedTime << "," << Pabs << "," << setpointVelocity << "\n";

    // // Sleep for a short duration to prevent busy-waiting
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  // Close the log file
  logFile.close();

  // Turn off motor driver
  err = SdkSetMotorOn(g_GtwyHandle, CANid, 0, &unRxData);
  if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return; }
}

int main() {
  configureMotor();

  return 0;
}
