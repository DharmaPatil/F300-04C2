#ifndef _MODBUS_H
#define _MODBUS_H

#include "71x_type.h"

#define MB_ASCII_STARTCHAR  0x3A

typedef enum
{
    DelayNo,
    DelayT15,
    DelayT35
}ISPDelay;

typedef enum
{
    CurWt,            //��ǰ����
    LoadMV,           //��ǰ���������
    RealMV,           //���Ժ�����
    ZeroMV            //��������
}BusWtType;


extern u16 gu16_MbIOOutInit;       //�����������ʼ�� 0093
extern u16 gu16_MbIoTestSwitch;             //���������Կ���
extern u16 gu16_MbIoOutTest;                //������������ԼĴ���
extern u16 gu16_MbIoInState;           //����������״̬
extern u16 gu16_MbIoOutState;          //���������״̬
extern bool gb_MbKeyLockFlg;                //modbus������
extern bool bFinishReceive_UART0;//��ʼ��������
extern bool bFinishReceive_UART1;//��ʼ��������
extern u32 gu32_GainMv_A;              //A��MODBUS�ֶ��궨����mv��
extern u32 gu32_GainMv_B;              //B��MODBUS�ֶ��궨����mv��
extern bool gb_S485Recieve;
extern u32 ulFrameLength_UART0;        //���ݳ���(���ջ��峤��)
extern u32 ulFrameLength_UART1;        //���ݳ���(���ջ��峤��)
extern u32 gu32_MbLastResult;      //�ϳӰ�װ���
extern u16 gu16_MbIoOutDebug;      //94~105
extern u16 gu16_MbIOOutInit;       //�����������ʼ��0  δ��ʼ��1 0093
extern u32 ulTBUFAvailNum_UART0;   //���ͻ������е���Ч���ݸ���
extern u32 ulTBUFBegin_UART0;      //���ͻ���������Ч��ʼ��ַ
extern bool bUARTSending_UART0;        //����û�з��Ͳ���

extern bool (*FpCheckCrcLrc)(u8 *fu8p_Arry, u32 fu32_Num); //ModbusУ���麯����ָ��
extern void (*FpBuildCrcLrc)(u8 *fu8p_Arry, u32 fu32_Num); //Modbus����У�麯����ָ��

void UartModbusInit_UART0(void);
void UartModbusInit_UART1(void);
void IRQ_ModbusRTU_UART0(void);
void DisableRecIRQ_UART0(void);
void DisableSendIRQ_UART0(void);
void EnableRecIRQ_UART0(void);
void EnableSendIRQ_UART0(void);
void DisableRecIRQ_UART1(void);
void DisableSendIRQ_UART1(void);
void EnableRecIRQ_UART1(void);
void EnableSendIRQ_UART1(void);
u16 Date_Time_Read(u8 fu8_Type);
bool is_leap_year(u16 fu16_Year);
void Com_Modbus(u8 *fpu8_Array);
void IRQ_Modbus_RTU_UART1(void);
u32 Modbus_WtOrMvData(BusWtType WtType);

#endif
