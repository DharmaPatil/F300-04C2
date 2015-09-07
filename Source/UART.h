#ifndef _UART_H
#define _UART_H

#include "71x_type.h"

#define UART0_SIPL      15   //串口0中断优先级
#define UART1_SIPL      6   //串口1中断优先级

#define  RXDLEN         50      //接收串口缓冲区长度
#define  TXDLEN         220     //发送缓冲区长度


//------------各协议发送和接收防错的最长字节限制，协议有改动要注意此处限制-------------------

#ifdef DEBUG_MONITORTIME_PRINT
    #define  TXDLEN_PRINT   214
#else
//打印串口发送数组长度 打印最多80列 加0x0D 0x0A
    #define  TXDLEN_PRINT   82
#endif
//Modbus-ASCII方式接收最长16功能码写两个寄存器，最长27个字节
#define  RXDLEN_BUS_A   27
//Modbus-ASCII方式发送03功能码读50个寄存器，最长213个字节
#define  TXDLEN_BUS_A   213
//Modbus-RTU方式接收最长16功能码写两个寄存器，最长13个字节
#define  RXDLEN_BUS_R   13
//Modbus-RTU方式发送03功能码读50个寄存器，最长105个字节
#define  TXDLEN_BUS_R   105
//Read方式接收最长7个字节
#define  RXDLEN_READ    9
//Read、Cont方式发送最长29个字节
#define  TXDLEN_READ_CONT   29

//PROFIBUS的串口缓存长度限制在PROFIBUS相关C文件中定义



#define STARTCHAR   0x02
#define ENDCHAR1    0X0D
#define ENDCHAR2    0X0A

#define COM_PRINT       0   //打印
#define COM_CONT        1   //cont连续方式
#define COM_READ        2   //read命令方式
#define COM_READ_O      3   //read命令方式
#define COM_BUS_R       4   //Modbus RTU方式
#define COM_BUS_A       5   //Modbus ASCII方式
#define COM_PBUS        6   //PROFIBUS通讯

#define P_18N2      0      //18N2
#define P_18E1      1      //18E1
#define P_18O1      2      //18O1
#define P_18N1      3      //18N1
#define P_17N2      4      //17N2
#define P_17E1      5      //17E1
#define P_17O1      6      //17O1





#define Sio_485_Enable()        GPIO_BitWrite(GPIO1,1,0)        //允许接收
#define Sio_485_Disable()       GPIO_BitWrite(GPIO1,1,1)        //不允许接收，允许发送

//串口1的相关参数
extern u8  gu8_RxdArray_UART0[RXDLEN];
extern u8  gu8_TxdArray_UART0[TXDLEN];
extern volatile u8  gvu8_RxdCnt_UART0;                 //接收计数
extern volatile bool  gvb_SioTxingFlg_UART0;
extern volatile bool  gvb_SioRxingFlg_UART0;

//串口2的相关参数
extern u8  gu8_RxdArray_UART1[RXDLEN];        //接收数组
extern u8  gu8_TxdArray_UART1[TXDLEN];        //发送数组
extern volatile u8  gvu8_RxdCnt_UART1;                 //接收计数
extern volatile bool  gvb_SioTxingFlg_UART1;           //串口正在发送
extern volatile bool  gvb_SioRxingFlg_UART1;           //串口正在接收
extern volatile bool  gvb_EndCharFlg_UART0;            //接收到结束符标志
extern volatile bool  gvb_EndCharFlg_UART1;            //接收到结束符标志

extern const u32 gu32_BaudRateArry[];
extern const UARTParity_TypeDef g_ParityArry[];
extern const UARTStopBits_TypeDef g_StopBitsArry[];
extern const UARTMode_TypeDef g_DataLenArry[];

#ifdef DEBUG_WT_SEND
    extern u8 gu8_DebugState2;     //调试功能 状态2变量
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
