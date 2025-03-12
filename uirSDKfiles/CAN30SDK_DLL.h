////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,								  //
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED									  //
// WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.									  //
//																											  //
//													Copyright (c) UIROBOT.									  //
//														All rights reserved.								  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning( disable: 4081 4083)

#include "uirSDKcommon3.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//                                            Static Library Function Pointers (Prototypes / Styles)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//【Note】1 CAN Network = 1 CAN Gateway (Leader) + N CAN Members (Employees)

//【Note】Calling StartCanNet(...), opens the CAN network = opens the hardware port + opens the gateway + collects gateway information + collects all member information + completes all preparations; after opening, you can operate on the members

//【Note】Calling StartCanNet(...), the SDK will allocate a unique GtwyHandle to the user-specified gateway; subsequent operations on the gateway should provide this GtwyHandle

//【Note】Supports up to 32 "Serial-CAN networks" (UIM2502/UIM2503) + 128 "Ethernet-CAN networks" (UIM2512/UIM2513) simultaneously; each CAN network contains 1 gateway and up to 100 members

//【Note】The original global setting functions such as SetgORG / SetgSPD... have been canceled; for global settings, just set CANid = 127 to perform global settings/queries on the members.

//【RTCN】Used to handle asynchronous messages, i.e., feedback that is not initiated by the user or cannot be foreseen by the user. For example, sensor triggers, point motion completion notifications, stall detection notifications, etc.

//【Note】Each gateway (Gtwy) should register a callback function through its GtwyHandle

