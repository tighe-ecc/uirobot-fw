////////////////////////////////////////////////////////////////////////////////
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,//
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED	  //
// WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.	  //
//																			  //
//							Copyright (c) UIROBOT.							  //
//							 All rights reserved.							  //
////////////////////////////////////////////////////////////////////////////////

#pragma once
#pragma warning( disable: 4081)

#include <string>

typedef int ERRO;
typedef int NUMB;
typedef unsigned int  uint;
typedef unsigned int  unint;
typedef unsigned char uchar;

using namespace std;

#define SDK_RETURN_SUCCESS    0           // 无错误
#define ERROR_UNKNOWN_FAILED  0xFFFFFFFF  // 未知错误
;
#pragma "KIN Error 0x8000 0000"{

#define SDK_ERROR_UART_HADLE		 (0x8000000A)	// UART 文件打开失败
#define SDK_ERROR_BWRT_TMOUT		 (0x8000000C)	// Message Write __AK 超时
#define SDK_ERROR_BWRT_ERPNT		 (0x8000000D)	// Message Write 无法找到续传所要的断点
#define SDK_ERROR_BWRT_REWRT		 (0x8000000E)	// Message Write 尝试发送 3次 均失败
#define SDK_ERROR_BWRT_CLOSE		 (0x80000009)	// Message Write 接收线程关闭

#define ERROR_CERTIFICATION_FAIL                    (0x80000100)  // 认证失败 
#define ERROR_OPEN_INIFILE_FAIL                     (0x80000101)  // 打开机器人类型定义文件失败 
#define ERROR_OPEN_CFGFILE_FAIL                     (0x80000102)  // 打开机器人结构参数文件失败 
#define ERROR_SET_ROBOTCFG_FAIL                     (0x80000103)  // 配置机器人结构参数失败
#define ERROR_INIT_ROBOT_NDEF                       (0x80000104)  // 机器人配置类型未定义
#define ERROR_OPEN_DATAFILE_FAIL                    (0x80000105)  // 创建仿真数据文件失败

#define ERROR_PARA_POINT_NULL                       (0x80000201)  // 参数指针为NULL
#define ERROR_TRJTM_OVER_LONG                       (0x80000202)  // 行程时间超长
#define ERROR_TRJTM_TOO_SHORT                       (0x80000203)  // 行程时间为零
#define ERROR_BELTSPD_TOO_LOW                       (0x80000204)  // 传输带速度为零

#define ERROR_PARA_SHORT_DIST		                (0x8000020A)  // 行程太短( <10mm )

#define WARNG_POSI_LIMIT_XYLH                       (0x00000301)  // POS > 工作半径上部
#define WARNG_POSI_LIMIT_XYLL                       (0x00000302)  // POS > 工作半径下部
#define WARNG_POSI_LIMIT_PZLH                       (0x00000303)  // POS > 工作高度
#define WARNG_POSI_LIMIT_PZLL                       (0x00000304)  // POS < 工作高度
#define WARNG_POSI_LIMIT_PRLH                       (0x00000305)  // 中轴法兰 > +360°
#define WARNG_POSI_LIMIT_PRLL                       (0x00000306)  // 中轴法兰 < -360°

#define ERROR_POSI_LIMIT_XYLH                       (0x80000301)  // SCARA: 大于工作范围 / DELTA: 超出上部工作范围
#define ERROR_POSI_LIMIT_XYLL                       (0x80000302)  // SCARA: 小于工作范围 / DELTA: 超出上部工作范围
#define ERROR_POSI_LIMIT_PZLH                       (0x80000303)  // POS > 工作高度
#define ERROR_POSI_LIMIT_PZLL                       (0x80000304)  // POS < 工作高度
#define ERROR_POSI_LIMIT_PRLH                       (0x80000305)  // 中轴法兰 > +360°
#define ERROR_POSI_LIMIT_PRLL                       (0x80000306)  // 中轴法兰 < -360°

