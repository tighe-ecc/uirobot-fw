#include "motor_actuator.h"

// Global variables
unsigned int MyActuator::g_GtwyHandle;
int MyActuator::MemberQuantity = 0;
std::vector<MyActuator*> MyActuator::instances;

// Connect to the gateway and store the gateway handle
void MyActuator::connectGateway()
{
    std::cout << "Starting gateway connection...\n";
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
        if ((!Gtwy.tcp.IPaddr)) {std::cout << "Gateway parameter error!\n"; return;}
        Gtwy.tcp.IPport = 8888;
    }

    MEMBER_INFO_OBJ Member[100]; // Members (Max. 100 members)
    // Initialize the gateway
    err = SdkStartCanNet(&Gtwy, Member, 0);
    if (err) {
        std::cout << "No gateway found, please check the gateway communication parameters!\n";
        return;
    }

    // Record the gateway handle
    g_GtwyHandle = Gtwy.GtwyHandle;
    MemberQuantity = Gtwy.MemberQty;
}

// Configure all motors on the bus
void MyActuator::configureMotors()
{
    std::cout << "Configuring motors on gateway...\n";
    ERRO err = 0;
    unint unRxData = 0;

    MEMBER_INFO_OBJ Member[100]; // Members (Max. 100 members)
    // Get the list of members in the gateway
    err = SdkGetMemberList(g_GtwyHandle, Member);
    if (err) {std::cout << "SdkGetMemberList Fail!\n"; return;}

    // Number of members
    int CANid = 0;

    for (int i = MemberQuantity; i > 0; i--) {
        CANid = Member[i - 1].CANnid;
        std::cout << "CANid:" << (int)Member[i - 1].CANnid
                    << "    Model:" << Member[i - 1].Model.ModelStr
                    << "    Firmware:" << Member[i - 1].Model.FirmVer
                    << std::endl;

        bool bSetPara = true;

        if (bSetPara) {
            // Initial Config

            // Obtain whether to power on Enable 0 = disable, 1 = Enable
            err = SdkGetInitialConfig(g_GtwyHandle, CANid, 0, &unRxData);
            if (err) {
                std::cout << "CANid:" << CANid << "    SdkGetInitialConfig Fail!\n";
                return;
            } else {
                // std::cout << "CANid:" << CANid << "    Power-on Enable = " << unRxData << "\n";
            }

            // Get positive turn setting, 0 = CW, 1= CCW
            err = SdkGetInitialConfig(g_GtwyHandle, CANid, 1, &unRxData);
            if (err) {
                std::cout << "CANid:" << CANid << "    SdkGetInitialConfig Fail!\n";
                return;
            } else {
                // std::cout << "CANid:" << CANid << "    Positive turn setting = " << unRxData << "\n";
            }

            // Acceleration and deceleration mode 0 = value, 1 = time
            err = SdkSetInitialConfig(g_GtwyHandle, CANid, 4, 0, &unRxData);
            if (err) {
                std::cout << "CANid:" << CANid << "    SdkSetInitialConfig Fail!\n";
                return;
            } else {
                // std::cout << "CANid:" << CANid << "    Acceleration and deceleration mode = " << unRxData << "\n";
            }

            // Set acceleration/deceleration rate
            err = SdkSetAcceleration(g_GtwyHandle, CANid, 13066, &unRxData);
            if (err) {
                std::cout << "CANid:" << CANid << "    SdkSetAcceleration Fail!\n";
                return;
            } else {
                // std::cout << "CANid:" << CANid << "    Acceleration rate = " << unRxData << "\n";
            }
            err = SdkSetDeceleration(g_GtwyHandle, CANid, 130660, &unRxData);
            if (err) {
                std::cout << "CANid:" << CANid << "    SdkSetDeceleration Fail!\n";
                return;
            } else {
                // std::cout << "CANid:" << CANid << "    Deceleration rate = " << unRxData << "\n";
            }
            err = SdkSetStopDeceleration(g_GtwyHandle, CANid, 1306600, &unRxData);
            if (err) {
                std::cout << "CANid:" << CANid << "    SdkSetStopDeceleration Fail!\n";
                return;
            } else {
                // std::cout << "CANid:" << CANid << "    Stop Deceleration rate = " << unRxData << "\n";
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

            // Set initial speed to 0
            err = SdkSetJogMxn(g_GtwyHandle, CANid, 0);
            if (err != 0) { std::cout << "CANid:" << CANid << "SdkSetJogMxn Stop Fail!\n";}
        }
    }
}

// Set the group ID for a motor
void MyActuator::setGroupID(int groupID)
{
    ERRO err = 0;

    err = SdkSetProtocolPara(g_GtwyHandle, CANid, 8, groupID);
    if (err) {
        std::cout << "  CANid:" << CANid << "    SdkSetProtocolPara Fail!\n";
        return;
    } else {
        std::cout << "  CANid:" << CANid << "    GroupID: " << groupID << "\n";
    }
}

// Set the target position for the motor
void MyActuator::setMotorPos(int position)
{
    // std::cout << "Setting new motor target position for CANid " << CANid << ": " << position << std::endl;

    // Record the time elapsed since the last iteration
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - md.lastTime).count();
    // std::cout << "Elapsed time: " << elapsedTime << " ms, CANid: " << CANid << std::endl;
    md.lastTime = currentTime;

    // Create a new log file if this is the first run
    if (md.firstRun) {
        md.firstRun = false;

        md.logFile.open("../utils/position_log_" + std::to_string(CANid) + ".csv");
        md.logFile << "ClockTime,ActualPosition,SetpointVelocity\n";
    }

    // Get the current actuator state
    md.err = SdkGetMotionStatus(g_GtwyHandle, CANid, &md.MotionStatus, &md.Velo, &md.Prel, &md.Pabs);
    if (md.err != 0) {
        std::cout << "CANid:" << CANid << "    SdkGetMotionStatus Fail!\n";
        md.logFile.close();
        return;
    }
    
    // Calculate the error between the desired and actual position
    float positionError = position - md.Pabs;
    int setpointVelocity = 0;
    int velLimit = 3200;  // counts/s
    int updateRate = 525;  // ms
    
    // Set the control mode
    bool bJog = true;
    bool bPtp = false;

    // Jog implementation -----------------------------------------------------

    if (bJog) {
        // PID controller parameters
        float Kp = 3.0f;  // Proportional gain
        float Ki = 0.0f;  // Integral gain
        float Kd = 0.0f;  // Derivative gain

        // Calculate the integral of the position error
        // static float integralError = 0;
        md.integralError += positionError * elapsedTime / 1000.0;
        if (Ki * md.integralError > setpointVelocity) {
            std::cout << "CANid:" << CANid << "Integral error is too high, limiting to 160000\n";
            md.integralError = setpointVelocity / Ki;
        }

        // Calculate the derivative of the position error
        // static float lastPositionError = 0;
        float derivativeError = (positionError - md.lastPositionError) / elapsedTime * 1000.0;
        md.lastPositionError = positionError;

        // Calculate the control output
        setpointVelocity = static_cast<int>(Kp * positionError + Ki * md.integralError + Kd * derivativeError);
        if (std::abs(setpointVelocity) > velLimit) {
            std::cout << "CANid:" << CANid << "Setpoint velocity is too high, limiting to 160000\n";
            setpointVelocity = (setpointVelocity > 0) ? velLimit : -velLimit;
        }

        // Apply acceleration limit
        static const int maxAcceleration = 131; // Maximum change in velocity per millisecond, ~9.8m/s^2
        int velocityChange = setpointVelocity - md.lastVelocity;
        if (std::abs(velocityChange) > maxAcceleration * updateRate) {
            std::cout << "CANid:" << CANid << "Acceleration limit exceeded, limiting velocity change\n";
            setpointVelocity = md.lastVelocity + ((velocityChange > 0) ? maxAcceleration * updateRate : -maxAcceleration * updateRate);
        }
        md.lastVelocity = setpointVelocity;

        // std::cout << "PosErr: " << positionError 
        //           << " IntErr: " << md.integralError
        //           << " DerErr: " << derivativeError 
        //           << " SetVel: " << setpointVelocity << std::endl;
        
        // Update the target motor velocity
        md.err = SdkSetJogMxn(g_GtwyHandle, CANid, setpointVelocity);
        if (md.err != 0) {
                std::cout << "CANid:" << CANid << "    SdkSetJogMxn Fail!\n";
                md.logFile.close();
                return;
            }
    }

    if (bPtp) {
        //  Update the target motor position and velocity
        setpointVelocity = static_cast<int>(positionError / 50.0 * 1000.0);
        if (std::abs(setpointVelocity) > velLimit) {
            // std::cout << "Setpoint velocity is too high, limiting to 160000\n";
            setpointVelocity = (setpointVelocity > 0) ? velLimit : -velLimit;
        }
        md.err = SdkSetPtpMxnA(g_GtwyHandle, CANid, setpointVelocity, position);
        if (md.err != 0) {
            std::cout << "CANid:" << CANid << "    SdkSetPtpMxnA Fail!\n";
            md.logFile.close();
            return;}
    }
     
    // Log the setpoint and actual positions
    //TODO: add actual velocity
    md.logFile << std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - md.startTime).count()/1000.0 << "," << md.Pabs << "," << setpointVelocity << "\n";

    // // Wait until time has passed since md.lastTime to proceed
    // while (std::chrono::steady_clock::now() < md.lastTime + std::chrono::milliseconds(updateRate)) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // }
}

