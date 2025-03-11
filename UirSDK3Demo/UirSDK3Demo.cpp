// UirSDK3Demo.cpp: This file contains the "main" function. Program execution begins and ends here.
//

#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"
#include <thread>

#define UIM_BIT_MASK(bit)          (1 << bit)
#define UIM_BIT_SET(var, bit)      (var |= (UIM_BIT_MASK(bit)))
#define UIM_BIT_CLR(var, bit)      (var &= (~UIM_BIT_MASK(bit)))

// Gateway handle
unint	g_GtwyHandle;

// Define flag bits
bool g_bSTPFlag_X = false;
bool g_bSTPFlag_Y = false;

// Define S1 trigger flag bit
bool g_bS1Flag_X = false;
bool g_bS1Flag_Y = false;

// Define X,Y CAN-ID (Device's ID)
int CANid_X = 5;
int CANid_Y = 6;

// Define normal motion speed
//RPM to pulse
int g_VELOCITY = 3200 * (300 / 60);

// Return to origin speed
int g_VELOCITY_Org = 10000;
// Normal speed of motion
int g_VELOCITY_PR = 10000;

// Define position 1
int g_POSITION1_X = 100000;
int g_POSITION1_Y = 60000;
// Define position 2
int g_POSITION2_X = 10000;
int g_POSITION2_Y = 10000;



//RTCN massage process
void CRevRtcnNotify(UI_MSG_OBJ* uiMsgObj)
{
	unsigned char rtcn_code = uiMsgObj->data[0];

	// System warning
	if (rtcn_code == 0)
	{// Error code
		switch (uiMsgObj->data[1])
		{
		case 0:
			// The controller is locked and needs to be powered off and restarted to restore ";
			break;
		case  ERR_SYS_GWSD:
			// The whole group is locked, and needs to be restarted to restore ";
			break;
		case ERR_SYS_ILCK:
			// System emergency stop and lock ";
			break;
		case ERR_SYS_STOP:
			// Motor in the process of emergency stop ";
			break;
		case ERR_SYS_OVHT:
			// System overheating ";
			break;
		case ERR_SYS_POVL:
			// Short circuit/overload detected in external power supply ";
			break;
		case ERR_ELOC_CODE:
			// System emergency stop and lock ";
			break;
		case ERR_URT_LCK:
			// An error occurred in receiving the packet. No subsequent packet will be accepted.
			break;

		case ERR_URT_MIX:
			// Message serial number discontinuous ");
			break;
		case ERR_URT_TMO:
			// Serial port message receiving timeout ");
			break;
		case ERR_URT_CRC:
			// Serial message CRC error ");
			break;
		case ERR_URT_PSI:
			// packet PS instruction subscript error ");
			break;
		case ERR_RRB_OVF:
			// Serial RRB overflow ");
			break;
		case ERR_RRB_LVW:
			// Serial RRB Water level warning ");
			break;
		case ERR_CSB_FUL:
			// Serial port CSB full warning ");
			break;
		case ERR_CTX_TMOT:
			//CAN message timeout ");
			break;
		case ERR_MXN_ACC:
			// Acceleration exceeds the upper limit ");
			break;
		case ERR_MXN_EBRK:
			// Brake locked, unable to BG");
			break;
		case ERR_MXN_NBRK:
			// Brake is not locked, can not be offline ");
			break;
		case ERR_MXN_MOFF:
			// Offline state, unable to BG");
			break;
		case ERR_MXN_SPHL:
			// Speed exceeds the upper limit ");
			break;
		case ERR_MXN_PALL:
			// Working position exceeds the lower limit ";
			break;
		case ERR_MXN_PAHL:
			// Working position exceeds the upper limit ");
			break;
		case ERR_MXN_SPEL:
			// The speed error is out of limit (reverse drag may have occurred) ");
			break;
		case ERR_MXN_PQER:
			// The displacement error is out of limit (possible gridlock) ");
			break;
		case ERR_MXN_ENCS:
			// encoder communication error ");
			break;
		case ERR_MXN_ENCB:
			// Absolute encoder battery low ")
			break;
		case ERR_INS_SYNT:
			// instruction syntax error ");
			break;
		case ERR_INS_NUMB:
			// Instruction data error ");
			break;
		case ERR_INS_IDXR:
			// Instruction index (subscript) error ");
			break;
		case ERR_SYS_STTM:
			// When the motor is running, the system time cannot be changed ");
			break;
		case ERR_MXN_DCSD:
			// SCram deceleration (SD) less than general deceleration (DC) ");
			break;
		case ERR_MXN_RUNG:
			//Q motor rotation, can not be changed ");
			break;
		case ERR_MXN_STOG:
			// When the motor is rotating, the absolute encoder origin cannot be set ");
			break;
		case ERR_PVT_RUNG:
			// When the motor is running, the PV starting point cannot be set ");
			break;
		case ERR_PVT_WPOV:
			//PVT index number greater than water level ");
			break;
		case ERR_PVT_IOFN:
			//QA Mask does not meet the I/O port function requirements ");
			break;
		case ERR_PVB_OVFL:
			//PVT cache overflow ");
			break;
		case ERR_IOC_ADIO:
			//I/O port function configuration is incorrect ");
			break;
		default:
			break;
		}
	}

	// Port level change correlation (RTCN_CODE = 1... 32)
	else if ((uiMsgObj->cw != 0x0F) && (rtcn_code >= 1) && (rtcn_code <= 32))
	{

		switch (rtcn_code)
		{
		case RTCN_DIO_P1L:
			cout << "ID:" << (int)uiMsgObj->id << " RTCN_DIO_P1L\n";
			if (uiMsgObj->id == CANid_X)
			{
				g_bS1Flag_X = true;
			}
			else if (uiMsgObj->id == CANid_Y)
			{
				g_bS1Flag_Y = true;
			}
			break;
		case RTCN_DIO_P1H:
			cout << "ID:" << (int)uiMsgObj->id << " RTCN_DIO_P1H\n";
	
			break;
		}



	}
	// Other real-time messages
	else
	{
		switch (rtcn_code)
		{
		case RTCN_MXN_STP:
			cout << "ID:"<< (int)uiMsgObj->id<<" RTCN_MXN_STP\n";
			if (uiMsgObj->id == CANid_X)
			{
				g_bSTPFlag_X = true;
			}
			else if (uiMsgObj->id == CANid_Y)
			{
				g_bSTPFlag_Y = true;
			}
			break;
		case RTCN_MXN_STL:
			break;
		default:
			cout << "ID:" << (int)uiMsgObj->id <<"Code"<<rtcn_code << "\n";
			break;

		}
	}

}
//RTCN thread
void RTCNTaskThread()
{
	while (1)
	{
		UI_MSG_OBJ uimsg = { 0 };
		// Read real-time messages
		ERRO retv = SdkGetRTCN(g_GtwyHandle, &uimsg);
		if (retv == 1)
		{
			CRevRtcnNotify(&uimsg);
		}

		PauseMS(5);
	}
}