//【Note】Function return value = 0 indicates correct; != 0 indicates an error occurred (remember, return -1 is only one of the errors, not all errors return -1); when an error occurs, you can determine the error based on the return value against the error code table

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//【系统操作】
;
#pragma "【CAN 网络】" {
//[*] 开启 CAN Network
typedef int(*SDKSTARTCANNET)(GATEWAY_INFO_OBJ* pGtwy, MEMBER_INFO_OBJ* pMember, uint UseConstLink);
//[*] 关闭 CAN Network
typedef int(*SDKCLOSECANNET)(uint GtwyHandle);
//[*] 获取 CAN 网关信息
typedef int(*SDKGETGTWYINFO)(GATEWAY_INFO_OBJ* pGtwy, MEMBER_INFO_OBJ* pMember);
//[*] 获取 CAN 成员信息列表
typedef int(*SDKGETMEMBERLIST)(uint GtwyHandle, MEMBER_INFO_OBJ* pMemberInfo, uint MemberQty);
//[*] 设置 ETH 网关（2512/2513）的 IPaddr
typedef int(*SDKSETIPADDR)(uint GtwyHandle, uchar* IPaddr);
#pragma }
;
#pragma "【通讯参数】(Protocol Parameter)" {
//[*] 通讯参数 
typedef int(*SDKSETPROTOCOLPARA)(uint GtwyHandle, uint CANid, uint Index, uint Data, uint* pRxdata);
typedef int(*SDKGETPROTOCOLPARA)(uint GtwyHandle, uint CANid, uint Index, uint* pRxdata);
#pragma}
;
#pragma "【系统信息】" {
//[*] 序列号
typedef int(*SDKSETSERIAL)(uint GtwyHandle, uint CANid, uint SerialNumber, uint* pRxData);
typedef int(*SDKGETSERIAL)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 器件型号
typedef int(*SDKGETMODEL)(uint GtwyHandle, uint CANid, MODEL_INFO_OBJ* pModelInfo);
//[*] 查询错误报告; 历史错误记录的序号 ErrIdx = 10....17(最新...最旧)
typedef int(*SDKGETERRREPORT)(uint GtwyHandle, uint CANid, uint ErrIdx, ERR_REPORT_OBJ* pRxData);
//[*] 清除所有错误; 
typedef int(*SDKCLRERRREPORT)(uint GtwyHandle, uint CANid, ERR_REPORT_OBJ* pRxData);
//[*] 查询最新错误;
typedef int(*SDKGETLASTERR)(uint GtwyHandle, uint CANid, ERR_REPORT_OBJ* pRxData);
//[*] 电机状态; SdkRstMotionStatus() 仅仅 清零 PAIF/PSIF/TCIF,并返回电机状态
typedef int(*SDKGETMOTIONSTATUS)(uint GtwyHandle, uint CANid, MOTION_STATUS_OBJ* pStatus, int* pVe, int* pPr, int* pPa);
typedef int(*SDKRSTMOTIONSTATUS)(uint GtwyHandle, uint CANid, MOTION_STATUS_OBJ* pStatus);
//[*] 电机状态推送周期;
typedef int(*SDKSETSTATUSREPRTPERIOD)(uint GtwyHandle, uint CANid, uint Period, uint* pAckPeriod);
typedef int(*SDKRSTSTATUSREPRTPERIOD)(uint GtwyHandle, uint CANid);
//[*] 器件上电至今的时长（unit: us）;
typedef int(*SDKGETSYSTEMTIME)(uint GtwyHandle, uint CANid, uint * pPoweOnTime);
typedef int(*SDKRSTSYSTEMTIME)(uint GtwyHandle, uint CANid, uint * piPoweOnTime);
//[*] 系统操作
#define _SY_ULK_SYS 0 // 系统解锁, Unlock System（仅620/720/630/730）;
#define _SY_RST_SYS 1 // 重启系统, Reset System;
#define _SY_RST_DFL 2 // 出厂设置, Restore Default;
#define _SY_SYN_ENA 4 // 使能同步信号,sy[4]=0/1=禁止/使能（仅2503/MMC900/MMC901）;
typedef int(*SDKSETSYSTEM)(uint GtwyHandle, uint CANid, int Index, int configIn, int * piConfigOut);
typedef int(*SDKGETSYSTEM)(uint GtwyHandle, uint CANid, int * piConfigOut);
//[*] 温度报警值
typedef int(*SDKSETTEMPERATURELIMIT)(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);
typedef int(*SDKGETTEMPERATURELIMIT)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 查询当前温度
typedef int(*SDKGETCURRTEMPERATURE)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 查询/设置 MMC 日期
typedef int(*SDKSETMMCDATE)(uint GtwyHandle, uint CANid, uint yr, uint mo, uint da, uint hr, uint mi, uint se);
typedef int(*SDKGETMMCDATE)(uint GtwyHandle, uint CANid, uint* yr, uint* mo, uint* da, uint* hr, uint* mi, uint* se);
#pragma }
;
#pragma "【上电配置】(Initial Config)" {
typedef int(*SDKSETINITIALCONFIG)(uint GtwyHandle, uint CANid, uint Index, uint TxData, uint* pRxData);
typedef int(*SDKGETINITIALCONFIG)(uint GtwyHandle, uint CANid, uint Index, uint* pRxData);
#pragma}
;
#pragma "【通知使能】(Inform Enable)" {
typedef int(*SDKSETINFORMENABLE)(uint GtwyHandle, uint CANid, uint Index, uint iValue, uint * pRxData);
typedef int(*SDKGETINFORMENABLE)(uint GtwyHandle, uint CANid, int Index, uint * piValueOut);
#pragma}
;
//【电机控制】
;
#pragma "【运动控制】静态参数" {

//[*] 驱动参数 (index:配置子项，取值如下)
#define MTS_MCS_IDX 0 // 0:细分[1.2.4.8.16]
#define MTS_CUR_IDX 1 // 1:电流[10...80]
#define MTS_PSV_IDX 2 // 2 待机节流[0...100(%)]
#define MTS_ENA_IDX 3 // 3:上电自动使能延时[0...60000(ms)]
#define MTS_CUC_IDX 4
#define MTS_BRK_IDX 5 // 5:刹车控制
typedef int(*SDKSETMOTORCONFIG)(uint GtwyHandle, uint CANid, uint Index, uint Data, uint* pRxData);
typedef int(*SDKGETMOTORCONFIG)(uint GtwyHandle, uint CANid, uint Index, uint* pRxData);
//[*] 加速度
typedef int(*SDKSETACCELERATION)(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);
typedef int(*SDKGETACCELERATION)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 减速度
typedef int(*SDKSETDECELERATION)(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);
typedef int(*SDKGETDECELERATION)(uint GtwyHandle, uint CANid, uint* piDeceleration);
//[*] 切入速度
typedef int(*SDKSETCUTINSPEED)(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);
typedef int(*SDKGETCUTINSPEED)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 急停减速度
typedef int(*SDKSETSTOPDECELERATION)(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);
typedef int(*SDKGETSTOPDECELERATION)(uint GtwyHandle, uint CANid, uint* pRxData);

