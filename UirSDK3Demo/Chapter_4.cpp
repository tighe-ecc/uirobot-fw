#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"

//chapter 4.1 Get serial number
int Get_Serial_Number()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	uint SerialNumber;


	//Get Device¡¯s SerialNumber
	err = SdkGetSerial(g_GtwyHandle, NodeID, &SerialNumber);
	if (err)
	{
		std::cout << "Modify Device¡¯s CAN bit rate Failed!\n";
		return false;
	}

	std::cout << "S/N:" << SerialNumber << std::endl;

	return true;
}

//chapter 4.2 Get device model
int Get_Device_Model()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	MODEL_INFO_OBJ modelinfo;


	//Get Device¡¯s SerialNumber
	err = SdkGetModel(g_GtwyHandle, NodeID, &modelinfo);
	if (err)
	{
		std::cout << "Modify Device¡¯s CAN bit rate Failed!\n";
		return false;
	}

	std::cout	<< "Firmware version:" << modelinfo.FirmVer 
				<< "PCB version:" << modelinfo.PCBVer
				<< "Model:" << modelinfo.ModelStr
				<< std::endl;

	return true;
}

//chapter 4.3 Get Error Report
int Get_Error_Report()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	ERR_REPORT_OBJ ErrReport;


	//Get Device¡¯s Error Report
	err = SdkGetErrReport(g_GtwyHandle, NodeID, 10,&ErrReport);
	if (err)
	{
		std::cout << "Get Device¡¯s Error Repor Failed!\n";
		return false;
	}

	std::cout	<< "index:" << ErrReport.index
				<< "ecode:" << ErrReport.ecode
				<< "errcw:" << ErrReport.errcw
				<< "errix:" << ErrReport.errix
				<< std::endl;

	return true;
}

//chapter 4.4 Clear Error Report
int Clear_Error_Report()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	ERR_REPORT_OBJ ErrReport;


	//Clear Device¡¯s Error Report
	err = SdkClrErrReport(g_GtwyHandle, NodeID, &ErrReport);
	if (err)
	{
		std::cout << "Clear Device¡¯s Error Repor Failed!\n";
		return false;
	}

	std::cout	<< "index:" << ErrReport.index
				<< "ecode:" << ErrReport.ecode
				<< "errcw:" << ErrReport.errcw
				<< "errix:" << ErrReport.errix
				<< std::endl;

	return true;
}

//chapter 4.5 Get Last Error
int Get_Last_Report()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	ERR_REPORT_OBJ ErrReport;


	//Get Device¡¯s Last Error Report
	err = SdkGetLastErr(g_GtwyHandle, NodeID, &ErrReport);
	if (err)
	{
		std::cout << "Get Device¡¯s Last Error Report Failed!\n";
		return false;
	}

	std::cout	<< "index:" << ErrReport.index
				<< "ecode:" << ErrReport.ecode
				<< "errcw:" << ErrReport.errcw
				<< "errix:" << ErrReport.errix
				<< std::endl;

	return true;
}

//chapter 4.6 Get Motion Status
int Get_Motion_Status()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	MOTION_STATUS_OBJ MotionStatus;
	int Velo, Pr, Pa;


	//Get Device¡¯s Motion Status
	err = SdkGetMotionStatus(g_GtwyHandle, NodeID, &MotionStatus,&Velo,&Pr,&Pa);
	if (err)
	{
		std::cout << "Get Device¡¯s Motion Status Failed!\n";
		return false;
	}

	std::cout << "Motion MODE:" << MotionStatus.bits.MODE
		<< "Servon Enable:" << MotionStatus.bits.SVON
		<< "Input Port 1 level:" << MotionStatus.bits.IN0LV
		<< "Input Port 2 level:" << MotionStatus.bits.IN1LV
		<< "Input Port 3 level:" << MotionStatus.bits.IN2LV
		<< "Output Port 1 level:" << MotionStatus.bits.QP0LV
		<< "Output Port 2 level:" << MotionStatus.bits.QP1LV
		<< "Motor speed is zero(Motor Stoped):" << MotionStatus.bits.STOP
		<< "Motor Torque limited:" << MotionStatus.bits.TCIF
		<< "Motor Temperature limited:" << MotionStatus.bits.HEAT
		<< "Crew Alert:" << MotionStatus.bits.GALM
		<< "System Error:" << MotionStatus.bits.SERR
		<< std::endl;

	return true;
}

//chapter 4.7 Reset Motion Status
int Reset_Motion_Status()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	MOTION_STATUS_OBJ MotionStatus;


	//Reset Device¡¯s Motion Status
	err = SdkRstMotionStatus(g_GtwyHandle, NodeID, &MotionStatus);
	if (err)
	{
		std::cout << "Reset Device¡¯s Motion Status Failed!\n";
		return false;
	}

	std::cout	<< "Motion MODE:" << MotionStatus.bits.MODE
				<< "Servon Enable:" << MotionStatus.bits.SVON
				<< "Input Port 1 level:" << MotionStatus.bits.IN0LV
				<< "Input Port 2 level:" << MotionStatus.bits.IN1LV
				<< "Input Port 3 level:" << MotionStatus.bits.IN2LV
				<< "Output Port 1 level:" << MotionStatus.bits.QP0LV
				<< "Output Port 2 level:" << MotionStatus.bits.QP1LV
				<< "Motor speed is zero(Motor Stoped):" << MotionStatus.bits.STOP
				<< "Motor Torque limited:" << MotionStatus.bits.TCIF
				<< "Motor Temperature limited:" << MotionStatus.bits.HEAT
				<< "Crew Alert:" << MotionStatus.bits.GALM
				<< "System Error:" << MotionStatus.bits.SERR
				<< std::endl;

	return true;
}

//chapter 4.8 Get System Power-ON Time
int Get_System_Power_ON_Time()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	uint PowerON_Time;


	//Get System Power-ON Time
	err = SdkGetSystemTime(g_GtwyHandle, NodeID, &PowerON_Time);
	if (err)
	{
		std::cout << "Get System Power-ON Time Failed!\n";
		return false;
	}

	std::cout << "Power ON Time:" << PowerON_Time << std::endl;

	return true;
}

//chapter 4.9 Reset System Power-ON Time
int Reset_System_Power_ON_Time()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID
	uint PowerON_Time;


	//Reset System Power-ON Time
	err = SdkRstSystemTime(g_GtwyHandle, NodeID, &PowerON_Time);
	if (err)
	{
		std::cout << "Reset System Power-ON Time Failed!\n";
		return false;
	}

	std::cout << "Power ON Time:" << PowerON_Time << std::endl;

	return true;
}

//chapter 4.10 System Operation
int System_Operation()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;
	GATEWAY_INFO_OBJ Gtwy;

	if (m_bIs232Gtwy)
	{
		//serial device config
		Gtwy.detail.type = UIGW3_GW2513;
		Gtwy.COMidx = 7;
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
			std::cout << "IP address Error!\n";
			return -1;
		}
		Gtwy.tcp.IPport = 8888;

	}

	MEMBER_INFO_OBJ  Member[100];     // Members (Max. 100 members)
	ERRO err = 0;
	err = SdkStartCanNet(&Gtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	uint NodeID = 5; //Device¡¯s ID


	//Reset System; Device will reboot,
	err = SdkSetSystem(g_GtwyHandle, NodeID, _SY_RST_SYS,1);
	if (err)
	{
		std::cout << "Reset System Failed!\n";
		return false;
	}


	return true;
}