// Wait flag bit
bool WaitFlag(bool* pFlag, unsigned int nWaitTimsMS = 15000)
{
	unsigned int nCurrWaitTime = 0;
	while (nCurrWaitTime < nWaitTimsMS)
	{
		if (*pFlag)
			return true;
		nCurrWaitTime += 100;
		PauseMS(100);
	}
	return false;
}
// Wait for two flag bits
bool WaitTwowFlag(bool* pFlag1, bool* pFlag2, unsigned int nWaitTimsMS = 15000)
{
	unsigned int nCurrWaitTime = 0;
	while (nCurrWaitTime < nWaitTimsMS)
	{
		if (*pFlag1 && *pFlag2)
			return true;
		nCurrWaitTime += 100;
		PauseMS(100);
	}
	return false;
}


bool XYMove(int posX, int posY)
{
	ERRO err = 0;

	int RxVelo = 0;
	int RxPa = 0;
	unint RxData = 0;

	g_bSTPFlag_X = false;
	g_bSTPFlag_Y = false;

	//set
	err = SdkSetPtpMxnA(g_GtwyHandle, CANid_X, g_VELOCITY, posX, &RxVelo, &RxPa);
	if (err)
	{
		std::cout << "CAN-ID:" << CANid_X << "SdkSetPtpMxnA Fail!\n";
		return false;
	}

	err = SdkSetPtpMxnA(g_GtwyHandle, CANid_Y, g_VELOCITY, posY, &RxVelo, &RxPa);
	if (err)
	{
		std::cout << "CAN-ID:" << CANid_Y << "SdkSetPtpMxnA Fail!\n";
		return false;
	}

	//begin 
	err = SdkSetBeginMxn(g_GtwyHandle, CANid_X, &RxData);
	if (err)
	{
		std::cout << "CAN-ID:" << CANid_X << "SdkSetPtpMxnA Fail!\n";
		return false;
	}
		
	err = SdkSetBeginMxn(g_GtwyHandle, CANid_Y, &RxData);
	if (err)
	{
		std::cout << "CAN-ID:" << CANid_Y << "SdkSetPtpMxnA Fail!\n";
		return false;
	}


	// Wait for position


	if (!WaitTwowFlag(&g_bSTPFlag_X, &g_bSTPFlag_Y))
	{
		std::cout << "XYMove:" << "等待到位 Fail!\n";
		return false;
	}

	return true;
}