//[*] 编码器参数
#define QES_QER_IDX 0 // 编码器线数（x4后=每圈脉冲数）范围[10...16000] 
#define QES_PQE_IDX 1 // 堵转容差(必须 >= 100)
#define QES_AER_IDX 2 // 绝对编码器单圈位数
#define QES_ABW_IDX 3 // 绝对编码器电池状态: =1:OK，=0:低电量
#define QES_QEX_IDX 4 // INC 编码器线数（实际值，比如磁编码器 90线）
#define QES_SYN_IDX 5 // 清除残留位置误差，一般在停止后设置
typedef int(*SDKSETENCODERCONFIG)(uint GtwyHandle, uint CANid, uint ParamCode, uint Data, uint* pRxData);
typedef int(*SDKGETENCODERCONFIG)(uint GtwyHandle, uint CANid, uint ParamCode, uint* pRxData);
typedef int(*SDKCALIBRATEENCODER)(uint GtwyHandle, uint CANid);
//[*] 辅助位置信息 (当闭环控制模式时，返回开环脉冲值；开环控制模式时，返回编码器值；)
typedef int(*SDKGETAUXPOSITION)(uint GtwyHandle, uint CANid, int* pRxData);
//[*] 回程间隙补偿值
typedef int(*SDKSETBACKLASHCOMP)(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);
typedef int(*SDKGETBACKLASHCOMP)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 运动限位
typedef int(*SDKSETMOTIONLIMITS)(uint GtwyHandle, uint CANid, MOTION_LIMIT_OBJ* pData, MOTION_LIMIT_OBJ* pRxData);
typedef int(*SDKGETMOTIONLIMITS)(uint GtwyHandle, uint CANid, MOTION_LIMIT_OBJ* pRxData);
typedef int(*SDKRSTMOTIONLIMITS)(uint GtwyHandle, uint CANid, MOTION_LIMIT_OBJ* pRxData);
//[*] 使能/禁止 限位检查(Data=0:禁止 / 1:使能)
typedef int(*SDKSETLIMITCHECK)(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);
typedef int(*SDKGETLIMITCHECK)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 读取参数目标值 (index:配置子项，取值如下)
#define DVR_MOD_IDX 0 // 目标控制模式: JV / PTP / PVT
#define DVR_CUR_IDX 1 // 目标工作电流
#define DVR_SPD_IDX 2 // 目标速度
#define DVR_PRM_IDX 3 // 目标位移
#define DVR_PAM_IDX 4 // 目标位置
#define DVR_TIS_IDX 5 // 目标报警温度
typedef int(*SDKGETDESIREDVALUE)(uint GtwyHandle, uint CANid, uint ParamCode, void* pRxData);
#pragma}
;
#pragma "【运动控制】动态参数" {
//[*] 使能 / 脱机
typedef int(*SDKSETMOTORON)(uint GtwyHandle, uint CANid, int bMotorOn, int *pbEnableOut);
typedef int(*SDKGETMOTORON)(uint GtwyHandle, uint CANid, int *pbEnable);
//[*] 开始运动
typedef int(*SDKSETBEGINMXN)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 开始运动的时间
typedef int(*SDKSETBEGINMXNTIME)(uint GtwyHandle, uint CANid, uint time, uint* pRxData);
//[*] 紧急停止
typedef int(*SDKSETSTOPMXN)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] 设置当前位置为原点 (即，PA = 0)
typedef int(*SDKSETORIGIN)(uint GtwyHandle, uint CANid);

//.............[速度控制].............

//[*] 设定速度模式 + 目标速度 ( Jog mode and Jog Velocity ) < 原 SetJogVelocity(...) >
typedef int(*SDKSETJOGMXN)(uint GtwyHandle, uint CANid, int Velo, int* pRxVelo);
//[*] 读取当前速度 < 原 SetJogVelocity(...) >
typedef int(*SDKGETJOGMXN)(uint GtwyHandle, uint CANid, int* pRxVelo);

//.............[点位控制].............

