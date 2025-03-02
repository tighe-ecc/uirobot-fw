#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"

//chapter 6.1 Set Information Enable
int Set_Information_Enable()
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

	//open all notification,index 0...11 set to enable 
	for (uint index = 0; index < 12; index++)
	{
		//Set Information Enable
		err = SdkSetInformEnable(g_GtwyHandle, NodeID, index, 1, &pRxdata);
		if (err)
		{
			std::cout	<< "Set Information Enable Failed!" 
						<<"Index =" << index 
						<< "pRxdata =" << pRxdata 
						<< std::endl;
			return false;
		}
	}


	return true;
}

//chapter 6.1 Get Information Enable
int Get_Information_Enable()
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

	//Get open all notification,index 0...11 
	for (uint index = 0; index < 12; index++)
	{
		//Get Information Enable
		err = SdkGetInformEnable(g_GtwyHandle, NodeID, index, &pRxdata);
		if (err)
		{
			std::cout << "Get Information Enable Failed!\n";
			return false;
		}


		std::cout 	<< "Index =" << index
					<< "Enable Status:" << pRxdata
					<< std::endl;

	}





	return true;
}