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

// Gateway handle
unint g_GtwyHandle;

int main() {
  std::cout << "Launching sandbox.cpp\n";
  ERRO err = 0;

  // Set the current configuration type
  bool m_bIs232Gtwy =  true;
  bool m_bIsEthGtwy = false;
  GATEWAY_INFO_OBJ Gtwy;          
  if (m_bIs232Gtwy) {
    //serial device config
    Gtwy.detail.type = UIGW3_GW2513;
    Gtwy.COMidx = 4;
    Gtwy.COMbaud = 57600;

  }
  else if (m_bIsEthGtwy) {
    //Ethernet device config
    Gtwy.detail.type = UIGW3_ETHCAN;
    Gtwy.detail.IP[0] = 254;
    Gtwy.detail.IP[1] = 1;
    Gtwy.detail.IP[2] = 168;
    Gtwy.detail.IP[3] = 192;
    if ((!Gtwy.tcp.IPaddr)) {
    std::cout << "Gateway parameter error!\n";
      return -1;
    }
    Gtwy.tcp.IPport = 8888;

  }

  MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
  // Initialize the gateway
  err = SdkStartCanNet(&Gtwy, Member, 0);
  // Record the gateway handle
  g_GtwyHandle = Gtwy.GtwyHandle;				 

  if (err) {
    std::cout << "No gateway found, please check the gateway communication parameters!\n";
    return -1;
  }
  // Number of members
  int  MemberQuantity = 0;           // Number of members
  MemberQuantity = Gtwy.MemberQty;

  int CANid = 0;

  for (int i = MemberQuantity; i > 0; i--) {
    CANid = Member[i - 1].CANnid;
    std::cout << "CAN id:" << (int)Member[i - 1].CANnid
      << "  Model:" << Member[i - 1].Model.ModelStr
      << "  Firmware:" << Member[i - 1].Model.FirmVer
      << std::endl;
  }

  unint unRxData = 0;
  uchar unRxIODcanid = 0;
  int RxPr;  // Position feedback
	int RxVelo;  // Velocity feedback

  MOTION_STATUS_OBJ MotionStatus;
  int Velo;
  int Prel;
  int Pabs;

  // Demo actuator config --------------------------------------------------------------------------
	bool bGetPara = true;

	if (bGetPara)
	{
		//	Intial Config
		
		// Obtain whether to power on Enable 0 = disable, 1 = Enable
		err = SdkGetInitialConfig(g_GtwyHandle, CANid, 0, &unRxData);
		if (err) { std::cout << "CANid:" << CANid << "  SdkGetInitialConfig Fail!\n"; return -1; }
    else {
      std::cout << "CANid:" << CANid << "  Power-on Enable = " << unRxData << "\n";
    }

    // Get positive turn setting, 0 = CW, 1= CCW
		err = SdkGetInitialConfig(g_GtwyHandle, CANid, 1, &unRxData);
		if (err) { std::cout << "CANid:" << CANid << "  SdkGetInitialConfig Fail!\n"; return -1; }
    else {
      std::cout << "CANid:" << CANid << "  Positive turn setting = " << unRxData << "\n";
    }

		// Acceleration and deceleration mode 0 = value, 1 = time
		err = SdkSetInitialConfig(g_GtwyHandle, CANid, 4, 0, &unRxData);
    if (err) { std::cout << "CANid:" << CANid << "  SdkSetInitialConfig Fail!\n"; return -1; }
    else {
      std::cout << "CANid:" << CANid << "  Acceleration and deceleration mode = " << unRxData << "\n";
    }
    
    // Set acceleration/deceleration rate to that of gravity
    err = SdkSetAcceleration(g_GtwyHandle, CANid, 130667, &unRxData);
    if (err) { std::cout << "CANid:" << CANid << "  SdkGetAcceleration Fail!\n"; return -1; }
    else {
      std::cout << "CANid:" << CANid << "  Acceleration rate = " << unRxData << "\n";
    }
    err = SdkSetDeceleration(g_GtwyHandle, CANid, 130667, &unRxData);
    if (err) { std::cout << "CANid:" << CANid << "  SdkGetAcceleration Fail!\n"; return -1; }
    else {
      std::cout << "CANid:" << CANid << "  Deceleration rate = " << unRxData << "\n";
    }

		// Information Enable

		//P1 Port change notification 0 = disable, 1 = Enable.
		err = SdkGetInformEnable(g_GtwyHandle, CANid, 0, &unRxData);
		if (err) { std::cout << "CANid:" << CANid << "  SdkGetInformEnable Fail!\n"; return -1; }

		//PTP movement in place notification 0 = disable, 1 = Enable;
		err = SdkGetInformEnable(g_GtwyHandle, CANid, 8, &unRxData);
		if (err) { std::cout << "CANid:" << CANid << "  SdkGetInformEnable Fail!\n"; return -1; }

		//	Motor Config Information
		
		//Get   subdivide[1.2.4.8.16]
		err = SdkGetMotorConfig(g_GtwyHandle, CANid, 0, &unRxData);
		if (err) { std::cout << "CANid:" << CANid << "  SdkGetMotorConfig Fail!\n"; return -1; }

		// Get current [10...80]
		err = SdkGetMotorConfig(g_GtwyHandle, CANid, 1, &unRxData);
		if (err) { std::cout << "CANid:" << CANid << "  SdkGetMotorConfig Fail!\n"; return -1; }

		// Get the standby current flow [0...100(%)]
		err = SdkGetMotorConfig(g_GtwyHandle, CANid, 2, &unRxData);
		if (err) { std::cout << "CANid:" << CANid << "  SdkGetMotorConfig Fail!\n"; return -1; }

		// Obtain automatic power-on enable delay [0...60000(ms)]
		err = SdkGetMotorConfig(g_GtwyHandle, CANid, 3, &unRxData);
		if (err) { std::cout << "CANid:" << CANid << "  SdkGetMotorConfig Fail!\n"; return -1; }
	}

  // Demo PTP move ---------------------------------------------------------------------------------
  bool bTest_PTPMove = false;

  if (bTest_PTPMove)
  {

    // Turn on motor driver
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 1, &unRxData);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }

    // Set origin
    err = SdkSetOrigin(g_GtwyHandle, CANid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetOrigin Fail!\n"; return -1; }

    // Cycle run relative displacement
    // g_bSTPFlag  = false;
    // Cycle run relative displacement
    err = SdkSetPtpMxnA(g_GtwyHandle, CANid, 160000, 10 * 3200, &RxVelo, &RxPr);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetPtpMxnA Fail!\n"; return -1; }


    err = SdkSetBeginMxn(g_GtwyHandle, CANid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetBeginMxn Fail!\n"; return -1; }


    std::cout << "CANid:" << CANid << "  SdkSetPtpMxnA 100000!\n";

    // // Wait flag bit
    // if (WaitFlag(&g_bSTPFlag))
    // {
    //   std::cout << "CANid:" << CANid << "STPIE!\n";

    // }
    // else
    // {
    //   unTimeOutCount++;
    //   std::cout << "CANid:" << CANid << "WaitFlag TimeOut!\n";
    // }

    Sleep(2000);

    // g_bSTPFlag = false;
    // Set relative displacement and speed
    err = SdkSetPtpMxnA(g_GtwyHandle, CANid, 160000, 0,  &RxVelo, &RxPr);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetPtpMxnA Fail!\n"; return -1; }


    err = SdkSetBeginMxn(g_GtwyHandle, CANid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetBeginMxn Fail!\n"; return -1; }

    std::cout << "CANid:" << CANid << "  SdkSetPtpMxnA 0!\n";

    // // Wait flag bit
    // if (WaitFlag(&g_bSTPFlag))
    // {
    //   std::cout << "CANid:" << CANid << "STPIE !\n";
    // }
    // else
    // {
    //   unTimeOutCount++;
    //   std::cout << "CANid:" << CANid << "WaitFlag TimeOut!\n";
    // }

    Sleep(2000);

    // Turn off motor driver
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 0, &unRxData);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }
  }

  // Demo PVT mode ---------------------------------------------------------------------------------
  bool bTest_PVTMove = false;

  if (bTest_PVTMove)
  {
    std::cout << "PVT mode test\n";

    // Turn on motor driver
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 1, &unRxData);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }

    // Set origin
    err = SdkSetOrigin(g_GtwyHandle, CANid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetOrigin Fail!\n"; return -1; }

    // Set actuator to PVT mode
    err = SdkSetPvtIODevice(g_GtwyHandle, CANid, CANid, &unRxIODcanid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetPvtIODevice Fail!\n"; return -1; }

    // Reset PVT buffer
    err = SdkSetPvtRstBuf(g_GtwyHandle, CANid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetPvtRstBuf Fail!\n"; return -1; }

    // Turn off motor driver
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 0, &unRxData);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }
  }

	// Demo jog sinusoid -----------------------------------------------------------------------------
	bool bTest_JogMove = false;

	if (bTest_JogMove)
	{
    std::cout << "Jog mode test\n";

		// Enable
		err = SdkSetMotorOn(g_GtwyHandle, CANid, 1, &unRxData);
		// if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }

    // Sinusoidal sweep to ±160000 cps (max speed 3000rpm)
    const int numEntries = 100;
    int velocities[numEntries];
    for (int i = 0; i < numEntries; ++i) {
      velocities[i] = static_cast<int>(160000 * sin(2 * M_PI * i / numEntries));
    }

    // Iterate through the array and update the SdkSetJogMxn setpoint every 25ms
    for (int i = 0; i < numEntries; ++i) {
      err = SdkGetMotionStatus(g_GtwyHandle, CANid, &MotionStatus, &Velo, &Prel, &Pabs);
      if (err != 0) {
        std::cout << "CANid:" << CANid << "  SdkGetMotionStatus Fail!\n";
        return -1;
      }

      err = SdkSetJogMxn(g_GtwyHandle, CANid, velocities[i], &RxVelo);
      if (err != 0) {
        std::cout << "CANid:" << CANid << "  Maximum velocity reached = " << velocities[i] << "\n";
        std::cout << "CANid:" << CANid << "  SdkSetJogMxn Fail!\n";
        return -1;
      }

      err = SdkSetBeginMxn(g_GtwyHandle, CANid);
      if (err != 0) {
        std::cout << "CANid:" << CANid << "  SdkSetBeginMxn Fail!\n";
        return -1;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    // Turn off motor driver
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 0, &unRxData);
    // if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }
	}

	// Demo jog with pos check -----------------------------------------------------------------------
	bool bTest_JogWithPos = true;

	if (bTest_JogWithPos)
	{
    std::cout << "Jog with position checking test\n";

		// Enable
		err = SdkSetMotorOn(g_GtwyHandle, CANid, 1, &unRxData);
		// if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }

    // Set origin
    err = SdkSetOrigin(g_GtwyHandle, CANid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetOrigin Fail!\n"; return -1; }

    int sample_time = 200; // ms

    // Sinusoidal sweep to ±160000 cps (max speed 3000rpm)
    const int numEntries = 10;
    int positions[numEntries];
    for (int i = 0; i < numEntries / 2; ++i) {
      positions[i] = static_cast<int>((1 * 3200) * (2.0 * i / numEntries));
    }
    for (int i = numEntries / 2; i < numEntries; ++i) {
      positions[i] = static_cast<int>((1 * 3200) * (2.0 - 2.0 * i / numEntries));
    }
    int velocities[numEntries];
    for (int i = 0; i < numEntries; ++i) {
      velocities[i] = static_cast<int>(160000 * sin(2 * M_PI * i / numEntries));
    }

    // Open a log file to save setpoint and actual positions
    std::ofstream setpointLogFile("../utils/setpoint_log.csv");
    setpointLogFile << "Time,Position\n";
    for (int i = 0; i < numEntries; ++i) {
      setpointLogFile << (i * sample_time) << "," << positions[i] << "\n";
    }
    setpointLogFile.close();
    std::ofstream logFile("../utils/position_log.csv");
    logFile << "ClockTime,ActualPosition,SetpointVelocity\n";

    auto startTime = std::chrono::steady_clock::now();

    // Iterate through the array and update the SdkSetJogMxn setpoint every sample_time
    for (int i = 0; i < numEntries; ++i) {
      int targetPosition = positions[i];

      while (std::chrono::steady_clock::now() - startTime < std::chrono::milliseconds(sample_time * (i + 1))) {
        err = SdkGetMotionStatus(g_GtwyHandle, CANid, &MotionStatus, &Velo, &Prel, &Pabs);
        if (err != 0) {
          std::cout << "CANid:" << CANid << "  SdkGetMotionStatus Fail!\n";
          logFile.close();
          return -1;
        }

        // Calculate the velocity based on the change in position
        int deltaPosition = targetPosition - Pabs;
        std::cout << "Delta Position: " << deltaPosition << "\n";
        int setpointVelocity = deltaPosition / sample_time * 1000; // Assuming sample_time interval
        std::cout << "Setpoint Velocity: " << setpointVelocity << "\n";

        err = SdkSetJogMxn(g_GtwyHandle, CANid, setpointVelocity, &RxVelo);
        if (err != 0) {
          std::cout << "CANid:" << CANid << "  SdkSetJogMxn Fail!\n";
          logFile.close();
          return -1;
        }

        err = SdkSetBeginMxn(g_GtwyHandle, CANid);
        if (err != 0) {
          std::cout << "CANid:" << CANid << "  SdkSetBeginMxn Fail!\n";
          logFile.close();
          return -1;
        }

        // Log the setpoint and actual positions
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        logFile << elapsedTime << "," << Pabs << "," << setpointVelocity << "\n";
      }
    }

    // Close the log file
    logFile.close();

    // Turn off motor driver
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 0, &unRxData);
    // if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }
	}

  // Demo PTP sinusoid -----------------------------------------------------------------------------
  bool bTest_PTPMoveSin = false;

  if (bTest_PTPMoveSin)
  {
    std::cout << "PTP sine test\n";
    
    // Linear motion to 10 revs and back, sinusoidally increasing speed
    const int numEntries = 100;
    int positions[numEntries];
    for (int i = 0; i < numEntries / 2; ++i) {
      positions[i] = static_cast<int>((10 * 3200) * (2.0 * i / numEntries));
    }
    for (int i = numEntries / 2; i < numEntries; ++i) {
      positions[i] = static_cast<int>((10 * 3200) * (2.0 - 2.0 * i / numEntries));
    }
    int velocities[numEntries];
    for (int i = 0; i < numEntries; ++i) {
      velocities[i] = static_cast<int>(160000 * sin(M_PI * i / numEntries));
    }

    // Enable
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 1, &unRxData);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }
    
    // Set origin
    err = SdkSetOrigin(g_GtwyHandle, CANid);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetOrigin Fail!\n"; return -1; }

    // Cycle run relative displacement
    // Iterate through the array and update the SdkSetPtpMxnA setpoint every 25ms
    for (int i = 0; i < numEntries; ++i) {
      err = SdkSetPtpMxnA(g_GtwyHandle, CANid, velocities[i], positions[i], &RxVelo, &RxPr);
      if (err != 0) {
        std::cout << "CANid:" << CANid << "  SdkSetPtpMxnA Fail!\n";
        return -1;
      }

      err = SdkSetBeginMxn(g_GtwyHandle, CANid);
      if (err != 0) {
        std::cout << "CANid:" << CANid << "  SdkSetBeginMxn Fail!\n";
        return -1;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    // Turn off motor driver
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 0, &unRxData);
    if (err != 0) { std::cout << "CANid:" << CANid << "  SdkSetMotorOn Fail!\n"; return -1; }
  }


}