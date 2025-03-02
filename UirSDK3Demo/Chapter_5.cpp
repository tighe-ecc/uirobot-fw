#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"

//chapter 5.1 Set Initial Config
int Set_Initial_Config()
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
	uint pRxdata;


	//Set Initial Config -> Power-on enable (0 = disabled, 1 = enabled)
	err = SdkSetInitialConfig(g_GtwyHandle, NodeID, 0, 1, &pRxdata);
	if (err)
	{
		std::cout << "Set Power-on enable Failed!\n";
		return false;
	}

	std::cout << "Power-on enable:" << pRxdata << std::endl;

	//Set Initial Config -> Acceleration and deceleration mode = time
	err = SdkSetInitialConfig(g_GtwyHandle, NodeID,4,1,&pRxdata);
	if (err)
	{
		std::cout << "Set Initial Config Failed!\n";
		return false;
	}

	std::cout << "Acceleration/Deceleration mode = " << pRxdata << std::endl;



	return true;
}

//chapter 5.2 Get Initial Config
int Get_Initial_Config()
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
	uint pRxdata;


	//Get Initial Config -> Power-on enable (0 = disabled, 1 = enabled)
	err = SdkGetInitialConfig(g_GtwyHandle, NodeID, 0, &pRxdata);
	if (err)
	{
		std::cout << "Set Power-on enable Failed!\n";
		return false;
	}

	std::cout << "Power-on enable:" << pRxdata << std::endl;

	//Get Initial Config -> Acceleration and deceleration mode = time
	err = SdkGetInitialConfig(g_GtwyHandle, NodeID, 4, &pRxdata);
	if (err)
	{
		std::cout << "Set Initial Config Failed!\n";
		return false;
	}

	std::cout << "Acceleration/Deceleration mode = " << pRxdata << std::endl;



	return true;
}