// Return all motors to zero position
void MyActuator::returnToZero()
{
    // std::cout << "Returning all motors to zero position...\n";
    ERRO err = 0;
    unint unRxData = 0;

    //  Update the target motor position and velocity
    err = SdkSetPtpMxnA(g_GtwyHandle, CANid, 6400, 0);
    if (err != 0) {
        std::cout << "CANid:" << CANid << "    SdkSetPtpMxnA Fail!\n";
        return;
    }
}

// Start all motors on bus tracking to latest target motion
void MyActuator::startMotion() {
    ERRO err = 0;
    err = SdkSetBeginMxn(g_GtwyHandle, 0);
    if (err != 0) {
        std::cout << "CANid:" << 0 << "    SdkSetBeginMxn Fail!\n";
        return;
    }
}

// Close the actuator data stream and power off the motor drivers
void MyActuator::disableMotors()
{
    std::cout << "Closing actuator stream..." << std::endl;
    ERRO err = 0;
    
    // Turn off all motor drivers on bus
    err = SdkSetMotorOn(g_GtwyHandle, 0, 0);
    if (err != 0) { std::cout << "CANid:" << 0 << "    SdkSetMotorOn Fail!\n"; return; }
}

// Close all open log files
void MyActuator::closeAllLogFiles()
{
    std::cout << "Closing all log files..." << std::endl;

    for (MyActuator* instance : instances) {
        if (instance->md.logFile.is_open()) {
            instance->md.logFile.close();
        }
    }
}
