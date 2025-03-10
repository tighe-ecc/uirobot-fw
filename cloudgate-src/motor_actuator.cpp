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

void configureMotor() {
  ERRO err = 0;

  // Set the current configuration type
  bool m_bIs232Gtwy = true;
  bool m_bIsEthGtwy = false;
  GATEWAY_INFO_OBJ Gtwy;

  if (m_bIs232Gtwy) {
    // Serial device config
    Gtwy.detail.type = UIGW3_GW2513;
    Gtwy.COMidx = 4;
    Gtwy.COMbaud = 57600;
  } else if (m_bIsEthGtwy) {
    // Ethernet device config
    Gtwy.detail.type = UIGW3_ETHCAN;
    Gtwy.detail.IP[0] = 254;
    Gtwy.detail.IP[1] = 1;
    Gtwy.detail.IP[2] = 168;
    Gtwy.detail.IP[3] = 192;
    if ((!Gtwy.tcp.IPaddr)) {
      std::cout << "Gateway parameter error!\n";
      return;
    }
    Gtwy.tcp.IPport = 8888;
  }

  MEMBER_INFO_OBJ Member[100]; // Members (Max. 100 members)
  // Initialize the gateway
  err = SdkStartCanNet(&Gtwy, Member, 0);
  // Record the gateway handle
  g_GtwyHandle = Gtwy.GtwyHandle;

  if (err) {
    std::cout << "No gateway found, please check the gateway communication parameters!\n";
    return;
  }

  // Number of members
  int MemberQuantity = Gtwy.MemberQty;
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

  // Demo actuator config
  bool bGetPara = true;

  if (bGetPara) {
    // Initial Config

    // Obtain whether to power on Enable 0 = disable, 1 = Enable
    err = SdkGetInitialConfig(g_GtwyHandle, CANid, 0, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetInitialConfig Fail!\n";
      return;
    } else {
      std::cout << "CANid:" << CANid << "  Power-on Enable = " << unRxData << "\n";
    }

    // Get positive turn setting, 0 = CW, 1= CCW
    err = SdkGetInitialConfig(g_GtwyHandle, CANid, 1, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetInitialConfig Fail!\n";
      return;
    } else {
      std::cout << "CANid:" << CANid << "  Positive turn setting = " << unRxData << "\n";
    }

    // Acceleration and deceleration mode 0 = value, 1 = time
    err = SdkSetInitialConfig(g_GtwyHandle, CANid, 4, 0, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkSetInitialConfig Fail!\n";
      return;
    } else {
      std::cout << "CANid:" << CANid << "  Acceleration and deceleration mode = " << unRxData << "\n";
    }

    // Set acceleration/deceleration rate to that of gravity
    err = SdkSetAcceleration(g_GtwyHandle, CANid, 130667, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetAcceleration Fail!\n";
      return;
    } else {
      std::cout << "CANid:" << CANid << "  Acceleration rate = " << unRxData << "\n";
    }
    err = SdkSetDeceleration(g_GtwyHandle, CANid, 130667, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetAcceleration Fail!\n";
      return;
    } else {
      std::cout << "CANid:" << CANid << "  Deceleration rate = " << unRxData << "\n";
    }

    // Information Enable

    // P1 Port change notification 0 = disable, 1 = Enable.
    err = SdkGetInformEnable(g_GtwyHandle, CANid, 0, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetInformEnable Fail!\n";
      return;
    }

    // PTP movement in place notification 0 = disable, 1 = Enable;
    err = SdkGetInformEnable(g_GtwyHandle, CANid, 8, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetInformEnable Fail!\n";
      return;
    }

    // Motor Config Information

    // Get subdivide[1.2.4.8.16]
    err = SdkGetMotorConfig(g_GtwyHandle, CANid, 0, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetMotorConfig Fail!\n";
      return;
    }

    // Get current [10...80]
    err = SdkGetMotorConfig(g_GtwyHandle, CANid, 1, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetMotorConfig Fail!\n";
      return;
    }

    // Get the standby current flow [0...100(%)]
    err = SdkGetMotorConfig(g_GtwyHandle, CANid, 2, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetMotorConfig Fail!\n";
      return;
    }

    // Obtain automatic power-on enable delay [0...60000(ms)]
    err = SdkGetMotorConfig(g_GtwyHandle, CANid, 3, &unRxData);
    if (err) {
      std::cout << "CANid:" << CANid << "  SdkGetMotorConfig Fail!\n";
      return;
    }
  }
}