//[*] 设定位置运动 (= 同时设置 目标速度 和 目标位置)
typedef int(*SDKSETPTPMXNA)(uint GtwyHandle, uint CANid, int Velo, int Pa, int* pRxVelo, int* pRxPa);
//[*] 读取 当前速度 和 当前位置
typedef int(*SDKGETPTPMXNA)(uint GtwyHandle, uint CANid, int* pRxVelo, int* pRxPa);
//[*] 设定位移运动 (= 同时设置 目标速度 和 目标位移)
typedef int(*SDKSETPTPMXNR)(uint GtwyHandle, uint CANid, int Velo, int Pr, int* pRxVelo, int* pRxPr);
//[*] 读取 当前速度 和 当前位移
typedef int(*SDKGETPTPMXNR)(uint GtwyHandle, uint CANid, int* pRxVelo, int* pRxPr);

//.............[曲线插补].............

//[*] 复位: PVT 轨迹点缓存区 + 水位 + 执行点; 注：建议发送新的 FIFO 序列前执行 ( 前提：必须先退出PVT模式，即等水位干或者发送 SD 指令 )
typedef int(*SDKSETPVTRSTBUF)(uint GtwyHandle, uint CANid);
//[*] PVT 序列使用的 I/O 输出设备的地址（CAN-ID）
typedef int(*SDKSETPVTIODEVICE)(uint GtwyHandle, uint CANid, uchar IODcanid, uchar* pRxIODcanid);
typedef int(*SDKGETPVTIODEVICE)(uint GtwyHandle, uint CANid, uchar* pRxIODcanid);
//[*] PVT 参数 ( Motion Parameter ) < 原函数: SetPVTMotionPara(...) >
typedef int(*SDKSETPVTMPARAM)(uint GtwyHandle, uint CANid, PVT_PARAM_OBJ* pData, PVT_PARAM_OBJ* pRxData);
typedef int(*SDKGETPVTMPARAM)(uint GtwyHandle, uint CANid, PVT_PARAM_OBJ* pRxData);
//[*] 设定 PVT 模式 + 运动起点标号 < 原函数: SetPVTStartPoint(...) >
typedef int(*SDKSETPVTBGNIDX)(uint GtwyHandle, uint CANid, uint Data, uint* pRxData);
//[*] 读取当前执行点标号 < 原函数: GetPVTCurrPoint(...) >
typedef int(*SDKGETPVTEXEIDX)(uint GtwyHandle, uint CANid, uint* pRxData);
//[*] PVT 轨迹点 (24位长度数据) < 原函数: SetQPVTPoint(...) >
typedef int(*SDKSETPVTPOINT24)(uint GtwyHandle, uint CANid, uint Index, PVT_POINT_OBJ* pData, PVT_POINT_OBJ* pRxData);
//[*] PVT 轨迹点 (32位长度数据)
typedef int(*SDKSETPVTPOINT32)(uint GtwyHandle, uint CANid, uint Index, PVT_POINT_OBJ* pData, PVT_POINT_OBJ* pRxData);
typedef int(*SDKGETPVTPOINT32)(uint GtwyHandle, uint CANid, uint Index, PVT_POINT_OBJ* pRxData);
//[*] PVT 轨迹点 (K-代码) < 原函数: DownloadKcode(...) >
typedef int(*SDKSETPVTKCODE32)(uint GtwyHandle, uint CANid, PVT_KCODE_OBJ* pData, PVT_KCODE_OBJ* pRxData);
#pragma}
;
#pragma "【I/O 操作】" {

//[*] I/O 端口功能 (CfgCode = 0:数字输出,1:数字输入, 2:模拟输入, 3:PWM/QEI )
#define IOC_DOU_IDX 0
#define IOC_DIN_IDX 1
#define IOC_AIN_IDX 2
#define IOC_QEI_IDX 3
#define IOC_PWM_IDX 3

