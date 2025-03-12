
#pragma once

// 开放 SDK 综合头文件

#include "uirSDKcommon.h"
#include "UIError.h"

// 内部 SDK 综合头文件（在 uirSDKcommon.h 头文件上增加 内部使用 部分 ）

#if defined( _WINDOWS )

    #undef UISIMCANFUNC_API
    #define UISIMCANFUNC_API extern "C" __declspec(dllexport)

	#define SDK_TYPE_CRISECT CRITICAL_SECTION
	#define SDK_EVENT_HANDLE HANDLE
	#define SDK_EVENT_SIGNAL(event) SetEvent(event)
	#define SDK_EVENT_RESET(event) ResetEvent(event)
	#define SDK_EVENT_CREATE(event) event = CreateEvent(NULL, FALSE, FALSE, NULL)
	#define SDK_EVENT_DELETE(event) CloseHandle(event)

	#define SDK_InitCriSect(mutex) InitializeCriticalSection(mutex)
	#define SDK_DelCriSect(mutex) DeleteCriticalSection(mutex)
	#define SDK_LckCriSect(mutex) EnterCriticalSection(mutex)
	#define SDK_UlkCriSect(mutex) LeaveCriticalSection(mutex)

	#define SDK_THREADPROC DWORD WINAPI
	#define SDK_CREATETHREAD(handle, attr, proc, param) handle = CreateThread(NULL, NULL, proc, (void *)(param), NULL, NULL)
	#define SDK_DELETETHREAD(handle) CloseHandle(handle); handle = NULL;

	#define SDK_Strncat(_Dst, _SizeInBytes, _Src, _MaxCount) strncat_s(_Dst, _SizeInBytes, _Src, _MaxCount);
	#define SDKsprintf sprintf_s //x86

#elif defined( _LINUX )

	#define SDK_TYPE_CRISECT       pthread_mutex_t
	#define SDK_InitCriSect(mutex) pthread_mutex_init(mutex, NULL)
	#define SDK_DelCriSect(mutex)  pthread_mutex_destroy(mutex)
	#define SDK_LckCriSect(mutex)  pthread_mutex_lock(mutex)
	#define SDK_UlkCriSect(mutex)  pthread_mutex_unlock(mutex)

    #define SDK_THREADPROC void *

    typedef unsigned long HANDLE;

	#ifndef SDK_DELETETHREAD
	#define SDK_DELETETHREAD(handle) pthread_cancel(handle);pthread_join(handle, NULL)
	#endif

	#define SDK_Strncat(_Dst, _MaxCount, _Src, _SizeInBytes) strncat(_Dst, _Src, _SizeInBytes);
	#define SDKsprintf sprintf
		
	#define TRUE  true
	#define FALSE 0

#endif 

//////////////////////////////////////////////////////////////////////////

#define MAX_ETHCAN_QTY 32       // 最大可支持的 ETHCAN 网关数量
#define MAX_232CAN_QTY 16       // 最大可支持的 232CAN 网关数量

//////////////////////////////////////////////////////////////////////////

#define MSG_INTERVAL_TMO100 100 // 相邻报文间的最大时间间隔 ms
#define MSG_INTERVAL_TMO50  50  // 相邻报文间的最大时间间隔 ms

//////////////////////////////////////////////////////////////////////////

#define MSG_TYPE_SKIP 0         // _AD/_BAD...
#define MSG_TYPE_NORM 1         // 正常，比对成功
#define MSG_TYPE_GROP 2         // 正常，但需要等待
#define MSG_TYPE_RTCN 3         // RTCN，Callback
#define MSG_TYPE_ERRO 4         // 反馈错误，语法/数据/格式等

//////////////////////////////////////////////////////////////////////////

#define SN_SET_CANID     1	    // 设置站点号命令字
#define SN_SET_SERIAL_NO 2      // 设置序列号命令字
#define SN_GET_SERIAL_NO 3      // 获取序列号命令字

//////////////////////////////////////////////////////////////////////////

#define UIRSDK_VERSION	20230301 // SDK 版本
#define MAX_CANID_RANGE 128      // 单个网络最多许用的节点数量（含网关）

//////////////////////////////////////////////////////////////////////////

// LINUX模式下各个线程优先级定义

#define LINUX_PRIORITY_CALLBACK     80
#define LINUX_PRIORITY_TCP_RECIEVE  90 // BRG 接收线程优先级
#define LINUX_PRIORITY_TCP_LISTEN   70 // BRG 监听线程优先级

#define SVR_TCP_PORT 8888
#define SVR_MAX_LINK 5

//////////////////////////////////////////////////////////////////////////

#define SDK_ENTER_LCK()       {SDK_LckCriSect(&m_RcvBuf.CriSection);}               // 上锁: 接收缓冲两个锁
#define SDK_RETURN_ULK(value) {SDK_UlkCriSect(&m_RcvBuf.CriSection); return value;} // 去锁: 接收缓冲两个锁

#define MAX_IC_CONFIG_COUNT (ICCommandMAX) // IC指令总数
#define MAX_IE_CONFIG_COUNT 12             // IE配置总数