#define WARNG_AXIS_RANGE_POS1                       (0x00000311)  // Axis-1 轴角过大
#define WARNG_AXIS_RANGE_NEG1                       (0x00000312)  // Axis-1 轴角过小
#define WARNG_AXIS_RANGE_POS2                       (0x00000313)  // Axis-2 轴角过大
#define WARNG_AXIS_RANGE_NEG2                       (0x00000314)  // Axis-2 轴角过小
#define WARNG_AXIS_RANGE_POS3                       (0x00000315)  // Axis-3 轴角过大
#define WARNG_AXIS_RANGE_NEG3                       (0x00000316)  // Axis-3 轴角过小
#define WARNG_AXIS_RANGE_POS4                       (0x00000317)  // Axis-4 轴角过大
#define WARNG_AXIS_RANGE_NEG4                       (0x00000318)  // Axis-4 轴角过小

#define ERROR_AXIS_RANGE_POS1                       (0x80000311)  // Axis-1 轴角过大
#define ERROR_AXIS_RANGE_NEG1                       (0x80000312)  // Axis-1 轴角过小
#define ERROR_AXIS_RANGE_POS2                       (0x80000313)  // Axis-2 轴角过大
#define ERROR_AXIS_RANGE_NEG2                       (0x80000314)  // Axis-2 轴角过小
#define ERROR_AXIS_RANGE_POS3                       (0x80000315)  // Axis-3 轴角过大
#define ERROR_AXIS_RANGE_NEG3                       (0x80000316)  // Axis-3 轴角过小
#define ERROR_AXIS_RANGE_POS4                       (0x80000317)  // Axis-4 轴角过大
#define ERROR_AXIS_RANGE_NEG4                       (0x80000318)  // Axis-4 轴角过小

#define WARNG_AXIS_SPEED_HIGH1                      (0x00000350)  // Axis-1 速度较大, > 2400rpm
#define WARNG_AXIS_SPEED_HIGH2                      (0x00000351)  // Axis-2 速度较大, > 2400rpm 
#define WARNG_AXIS_SPEED_HIGH3                      (0x00000352)  // Axis-3 速度较大, > 2400rpm
#define WARNG_AXIS_SPEED_HIGH4                      (0x00000353)  // Axis-4 速度较大, > 2400rpm

#define ERROR_DIVIDED_BYZERO                        (0x80000500)  // 除数（即分子）为零

#pragma}
;
#pragma "SDK Error 0x9000 0000"{

#define SDK_RETURN_NOERR             (0x00000000) // SDK成功返回 (No Error)

#define ERROR_SDK_GATEWAY_DOWN		 (0x90000003) // 网关通讯错误，关闭后仍企图操作网关
#define ERROR_SDK_WAITACK_TMOUT		 (0x90000004) // 指令等待超时，指令发送后等待 ACK 无响应

#define ERROR_SDK_FUNCINPUT_NULL	 (0x9000000A) // SDK函数 收到非法输入，指针为 NULL
#define ERROR_SDK_FUNCINPUT_PARA	 (0x9000000B) // SDK函数 收到错误数值，非法 / 缺失

#define SDK_ERROR_SHUTDOWN           (0x90000002) // SDK已经退出 
#define SDK_ERROR_PACKAGE            (0x90000003) // 封装报文错误
#define SDK_ERROR_TIMEOUT            (0x90000004) // 等待超时;
#define SDK_ERROR_ANAYLSEPACKAGE     (0x90000005) // 解析报文失败
#define SDK_ERROR_ANAYLSENOTPACKAGE  (0x90000006) // 解析报文时没有报文输入
#define SDK_ERROR_NOT_OUTBUFFER      (0x90000007) // 解析报文时没有传入输出地址
#define SDK_ERROR_RECEIVE_ERROR      (0x90000008) // 接收到错误报文
#define SDK_ERROR_ANAYLSE_ERROR_LEN  (0x90000009) // 接收到错误长度报文
#define SDK_ERROR_PARAMETER          (0x90000010) // 输入参数错误
#define SDK_ERROR_WRITE_ERROR        (0x90000011) // 写失败
#define SDK_ERROR_TCW_ERROR          (0x90000012) // 错误指令
#define SDK_ERROR_PORT_ERROR         (0x90000013) // 端口操作失败
#define SDK_ERROR_INITIAL_ERROR      (0x90000014) // 初始化失败

