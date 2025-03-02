#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"

//chapter 2.1 open can network
int OpenCAN()
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


	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle


	return true;
}

//chapter 2.2 close can network
int CloseCAN()
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


	unint g_GtwyHandle = Gtwy.GtwyHandle;	// Get the Gateway Handle			 // Record the gateway handle

	if (Gtwy.GtwyHandle)
	{
		err = SdkCloseCanNet(g_GtwyHandle);		//close can network
		if (err)
		{
			std::cout << "Close CAN Network Failed!\n";
			return -1;
		}
	}

	return true;
}

//chapter 2.3 get can gateway information
int GatewayInfo()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;

	GATEWAY_INFO_OBJ pGtwy;			//Gateway information
	MEMBER_INFO_OBJ  Member[100];   //Members (Max. 100 members)

	if (m_bIs232Gtwy)
	{
		//serial device config
		pGtwy.detail.type = UIGW3_GW2513;
		pGtwy.COMidx = 7;
		pGtwy.COMbaud = 57600;

	}
	else if (m_bIsEthGtwy)
	{
		//Ethernet device config
		pGtwy.detail.type = UIGW3_ETHCAN;
		pGtwy.detail.IP[0] = 254;
		pGtwy.detail.IP[1] = 1;
		pGtwy.detail.IP[2] = 168;
		pGtwy.detail.IP[3] = 192;
		if ((!pGtwy.tcp.IPaddr))
		{
			std::cout << "IP address Error!\n";
			return -1;
		}
		pGtwy.tcp.IPport = 8888;

	}

	ERRO err = 0;
	err = SdkStartCanNet(&pGtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	//print CAN Gateway information
	err = SdkGetGtwyInfo(&pGtwy, Member);

	std::cout << "Gateway Handle:" << pGtwy.GtwyHandle
		<< "Serial Port Number:" << pGtwy.COMidx
		<< "Serial port baud rate:" << pGtwy.COMbaud
		<< "CAN BIT RATE:" << pGtwy.BTRidx
		<< "Gateway S/N Number:" << pGtwy.SerialNo
		<< "����"
		<< std::endl;

	return true;
}

//chapter 2.4 get can gateway information
int MemberInfo()
{
	// Set the current configuration type
	bool m_bIs232Gtwy = true;
	bool m_bIsEthGtwy = false;

	GATEWAY_INFO_OBJ pGtwy;			//Gateway information
	MEMBER_INFO_OBJ  Member[100];   //Members (Max. 100 members)

	if (m_bIs232Gtwy)
	{
		//serial device config
		pGtwy.detail.type = UIGW3_GW2513;
		pGtwy.COMidx = 7;
		pGtwy.COMbaud = 57600;

	}
	else if (m_bIsEthGtwy)
	{
		//Ethernet device config
		pGtwy.detail.type = UIGW3_ETHCAN;
		pGtwy.detail.IP[0] = 254;
		pGtwy.detail.IP[1] = 1;
		pGtwy.detail.IP[2] = 168;
		pGtwy.detail.IP[3] = 192;
		if ((!pGtwy.tcp.IPaddr))
		{
			std::cout << "IP address Error!\n";
			return -1;
		}
		pGtwy.tcp.IPport = 8888;

	}

	ERRO err = 0;
	err = SdkStartCanNet(&pGtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	//print CAN member information
	err = SdkGetMemberList(pGtwy.GtwyHandle, Member);

	// Number of members
	int  MemberQuantity = 0;           // Number of members
	MemberQuantity = pGtwy.MemberQty;

	//print Member information
	for (int i = MemberQuantity; i > 0; i--)
	{
		std::cout << "CAN id:" << (int)Member[i - 1].CANnid
			<< " Model:" << Member[i - 1].Model.ModelStr
			<< " Firmware:" << Member[i - 1].Model.FirmVer
			<< std::endl;
	}

	return true;
}

//chapter 2.5 set eth gateway ip address
int Set_ETH_IP_Addr()
{
	// Set the current configuration type
	GATEWAY_INFO_OBJ pGtwy;			//Gateway information
	MEMBER_INFO_OBJ  Member[100];   //Members (Max. 100 members)

	//Ethernet device config
	pGtwy.detail.type = UIGW3_ETHCAN;
	pGtwy.detail.IP[0] = 254;
	pGtwy.detail.IP[1] = 1;
	pGtwy.detail.IP[2] = 168;
	pGtwy.detail.IP[3] = 192;
	if ((!pGtwy.tcp.IPaddr))
	{
		std::cout << "IP address Error!\n";
		return -1;
	}
	pGtwy.tcp.IPport = 8888;



	ERRO err = 0;
	err = SdkStartCanNet(&pGtwy, Member, 0);
	if (err)
	{
		std::cout << "No Gateway,please check the parameter!\n";
		return -1;
	}

	uchar IPaddr[4] = { 25, 1, 168, 192 };	//new ip address

	err = SdkSetIPaddr(pGtwy.GtwyHandle, IPaddr);
	if (err)
	{
		std::cout << "Set IPaddr Failed!\n";
		return -1;
	}

	return true;
}