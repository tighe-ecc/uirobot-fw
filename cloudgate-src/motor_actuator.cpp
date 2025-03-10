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

// Global variables
unint g_GtwyHandle;
static std::ofstream logFile;

// Function to configure the motor
void configureMotor() {
    std::cout << "Motor configuration is starting...\n";
    ERRO err = 0;
    unint unRxData = 0;

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
                    << "    Model:" << Member[i - 1].Model.ModelStr
                    << "    Firmware:" << Member[i - 1].Model.FirmVer
                    << std::endl;
    }

    // Demo actuator config
    bool bGetPara = true;

    if (bGetPara) {
        // Initial Config

        // Obtain whether to power on Enable 0 = disable, 1 = Enable
        err = SdkGetInitialConfig(g_GtwyHandle, CANid, 0, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetInitialConfig Fail!\n";
            return;
        } else {
            std::cout << "CANid:" << CANid << "    Power-on Enable = " << unRxData << "\n";
        }

        // Get positive turn setting, 0 = CW, 1= CCW
        err = SdkGetInitialConfig(g_GtwyHandle, CANid, 1, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetInitialConfig Fail!\n";
            return;
        } else {
            std::cout << "CANid:" << CANid << "    Positive turn setting = " << unRxData << "\n";
        }

        // Acceleration and deceleration mode 0 = value, 1 = time
        err = SdkSetInitialConfig(g_GtwyHandle, CANid, 4, 0, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkSetInitialConfig Fail!\n";
            return;
        } else {
            std::cout << "CANid:" << CANid << "    Acceleration and deceleration mode = " << unRxData << "\n";
        }

        // Set acceleration/deceleration rate to that of gravity
        err = SdkSetAcceleration(g_GtwyHandle, CANid, 130667, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetAcceleration Fail!\n";
            return;
        } else {
            std::cout << "CANid:" << CANid << "    Acceleration rate = " << unRxData << "\n";
        }
        err = SdkSetDeceleration(g_GtwyHandle, CANid, 130667, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetAcceleration Fail!\n";
            return;
        } else {
            std::cout << "CANid:" << CANid << "    Deceleration rate = " << unRxData << "\n";
        }

        // Information Enable

        // P1 Port change notification 0 = disable, 1 = Enable.
        err = SdkGetInformEnable(g_GtwyHandle, CANid, 0, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetInformEnable Fail!\n";
            return;
        }

        // PTP movement in place notification 0 = disable, 1 = Enable;
        err = SdkGetInformEnable(g_GtwyHandle, CANid, 8, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetInformEnable Fail!\n";
            return;
        }

        // Motor Config Information

        // Get subdivide[1.2.4.8.16]
        err = SdkGetMotorConfig(g_GtwyHandle, CANid, 0, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetMotorConfig Fail!\n";
            return;
        }

        // Get current [10...80]
        err = SdkGetMotorConfig(g_GtwyHandle, CANid, 1, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetMotorConfig Fail!\n";
            return;
        }

        // Get the standby current flow [0...100(%)]
        err = SdkGetMotorConfig(g_GtwyHandle, CANid, 2, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetMotorConfig Fail!\n";
            return;
        }

        // Obtain automatic power-on enable delay [0...60000(ms)]
        err = SdkGetMotorConfig(g_GtwyHandle, CANid, 3, &unRxData);
        if (err) {
            std::cout << "CANid:" << CANid << "    SdkGetMotorConfig Fail!\n";
            return;
        }

        // Enable
        err = SdkSetMotorOn(g_GtwyHandle, CANid, 1, &unRxData);
        if (err != 0) { std::cout << "CANid:" << CANid << "    SdkSetMotorOn Fail!\n"; return; }

        // Set origin
        err = SdkSetOrigin(g_GtwyHandle, CANid);
        if (err != 0) { std::cout << "CANid:" << CANid << "    SdkSetOrigin Fail!\n"; return; }
    }
}

// Function to set the motor to a specific position
void moveActuatorToPosition(int CANid, int position) {
    // std::cout << "Moving actuator to position: " << position << std::endl;

    // One-time initialization
    static bool firstRun = true;
    static ERRO err = 0;
    static unint unRxData = 0;
    static int RxPr;    // Position feedback
    static int RxVelo;    // Velocity feedback

    static MOTION_STATUS_OBJ MotionStatus;
    static int Velo;
    static int Prel;
    static int Pabs;

    static auto startTime = std::chrono::steady_clock::now();
    static auto lastTime = startTime;

    if (firstRun) {
        firstRun = false;

        err = 0;
        unRxData = 0;

        logFile.open("../utils/position_log.csv");
        logFile << "ClockTime,ActualPosition,SetpointVelocity\n";
    }

    // Get the current actuator state
    err = SdkGetMotionStatus(g_GtwyHandle, CANid, &MotionStatus, &Velo, &Prel, &Pabs);
    if (err != 0) {
        std::cout << "CANid:" << CANid << "    SdkGetMotionStatus Fail!\n";
        logFile.close();
        return;
    }

    // Record the time elapsed since the last iteration
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    lastTime = currentTime;

    // Calculate the velocity based on the change in position
    int deltaPosition = position - Pabs;
    int setpointVelocity = static_cast<int>(static_cast<float>(deltaPosition) / static_cast<float>(elapsedTime) * 1000.0f); // Assuming elapsedTime interval in ms
    if (setpointVelocity > 160000) {
        std::cout << "Setpoint velocity is too high, limiting to 160000\n";
        setpointVelocity = 160000;
    }
    std::cout << "Delta Position: " << deltaPosition << "    Setpoint Velocity: " << setpointVelocity << "    Elapsed Time: " << elapsedTime << " ms" << std::endl;

    err = SdkSetJogMxn(g_GtwyHandle, CANid, setpointVelocity, &RxVelo);
    if (err != 0) {
        std::cout << "CANid:" << CANid << "    SdkSetJogMxn Fail!\n";
        logFile.close();
        return;
    }

    err = SdkSetBeginMxn(g_GtwyHandle, CANid);
    if (err != 0) {
        std::cout << "CANid:" << CANid << "    SdkSetBeginMxn Fail!\n";
        logFile.close();
        return;
    }

    // Log the setpoint and actual positions
    logFile << std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count()/1000.0 << "," << Pabs << "," << setpointVelocity << "\n";

    // // Sleep for a short duration to prevent busy-waiting
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

void closeActuatorStream(int CANid) {
    std::cout << "Closing actuator stream..." << std::endl;
    ERRO err = 0;
    unint unRxData = 0;

    // Close the log file
    logFile.close();

    // Turn off motor driver
    err = SdkSetMotorOn(g_GtwyHandle, CANid, 0, &unRxData);
    if (err != 0) { std::cout << "CANid:" << CANid << "    SdkSetMotorOn Fail!\n"; return; }
}