// ==== 232 Gateway 错误码 ====
// ==== ETH Gateway 错误码 ====
// ==== USB Gateway 错误码 ====
#define ERROR_SDK_USB_BASE  (0x90000300)  //

#define ERROR_SDK_LIBUSB_IO            ERROR_SDK_USB_BASE +1  // Input/output error
#define ERROR_SDK_LIBUSB_INVALID_PARAM ERROR_SDK_USB_BASE +2  // Invalid parameter
#define ERROR_SDK_LIBUSB_ACCESS        ERROR_SDK_USB_BASE +3  // Access denied (insufficient permissions)
#define ERROR_SDK_LIBUSB_NO_DEVICE     ERROR_SDK_USB_BASE +4  // No such device (it may have been disconnected)
#define ERROR_SDK_LIBUSB_NOT_FOUND     ERROR_SDK_USB_BASE +5  // Entity not found
#define ERROR_SDK_LIBUSB_BUSY          ERROR_SDK_USB_BASE +6  // Resource busy
#define ERROR_SDK_LIBUSB_TIMEOUT       ERROR_SDK_USB_BASE +7  // transfer timed out
#define ERROR_SDK_LIBUSB_OVERFLOW      ERROR_SDK_USB_BASE +8  // Overflow, USB device offered too much data
#define ERROR_SDK_LIBUSB_PIPE          ERROR_SDK_USB_BASE +9  // Pipe error, endpoint halted
#define ERROR_SDK_LIBUSB_INTERRUPTED   ERROR_SDK_USB_BASE +10 // System call interrupted (perhaps due to signal)
#define ERROR_SDK_LIBUSB_NO_MEM        ERROR_SDK_USB_BASE +11 // Insufficient memory
#define ERROR_SDK_LIBUSB_NOT_SUPPORTED ERROR_SDK_USB_BASE +12 // transfer size is larger than the OS and/or hardware can support
#define ERROR_SDK_LIBUSB_OTHER         ERROR_SDK_USB_BASE +99 // Other error

#pragma}
;
#pragma "UIM Error 0xA000 0100"{

#define SDK_ERROR_CAN20_BASE (0x92000000) // SimpleCAN 2.0 错误，实际返回的错误码 = SDK_ERROR_CAN20_BASE + UIM 错误代码
#define SDK_ERROR_CAN30_BASE (0x93000000) // SimpleCAN 3.0 错误，实际返回的错误码 = SDK_ERROR_CAN20_BASE + UIM 错误代码
#define ERROR_MODULE_BASE    (0xA0000100) // 机电系统 ：ABS 编码器低电量

#pragma "SimpleCAN 2.0"{

// 系统状态
#define ERR_ELOC_CODE	40  // 242 系统急停并上锁
#define ERR_CTX_TMO 	60  // CAN 报文发送超时（无人响应）
#define ERR_CAN_ID_CODE 62
#define ERR_RRBOVF_CODE	63
#define ERR_RSBOVF_CODE	64

// 运动状态
#define ERR_QEC_CODE	52	// 闭环模式下，无法设置
#define ERR_POS_CODE	53	// 开环模式下，无法设置	
#define ERR_NMD_CODE	54	// 无相关软件模块支持 
#define ERR_QEI_CODE	55	// 无编码器信号
#define ERR_PVS_CODE	56	// PVS FIFO 溢出

// 运动参数
#define ERR_MXN_DCSD	60  // SD减速度 小于DC减速度;
#define ERR_MXN_RUNG	61  // 电机使能中，无法更改;

// 指令语法
#define ERR_SYN_CODE	50	// 指令语法错误	242;
#define ERR_NUM_CODE	51	// 指令数据错误 242;

// 编码器
#define ERR_MXN_STOG	65	// 电机转动中，无法设置绝对值编码器原点	;

// 设置 I/O
#define ERR_IOC_ADIO	90	// I/O 端口功能配置错误

#pragma }
#pragma "SimpleCAN 3.0"{
// System
#define ERR_SYS_GWSD    8   // 机组警告并全部锁定
#define ERR_SYS_INIT    9   // 上电初始化发生故障