#define UITCP_TXMSG_HEAD  0xAA         
#define UITCP_TXMSG_TAIL  0xCC

#define UI_SUB_SOCKET_COMMAND_TYPE   (127)

///////////////////////////////////////////////////////////////////////////////

#define RCV_BUF_SIZE 1500  // Pod Rcv Buf 存储量(Bytes)，MTU
#define CBK_BUF_SIZE 1024  // Callback    存储量(报文数)
#define BRG_BUF_SIZE 1024  // Bridge      存储量(Bytes)

#define UIMSG3_MASK 0x80   //
#define UIMSG3_HEAD 0x80   // UI Message 的 BOF(Begin of Frame)
#define UIMSG3_TAIL 0xC0   // UI Message 的 EOF(End of Frame)

#pragma pack(1)
typedef struct _RCV_BUF_OBJ{

	SDK_TYPE_CRISECT CriSection;

	uchar      tcw[MAX_CANID_RANGE];      // 期望的 报文指令主码，用于和 msg 中的 cw 比对/匹配 （Target Control Word）
	uchar      tix[MAX_CANID_RANGE];      // 报文指令副码
	unint      qty[MAX_CANID_RANGE];      // 各站点已缓存的报文条数
	UI_MSG_OBJ msg[MAX_CANID_RANGE][100]; // 站点报文缓存，每个站点最多缓存接收 32 条报文（格式 = id cw dl d0...d8） 

// Substation receive buffer data structure under the specified gateway
#pragma pack()

#define IS_SET_MSG 1 // Need to feedback the expected value
#define IS_GET_MSG 0 // Need to feedback the current value

#define DO_ACK 1 // Expect feedback
#define NO_ACK 0 // No feedback needed

#define _NODATA_ NULL
#define _NOTIX_  0xFFFF

// 简洁函数定义
#define WAIT_FLAG_TIMEOUT(f,t) { int tmo = t; while (tmo--) { PauseMS(1); if (f) break; } }  // 等待反馈，超时退出
#define WAIT_FOR_ACK_MSG(tmo,fg)  {int t = tmo; while (t--){ if(fg) break; PauseMS(1); } if (!fg) { SDK_RETURN_ULK(SDK_ERROR_TIMEOUT); }} // 等待反馈，超时退出
#define WATCHDOG_GATEWAY_CLOSE(p)  if (!p) return SDK_RETURN_NOERR; // 看门狗：防止等待过程中关闭网关
#define WATCHDOG_THREAD_CLOSE(p)  if (!p) return 0;                 // 看门狗：防止线程处理过程中外界关闭线程
#define UPD_RXBUF_INC(idx) {pRcvBuf->qty[idx]++; if (pRcvBuf->qty[idx] == MAX_CANID_RANGE) pRcvBuf->qty[idx] = 0;}

#define ShiftBuf(buf, dln, shf)  { ::memmove(buf, buf + shf, dln - shf); ::memset(&buf[dln - shf], 0, shf); dln -= shf; shf = 0; }

#define Debug_SetTs() ts[tsix] = chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now() - m_SyTimeStamp).count(); tsix++; if(tsix == 1000) tsix=0;
#define Debug_RstTs() ts[tsix] = 0; tsix++; if(tsix == 1000) tsix=0;

// 报文长度
#define URT_MSG_SIZE 16 // 16 Bytes: 【AA】[dl][id][cw][b0]...[b8][c0][c1]【CC】
#define ETH_MSG_SIZE 16 // 16 Bytes: 【AA/AB】[dl][id][cw][b0]...[b8][c0][c1]【CC】

// Socket Function Macro 
#define SOCK_KEEPALIVE_ENABLE(sock, opr) { int v = opr; setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,  &v, sizeof(int)); }
#define SOCK_KEEPALIVE_T_IDLE(sock, opr) { int v = opr; setsockopt(sock, SOL_TCP,    TCP_KEEPIDLE,  &v, sizeof(int)); }
#define SOCK_KEEPALIVE_T_INTV(sock, opr) { int v = opr; setsockopt(sock, SOL_TCP,    TCP_KEEPINTVL, &v, sizeof(int)); }
#define SOCK_KEEPALIVE_N_PROB(sock, opr) { int v = opr; setsockopt(sock, SOL_TCP,    TCP_KEEPCNT,   &v, sizeof(int)); }
#define SOCK_TXMSG_NOPIPE(sock, opr)     { int v = opr; setsockopt(sock, SOL_SOCKET, MSG_NOSIGNAL,  &v, sizeof(int)); }

#define UISDK_FILE_EXIST 0
#define NameRbSvrON "/usr/uirRbSvrON"
#define NameUicmdON "/usr/uirUicmdON"
#define NameEGtwyON "/usr/uirEGtwyON"

#if defined __USE_232CAN_GATEWAY__
#include "uir232CAN.h"
#endif
#if defined __USE_ETHCAN_GATEWAY__
#include "uirETHCAN.h"
#endif
#if defined __USE_URTCAN_GATEWAY__
#include "uirURTCAN.h"
#endif
#if defined __USE_UFQCAN_GATEWAY__
#include "uirUFQCAN.h"
#endif

#include "BaseFunc.h"