#ifndef _SETIOCONFIG_
#define _SETIOCONFIG_
typedef int(*SDKSETIOCONFIG)(uint GtwyHandle, uint CANid, uint PortIx, uint CfgCode, uint* pRxData);
typedef int(*SDKGETIOCONFIG)(uint GtwyHandle, uint CANid, uint PortIx, uint* pRxData);
#endif
//[*] 数字输入触发延时 ms (消抖作用, 如 TrigDelay > 60000, 认作单次触发, 原触发动作会被清除) < 原：SetSensorActionCfg(...) / GetSensorActionCfg(...) >
typedef int(*SDKSETTRIGGERMODE)(uint GtwyHandle, uint CANid, uint PortIx, uint TrigTime, uint* pRxData);
typedef int(*SDKGETTRIGGERMODE)(uint GtwyHandle, uint CANid, uint PortIx, uint* pRxData);
//[*] 模拟量输入
typedef int(*SDKGETANALOGINPUT)(uint GtwyHandle, uint CANid, uint PortIx, int* pRxData);
//[*] 数字量输出
typedef int(*SDKSETDIGITALOUTPUT)(uint GtwyHandle, uint CANid, DIGITAL_OUTPUT_OBJ* pDOValue, UIM_DIO_OBJ* pRxData);
//[*] 数字量端口电平 (包括输入端口和输出端口)
typedef int(*SDKGETDIOPORT)(uint GtwyHandle, uint CANid, UIM_DIO_OBJ* pRxData);

//[输入触发动作参数] 分类(Catagory)表示被设参数作用于以下场合:
#define IOL_CAT_GENERAL 0 // 0: 收到指令应用的参数 
#define IOL_CAT_STALLED 1 // 1: 电机堵转 检出后应用
#define IOL_CAT_IN1RISE 2 // 2: IN1上升沿检出后应用  
#define IOL_CAT_IN1FALL 3 // 3: IN1下降沿检出后应用
#define IOL_CAT_IN2RISE 4 // 4: IN2上升沿检出后应用
#define IOL_CAT_IN2FALL 5 // 5: IN2下降沿检出后应用
#define IOL_CAT_IN3RISE 6 // 6: IN3上升沿检出后应用
#define IOL_CAT_IN3FALL 7 // 7: IN3下降沿检出后应用
#define IOL_CAT_IN4RISE 8 // 8: IN4上升沿检出后应用
#define IOL_CAT_IN4FALL 9 // 9: IN4下降沿检出后应用

//[*] 输入触发动作.AC (加速度)
typedef int(*SDKSETINPUTLOGICAC)(uint GtwyHandle, uint CANid, uint Catagory, uint Data, uint* pRxData);
typedef int(*SDKGETINPUTLOGICAC)(uint GtwyHandle, uint CANid, uint Catagory, uint* pRxData);
//[*] 输入触发动作.DC (减速度)
typedef int(*SDKSETINPUTLOGICDC)(uint GtwyHandle, uint CANid, uint Catagory, uint Data, uint* pRxData);
typedef int(*SDKGETINPUTLOGICDC)(uint GtwyHandle, uint CANid, uint Catagory, uint* piDeceleration);
//[*] 输入触发动作.SP (速度)
typedef int(*SDKSETINPUTLOGICSP)(uint GtwyHandle, uint CANid, uint Catagory, int PTPSpeed, int* pRxPTPSpd);
typedef int(*SDKGETINPUTLOGICSP)(uint GtwyHandle, uint CANid, uint Catagory, int* pRxPTPSpd);
//[*] 输入触发动作.PR (位移)
typedef int(*SDKSETINPUTLOGICPR)(uint GtwyHandle, uint CANid, uint Catagory, int Data, int* pRxData);
typedef int(*SDKGETINPUTLOGICPR)(uint GtwyHandle, uint CANid, uint Catagory, int* pRxData);
//[*] 输入触发动作.PA (位置)
typedef int(*SDKSETINPUTLOGICPA)(uint GtwyHandle, uint CANid, uint Catagory, int Data, int* pRxData);
typedef int(*SDKGETINPUTLOGICPA)(uint GtwyHandle, uint CANid, uint Catagory, int* pRxData);

