#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"

//chapter 7.1 Set Motor Config
int Set_Motor_Config()
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
	uint pRxdata;


	//Set Motor Config -> Current = 5A
	err = SdkSetMotorConfig(g_GtwyHandle, NodeID, MTS_CUR_IDX, 50, &pRxdata);
	if (err)
	{
		std::cout << "Set Motor Config Failed!\n";
		return false;
	}

	std::cout << "Motor Current ="<< pRxdata << std::endl;

	//Set Motor Config -> Brake ON
	err = SdkSetMotorConfig(g_GtwyHandle, NodeID, MTS_BRK_IDX, 1, &pRxdata);
	if (err)
	{
		std::cout << "Set Motor Config Failed!\n";
		return false;
	}

	std::cout << "Motor Brake Status =" << pRxdata << std::endl;

	return true;
}

//chapter 7.2 Get Motor Config
int Get_Motor_Config()
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
	uint pRxdata[6];

	for (uint i = 0; i < 6; i++)
	{
		if (i == 4){i++;}

		//Get Motor Config
		err = SdkGetMotorConfig(g_GtwyHandle, NodeID, i, &pRxdata[i]);
		if (err)
		{
			std::cout << "Get Motor Config Failed!\n";
			return false;
		}
	}


	std::cout	<< "Motor Subdivision:"		<< pRxdata[0]
				<< "Motor Current:"			<< pRxdata[1]
				<< "Motor Standby Current:" << pRxdata[2]
				<< "Motor PowerON Delay Time:"<< pRxdata[3]
				<< "Motor Break Status:"	<< pRxdata[5]
				<< std::endl;


	return true;
}

//chapter 7.3 Set Acceleration
int Set_Acceleration()
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
	uint pRxdata;

	//Acceleration and deceleration mode refer to chapter 5.1
	//Set Acceleration -> Acceleration = 500 ms (time mode)
	err = SdkSetAcceleration(g_GtwyHandle, NodeID, 500, &pRxdata);
	if (err)
	{
		std::cout << "Set Acceleration Failed!\n";
		return false;
	}

	std::cout << "Motor Acceleration(time mode) =" << pRxdata << "/ms" << std::endl;


	return true;
}

//chapter 7.4 Get Acceleration
int Get_Acceleration()
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
	uint pRxdata;

	//Acceleration and deceleration mode refer to chapter 5.1
	//Set Acceleration
	err = SdkGetAcceleration(g_GtwyHandle, NodeID, &pRxdata);
	if (err)
	{
		std::cout << "Get Acceleration Failed!\n";
		return false;
	}

	std::cout << "Motor Acceleration(time mode) =" << pRxdata << "/ms" << std::endl;


	return true;
}

//chapter 7.5 Set Deceleration
int Set_Deceleration()
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
	uint pRxdata;

	//Acceleration and deceleration mode refer to chapter 5.1
	//Set Deceleration -> Deceleration = 500 ms (time mode)
	err = SdkSetDeceleration(g_GtwyHandle, NodeID, 500, &pRxdata);
	if (err)
	{
		std::cout << "Set Acceleration Failed!\n";
		return false;
	}

	std::cout << "Motor Deceleration(time mode) =" << pRxdata << "/ms" << std::endl;


	return true;
}

//chapter 7.6 Get Deceleration
int Get_Deceleration()
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
	uint pRxdata;

	//Acceleration and deceleration mode refer to chapter 5.1
	//Get Deceleration
	err = SdkGetDeceleration(g_GtwyHandle, NodeID, &pRxdata);
	if (err)
	{
		std::cout << "Get Deceleration Failed!\n";
		return false;
	}

	std::cout << "Motor Deceleration(time mode) =" << pRxdata << "/ms" << std::endl;


	return true;
}

//chapter 7.7 Set Cut-In Speed
int Set_Cut_In_Speed()
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
	uint pRxdata;

	//Set Cut-In Speed ->  = 5000 pulse/sec
	err = SdkSetCutInSpeed(g_GtwyHandle, NodeID, 5000, &pRxdata);
	if (err)
	{
		std::cout << "Set Cut-In Speed Failed!\n";
		return false;
	}

	std::cout << "Motor Cut-In Speed =" << pRxdata << " pulse/sec" << std::endl;


	return true;
}

