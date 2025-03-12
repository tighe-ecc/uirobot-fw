////////////////////////////////////////////////////////////////////////////////
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,//
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED	  //
// WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.	  //
//																			  //
//							Copyright (c) UIROBOT.							  //
//							 All rights reserved.							  //
////////////////////////////////////////////////////////////////////////////////

#pragma once
#pragma warning(disable: 4081 4083)

const char * STDCALLAPI UIMSDKGetVersion(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                               Static Library Prototype

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Notes:

// 1) 1 CAN network = 1 gateway + N members; 
//    1 个 CAN 网络 = 1 网关 + N 成员;

// 2) StartCanNet() = open hardware port + open gateway + get gateway information + get members' information + complete all preparatory work;
//    StartCanNet() = 打开硬件端口 + 打开网关 + 统计网关信息 + 统计全部成员信息 + 完成一切准备工作;

// 3) When calling StartCanNet(), the SDK will assign a unique GtwyHandle to the Gtwy specified by the user, and the GtwyHandle should be provided for all subsequent operations on the gateway.
//    调用 StartCanNet(), SDK 会分配唯一的 GtwyHandle 给到用户指定的 Gtwy；之后对该网关的操作，都应提供该 GtwyHandle;

// 4) Up to 32 "RS232-CAN" (UIM2512 / UIM2513) + 64 "Eth-CAN" (UIM2522/UIM2523) can be useed at the same time; Each CAN network contains 1 gateway and up to 100 members;
//    最多同时支持32个"串口－CAN网络"（UIM2502/UIM2503）+ 64个"以太网-CAN网络"(UIM2512/UIM2513); 每个CAN网络包含 1个网关 和 最多100个成员 ;

// 5) Function return value: 0 = success, !0 = error code(refer to UIError.h).
//    函数返回值 : 0 = 成功, !0 = 错误代码（参考 UIError.h);

// 6) When using GroupID, if you want to receive ACK from devices within the group, you need to set CANid = GroupID | 0x80.
//    使用 GroupID 时，如果希望收到组内各器件的 ACK, 需要设置  CANid = GroupID|0x80;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// [System]
;
#pragma "[CAN Network   ]" {
/*==========================================================================
Name  : SdkStartCanNet
Param : pGtwy			Gateway information (for the definition of GATEWAY_INFO_OBJ, see uirSDKcommon.h) 
						网关信息(GATEWAY_INFO_OBJ定义详见uirSDKcommon.h)   						
        pMember			Member information (MEMBER_INFO_OBJ is defined in uirSDKcommon.h)
						成员信息(MEMBER_INFO_OBJ定义详见uirSDKcommon.h)    					
        UseConstLink	User connection network mode        The value can only be the following:
						0  					Normal mode
						1					Debug mode
						用户连接网络模式    只能是如下参数:                  	
						0                   正常模式					 
						1                   调试模式					   
Return: 0				No Error (Success)
        !0				Error Code
Note  : Open CAN Network
		开启 CAN Network  	
===========================================================================*/
UISIMCANFUNC_API ERRO STDCALLAPI SdkStartCanNet(GATEWAY_INFO_OBJ* pGtwy, MEMBER_INFO_OBJ* pMember, uint UseConstLink = 0);

/*==========================================================================
Name  : SdkCloseCanNet
Param : GtwyHandle		Gateway handle 
						网关句柄
return: 0				No Error (Success)
		!0				Error Code
Note  : Close Can Network
		关闭 CAN Network

==========================================================================*/
UISIMCANFUNC_API ERRO STDCALLAPI SdkCloseCanNet(uint GtwyHandle);

/****************************************************************************
Name  : SdkGetGtwyInfo
Param : pGtwy			Gateway information (for the definition of GATEWAY_INFO_OBJ, see uirSDKcommon.h)
						网关信息(结构体GATEWAY_INFO_OBJ定义详见uirSDKcommon.h)
		pMember			Member information (structure MEMBER_INFO_OBJ is defined in uirSDKcommon.h)
						成员信息(结构体MEMBER_INFO_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						查询 CAN 网关信息 成功 
		!0			Failure
						查询 CAN 网关信息 失败 
Note  : Get Can Gateway Information
		查询 CAN 网关信息
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetGtwyInfo(GATEWAY_INFO_OBJ* pGtwy, MEMBER_INFO_OBJ* pMember);

/****************************************************************************
Name  : SdkGetMemberList
Param : GtwyHandle		Gateway handle
						网关句柄
		pMember			Member information (structure MEMBER_INFO_OBJ is defined in uirSDKcommon.h)
						成员信息(结构体MEMBER_INFO_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						查询 CAN 网关信息 成功 
		!0			Failure
						查询 CAN 网关信息 失败
Note  : Get Can Member Information List
		查询 CAN 网关信息
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetMemberList(uint GtwyHandle, MEMBER_INFO_OBJ* pMember);

/****************************************************************************
Name  : SdkSetIPaddr
Param : GtwyHandle		Gateway handle
						网关句柄
		IPaddr			IP address (string)
						IP地址（字符串）
return: 0				Success
						设置 ETH 网关（2512/2513）的 IPaddr 成功
		!0			Failure 
						设置 ETH 网关（2512/2513）的 IPaddr 失败
Note  : Set Eth Gateway Ip Address
		设置 ETH 网关（2512/2513）的 IPaddr
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetIPaddr(uint GtwyHandle, uchar* IPaddr);
#pragma }
;
#pragma "[Protocol Param]" {
/****************************************************************************
Name  : SdkSetProtocolPara
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Set parameter index  The value can only be the following:
						设定参数下标		只能是如下参数:
						PPS_SysUpdate       0 (System upgrade)（系统升级）						
						PPS_BDR             1 (serial baud rate)（串口波特率）
						PPS_Parity			3 （？？）
						PPS_BTR				5  (CAN bit rate)（CAN 比特率）
						PPS_TargetID        6 （？？）
						PPS_NodeID          7 (Device’s ID number)（站点号）
						PPS_GroupID         8 (Group number)（组号）
						PPS_GatewayACK      11（？？）
		data            Set parameter content
						设定参数内容
		pRxdata         Set result feedback
						设定结果反馈
return: 0				Success
						设置 通讯参数 成功
		!0			Failure  
						设置 通讯参数 失败
Note  : Set Communication Parameters
		设置 通讯参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetProtocolPara(uint GtwyHandle, uint CANid, uint Index, uint Data, uint* pRxdata = NULL);

/****************************************************************************
Name  : SdkGetProtocolPara
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Gets the parameter index  The value can only be the following:
						获取参数下标        只能是如下参数:
						PPS_SysUpdate       0 (System upgrade)（系统升级）						
						PPS_BDR             1 (serial baud rate)（串口波特率）
						PPS_Parity			3 （？？）
						PPS_BTR				5  (CAN bit rate)（CAN 比特率）
						PPS_TargetID        6 （？？）
						PPS_NodeID          7 (Device’s ID number)（站点号）
						PPS_GroupID         8 (Group number)（组号）
						PPS_GatewayACK      11（？？）
		pRxdata         Communication parameter content
						通讯参数内容
return: 0				Success
						查询 通讯参数 成功
		!0			Failure 
						查询 通讯参数 失败
Note  : Get Communication Parameters
		查询 通讯参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetProtocolPara(uint GtwyHandle, uint CANid, uint Index, uint* pRxdata);
#pragma}
;
#pragma "[System Info.  ]" {
/****************************************************************************
Name  : SdkSetSerial
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		iSerialNumberIn		Set Serial Number
							设置的序列号
		pSerialNumberOut	Result feedback
							设置结果反馈
return: 0					Success
							设置 序列号 成功 
		!0				Failure
							设置 序列号 失败  
Note  : Set Serial Number
		设置 序列号
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetSerial(uint GtwyHandle, uint CANid, uint iSerialNumberIn, uint* pSerialNumberOut = NULL);

/****************************************************************************
Name  : SdkGetSerial
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pSerialNumber	Serial number
						序列号
return: 0				Query serial number Success
						查询 序列号 成功
		!0			Query serial number Failure 
						查询 序列号 失败
Note  : Query serial number
		查询 序列号
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetSerial(uint GtwyHandle, uint CANid, uint * pSerialNumber);

/****************************************************************************
Name  : SdkGetModel
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pModelInfo		Device model
						器件型号
return: 0				Querying device model Success
						查询 器件型号 成功
		!0			Querying device model Failure 
						查询 器件型号 失败
Note  : Get serial numberl
		查询 器件型号
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetModel(uint GtwyHandle, uint CANid, MODEL_INFO_OBJ* pModelInfo);

/****************************************************************************
Name  : SdkGetErrReport
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		ErrIdx			Serial number of the historical error record  The value can only be the following:
						历史错误记录的序号	只能是如下参数:
						ErrIdx = 10....17(最新...最旧)(The latest... oldest)
		pRxData			Error report (See uirSDKcommon.h for the definition of structure ERR_REPORT_OBJ)
						错误报告(结构体ERR_REPORT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						查询错误报告 成功
		!0			Failure 
						查询错误报告 失败
Note  : Get Error Report
		查询错误报告
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetErrReport(uint GtwyHandle, uint CANid, uint ErrIdx, ERR_REPORT_OBJ* pRxData);

/****************************************************************************
Name  : SdkClrErrReport
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Feedback error report (see uirSDKcommon.h for the definition of structure ERR_REPORT_OBJ)
						反馈的错误报告(结构体ERR_REPORT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						清除所有错误 成功
		!0			Failure 
						清除所有错误 失败
Note  : Clear Error Report
		清除所有错误
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkClrErrReport(uint GtwyHandle, uint CANid, ERR_REPORT_OBJ* pRxData);

/****************************************************************************
Name  : SdkGetLastErr
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			The latest error report obtained (see uirSDKcommon.h for the definition of structure ERR_REPORT_OBJ)
						获取的最新的错误报告(结构体ERR_REPORT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						查询最新错误 成功
		!0			Failure 
						查询最新错误 失败
Note  : Get Motor Status Feedback
		查询最新错误
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetLastErr(uint GtwyHandle, uint CANid, ERR_REPORT_OBJ* pRxData);

/****************************************************************************
Name  : SdkGetMotionStatus
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pStatus			Motor status (Structure MOTION_STATUS_OBJ is defined in uirSDKcommon.h)
						电机状态(结构体MOTION_STATUS_OBJ定义详见uirSDKcommon.h)
		pVe				Motor speed
						电机速度
		pPr				Motor displacement(Relative position)
						电机位移（相对位置）
		pPa				Motor position (absolute position)
						电机位置（绝对位置）
return: 0				Success
						查询电机状态 成功
		!0			Failure 
						查询电机状态 失败
Note  : Query motor status
		查询电机状态
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetMotionStatus(uint GtwyHandle, uint CANid, MOTION_STATUS_OBJ* pStatus, int* pVe, int* pPr, int* pPa);

/****************************************************************************
Name  : SdkRstMotionStatus
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pStatus			Motor state feedback (Structure MOTION_STATUS_OBJ is defined in uirSDKcommon.h)
						电机状态反馈(结构体MOTION_STATUS_OBJ定义详见uirSDKcommon.h)
return: 0				Reset motor status Success
						重置电机状态 成功
		!0			Reset motor status Failure 
						重置电机状态 失败
Note  : Reset motor status (just clear PAIF/PSIF/TCIF and return to motor status)
		重置电机状态（仅仅 清零 PAIF/PSIF/TCIF,并返回电机状态）
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkRstMotionStatus(uint GtwyHandle, uint CANid, MOTION_STATUS_OBJ* pStatus);

/****************************************************************************
Name  : SdkGetSystemTime
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pPoweOnTime		Power-on duration (unit: us)
						上电时长(单位: us)
return: 0				Query the duration of the device power-on period Success
						查询器件上电至今的时长 成功
		!0			Query the duration of the device power-on period Failure 
						查询器件上电至今的时长 失败
Note  : Query the duration of the device power-on period
		查询器件上电至今的时长
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetSystemTime(uint GtwyHandle, uint CANid, uint * pPoweOnTime);

/****************************************************************************
Name  : SdkRstSystemTime
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pPoweOnTime		Power-on duration (unit: us)
						上电时长(单位: us)
return: 0				Resets the duration since the device is powered on Success
						重置器件上电至今的时长 成功
		!0			Resets the duration since the device is powered on Failure 
						重置器件上电至今的时长 失败
Note  : Resets the duration since the device is powered on
		重置器件上电至今的时长
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkRstSystemTime(uint GtwyHandle, uint CANid, uint * piPoweOnTime);

//[系统操作参数对象] index 使用如下参数
#define _SY_ULK_SYS 0 // 系统解锁, Unlock System（仅620/720/630/730）;
#define _SY_RST_SYS 1 // 重启系统, Reset System;
#define _SY_RST_DFL 2 // 出厂设置, Restore Default;
#define _SY_SYN_ENA 4 // 使能同步信号,sy[4]=0/1=禁止/使能（仅2503/MMC900/MMC901）;
/****************************************************************************
Name  : SdkSetSystem
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			System operation parameter index
						系统操作参数下标
		CfgData			System operation parameters
						系统操作参数内容
		pRxData			Feedback result of setting system operation parameters
						设置系统操作参数结果反馈
return: 0				Set system operation parameters Success
						设置系统操作参数 成功
		!0			Set system operation parameters Failure 
						设置系统操作参数 失败
Note  : Set system operation parameters
		设置系统操作参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetSystem(uint GtwyHandle, uint CANid, uint Index, uint CfgData, uchar* pRxData = NULL);

/****************************************************************************
Name  : SdkGetSystem
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			System operation parameter index
						系统操作参数下标
		pRxData			System operation parameters
						系统操作参数内容
return: 0				Query system operation parameters Success
						查询系统操作参数 成功
		!0			Query system operation parameters Failure 
						查询系统操作参数 失败
Note  : Query system operation parameters
		查询系统操作参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetSystem(uint GtwyHandle, uint CANid, uint Index, uchar* pRxData);

/****************************************************************************
Name  : SdkSetHeatLimit
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		TempLimit		Temperature alarm value
						温度警报值
		pRxData			Set temperature alarm value result feedback
						设置温度报警值结果反馈
return: 0				Set the temperature alarm value Success
						设置温度报警值 成功
		!0			Set the temperature alarm value Failure 
						设置温度报警值 失败
Note  : Set the temperature alarm value
		设置温度报警值
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetHeatLimit(uint GtwyHandle, uint CANid, uint TempLimit, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkGetHeatLimit
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Temperature alarm value
						温度警报值
return: 0				Query the temperature alarm value Success
						查询温度报警值 成功
		!0			Query the temperature alarm value Failure 
						查询温度报警值 失败
Note  : Query the temperature alarm value
		查询温度报警值
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetHeatLimit(uint GtwyHandle, uint CANid, uint* pRxData);

/****************************************************************************
Name  : SdkGetTemperature
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			The obtained temperature value
						获取的温度值
return: 0				Querying the current temperature Success
						查询当前温度 成功
		!0			Querying the current temperature Failure 
						查询当前温度 失败
Note  : Querying the current temperature
		查询当前温度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetTemperature(uint GtwyHandle, uint CANid, uint* pRxData);

/****************************************************************************
Name  : SdkSetMmcDate
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		yr				year 年 
		mo				month 月 
		da				day 日 
		hr				hour 时
		mi				minute 分
		se				second 秒
return: 0				Set the MMC date Success
						设置 MMC 日期 成功
		!0			Set the MMC date
						设置 MMC 日期 失败 Failure 
Note  : Set the MMC date
		设置 MMC 日期
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetMmcDate(uint GtwyHandle, uint CANid, uint yr, uint mo, uint da, uint hr, uint mi, uint se);

/****************************************************************************
Name  : SdkGetMmcDate
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		yr				year 年 
		mo				month 月 
		da				day 日 
		hr				hour 时
		mi				minute 分
		se				second 秒
return: 0				Query the MMC date Success
					 	成功
		!0			Query the MMC date Failure 
						查询 MMC 日期 失败
Note  : Query the MMC date
		查询 MMC 日期
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetMmcDate(uint GtwyHandle, uint CANid, uint* yr, uint* mo, uint* da, uint* hr, uint* mi, uint* se);

#pragma}
;
#pragma "[Initial Config]" {
/****************************************************************************
Name  : SdkSetInitialConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Power-on configuration parameter index  The value can only be the following		
						index									TxData
						0 Power-on Enable 						0 = disable, 1 = Enable.
						1 Positive turn setting 				0 = cw, 1 = ccw;
						2 UPG enable 							0 = disabled, 1 = enabled.
						3 Input trigger lock					0 = disable, 1 = Enable.
						4 Acceleration and deceleration mode 	0 = value, 1 = time;
						5 Encoder type 							0 = incremental, 1 = absolute value (720 determined by servo drive);
						6 Open Closed loop Enable 				0 = open loop, 1 = closed loop.
						7 Soft limit Enable 					0 = disable, 1 = enable.
						8 brake ports							0 = normal, 1 = enable brake control (brake signal output OP01);
						9 Brake closed loop						0 = normal, 1 = enable brake monitoring (brake signal input IN02);
						10 Forced hibernation					0 = normal, 1 = power-on into hibernation (no operation control and CAN transmission);
						255 Overall query/Settings (i.e.uiValue)

						上电配置参数下标		只能是如下参数:
						0  上电使能				0 = 禁止, 1 = 使能;
						1  正转向设定			0 = cw,   1 = ccw;
						2  UPG 使能				0 = 禁止, 1 = 使能;
						3  输入触发锁定			0 = 禁止, 1 = 使能;
						4  加减速方式			0 = 数值, 1 = 时间;
						5  编码器类型			0 = 增量, 1 = 绝对值（720 由伺服驱动器决定）;
						6  开闭环使能			0 = 开环, 1 = 闭环;
						7  软限位使能			0 = 禁止, 1 = 使能;
						8  刹车端口				0 = 正常, 1 = 使能刹车控制（刹车信号输出 OP01）;
						9  刹车闭环				0 = 正常, 1 = 使能刹车监控（刹车信号输入 IN02）;
						10 强制休眠				0 = 正常, 1 = 上电进入休眠（无运控及CAN发送）;
						255 整体查询/设置 (即 .uiValue)
		TxData			Power-on configuration parameters
						上电配置参数内容
		pRxData			Set the power-on configuration result feedback 
						设置上电配置结果反馈
return: 0				Set the power-on configuration Success
						设置上电配置 成功
		!0			SSet the power-on configuration Failure 
						设置上电配置 失败
Note  : Set the power-on configuration result feedback
		设置上电配置
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetInitialConfig(uint GtwyHandle, uint CANid, uint Index, uint TxData, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetInitialConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Power-on configuration parameter index  The value can only be the following		
						index									TxData
						0 Power-on Enable 						0 = disable, 1 = Enable.
						1 Positive turn setting 				0 = cw, 1 = ccw;
						2 UPG enable 							0 = disabled, 1 = enabled.
						3 Input trigger lock					0 = disable, 1 = Enable.
						4 Acceleration and deceleration mode 	0 = value, 1 = time;
						5 Encoder type 							0 = incremental, 1 = absolute value (720 determined by servo drive);
						6 Open Closed loop Enable 				0 = open loop, 1 = closed loop.
						7 Soft limit Enable 					0 = disable, 1 = enable.
						8 brake ports							0 = normal, 1 = enable brake control (brake signal output OP01);
						9 Brake closed loop						0 = normal, 1 = enable brake monitoring (brake signal input IN02);
						10 Forced hibernation					0 = normal, 1 = power-on into hibernation (no operation control and CAN transmission);
						255 Overall query/Settings (i.e.uiValue)
						
						上电配置参数下标		只能是如下参数:
						index					内容
						0  上电使能				0 = 禁止, 1 = 使能;
						1  正转向设定			0 = cw,   1 = ccw;
						2  UPG 使能				0 = 禁止, 1 = 使能;
						3  输入触发锁定			0 = 禁止, 1 = 使能;
						4  加减速方式			0 = 数值, 1 = 时间;
						5  编码器类型			0 = 增量, 1 = 绝对值（720 由伺服驱动器决定）;
						6  开闭环使能			0 = 开环, 1 = 闭环;
						7  软限位使能			0 = 禁止, 1 = 使能;
						8  刹车端口,			0 = 正常, 1 = 使能刹车控制（刹车信号输出 OP01）;
						9  刹车闭环,			0 = 正常, 1 = 使能刹车监控（刹车信号输入 IN02）;
						10 强制休眠,			0 = 正常, 1 = 上电进入休眠（无运控及CAN发送）;
						255 整体查询/设置 (即 .uiValue)
		pRxData			Corresponding contents of power-on configuration parameters
						对应上电配置参数的内容对应上电配置参数的内容
return: 0				Query the power-on configuration Success
						查询上电配置 成功
		!0			Query the power-on configuration Failure 
						查询上电配置 失败
Note  : Query the power-on configuration
		查询上电配置
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkGetInitialConfig(uint GtwyHandle, uint CANid, uint Index, uint* pRxData);
#pragma}
;
#pragma "[Inform Enable ]" {
/****************************************************************************
Name  : SdkSetInformEnable
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			站点
		index			Notify the enable parameter index 		The value can only be the following
						index									TxData
						0 P1 Port change notification 					0 = disable, 1 = Enable.
						1 P2 port change notification 					0 = disable, 1 = Enable.
						2 P3 port change notification  					0 = disable, 1 = Enable.
						3 P4 Port change notification 					0 = disable, 1 = Enable.
						4 P5 Port change notification 					0 = Disable, 1 = Enable.
						5 P6 Port change notification 					0 = Disable, 1 = Enable.
						6 P7 Port change notification					0 = Disable, 1 = Enable.
						7 P8 Port change notification 					0 = disable, 1 = Enable.
						8 Notification of PTP movement in place 		0 = disable, 1 = Enable;
						9 Motor gridlock notification 					0 = prohibit, 1 = enable;
						10 PVT data buffer water Dry notification 		0 = Disable, 1 = Enable.
						11 When the PVT data buffer water level is low	0 = disable, 1 = Enable.
						255 Overall query/Settings (i.e.uiValue)

						通知使能参数下标		只能是如下参数:
						index							TxData
						0  P1端口变化通知				0 = 禁止, 1 = 使能;
						1  P2端口变化通知				0 = 禁止, 1 = 使能;
						2  P3端口变化通知				0 = 禁止, 1 = 使能;
						3  P4端口变化通知				0 = 禁止, 1 = 使能;
						4  P5端口变化通知				0 = 禁止, 1 = 使能;
						5  P6端口变化通知				0 = 禁止, 1 = 使能;
						6  P7端口变化通知				0 = 禁止, 1 = 使能;
						7  P8端口变化通知				0 = 禁止, 1 = 使能;
						8  PTP 运动到位 通知			0 = 禁止, 1 = 使能;
						9  电机发生堵转 通知			 0 = 禁止, 1 = 使能;
						10 PVT 数据缓冲区水位全干 通知   0 = 禁止, 1 = 使能;
						11 PVT 数据缓冲区水位很低 通知   0 = 禁止, 1 = 使能;
						255 整体查询/设置 (即 .uiValue)
		TxData			Notifies the content of the enable parameter
						通知使能参数内容
		pRxData			Set notification result feedback
						设置通知使能结果反馈
return: 0				Set notification function  Success
						设置通知使能 成功
		!0			Set notification function  Failure 
						设置通知使能 失败
Note  : Set notification function 
		设置通知使能
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetInformEnable(uint GtwyHandle, uint CANid, uint Index, uint TxData, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetInformEnable
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			index			Notify the enable parameter index 		The value can only be the following
						index									TxData
						0 P1 Port change notification 					0 = disable, 1 = Enable.
						1 P2 port change notification 					0 = disable, 1 = Enable.
						2 P3 port change notification  					0 = disable, 1 = Enable.
						3 P4 Port change notification 					0 = disable, 1 = Enable.
						4 P5 Port change notification 					0 = Disable, 1 = Enable.
						5 P6 Port change notification 					0 = Disable, 1 = Enable.
						6 P7 Port change notification					0 = Disable, 1 = Enable.
						7 P8 Port change notification 					0 = disable, 1 = Enable.
						8 Notification of PTP movement in place 		0 = disable, 1 = Enable;
						9 Motor gridlock notification 					0 = prohibit, 1 = enable;
						10 PVT data buffer water Dry notification 		0 = Disable, 1 = Enable.
						11 When the PVT data buffer water level is low	0 = disable, 1 = Enable.
						255 Overall query/Settings (i.e.uiValue)

						通知使能参数下标		只能是如下参数:
						index							内容
						0  P1端口变化通知				0 = 禁止, 1 = 使能;
						1  P2端口变化通知				0 = 禁止, 1 = 使能;
						2  P3端口变化通知				0 = 禁止, 1 = 使能;
						3  P4端口变化通知				0 = 禁止, 1 = 使能;
						4  P5端口变化通知				0 = 禁止, 1 = 使能;
						5  P6端口变化通知				0 = 禁止, 1 = 使能;
						6  P7端口变化通知				0 = 禁止, 1 = 使能;
						7  P8端口变化通知				0 = 禁止, 1 = 使能;
						8  PTP 运动到位 通知			0 = 禁止, 1 = 使能;
						9  电机发生堵转 通知			0 = 禁止, 1 = 使能;
						10 PVT 数据缓冲区水位全干 通知  0 = 禁止, 1 = 使能;
						11 PVT 数据缓冲区水位很低 通知  0 = 禁止, 1 = 使能;
						255 整体查询/设置 (即 .uiValue)
		pRxData			The content of the notification enable parameter
						对应通知使能参数的内容
return: 0				Query notification Setting Success
						查询通知使能 成功
		!0			Query notification Setting Failure 
						查询通知使能 失败
Note  : Query notification Setting
		查询通知使能
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkGetInformEnable(uint GtwyHandle, uint CANid, uint Index, uint* pRxData);
#pragma}

// [Motion & I/O]
;
#pragma "[Motion Control] Static" {

//[*] Driver parameter (index: configuration subitem. The value can be:
//[*] 驱动参数 (index:配置子项，取值如下)
#define MTS_MCS_IDX 0 // 0: subdivide 细分[1.2.4.8.16]
#define MTS_CUR_IDX 1 // 1: Electric current 电流[10...80]
#define MTS_PSV_IDX 2 // 2  Standby throttling 待机节流[0...100(%)]
#define MTS_ENA_IDX 3 // 3: Automatic power-on delay 上电自动使能延时[0...60000(ms)]
#define MTS_CUC_IDX 4
#define MTS_BRK_IDX 5 // 5: Brake control 刹车控制
/****************************************************************************
Name  : SdkSetMotorConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Drive parameter index
						驱动参数下标
		data			Driver parameter content
						驱动参数内容
		pRxData			Feedback on the result of setting drive parameters
						设置驱动参数结果反馈
return: 0				Set driver parameters Success
						设置驱动参数 成功
		!0			Set driver parameters Failure 
						设置驱动参数 失败
Note  : Set driver parameters
		设置驱动参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetMotorConfig(uint GtwyHandle, uint CANid, uint Index, uint Data, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetMotorConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Drive parameter index The value can only be the following
				
						0               subdivide  [1.2.4.8.16]
						1				Electric current  [10...80]
						2				Standby throttling [0...100(%)]
						3				Automatic power-on delay [0...60000(ms)]
						4				？？
						5				Brake control 

						驱动参数下标		只能是如下参数:
						0                   细分[1.2.4.8.16]
						1					电流[10...80]
						2					待机节流[0...100(%)]
						3					上电自动使能延时[0...60000(ms)]
						4					？？
						5					刹车控制

		pRxData			Driver parameter content
						驱动参数内容
return: 0				Query driver parameter Success
						查询驱动参数 成功
		!0			Query driver parameter Failure 
						查询驱动参数 失败
Note  : Query driver parameter
		查询驱动参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetMotorConfig(uint GtwyHandle, uint CANid, uint Index, uint* pRxData);

/****************************************************************************
Name  : SdkSetAcceleration
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		data			Acceleration
		pRxData			Result feedback
return: 0				Set acceleration Success
						设置加速度 成功
		!0			Set acceleration Failure 
						设置加速度 失败
Note  : Set acceleration
		设置加速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetAcceleration(uint GtwyHandle, uint CANid, uint Data, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetAcceleration
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Acceleration
return: 0				Query acceleration Success
						查询加速度 成功
		!0			Query acceleration Failure 
						查询加速度 失败
Note  : Query acceleration
		查询加速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetAcceleration(uint GtwyHandle, uint CANid, uint* pRxData);

/****************************************************************************
Name  : SdkSetDeceleration
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		data			Deceleration
		pRxData			Result feedback
return: 0				Set deceleration Success
						设置减速度 成功
		!0			Set deceleration Failure 
						设置减速度 失败
Note  : Set deceleration
		设置减速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetDeceleration(uint GtwyHandle, uint CANid, uint Data, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetDeceleration
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		piDeceleration	Deceleration
						减速度
return: 0				Query deceleration Success
						查询减速度 成功
		!0			Query deceleration Failure 
						查询减速度 失败
Note  : Query deceleration
		查询减速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetDeceleration(uint GtwyHandle, uint CANid, uint* piDeceleration);

/****************************************************************************
Name  : SdkSetCutInSpeed
Func  :	Set cut-in speed (设置切入速度)
Param : GtwyHandle		Gateway handle
						 (网关句柄)
		CANid			Device’s ID
						站点
		data			Cutting speed
						切入速度
		pRxData			Result feedback
						设置切入速度结果反馈
Return: Zero			Set cutting speed Success
						设置切入速度 成功
		None Zero	    Set cutting speed Failure 
						设置切入速度 失败
Note  : Set cutting speed
		查询切入速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetCutInSpeed(uint GtwyHandle, uint CANid, uint Data, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetCutInSpeed
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Cutting speed
						切入速度
return: 0				Query Cutting speed Success
						查询切入速度 成功
		!0			Query Cutting speed Failure 
						查询切入速度 失败
Note  : Query Cutting speed
		查询切入速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetCutInSpeed(uint GtwyHandle, uint CANid, uint* pRxData);

/****************************************************************************
Name  : SdkSetStopDeceleration
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		data			Emergency stop deceleration value
						急停减速度
		pRxData			Result feedback
						设置急停减速度结果反馈
return: 0				Success
		!0			Failure 
Note  : Set Emergency stop deceleration value
		设置急停减速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetStopDeceleration(uint GtwyHandle, uint CANid, uint Data, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetStopDeceleration
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Emergency stop deceleration value 
						急停减速度
return: 0				Success
		!0			Failure 
Note  : Query Emergency stop deceleration value
		查询急停减速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetStopDeceleration(uint GtwyHandle, uint CANid, uint* pRxData);

//[encoder parameter object] index uses the following parameters
//[编码器参数对象] index 使用如下参数
#define QES_QER_IDX 0 
// Number of encoder lines (after x4 = number of pulses per turn) range [10...16000]
// 编码器线数（x4后=每圈脉冲数）范围[10...16000] 
#define QES_PQE_IDX 1 
// Gridlock tolerance (must >= 100)
// 堵转容差(必须 >= 100)
#define QES_AER_IDX 2 
//// Absolute encoder number per turn
// 绝对编码器单圈位数
#define QES_ABW_IDX 3 
// Absolute encoder battery status: =1:OK, =0: low charge
// 绝对编码器电池状态: =1:OK，=0:低电量
#define QES_QEX_IDX 4 
// INC Number of encoder lines (actual value, such as magnetic encoder 90 lines)
// INC 编码器线数（实际值，比如磁编码器 90线）
#define QES_SYN_IDX 5 
// Clear residual position errors, usually set after stopping
// 清除残留位置误差，一般在停止后设置

/****************************************************************************
Name  : SdkSetEncoderConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Parameter index number
						编码器参数下标
		data			Parameter value
						编码器参数内容
		pRxData			Result feedback
						设置编码器参数结果反馈
return: 0				Success
						设置编码器参数 成功
		!0			Failure 
						设置编码器参数 失败
Note  : Set encoder parameters
		设置编码器参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetEncoderConfig(uint GtwyHandle, uint CANid, uint Index, uint Data, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetEncoderConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Parameter index number    只能是如下参数
						0					Number of encoder lines (after x4 = number of pulses per revolution)
						1					stall tolerance(must >= 100)
						2					Number of single revolutions for the encoder
						3					Absolute encoder battery status (0: low battery, 1: OK)
						4					INC encoder line number (actual value, such as magnetic encoder 90 lines)
						5					清除残留位置误差，一般在停止后设置
		
						编码器参数下标		只能是如下参数:
						0					编码器线数
						1					堵转容差(必须 >= 100)
						2					绝对编码器单圈位数
						3					绝对编码器电池状态: =1:OK，=0:低电量
						4					INC 编码器线数（实际值，比如磁编码器 90线）
						5					清除残留位置误差，一般在停止后设置
		pRxData			编码器参数内容
return: 0				Success
						查询编码器参数 成功
		!0			Failure 
						查询编码器参数 失败
Note  : Query encoder parameters
		查询编码器参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetEncoderConfig(uint GtwyHandle, uint CANid, uint Index, uint* pRxData);
/****************************************************************************
Name  : SdkCalibrateEncoder
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Parameter index number
						编码器参数下标
		data			Parameter value
						编码器参数内容
		pRxData			Result feedback
						设置编码器参数结果反馈
return: 0				Success
						设置编码器参数 成功
		!0			    Failure
						设置编码器参数 失败
Note  : Calibrate MegEncoder
		校准磁编码器
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkCalibrateEncoder(uint GtwyHandle, uint CANid);
/****************************************************************************
Name  : SdkGetAuxPosition
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Auxiliary position information (when closed loop control mode, return the open loop pulse value; In open loop control mode, return the encoder value;)
						辅助位置信息(当闭环控制模式时，返回开环脉冲值；开环控制模式时，返回编码器值；)
return: 0				Success
						查询辅助位置信息 成功
		!0			Failure 
						查询辅助位置信息 失败
Note  : Query the auxiliary location
		查询辅助位置信息
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetAuxPosition(uint GtwyHandle, uint CANid, int* pRxData);

/****************************************************************************
Name  : SdkSetBacklashComp
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		data			Backlash compensation value
						回程间隙补偿值
		pRxData			Result feedback
						设置回程间隙补偿值结果反馈
return: 0				Success
						设置回程间隙补偿值 成功
		!0			Failure 
						设置回程间隙补偿值 失败
Note  : Set backlash compensation value
		设置回程间隙补偿值
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetBacklashComp(uint GtwyHandle, uint CANid, uint Data, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetBacklashComp
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Backlash compensation value
						回程间隙补偿值
return: 0				Success
						查询回程间隙补偿值 成功
		!0			Failure 
						查询回程间隙补偿值 失败
Note  : Query backlash compensation value
		查询回程间隙补偿值
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetBacklashComp(uint GtwyHandle, uint CANid, uint* pRxData);

/****************************************************************************
Name  : SdkSetMotionLimits
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pData			Motion limit value (structure MOTION_LIMIT_OBJ is defined in uirSDKcommon.h)
						运动限位值(结构体MOTION_LIMIT_OBJ定义详见uirSDKcommon.h)
		pRxData			Result feedback
						设置运动限位结果反馈(结构体MOTION_LIMIT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						设置运动限位 成功
		!0			Failure 
						设置运动限位 失败
Note  : Set Motion limit value 
		设置运动限位
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetMotionLimits(uint GtwyHandle, uint CANid, MOTION_LIMIT_OBJ* pData, MOTION_LIMIT_OBJ* pRxData = NULL);
/****************************************************************************
Name  : SdkGetMotionLimits
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Motion limit value (structure MOTION_LIMIT_OBJ is defined in uirSDKcommon.h)
						运动限位值(结构体MOTION_LIMIT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						查询运动限位 成功
		!0			Failure 
						查询运动限位 失败
Note  : Query motion limit value 
		查询运动限位
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetMotionLimits(uint GtwyHandle, uint CANid, MOTION_LIMIT_OBJ* pRxData);
/****************************************************************************
Name  : SdkRstMotionLimits
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
return: 0				Success
						复位行程限位 成功
		!0			Failure 
						复位行程限位 失败
Note  : Reset the travel limit
		复位行程限位
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkRstMotionLimits(uint GtwyHandle, uint CANid, MOTION_LIMIT_OBJ* pRxData = NULL);

/****************************************************************************
Name  : SdkSetLimitCheck
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		data			Enable/disable (0: disable / 1: enable)
						使能/禁止(0:禁止 / 1:使能)
		pRxData			Result feedback
						设置限位检查使能/禁止结果反馈
return: 0				Success
						设置限位检查使能/禁止 成功
		!0			Failure 
						设置限位检查使能/禁止 失败
Note  : Set the limit check enable/disable
		设置限位检查使能/禁止
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetLimitCheck(uint GtwyHandle, uint CANid, uint Data, uint* pRxData = NULL);
/****************************************************************************
Name  : SdkGetLimitCheck
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Result feedback
						使能/禁止(0:禁止 / 1:使能)
return: 0				Success
						查询限位检查使能/禁止状态 成功
		!0			Failure 
						查询限位检查使能/禁止状态 失败
Note  : Query whether limit check is enabled or disabled
		查询限位检查使能/禁止状态
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetLimitCheck(uint GtwyHandle, uint CANid, uint* pRxData);

// [Desired Value] Index defined as following (目标值 index 定义如下)
#define DVR_MOD_IDX 0 // Control Mode: JV/PTP/PVT (控制模式: JV / PTP / PVT)
#define DVR_CUR_IDX 1 // Working Current   (工作电流)
#define DVR_SPD_IDX 2 // Speed             (目标速度)
#define DVR_PRM_IDX 3 // Position Relative (位移)
#define DVR_PAM_IDX 4 // Position Absolute (位置)
#define DVR_TIS_IDX 5 
/****************************************************************************
Name  : SdkGetDesiredValue
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		index			Parameter index
						参数下标
		pRxData			Parameter target value
						参数目标值
return: 0				Success
						读取参数目标值 成功
		!0			Failure 
						读取参数目标值 失败
Note  : Reads the parameter target value
		读取参数目标值
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetDesiredValue(uint GtwyHandle, uint CANid, uint Index, void* pRxData);
#pragma}
;
#pragma "[Motion Control] Dynamic" {
/****************************************************************************
Name  : SdkSetMotorOn
Param : GtwyHandle		网关句柄
		CANid			站点
		bMotorOn		使能 / 脱机（0:脱机 / 1:使能）
		pbEnableOut		设置使能 / 脱机结果反馈
return: 0				设置使能 / 脱机 成功
		!0				设置使能 / 脱机 失败
Note  : 设置使能 / 脱机
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetMotorOn(uint GtwyHandle, uint CANid, uint bMotorOn, uint *pbEnable = NULL);

/****************************************************************************
Name  : SdkGetMotorOn
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pbEnable		Result feedback
						使能 / 脱机状态（0:脱机 / 1:使能）
return: 0				Success
						获取使能 / 脱机状态 成功
		!0			Failure 
						获取使能 / 脱机状态 失败
Note  : Gets the enabled/offline status
		获取使能 / 脱机状态
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetMotorOn(uint GtwyHandle, uint CANid, uint *pbEnable);

/****************************************************************************
Name  : SdkSetBeginMxn
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Result feedback
						设置开始运动结果反馈
return: 0				Success
						设置开始运动 成功
		!0			Failure 
						设置开始运动 失败
Note  : Set begin motion
		设置开始运动
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetBeginMxn(uint GtwyHandle, uint CANid, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkSetBeginMxnTime
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Result feedback
						设置开始运动结果反馈
return: 0				Success
						设置开始运动 成功
		!0			    Failure
						设置开始运动 失败
Note  : Set the time to begin motion
		设置开始运动的时间
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetBeginMxnTime(uint GtwyHandle, uint CANid, uint time, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkSetStopMxn
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Result feedback
						设置紧急停止结果反馈
return: 0				Success
						设置紧急停止 成功
		!0			Failure 
						设置紧急停止 失败
Note  : Set emergency stop
		设置紧急停止
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetStopMxn(uint GtwyHandle, uint CANid, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkSetOrigin
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Result feedback
						设置当前位置为原点结果反馈
return: 0				Success
						设置当前位置为原点 成功
		!0			Failure 
						设置当前位置为原点 失败
Note  : Set the current position to the origin (i.e., PA = 0)
		设置当前位置为原点(即，PA = 0) 
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetOrigin(uint GtwyHandle, uint CANid);

//.............[速度控制].............
/****************************************************************************
Name  : SdkSetJogMxn
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Velo			Target speed
						目标速度
		pRxVelo			Result feedback
						设定速度模式 + 目标速度结果反馈（typedef int int;）
return: 0				Success
						设定速度模式 + 目标速度 成功
		!0			Failure 
						设定速度模式 + 目标速度 失败
Note  : Set Speed mode + target speed
		设定速度模式 + 目标速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetJogMxn(uint GtwyHandle, uint CANid, int Velo, int* pRxVelo = NULL);

/****************************************************************************
Name  : SdkGetJogMxn
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxVelo			Result feedback
						当前速度（typedef int int;）
return: 0				Success
						查询当前速度 成功
		!0			Failure 
						查询当前速度 失败
Note  : Query current speed
		查询当前速度
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetJogMxn(uint GtwyHandle, uint CANid, int* pRxVelo);

//.............[点位控制].............

/****************************************************************************
Name  : SdkSetPtpMxnA
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Velo			Target speed（typedef int int;）
						目标速度（typedef int int;）
		Pa				Target position（typedef int int;）
						目标位置（typedef int int;）
		pRxVelo			Velocity feedback typedef int int;）
						速度反馈（typedef int int;）
		pRxPa			Position feedback typedef int int;）
						位置反馈（typedef int int;）
return: 0				Success
						设定位置运动 成功
		!0			Failure 
						设定位置运动 失败
Note  : Set position motion (= Set both target speed and target position)
		设定位置运动(= 同时设置 目标速度 和 目标位置)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetPtpMxnA(uint GtwyHandle, uint CANid, int Velo, int Pa, int* pRxVelo = NULL, int* pRxPa = NULL);

/****************************************************************************
Name  : SdkGetPtpMxnA
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxVelo			Current speed
						当前速度（typedef int int;）
		pRxPa			Current position
						当前位置（typedef int int;）
return: 0				Success
						查询当前速度 和 当前位置 成功
		!0			Failure 
						查询当前速度 和 当前位置 失败
Note  : Query the current speed and location
		查询当前速度 和 当前位置
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetPtpMxnA(uint GtwyHandle, uint CANid, int* pRxVelo, int* pRxPa);

/****************************************************************************
Name  : SdkSetPtpMxnA
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Velo			Target speed （typedef int int;）
						目标速度（typedef int int;）
		Pr				Target displacement （typedef int int;）
						目标位移（typedef int int;）
		pRxVelo			Velocity feedback （typedef int int;）
						速度反馈（typedef int int;）
		pRxPr			Displacement feedback（typedef int int;）
						位移反馈（typedef int int;）
return: 0				Success
						设定位移运动 成功
		!0			Failure 
						设定位移运动 失败
Note  : Set displacement motion (= Set both target speed and target displacement)
		设定位移运动 (= 同时设置 目标速度 和 目标位移)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetPtpMxnR(uint GtwyHandle, uint CANid, int Velo, int Pr, int* pRxVelo = NULL, int* pRxPr = NULL);

/****************************************************************************
Name  : SdkGetPtpMxnR
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxVelo			Current speed （typedef int int;）
						当前速度（typedef int int;）
		pRxPr			Current displacement （typedef int int;）
						当前位移（typedef int int;）
return: 0				Success
						查询当前速度 和 当前位移 成功
		!0			Failure 
						查询当前速度 和 当前位移 失败
Note  : Query the current speed and displacement
		查询当前速度 和 当前位移
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetPtpMxnR(uint GtwyHandle, uint CANid, int* pRxVelo, int* pRxPr);

//.............[曲线插补].............

/****************************************************************************
Name  : SdkSetPvtRstBuf
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
return: 0				Success
						复位PVT 成功
		!0			Failure 
						复位PVT 失败
Note  : 
Reset PVT: PVT track point cache + water level + execution point; Note: It is recommended to execute before sending the new FIFO sequence (prerequiDevice’s ID: must exit the PVT mode first, that is, wait for the water level to dry or send the SD command)
复位PVT: PVT 轨迹点缓存区 + 水位 + 执行点;注：建议发送新的 FIFO 序列前执行 ( 前提：必须先退出PVT模式，即等水位干或者发送 SD 指令 )
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetPvtRstBuf(uint GtwyHandle, uint CANid);

/****************************************************************************
Name  : SdkSetPvtIODevice
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		IODcanid		CAN ID
		pRxIODcanid		CAN ID反馈
return: 0				Success
						设置PVT 序列使用的 I/O 输出设备的地址（CAN-ID） 成功
		!0			Failure 
						设置PVT 序列使用的 I/O 输出设备的地址（CAN-ID） 失败
Note  : Set the address of the I/O output device used by the PVT sequence (CAN-ID) 
		设置PVT 序列使用的 I/O 输出设备的地址（CAN-ID）
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetPvtIODevice(uint GtwyHandle, uint CANid, uchar IODcanid, uchar* pRxIODcanid);

/****************************************************************************
Name  : SdkGetPvtIODevice
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxIODcanid		CAN ID
return: 0				Success
						查询PVT 序列使用的 I/O 输出设备的地址（CAN-ID） 成功
		!0			Failure 
						查询PVT 序列使用的 I/O 输出设备的地址（CAN-ID） 失败
Note  : Query the address of the I/O output device used by the PVT sequence (CAN-ID)
		查询PVT 序列使用的 I/O 输出设备的地址（CAN-ID）
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkGetPvtIODevice(uint GtwyHandle, uint CANid, uchar* pRxIODcanid);

/****************************************************************************
Name  : SdkSetPvtMParam
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pData			Parameters (Structure PVT_PARAM_OBJ is defined in uirSDKcommon.h)
						参数(结构体PVT_PARAM_OBJ定义详见uirSDKcommon.h)
		pRxData			Parameter feedback (structure PVT_PARAM_OBJ is defined in uirSDKcommon.h)
						参数反馈(结构体PVT_PARAM_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						设置PVT 参数 成功
		!0			Failure 
						设置PVT 参数 失败
Note  : Set PVT parameters
		设置PVT 参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetPvtMParam(uint GtwyHandle, uint CANid, PVT_PARAM_OBJ* pData, PVT_PARAM_OBJ* pRxData);

/****************************************************************************
Name  : SdkGetPvtMParam
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Parameters (Structure PVT_PARAM_OBJ is defined in uirSDKcommon.h)
						参数(结构体PVT_PARAM_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						查询PVT 参数 成功
		!0			Failure 
						查询PVT 参数 失败
Note  : Query PVT parameters
		查询PVT 参数
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkGetPvtMParam(uint GtwyHandle, uint CANid, PVT_PARAM_OBJ* pRxData);

/****************************************************************************
Name  : SdkSetPvtBgnIdx
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		data			Start motion label
						运动起点标号
		pRxData			Motion start label feedback value
						运动起点标号反馈值
return: 0				Success
						设定 PVT 模式 + 运动起点标号 成功
		!0			Failure 
						设定 PVT 模式 + 运动起点标号 失败
Note  : Set PVT mode + motion start label
		设定 PVT 模式 + 运动起点标号
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetPvtBgnIdx(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);

/****************************************************************************
Name  : SdkGetPvtExeIdx
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Execute point label
						执行点标号
return: 0				Success
						读取当前执行点标号 成功
		!0			Failure 
						读取当前执行点标号 失败
Note  : Reads the current execution point label
		读取当前执行点标号
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkGetPvtExeIdx(uint GtwyHandle, uint CANid, uint* pRxData);

/****************************************************************************
Name  : SdkSetPvtPoint24
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Index			Track point index
						轨迹点下标
		pData			Track point data (structure PVT_POINT_OBJ is defined in uirSDKcommon.h)
						轨迹点数据(结构体PVT_POINT_OBJ定义详见uirSDKcommon.h)
		pRxData			Track point data feedback value (structure PVT_POINT_OBJ is defined in uirSDKcommon.h)
						轨迹点数据反馈值(结构体PVT_POINT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						设置PVT 轨迹点 成功
		!0			Failure 
						设置PVT 轨迹点 失败
Note  : Set PVT track points (24-bit length data)
		设置PVT 轨迹点(24位长度数据) 
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetPvtPoint24(uint GtwyHandle, uint CANid, uint Index, PVT_POINT_OBJ* pData, PVT_POINT_OBJ* pRxData);

/****************************************************************************
Name  : SdkSetPvtPoint32
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Index			Track point index
						轨迹点下标
		pData			Track point data (structure PVT_POINT_OBJ is defined in uirSDKcommon.h)
						轨迹点数据(结构体PVT_POINT_OBJ定义详见uirSDKcommon.h)
		pRxData			Track point data feedback value (structure PVT_POINT_OBJ is defined in uirSDKcommon.h)
						轨迹点数据反馈值(结构体PVT_POINT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						设置PVT 轨迹点 成功
		!0			Failure 
						设置PVT 轨迹点 失败
Note  : Set PVT track points (32-bit length data)
		设置PVT 轨迹点 (32位长度数据)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetPvtPoint32(uint GtwyHandle, uint CANid, uint Index, PVT_POINT_OBJ* pData, PVT_POINT_OBJ* pRxData);

/****************************************************************************
Name  : SdkSetPvtPoint32
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Index			Track point index
						轨迹点下标
		pRxData			Track point data (structure PVT_POINT_OBJ is defined in uirSDKcommon.h)
						轨迹点数据(结构体PVT_POINT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						查询PVT 轨迹点 成功
		!0			Failure 
						查询PVT 轨迹点 失败
Note  : Query PVT track points (32-bit length data)
		查询PVT 轨迹点 (32位长度数据)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkGetPvtPoint32(uint GtwyHandle, uint CANid, uint Index, PVT_POINT_OBJ* pRxData);

/****************************************************************************
Name  : SdkSetPvtKcode32
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pData			Track point data (structure PVT_KCODE_OBJ is defined in uirSDKcommon.h)
						轨迹点数据(结构体PVT_KCODE_OBJ定义详见uirSDKcommon.h)
		pRxData			Track point data feedback (structure PVT_KCODE_OBJ is defined in uirSDKcommon.h)
						轨迹点数据反馈值(结构体PVT_KCODE_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						设置PVT 轨迹点 成功
		!0			Failure 
						设置PVT 轨迹点 失败
Note  : Set PVT track points (K-code)
		设置PVT 轨迹点 (K-代码)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetPvtKcode32(uint GtwyHandle, uint CANid, PVT_KCODE_OBJ* pData, PVT_KCODE_OBJ* pRxData);
#pragma}
;
#pragma "[I/O Control   ]" {
//[Port Function value] CfgCode Use the following parameters
//[端口功能值] CfgCode 使用如下参数
#define IOC_DOU_IDX 0 
// Digital output
// 数字输出
#define IOC_DIN_IDX 1 
// Digital input
// 数字输入
#define IOC_AIN_IDX 2 
// Analog input
// 模拟输入
#define IOC_QEI_IDX 3 // QEI
#define IOC_PWM_IDX 3 // PWM ？？
/****************************************************************************
Name  : SdkSetIOConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		PortIx			IO port number
						IO端口号
		CfgCode			Functional value
						功能值
		pRxData			Function data feedback value
						功能数据反馈值
return: 0				Success
						设置I/O 端口功能 成功
		!0			Failure 
						设置I/O 端口功能 失败
Note  : Set the I/O port function
		设置I/O 端口功能
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetIOConfig(uint GtwyHandle, uint CANid, uint PortIx, uint CfgCode, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkGetIOConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		PortIx			IO port number
						IO端口号
		pRxData			Functional value
						功能值
return: 0				Success
						查询I/O 端口功能 成功
		!0			Failure 
						查询I/O 端口功能 失败
Note  : Query the I/O port function
		查询I/O 端口功能
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetIOConfig(uint GtwyHandle, uint CANid, uint PortIx, uint* pRxData);


/****************************************************************************
Name  : SdkSetTriggerMode
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		PortIx			IO port number
						IO端口号
		TrigDelay		Delay time (unit: ms)
						延时时间（单位：ms）
		pRxData			Function data feedback value
						功能数据反馈值
return: 0				Success
						设置数字输入触发延时 成功
		!0			Failure 
						设置数字输入触发延时 失败
Note  : Set the digital input trigger delay (anti-shake action, such as TrigDelay &gt; 60000, considered as a single trigger, the original trigger action will be cleared)
		设置数字输入触发延时(消抖作用, 如 TrigDelay > 60000, 认作单次触发, 原触发动作会被清除)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetTriggerMode(uint GtwyHandle, uint CANid, uint PortIx, uint TrigTime, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkGetTriggerMode
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		PortIx			IO port number
						IO端口号
		pRxData			Delay time (unit: ms)
						延时时间（单位：ms）
return: 0				Success
						查询数字输入触发延时 成功
		!0			Failure 
						查询数字输入触发延时 失败
Note  : Query digit input trigger delay
		查询数字输入触发延时
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetTriggerMode(uint GtwyHandle, uint CANid, uint PortIx, uint* pRxData);

/****************************************************************************
Name  : SdkGetAnalogInput
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		PortIx			IO port number
						IO端口号
		pRxData			Analog quantity
						模拟量
return: 0				Success
						查询模拟输入值 成功
		!0			Failure 
						查询模拟输入值 失败
Note  : Query simulated input values
		查询模拟输入值
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetAnalogInput(uint GtwyHandle, uint CANid, uint PortIx, int* pRxData);

/****************************************************************************
Name  : SdkSetDigitalOutput
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pDOValue		IO port output value (for the definition of DIGITAL_OUTPUT_OBJ structure, see uirSDKcommon.h)
						IO端口输出值(结构体DIGITAL_OUTPUT_OBJ定义详见uirSDKcommon.h)
		pRxData			IO port output value feedback (structure DIGITAL_OUTPUT_OBJ is defined in uirSDKcommon.h)
						IO端口输出值反馈(结构体DIGITAL_OUTPUT_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						设置数字输出 成功
		!0			Failure 
						设置数字输出 失败
Note  : Set digital output
		设置数字输出
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetDigitalOutput(uint GtwyHandle, uint CANid, DIGITAL_OUTPUT_OBJ* pDOValue, UIM_DIO_OBJ* pRxData = NULL);

/****************************************************************************
Name  : SdkGetDIOport
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Digital port level (UIM_DIO_OBJ structure is defined in uirSDKcommon.h)
						数字端口电平(结构体UIM_DIO_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						查询数字端口电平 成功
		!0			Failure 
						查询数字端口电平 失败
Note  : Query digital port levels (both input and output)
		查询数字端口电平(包括输入端口和输出端口)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetDIOport(uint GtwyHandle, uint CANid, UIM_DIO_OBJ* pRxData);

//[Input trigger action parameters] Category (Catagory) indicates that the parameter is used in the following situations:
//[输入触发动作参数] 分类(Catagory)表示被设参数作用于以下场合:
#define IOL_CAT_GENERAL 0 
// 0: indicates the parameter applied by the received instruction
// 0: 收到指令应用的参数 
#define IOL_CAT_STALLED 1 
// 1: Application after motor lock-turn detection
// 1: 电机堵转 检出后应用
#define IOL_CAT_IN1RISE 2 
// 2: Apply after IN1 rising edge is detected
// 2: IN1上升沿检出后应用  
#define IOL_CAT_IN1FALL 3 
// 3: Apply after IN1 falling edge is detected
// 3: IN1下降沿检出后应用
#define IOL_CAT_IN2RISE 4 
// 4: Apply after IN2 rising edge is detected
// 4: IN2上升沿检出后应用
#define IOL_CAT_IN2FALL 5 
// 5: Apply after IN2 falling edge is detected
// 5: IN2下降沿检出后应用
#define IOL_CAT_IN3RISE 6 
// 6: Apply after IN3 rising edge is detected
// 6: IN3上升沿检出后应用
#define IOL_CAT_IN3FALL 7 
// 7: Apply after IN3 falling edge is detected
// 7: IN3下降沿检出后应用
#define IOL_CAT_IN4RISE 8 
// 8: Apply after IN4 rising edge is detected
// 8: IN4上升沿检出后应用
#define IOL_CAT_IN4FALL 9 
// 9: Apply after IN4 falling edge is detected
// 9: IN4下降沿检出后应用
/****************************************************************************
Name  : SdkSetInputLogicAC
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Catagory		Trigger classification
						触发分类
		data			Acceleration
						加速度
		pRxData			Acceleration feedback value
						加速度反馈值
return: 0				Success
						设置输入触发动作.AC (加速度) 成功
		!0			Failure 
						设置输入触发动作.AC (加速度) 失败
Note  : Set the input trigger action. AC (Acceleration)
		设置输入触发动作.AC (加速度)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetInputLogicAC(uint GtwyHandle, uint CANid, uint Catagory, uint Data, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkGetInputLogicAC
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Catagory		Trigger classification	
						触发分类
		pRxData			Acceleration
						加速度
return: 0				Success
						查询输入触发动作.AC (加速度) 成功
		!0			Failure 
						查询输入触发动作.AC (加速度) 失败
Note  : Query input triggers an action. AC (Acceleration)
		查询输入触发动作.AC (加速度)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetInputLogicAC(uint GtwyHandle, uint CANid, uint Catagory, uint* pRxData);

/****************************************************************************
Name  : SdkSetInputLogicDC
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Catagory		Trigger classification	
						触发分类
		data			Deceleration
						减速度
		pRxData			Deceleration feedback value
						减速度反馈值
return: 0				Success
						设置输入触发动作.DC (减速度) 成功
		!0			Failure 
						设置输入触发动作.DC (减速度) 失败
Note  :Set the input trigger action. DC (Deceleration)
	   设置输入触发动作.DC (减速度)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetInputLogicDC(uint GtwyHandle, uint CANid, uint Catagory, uint Data, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkGetInputLogicDC
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Catagory		Trigger classification	
						触发分类
		piDeceleration	Deceleration
						减速度
return: 0				Success
						查询输入触发动作.DC (减速度) 成功
		!0			Failure 
						查询输入触发动作.DC (减速度) 失败
Note  : Query input triggers an action. DC (Deceleration)
		查询输入触发动作.DC (减速度)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetInputLogicDC(uint GtwyHandle, uint CANid, uint Catagory, uint* piDeceleration);

/****************************************************************************
Name  : SdkSetInputLogicSP
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Catagory		Trigger classification	
						触发分类
		PTPSpeed		Speed
						速度
		pRxPTPSpd		Velocity feedback value
						速度反馈值
return: 0				Success
						设置输入触发动作.SP (速度) 成功
		!0			Failure 
						设置输入触发动作.SP (速度) 失败
Note  : Set the input trigger action. SP (Speed)
		设置输入触发动作.SP (速度)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetInputLogicSP(uint GtwyHandle, uint CANid, uint Catagory, int PTPSpeed, int* pRxPTPSpd = NULL);

/****************************************************************************
Name  : SdkGetInputLogicSP
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Catagory		Trigger classification	
						触发分类
		pRxPTPSpd		Speed
						速度
return: 0				Success
						查询输入触发动作.SP (速度) 成功
		!0			Failure 
						查询输入触发动作.SP (速度) 失败
Note  : Query input triggers an action. SP (Speed)
		查询输入触发动作.SP (速度)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetInputLogicSP(uint GtwyHandle, uint CANid, uint Catagory, int* pRxPTPSpd);

/****************************************************************************
Name  : SdkSetInputLogicPR
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Catagory		Trigger classification	
						触发分类
		data			Displacement
						位移
		pRxData			Displacement feedback value
						位移反馈值
return: 0				Success
						设置输入触发动作.PR (位移) 成功
		!0			Failure 
						设置输入触发动作.PR (位移) 失败
Note  : Set the input trigger action. PR (Displacement)
		设置输入触发动作.PR (位移)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetInputLogicPR(uint GtwyHandle, uint CANid, uint Catagory, int Data, int* pRxData = NULL);

/****************************************************************************
Name  : SdkGetInputLogicPR
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID		
						站点
		Catagory		Trigger classification	
						触发分类
		pRxData			Displacement feedback value
						位移反馈值
return: 0				Success
						查询输入触发动作.PR (位移) 成功
		!0			Failure 
						查询输入触发动作.PR (位移) 失败
Note  : Query input triggers an action. PR (Displacement)
		查询输入触发动作.PR (位移)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetInputLogicPR(uint GtwyHandle, uint CANid, uint Catagory, int* pRxData);

/****************************************************************************
Name  : SdkSetInputLogicPA
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		Catagory		Trigger classification	
						触发分类
		data			Position
						位置
		pRxData			Position feedback value
						位置反馈值
return: 0				Success
						设置输入触发动作.PA (位置) 成功
		!0			Failure 
						设置输入触发动作.PA (位置) 失败
Note  : Set the input trigger action. PA (Location)
		设置输入触发动作.PA (位置)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetInputLogicPA(uint GtwyHandle, uint CANid, uint Catagory, int Data, int* pRxData = NULL);

/****************************************************************************
Name  : SdkGetInputLogicPA
Param : GtwyHandle		Gateway handle
						网关句柄
		Catagory		Trigger classification	
						触发分类
		data			Position
						位置
return: 0				Success
						查询输入触发动作.PA (位置) 成功
		!0			Failure 
						查询输入触发动作.PA (位置) 失败
Note  : Query input triggers an action. PA (Location)
		查询输入触发动作.PA (位置)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetInputLogicPA(uint GtwyHandle, uint CANid, uint Catagory, int* pRxData);

//[Input trigger action] Input Logic (Input trigger default action MxnCode)
//[输入触发动作] Input Logic (输入触发的预设动作 MxnCode)
#define ILG_NULL_0000_IDX 0  
// 00: No action (i.e. prohibited)
// 00: 无动作(即禁止) 
#define ILG_MOFF_0000_IDX 1  
// Offline
// 脱机
#define ILG_JVSD_0000_IDX 2  
// Stop quickly
// 急速停止
#define ILG_JVDC_0000_IDX 3  
// Slow down and stop
// 减速停止
#define ILG_SORG_PTPR_IDX 4  
// Origin + commutation + motion according to displacement abs(PR)
// 原点+换向+按位移abs(PR)运动
#define ILG_SORG_JVSD_IDX 5  
// Origin + quick stop
// 原点+急速停止
#define ILG_SORG_JVDC_IDX 6 
// Origin + Slow down to stop
 // 原点+减速停止
#define ILG_NDIR_JOGV_IDX 7  
// Reversing + Motion according to preset abs(JV)
// 换向+按预设abs(JV)运动
#define ILG_JOGV_0000_IDX 8  
// Move as preset +/-JV
// 按预设 +/-JV 运动
#define ILG_NDIR_PTPR_IDX 9  
// Commutation + Motion according to preset displacement abs(PR)
// 换向+按预设位移abs(PR)运动
#define ILG_PTPR_0000_IDX 10 
// Move according to preset displacement (+/-PR)
// 按预设位移(+/-PR)运动
#define ILG_PTPA_0000_IDX 11 
// Move at preset position (+/-PA)
// 按预设位置(+/-PA)运动
#define ILG_PVTA_0000_IDX 15 
// Execute the PVT movement according to the preset parameters
// 按预设参数执行 PVT 运动


/****************************************************************************
Name  : SdkSetInputLogic
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		PortIx			Trigger port number
						触发端口号
		MxnCode			The default action of Input trigger
						输入触发的预设动作
		pRxData			The default action feedback of Input trigger
						输入触发的预设动作反馈值
return: 0				Success
						设置输入触发动作逻辑 成功
		!0			Failure  
						设置输入触发动作逻辑 失败
Note  : Set the input trigger action logic
		设置输入触发动作逻辑
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetInputLogic(uint GtwyHandle, uint CANid, uint InputEdgeIdx, INPUT_ACT_OBJ MxnCode, INPUT_ACT_OBJ* pRxData = NULL);

/****************************************************************************
Name  : SdkGetInputLogic
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		PortIx			Trigger port number
						触发端口号
		pRxData			The default action of Input trigger
						输入触发的预设动作
return: 0				Success
						查询输入触发动作逻辑 成功
		!0			Failure 
						查询输入触发动作逻辑 失败
Note  : Query the input trigger action logic
		查询输入触发动作逻辑
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetInputLogic(uint GtwyHandle, uint CANid, uint PortIx, INPUT_ACT_OBJ* pRxData);
#pragma}
;
#pragma "[Servo Motor   ]" {

/****************************************************************************
Name  : SdkSetServoParam
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		SrvoData		Servosetting content (primary code + secondary code + data) (structure SERVO_PARAM_OBJ is defined in uirSDKcommon.h)
						伺服设置内容（主码+辅码+数据）(结构体SERVO_PARAM_OBJ定义详见uirSDKcommon.h)
		pRxData			Servoset content feedback value (structure SERVO_PARAM_OBJ is defined in uirSDKcommon.h)
						伺服设置内容反馈值(结构体SERVO_PARAM_OBJ定义详见uirSDKcommon.h)
return: 0				Success
						设置伺服通用设置 成功
		!0			Failure 
						设置伺服通用设置 失败
Note  : Set the servo general Settings
		设置伺服通用设置
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetServoParam(uint GtwyHandle, uint CANid, SERVO_PARAM_OBJ* SrvoData, SERVO_PARAM_OBJ* pRxData = NULL);

/****************************************************************************
Name  : SdkGetServoParam
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			伺服设置内容（主码+辅码+数据,需事先填写主码+辅码）
return: 0				Success
						查询伺服通用设置 成功
		!0			Failure 
						查询伺服通用设置 失败
Note  : Query server general Settings
		查询伺服通用设置
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetServoParam(uint GtwyHandle, uint CANid, SERVO_PARAM_OBJ* pRxData);

//[*] Shortcut function (FuncCode: function code)
//[*] 快捷功能 (FuncCode:功能码）
#define _SV_SET_STF 0xF6 // 刚度设定,    Stiffness
#define _SV_SET_ADP 0xF7 // 自适应功能,  Parameter Auto Adaption
#define _SV_SET_FLT 0xF8 // 指令滤波,    Filter
#define _SV_CLR_ABS 0xF9 // 编码器清零,  Origin
#define _SV_CLR_ALM 0xFA // 清除当前报警,Clear Current Alarm 
#define _SV_GET_ALM 0xFC // 读取当前报警,Get Current Alarm
#define _SV_SET_EEP 0xFD // 固化参数EEP, Store Para->EEPROM
#define _SV_RST_AUT 0xFE // 释放修改权,  reset/release Authentication
#define _SV_GET_AUT 0xFF // 取得修改权,  get Authentication
/****************************************************************************
Name  : SdkSetServoFunc
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		FuncCode		Function code
						功能码
		TxData			Set content
						设置内容
		pRxData			Set the content feedback value
						设置内容反馈值
return: 0				Success
						快捷设置伺服参数 成功
		!0			Failure 
						快捷设置伺服参数 失败
Note  : Set servo parameters quickly
		快捷设置伺服参数
****************************************************************************/
UISIMCANFUNC_API ERRO  STDCALLAPI SdkSetServoFunc(uint GtwyHandle, uint CANid, uint FuncCode, uint TxData, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkGetServoFunc
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		FuncCode		Function code
						功能码
		pRxData			Servo parameter content
						伺服参数内容
return: 0				Success
						快捷读取伺服参数 成功
		!0			Failure 
						快捷读取伺服参数 失败
Note  : Quick reading of servo parameters
		快捷读取伺服参数
****************************************************************************/
UISIMCANFUNC_API ERRO  STDCALLAPI SdkGetServoFunc(uint GtwyHandle, uint CANid, uint FuncCode, uint* pRxData);

/****************************************************************************
Name  : SdkSetTorqueCtrl
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		ActionCode		Action code triggered by the overlimit
						超限触发的动作代码
		Threshold		Maximum torque/rated torque *100%
						最大力矩/额定力矩*100%
		pRxData			Feedback value (ActionCode = RxData.b0; Threshold = RxData.b1;  RxData.b2 = 0;  RxData.b3 = 0;)
						反馈值（ActionCode = RxData.b0; Threshold = RxData.b1; RxData.b2 = 0; RxData.b3 = 0;）
return: 0				Success
						设置力矩控制 成功
		!0			Failure 
						设置力矩控制 失败
Note  : Set torque control
		设置力矩控制
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkSetTorqueCtrl(uint GtwyHandle, uint CANid, uint ActionCode, uint Threshold, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkGetTorqueCtrl
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxData			Torque control value (ActionCode = RxData.b0; Threshold = RxData.b1;  RxData.b2 = 0;  RxData.b3 = 0;)
						力矩控制值（ActionCode = RxData.b0; Threshold = RxData.b1; RxData.b2 = 0; RxData.b3 = 0;）
return: 0				Success
						查询力矩控制 成功
		!0			Failure 
						查询力矩控制 失败
Note  : Query torque control
		查询力矩控制
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI  SdkGetTorqueCtrl(uint GtwyHandle, uint CANid, int* pRxData);

#pragma}
;
#pragma "[Aux Encoder   ] UIM0808/1616 Only" {

/****************************************************************************
Name  : SdkSetEncoderConfig
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		data			Encoder increments count cycles
						编码器增量计数周期
		pRxData			Result feedback
						结果反馈
return: 0				成功
		!0				失败
Note  : Set Aux Encoder Counting Period (ms) + enable/disable(period=0) Encoder
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkSetEncoderPeriod(uint GtwyHandle, uint CANid, uint Period, uint* pRxData = NULL);

/****************************************************************************
Name  : SdkRstEncoderCount
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
return: 0				Success	
						成功
		!0			Failure 
						失败
Note  : Reset Aux Encoder Counter (= 0)
		复位辅助编码器计数器(= 0)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkRstEncoderCount(uint GtwyHandle, uint CANid);

/****************************************************************************
Name  : SdkGetEncoderSpeed
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxCount		Read-out value
						读取值
return: 0				Success
						成功
		!0			Failure 
						失败
Note  : Get Aux Encoder Speed (*pRxData = Counts within 1 second)
		获取辅助编码器速度(*pRxData =计数在1秒内)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetEncoderSpeed(uint GtwyHandle, uint CANid, int* pRxData);

/****************************************************************************
Name  : SdkGetEncoderCount
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxCount		Read-out value
						读取值	
return: 0				Success
						成功
		!0			Failure 
						失败
Note  : Get Aux Encoder Counter
		获得辅助编码器计数器
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetEncoderCount(uint GtwyHandle, uint CANid, int* pRxCount);

/****************************************************************************
Name  : SdkGetEncoderIncrement
Param : GtwyHandle		Gateway handle
						网关句柄
		CANid			Device’s ID
						站点
		pRxCount		Read-out value
						读取值	
return: 0				Success
						成功
		!0			Failure 
						失败
Note  : Get Aux Encoder Count within Period (*pRxData = Counts within Period)
		获取周期内的编码器计数(*pRxData =周期内的计数)
****************************************************************************/
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetEncoderIncrement(uint GtwyHandle, uint CANid, int* pRxData);
#pragma}

// [versatile instruction]
;
#pragma "[Versatile Instruction]" {
/****************************************************************************
Name  : SdkSendMsg
Param : GtwyHandle		Gateway handle
						网关句柄
		TxMsg			Message (For structure UI_MSG_OBJ, see uirSDKcommon.h)
						报文(结构体UI_MSG_OBJ定义详见uirSDKcommon.h)
		RxMsg			Feedback message (structure UI_MSG_OBJ is defined in uirSDKcommon.h)
						反馈报文(结构体UI_MSG_OBJ定义详见uirSDKcommon.h)
return: 0				Success	
						发送指令 成功
		!0			Failure 
						发送指令 失败
Note  : Universal instruction
		万能指令
****************************************************************************/
UISIMCANFUNC_API ERRO  STDCALLAPI SdkSendMsg(uint GtwyHandle, UI_MSG_OBJ* TxMsg, UI_MSG_OBJ* RxMsg);
#pragma}

// [UFQ control]
;
#pragma "【UFQ Operation 】" {

UISIMCANFUNC_API ERRO STDCALLAPI GetUIRobotSDKUFQ(void);
UISIMCANFUNC_API void STDCALLAPI SetUIRobotSDKUFQ(int iSDKUFQ);

UISIMCANFUNC_API ERRO STDCALLAPI  OPRoperation(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataIn, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQStart(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQShutDown(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQFreez(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQHome(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
//UFQ Set the system working mode, MD=0 debugging mode, MD=1 working mode (power-on default)
 //UFQ 设置系统工作模式，MD=0 调试模式, MD=1 工作模式（上电默认）
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQSetMode(uint GtwyHandle, uint CANid, int iMode, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQGetODO(uint GtwyHandle, uint CANid, int * pNumber);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQGetErr(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQSetNodeID(uint GtwyHandle, uint CANid, uint dwNewCanNodeId, OPR_DATA_OBJ* pRxData);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQSetGroupID(uint GtwyHandle, uint CANid, uint dwNewGroupId, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQGetGroupID(uint GtwyHandle, uint CANid, uint *dwGroupId);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQSetPVTWarn(uint GtwyHandle, uint CANid, bool bEnabled, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQStateCheck(uint GtwyHandle, uint CANid, uchar *pStatus);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_SystemCheck(uint GtwyHandle, uint CANid, bool bPortAndDecode, OPR_DATA_OBJ* pRxData);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_SystemShutDown(uint GtwyHandle, uint CANid, unsigned short iDelayTime, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_SystemSetCurrent(uint GtwyHandle, uint CANid, uchar iCurrent, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_SystemGetCurrent(uint GtwyHandle, uint CANid, uchar *pCurrent);
// Float brake control
 // 浮球刹车控制
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_UFQSetBrake(uint GtwyHandle, uint CANid, uint dwBrake, OPR_DATA_OBJ* pRxData);
// Float Reset command
// 浮球Reset指令
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_ResetCommand(uint GtwyHandle, uint CANid, int iCommand, OPR_DATA_OBJ* pOPRDataObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  OPR_SystemCommand(uint GtwyHandle, uint CANid, int iCommand, OPR_DATA_OBJ* pOPRDataObjOut);

UISIMCANFUNC_API ERRO STDCALLAPI  UFQSetMotorPara(uint GtwyHandle, uint CANid, MOTOR_PARAM_OBJ* pMotorParaObjIn, MOTOR_PARAM_OBJ* pMotorParaOut);
UISIMCANFUNC_API ERRO STDCALLAPI  UFQGetMotorPara(uint GtwyHandle, uint CANid, MOTOR_PARAM_OBJ* pMotorParaObj);

UISIMCANFUNC_API ERRO STDCALLAPI  SetQickRgb(uint GtwyHandle, uint CANid, QUICKRGB_PARAMETER_OBJ* pQuickRGBCfgObjIn, QUICKRGB_PARAMETER_OBJ* pQuickRGBCfgObjOut);
UISIMCANFUNC_API ERRO STDCALLAPI  SetRgb(uint GtwyHandle, uint CANid, int rgb, int * outRgb);
UISIMCANFUNC_API ERRO STDCALLAPI  GetRgb(uint GtwyHandle, uint CANid, int* rgb);
// Send Float Command (UFQ)
 // 发送浮球指令(UFQ)
UISIMCANFUNC_API ERRO STDCALLAPI SendFloatballCmd(uint GtwyHandle, UI_FLOATSOCKETMSG_OBJ* pPackageObj, UI_FLOATSOCKETMSG_OBJ* pPackageObjOut, int iMaxLen, int *pMsgNum);
#pragma}

// [Real-Time Notification]
UISIMCANFUNC_API ERRO STDCALLAPI SdkGetRTCN(uint GtwyHandle, UI_MSG_OBJ* pUiMsg);
