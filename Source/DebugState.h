#ifndef _DEBUGSTATE_H
#define _DEBUGSTATE_H

#include "71x_type.h"

extern bool gb_DebugUart1;     //����1���ڵ��Ա�־λ��Ҫ����������ͨѶ����
extern bool gb_DebugUart0;     //����2���ڵ��Ա�־λ��Ҫ����������ͨѶ����
extern bool gb_ModbusAnswerFlg_Uart0;   //ModbusЭ���з��ط��ͱ�־λ
extern bool gb_ModbusAnswerFlg_Uart1;   //ModbusЭ���з��ط��ͱ�־λ
extern u8 gu8_DebugRxdArray_UART1[RXDLEN];        //��������洢UARTn->RxBUFR��8λ��У��λ��֡����
extern u8 gu8_DebugRxdArray_UART0[RXDLEN];        //��������洢UARTn->RxBUFR��8λ��У��λ��֡����


extern void Key_DebugState(void);
extern void Disp_DebugState(void);


#endif