//[输入触发动作] Input Logic (输入触发的预设动作 MxnCode)
#define ILG_NULL_0000_IDX 0  // 00: 无动作(即禁止) 
#define ILG_MOFF_0000_IDX 1  // 脱机
#define ILG_JVSD_0000_IDX 2  // 急速停止
#define ILG_JVDC_0000_IDX 3  // 减速停止
#define ILG_SORG_PTPR_IDX 4  // 原点+换向+按位移abs(PR)运动
#define ILG_SORG_JVSD_IDX 5  // 原点+急速停止
#define ILG_SORG_JVDC_IDX 6  // 原点+减速停止
#define ILG_NDIR_JOGV_IDX 7  // 换向+按预设abs(JV)运动
#define ILG_JOGV_0000_IDX 8  // 按预设 +/-JV 运动
#define ILG_NDIR_PTPR_IDX 9  // 换向+按预设位移abs(PR)运动
#define ILG_PTPR_0000_IDX 10 // 按预设位移(+/-PR)运动
#define ILG_PTPA_0000_IDX 11 // 按预设位置(+/-PA)运动
#define ILG_PVTA_0000_IDX 15 // 按预设参数执行 PVT 运动
//[*] 输入触发动作逻辑
typedef int(*SDKSETINPUTLOGIC)(uint GtwyHandle, uint CANid, uint PortIx, INPUT_ACT_OBJ MxnCode, INPUT_ACT_OBJ* pRxData);
typedef int(*SDKGETINPUTLOGIC)(uint GtwyHandle, uint CANid, uint PortIx, INPUT_ACT_OBJ* pRxData);
#pragma}
;
#pragma "【伺服电机】" {

