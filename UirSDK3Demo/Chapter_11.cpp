#include <iostream>
#include "uirSDKcommon3.h"
#include "UIError.h"
#include "CAN30SDK.h"
#include <thread>

//chapter 11.1 Real-Time Notification
int Real_Time_Notification()
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

	//Creative a thread to process RTCN 
	//reference to UirSDK3Demo.cpp -> "RTCN thread" & "RTCN massage process" 
	while (1)
	{
		UI_MSG_OBJ uimsg = { 0 };

		// Get real-time messages
		ERRO retv = SdkGetRTCN(g_GtwyHandle, &uimsg);
		if (retv == 1)
		{
			//process funtion code here
		}

		PauseMS(5);
	}

	return true;
}
