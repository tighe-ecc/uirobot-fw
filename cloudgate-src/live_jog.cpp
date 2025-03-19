// live_jog.cpp
// This script enables real time control of the winch, allowing the user to jog a winch up or down using keyboard inputs.
//
// Author: tighe
// Date: 2025-03-18

#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"
#include <thread>

#include <string>
#include <vector>
#include <chrono>
#include <conio.h> // For _kbhit() and _getch()


// Gateway handle
unint g_GtwyHandle;


// Function to jog the winch of an input CANid
void JogWinch(int CANid, int speed) {
  ERRO err = 0;
  unint unRxData = 0;
	int RxVelo;

  // Code to lower the winch on the device
  std::cout << "Jogging winch on device " << CANid << std::endl;

  // Set the jog mode -> speed -2 rps
  err = SdkSetJogMxn(g_GtwyHandle, CANid, speed);
  if (err != 0) { std::cout << "CANid:" << CANid << "SdkSetJogMxn Move Fail!\n";}

  err = SdkSetBeginMxn(g_GtwyHandle, CANid);
  if (err != 0) { std::cout << "CANid:" << CANid << "SdkSetBeginMxn Fail!\n";}
}


// Function to stop the winch of an input CANid
void StopWinch(int CANid) {
  ERRO err = 0;
  unint unRxData = 0;

  // Code to stop the winch on the device
  std::cout << "Stopping winch on device " << CANid << std::endl;
  
  // Set the jog mode -> speed 0
  err = SdkSetJogMxn(g_GtwyHandle, CANid, 0);
  if (err != 0) { std::cout << "CANid:" << CANid << "SdkSetJogMxn Stop Fail!\n";}

  err = SdkSetBeginMxn(g_GtwyHandle, CANid);
  if (err != 0) { std::cout << "CANid:" << CANid << "SdkSetBeginMxn Fail!\n";}
}


// Main function
int main() {
  std::cout << "Launching setup.cpp\n";

  int lower_speed = -1600;
  int raise_speed = 1600;

  // Set the current configuration type
  bool m_bIs232Gtwy =  true;
  bool m_bIsEthGtwy = false;
  GATEWAY_INFO_OBJ Gtwy;          
  if (m_bIs232Gtwy) {
    // Serial device config
    Gtwy.detail.type = UIGW3_GW2513;
    Gtwy.COMidx = 4;
    Gtwy.COMbaud = 57600;
  }
  else if (m_bIsEthGtwy) {
    // Ethernet device config
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
  ERRO err = 0;
  // Initialize the gateway
  err = SdkStartCanNet(&Gtwy, Member, 0);
  // Record the gateway handle
  g_GtwyHandle = Gtwy.GtwyHandle;				 
  if (err) {
    std::cout << "No gateway found, please check the gateway communication parameters!\n";
    return -1;
  }
  
  // Print out the member information
  int  MemberQuantity = 0;           // Number of members
  MemberQuantity = Gtwy.MemberQty;
  for (int i = MemberQuantity; i > 0; i--) {
    std::cout << "CAN id:" << (int)Member[i - 1].CANnid
      << "   Model:" << Member[i - 1].Model.ModelStr
      << "   Firmware:" << Member[i - 1].Model.FirmVer
      << std::endl;
  }

  // Loop to allow user to enter CANid and control the winch
  while (true) {
    int CANid;
    std::cout << "Enter CANid (or -1 to exit): ";
    std::cin >> CANid;

    if (CANid == -1) {
      std::cout << "Exiting program." << std::endl;
      break;
    }

    // Enable the motor with the entered CANid
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 1);
    if (err != 0) {
      std::cout << "CANid:" << CANid << " SdkSetMotorOn Fail!\n";
      continue;
    }

    std::cout << "Use the up/down arrow keys to raise/lower the winch, hit any key to stop. Press ESC to stop and enter a new CANid." << std::endl;

    while (true) {
      if (_kbhit()) {
        int key = _getch();

        if (key == 27) { // ESC key
          std::cout << "Disabling motor for CANid " << CANid << std::endl;
          StopWinch(CANid);
          if (err != 0) {
            std::cout << "CANid:" << CANid << " SdkSetMotorOn (Disable) Fail!\n";
          }
          break;
        } else if (key == 0 || key == 224) { // Arrow keys
          key = _getch(); // Get the actual arrow key code
          if (key == 72) { // Up arrow
            JogWinch(CANid, raise_speed);
          } else if (key == 80) { // Down arrow
            JogWinch(CANid, lower_speed);
          }
        } else {
          StopWinch(CANid);
        }
      }
    }
  }
}