//[*] 通用设置 (SrvoData:主码+辅码+数据）
typedef int(*SDKSETSERVOPARAM)(uint GtwyHandle, uint CANid, SERVO_PARAM_OBJ* SrvoData, SERVO_PARAM_OBJ* pRxData);
//[*] 通用读取 (pRxData:主码+辅码+数据,需事先填写主码+辅码）
typedef int(*SDKGETSERVOPARAM)(uint GtwyHandle, uint CANid, SERVO_PARAM_OBJ* pRxData);
//[*] 快捷功能 (FuncCode:功能码）
#define _SV_SET_STF 0xF6 // 刚度设定,    Stiffness
#define _SV_SET_ADP 0xF7 // 自适应功能,  Parameter Auto Adaption
#define _SV_SET_FLT 0xF8 // 指令滤波,    Filter
#define _SV_CLR_ABS 0xF9 // 编码器清零,  Origin
#define _SV_CLR_ALM 0xFA // 清除当前报警,Clear Current Alarm 
#define _SV_GET_ALM 0xFC // 读取当前报警,Get Current Alarm
#define _SV_SET_EEP 0xFD // 固化参数EEP, Store Para->EEPRUISIMCANFUNC_APIOM
#define _SV_RST_AUT 0xFE // 释放修改权,  reset/release Authentication
#define _SV_GET_AUT 0xFF // 取得修改权,  get Authentication
//[*] 快捷读写 (FuncCode:功能码）
typedef int(*SDKSETSERVOFUNC)(uint GtwyHandle, uint CANid, uint FuncCode, uint TxData, uint* pRxData);
typedef int(*SDKGETSERVOFUNC)(uint GtwyHandle, uint CANid, uint FuncCode, uint* pRxData);
//[*] 力矩控制 (Cfg:配置, Thd:目标力矩/额定力矩*100%)
typedef int(*SDKSETTORQUECTRL)(uint GtwyHandle, uint CANid, uint Cfg, uint Thd, uint* pRxData);
typedef int(*SDKGETTORQUECTRL)(uint GtwyHandle, uint CANid, int* pRxData);
#pragma}
;
//【Aux Encoder】UIM0808/1616 Only
;
#pragma "【Aux Encoder】"{
//[*] Set Aux Encoder Counting Period + enable/disable(period=0) Encoder
typedef int(*SDKSETENCODERPERIOD)(uint GtwyHandle, uint CANid, uint Period, uint* pRxData);
//[*] Get Aux Encoder Counter
typedef int(*SDKGETENCODERCOUNT)(uint GtwyHandle, uint CANid, int* pRxCount);
//[*] Reset Aux Encoder Counter (= 0)
typedef int(*SDKRSTENCODERCOUNT)(uint GtwyHandle, uint CANid);
//[*] Get Aux Encoder Speed (*pRxData = Counts within 1 second)
typedef int(*SDKGETENCODERSPEED)(uint GtwyHandle, uint CANid, int* pRxData);
//[*] Get Aux Encoder Count within Period (*pRxData = Counts within Period)
typedef int(*SDKGETENCODERINCREMENT)(uint GtwyHandle, uint CANid, int* pRxData);
#pragma}
;
//【万能指令】
;
#pragma "【自由指令】" {
//[*] 
typedef int(*SDKSENDMSG)(uint GtwyHandle, UI_MSG_OBJ* TxMsg, UI_MSG_OBJ* RxMsg);
#pragma}
;
//【浮球控制】
;
#pragma "【UFQ 操作】" {
typedef int (*SDKGETUIROBOTSDKUFQ)(void);
typedef void(*SDKSETUIROBOTSDKUFQ)(int iSDKUFQ);
typedef int(*SDKOPROPERATION)(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataIn, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_UFQSTART)(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_UFQSHUTDOWN)(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_UFQFREEZ)(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_UFQHOME)(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
//UFQ 设置系统工作模式，MD=0 调试模式, MD=1 工作模式（上电默认）
typedef int(*SDKOPR_UFQSETMODE)(uint GtwyHandle, uint CANid, int iMode, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_UFQGETODO)(uint GtwyHandle, uint CANid, int * pNumber);
typedef int(*SDKOPR_UFQGETERR)(uint GtwyHandle, uint CANid, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_UFQSETNODEID)(uint GtwyHandle, uint CANid, uint dwNewCanNodeId, OPR_DATA_OBJ* pRxData);
typedef int(*SDKOPR_UFQSETGROUPID)(uint GtwyHandle, uint CANid, uint dwNewGroupId, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_UFQGETGROUPID)(uint GtwyHandle, uint CANid, uint *dwGroupId);
typedef int(*SDKOPR_UFQSETPVTWARN)(uint GtwyHandle, uint CANid, bool bEnabled, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_UFQSTATECHECK)(uint GtwyHandle, uint CANid, uchar *pStatus);
typedef int(*SDKOPR_SYSTEMCHECK)(uint GtwyHandle, uint CANid, bool bPortAndDecode, OPR_DATA_OBJ* pRxData);
typedef int(*SDKOPR_SYSTEMSHUTDOWN)(uint GtwyHandle, uint CANid, unsigned short iDelayTime, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_SYSTEMSETCURRENT)(uint GtwyHandle, uint CANid, uchar iCurrent, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_SYSTEMGETCURRENT)(uint GtwyHandle, uint CANid, uchar *pCurrent);
// 浮球刹车控制
typedef int(*SDKOPR_UFQSETBRAKE)(uint GtwyHandle, uint CANid, uint dwBrake, OPR_DATA_OBJ* pRxData);
// 浮球Reset指令
typedef int(*SDKOPR_RESETCOMMAND)(uint GtwyHandle, uint CANid, int iCommand, OPR_DATA_OBJ* pOPRDataObjOut);
typedef int(*SDKOPR_SYSTEMCOMMAND)(uint GtwyHandle, uint CANid, int iCommand, OPR_DATA_OBJ* pOPRDataObjOut);

typedef int(*SDKUFQSETMOTORPARA)(uint GtwyHandle, uint CANid, MOTOR_PARAM_OBJ* pMotorParaObjIn, MOTOR_PARAM_OBJ* pMotorParaOut);
typedef int(*SDKUFQGETMOTORPARA)(uint GtwyHandle, uint CANid, MOTOR_PARAM_OBJ* pMotorParaObj);

typedef int(*SDKSETQUICKRGB)(uint GtwyHandle, uint CANid, QUICKRGB_PARAMETER_OBJ* pQuickRGBCfgObjIn, QUICKRGB_PARAMETER_OBJ* pQuickRGBCfgObjOut);
typedef int(*SDKSETRGB)(uint GtwyHandle, uint CANid, int rgb, int * outRgb);
typedef int(*SDKGETRGB)(uint GtwyHandle, uint CANid, int* rgb);

// 发送浮球指令(UFQ)
typedef int(*SDKSENDFLOATBALLCMD)(uint GtwyHandle, UI_FLOATSOCKETMSG_OBJ* pPackageObj, UI_FLOATSOCKETMSG_OBJ* pPackageObjOut, int iMaxLen, int *pMsgNum);
#pragma}

//【异步通知】
typedef ERRO(*SDKGETRTCN)(uint GtwyHandle, UI_MSG_OBJ* pUiMsg);
