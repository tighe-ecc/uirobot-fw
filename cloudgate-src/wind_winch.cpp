// wind_winch.cpp
// This file initializes a gateway, finds all devices on that gateway, and iteratively winds 50ft of string onto the winch before stopping.
//
// Author: tighe
// Date: 2025-03-11

#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"
#include <thread>

#include <string>
#include <vector>
#include <chrono>

// Gateway handle
unint g_GtwyHandle;

// Function to initialize the gateway
bool InitializeGateway() {
  // Initialization code here
  // Example: g_GtwyHandle = InitializeYourGateway();
  return true; // Return true if initialization is successful
}

// Function to find all devices on the gateway
std::vector<int> FindDevices() {
  std::vector<int> devices;
  // Code to find devices and populate the vector
  // Example: devices.push_back(deviceID);
  return devices;
}

// Function to lower the winch on a device
void LowerWinch(int CANid) {
  ERRO err = 0;
  unint unRxData = 0;
	int RxVelo;
  int RxPr;

  // Code to wind string onto device
  std::cout << "Winding string on device " << CANid << std::endl;
  
  // Enable
  err = SdkSetMotorOn(g_GtwyHandle, CANid, 1, &unRxData);
  if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetMotorOn Fail!\n";}

  // Wind 50ft of string onto winch at 60Hz
  err = SdkSetPtpMxnR(g_GtwyHandle, CANid, 3200*5, 170000, &RxVelo, &RxPr);
  if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetPtpMxnR Fail!\n";}

  err = SdkSetBeginMxn(g_GtwyHandle, CANid);
  if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetBeginMxn Fail!\n";}
}

int main() {
  std::cout << "Launching wind_winch.cpp\n";

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
  ERRO err = 0;
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

  for (int i = MemberQuantity; i > 0; i--) {
    std::cout << "CAN id:" << (int)Member[i - 1].CANnid
      << "   Model:" << Member[i - 1].Model.ModelStr
      << "   Firmware:" << Member[i - 1].Model.FirmVer
      << std::endl;
  }

  char userResponse;
  std::cout << "Do you want to wind the winches on all devices? (y/n): ";
  std::cin >> userResponse;
  if (userResponse == 'y' || userResponse == 'Y') {
    for (int i = MemberQuantity; i > 0; i--) {
          int CANid = Member[i - 1].CANnid;
          // std::cout << "Lowering winch on device " << CANid << std::endl;
          std::cout << " " << std::endl;
          LowerWinch(CANid);
          std::cout << "Press Enter to continue to the next device..." << std::endl;
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cin.get();
      }
  } else {
      std::cout << "Skipping all devices." << std::endl;
  }
}