#ifndef _UART_H
#define _UART_H

#include "71x_type.h"

#define UART0_SIPL      15   //����0�ж����ȼ�
#define UART1_SIPL      6   //����1�ж����ȼ�

#define  RXDLEN         50      //���մ��ڻ���������
#define  TXDLEN         220     //���ͻ���������


//------------��Э�鷢�ͺͽ��շ������ֽ����ƣ�Э���иĶ�Ҫע��˴�����-------------------

#ifdef DEBUG_MONITORTIME_PRINT
    #define  TXDLEN_PRINT   214
#else
//��ӡ���ڷ������鳤�� ��ӡ���80�� ��0x0D 0x0A
    #define  TXDLEN_PRINT   82
#endif
//Modbus-ASCII��ʽ�����16������д�����Ĵ������27���ֽ�
#define  RXDLEN_BUS_A   27
//Modbus-ASCII��ʽ����03�������50���Ĵ������213���ֽ�
#define  TXDLEN_BUS_A   213
//Modbus-RTU��ʽ�����16������д�����Ĵ������13���ֽ�
#define  RXDLEN_BUS_R   13
//Modbus-RTU��ʽ����03�������50���Ĵ������105���ֽ�
#define  TXDLEN_BUS_R   105
//Read��ʽ�����7���ֽ�
#define  RXDLEN_READ    9
//Read��Cont��ʽ�����29���ֽ�
#define  TXDLEN_READ_CONT   29

//PROFIBUS�Ĵ��ڻ��泤��������PROFIBUS���C�ļ��ж���



#define STARTCHAR   0x02
#define ENDCHAR1    0X0D
#define ENDCHAR2    0X0A

#define COM_PRINT       0   //��ӡ
#define COM_CONT        1   //cont������ʽ
#define COM_READ        2   //read���ʽ
#define COM_READ_O      3   //read���ʽ
#define COM_BUS_R       4   //Modbus RTU��ʽ
#define COM_BUS_A       5   //Modbus ASCII��ʽ
#define COM_PBUS        6   //PROFIBUSͨѶ

#define P_18N2      0      //18N2
#define P_18E1      1      //18E1
#define P_18O1      2      //18O1
#define P_18N1      3      //18N1
#define P_17N2      4      //17N2
#define P_17E1      5      //17E1
#define P_17O1      6      //17O1





#define Sio_485_Enable()        GPIO_BitWrite(GPIO1,1,0)        //�������
#define Sio_485_Disable()       GPIO_BitWrite(GPIO1,1,1)        //��������գ�������

//����1����ز���
extern u8  gu8_RxdArray_UART0[RXDLEN];
extern u8  gu8_TxdArray_UART0[TXDLEN];
extern volatile u8  gvu8_RxdCnt_UART0;                 //���ռ���
extern volatile bool  gvb_SioTxingFlg_UART0;
extern volatile bool  gvb_SioRxingFlg_UART0;

//����2����ز���
extern u8  gu8_RxdArray_UART1[RXDLEN];        //��������
extern u8  gu8_TxdArray_UART1[TXDLEN];        //��������
extern volatile u8  gvu8_RxdCnt_UART1;                 //���ռ���
extern volatile bool  gvb_SioTxingFlg_UART1;           //�������ڷ���
extern volatile bool  gvb_SioRxingFlg_UART1;           //�������ڽ���
extern volatile bool  gvb_EndCharFlg_UART0;            //���յ���������־
extern volatile bool  gvb_EndCharFlg_UART1;            //���յ���������־

extern const u32 gu32_BaudRateArry[];
extern const UARTParity_TypeDef g_ParityArry[];
extern const UARTStopBits_TypeDef g_StopBitsArry[];
extern const UARTMode_TypeDef g_DataLenArry[];

#ifdef DEBUG_WT_SEND
    extern u8 gu8_DebugState2;     //���Թ��� ״̬2����
    #define T1START     '1'
    #define SP1START    '2'
    #define SP2START    '3'
    #define T2AEND      '4'
    #define SP3START    '5'
    #define T2BEND      '6'
    #define T3START     '7'
    #define T4START     '8'
    #define DISCSTART   '9'
    #define T5START     ':'
    #define MAXK        ';'
    
#endif

extern void Uart0_Init(void);
extern void Uart1_Init(void);
extern void Commumication(void);
extern void Com_Cont(u8 *fpu8_TxdArry);
extern void UartGMInit_UART0(void);
extern void RxdArry_To_TxdArry(u8 fu8_Uart, u8 fu8_Num);
extern void UartGMInit_UART1(void);
extern void Read_CE(u8 *fu8p_Arry);
extern void StartSend_Com(u8 *fu8p_Array);
extern void Com_Cont_o(u8 *fu8p_Array);
extern bool RxData_Check(u8 *fu8p_Array);
extern void IRQ_GM_ModbusASCII_UART0(void);
extern void IRQ_GM_ModbusASCII_UART1(void);
void IRQ_Debug_UART0(void);
bool Debug_Send(u8 *fu8p_Data, u32 fu32_Num);
void DebugInit_UART0(void);

#endif
