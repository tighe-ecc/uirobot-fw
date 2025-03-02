#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"

//chapter 3.1 Set Communication Parameters
int Set_Protocol_Para()
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

	//modify Device¡¯s CAN bit rate -> 500K,If you modify it, you may not be able to connect
	err = SdkSetProtocolPara(g_GtwyHandle, NodeID, PPS_BTR, 2);
	if (err)
	{
		std::cout << "Modify Device¡¯s CAN bit rate Failed!\n";
		return false;
	}

	//modify Device¡¯s ID£¬5->10, To modify the Device¡¯s ID, you need to power off and restart to take effect
	err = SdkSetProtocolPara(g_GtwyHandle, NodeID, PPS_NodeID, 10);
	if (err)
	{
		std::cout << "Modify Device¡¯s ID Failed!\n";
		return false;
	}

	//modify Group ID ->10
	err = SdkSetProtocolPara(g_GtwyHandle, NodeID, PPS_GroupID, 20);
	if (err)
	{
		std::cout << "Modify Group ID Failed!\n";
		return false;
	}


	return true;
}

//chapter 3.2 Get Communication Parameters
int Get_Protocol_Para()
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

	//modify Device¡¯s CAN bit rate -> 500K,If you modify it, you may not be able to connect
	err = SdkGetProtocolPara(g_GtwyHandle, NodeID, PPS_BTR, &pRxdata);
	if (err)
	{
		std::cout << "Get Device¡¯s CAN bit rate Failed!\n";
		return false;
	}

	std::cout << "CAN bit rate:" << pRxdata << std::endl;

	//modify Device¡¯s ID£¬5->10, To modify the Device¡¯s ID, you need to power off and restart to take effect
	err = SdkGetProtocolPara(g_GtwyHandle, NodeID, PPS_NodeID, &pRxdata);
	if (err)
	{
		std::cout << "Get Device¡¯s ID Failed!\n";
		return false;
	}

	std::cout << "CAN ID:" << pRxdata << std::endl;

	//modify Group ID ->10
	err = SdkGetProtocolPara(g_GtwyHandle, NodeID, PPS_GroupID, &pRxdata);
	if (err)
	{
		std::cout << "Get Group ID Failed!\n";
		return false;
	}

	std::cout << "Group ID:" << pRxdata << std::endl;

	return true;
}