#ifndef _PROFIBUS_DP_H
#define _PROFIBUS_DP_H

#include "71x_type.h"
#include "71x_lib.h" 

#define UART_UART       0   //��չ�����ͣ�˫����
#define PBUS_UART       1   //��չ�����ͣ�PROFIBUS-DP + ����

#define GSD_1           0
#define GSD_2           1   //GSD�ļ�ѡ��

//SE�ӿڰ���CPUоƬ����������
#define SE_REQ_IT     GPIO_BitRead(GPIO2,9)   //0��SE��ʼ�����         1��SE��ʼ���ȴ�      
#define SE_S_RTS      GPIO_BitRead(GPIO1,11)  //0��SE�ȴ�������������   1��SE�ܾ�����        
#define SE_LINK       GPIO_BitRead(GPIO1,12)  //0��SE�Ѿ��ɹ�����DP���� 1��SEδ�ɹ�����DP����

#define Pbus_InitFail()      (SE_REQ_IT)    //true:SE�ӿڰ��ʼ��ʧ��  false:��ʼ���ɹ�
#define Pbus_NotLinked()     (SE_LINK)      //true:PROFISBU����δ����  false:PROFIBUS�����Ѿ�����
//#define Pbus_NotLinked()     (SE_S_RTS)      //true:PROFISBU����δ����  false:PROFIBUS�����Ѿ�����



extern u8 gu8_ExtType;     //��չ������
extern u32 gu32_GSDType;   //GSD�ļ�
extern u32 gu32_FollowMasterSwitch;    //���������궨��������վ���ƿ���

extern bool Pbus_CheckExt(void);
extern void UARTInit_Pbus(void);
u8 Pbus_Checksum(u8 *fu8p_Array, u8 fu8_i);
void Pbus_StartSend(u8 *fu8_Array, u8 fu8_TxdLen, u8 fu8_RxdLen);
void IRQ_Pbus(void);
void Com_Pbus_DP(void);

#endif