//chapter 7.8 Get Cut-In Speed
int Get_Cut_In_Speed()
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
	uint pRxdata;

	//Get Cut-In Speed ->  = 5000 pulse/sec
	err = SdkGetCutInSpeed(g_GtwyHandle, NodeID, &pRxdata);
	if (err)
	{
		std::cout << "Get Cut-In Speed Failed!\n";
		return false;
	}

	std::cout << "Motor Cut-In Speed =" << pRxdata << " pulse/sec" << std::endl;


	return true;
}

//chapter 7.9 Set Emergency Stop Deceleration
int Set_Emergency_Stop_Deceleration()
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
	uint pRxdata;

	//Set Stop Deceleration ->  = 200000000 pulse/sec
	err = SdkSetStopDeceleration(g_GtwyHandle, NodeID, 200000000, &pRxdata);
	if (err)
	{
		std::cout << "Set Stop Deceleration Failed!\n";
		return false;
	}

	std::cout << "Stop Deceleration =" << pRxdata << " pulse/sec" << std::endl;

	return true;
}

//chapter 7.10 Get Emergency Stop Deceleration
int Get_Emergency_Stop_Deceleration()
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
	uint pRxdata;

	//Get Stop Deceleration
	err = SdkGetStopDeceleration(g_GtwyHandle, NodeID, &pRxdata);
	if (err)
	{
		std::cout << "Get Stop Deceleration!\n";
		return false;
	}

	std::cout << "Stop Deceleration =" << pRxdata << " pulse/sec" << std::endl;

	return true;
}

//chapter 7.11 Set Encoder Parameters
int Set_Encoder_Parameters()
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
	uint pRxdata;


	//Set Encoder Parameters -> Encoder Lines = 800
	err = SdkSetEncoderConfig(g_GtwyHandle, NodeID, QES_QER_IDX, 800, &pRxdata);
	if (err)
	{
		std::cout << "Set Encoder Parameters Failed!\n";
		return false;
	}

	std::cout << "Encoder Lines =" << pRxdata << std::endl;

	//Set Encoder Parameters -> stall tolerance = 10
	err = SdkSetEncoderConfig(g_GtwyHandle, NodeID, QES_PQE_IDX, 10, &pRxdata);
	if (err)
	{
		std::cout << "Set Encoder Parameters Failed!\n";
		return false;
	}

	std::cout << "Stall Tolerance =" << pRxdata << std::endl;

	return true;
}

//chapter 7.12 Get Encoder Parameters
int Get_Encoder_Parameters()
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
	uint pRxdata;


	//Get Encoder Parameters -> Encoder Lines
	err = SdkGetEncoderConfig(g_GtwyHandle, NodeID, QES_QER_IDX, &pRxdata);
	if (err)
	{
		std::cout << "Set Encoder Parameters Failed!\n";
		return false;
	}

	std::cout << "Encoder Lines =" << pRxdata << std::endl;

	//Get Encoder Parameters -> stall tolerance
	err = SdkGetEncoderConfig(g_GtwyHandle, NodeID, QES_PQE_IDX, &pRxdata);
	if (err)
	{
		std::cout << "Set Encoder Parameters Failed!\n";
		return false;
	}

	std::cout << "Stall Tolerance =" << pRxdata << std::endl;

	//Get Encoder Parameters -> Absolute encoder battery status
	err = SdkGetEncoderConfig(g_GtwyHandle, NodeID, QES_ABW_IDX, &pRxdata);
	if (err)
	{
		std::cout << "Set Encoder Parameters Failed!\n";
		return false;
	}

	std::cout << "Encoder battery status =" << pRxdata << std::endl;

	return true;
}

//chapter 7.13 Get Auxiliary Location Information
int Get_Auxiliary_Location_Information()
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
	int pRxdata;


	//Get Auxiliary Location Information
	err = SdkGetAuxPosition(g_GtwyHandle, NodeID, &pRxdata);
	if (err)
	{
		std::cout << "Get Auxiliary Location Information Failed!\n";
		return false;
	}

	std::cout << "Absolute encoder position =" << pRxdata << " pulse" << std::endl;


	return true;
}

//chapter 7.14 Set Backlash Compensation Value
int Set_Backlash_Compensation_Value()
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
	uint pRxdata;


	//Set Backlash Compensation Value -> 100
	err = SdkSetBacklashComp(g_GtwyHandle, NodeID, 100, &pRxdata);
	if (err)
	{
		std::cout << "Set Backlash Compensation Value Failed!\n";
		return false;
	}

	std::cout << "Backlash Compensation Value =" << pRxdata << std::endl;

	return true;
}