#define ERR_SYS_ILCK	10	// 传感器（INPUT）触发急停并上锁
#define ERR_SYS_STOP	11	// 620 电机急停过程中
#define ERR_SYS_OVHT	12	// 620 系统温度过热
#define ERR_SYS_POVL	13	// 620 对外供电系统检测到短路/过载
#define ERR_SYS_S232	14	// 720 与 SERVO 通讯故障
#define ERR_SYS_SRST	15	// 720 与 SERVO 需要断电重启
#define ERR_SYS_SALM	16	// 720 检出 SERVO 报错
#define ERR_SYS_SIOS    17  // 720 与 SERVO IO故障
#define ERR_SYS_SLAV	18	// 附属芯片发生通讯故障

// Protocol
#define ERR_CTX_TMOT 	20	// CAN 报文发送超时（无人响应）

// Motion
#define ERR_MXN_ACC	    21	// 加速度超限
#define ERR_MXN_EBRK	22	// 刹车上锁, 无法BG
#define ERR_MXN_NBRK	23	// 刹车未锁, 无法脱机
#define ERR_MXN_MOFF	24	// 脱机状态, 无法 BG
#define ERR_MXN_SPHL	25	// 速度超限检出
#define ERR_MXN_PALL	26	// -工作位置超限检出
#define ERR_MXN_PAHL	27	// +工作位置超限检出
#define ERR_MXN_SPEL	28	// 速度误差超限检出（仅闭环控制）
#define ERR_MXN_PQER	29	// 位移误差超限检出（Stepper 闭环控制）

#define ERR_MXN_TCHL	29	// 扭矩超限检出（仅 720）
#define ERR_MXN_ENCS	30	// 编码器通讯错误
#define ERR_MXN_ENCB	31	// ABS 电池无电压

// Instrution Error
#define ERR_INS_SYNT	50	// 指令语法错误 620;
#define ERR_INS_NUMB	51	// 指令数据错误 620;
#define ERR_INS_IDXR	52	// 指令索引（下标）错误;
#define ERR_SYS_STTM	53	// 电机转动中，无法更改系统时间;

// Montion Parameter Error
#define ERR_MXN_DCSD	60  // SD减速度 小于DC减速度;
#define ERR_MXN_RUNG	61  // 电机使能中，无法更改;

// Encoder
#define ERR_MXN_STOG	65	// 电机转动中，无法设置绝对值编码器原点	;

// PVT
#define ERR_PVT_RUNG	70	// 电机运动中，无法设置 PV 起始点 或 MP[0];
#define ERR_PVT_WPOV	71	// QP,QV,QT,QA 索引号大于MP[6](水位);
#define ERR_PVT_IOFN	72	// QA Mask 不符合 I/O 端口功能要求;
#define ERR_PVB_OVFL    73  // PVT Buffer 溢出;
#define ERR_SXP_BUSY    74  // Sx 处理未完成，不接受下一组参数;

// I/O 
#define ERR_IOC_ADIO	90	// I/O 端口功能配置错误

// RTCN
#define RTCN_ERR_SYS_GWSD   9   // 系统初始化/自检错误
#define RTCN_ERR_SYS_LOCK	10	// 系统急停并上锁
#define RTCN_ERR_SYS_STOP	11	// 620 电机急停过程中
#define RTCN_ERR_SYS_OVHT	12	// 620 系统温度过热
#define RTCN_ERR_SYS_POVL	13	// 620 对外供电系统检测到短路/过载
#define RTCN_ERR_SYS_SCME	14	// 720 与 SERVO 通讯故障
#define RTCN_ERR_SYS_SRST	15	// 720 与 SERVO 需要断电重启
#define RTCN_ERR_SYS_SALM	16	// 720 检出 SERVO 报错
#define RTCN_ERR_SYS_SIOE   17  // 720 与 SERVO IO故障
#define RTCN_ERR_SYS_SLAV	18	// 附属芯片发生通讯故障