// Execute the return to the origin logic based on the site ID
bool GotoOrg(int CANid)
{
	ERRO err = 0;
	bool* pSTPFlag = NULL;
	bool* pS1Flag = NULL;

	//获取标志位指针
	if (CANid == CANid)
	{
		pSTPFlag = &g_bSTPFlag_X;
		pS1Flag = &g_bS1Flag_X;
	}
	else
	{
		pSTPFlag = &g_bSTPFlag_Y;
		pS1Flag = &g_bS1Flag_Y;
	}

	// Get the current sensor position
	UIM_DIO_OBJ uim_dio_obj = { 0 };
	err = SdkGetDIOport(g_GtwyHandle, CANid, &uim_dio_obj);
	if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkGetDIOport Fail!\n"; return false; }

	// Check whether S1 is high // If yes, go to the origin
	int RxVelo = 0;
	int RxPr = 0;
	unint RxData = 0;
	if (uim_dio_obj.bits.bIN00 == 0)
	{
		*pSTPFlag = false;
		// Velocity 3000, relative displacement 10000
		err = SdkSetPtpMxnR(g_GtwyHandle, CANid, g_VELOCITY_Org, g_VELOCITY_PR, &RxVelo, &RxPr);
		if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetPtpMxnR Fail!\n"; return false; }


		//begin
		err = SdkSetBeginMxn(g_GtwyHandle, CANid);
		if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetBeginMxn Fail!\n"; return false; }

		// Wait for position
		if (!WaitFlag(pSTPFlag))
		{
			std::cout << "CAN-ID:" << CANid << "WaitFlag Fail!\n";
			return false;
		}

	}

	// Turn on the sensor trigger switch
	//Action0 is S1
	INPUT_ACT_OBJ InputAct;
	// The falling edge action is ILG_SORG_JVSD_IDX origin clear + emergency stop
	InputAct.Action0 = ILC_DST_IDX;
	InputAct.bNoTrig0 = 1;
	// Set the rising and falling edge action to blank
	InputAct.Action1= ILC_NOP_IDX;
	InputAct.bNoTrig1 = 1;

	INPUT_ACT_OBJ RxActData;

	err = SdkSetInputLogic(g_GtwyHandle, CANid, 0, InputAct, &RxActData);
	if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetInputLogic Fail!\n"; return false; }

	err = SdkGetInputLogic(g_GtwyHandle, CANid, 0, &RxActData);
	if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkGetInputLogic Fail!\n"; return false; }

	// Low speed return to the origin
	*pS1Flag = false;
	err = SdkSetJogMxn(g_GtwyHandle, CANid, -g_VELOCITY_Org, &RxVelo);
	if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetJogMxn Fail!\n"; return false; }


	err = SdkSetBeginMxn(g_GtwyHandle, CANid);
	if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetBeginMxn Fail!\n"; return false; }
		
	// Wait for the origin signal
	if (!WaitFlag(pS1Flag))
	{
		std::cout << "CAN-ID:" << CANid << "Wait WaitFlag S1 Fail!\n";
		return false;
	}



	// Disable the trigger action
	// Set the falling edge action to blank
	InputAct.Action0 = ILC_NOP_IDX;
	InputAct.bNoTrig0 = 1;
	// Set the rising and falling edge action to blank
	InputAct.Action1 = ILC_NOP_IDX;
	InputAct.bNoTrig1 = 1;
	err = SdkSetInputLogic(g_GtwyHandle, CANid, 0, InputAct, &RxActData);
	if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkSetInputLogic Fail!\n"; return false; }

	err = SdkGetInputLogic(g_GtwyHandle, CANid, 0, &RxActData);
	if (err != 0) { std::cout << "CAN-ID:" << CANid << "SdkGetInputLogic Fail!\n"; return false; }

	return true;
}

