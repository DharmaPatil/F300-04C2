#ifndef _DEBUGSTATE_H
#define _DEBUGSTATE_H

#include "71x_type.h"

extern bool gb_DebugUart1;     //串口1正在调试标志位，要屏蔽正常的通讯函数
extern bool gb_DebugUart0;     //串口2正在调试标志位，要屏蔽正常的通讯函数
extern bool gb_ModbusAnswerFlg_Uart0;   //Modbus协议有返回发送标志位
extern bool gb_ModbusAnswerFlg_Uart1;   //Modbus协议有返回发送标志位
extern u8 gu8_DebugRxdArray_UART1[RXDLEN];        //接收数组存储UARTn->RxBUFR高8位（校验位和帧错误）
extern u8 gu8_DebugRxdArray_UART0[RXDLEN];        //接收数组存储UARTn->RxBUFR高8位（校验位和帧错误）


extern void Key_DebugState(void);
extern void Disp_DebugState(void);


#endif