#define RTCN_ERR_MXN_SPHL	25	// 速度超限检出
#define RTCN_ERR_MXN_PALL	26	// 反向工作位置超限检出
#define RTCN_ERR_MXN_PAHL	27	// 正向工作位置超限检出
#define RTCN_ERR_MXN_SPEL	28	// 速度误差超限检出（仅闭环控制）
#define RTCN_ERR_MXN_PQER	29	// 位移误差超限检出（仅闭环控制）
#define RTCN_ERR_MXN_ENCS	30	// 编码器通讯错误
#define RTCN_ERR_MXN_ENCB	31	// ABS 电池无电压

#define RTCN_ERR_URT_TMO	253 // UART 报文接受超时（字节间隔太长）

// MMC901 Only
#define ERR_URT_LCK		255 // UART 报文接收错误导致不接受任何报文
#define ERR_URT_MIX		254 // UART 报文序号不连续
#define ERR_URT_TMO		253 // UART 报文接受超时（字节间隔太长）
#define ERR_URT_CRC		252 // UART 报文CRC错误
#define ERR_URT_PSI		251 // UART 报文PS指令下标错误
#define ERR_RRB_OVF		250 // UART RRB overflow
#define ERR_RRB_LVW		249 // UART RRB level Warning
#define ERR_CSB_FUL		248 // UART CSB full Warning

#pragma }

#define ERROR_MODULE_SYS_GWSD	ERROR_MODULE_BASE + 9   // 系统初始化/自检错误
#define ERROR_MODULE_SYS_LOCK	ERROR_MODULE_BASE + 10	// 系统急停并上锁	
#define ERROR_MODULE_SYS_STOP	ERROR_MODULE_BASE + 11	// 电机急停过程中
#define ERROR_MODULE_SYS_OVHT	ERROR_MODULE_BASE + 12	// 系统温度过热
#define ERROR_MODULE_SYS_POVL	ERROR_MODULE_BASE + 13	// 对外供电系统检测到短路/过载	
#define ERROR_MODULE_SYS_S232	ERROR_MODULE_BASE + 14	// 与 SERVO 通讯故障
#define ERROR_MODULE_SYS_SRST	ERROR_MODULE_BASE + 15	// 与 SERVO 需要断电重启
#define ERROR_MODULE_SYS_SALM	ERROR_MODULE_BASE + 16	// 检出 SERVO  报错
#define ERROR_MODULE_SYS_SIOE	ERROR_MODULE_BASE + 17  // 与 SERVO IO 故障
#define ERROR_MODULE_SYS_SLAV	ERROR_MODULE_BASE + 18	// 发生内部通讯故障
#define ERROR_MODULE_CTX_TMOT	ERROR_MODULE_BASE + 20	// CAN 报文发送超时（无人响应）
#define ERROR_MODULE_MXN_ACC	ERROR_MODULE_BASE + 21	// 加速度超限
#define ERROR_MODULE_MXN_EBRK	ERROR_MODULE_BASE + 22	// 刹车上锁, 无法BG

#define ERROR_MOTOR_DISABLE_FAIL (0xA0000117)           // 电机脱机失败

#define ERROR_MODULE_MXN_NBRK	ERROR_MODULE_BASE + 23	// 刹车未锁, 无法脱机
#define ERROR_MODULE_MXN_MOFF	ERROR_MODULE_BASE + 24	// 脱机状态, 无法 BG
#define ERROR_MODULE_MXN_SPHL	ERROR_MODULE_BASE + 25	// 速度超限检出
#define ERROR_MODULE_MXN_PALL	ERROR_MODULE_BASE + 26	// 反向工作位置超限检出
#define ERROR_MODULE_MXN_PAHL	ERROR_MODULE_BASE + 27	// 正向工作位置超限检出
#define ERROR_MODULE_MXN_SPEL	ERROR_MODULE_BASE + 28	// 速度误差超限检出（仅闭环控制）
#define ERROR_MODULE_MXN_PQER	ERROR_MODULE_BASE + 29	// 位移误差超限检出（仅闭环控制）

#define ERROR_MODULE_ENC_COMM	ERROR_MODULE_BASE + 30	// 编码器通讯错误
#define ERROR_MODULE_BATT_LOW   ERROR_MODULE_BASE + 31	// ABS 电池无电压

