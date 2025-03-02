#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"

//chapter 9.1 Set Port Trigger Mode
int Set_Port_Trigger_Mode()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID

	//Set Port Trigger Mode -> port1 ,Filter time = 500 ms
	err = SdkSetTriggerMode(g_GtwyHandle, NodeID, 0,500);
	if (err)
	{
		std::cout << "Set Port Trigger Mode Failed!\n";
		return false;
	}


	return true;
}

//chapter 9.2 Get Port Trigger Mode
int Get_Port_Trigger_Mode()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	uint pRxData;

	//Get Port Trigger Mode -> port1
	err = SdkGetTriggerMode(g_GtwyHandle, NodeID, 0, &pRxData);
	if (err)
	{
		std::cout << "Get Port Trigger Mode Failed!\n";
		return false;
	}

	std::cout << "Port 1 Trigger Mode =" << pRxData << std::endl;

	return true;
}

//chapter 9.3 Set Port Digital Output
int Set_Port_Digital_Output()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	DIGITAL_OUTPUT_OBJ PortOut;

	PortOut.bits.bQP00 = 1;	// Port 1 level = high
	PortOut.bits.bQP03 = 0; // Port 4 level = low
	PortOut.bits.bMSK00 = 1;	// Port 1 Mask enable
	PortOut.bits.bMSK03 = 1;	// Port 4 Mask enable

	//Set Port Digital Output -> Port 1 level = high, Port 3 level = low
	err = SdkSetDigitalOutput(g_GtwyHandle, NodeID, &PortOut);
	if (err)
	{
		std::cout << "Set Port Trigger Mode Failed!\n";
		return false;
	}


	return true;
}

//chapter 9.4 Get IO Port Digital Status
int Get_IO_Port_Digital_Status()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	UIM_DIO_OBJ IOStatus;

	//Get IO Port Digital Status
	err = SdkGetDIOport(g_GtwyHandle, NodeID, &IOStatus);
	if (err)
	{
		std::cout << "Get Port Trigger Mode Failed!\n";
		return false;
	}

	std::cout	<< "Input Status:\n" 
				<< "IN00:" << IOStatus.bits.bIN00 << "IN01:" << IOStatus.bits.bIN01
				<< "IN02:" << IOStatus.bits.bIN02 << "IN03:" << IOStatus.bits.bIN03
				<<"......"
				<<std::endl;

	std::cout	<< "Output Status:\n"
				<< "QP00:" << IOStatus.bits.bQP00 << "QP01:" << IOStatus.bits.bQP01
				<< "QP02:" << IOStatus.bits.bQP02 << "QP03:" << IOStatus.bits.bQP03
				<< "......"
				<< std::endl;

	return true;
}

//chapter 9.5 Set Input Logic Acceleration
int Set_Input_Logic_Acceleration()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID

	//Set Input Logic Acceleration -> IN1 Rising Edge Trigger,Acceleration = 500 ms(time mode)
	err = SdkSetInputLogicAC(g_GtwyHandle, NodeID, IOL_CAT_IN1RISE, 500);
	if (err)
	{
		std::cout << "Set Input Logic Acceleration Failed!\n";
		return false;
	}


	return true;
}

//chapter 9.6 Get Input Logic Acceleration
int Get_Input_Logic_Acceleration()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	uint pRxData;

	//Get Input Logic Acceleration
	err = SdkGetInputLogicAC(g_GtwyHandle, NodeID, IOL_CAT_IN1RISE, &pRxData);
	if (err)
	{
		std::cout << "Set Input Logic Acceleration Failed!\n";
		return false;
	}

	std::cout << "IN1 Falling Edge Trigger Acceleration ="<< pRxData <<std::endl;

	return true;
}

//chapter 9.7 Set Input Logic Deceleration
int Set_Input_Logic_Deceleration()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID

	//Set Input Logic Deceleration -> IN1 Rising Edge Trigger,Acceleration = 500 ms(time mode)
	err = SdkSetInputLogicDC(g_GtwyHandle, NodeID, IOL_CAT_IN1RISE, 500);
	if (err)
	{
		std::cout << "Set Input Logic Deceleration Failed!\n";
		return false;
	}


	return true;
}

//chapter 9.8 Get Input Logic Deceleration
int Get_Input_Logic_Deceleration()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	uint pRxData;

	//Get Input Logic Deceleration
	err = SdkGetInputLogicDC(g_GtwyHandle, NodeID, IOL_CAT_IN1RISE, &pRxData);
	if (err)
	{
		std::cout << "Set Input Logic Deceleration Failed!\n";
		return false;
	}

	std::cout << "IN1 Falling Edge Trigger Deceleration =" << pRxData << std::endl;

	return true;
}