//chapter 7.15 Get Backlash Compensation Value
int Get_Backlash_Compensation_Value()
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
	uint pRxdata;


	//Get Backlash Compensation Value
	err = SdkGetBacklashComp(g_GtwyHandle, NodeID, &pRxdata);
	if (err)
	{
		std::cout << "Get Backlash Compensation Value Failed!\n";
		return false;
	}

	std::cout << "Backlash Compensation Value =" << pRxdata << " pulse" << std::endl;


	return true;
}

//chapter 7.16 Set Motion Limits
int Set_Motion_Limits()
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
	MOTION_LIMIT_OBJ MotionLimit;

	MotionLimit.LowWorkRange = -32000000;	//Minimum working limit
	MotionLimit.UppWorkRange = 32000000;	//Maximum working limit
	MotionLimit.LowBumpLimit = -35000000;	//Minimum collision limit
	MotionLimit.UppBumpLimit = 35000000;	//Minimum collision limit

	//Set Motion Limits -> motor work position range -32000000...+32000000;motor Bump position range -35000000...+35000000
	err = SdkSetMotionLimits(g_GtwyHandle, NodeID, &MotionLimit);
	if (err)
	{
		std::cout << "Set Motion Limits Failed!\n";
		return false;
	}

	return true;
}

//chapter 7.17 Get Motion Limits
int Get_Motion_Limits()
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
	MOTION_LIMIT_OBJ MotionLimit;

	//Get Motion Limits
	err = SdkGetMotionLimits(g_GtwyHandle, NodeID, &MotionLimit);
	if (err)
	{
		std::cout << "Get Motion Limits Failed!\n";
		return false;
	}

	std::cout	<< "LowWorkRange:" << MotionLimit.LowWorkRange
				<< "UppWorkRange:" << MotionLimit.UppWorkRange
				<< "LowBumpLimit:" << MotionLimit.LowBumpLimit
				<< "UppBumpLimit:" << MotionLimit.UppBumpLimit
				<< "MaxVelocity:" << MotionLimit.MaxVelocity
				<< "MaxAccValue:" << MotionLimit.MaxAccValue
				<< std::endl;

	return true;
}

//chapter 7.18 Reset Motion Limits
int Reset_Motion_Limits()
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
	MOTION_LIMIT_OBJ MotionLimit;

	//Reset Motion Limits
	err = SdkRstMotionLimits(g_GtwyHandle, NodeID);
	if (err)
	{
		std::cout << "Reset Motion Limits Failed!\n";
		return false;
	}

	return true;
}

//chapter 7.19 Set Limit Check Status
int Set_Limit_Check_Status()
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
	uint CheckStatus;

	//Set Limit Check Status -> Enable limit check
	err = SdkSetLimitCheck(g_GtwyHandle, NodeID,1,&CheckStatus);
	if (err)
	{
		std::cout << "Set Limit Check Status Failed!\n";
		return false;
	}

	std::cout << "CheckStatus:" << CheckStatus << std::endl;

	return true;
}

//chapter 7.20 Get Limit Check Status
int Get_Limit_Check_Status()
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
	uint CheckStatus;

	//Get Limit Check Status -> Enable limit check
	err = SdkGetLimitCheck(g_GtwyHandle, NodeID, &CheckStatus);
	if (err)
	{
		std::cout << "Get Limit Check Status Failed!\n";
		return false;
	}

	std::cout << "CheckStatus:" << CheckStatus << std::endl;

	return true;
}

//chapter 7.21 Get Desired Value
int Get_Desired_Value()
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
	uint index;
	int pRxData[6];

	for (index = 0; index < 6; index++)
	{
		//Get Desired Value
		err = SdkGetDesiredValue(g_GtwyHandle, NodeID, index, &pRxData[index]);
		if (err)
		{
			std::cout << "Get Desired Value Failed!\n";
			return false;
		}
	}


	std::cout	<< "Target control mode:"		<< pRxData[0]
				<< "Target operating current:"	<< pRxData[1]
				<< "target speed:"				<< pRxData[2]
				<< "target displacement:"		<< pRxData[3]
				<< "target location:"			<< pRxData[4]
				<< "target alarm temperature:"	<< pRxData[5]
				<< std::endl;

	return true;
}