#define ERROR_MODULE_INS_SYNT	ERROR_MODULE_BASE + 50	// 指令语法错误 620
#define ERROR_MODULE_INS_NUMB	ERROR_MODULE_BASE + 51	// 指令数据错误 620
#define ERROR_MODULE_INS_IDXR	ERROR_MODULE_BASE + 52	// 指令索引（下标）错误
#define ERROR_MODULE_SYS_STTM	ERROR_MODULE_BASE + 53	// 电机转动中，无法更改系统时间
#define ERROR_MODULE_MXN_DCSD	ERROR_MODULE_BASE + 60  // SD减速度 小于DC减速度
#define ERROR_MODULE_MXN_RUNG	ERROR_MODULE_BASE + 61  // 电机转动中，无法更改

#define ERROR_MODULE_MXN_STOG	ERROR_MODULE_BASE + 65	// 电机转动中，无法设置绝对值编码器原点	
#define ERROR_MODULE_PVT_RUNG	ERROR_MODULE_BASE + 70	// 电机运动中，无法设置 PV 起始点 或 MP[0]
#define ERROR_MODULE_PVT_WPOV	ERROR_MODULE_BASE + 71	// QP,QV,QT,QA 索引号大于MP[6](水位)
#define ERROR_MODULE_PVT_IOFN	ERROR_MODULE_BASE + 72	// QA Mask 不符合 I/O 端口功能要求
#define ERROR_MODULE_PVB_OVFL	ERROR_MODULE_BASE + 73  // PVT Buffer 溢出
#define ERROR_MODULE_IOC_ADIO	ERROR_MODULE_BASE + 90	// I/O 端口功能配置错误

// MMC901 Only
#define ERROR_MODULE_URT_SHT	ERROR_MODULE_BASE + 255 // UART 报文小于5字节(包括所有)
#define ERROR_MODULE_URT_OLN	ERROR_MODULE_BASE + 254 // UART 报文超过最大长度
#define ERROR_MODULE_URT_TMO	ERROR_MODULE_BASE + 253 // UART 报文接受超时（字节间隔太长）
#define ERROR_MODULE_URT_CRC	ERROR_MODULE_BASE + 252 // UART 报文CRC错误
#define ERROR_MODULE_URT_NEN	ERROR_MODULE_BASE + 251 // UART 报文没有结尾符
#define ERROR_MODULE_RRB_OVF	ERROR_MODULE_BASE + 250 // UART RRB overflow
#define ERROR_MODULE_RRB_LVW	ERROR_MODULE_BASE + 249 // UART RRB level Warning
#define ERROR_MODULE_CSB_FUL	ERROR_MODULE_BASE + 248 // UART CSB full Warning

#pragma}
;
#pragma "RBT Error 0xA000-0000"{

#define ERROR_ROBOT_FUNCTION_PARAM_ERROR		    (0xA0000027)  // 参数错误

#define RTCN_ROBOT_SYS_ERROR                        (0xA0000000)  // 机器人反馈错误基码 （实际错误 = 基码 + 相应 RTCN 错误）
#define ERROR_PVTWRITE_FAILED                       (0xA0000001)  // 写入PVT失败
#define ERROR_ROBOT_URGENT_STOP                     (0xA0000002)  // 机器人发生紧急停止
#define ERROR_GETPVT_FAILED                         (0xA0000003)  // 无法生成PVT列表
#define ERROR_CAMERAOBJECT_NOTFOUND_FAILED          (0xA0000004)  // 相机用户变量未找到
#define ERROR_ROBOT_STATUS_WRONG                    (0xA0000005)  // 机器人状态不正确（e.g.机器人正忙) 目前无法执行要求的任务
#define ERROR_ROBOT_TCP_OUTRANGE					(0xA0000006)  // 坐标错误
#define ERROR_ROBOT_TCP_OVERLAP                     (0xA0000101)  // 目标点 TCP 与 当前 TCP 重合，无法运动
#define ERROR_ROBOT_TRANSMODEL_STATE_FAILED         (0xA0000007)  // 透传模式下
#define ERROR_ROBOT_FILE_NOT_EXIST_FAILED           (0xA0000008)  // 文件不存在
#define ERROR_ILLEGAL_SCRIPT                        (0xA0000009)  // 不是机器人程序
#define ERROR_ROBOT_TEMPERATURE                     (0xA000000A)  // 温度报警
#define ERROR_ROBOT_EMERGYSTOP                      (0xA000000B)  // 紧急停止
#define ERROR_ROBOT_FILE_OPEN_FAILED                (0xA000000E)  // 文件打开出错
#define ERROR_ROBOT_CANNET_FAULT					(0xA000000F)  // CAN 网络通讯错误