//chapter 9.9 Set Input Logic Speed
int Set_Input_Logic_Speed()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID

	//Set Input Logic Speed -> IN2 Falling Edge Trigger,Speed = 50000 pulse/sec
	err = SdkSetInputLogicSP(g_GtwyHandle, NodeID, IOL_CAT_IN2FALL, 50000);
	if (err)
	{
		std::cout << "Set Input Logic Speed Failed!\n";
		return false;
	}


	return true;
}

//chapter 9.10 Get Input Logic Speed
int Get_Input_Logic_Speed()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	int pRxSpeed;

	//Get Input Logic Speed -> IN2 Falling Edge Trigger
	err = SdkGetInputLogicSP(g_GtwyHandle, NodeID, IOL_CAT_IN2FALL, &pRxSpeed);
	if (err)
	{
		std::cout << "Set Input Logic Speed Failed!\n";
		return false;
	}

	std::cout << "IN2 Falling Edge Trigger Speed =" << pRxSpeed << std::endl;

	return true;
}

//chapter 9.11 Set Input Logic Relative Position
int Set_Input_Logic_Relative_Position()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID

	//Set Input Logic Relative Position -> IN3 Rising Edge Trigger,Relative Position = 80000 pulse
	err = SdkSetInputLogicPR(g_GtwyHandle, NodeID, IOL_CAT_IN3RISE, 80000);
	if (err)
	{
		std::cout << "Set Input Logic Relative Position Failed!\n";
		return false;
	}


	return true;
}

//chapter 9.12 Get Input Logic Relative Position
int Get_Input_Logic_Relative_Position()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	int pRxData;

	//Get Input Logic Relative Position -> IN3 Rising Edge Trigger,Relative Position = 80000 pulse/sec
	err = SdkGetInputLogicPR(g_GtwyHandle, NodeID, IOL_CAT_IN3RISE, &pRxData);
	if (err)
	{
		std::cout << "Get Input Logic Relative Position Failed!\n";
		return false;
	}

	std::cout << "IN3 Rising Edge Trigger Relative Position =" << pRxData << std::endl;

	return true;
}

//chapter 9.13 Set Input Logic Absolute Position
int Set_Input_Logic_Absolute_Position()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID

	//Set Input Logic Absolute Position -> IN2 Rising Edge Trigger,Absolute Position = 100000 pulse
	err = SdkSetInputLogicPA(g_GtwyHandle, NodeID, IOL_CAT_IN2RISE, 100000);
	if (err)
	{
		std::cout << "Set Input Logic Absolute Position Failed!\n";
		return false;
	}


	return true;
}

//chapter 9.14 Get Input Logic Absolute Position
int Get_Input_Logic_Absolute_Position()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	int pRxData;

	//Get Input Logic Absolute Position -> IN2 Rising Edge Trigger
	err = SdkGetInputLogicPA(g_GtwyHandle, NodeID, IOL_CAT_IN2RISE, &pRxData);
	if (err)
	{
		std::cout << "Get Input Logic Absolute Position Failed!\n";
		return false;
	}

	std::cout << "IN2 Rising Edge Trigger Absolute Position =" << pRxData << std::endl;

	return true;
}

//chapter 9.13 Set Input Logic
int Set_Input_Logic()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	INPUT_ACT_OBJ MxnCode;

	MxnCode.Action0 = ILC_DST_IDX;	//Falling Edge Trigger -> Deceleration to stop
	MxnCode.bNoTrig0 = 1;					//Don't trigger when powered on

	MxnCode.Action1 = ILC_SPR_IDX;	//Rising Edge Trigger -> Move according to preset displacement (+/-PR)
	MxnCode.bNoTrig1 = 1;					//Don't trigger when powered on

	//Set Input Logic -> set to port 2
	err = SdkSetInputLogic(g_GtwyHandle, NodeID, 1,MxnCode);
	if (err)
	{
		std::cout << "Set Input Logic Failed!\n";
		return false;
	}


	return true;
}

//chapter 9.14 Get Input Logic
int Get_Input_Logic()
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

	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 

	uint NodeID = 5; //Device¡¯s ID
	INPUT_ACT_OBJ MxnCode;

	//Get Input Logic -> Get port 2 action
	err = SdkGetInputLogic(g_GtwyHandle, NodeID, 1, &MxnCode);
	if (err)
	{
		std::cout << "Get Input Logic Failed!\n";
		return false;
	}

	std::cout	<< "Port 2 Falling Edge Trigger ActionCode:"		<< MxnCode.Action0
				<< "Port 2 Falling Edge Trigger PowerON Status:"	<< MxnCode.bNoTrig0
				<< "Port 2 Rising Edge Trigger ActionCode:"			<< MxnCode.Action1
				<< "Port 2 Rising Edge Trigger PowerON Status:"		<< MxnCode.bNoTrig1
				<< std::endl;

	return true;
}