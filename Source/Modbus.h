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
    CurWt,            //当前重量
    LoadMV,           //当前增益毫伏数
    RealMV,           //绝对毫伏数
    ZeroMV            //零点毫伏数
}BusWtType;


extern u16 gu16_MbIOOutInit;       //开关量输出初始化 0093
extern u16 gu16_MbIoTestSwitch;             //开关量测试开关
extern u16 gu16_MbIoOutTest;                //开关量输出测试寄存器
extern u16 gu16_MbIoInState;           //开关量输入状态
extern u16 gu16_MbIoOutState;          //开关量输出状态
extern bool gb_MbKeyLockFlg;                //modbus键盘锁
extern bool bFinishReceive_UART0;//开始发送数据
extern bool bFinishReceive_UART1;//开始发送数据
extern u32 gu32_GainMv_A;              //A秤MODBUS手动标定输入mv数
extern u32 gu32_GainMv_B;              //B秤MODBUS手动标定输入mv数
extern bool gb_S485Recieve;
extern u32 ulFrameLength_UART0;        //数据长度(接收缓冲长度)
extern u32 ulFrameLength_UART1;        //数据长度(接收缓冲长度)
extern u32 gu32_MbLastResult;      //上秤包装结果
extern u16 gu16_MbIoOutDebug;      //94~105
extern u16 gu16_MbIOOutInit;       //开关量输出初始化0  未初始化1 0093
extern u32 ulTBUFAvailNum_UART0;   //发送缓冲区中的有效数据个数
extern u32 ulTBUFBegin_UART0;      //发送缓冲区的有效起始地址
extern bool bUARTSending_UART0;        //串口没有发送操作

extern bool (*FpCheckCrcLrc)(u8 *fu8p_Arry, u32 fu32_Num); //Modbus校验检查函数的指针
extern void (*FpBuildCrcLrc)(u8 *fu8p_Arry, u32 fu32_Num); //Modbus构造校验函数的指针

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