#define ERROR_ROBOT_STALL_FAILED                    (0xA0000010)  // 堵转

#define ERROR_ROBOT_SCRIPT_OVERSIZE                 (0xA0000012)  // 文件太大

#define ERROR_ROBOT_MOTOR_FAULT                     (0xA0000016)  // 电机报错
#define ERROR_ROBOT_MALLOC_MEMORY_FAILED            (0xA000001F)  // 申请内存出错
#define ERROR_MOTOR_SOFTSAFE_ERR                    (0xA0000020)  // 超过软件限位
#define ERROR_ROBOT_LOCKDOWN                        (0xA0000024)  // 电机锁定
#define ERROR_TASK_PVT_COUNT_ZERO                   (0xA0000025)  // 没有PVT可以运行

#define ERROR_ROBOT_PVT_BPCHK_FAIL                  (0xA0000026)  // PVT起点与当前电机脉冲误差超限
#define ERROR_ROBOT_FUNCTION_PARAM_ERROR		    (0xA0000027)  // 参数错误
#define ERROR_ROBOT_CURRENT_COORDINATE_FAILED       (0xA000002C)  // 当前机器人坐标与运动起始点坐标不一致
#define ERROR_ROBOT_WAITSTOP_TIMEOUT    			(0xA000002D)  // 机器人等待运动结束超时
#define ERROR_ROBOT_KINETICS_UNINITIAL              (0xB0000308)  // 机器人运动学加载失败

#pragma}
;
#pragma "APP Error 0xB000-0000"{

#define ERROR_FILE_EXTENSION_ILLEGAL	(0xB1000001)	// 文件后缀名错误

#define ERROR_ROBOT_CONNECT_FAIL		(0xB0000003)    // 机器人连接失败 (不存在 / 断线)
#define ERROR_ROBOT_ACKMSG_WRONG		(0xB0000301)    // 机器人返回信息有错误

#define ERROR_MODULE_ACKMSG_WRONG		(0xB0000103)    // UIM 控制模块返回信息错误
#define ERROR_MODULE_INITIAL_FAIL		(0xB0000104)    // UIM 控制模块初始化失败
#define ERROR_MODULE_AUX_ENCODER		(0xB0000201)    // 辅助编码器（例如计米轮）缺失

#pragma}

// 时间      地点       人物      起因/经过       结果
// N/A   CANNET(0x60)   CANid   TCP_SET_XXXX    ERR_CODE   
//       ROBOT(0x70)    FUNC    TCP_GET_XXXX    
//        KIN(0x80)     FUNC     EXE_SCRIPT     
//        SDK(0x90)     FUNC     INIT_ROBOT     
//        USR(0xA0)     FUNC        ...       
//       [ErrCode]    [ErrCwd]  [CANid][TcpCwd][00][00]
typedef union _UI_ALARM_OBJ
{
	struct {

		uint ErrorCode = 0; // 错误代码
		uint ErrorInfo = 0; // 错误信息 
	}word;

	struct {

		uint ErrorCode; // 错误代码

		uchar product;	// 产品类别（USRAPP / SDK / KIN / ROBOT / CANMODULE / ...）
		uchar makerid;	// 生产者id（CANid / FUNCid /...）
		uchar process;	// 过程编号
		uchar auxinfo;	// 辅助信息
	};

} UI_ALARM_OBJ; // 报警信息

class CUIError
{
public:
	CUIError() {};
	~CUIError() {};
    string getErrorCode(int iErrorCode);
	int PraseErrorCode(int iErrorCode);
private:
	string m_strError;
};

int hex2dec(const char *hex);