int main()
{
    std::cout << "Program initiation!\n";

	// Set the current configuration type
	bool m_bIs232Gtwy =  true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;          
	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 4;
		Gtwy.COMbaud = 57600;

	}
	else if (m_bIsEthGtwy)
	{
		//Ethernet device config
		Gtwy.detail.type = UIGW3_ETHCAN;
		Gtwy.detail.IP[0] = 254;
		Gtwy.detail.IP[1] = 1;
		Gtwy.detail.IP[2] = 168;
		Gtwy.detail.IP[3] = 192;
		if ((!Gtwy.tcp.IPaddr))
		{
			std::cout << "网关参数错误!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);

	if (err)
	{
		std::cout << "No gateway found, please check the gateway communication parameters!\n";
		return -1;
	}
	// Number of members
	int  MemberQuantity = 0;           // Number of members
	MemberQuantity = Gtwy.MemberQty;

	for (int i = MemberQuantity; i > 0; i--)
	{
		std::cout << "CAN id:" << (int)Member[i - 1].CANnid
			<< " Model:" << Member[i - 1].Model.ModelStr
			<< " Firmware:" << Member[i - 1].Model.FirmVer
			<< std::endl;
	}


	g_GtwyHandle = Gtwy.GtwyHandle;				 // Record the gateway handle

	// Open the real-time message thread, which is used to get the position message, sensor message, etc.
	thread rtcnThread(RTCNTaskThread);
	rtcnThread.detach();

	//uint CANid_X = Member[0].CANnid;

	unint unRxData = 0;
	int RxPr;
	int RxVelo;

	bool bGetPara = false;

	if (bGetPara)
	{
		//	Intial Config
		
		// Obtain whether to power on Enable 0 = disable, 1 = Enable.
		err = SdkGetInitialConfig(g_GtwyHandle, CANid_X, 0, &unRxData);
		if (err) { std::cout << "CAN-ID:" << CANid_X << "SdkGetInitialConfig Fail!\n"; return -1; }

		// Acceleration and deceleration mode 0 = value, 1 = time;
		err = SdkGetInitialConfig(g_GtwyHandle, CANid_X, 4, &unRxData);
		if (err) { std::cout << "CAN-ID:" << CANid_X << "SdkGetInitialConfig Fail!\n"; return -1; }

		// Information Enable

		//P1 Port change notification 0 = disable, 1 = Enable.
		err = SdkGetInformEnable(g_GtwyHandle, CANid_X, 0, &unRxData);
		if (err) { std::cout << "CAN-ID:" << CANid_X << "SdkGetInformEnable Fail!\n"; return -1; }

		//PTP movement in place notification 0 = disable, 1 = Enable;
		err = SdkGetInformEnable(g_GtwyHandle, CANid_X, 8, &unRxData);
		if (err) { std::cout << "CAN-ID:" << CANid_X << "SdkGetInformEnable Fail!\n"; return -1; }

		//	Motor Config Information
		
		//Get   subdivide[1.2.4.8.16]
		err = SdkGetMotorConfig(g_GtwyHandle, CANid_X, 0, &unRxData);
		if (err) { std::cout << "CAN-ID:" << CANid_X << "SdkGetMotorConfig Fail!\n"; return -1; }

		// Get current [10...80]
		err = SdkGetMotorConfig(g_GtwyHandle, CANid_X, 1, &unRxData);
		if (err) { std::cout << "CAN-ID:" << CANid_X << "SdkGetMotorConfig Fail!\n"; return -1; }


		// Get the standby current flow [0...100(%)]
		err = SdkGetMotorConfig(g_GtwyHandle, CANid_X, 2, &unRxData);
		if (err) { std::cout << "CAN-ID:" << CANid_X << "SdkGetMotorConfig Fail!\n"; return -1; }

		// Obtain automatic power-on enable delay [0...60000(ms)]
		err = SdkGetMotorConfig(g_GtwyHandle, CANid_X, 3, &unRxData);
		if (err) { std::cout << "CAN-ID:" << CANid_X << "SdkGetMotorConfig Fail!\n"; return -1; }
	}



	// Whether to perform JOG speed mode test
	bool bTest_Jog_Move = false;


	if (bTest_Jog_Move)
	{
		// Enable
		err = SdkSetMotorOn(g_GtwyHandle, CANid_X, 1, &unRxData);
		if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetMotorOn Fail!\n"; return -1; }


		// Set the jog mode -> speed 1000
		err = SdkSetJogMxn(g_GtwyHandle, CANid_X, 1000,  &RxVelo);
		if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetJogMxn Fail!\n"; return -1; }

		err = SdkSetBeginMxn(g_GtwyHandle, CANid_X);
		if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetBeginMxn Fail!\n"; return -1; }

		Sleep(2000);

		// Set the jog mode -> speed 0
		err = SdkSetJogMxn(g_GtwyHandle, CANid_X, 0, &RxVelo);
		if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetJogMxn Fail!\n"; return -1; }

		err = SdkSetBeginMxn(g_GtwyHandle, CANid_X);
		if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetBeginMxn Fail!\n"; return -1; }

	}


	// Whether to perform the PTP mode test
	bool bTest_PTPMove = false;

	if (bTest_PTPMove)
	{

		Sleep(1000);


		//SetOrigin
		err = SdkSetOrigin(g_GtwyHandle, CANid_X);
		if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetOrigin Fail!\n"; return -1; }

		Sleep(1000);


		uint unTestCount = 0;
		uint unTimeOutCount = 0;
		// Cycle run relative displacement
		//while (true)
		{

			g_bSTPFlag_X  = false;
			// Cycle run relative displacement
			err = SdkSetPtpMxnA(g_GtwyHandle, CANid_X, 100000, 100000, &RxVelo, &RxPr);
			if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetPtpMxnA Fail!\n"; return -1; }


			err = SdkSetBeginMxn(g_GtwyHandle, CANid_X);
			if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetBeginMxn Fail!\n"; return -1; }


			std::cout << "CAN-ID:" << CANid_X << "SdkSetPtpMxnA 100000!\n";

			// Wait flag bit
			if (WaitFlag(&g_bSTPFlag_X))
			{
				std::cout << "CAN-ID:" << CANid_X << "STPIE!\n";

			}
			else
			{
				unTimeOutCount++;
				std::cout << "CAN-ID:" << CANid_X << "WaitFlag TimeOut!\n";
			}



			g_bSTPFlag_X = false;
			// Set relative displacement and speed
			err = SdkSetPtpMxnA(g_GtwyHandle, CANid_X, 100000, 0,  &RxVelo, &RxPr);
			if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetPtpMxnA Fail!\n"; return -1; }


			err = SdkSetBeginMxn(g_GtwyHandle, CANid_X);
			if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetBeginMxn Fail!\n"; return -1; }

			std::cout << "CAN-ID:" << CANid_X << "SdkSetPtpMxnA 0!\n";

			// Wait flag bit
			if (WaitFlag(&g_bSTPFlag_X))
			{
				std::cout << "CAN-ID:" << CANid_X << "STPIE !\n";
			}
			else
			{
				unTimeOutCount++;
				std::cout << "CAN-ID:" << CANid_X << "WaitFlag TimeOut!\n";
			}

			unTestCount++;

			std::cout << "TestCount:" << unTestCount << " TimeOutCount:"<< unTimeOutCount<<"\n";

		}
	}


	if (bTest_PTPMove)
	{
		// Cycle run relative displacement
		//while (true)
		{

			g_bSTPFlag_X = false;
			// Set relative displacement and speed
			err = SdkSetPtpMxnR(g_GtwyHandle, CANid_X, 1000, 1000, &RxVelo, &RxPr);
			if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetPtpMxnR Fail!\n"; return -1; }


			err = SdkSetBeginMxn(g_GtwyHandle, CANid_X);
			if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetBeginMxn Fail!\n"; return -1; }

			std::cout << "CAN-ID:" << CANid_X << "SdkSetBeginMxn!\n";

			// Wait flag bit
			if (WaitFlag(&g_bSTPFlag_X))
			{
				std::cout << "CAN-ID:" << CANid_X << "STPIE!\n";
			}

		}
	}

	bool BTestXYMove = true;
	if (BTestXYMove)
	{


		//X Enable
		err = SdkSetMotorOn(g_GtwyHandle, CANid_X, 1, &unRxData);
		if (err != 0) { std::cout << "CAN-ID:" << CANid_X << "SdkSetMotorOn Fail!\n"; return -1; }


		//Y Enable
		err = SdkSetMotorOn(g_GtwyHandle, CANid_Y, 1, &unRxData);
		if (err != 0) { std::cout << "CAN-ID:" << CANid_Y << "SdkSetMotorOn Fail!\n"; return -1; }



		std::cout << "Initialize execution back to origin\n";
		//X Executes the command back to the origin
		if (!GotoOrg(CANid_X))
			return -1;
		//Y Executes the command back to the origin
		if (!GotoOrg(CANid_Y))
			return -1;

		int nCount = 0;

		//XY cycle motion test
		while (true)
		{
			if (!XYMove(g_POSITION1_X, g_POSITION1_Y))
				return -1;

			PauseMS(1000);

			if (!XYMove(g_POSITION2_X, g_POSITION2_Y))
				return -1;

			PauseMS(1000);

			nCount++;
			// Loop 10 times and then perform one return to the origin
			if (nCount == 10)
			{
				std::cout << "Execute homing\n";

				if (!GotoOrg(CANid_X))
					return -1;

				if (!GotoOrg(CANid_Y))
					return -1;

			}

			std::cout << "Number of tests:" << nCount << "\n";
		}

	}


	while (1)
	{
		PauseMS(100);
	}

	return 0;
}
