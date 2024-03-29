/*
Description:            Modbus通讯相关
Author:                 丛海旭
Date:                   2013-12-9
*/


#include "include.h"

#define OFLVAL_MB       0xFFFFFFFF


#define READREGISTER   0x03         //读寄存器
#define WRITEREGISTER  0x06         //写双字节寄存器
#define WRITEREGISTERS 0x10         //写多字节寄存器
#define READCOIL       0x01         //读线圈
#define WRITECOIL      0x05         //写线圈

#define FUNCTIONERRO   0x01       //非法的功能码
#define DATAPLACEERRO  0x02       //非法的数据位置
#define DATAERRO       0x03       //非法的数据 
#define SLAVEBROKEN    0x04       //从机故障
#define NOSUCCESS      0x07       //不成功的编程请求
#define RESPONSEOK     0x00       //正常响应

#define STARTADS 0               //起始功能地址
#define RW2BYTESTARTADS 12       //读写双字节起始功能地址 
#define RW2BYTEENDADS 276        //读写双字节结束功能地址 
#define RW4BYTESTARTADS 16       //读写四字节起始功能地址 
#define RW4BYTEENDADS 43         //读写四字节结束功能地址 
#define RW4BYTESTARTADS_1 178     //读写四字节起始功能地址 
#define RW4BYTEENDADS_1   268     //读写四字节结束功能地址 
#define R2BYTETOTAL    (RW2BYTEENDADS+1)       //可读的字的总数 
#define RWCOILSTARTADSEE 64     //读写线圈起始功能地址
#define RWCOILENDADSEE   95     //读写线圈结束功能地址
#define RWCOILSTARTADSDEAL 96   //读写线圈特殊处理起始地址 
#define RWCOILENDADSDEAL   127  //读写线圈特殊处理结束地址
#define READMAXNUM     50      //一次性读取字长为50,即100个字节
//for test
#define F1_1ADDRESS         28      //F1.1地址
#define F1_6ADDRESS         38      //F1.6地址



#define YEAR    0
#define MONTH   1
#define DATE    2
#define WEEK    3
#define HOUR    4
#define MINUTE  5
#define SECOND  6

/* CRC 高位字节值表 */ 
const u8 auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 
/* CRC低位字节值表*/ 
const u8 auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;



/*
系统中的gs_Cal.Point定义顺序是
#define     UNIT_g      0
#define     UNIT_kg     1
#define     UNIT_t      2
#define     UNIT_lb     3
Modbus的单位顺序是
t       0
g       1
kg      2
lb      3
所以读写Modbus时要进行转换
*/
const u8 cu8_ReadUnit[] = {1, 2, 0, 3};
const u8 cu8_WriteUnit[] = {UNIT_t, UNIT_g, UNIT_kg, UNIT_lb};

bool (*FpCheckCrcLrc)(u8 *fu8p_Arry, u32 fu32_Num); //Modbus校验检查函数的指针
void (*FpBuildCrcLrc)(u8 *fu8p_Arry, u32 fu32_Num); //Modbus构造校验函数的指针
bool (*FpSendRtuAscII)(u8 *fu8p_Data, u32 fu32_Num);    //Modbus发送函数的指针



//uart0相关modbus通讯参数
u32 ulTBUFAvailNum_UART0;   //发送缓冲区中的有效数据个数
u32 ulTBUFBegin_UART0;      //发送缓冲区的有效起始地址
ISPDelay DelayType_UART0;       //延时类型
bool bOverT15_UART0=false;      //T1.5超时标志
bool bOverT35_UART0=false;      //T3.5超时标志
bool bBeginReceive_UART0=false; //开始接收数据
bool bFinishReceive_UART0;//开始发送数据
u32 ulInsertPos_UART0;          //数据插入指针点
u32 ulFrameLength_UART0;        //数据长度(接收缓冲长度)
u8  u8ModbusAddr_UART0;
bool bUARTSending_UART0=false;        //串口没有发送操作
u16 gu16_ModBusReadNumber_UART0;

//uart1相关modbus通讯参数
u32 ulTBUFAvailNum_UART1;   //发送缓冲区中的有效数据个数
u32 ulTBUFBegin_UART1;      //发送缓冲区的有效起始地址
ISPDelay DelayType_UART1;       //延时类型
bool bOverT15_UART1=false;      //T1.5超时标志
bool bOverT35_UART1=false;      //T3.5超时标志
bool bBeginReceive_UART1=false; //开始接收数据
bool bFinishReceive_UART1;//开始发送数据
u32 ulInsertPos_UART1;          //数据插入指针点
u32 ulFrameLength_UART1;        //数据长度(接收缓冲长度)
u8  u8ModbusAddr_UART1;
bool bUARTSending_UART1=false;        //串口没有发送操作
u16 gu16_ModBusReadNumber_UART1;


u16 gu16_MbIoTestSwitch;        //开关量测试开关
u16 gu16_MbIoOutTest;           //开关量输出测试寄存器
u16 gu16_MbIoInState;           //开关量输入状态
u16 gu16_MbIoOutState;          //开关量输出状态
u32 gu32_GainMv = 0;              //A秤MODBUS手动标定输入mv数
u32 gu32_ReadCoilIo;            //读写开关量存储
u32 gu32_ReadCoilSwitch;        //读写线圈存储
bool gb_MbKeyLockFlg;           //串口操作键盘锁
bool gb_S485Recieve = false;
u16 gu16_MbIOOutInit;       //开关量输出初始化0  未初始化1 0093
u16 gu16_MbIoOutDebug;      //94~105
u32 gu32_MbLastResult = 0;      //上秤包装结果

u8 BuildModbusLrc(u8 *fuc_Msg, u8 fuc_DataLen);

/***********************************************************************
函 数 名：void DisableRecIRQ_UART0(void)
功    能：禁止接收中断
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-8-2 14:42
修    改：                     日    期：
***********************************************************************/
void DisableRecIRQ_UART0(void)
{
    //关闭非空中断
    UART_ItConfig(UART0,UART_RxBufNotEmpty,DISABLE);
}

/***********************************************************************
函 数 名：void DisableRecIRQ_UART1(void)
功    能：禁止接收中断
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-8-2 14:42
修    改：                     日    期：
***********************************************************************/
void DisableRecIRQ_UART1(void)
{
    //关闭非空中断
    UART_ItConfig(UART1,UART_RxBufNotEmpty,DISABLE);
}

/***********************************************************************
函 数 名：void EnableRecIRQ_UART0(void)
功    能：启动接收中断
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-8-2 14:43
修    改：                     日    期：
***********************************************************************/
void EnableRecIRQ_UART0(void)
{
    //开启非空中断
    UART_ItConfig(UART0,UART_RxBufNotEmpty,ENABLE);
}

/***********************************************************************
函 数 名：void EnableRecIRQ_UART1(void)
功    能：启动接收中断
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-8-2 14:43
修    改：                     日    期：
***********************************************************************/
void EnableRecIRQ_UART1(void)
{
    //开启非空中断
    UART_ItConfig(UART1,UART_RxBufNotEmpty,ENABLE);
}

/***********************************************************************
函 数 名：void DisableSendIRQ_UART0(void)
功    能：启动发送中断
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-8-2 16:30
修    改：                     日    期：
***********************************************************************/
void DisableSendIRQ_UART0(void)
{
    //关闭发送缓冲区半空中断和空中断
    UART_ItConfig(UART0,UART_TxHalfEmpty,DISABLE);
    UART_ItConfig(UART0,UART_TxEmpty,DISABLE);
}

/***********************************************************************
函 数 名：void DisableSendIRQ_UART1(void)
功    能：启动发送中断
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-8-2 16:30
修    改：                     日    期：
***********************************************************************/
void DisableSendIRQ_UART1(void)
{
    //关闭发送缓冲区半空中断和空中断
    UART_ItConfig(UART1,UART_TxHalfEmpty,DISABLE);
    UART_ItConfig(UART1,UART_TxEmpty,DISABLE);
}

/***********************************************************************
函 数 名：void EnableSendIRQ_UART0(void)
功    能：启动发送中断
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-8-2 16:30
修    改：                     日    期：
***********************************************************************/
void EnableSendIRQ_UART0(void)
{
    //开启发送缓冲区半空中断和空中断
    UART_ItConfig(UART0,UART_TxHalfEmpty,ENABLE);
    UART_ItConfig(UART0,UART_TxEmpty,ENABLE);
}

/***********************************************************************
函 数 名：void EnableSendIRQ_UART1(void)
功    能：启动发送中断
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-8-2 16:30
修    改：                     日    期：
***********************************************************************/
void EnableSendIRQ_UART1(void)
{
    //开启发送缓冲区半空中断和空中断
    UART_ItConfig(UART1,UART_TxHalfEmpty,ENABLE);
    UART_ItConfig(UART1,UART_TxEmpty,ENABLE);
}

/***********************************************************************
函 数 名：void UartModbusInit_UART0(UartSet_Typedef *pUartSet)
功    能：初始化Modebus使用的串口
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：丛海旭               日    期：2014-1-22
修    改：                     日    期：
***********************************************************************/
void UartModbusInit_UART0(void)
{
    UART_Init(UART0);
    Sio_485_Enable();
    
    //启动的时候，发送队列有效个数为0，首指针都指向数组的头部。
    ulTBUFAvailNum_UART0=0;
    ulTBUFBegin_UART0=0;
    
    DelayType_UART0=DelayNo;        //初始化延时类型为无延时
    bOverT15_UART0=true;            //T1.5超时标志
    bOverT35_UART0=true;            //T3.5超时标志
    bBeginReceive_UART0=false;      //开始接收数据
    bFinishReceive_UART0=false;     //开始发送数据
    
    u8ModbusAddr_UART0=gs_Setup.TareNumber_2;      //首先把Modbus地址设置为通用地址
    ulInsertPos_UART0=0;            //数据插入指针点
    ulFrameLength_UART0=0;          //数据长度
    ulTBUFAvailNum_UART0=0;         //发送数据的总长度
    ulTBUFBegin_UART0=0;            //发送数据的开始端
    bUARTSending_UART0=false;       //串口没有发送操作
    
    //使能UART0，清空接收FIFO，清空发送FIFO，使能UART0的FIFO
    UART_OnOffConfig(UART0,ENABLE);
    UART_FifoReset(UART0,UART_RxFIFO);
    UART_FifoReset(UART0,UART_TxFIFO);
    UART_FifoConfig(UART0,ENABLE);
    
    //回写方式关
    UART_LoopBackConfig(UART0,DISABLE);
    
    //串口0波特率，校验方式等设置
    UART_Config(UART0,gu32_BaudRateArry[gs_Setup.BaudRate_2],g_ParityArry[gs_Setup.ParityMode_2],g_StopBitsArry[gs_Setup.ParityMode_2],g_DataLenArry[gs_Setup.ParityMode_2]);
    
    //接收允许
    UART_RxConfig(UART0,ENABLE);
    
    //设置超时非空中断的超时长度
//  CLR485S;
    EnableRecIRQ_UART0(); 
    DisableSendIRQ_UART0();
    
    //设置中断的优先级，并且使能UART0中断
    EIC_IRQChannelPriorityConfig(UART0_IRQChannel,UART0_SIPL);
    EIC_IRQChannelConfig(UART0_IRQChannel,ENABLE);//
}

/***********************************************************************
函 数 名：void UartModbusInit_UART1(UartSet_Typedef *pUartSet)
功    能：初始化Modebus使用的串口
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：丛海旭               日    期：2014-1-22
修    改：                     日    期：
***********************************************************************/
void UartModbusInit_UART1(void)
{
    UART_Init(UART1);
        
    //启动的时候，发送队列有效个数为0，首指针都指向数组的头部。
    ulTBUFAvailNum_UART1=0;
    ulTBUFBegin_UART1=0;
    
    DelayType_UART1=DelayNo;        //初始化延时类型为无延时
    bOverT15_UART1=true;            //T1.5超时标志
    bOverT35_UART1=true;            //T3.5超时标志
    bBeginReceive_UART1=false;      //开始接收数据
    bFinishReceive_UART1=false;     //开始发送数据
    
    u8ModbusAddr_UART1=gs_Setup.TareNumber_1;      //首先把Modbus地址设置为通用地址
    ulInsertPos_UART1=0;            //数据插入指针点
    ulFrameLength_UART1=0;          //数据长度
    ulTBUFAvailNum_UART1=0;         //发送数据的总长度
    ulTBUFBegin_UART1=0;            //发送数据的开始端
    bUARTSending_UART1=false;       //串口没有发送操作
    
    //使能UART0，清空接收FIFO，清空发送FIFO，使能UART0的FIFO
    UART_OnOffConfig(UART1,ENABLE);
    UART_FifoReset(UART1,UART_RxFIFO);
    UART_FifoReset(UART1,UART_TxFIFO);
    UART_FifoConfig(UART1,ENABLE);
    
    //回写方式关
    UART_LoopBackConfig(UART1,DISABLE);
    
    //串口0波特率，校验方式等设置
    UART_Config(UART1,gu32_BaudRateArry[gs_Setup.BaudRate_1],g_ParityArry[gs_Setup.ParityMode_1],g_StopBitsArry[gs_Setup.ParityMode_1],g_DataLenArry[gs_Setup.ParityMode_1]);
    
    //接收允许
    UART_RxConfig(UART1,ENABLE);
    
    //设置超时非空中断的超时长度
//  CLR485S;
    EnableRecIRQ_UART1(); 
    DisableSendIRQ_UART1();
    
    //设置中断的优先级，并且使能UART0中断
    EIC_IRQChannelPriorityConfig(UART1_IRQChannel,UART1_SIPL);
    EIC_IRQChannelConfig(UART1_IRQChannel,ENABLE);//
}

/***********************************************************
函 数 名：u16 BuildModbusCrc16(u8 *puchMsg, u32 usDataLen) 
功    能：冗余校验(MODBUS协议下校验算法)
说    明：
入口参数：无
调用程序: 
返 回 值：无
设    计：wbj                  日    期：2008-1-24 9:26
修    改：丛海旭               日    期：2014-1-22
***********************************************************/
u16 BuildModbusCrc16(u8 *puchMsg, u32 ulDataLen) 
{ 
    u8 uchCRCHi = 0xFF ; /* 高CRC字节初始化 */ 
    u8 uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */ 
    u32 ulIndex ; /* CRC循环中的索引 */ 
    
    while (ulDataLen--) /* 传输消息缓冲区 */ 
    { 
        ulIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */ 
        uchCRCHi = uchCRCLo ^ auchCRCHi[ulIndex] ; 
        uchCRCLo = auchCRCLo[ulIndex] ; 
    } 
    return (uchCRCHi << 8 | uchCRCLo) ; 
}

/***********************************************************
函 数 名：void BuildCrc16(u8 *fu8p_Arry, u32 fu32_Num) 
功    能：冗余校验(MODBUS协议下校验算法)
说    明：
入口参数：无
调用程序: 
返 回 值：无
设    计：丛海旭               日    期：2014-3-31
修    改：                     日    期：
***********************************************************/
void BuildCrc16(u8 *fu8p_Arry, u32 fu32_Num) 
{
    u16 lu16_CRCData;
    
    lu16_CRCData = BuildModbusCrc16(fu8p_Arry, fu32_Num);
    fu8p_Arry[fu32_Num] = (u8)(lu16_CRCData >> 8);
    fu8p_Arry[fu32_Num+1] = (u8)(lu16_CRCData & 0xFF);
}


/***********************************************************
函 数 名：void BuildLrc(u8 *fu8p_Arry, u32 fu32_Num) 
功    能：LRC校验(MODBUS协议下校验算法)
说    明：
入口参数：无
调用程序: 
返 回 值：无
设    计：丛海旭               日    期：2014-4-1
修    改：                     日    期：
***********************************************************/
void BuildLrc(u8 *fu8p_Arry, u32 fu32_Num)
{
    fu8p_Arry[fu32_Num] = BuildModbusLrc(fu8p_Arry, fu32_Num);
}

/***********************************************************
函 数 名：bool CheckModBusCrc16(u8 *puchMsg, u32 ulDataLen)
功    能：判断CRC校验
说    明：
入口参数：无
调用程序: 
返 回 值：无
设    计：wbj                  日    期：2008-1-24 9:26
修    改：丛海旭               日    期：2014-1-22
***********************************************************/
bool CheckModBusCrc16(u8 *puchMsg, u32 ulDataLen)
{
    u16 uiCRC;
        
    uiCRC=BuildModbusCrc16(puchMsg,ulDataLen);
    
    if (uiCRC == (puchMsg[ulDataLen+1] + (puchMsg[ulDataLen] << 8)))
    {
        if (puchMsg == gu8_TxdArray_UART0)       //uart0modbus通讯
            gb_ModbusAnswerFlg_Uart0 = true;      //串口参数自动匹配功能中使用
        else                                        //uart1modbus通讯
            gb_ModbusAnswerFlg_Uart1 = true;      //串口参数自动匹配功能中使用
        return(true);
    }
    else
    {
        return(false);
    }
}

/*************************************************************
函 数 名：bool Modbus_Send_RTU(u8 *fu8p_Data, u32 fu32_Num)
功    能：发送函数
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：丛海旭               日    期：2014-1-22
修    改：                     日    期：
*************************************************************/
bool Modbus_Send_RTU(u8 *fu8p_Data, u32 fu32_Num)
{
    if (fu8p_Data == gu8_TxdArray_UART0)
    {
        if (true==bUARTSending_UART0){
            return(false);
        }
        
        //如果串口正在发送的话，串口是不会有接收数据的
        DisableSendIRQ_UART0();
        DisableRecIRQ_UART0();      //关闭接收中断
    
        Sio_485_Disable();
        gb_S485Recieve = false;     //修正RS485总线Modbus ASCII方式通讯时偶尔出现中断的问题。 
    
        ulTBUFAvailNum_UART0 = fu32_Num; //更新数据个数寄存器   
        ulTBUFBegin_UART0 = 0; 
        
        ulFrameLength_UART0=0;
        
        bUARTSending_UART0=true;          //串口正在发送之中
        
        //注意一个问题，千万不要在bUARTSending_UART0置位成true之前开中断
        //否则，中断一开，然后马上进入半空和空中断，bUARTSending_UART0
        //没有置上，就马上被中断置位False了，相当于没有开。
        //同理对ulTBUFAvailNum_UART0，ulTBUFBegin_UART0也一样。
        
        EnableSendIRQ_UART0();      //打开发送中断
        
        return(true);
    }
    else
    {
        if (true==bUARTSending_UART1)
            return(false);
        
        //如果串口正在发送的话，串口是不会有接收数据的
        DisableSendIRQ_UART1();
        DisableRecIRQ_UART1();      //关闭接收中断
    
    
        ulTBUFAvailNum_UART1 = fu32_Num; //更新数据个数寄存器   
        ulTBUFBegin_UART1 = 0; 
        
        ulFrameLength_UART1=0;
        
        bUARTSending_UART1=true;          //串口正在发送之中
        
        //注意一个问题，千万不要在bUARTSending_UART0置位成true之前开中断
        //否则，中断一开，然后马上进入半空和空中断，bUARTSending_UART0
        //没有置上，就马上被中断置位False了，相当于没有开。
        //同理对ulTBUFAvailNum_UART0，ulTBUFBegin_UART0也一样。
        
        EnableSendIRQ_UART1();      //打开发送中断
        
        return(true);
    }
}

/*************************************************************
函 数 名：bool Modbus_Send_ASCII(u8 *fu8p_Data, u32 fu32_Num)
功    能：发送函数
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：丛海旭               日    期：2014-4-1
修    改：                     日    期：
*************************************************************/
bool Modbus_Send_ASCII(u8 *fu8p_Data, u32 fu32_Num)
{
    if (fu8p_Data == gu8_TxdArray_UART0)
    {
        if (true==gvb_SioTxingFlg_UART0){
            return(false);
        }
    }
    else
    {
        if (true==gvb_SioTxingFlg_UART1)
            return(false);
    }
    //fu32_Num - 1 因为ASCII方式的LRC校验只占用一个字节
    HextoASC(fu8p_Data, fu8p_Data + 1, fu32_Num - 1);
    fu8p_Data[0] = ':';   //起始符
    fu8p_Data[fu32_Num*2-1] = ENDCHAR1;
    fu8p_Data[fu32_Num*2] = ENDCHAR2;
    StartSend_Com(fu8p_Data);
    return true;
}

/*********************************************************************
函数名称: void Modbus_Error(u8 fu8_ErrorCode, u8 *fu8p_Data)
功    能: Modbus通讯错误码返回处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-22
修    改:                      时    间: 
*********************************************************************/
void Modbus_Error(u8 fu8_ErrorCode, u8 *fu8p_Data)
{
    fu8p_Data[1] += 0x80;
    fu8p_Data[2] = fu8_ErrorCode;
    FpBuildCrcLrc(fu8p_Data, 3);
    
    FpSendRtuAscII(fu8p_Data, 5);
}

/***********************************************************
函 数 名：bool ModBus_2BinBCD(u8 *fu8p_Data, u8 *fu8p_Place, u32 fu32_Value)
功    能：MODBUS协议获取要发送的2字节数据
说    明：
入口参数：无
调用程序: 
返 回 值：无
设    计：wbj                  日    期：2008-1-28 14:05
修    改：丛海旭               日    期：2011-5-12
***********************************************************/
bool ModBus_2BinBCD(u8 *fu8p_Data, u8 *fu8p_Place, u32 fu32_Value)
{
    fu8p_Data[(*fu8p_Place)++] = (u8)(fu32_Value >> 8);
    fu8p_Data[(*fu8p_Place)++] = (u8)(fu32_Value & 0xff);
    gu16_ModBusReadNumber_UART0 += 2;
    if (gu16_ModBusReadNumber_UART0 >= fu8p_Data[2])
    {
        return true;
    }
    return false;
}

/***********************************************************
函 数 名：uint Modbus_ReadStateflg(u8 statetype)
功    能：Modbus返回仪表状态
说    明：传入状态参数，返回此项状态值
入口参数：
调用程序:
返 回 值：uint
设    计：丛海旭               日    期：2014-1-22
修    改：                     日    期：
***********************************************************/
u16 Modbus_ReadStateflg(u8 statetype)
{
    u16 lu16_State = 0;

    if(statetype == 1)
    {
/*运行    */ lu16_State |=(gb_RunFlg)      ?(0x0001):(0);
/*加料前  */ lu16_State |=(gu32_Process == T1PROCESS && gb_RunFlg == true)      ?(0x0002):(0);
/*大投    */ lu16_State |=(Sp_Disc_State(SP1_STATE))        ?(0x0004):(0);
/*中投    */ lu16_State |=(Sp_Disc_State(SP2_STATE))        ?(0x0008):(0);
/*小投    */ lu16_State |=(Sp_Disc_State(SP3_STATE))        ?(0x0010):(0);
/*定值    */ lu16_State |=(gb_CompFlg)     ?(0x0020):(0);
/*超欠差  */ lu16_State |=(gb_OverFlg || gb_UnderFlg || gb_OUOverFlg || gb_OUUnderFlg)   ?(0x0040):(0);
/*报警    */ lu16_State |=(gb_AlarmFlg)          ?(0x0080):(0);
/*夹袋    */ lu16_State |=(gb_PackFlg)           ?(0x0100):(0);
/*拍袋    */ lu16_State |=(gb_FlapBagOutFlg || gb_FlapAllHighFlg)    ?(0x0200):(0);
/*卸料    */ lu16_State |=(Sp_Disc_State(DISC_STATE))    ?(0x0400):(0);
/*零区    */ lu16_State |=(gb_NzFlg)             ?(0x0800):(0);
/*供料    */ lu16_State |=(gb_FillFlg)           ?(0x1000):(0);
/*批次完成*/ lu16_State |=(gb_BatchOverFlg)      ?(0x2000):(0);
/*缺料    */ lu16_State |=(gb_LackFlg)           ?(0x4000):(0);
/*毛净重  */ lu16_State |=(gb_DispNetFlg)        ?(0x8000):(0);
    }
    else if(statetype == 2)
    {
/*稳定   */ lu16_State |=(gb_StabFlg)      ?(0x0001):(0);
/*溢出   */ lu16_State |=(gb_OflFlg)       ?(0x0002):(0);
/*负号   */ lu16_State |=(gb_SigWtFlg)       ?(0x0004):(0);
/*上料位 */ lu16_State |=(gb_HighFlg)      ?(0x0008):(0);
/*中料位 */ lu16_State |=(gb_MiddleFlg)       ?(0x0010):(0);
/*下料位 */ lu16_State |=(gb_LowFlg)    ?(0x0020):(0);
/*卸到位 */ lu16_State |=(gb_DiscCloseOKFlg)    ?(0x0040):(0);
/*夹到位 */ lu16_State |=(gb_PackOKFlg)    ?(0x0080):(0);
/*振打   */ lu16_State |=(gb_DiscShakeFlg) ?(0x0100):(0);
/*打码   */ lu16_State |=(gb_PrintCodeOutFlg)   ?(0x0200):(0);
/*截料   */ lu16_State |=(gb_Sp3Flg)       ?(0x0400):(0);
/*提袋   */ lu16_State |=(gb_LiftBagFlg)   ?(0x0800):(0);
/*推袋   */ lu16_State |=(gb_PushBagFlg)   ?(0x1000):(0);
    }

    return lu16_State;
}


/***********************************************************
函 数 名：u32 Modbus_WtOrMvData(BusWtType WtType)
功    能：返回重量和毫伏数等数据
说    明：
入口参数：
返 回 值：无
设    计：丛海旭               日    期：2014-1-22
修    改：                     日    期：
***********************************************************/
u32 Modbus_WtOrMvData(BusWtType WtType)
{
    u32 lu32_Data = 0;

    switch(WtType)
    {
        case CurWt:       //当前重量
            lu32_Data = (gb_OflFlg == true) ? OFLVAL_MB : gu32_Wt;
            break;
        case ZeroMV:      //零点毫伏数
            lu32_Data = AdToMv(gs_Cal.Zero);
            break;
        case LoadMV:      //增益毫伏数
            if (gb_NetAdFlg)
                lu32_Data = 0;
            else
                lu32_Data = (gb_AdOflFlg == true) ? (OFLVAL_MB) : (AdToMv(gu32_NetAdFedValue));
            break;
        case RealMV:      //绝对毫伏数
            lu32_Data = (gb_AdOflFlg == true) ? (OFLVAL_MB) : (AdToMv(gu32_AdFedValue));
            break;
        default:
            break;
    }
    return lu32_Data;
}

/*********************************************************************
函数名称: u8 week_day(u16 fu16_Year, u16 fu16_Month, u16 fu16_Day)
功    能: 根据日期计算星期
说    明: 
调    用: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2010-2-1
修    改:                      时    间: 
*********************************************************************/
u8 week_day(u16 fu16_Year, u16 fu16_Month, u16 fu16_Day)
{
    u8 monthday[]={0,3,3,6,1,4,6,2,5,0,3,5};
    u16 lu16_Days = (fu16_Year-1)%7 + (fu16_Year-1)/4 - (fu16_Year-1)/100 +(fu16_Year-1)/400;
    lu16_Days += (monthday[fu16_Month-1] + fu16_Day) ;
    
    if (LeapYear(fu16_Year) && fu16_Month>2)
        lu16_Days++;
    
    return lu16_Days%7;
}

/*********************************************************************
函数名称: u8 get_week(void)
功    能: 
说    明: 
调    用: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2010-2-1
修    改:                      时    间: 
*********************************************************************/
u8 get_week(void)
{
    u16 lu16_Year = Date_Time_Read(YEAR);
    u16 lu16_Month = Date_Time_Read(MONTH);
    u16 lu16_Date = Date_Time_Read(DATE);
    
    return week_day(lu16_Year, lu16_Month, lu16_Date);
}

/*********************************************************************
函数名称: u8 Date_Time_Read(u8 fu8_Type)
功    能: modbus返回日期和时间
说    明: 
调    用: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2010-3-2
修    改:                      时    间: 
*********************************************************************/
u16 Date_Time_Read(u8 fu8_Type)
{
    u16 lu16_Result;
    sTime ls_Time;
    
    RTC_ReadDateTime(&ls_Time);
    switch(fu8_Type)
    {
        case YEAR:
            lu16_Result = (ls_Time.year >> 4) * 10 + (ls_Time.year & 0x0F);
            break;
        
        case MONTH:
            lu16_Result = ((ls_Time.month & 0x1F) >> 4) * 10 + (ls_Time.month & 0x0F);
            break;
        
        case DATE:
            lu16_Result = ((ls_Time.date & 0x3F) >> 4) * 10 + (ls_Time.date & 0x0F);
            break;
            
        case WEEK:
            lu16_Result = (ls_Time.day & 0x07);
            break;
            
        case HOUR:
            lu16_Result = ((ls_Time.hour & 0x3F) >> 4) * 10 + (ls_Time.hour & 0x0F);
            break;
            
        case MINUTE:
            lu16_Result = ((ls_Time.min & 0x7F) >> 4) * 10 + (ls_Time.min & 0x0F);
            break;
            
        case SECOND:
            lu16_Result = ((ls_Time.sec & 0x7F) >> 4) * 10 + (ls_Time.sec & 0x0F);
            break;
        
        default:
            lu16_Result = 0;
            break;
    }
    
    return lu16_Result;
}

/*********************************************************************
函数名称: void Date_Time_Write(u8 fu8_Type, u16 fu16_Data)
功    能: modbus写日期和时间
说    明: 
调    用: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-5-17
修    改:                      时    间: 
*********************************************************************/
void Date_Time_Write(u8 fu8_Type, u16 fu16_Data)
{
    sTime ls_Time;
    
    RTC_ReadDateTime(&ls_Time);
    switch(fu8_Type)
    {
        case YEAR:
            ls_Time.year = ((fu16_Data / 10) << 4) | (fu16_Data % 10);
//            ls_Time.chWeek = get_week();                     //改年时重新修改星期
            break;
        
        case MONTH:
            ls_Time.month = ((fu16_Data / 10) << 4) | (fu16_Data % 10);
//            ls_Time.chWeek = get_week();                     //改月时重新修改星期
            break;
        
        case DATE:
            ls_Time.date = ((fu16_Data / 10) << 4) | (fu16_Data % 10);
//            ls_Time.chWeek = get_week();                     //改日时重新修改星期
            break;
            
        case WEEK:
            break;
            
        case HOUR:
            ls_Time.hour = ((fu16_Data / 10) << 4) | (fu16_Data % 10);
            break;
            
        case MINUTE:
            ls_Time.min = ((fu16_Data / 10) << 4) | (fu16_Data % 10);
            break;
            
        case SECOND:
            ls_Time.sec = ((fu16_Data / 10) << 4) | (fu16_Data % 10);
            break;
        
        default:
            break;
    }
    RTC_WriteDateTime(&ls_Time);
}

/*********************************************************************
函数名称: bool Judge_Date(u16 fu16_Date)
功    能: 判断日期输入是否合理
说    明: 
调    用: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2010-2-1
修    改:                      时    间: 
*********************************************************************/
bool Judge_Date(u16 fu16_Date)
{
    bool i;
    u16 lu16_Year, lu16_Month;
    //每月的天数，不包括闰年
    u8 judgedate[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};           //chx 100201

    lu16_Year = 2000 + Date_Time_Read(YEAR);
    lu16_Month = Date_Time_Read(MONTH);
    
    if (fu16_Date > judgedate[lu16_Month])
    {
        i = false;
    }
    else
    {
        i = true;
    }
    
    if (LeapYear(lu16_Year) && lu16_Month == 2 && fu16_Date == 29)        //闰年2月29号合法
    {
        i = true;
    }
    
    return i;
}

/*********************************************************************
函数名称: void ModBus_Write_2ByteOk(u16 *fu8p_Data)
功    能: modbus写单寄存器返回OK
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-5-16
修    改:                      时    间: 
*********************************************************************/
void ModBus_Write_2ByteOk(u8 *fu8p_Data)
{
    FpSendRtuAscII(fu8p_Data, 8);
}

/***********************************************************
函 数 名：void ModBus_Read_RegisterDeal(u8 *fu8p_Data, fu16_Address, u16 fu16_ReadLenth)
功    能：MODBUS协议要读取的数据组合
说    明：该函数的功能是可以用数组先把数据整理好后再发送
入口参数：无
调用程序: 
返 回 值：无
设    计：丛海旭               日    期：2014-1-22
修    改：                     日    期：
***********************************************************/
void ModBus_Read_RegisterDeal(u8 *fu8p_Data, u16 fu16_Address, u16 fu16_ReadLenth)
{
    u8 lu8_Place;
    ULIDATA MBTemp;
    bool i, j;                 //chx 090908
        
    fu8p_Data[2] = fu16_ReadLenth * 2;
    gu16_ModBusReadNumber_UART0 = 0;
    lu8_Place = 3;
    
    if (fu8p_Data == gu8_TxdArray_UART0)
    {
        //注意arm小端模式顺序
        if (gs_Setup.HiLo_2 == 1)               //低字在前  chx 090908
        {
            i = 0;
            j = 1;
        }
        else                            //高字在前  chx 090908
        {
            i = 1;
            j = 0;
        }
    }
    else
    {
        if (gs_Setup.HiLo_1 == 1)               //低字在前  chx 090908
        {
            i = 0;
            j = 1;
        }
        else                            //高字在前  chx 090908
        {
            i = 1;
            j = 0;
        }
    }
    
    switch(fu16_Address)
    {
        //RO寄存器
        case 0:     //状态1
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,Modbus_ReadStateflg(1))) break;
        case 1:     //状态2
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,Modbus_ReadStateflg(2))) break;
        case 2:     //当前重量
            if ((gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B) && gs_Setup.ResultHoldSwitch != 0 && gb_DiscWtGetedFlg == true && gb_RunFlg == true)
                MBTemp.ul = gu32_HoldWt;    //定值重量保持时显示的重量
            else
                MBTemp.ul = Modbus_WtOrMvData(CurWt);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 3:
            if ((gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B) && gs_Setup.ResultHoldSwitch != 0 && gb_DiscWtGetedFlg == true && gb_RunFlg == true)
                MBTemp.ul = gu32_HoldWt;    //定值重量保持时显示的重量
            else
            MBTemp.ul = Modbus_WtOrMvData(CurWt);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 4:     //累计次数
            MBTemp.ul = gu32_SumNum;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 5:
            MBTemp.ul = gu32_SumNum;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 6:     //累计重量
            MBTemp.ul = gu32_SumWt;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 7:
            MBTemp.ul = gu32_SumWt;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 8:    //报警信息
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gu8_DispErrorNumber)) break;
        case 9:     //总累计单位
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)cu8_ReadUnit[gu32_SumWtUnit])) break;
        case 10:    //总累计小数点
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gu32_SumWtPoint)) break;
        case 11:    //剩余批次数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gu32_BatchTimeCnt)) break;
        //R/W寄存器
        case 12:    //单位
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)cu8_ReadUnit[gs_Cal.Unit])) break;
        case 13:    //小数点位置
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Cal.Point)) break;
        case 14:    //最小分度值
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Cal.Div)) break;
        case 15:    //传感器灵敏度
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Cal.Sensor)) break;
        case 16:    //最大量程
            MBTemp.ul = gs_Cal.Cap;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i]))  break;
        case 17:
            MBTemp.ul = gs_Cal.Cap;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j]))  break;
        case 18:    //有砝码零点标定,返回当前零点毫伏数
            MBTemp.ul = Modbus_WtOrMvData(ZeroMV);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i]))  break;
        case 19:
            MBTemp.ul = Modbus_WtOrMvData(ZeroMV);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j]))  break;
        case 20:    //有砝码增益标定，返回当前增益毫伏数
            MBTemp.ul = Modbus_WtOrMvData(LoadMV);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i]))  break;
        case 21:
            MBTemp.ul = Modbus_WtOrMvData(LoadMV);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j]))  break;
        case 22:    //无砝码零点标定，返回当前零点毫伏数
            MBTemp.ul = Modbus_WtOrMvData(ZeroMV);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i]))  break;
        case 23:
            MBTemp.ul = Modbus_WtOrMvData(ZeroMV);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j]))  break;
        case 24:    //无砝码增益标定，输入增益毫伏数，返回当前绝对毫伏数
            MBTemp.ul = Modbus_WtOrMvData(RealMV);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i]))  break;
        case 25:
            MBTemp.ul = Modbus_WtOrMvData(RealMV);
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j]))  break;
        case 26:    //无砝码增益标定，输入增益重量，返回0
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 27:
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 28:    //目标值
            MBTemp.ul = gs_Rec.Target;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 29:
            MBTemp.ul = gs_Rec.Target;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 30:    //大投提前量
            MBTemp.ul = gs_Rec.Large;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 31:
            MBTemp.ul = gs_Rec.Large;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 32:    //中投提前量
            MBTemp.ul = gs_Rec.Middle;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 33:
            MBTemp.ul = gs_Rec.Middle;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 34:    //落差值
            MBTemp.ul = gs_Rec.Small;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 35:
            MBTemp.ul = gs_Rec.Small;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 36:    //零区值
            MBTemp.ul = gs_Rec.NearZero;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 37:
            MBTemp.ul = gs_Rec.NearZero;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 38:    //拍袋起始重量
            MBTemp.ul = gs_Rec.FlapBagWt;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 39:
            MBTemp.ul = gs_Rec.FlapBagWt;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 40:    //超量值
            MBTemp.ul = gs_Rec.OverValue;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 41:
            MBTemp.ul = gs_Rec.OverValue;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 42:    //欠量值
            MBTemp.ul = gs_Rec.UnderValue;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 43:
            MBTemp.ul = gs_Rec.UnderValue;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 44:    //T1
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.TimeT1)) break;
        case 45:    //T2b
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.TimeT2b)) break;
        case 46:    //T3
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.TimeT3)) break;
        case 47:    //T4
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.TimeT4)) break;
        case 48:    //T5
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.TimeT5)) break;
        case 49:    //T6
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.TimeT6)) break;
        case 50:    //T7
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.TimeT7)) break;
        case 51:    //落差修正次数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.CorrectFallTime)) break;
        case 52:    //落差修正范围
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.CorrectFallRng1)) break;
        case 53:    //落差修正幅度
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.CorrectFallRng2)) break;
        case 54:    //拍袋开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.FlapBagSwitch)) break;
        case 55:    //加料中拍袋次数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.FlapBagAddNum)) break;
        case 56:    //定值后拍袋次数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.FlapBagNum)) break;
        case 57:    //拍袋延时
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.FlapBagDelay)) break;
        case 58:    //拍袋有效电平
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.FlapBagTimeHi)) break;
        case 59:    //拍袋无效电平
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.FlapBagTimeLow)) break;
        case 60:    //小投点动有效电平时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.SmallPluseHi)) break;
        case 61:    //小投点动无效电平时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.SmallPluseLow)) break;
        case 62:    //单秤组合次数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.CombinedNum)) break;
        case 63:    //自动清零间隔
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Setup.AutoClrZeroInterval)) break;
        case 64:    //AD数字滤波参数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Setup.AdFilterLevel)) break;
        case 65:    //判稳范围
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Setup.JudgeStabRng)) break;
        case 66:    //清零范围
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Setup.ClrZeroRng)) break;
        case 67:    //零点跟踪范围
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Setup.TraceZeroRng)) break;
        case 68:    //配方号
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gu32_RecNum)) break;
        case 69:    //批次数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gu32_BatchTime)) break;
        case 70:    //额外拍袋输出有效时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,(u16)gs_Rec.FlapBagExtTime)) break;
        case 71:    //年
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,Date_Time_Read(YEAR))) break;
        case 72:    //月
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,Date_Time_Read(MONTH))) break;
        case 73:    //日
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,Date_Time_Read(DATE))) break;
        case 74:    //星期
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,get_week())) break;
        case 75:   //时
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,Date_Time_Read(HOUR))) break;
        case 76:   //分
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,Date_Time_Read(MINUTE))) break;
        case 77:   //秒
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,Date_Time_Read(SECOND))) break;
        case 78:       //启动/结束开关量测试
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gu16_MbIoTestSwitch)) break;
        case 79:       //输入开关量测试
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gu16_MbIoInState)) break;
        case 80:       //输出开关量测试
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gu16_MbIoOutState)) break;
        case 81:       //IN1
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoInBuf[1])) break;
        case 82:       //IN2
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoInBuf[2])) break;
        case 83:       //IN3
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoInBuf[3])) break;
        case 84:       //IN4
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoInBuf[4])) break;
        case 85:       //IN5
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoInBuf[5])) break;
        case 86:       //IN6
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoInBuf[6])) break;
        case 87:       //IN7
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoInBuf[7])) break;
        case 88:       //IN8
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoInBuf[8])) break;
        case 89:       //OUT1
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[1])) break;
        case 90:       //OUT2
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[2])) break;
        case 91:       //OUT3
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[3])) break;
        case 92:       //OUT4
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[4])) break;
        case 93:       //OUT5
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[5])) break;
        case 94:       //OUT6
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[6])) break;
        case 95:       //OUT7
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[7])) break;
        case 96:       //OUT8
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[8])) break;
        case 97:       //OUT9
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[9])) break;
        case 98:       //OUT10
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[10])) break;
        case 99:       //OUT11
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[11])) break;
        case 100:       //OUT12
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,*gu32p_IoOutBuf[12])) break;
        case 101:    //秤体模式
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.TareMode)) break;
        case 102:    //无斗毛净重
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.GorNMode)) break;
        case 103:    //给料方式
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.FeedMode)) break;
        case 104:    //运行状态判稳超时时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.JudgeStabOverTime)) break;
        case 105:    //副显示内容
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.AuxDispType)) break;
        case 106:    //卸料滤波级数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.AdFilterLevelDisc)) break;
        case 107:    //定值滤波级数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.AdFilterLevelComp)) break;
        case 108:    //AD采样速度
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.AdSamplingRate)) break;
        case 109:    //上电自动清零开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PowerOnZeroSwitch)) break;
        case 110:    //手动卸料累计开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.DiscSumSwitch)) break;
        case 111:    //有斗定值重量保持开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.ResultHoldSwitch)) break;
        case 112:    //累计查询密码开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.SumCheckCodeSwitch)) break;
        case 113:    //累计清除密码开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.SumClrCodeSwitch)) break;
        case 114:    //累计范围扩展开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.SumExtendSwitch)) break;
        case 115:    //串行口1自动打印开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PrintSwitch_1)) break;
        case 116:    //串行口1打印机类型
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PrintType_1)) break;
        case 117:    //串行口1打印语言
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PrintLanguage_1)) break;
        case 118:    //串行口1打印走纸行数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PrintBlankRows_1)) break;
        case 119:    //串行口2自动打印开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PrintSwitch_2)) break;
        case 120:    //串行口2打印机类型
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PrintType_2)) break;
        case 121:    //串行口2打印语言
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PrintLanguage_2)) break;
        case 122:    //串行口2打印走纸行数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.PrintBlankRows_2)) break;
        case 123:    //双无斗秤互锁模式同时松袋开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.No2PackOffTwoSwitch)) break;
        case 124:    //输送机启动延时时间t13
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.No2ConveyorDelayTime)) break;
        case 125:    //输送机运行时间t14
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.No2ConveyorRunTime)) break;
        case 126:    //再次夹袋延时启动加料时间t15
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Setup.No2AddAgainDelayTime)) break;
        case 127:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 128:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 129:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 130:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 131:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 132:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 133:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 134:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 135:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 136:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 137:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 138:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 139:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 140:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 141:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 142:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 143:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
                
        case 144:    //T2_a
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.TimeT2a)) break;
        case 145:    //T9提袋延时时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.TimeT9)) break;
        case 146:    //T10提袋输出有效时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.TimeT10)) break;
        case 147:    //T11推袋延时时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.TimeT11)) break;
        case 148:    //T12推袋输出有效时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.TimeT12)) break;
        case 149:    //卸料振打开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.DiscShakeSwitch)) break;
        case 150:    //卸料超时时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.DiscEffTime)) break;
        case 151:    //卸料振打有效时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.DiscShakeTimeHi)) break;
        case 152:    //卸料振打无效时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.DiscShakeTimeLow)) break;
        case 153:    //卸料振打次数
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.DiscShakeNum)) break;
        case 154:    //打码控制功能开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.PrintCodeSwitch)) break;
        case 155:    //打码启动延时TP1
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.PrintCodeRunDelay)) break;
        case 156:    //打码输出有效时间TP2
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.PrintCodeTime)) break;
        case 157:    //打码时不允许卸料/加料 开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.SheildDiscSwitch)) break;
        case 158:    //超欠差报警开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.OUSwitch)) break;
        case 159:    //超欠差暂停开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.OUStopSwitch)) break;
        case 160:    //落差修正开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.CorrectFallSwitch)) break;
        case 161:    //小投点动开关
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_Rec.SmallPluseSwitch)) break;
                
        case 162:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 163:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 164:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 165:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 166:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 167:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 168:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 169:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 170:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 171:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 172:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 173:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 174:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 175:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 176:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 177:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 178:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 179:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 180:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 181:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 182:    //
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
                
        case 183:    //总累计数据打印
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 184:    //配方设置表打印
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 185:    //配方累计报表打印
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 186:    //恢复出厂设置
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 187:
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
        case 188:    //上次包装结果
            MBTemp.ul = gu32_MbLastResult;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 189:
            MBTemp.ul = gu32_MbLastResult;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 190:   //配方1累计次数
            MBTemp.ul = gu32_SumNum_Rec[1];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 191:
            MBTemp.ul = gu32_SumNum_Rec[1];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 192:   //配方1累计重量
            MBTemp.ul = gu32_SumWt_Rec[1];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 193:
            MBTemp.ul = gu32_SumWt_Rec[1];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 194:   //配方2累计次数
            MBTemp.ul = gu32_SumNum_Rec[2];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 195:
            MBTemp.ul = gu32_SumNum_Rec[2];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 196:   //配方2累计重量
            MBTemp.ul = gu32_SumWt_Rec[2];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 197:
            MBTemp.ul = gu32_SumWt_Rec[2];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 198:   //配方3累计次数
            MBTemp.ul = gu32_SumNum_Rec[3];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 199:
            MBTemp.ul = gu32_SumNum_Rec[3];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 200:   //配方3累计重量
            MBTemp.ul = gu32_SumWt_Rec[3];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 201:
            MBTemp.ul = gu32_SumWt_Rec[3];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 202:   //配方4累计次数
            MBTemp.ul = gu32_SumNum_Rec[4];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 203:
            MBTemp.ul = gu32_SumNum_Rec[4];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 204:   //配方4累计重量
            MBTemp.ul = gu32_SumWt_Rec[4];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 205:
            MBTemp.ul = gu32_SumWt_Rec[4];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 206:   //配方5累计次数
            MBTemp.ul = gu32_SumNum_Rec[5];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 207:
            MBTemp.ul = gu32_SumNum_Rec[5];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 208:   //配方5累计重量
            MBTemp.ul = gu32_SumWt_Rec[5];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 209:
            MBTemp.ul = gu32_SumWt_Rec[5];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 210:   //配方6累计次数
            MBTemp.ul = gu32_SumNum_Rec[6];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 211:
            MBTemp.ul = gu32_SumNum_Rec[6];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 212:   //配方6累计重量
            MBTemp.ul = gu32_SumWt_Rec[6];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 213:
            MBTemp.ul = gu32_SumWt_Rec[6];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 214:   //配方7累计次数
            MBTemp.ul = gu32_SumNum_Rec[7];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 215:
            MBTemp.ul = gu32_SumNum_Rec[7];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 216:   //配方7累计重量
            MBTemp.ul = gu32_SumWt_Rec[7];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 217:
            MBTemp.ul = gu32_SumWt_Rec[7];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 218:   //配方8累计次数
            MBTemp.ul = gu32_SumNum_Rec[8];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 219:
            MBTemp.ul = gu32_SumNum_Rec[8];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 220:   //配方8累计重量
            MBTemp.ul = gu32_SumWt_Rec[8];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 221:
            MBTemp.ul = gu32_SumWt_Rec[8];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 222:   //配方9累计次数
            MBTemp.ul = gu32_SumNum_Rec[9];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 223:
            MBTemp.ul = gu32_SumNum_Rec[9];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 224:   //配方9累计重量
            MBTemp.ul = gu32_SumWt_Rec[9];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 225:
            MBTemp.ul = gu32_SumWt_Rec[9];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 226:   //配方10累计次数
            MBTemp.ul = gu32_SumNum_Rec[10];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 227:
            MBTemp.ul = gu32_SumNum_Rec[10];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 228:   //配方10累计重量
            MBTemp.ul = gu32_SumWt_Rec[10];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 229:
            MBTemp.ul = gu32_SumWt_Rec[10];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 230:   //配方11累计次数
            MBTemp.ul = gu32_SumNum_Rec[11];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 231:
            MBTemp.ul = gu32_SumNum_Rec[11];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 232:   //配方11累计重量
            MBTemp.ul = gu32_SumWt_Rec[11];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 233:
            MBTemp.ul = gu32_SumWt_Rec[11];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 234:   //配方12累计次数
            MBTemp.ul = gu32_SumNum_Rec[12];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 235:
            MBTemp.ul = gu32_SumNum_Rec[12];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 236:   //配方12累计重量
            MBTemp.ul = gu32_SumWt_Rec[12];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 237:
            MBTemp.ul = gu32_SumWt_Rec[12];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 238:   //配方13累计次数
            MBTemp.ul = gu32_SumNum_Rec[13];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 239:
            MBTemp.ul = gu32_SumNum_Rec[13];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 240:   //配方13累计重量
            MBTemp.ul = gu32_SumWt_Rec[13];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 241:
            MBTemp.ul = gu32_SumWt_Rec[13];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 242:   //配方14累计次数
            MBTemp.ul = gu32_SumNum_Rec[14];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 243:
            MBTemp.ul = gu32_SumNum_Rec[14];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 244:   //配方14累计重量
            MBTemp.ul = gu32_SumWt_Rec[14];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 245:
            MBTemp.ul = gu32_SumWt_Rec[14];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 246:   //配方15累计次数
            MBTemp.ul = gu32_SumNum_Rec[15];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 247:
            MBTemp.ul = gu32_SumNum_Rec[15];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 248:   //配方15累计重量
            MBTemp.ul = gu32_SumWt_Rec[15];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 249:
            MBTemp.ul = gu32_SumWt_Rec[15];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 250:   //配方16累计次数
            MBTemp.ul = gu32_SumNum_Rec[16];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 251:
            MBTemp.ul = gu32_SumNum_Rec[16];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 252:   //配方16累计重量
            MBTemp.ul = gu32_SumWt_Rec[16];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 253:
            MBTemp.ul = gu32_SumWt_Rec[16];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 254:   //配方17累计次数
            MBTemp.ul = gu32_SumNum_Rec[17];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 255:
            MBTemp.ul = gu32_SumNum_Rec[17];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 256:   //配方17累计重量
            MBTemp.ul = gu32_SumWt_Rec[17];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 257:
            MBTemp.ul = gu32_SumWt_Rec[17];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 258:   //配方18累计次数
            MBTemp.ul = gu32_SumNum_Rec[18];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 259:
            MBTemp.ul = gu32_SumNum_Rec[18];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 260:   //配方18累计重量
            MBTemp.ul = gu32_SumWt_Rec[18];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 261:
            MBTemp.ul = gu32_SumWt_Rec[18];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 262:   //配方19累计次数
            MBTemp.ul = gu32_SumNum_Rec[19];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 263:
            MBTemp.ul = gu32_SumNum_Rec[19];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 264:   //配方19累计重量
            MBTemp.ul = gu32_SumWt_Rec[19];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 265:
            MBTemp.ul = gu32_SumWt_Rec[19];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 266:   //配方20累计次数
            MBTemp.ul = gu32_SumNum_Rec[20];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 267:
            MBTemp.ul = gu32_SumNum_Rec[20];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 268:   //配方20累计重量
            MBTemp.ul = gu32_SumWt_Rec[20];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 269:
            MBTemp.ul = gu32_SumWt_Rec[20];
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        case 270:   //快加时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_DebugRunTime.u32Sp1Time)) break;
        case 271:   //中加时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_DebugRunTime.u32Sp2Time)) break;
        case 272:   //慢加时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_DebugRunTime.u32Sp3Time)) break;
        case 273:   //启动到定值时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_DebugRunTime.u32RunCmpTime)) break;
        case 274:   //一次加料过程时间
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,gs_DebugRunTime.u32OneTareTime)) break;
        case 275:   //定值时刻重量
            MBTemp.ul = gu32_HoldWt;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[i])) break;
        case 276:   //定值时刻重量
            MBTemp.ul = gu32_HoldWt;
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,MBTemp.ui[j])) break;
        default:
            if (ModBus_2BinBCD(fu8p_Data,&lu8_Place,0)) break;
            break;
    }
    FpBuildCrcLrc(fu8p_Data, (fu8p_Data[2] + 3));
    
    FpSendRtuAscII(fu8p_Data, fu8p_Data[2] + 5);
}

/*********************************************************************
函数名称: void ModBus_Read_Register(u8 *fu8p_Data)
功    能: modbus读保持寄存器的合理性判断
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-22
修    改:                      时    间: 
*********************************************************************/
void ModBus_Read_Register(u8 *fu8p_Data)
{
    u16 lu16_Address;
    u16 lu16_ReadLenth;
    u16 lu16_ReadNum;
    
    lu16_Address = (fu8p_Data[2] << 8) + fu8p_Data[3];
    lu16_ReadLenth = (fu8p_Data[4] << 8) + fu8p_Data[5];
    lu16_ReadNum = R2BYTETOTAL - lu16_Address;
    
    //可读地址范围的合理性判断
    if ((lu16_Address <= R2BYTETOTAL - 1)
        && (lu16_ReadLenth <= lu16_ReadNum) && (lu16_ReadLenth <= READMAXNUM))
    {
        ModBus_Read_RegisterDeal(fu8p_Data, lu16_Address, lu16_ReadLenth);
    }
    else
    {
        Modbus_Error(DATAPLACEERRO, fu8p_Data);
    }
}


/*********************************************************************
函数名称: u16 ModBus_Write_2ByteDeal(u16 *fu8p_Data)
功    能: modbus写单寄存器地址处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-22
修    改:                      时    间: 
*********************************************************************/
u16 ModBus_Write_2ByteDeal(u8 *fu8p_Data)
{
    u16 lu16_Address;
    u16 lu16_WriteData;
    u32 lu32_Temp;
    
    //写入的地址
    lu16_Address = fu8p_Data[2];
    lu16_Address <<= 8;
    lu16_Address += fu8p_Data[3];   
    //写入的数据
    lu16_WriteData = fu8p_Data[4];
    lu16_WriteData <<= 8;
    lu16_WriteData += fu8p_Data[5];

    switch (lu16_Address)
    {
        case 12:        //标定单位
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (lu16_WriteData  <= 3)
            {
                if (lu16_WriteData != cu8_ReadUnit[gs_Cal.Unit])
                {
                    gs_Cal.Unit = cu8_WriteUnit[lu16_WriteData];
                    FRAM_WriteDW(FMAddr_CalUnit, gs_Cal.Unit);
                    ClrSum();
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 13:        //标定小数点
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (lu16_WriteData <= 4)
            {
                if (lu16_WriteData != gs_Cal.Point)
                {
                    gs_Cal.Point = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_CalPoint, gs_Cal.Point);
                    ClrSum();
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 14:        //最小分度值
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if ((lu16_WriteData==1)||(lu16_WriteData==2)||(lu16_WriteData==5)||
               (lu16_WriteData==10)||(lu16_WriteData==20)|(lu16_WriteData==50))
            {
                if (lu16_WriteData != gs_Cal.Div)
                {
//                    UpdateRecF1(lu16_WriteData);  //分度修改更新配方F1  //for test
                    lu32_Temp = FRAM_ReadDW(FMAddr_CalDiv);
                    //需要调整分度,和追零零点
                    gs_Cal.ADK *= (float)lu16_WriteData;    //新分度值=原分度值*new dd/old dd
                    gs_Cal.ADK /= (float)gs_Cal.Div;
                    FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);
                
                    //调整追零零点:调整分度值后,如果是0,仍然显示零
                    gu32_FloatZero *= gs_Cal.Div;
                    gu32_FloatZero /= lu16_WriteData;
                    //保存新的分度值
                    gs_Cal.Div = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_CalDiv, gs_Cal.Div);
    
                    //分度值改小后的最大量程处理
                    if (gs_Cal.Div < lu32_Temp)                        //分度值由大变小时最大量程需要调整
                    {
                        if (gs_Cal.Cap > (gs_Cal.Div * 100000))
                        {
                            gs_Cal.Cap = gs_Cal.Div * 100000;
                            FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                        }
                    }
                    else if (gs_Cal.Div > gs_Cal.Cap)            //修改分度后量程小于分度，则修改量程为分度值
                    {
                        gs_Cal.Cap = gs_Cal.Div;
                        FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                    }
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 15:        //传感器灵敏度
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if ((lu16_WriteData >= 1) && (lu16_WriteData <= 3))
            {
                if (lu16_WriteData != gs_Cal.Sensor)
                {
                    gs_Cal.Sensor = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_CalSensor, gs_Cal.Sensor);
                }
                return RESPONSEOK;
            }
            else 
                return DATAERRO;
        case 44:        //T1
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT1)
                {
                    gs_Rec.TimeT1 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT1(gu32_RecNum), gs_Rec.TimeT1);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 45:        //T2b
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT2b)
                {
                    gs_Rec.TimeT2b = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT2b(gu32_RecNum), gs_Rec.TimeT2b);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 46:        //T3
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT3)
                {
                    gs_Rec.TimeT3 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT3(gu32_RecNum), gs_Rec.TimeT3);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 47:        //T4
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT4)
                {
                    gs_Rec.TimeT4 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT4(gu32_RecNum), gs_Rec.TimeT4);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 48:        //T5
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT5)
                {
                    gs_Rec.TimeT5 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT5(gu32_RecNum), gs_Rec.TimeT5);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 49:        //T6
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT6)
                {
                    gs_Rec.TimeT6 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT6(gu32_RecNum), gs_Rec.TimeT6);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 50:        //T7
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT7)
                {
                    gs_Rec.TimeT7 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT7(gu32_RecNum), gs_Rec.TimeT7);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 51:        //落差修正次数
            if (lu16_WriteData <= 99)
            {
                if (lu16_WriteData != gs_Rec.CorrectFallTime)
                {
                    gs_Rec.CorrectFallTime = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_CorrectFallTime(gu32_RecNum), gs_Rec.CorrectFallTime);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 52:        //落差修正范围
            if (lu16_WriteData <= 99)
            {
                if (lu16_WriteData != gs_Rec.CorrectFallRng1)
                {
                    gs_Rec.CorrectFallRng1 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_CorrectFallRng1(gu32_RecNum), gs_Rec.CorrectFallRng1);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 53:        //落差修正幅度
            if ((lu16_WriteData >= 1) && (lu16_WriteData <= 3))
            {
                if (lu16_WriteData != gs_Rec.CorrectFallRng2)
                {
                    gs_Rec.CorrectFallRng2 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_CorrectFallRng2(gu32_RecNum), gs_Rec.CorrectFallRng2);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 54:        //拍袋开关
            if (lu16_WriteData <= 3)
            {
                if (lu16_WriteData != gs_Rec.FlapBagSwitch)
                {
                    gs_Rec.FlapBagSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_FlapBagSwitch(gu32_RecNum), gs_Rec.FlapBagSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 55:        //加料中拍袋次数
            if (lu16_WriteData <= 99)
            {
                if (lu16_WriteData != gs_Rec.FlapBagAddNum)
                {
                    gs_Rec.FlapBagAddNum = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_FlapBagAddNum(gu32_RecNum), gs_Rec.FlapBagAddNum);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 56:        //定值后拍袋次数
            if (lu16_WriteData <= 99)
            {
                if (lu16_WriteData != gs_Rec.FlapBagNum)
                {
                    gs_Rec.FlapBagNum = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_FlapBagNum(gu32_RecNum), gs_Rec.FlapBagNum);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 57:        //拍袋延时
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.FlapBagDelay)
                {
                    gs_Rec.FlapBagDelay = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_FlapBagDelay(gu32_RecNum), gs_Rec.FlapBagDelay);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 58:        //拍袋有效电平时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.FlapBagTimeHi)
                {
                    gs_Rec.FlapBagTimeHi = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_FlapBagTimeHi(gu32_RecNum), gs_Rec.FlapBagTimeHi);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 59:        //拍袋无效电平时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.FlapBagTimeLow)
                {
                    gs_Rec.FlapBagTimeLow = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_FlapBagTimeLow(gu32_RecNum), gs_Rec.FlapBagTimeLow);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 60:        //小投点动有效时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.SmallPluseHi)
                {
                    gs_Rec.SmallPluseHi = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_SmallPluseHi(gu32_RecNum), gs_Rec.SmallPluseHi);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 61:        //小投点动无效时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.SmallPluseLow)
                {
                    gs_Rec.SmallPluseLow = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_SmallPluseLow(gu32_RecNum), gs_Rec.SmallPluseLow);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 62:        //单秤组合次数
            if (lu16_WriteData <= 99)
            {
                if (lu16_WriteData != gs_Rec.CombinedNum)
                {
                    gs_Rec.CombinedNum = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_CombinedNum(gu32_RecNum), gs_Rec.CombinedNum);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 63:        //自动清零间隔
            if (lu16_WriteData <= 99)
            {
                if (lu16_WriteData != gs_Setup.AutoClrZeroInterval)
                {
                    gs_Setup.AutoClrZeroInterval = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_AutoClrZeroInterval, gs_Setup.AutoClrZeroInterval);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 64:        //AD数字滤波参数
            if (lu16_WriteData <= 9)
            {
                if (lu16_WriteData != gs_Setup.AdFilterLevel)
                {
                    gs_Setup.AdFilterLevel = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_AdFilterLevel, gs_Setup.AdFilterLevel);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 65:        //判稳范围
            if (lu16_WriteData <= 99)
            {
                if (lu16_WriteData != gs_Setup.JudgeStabRng)
                {
                    gs_Setup.JudgeStabRng = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_JudgeStabRng, gs_Setup.JudgeStabRng);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 66:        //清零范围
            if ((lu16_WriteData >= 1) && (lu16_WriteData <= 99))
            {
                if (lu16_WriteData != gs_Setup.ClrZeroRng)
                {
                    gs_Setup.ClrZeroRng = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_ClrZeroRng, gs_Setup.ClrZeroRng);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 67:        //零点跟踪范围
            if (lu16_WriteData <= 9)
            {
                if (lu16_WriteData != gs_Setup.TraceZeroRng)
                {
                    gs_Setup.TraceZeroRng = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TraceZeroRng, gs_Setup.TraceZeroRng);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 68:        //配方号
            if (lu16_WriteData <= 20 && lu16_WriteData >= 1)
            {
                if (lu16_WriteData != gu32_RecNum)
                {
                    gu32_RecNum = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_RecNum, gu32_RecNum);
                    Read_RecipeData(gu32_RecNum);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 69:        //批次数
            if (lu16_WriteData <= 9999)
            {
                if (lu16_WriteData != gu32_BatchTime)
                {
                    gu32_BatchTime = lu16_WriteData;
                    gu32_BatchTimeCnt = gu32_BatchTime;
                    FRAM_WriteDW(FMAddr_BatchTime, gu32_BatchTime);
                    FRAM_WriteDW(FMAddr_BatchTimeCnt, gu32_BatchTimeCnt);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 70:        //额外拍袋输出有效时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.FlapBagExtTime)
                {
                    gs_Rec.FlapBagExtTime = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_FlapBagExtTime(gu32_RecNum), gs_Rec.FlapBagExtTime);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 71:        //年
            if (lu16_WriteData <= 99)
            {
                Date_Time_Write(YEAR, lu16_WriteData);
                FRAM_WriteDW(FMAddr_CmpYear, lu16_WriteData);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 72:        //月
            if ((lu16_WriteData >= 1) && (lu16_WriteData <= 12))
            {
                Date_Time_Write(MONTH, lu16_WriteData);
                FRAM_WriteDW(FMAddr_CmpMonth, lu16_WriteData);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 73:        //日 
            if (Judge_Date(lu16_WriteData))
            {
                Date_Time_Write(DATE, lu16_WriteData);
                FRAM_WriteDW(FMAddr_CmpDate, lu16_WriteData);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 74:        //星期
            return RESPONSEOK;
        case 75:       //时
            if (lu16_WriteData <= 23)
            {
                Date_Time_Write(HOUR, lu16_WriteData);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 76:       //分
            if (lu16_WriteData <= 59)
            {
                Date_Time_Write(MINUTE, lu16_WriteData);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 77:       //秒
            if (lu16_WriteData <= 59)
            {
                Date_Time_Write(SECOND, lu16_WriteData);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 78:       //启动/结束开关量测试
            if (lu16_WriteData <= 1)
            {
                if (gb_RunFlg == false)
                {
                    if (lu16_WriteData != gu16_MbIoTestSwitch)
                    {
                        gu16_MbIoOutTest = 0;
                        gu16_MbIoTestSwitch = lu16_WriteData;
                    }
                    return RESPONSEOK;
                }
                else
                    return NOSUCCESS;
            }
            else
                return DATAERRO;
        case 79:       //输入开关量测试
            return NOSUCCESS;
        case 80:       //输出开关量测试
            if (gu16_MbIoTestSwitch == 1)
            {
                gu16_MbIoOutTest = lu16_WriteData;
                return RESPONSEOK;
            }
            else
            {
                gu16_MbIoOutTest = 0;
                return NOSUCCESS;
            }
        //开关量自定义
        case 81:       //IN1
            if (lu16_WriteData <= IO_INMAX)
            {
                *gu32p_IoInBuf[1] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_IN, 1, gs_Setup.TareMode), *gu32p_IoInBuf[1]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 82:       //IN2
            if (lu16_WriteData <= IO_INMAX)
            {
                *gu32p_IoInBuf[2] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_IN, 2, gs_Setup.TareMode), *gu32p_IoInBuf[2]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 83:       //IN3
            if (lu16_WriteData <= IO_INMAX)
            {
                *gu32p_IoInBuf[3] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_IN, 3, gs_Setup.TareMode), *gu32p_IoInBuf[3]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 84:       //IN4
            if (lu16_WriteData <= IO_INMAX)
            {
                *gu32p_IoInBuf[4] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_IN, 4, gs_Setup.TareMode), *gu32p_IoInBuf[4]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 85:       //IN5
            if (lu16_WriteData <= IO_INMAX)
            {
                *gu32p_IoInBuf[5] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_IN, 5, gs_Setup.TareMode), *gu32p_IoInBuf[5]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 86:       //IN6
            if (lu16_WriteData <= IO_INMAX)
            {
                *gu32p_IoInBuf[6] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_IN, 6, gs_Setup.TareMode), *gu32p_IoInBuf[6]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 87:       //IN7
            if (lu16_WriteData <= IO_INMAX)
            {
                *gu32p_IoInBuf[7] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_IN, 7, gs_Setup.TareMode), *gu32p_IoInBuf[7]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 88:       //IN8
            if (lu16_WriteData <= IO_INMAX)
            {
                *gu32p_IoInBuf[8] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_IN, 8, gs_Setup.TareMode), *gu32p_IoInBuf[8]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 89:       //OUT1
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[1] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 1, gs_Setup.TareMode), *gu32p_IoOutBuf[1]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 90:       //OUT2
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[2] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 2, gs_Setup.TareMode), *gu32p_IoOutBuf[2]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 91:       //OUT3
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[3] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 3, gs_Setup.TareMode), *gu32p_IoOutBuf[3]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 92:       //OUT4
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[4] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 4, gs_Setup.TareMode), *gu32p_IoOutBuf[4]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 93:       //OUT5
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[5] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 5, gs_Setup.TareMode), *gu32p_IoOutBuf[5]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 94:       //OUT6
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[6] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 6, gs_Setup.TareMode), *gu32p_IoOutBuf[6]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 95:       //OUT7
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[7] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 7, gs_Setup.TareMode), *gu32p_IoOutBuf[7]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 96:       //OUT8
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[8] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 8, gs_Setup.TareMode), *gu32p_IoOutBuf[8]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 97:       //OUT9
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[9] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 9, gs_Setup.TareMode), *gu32p_IoOutBuf[9]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 98:       //OUT10
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[10] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 10, gs_Setup.TareMode), *gu32p_IoOutBuf[10]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 99:       //OUT11
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[11] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 11, gs_Setup.TareMode), *gu32p_IoOutBuf[11]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 100:       //OUT12
            if (lu16_WriteData <= IO_OUTMAX)
            {
                *gu32p_IoOutBuf[12] = lu16_WriteData;
                FRAM_WriteDW(GetIoAddr( IO_OUT, 12, gs_Setup.TareMode), *gu32p_IoOutBuf[12]);
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 101:        //秤体模式
            if (lu16_WriteData <= 5)
            {
                if (lu16_WriteData != gs_Setup.TareMode)
                {
                    gs_Setup.TareMode = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TareMode, gs_Setup.TareMode);
                    TareModeChange(gs_Setup.TareMode);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 102:        //无斗毛净重模式
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.GorNMode)
                {
                    gs_Setup.GorNMode = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_GorNMode, gs_Setup.GorNMode);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 103:        //给料方式选择
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.FeedMode)
                {
                    gs_Setup.FeedMode = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_FeedMode, gs_Setup.FeedMode);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 104:        //运行状态判稳超时时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Setup.JudgeStabOverTime)
                {
                    gs_Setup.JudgeStabOverTime = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_JudgeStabOverTime, gs_Setup.JudgeStabOverTime);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 105:        //副显示内容
            if (lu16_WriteData <= 2)
            {
                if (lu16_WriteData != gs_Setup.AuxDispType)
                {
                    gs_Setup.AuxDispType = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_AuxDispType, gs_Setup.AuxDispType);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 106:        //卸料滤波级数
            if (lu16_WriteData <= 9)
            {
                if (lu16_WriteData != gs_Setup.AdFilterLevelDisc)
                {
                    gs_Setup.AdFilterLevelDisc = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_AdFilterLevelDisc, gs_Setup.AdFilterLevelDisc);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 107:        //定值滤波级数
            if (lu16_WriteData <= 9)
            {
                if (lu16_WriteData != gs_Setup.AdFilterLevelComp)
                {
                    gs_Setup.AdFilterLevelComp = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_AdFilterLevelComp, gs_Setup.AdFilterLevelComp);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 108:        //AD采样速度
            if (lu16_WriteData <= 3)
            {
                if (lu16_WriteData != gs_Setup.AdSamplingRate)
                {
                    gs_Setup.AdSamplingRate = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_AdSamplingRate, gs_Setup.AdSamplingRate);
                    gb_AdOKFlg = AD_Init(gs_Setup.AdSamplingRate);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 109:        //上电自动清零开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.PowerOnZeroSwitch)
                {
                    gs_Setup.PowerOnZeroSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PowerOnZeroSwitch, gs_Setup.PowerOnZeroSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 110:        //手动卸料累计开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.DiscSumSwitch)
                {
                    gs_Setup.DiscSumSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_DiscSumSwitch, gs_Setup.DiscSumSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 111:        //手动卸料累计开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.ResultHoldSwitch)
                {
                    gs_Setup.ResultHoldSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_ResultHoldSwitch, gs_Setup.ResultHoldSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 112:        //累计查询密码开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.SumCheckCodeSwitch)
                {
                    gs_Setup.SumCheckCodeSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_SumCheckCodeSwitch, gs_Setup.SumCheckCodeSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 113:        //累计清除密码开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.SumClrCodeSwitch)
                {
                    gs_Setup.SumClrCodeSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_SumClrCodeSwitch, gs_Setup.SumClrCodeSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 114:        //累计范围扩展开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.SumExtendSwitch)
                {
                    gs_Setup.SumExtendSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_SumExtendSwitch, gs_Setup.SumExtendSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 115:        //串行口1自动打印开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.PrintSwitch_1)
                {
                    gs_Setup.PrintSwitch_1 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintSwitch_1, gs_Setup.PrintSwitch_1);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 116:        //串行口1打印机格式
            if (lu16_WriteData <= 2)
            {
                if (lu16_WriteData != gs_Setup.PrintType_1)
                {
                    gs_Setup.PrintType_1 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintType_1, gs_Setup.PrintType_1);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 117:        //串行口1打印语言
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.PrintLanguage_1)
                {
                    gs_Setup.PrintLanguage_1 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintLanguage_1, gs_Setup.PrintLanguage_1);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 118:        //串行口1打印走纸行数
            if (lu16_WriteData <= 9)
            {
                if (lu16_WriteData != gs_Setup.PrintBlankRows_1)
                {
                    gs_Setup.PrintBlankRows_1 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintBlankRows_1, gs_Setup.PrintBlankRows_1);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 119:        //串行口2自动打印开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.PrintSwitch_2)
                {
                    gs_Setup.PrintSwitch_2 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintSwitch_2, gs_Setup.PrintSwitch_2);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 120:        //串行口2打印机格式
            if (lu16_WriteData <= 2)
            {
                if (lu16_WriteData != gs_Setup.PrintType_2)
                {
                    gs_Setup.PrintType_2 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintType_2, gs_Setup.PrintType_2);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 121:        //串行口2打印语言
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Setup.PrintLanguage_2)
                {
                    gs_Setup.PrintLanguage_2 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintLanguage_2, gs_Setup.PrintLanguage_2);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 122:        //串行口2打印走纸行数
            if (lu16_WriteData <= 9)
            {
                if (lu16_WriteData != gs_Setup.PrintBlankRows_2)
                {
                    gs_Setup.PrintBlankRows_2 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintBlankRows_2, gs_Setup.PrintBlankRows_2);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 123:        //双无斗秤互锁模式同时松袋开关
            if (lu16_WriteData <= 2)
            {
                if (lu16_WriteData != gs_Setup.No2PackOffTwoSwitch)
                {
                    gs_Setup.No2PackOffTwoSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_No2PackOffTwoSwitch, gs_Setup.No2PackOffTwoSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 124:        //输送机启动延时时间t13
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Setup.No2ConveyorDelayTime)
                {
                    gs_Setup.No2ConveyorDelayTime = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_No2ConveyorDelayTime, gs_Setup.No2ConveyorDelayTime);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 125:        //输送机运行时间t14
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Setup.No2ConveyorRunTime)
                {
                    gs_Setup.No2ConveyorRunTime = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_No2ConveyorRunTime, gs_Setup.No2ConveyorRunTime);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 126:        //再次夹袋延时启动加料时间t15
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Setup.No2AddAgainDelayTime)
                {
                    gs_Setup.No2AddAgainDelayTime = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_No2AddAgainDelayTime, gs_Setup.No2AddAgainDelayTime);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
                
        case 144:        //T2_a
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT2a)
                {
                    gs_Rec.TimeT2a = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT2a(gu32_RecNum), gs_Rec.TimeT2a);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 145:        //T9提袋延时时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT9)
                {
                    gs_Rec.TimeT9 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT9(gu32_RecNum), gs_Rec.TimeT9);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 146:        //T10提袋输出有效时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT10)
                {
                    gs_Rec.TimeT10 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT10(gu32_RecNum), gs_Rec.TimeT10);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 147:        //T11推袋延时时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT11)
                {
                    gs_Rec.TimeT11 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT11(gu32_RecNum), gs_Rec.TimeT11);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 148:        //T12推袋输出有效时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.TimeT12)
                {
                    gs_Rec.TimeT12 = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_TimeT12(gu32_RecNum), gs_Rec.TimeT12);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 149:        //卸料振打开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Rec.DiscShakeSwitch)
                {
                    gs_Rec.DiscShakeSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_DiscShakeSwitch(gu32_RecNum), gs_Rec.DiscShakeSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 150:        //卸料超时时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.DiscEffTime)
                {
                    gs_Rec.DiscEffTime = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_DiscEffTime(gu32_RecNum), gs_Rec.DiscEffTime);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 151:        //卸料振打有效时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.DiscShakeTimeHi)
                {
                    gs_Rec.DiscShakeTimeHi = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_DiscShakeTimeHi(gu32_RecNum), gs_Rec.DiscShakeTimeHi);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 152:        //卸料振打无效时间
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.DiscShakeTimeLow)
                {
                    gs_Rec.DiscShakeTimeLow = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_DiscShakeTimeLow(gu32_RecNum), gs_Rec.DiscShakeTimeLow);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 153:        //卸料振打次数
            if (lu16_WriteData <= 99)
            {
                if (lu16_WriteData != gs_Rec.DiscShakeNum)
                {
                    gs_Rec.DiscShakeNum = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_DiscShakeNum(gu32_RecNum), gs_Rec.DiscShakeNum);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 154:        //打码控制功能开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Rec.PrintCodeSwitch)
                {
                    gs_Rec.PrintCodeSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintCodeSwitch(gu32_RecNum), gs_Rec.PrintCodeSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 155:        //打码启动延时TP1
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.PrintCodeRunDelay)
                {
                    gs_Rec.PrintCodeRunDelay = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintCodeRunDelay(gu32_RecNum), gs_Rec.PrintCodeRunDelay);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 156:        //打码输出有效时间TP2
            if (lu16_WriteData <= 999)
            {
                if (lu16_WriteData != gs_Rec.PrintCodeTime)
                {
                    gs_Rec.PrintCodeTime = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_PrintCodeTime(gu32_RecNum), gs_Rec.PrintCodeTime);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 157:        //打码时不允许卸料/加料 开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Rec.SheildDiscSwitch)
                {
                    gs_Rec.SheildDiscSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_SheildDiscSwitch(gu32_RecNum), gs_Rec.SheildDiscSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 158:        //超欠差报警开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Rec.OUSwitch)
                {
                    gs_Rec.OUSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_OUSwitch(gu32_RecNum), gs_Rec.OUSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 159:        //超欠差暂停开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Rec.OUStopSwitch)
                {
                    gs_Rec.OUStopSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_OUStopSwitch(gu32_RecNum), gs_Rec.OUStopSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 160:        //落差修正开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Rec.CorrectFallSwitch)
                {
                    gs_Rec.CorrectFallSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_CorrectFallSwitch(gu32_RecNum), gs_Rec.CorrectFallSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 161:        //小投点动开关
            if (lu16_WriteData <= 1)
            {
                if (lu16_WriteData != gs_Rec.SmallPluseSwitch)
                {
                    gs_Rec.SmallPluseSwitch = lu16_WriteData;
                    FRAM_WriteDW(FMAddr_SmallPluseSwitch(gu32_RecNum), gs_Rec.SmallPluseSwitch);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
                
        case 183:        //总累计数据打印
            if (lu16_WriteData == 1)
            {
                if (gb_RunFlg == false)
                {
                    gb_PrintAllRecFlg_UART0 = false;
                    gb_PrintAllRecFlg_UART1 = false;//修正Modbus打印所有配方累计后，再打印总累计会打印多条数据的问题 v1.01
                    PrintStart(PRINT_TOTAL_REPORT, 0, 0);
                    return RESPONSEOK;
                }
                else
                    return NOSUCCESS;
            }
            else
                return DATAERRO;
        case 184:        //配方设置表打印
            if (lu16_WriteData <= 21)
            {
                if (gb_RunFlg == false)
                {
                    if (lu16_WriteData == 0)
                    {   //打印当前配方设置表
                        gb_FirstPrintFlg_UART0 = true;
                        gb_PrintAllRecFlg_UART0 = false;
                        gb_FirstPrintFlg_UART1 = true;
                        gb_PrintAllRecFlg_UART1 = false;
                        PrintStart(PRINT_REC_SET, gu32_RecNum, 0);
                    }
                    else if (lu16_WriteData >= 1 && lu16_WriteData <= 20)
                    {   //打印对应配方设置表
                        gb_FirstPrintFlg_UART0 = true;
                        gb_PrintAllRecFlg_UART0 = false;
                        gb_FirstPrintFlg_UART1 = true;
                        gb_PrintAllRecFlg_UART1 = false;
                        PrintStart(PRINT_REC_SET, lu16_WriteData, 0);
                    }
                    else
                    {   //打印所有配方设置表
                        gb_FirstPrintFlg_UART0 = true;
                        gb_PrintAllRecFlg_UART0 = true;
                        gb_FirstPrintFlg_UART1 = true;
                        gb_PrintAllRecFlg_UART1 = true;
                        gu8_Item = PRINT_REC_SET;
                        //打印所有目标值不为0的配方
                        if (gs_Rec.Target != 0)
                        {
                            gu32_PrintRecNumStart = gu32_RecNum;    //保存起始配方号，用来判断是否有所配方都打印完成
                            PrintStart(PRINT_REC_SET, gu32_RecNum, 0);  //从当前配方开始打印
                        }
                        else
                        {   //当前配方目标值为0，获取下一个目标值不为0的配方号
                            gu32_PrintRecNumStart = NextRecNum(gu32_RecNum);
                            if (gu32_PrintRecNumStart != gu32_RecNum)
                            {   //如果获取到的下一个不为0的配方号和目前配方号相等，代表所有配方目标值都为0，不打印
                                PrintStart(PRINT_REC_SET, gu32_PrintRecNumStart, 0);  //从目标值不为0的配方号开始打印
                            }
                        }
                    }
                }
                else
                    return NOSUCCESS;
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 185:        //配方累计报表打印
            if (lu16_WriteData <= 21)
            {
                if (gb_RunFlg == false)
                {
                    if (lu16_WriteData == 0)
                    {   //打印当前配方累计报表
                        gb_FirstPrintFlg_UART0 = true;
                        gb_PrintAllRecFlg_UART0 = false;
                        gb_FirstPrintFlg_UART1 = true;
                        gb_PrintAllRecFlg_UART1 = false;
                        PrintStart(PRINT_REC_REPORT, gu32_RecNum, 0);
                    }
                    else if (lu16_WriteData >= 1 && lu16_WriteData <= 20)
                    {   //打印对应配方累计报表
                        gb_FirstPrintFlg_UART0 = true;
                        gb_PrintAllRecFlg_UART0 = false;
                        gb_FirstPrintFlg_UART1 = true;
                        gb_PrintAllRecFlg_UART1 = false;
                        PrintStart(PRINT_REC_REPORT, lu16_WriteData, 0);
                    }
                    else
                    {   //打印所有配方累计报表
                        gb_FirstPrintFlg_UART0 = true;
                        gb_PrintAllRecFlg_UART0 = true;
                        gb_FirstPrintFlg_UART1 = true;
                        gb_PrintAllRecFlg_UART1 = true;
                        gu8_Item = PRINT_REC_REPORT;
                        //打印所有目标值不为0的配方
                        if (gs_Rec.Target != 0)
                        {
                            gu32_PrintRecNumStart = gu32_RecNum;    //保存起始配方号，用来判断是否有所配方都打印完成
                            PrintStart(PRINT_REC_REPORT, gu32_RecNum, 0);  //从当前配方开始打印
                        }
                        else
                        {   //当前配方目标值为0，获取下一个目标值不为0的配方号
                            gu32_PrintRecNumStart = NextRecNum(gu32_RecNum);
                            if (gu32_PrintRecNumStart != gu32_RecNum)
                            {   //如果获取到的下一个不为0的配方号和目前配方号相等，代表所有配方目标值都为0，不打印
                                PrintStart(PRINT_REC_REPORT, gu32_PrintRecNumStart, 0);  //从目标值不为0的配方号开始打印
                            }
                        }
                    }
                }
                else
                    return NOSUCCESS;
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 186:       //备份还原恢复出厂设置
            if ((lu16_WriteData >= 8800) && (lu16_WriteData <= 8806))
            {
                switch (lu16_WriteData)
                {
                    case 8800:              //恢复出厂设置
                        Reset_Cal();
                        Reset_Setup(true);
                        Reset_Rec();
                        Reset_IoDef();
                        FRAM_WriteDW(FMAddr_BatchTime, 0);
                        FRAM_WriteDW(FMAddr_BatchTimeCnt, 0);
                        break;
                    case 8801:              //恢复标定
                        Reset_Cal();
                        break;
                    case 8802:              //恢复工作参数
                        Reset_Setup(true);
                        break;
                    case 8803:              //恢复配方
                        Reset_Rec();
                        break;
                    case 8804:              //恢复IO定义
                        Reset_IoDef();
                        break;
                    case 8805:              //执行备份
                        Backup_Parameter();
                        break;
                    case 8806:              //执行恢复
                        Reload_Parameter();
                        break;
                }
                Init_Parameter();
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        default:
            return DATAPLACEERRO;
    }
}

/*********************************************************************
函数名称: void ModBus_Write_2Byte(u8 *fu8p_Data)
功    能: modbus写单寄存器处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-22
修    改:                      时    间: 
*********************************************************************/
void ModBus_Write_2Byte(u8 *fu8p_Data)
{
    u16 lu16_Address;
    u16 lu16_ErrCode;
    
    lu16_Address = (fu8p_Data[2] << 8) + fu8p_Data[3];
    
    //写双字节的地址范围是
    if ((lu16_Address >= RW2BYTESTARTADS) && (lu16_Address <= RW2BYTEENDADS))
    {
        if (gb_RunFlg == false)       //不在运行状态才允许写入
            lu16_ErrCode = ModBus_Write_2ByteDeal(fu8p_Data);
        else
            lu16_ErrCode = NOSUCCESS;
        if (lu16_ErrCode == RESPONSEOK)
        {
            //数据写入成功
            ModBus_Write_2ByteOk(fu8p_Data);
        }
        else
        {
            //数据范围或者类型错误
            Modbus_Error(lu16_ErrCode, fu8p_Data);
        }
    }
    else
    {
        //功能地址错误
        Modbus_Error(DATAPLACEERRO, fu8p_Data);
    }
}

/***********************************************************
函 数 名：void ModBus_Write_4ByteOk(void)
功    能：MODBUS协议下的四字节写入的数据正确
说    明：
入口参数：无
调用程序: 
返 回 值：无
设    计：丛海旭               日    期：2011-5-17
修    改：                     日    期：
***********************************************************/
void ModBus_Write_4ByteOk(u8 *fu8p_Data)
{
    FpBuildCrcLrc(fu8p_Data, 6);
    FpSendRtuAscII(fu8p_Data, 8);
}


/***********************************************************
函 数 名：bit ModBus_Write_4ByteDeal(u8 *fu8p_Data)
功    能：MODBUS协议下的4字节写入的数据
说    明：
入口参数：无
返 回 值：无
设    计：wbj                  日    期：2008-1-28 9:15
修    改：丛海旭               日    期：2011-5-17
***********************************************************/
u8 ModBus_Write_4ByteDeal(u8 *fu8p_Data)
{
    u16 lu16_Address;
    ULDATA ulWriteData;
    u32 lu32_Temp;
    
    //写入的地址
    lu16_Address = fu8p_Data[2];
    lu16_Address <<= 8;
    lu16_Address += fu8p_Data[3]; 
  
    //写入的数据
    if ((fu8p_Data == gu8_TxdArray_UART0 ? gs_Setup.HiLo_2 : gs_Setup.HiLo_1) == 0)                      //高字在前      chx 090908
    {
        ulWriteData.uc[3] = fu8p_Data[7];
        ulWriteData.uc[2] = fu8p_Data[8];
        ulWriteData.uc[1] = fu8p_Data[9];
        ulWriteData.uc[0] = fu8p_Data[10];
    }
    else                                    //低字在前      chx 090908
    {
        ulWriteData.uc[3] = fu8p_Data[9];
        ulWriteData.uc[2] = fu8p_Data[10];
        ulWriteData.uc[1] = fu8p_Data[7];
        ulWriteData.uc[0] = fu8p_Data[8];
    }
    
    switch (lu16_Address)
    {
        case 16:        //最大量程
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if ((ulWriteData.ul % gs_Cal.Div) !=0)
                lu32_Temp = 1;
            else
                lu32_Temp = 0;
            if ((ulWriteData.ul <= 999999) && ((ulWriteData.ul / gs_Cal.Div + lu32_Temp) <= 100000) && (ulWriteData.ul >= gs_Cal.Div))  //最大量程大于等于最小分度值
            {
                if (ulWriteData.ul != gs_Cal.Cap)
                {
                    gs_Cal.Cap = ulWriteData.ul;
                    FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 18:        //有砝码零点标定
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (ulWriteData.ul == 1)
            {
                //稳定并且AD不溢出才允许标零点
                if (AdToMv(gu32_AdFedValue) <= 12000 * gs_Cal.Sensor && 
                    gb_AdOflFlg == false && gb_MvStabFlg == true)
                {
                    gs_Cal.Zero = gu32_AdFedValue;
                    FRAM_WriteDW(FMAddr_CalZero, gs_Cal.Zero);
                    gu32_FloatZero = 0;
                    gb_SigFZeroFlg = false;
                    return RESPONSEOK;
                }
                else
                    return NOSUCCESS;
            }
            else
                return DATAERRO;
        case 20:        //有砝码增益标定(输入增益重量值)
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (ulWriteData.ul <= 999999 && ulWriteData.ul > 0)
            {
                //稳定、AD不溢出并且当前AD大于零点AD值才允许标定增益
                if (gb_AdOflFlg == false && gb_MvStabFlg == true && AdToMv(gu32_AdFedValue) > AdToMv(gs_Cal.Zero))
                {
                    //         ADc * (2^13)
                    //  ADK = ------------------
                    //         (Wc / dd) * 4
                    gs_Cal.ADK = ((float)(gu32_AdFedValue - gs_Cal.Zero))/((float)ulWriteData.ul * 4.0f) * (float)gs_Cal.Div;
                    FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);    //保存ADK
                    gu32_FloatZero = 0;
                    gb_SigFZeroFlg = false;
                    return RESPONSEOK;
                }
                else
                    return NOSUCCESS;
            }
            else
                return DATAERRO;
        case 22:        //无砝码零点标定 输入的毫伏数转化成ADcode
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (ulWriteData.ul <= 12000 * gs_Cal.Sensor)
            {
                gs_Cal.Zero = MvToAd(ulWriteData.ul);      //Mv值转为AD码
                FRAM_WriteDW(FMAddr_CalZero, gs_Cal.Zero);
                gu32_FloatZero = 0;
                gb_SigFZeroFlg = false;
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 24:        //无砝码增益标定(增益毫伏数值)
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (ulWriteData.ul <= gs_Cal.Sensor * 12000)
            {
                gu32_GainMv = ulWriteData.ul;
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 26:        //无砝码增益标定(增益重量值) (A秤)
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (ulWriteData.ul <= 999999 && gu32_GainMv != 0)
            {
                //         ADc * (2^13)
                //  ADK = ------------------
                //         (Wc / dd) * 4
                gs_Cal.ADK = ((float)MvToAd(gu32_GainMv))/((float)ulWriteData.ul * 4.0f) * (float)gs_Cal.Div;
                FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);    //保存ADK
                gu32_FloatZero = 0;
                gb_SigFZeroFlg = false;
                gu32_GainMv = 0;
                return RESPONSEOK;
            }
            else
                return NOSUCCESS;
        case 28:        //目标值
            if (ulWriteData.ul  <= gs_Cal.Cap)
            {
                if (ulWriteData.ul != gs_Rec.Target)
                {
                    if (gb_RunFlg == false)     //停止状态下立即修改配方
                    {
                        gs_Rec.Target = ulWriteData.ul;   
                        FRAM_WriteDW(FMAddr_Target(gu32_RecNum), gs_Rec.Target);
                    }
                    else        //运行时修改
                    {
                        gu32_FastSetTemp[0] = ulWriteData.ul;
                        gu8_FastSetSigTemp |= 0x01;
                    }
                }
                return RESPONSEOK;
            }       
            else
                return DATAERRO;
        case 30:        //大投提前量
            if (ulWriteData.ul  <= gs_Cal.Cap)
            {
                if (ulWriteData.ul != gs_Rec.Large)
                {
                    if (gb_RunFlg == false)
                    {
                        gs_Rec.Large = ulWriteData.ul;
                        FRAM_WriteDW(FMAddr_Large(gu32_RecNum), gs_Rec.Large);
                    }
                    else        //运行时修改
                    {
                        gu32_FastSetTemp[1] = ulWriteData.ul;
                        gu8_FastSetSigTemp |= 0x02;
                    }
                }
                return RESPONSEOK;
            }   
            else
                return DATAERRO;    
        case 32:        //中投提前量
            if (ulWriteData.ul  <= gs_Cal.Cap)
            {
                if (ulWriteData.ul != gs_Rec.Middle)
                {
                    if (gb_RunFlg == false)
                    {
                        gs_Rec.Middle = ulWriteData.ul;
                        FRAM_WriteDW(FMAddr_Middle(gu32_RecNum), gs_Rec.Middle);
                    }
                    else        //运行时修改
                    {
                        gu32_FastSetTemp[2] = ulWriteData.ul;
                        gu8_FastSetSigTemp |= 0x04;
                    }
                }
                return RESPONSEOK;
            }       
            else
                return DATAERRO;
        case 34:        //落差值
            if (ulWriteData.ul  <= gs_Cal.Cap)
            {
                if (ulWriteData.ul != gs_Rec.Small)
                {
                    if (gb_RunFlg == false)
                    {
                        gs_Rec.Small = ulWriteData.ul;
                        FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);
                    }
                    else        //运行时修改
                    {
                        gu32_FastSetTemp[3] = ulWriteData.ul;
                        gu8_FastSetSigTemp |= 0x08;
                    }
                }
                return RESPONSEOK;
            }       
            else
                return DATAERRO;
        case 36:        //近零值
            if (ulWriteData.ul  <= gs_Cal.Cap)
            {
                if (ulWriteData.ul != gs_Rec.NearZero)
                {
                    gs_Rec.NearZero = ulWriteData.ul;
                    FRAM_WriteDW(FMAddr_NearZero(gu32_RecNum), gs_Rec.NearZero);
                }
                return RESPONSEOK;
            }       
            else
                return DATAERRO;
        case 38:        //拍袋起始重量
            if (ulWriteData.ul  <= gs_Cal.Cap)
            {
                if (ulWriteData.ul != gs_Rec.FlapBagWt)
                {
                    if (gb_RunFlg == false)
                    {
                        gs_Rec.FlapBagWt = ulWriteData.ul;
                        FRAM_WriteDW(FMAddr_FlapBagWt(gu32_RecNum), gs_Rec.FlapBagWt);
                    }
                    else        //运行时修改
                    {
                        gu32_FastSetTemp[5] = ulWriteData.ul;
                        gu8_FastSetSigTemp |= 0x20;
                    }
                }
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 40:        //超量值
            if (ulWriteData.ul  <= gs_Cal.Cap)
            {
                if (ulWriteData.ul != gs_Rec.OverValue)
                {
                    gs_Rec.OverValue = ulWriteData.ul;
                    FRAM_WriteDW(FMAddr_OverValue(gu32_RecNum), gs_Rec.OverValue);
                }
                return RESPONSEOK;
            }       
            else
                return DATAERRO;
        case 42:        //欠量值
            if (ulWriteData.ul  <= gs_Cal.Cap)
            {
                if (ulWriteData.ul != gs_Rec.UnderValue)
                {
                    gs_Rec.UnderValue = ulWriteData.ul;
                    FRAM_WriteDW(FMAddr_UnderValue(gu32_RecNum), gs_Rec.UnderValue);
                }
                return RESPONSEOK;
            }   
            else
                return DATAERRO;
        case 178:    //增益校准前重量 v1.01
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (ulWriteData.ul <= 999999)
            {
                gu32_CalDispWt = ulWriteData.ul;
                return RESPONSEOK;
            }
            else
                return DATAERRO;
        case 180:    //增益校准后重量 v1.01
            if (gb_CalLockDefFlg && gb_CalLockFlg == false)
            {   //标定锁有定义但是输入无效，不能标定
                return NOSUCCESS;
            }
            if (ulWriteData.ul <= 999999 && gu32_CalDispWt != 0)
            {
                gs_Cal.ADK = ((float)gu32_CalDispWt*gs_Cal.ADK)/(float)ulWriteData.ul;
                FRAM_WriteFloat( FMAddr_CalADK, gs_Cal.ADK );    //保存ADK
                gu32_FloatZero = RoundOff(gu32_FloatZero * ulWriteData.ul, gu32_CalDispWt);
                gu32_CalDispWt = 0;
                return RESPONSEOK;
            }
            else
                return DATAERRO;    
                
	    case 190:   //配方1累计次数
	    case 192:   //配方1累计重量
	    case 194:   //配方2累计次数
	    case 196:   //配方2累计重量
	    case 198:   //配方3累计次数
	    case 200:   //配方3累计重量
	    case 202:   //配方4累计次数
	    case 204:   //配方4累计重量
	    case 206:   //配方5累计次数
	    case 208:   //配方5累计重量
	    case 210:   //配方6累计次数
	    case 212:   //配方6累计重量
	    case 214:   //配方7累计次数
	    case 216:   //配方7累计重量
	    case 218:   //配方8累计次数
	    case 220:   //配方8累计重量
	    case 222:   //配方9累计次数
	    case 224:   //配方9累计重量
	    case 226:   //配方10累计次数
	    case 228:   //配方10累计重量
	    case 230:   //配方11累计次数
	    case 232:   //配方11累计重量
	    case 234:   //配方12累计次数
	    case 236:   //配方12累计重量
	    case 238:   //配方13累计次数
	    case 240:   //配方13累计重量
	    case 242:   //配方14累计次数
	    case 244:   //配方14累计重量
	    case 246:   //配方15累计次数
	    case 248:   //配方15累计重量
	    case 250:   //配方16累计次数
	    case 252:   //配方16累计重量
	    case 254:   //配方17累计次数
	    case 256:   //配方17累计重量
	    case 258:   //配方18累计次数
	    case 260:   //配方18累计重量
	    case 262:   //配方19累计次数
	    case 264:   //配方19累计重量
	    case 266:   //配方20累计次数
	    case 268:   //配方20累计重量
	        if (ulWriteData.ul == 0)    //写入0，清除对应的配方累计次数和重量
	        {
	            ClrSum_Rec((lu16_Address - 190)/4 + 1);
				return RESPONSEOK;
	        }
	        else
    			return DATAERRO;
        default:
            return DATAPLACEERRO;
    }
}

/***********************************************************
函 数 名：void ModBus_Write_4Byte(u8 *fu8p_Data)
功    能：MODBUS协议下的四字节写入
说    明：
入口参数：无
调用程序: 
返 回 值：无
设    计：wbj                  日    期：2008-1-28 9:15
修    改：                     日    期：
***********************************************************/
void ModBus_Write_4Byte(u8 *fu8p_Data)
{
    u16 lu16_Address;
    u16 lu16_ErrCode;
    
    lu16_Address = (fu8p_Data[2] << 8) + fu8p_Data[3];
    //可读区的地址范围是 for test
    if (((lu16_Address >= RW4BYTESTARTADS) && (lu16_Address <= RW4BYTEENDADS) || 
         (lu16_Address >= RW4BYTESTARTADS_1) && (lu16_Address <= RW4BYTEENDADS_1)) &&
         (lu16_Address % 2 == 0))
    {
        //只允许写入四字节
        if (fu8p_Data[6] != 4)
        {
            //数据范围或者类型错误
            Modbus_Error(DATAPLACEERRO, fu8p_Data);
        }
        else
        {
            //不在运行状态才允许写入 运行时只可以修改F1.x
            if ((gb_RunFlg == false) || (lu16_Address >= F1_1ADDRESS && lu16_Address <= F1_6ADDRESS))
                lu16_ErrCode = ModBus_Write_4ByteDeal(fu8p_Data);
            else
                lu16_ErrCode = NOSUCCESS;
            if (lu16_ErrCode == RESPONSEOK)
            {
                //数据写入成功
                ModBus_Write_4ByteOk(fu8p_Data);
            }
            else
            {
                //数据范围或者类型错误
                Modbus_Error(lu16_ErrCode, fu8p_Data);
            }
        }
    }
    else
    {
        //功能地址错误
        Modbus_Error(DATAPLACEERRO, fu8p_Data);
    }
}


/***********************************************************************
函 数 名：void Deal8byteShift(u32 *fu32_Buf,u8 fu8_Lenth,bool fb_ShiftMode)
功    能：仅仅适用于移位次数小于等于32
          shiftmode:0 左移   shiftmode:1 右移
说    明：
入口参数：
返 回 值：
设    计：                     日    期：
修    改：丛海旭               日    期：2011-5-17
***********************************************************************/
void Deal8byteShift(u32 *fu32_Buf, u8 fu8_Lenth, bool fb_ShiftMode)
{
    u32 lu32_TempBit0, lu32_TempBit1;

    lu32_TempBit0 = fu32_Buf[0];
    lu32_TempBit1 = fu32_Buf[1];

    if ((fu8_Lenth > 32) && (fb_ShiftMode==0)) return;
    if ((fu8_Lenth > 64) && (fb_ShiftMode == 1)) return;
        
    if (fb_ShiftMode==0)
    {
        lu32_TempBit1 <<= fu8_Lenth;
        lu32_TempBit1 |= (lu32_TempBit0 >> (32 - fu8_Lenth));
        lu32_TempBit0 <<= fu8_Lenth;
    }
    else
    {
        if (fu8_Lenth <= 32)
        {
            lu32_TempBit0 >>= fu8_Lenth;
            lu32_TempBit0 |= (lu32_TempBit1 << (32 - fu8_Lenth));
            lu32_TempBit1 >>= fu8_Lenth;
        }
        else
        {
            lu32_TempBit0 = lu32_TempBit1;
            lu32_TempBit0 >>= (fu8_Lenth - 32);
            lu32_TempBit1 = 0;
        }
    }

    fu32_Buf[0] = lu32_TempBit0;
    fu32_Buf[1] = lu32_TempBit1;
}

/*********************************************************************
函数名称: void Modbus_Read_CoilDeal(void)
功    能: 读取线圈数据整理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-5-17
修    改:                      时    间: 
*********************************************************************/
void Modbus_Read_CoilDeal(void)
{
    u32 i = 0x01;
    
    #define SumWt0    ((gu32_SumNum == 0)&&(gu32_SumWt == 0))
        
    //===========读写存储区域=======================
    //前32个线圈
    //64~76 全部 = 0 
    gu32_ReadCoilSwitch = 0;
    gu32_ReadCoilSwitch |=((gs_Setup.PowerOnZeroSwitch != 0)     ? (i << 13) : 0x00);      //0077
    //此处为兼容老单片机程序有斗=1，无斗=0，此程序是有斗=0，无斗=1
    gu32_ReadCoilSwitch |=((gs_Setup.TareMode != 1)              ? (i << 14) : 0x00);      //0078
    gu32_ReadCoilSwitch |=((gs_Setup.PrintSwitch_1 != 0)         ? (i << 15) : 0x00);      //0079 v1.01
    gu32_ReadCoilSwitch |=((gs_Rec.OUSwitch != 0)                ? (i << 16) : 0x00);      //0080
    gu32_ReadCoilSwitch |=((gs_Rec.OUStopSwitch != 0)            ? (i << 17) : 0x00);      //0081
    gu32_ReadCoilSwitch |=((gs_Rec.CorrectFallSwitch != 0)       ? (i << 18) : 0x00);      //0082
    gu32_ReadCoilSwitch |=((gs_Rec.SmallPluseSwitch != 0)        ? (i << 19) : 0x00);      //0083
    gu32_ReadCoilSwitch |=((gs_Setup.GorNMode == NET)            ? (i << 20) : 0x00);      //0084
    gu32_ReadCoilSwitch |=((gs_Setup.FeedMode != 0)              ? (i << 21) : 0x00);      //0085
                    
    gu32_ReadCoilSwitch |=((gu16_MbIOOutInit == 0) ? (i << 29) : 0x00);    //0093
    gu32_ReadCoilSwitch |=(((gu16_MbIoOutState&0x01)!=0)     ? (i << 30) :(0x00));     //0094
    gu32_ReadCoilSwitch |=(((gu16_MbIoOutState&0x02)!=0)     ? (i << 31) :(0x00));     //0095
        
    //后32个线圈
    gu32_ReadCoilIo = 0;
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x04)!=0)     ? (i):(0x00));          //0096
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x08)!=0)     ? (i<<1):(0x00));       //0097
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x10)!=0)     ? (i<<2):(0x00));       //0098
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x20)!=0)     ? (i<<3):(0x00));       //0099
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x40)!=0)     ? (i<<4):(0x00));       //0100
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x80)!=0)     ? (i<<5):(0x00));       //0101
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x100)!=0)    ? (i<<6):(0x00));       //0102
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x200)!=0)    ? (i<<7):(0x00));       //0103
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x400)!=0)    ? (i<<8):(0x00));       //0104
    gu32_ReadCoilIo |= (((gu16_MbIoOutState&0x800)!=0)    ? (i<<9):(0x00));       //0105
        
    gu32_ReadCoilIo |= ((gb_RunFlg)          ? i<<10 : 0x00);    //0106
    gu32_ReadCoilIo |= ((!gb_RunFlg)         ? i<<11 : 0x00);    //0107
    gu32_ReadCoilIo |= ((gb_ZeroPosition)    ? i<<12 : 0x00);    //0108
    gu32_ReadCoilIo |= ((gb_AlarmFlg == false)         ? i<<13 : 0x00);    //0109
  //gu32_ReadCoilIo |= 0x00;           //i<<14            //0110选配方,恒定读出0
    gu32_ReadCoilIo |= ((gb_PackFlg)         ? i<<15 : 0x00);    //0111
    gu32_ReadCoilIo |= ((Sp_Disc_State(DISC_STATE))    ? i<<16 : 0x00);    //0112
    gu32_ReadCoilIo |= ((Sp_Disc_State(SP3_STATE))     ? i<<17 : 0x00);    //0113  
    //gu32_ReadCoilIo |= 0x00;         //i<<18            //0114打印,恒定读出0
    gu32_ReadCoilIo |= ((gb_MbKeyLockFlg)              ? i<<19 : 0x00);    //0115
    gu32_ReadCoilIo |= ((SumWt0)             ? i<<20 : 0x00);    //0116
    gu32_ReadCoilIo |= ((gb_DispNetFlg)      ? i<<21 : 0x00);    //0117
    gu32_ReadCoilIo |= ((gb_GoFlg)           ? i<<22 : 0x00);    //0118
    gu32_ReadCoilIo |= ((gb_EndFlg)         ? i<<23 : 0x00);    //0119
    gu32_ReadCoilIo |= ((gb_Sp1Flg)          ? i<<24 : 0x00);    //0120
}

/***********************************************************
函 数 名：void Modbus_Read_Coil(u8 *fu8p_Data)
功    能：读线圈状态,存放在ulOnlyReadColl位中
          所有位的数目不超过64
说    明: 先将要读的位整理好后,存放入相应的存储变量中,然后
          将个数除以8,算出多少个字节,最后以每8位移除一次的
          方式将位全部读出,不够8位的,低端补零.
入口参数：
返 回 值：
设    计：wbj                  日    期：2008-1-28 9:15
修    改：丛海旭               日    期：2011-5-17
***********************************************************/
void Modbus_Read_Coil(u8 *fu8p_Data)
{
    u8  i;
    u8  lu8_ByteNum;
    u32 lu32_DataBuf[2] = {0,0};
    u16 lu16_Address;
    u16 lu16_ReadLenth;
    u8 lu8_EmptyBit;     //未读线圈的空位
    
    lu16_Address = (fu8p_Data[2] << 8) + fu8p_Data[3];
    lu16_ReadLenth = (fu8p_Data[4] << 8) + fu8p_Data[5];
    
    //将特殊线圈处理位地址存入64位低32位缓冲区中
    lu32_DataBuf[0] = gu32_ReadCoilIo;
    //将64位缓冲区左移ReadBitEEProm地址长度,腾出空间给ReadBitEEProm地址存放
    Deal8byteShift(lu32_DataBuf,(RWCOILENDADSEE-RWCOILSTARTADSEE+1),false);
    //将地址存入64位缓冲区低32位中
    lu32_DataBuf[0] |= gu32_ReadCoilSwitch;
    //将缓冲区右移出 当前地址-线圈可读首地址 长度(即从当前地址读起,其他被移位的剔除) 
    Deal8byteShift(lu32_DataBuf,(u8)(lu16_Address-RWCOILSTARTADSEE),true);
    
    if ((lu16_ReadLenth % 8) == 0)
    {
        lu8_ByteNum = lu16_ReadLenth / 8;
        lu8_EmptyBit = 0;
    }
    else
    {
        lu8_ByteNum = (lu16_ReadLenth / 8) + 1;
        lu8_EmptyBit = 8 - lu16_ReadLenth % 8;     //取得未读线圈空位的个数
    }

    if ((lu16_Address <= RWCOILENDADSDEAL) && (lu16_Address >= RWCOILSTARTADSEE) &&
       (lu16_ReadLenth != 0) &&
       (lu16_ReadLenth <= (RWCOILENDADSDEAL - lu16_Address + 1)))
    {
        fu8p_Data[2] = lu8_ByteNum;
        for (i = 0; i < lu8_ByteNum; i++)
        {
            fu8p_Data[3 + i] = (u8)(lu32_DataBuf[0]);
            Deal8byteShift(lu32_DataBuf,8,true);
        }
        
        //将没有读的线圈置0
        fu8p_Data[3+i-1] <<= lu8_EmptyBit;
        fu8p_Data[3+i-1] >>= lu8_EmptyBit;
        
        FpBuildCrcLrc(fu8p_Data, lu8_ByteNum + 3);
        FpSendRtuAscII(fu8p_Data, lu8_ByteNum + 5);
    }
    else
    {
        Modbus_Error(DATAPLACEERRO, fu8p_Data);
    }
}


/*********************************************************************
函数名称: void ModBus_Write_CoilOk(u8 *fu8p_Data)
功    能: modbus写线圈成功返回OK
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-5-18
修    改:                      时    间: 
*********************************************************************/
void ModBus_Write_CoilOk(u8 *fu8p_Data)
{    
    FpBuildCrcLrc(fu8p_Data, 6);
    FpSendRtuAscII(fu8p_Data, 8);
}

/***********************************************************
函数名称: void Modbus_Write_CoilDeal(u8 *fu8p_Data)
功    能: 写线圈(参数开关)
说    明: 无
入口参数: 
返 回 值: 无
设    计:                      日    期：
修    改: 丛海旭               日    期：2011-5-18
***********************************************************/
void Modbus_Write_CoilDeal(u8 *fu8p_Data)
{
    bool lb_ResultOKFlg = false;
    bool lb_ErrorFlg = false;
    u16 lu16_Address;
    u16 lu16_WriteData;

    lu16_Address = (fu8p_Data[2] << 8) + fu8p_Data[3];
    lu16_WriteData = (fu8p_Data[4] << 8) + fu8p_Data[5];

    //读写线圈起始功能~读写线圈特殊处理功能结束
    if (lu16_Address <= RWCOILENDADSDEAL)
    {
        if ((lu16_Address <= 93) && (gb_RunFlg == true))     //运行时不允许修改开关
        {
            lb_ErrorFlg = true;
        }
        //开关量测试打开时不允许操作开关量输出相关功能
        else if(lu16_Address >= 94 && gu16_MbIoTestSwitch == 1)
        {
            lb_ErrorFlg = true;
        }
        else
        {
            switch (lu16_Address)
            {
                case 77:       //上电自动清零开关
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        gs_Setup.PowerOnZeroSwitch = (lu16_WriteData == 0xFF00) ? 1 : 0;
                        FRAM_WriteDW(FMAddr_PowerOnZeroSwitch, gs_Setup.PowerOnZeroSwitch);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 78:       //秤体模式
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {   //此处为兼容老程序，0是无斗1是有斗，跟此程序相反
                        gs_Setup.TareMode = (lu16_WriteData == 0xFF00) ? 0 : 1;
                        if (FRAM_ReadDW(FMAddr_TareMode) != gs_Setup.TareMode)
                        {
                            FRAM_WriteDW(FMAddr_TareMode, gs_Setup.TareMode);
                            TareModeChange(gs_Setup.TareMode);
                        }
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 79:       //自动打印开关
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        gs_Setup.PrintSwitch_1 = (lu16_WriteData == 0xFF00) ? 1 : 0;    //v1.01
                        FRAM_WriteDW(FMAddr_PrintSwitch_1, gs_Setup.PrintSwitch_1);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 80:       //超欠差报警开关
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        gs_Rec.OUSwitch = (lu16_WriteData == 0xFF00) ? 1 : 0;
                        FRAM_WriteDW(FMAddr_OUSwitch(gu32_RecNum), gs_Rec.OUSwitch);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 81:       //差欠差暂停开关
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        gs_Rec.OUStopSwitch = (lu16_WriteData == 0xFF00) ? 1 : 0;
                        FRAM_WriteDW(FMAddr_OUStopSwitch(gu32_RecNum), gs_Rec.OUStopSwitch);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 82:       //落差修正开关
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        gs_Rec.CorrectFallSwitch = (lu16_WriteData == 0xFF00) ? 1 : 0;
                        FRAM_WriteDW(FMAddr_CorrectFallSwitch(gu32_RecNum), gs_Rec.CorrectFallSwitch);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 83:       //小投点动开关
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        gs_Rec.SmallPluseSwitch = (lu16_WriteData == 0xFF00) ? 1 : 0;
                        FRAM_WriteDW(FMAddr_SmallPluseSwitch(gu32_RecNum), gs_Rec.SmallPluseSwitch);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 84:       //无斗毛净重
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        gs_Setup.GorNMode = (lu16_WriteData == 0xFF00) ? 1 : 0;
                        FRAM_WriteDW(FMAddr_GorNMode, gs_Setup.GorNMode);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 85:       //投料方式
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        gs_Setup.FeedMode = (lu16_WriteData == 0xFF00) ? 1 : 0;
                        FRAM_WriteDW(FMAddr_FeedMode, gs_Setup.FeedMode);
                        lb_ResultOKFlg = true;
                    }
                    break;
                    
                    
                    
                case 93://DEBUG OUT INIT
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 0;
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 94://DEBUG OUT1
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & 0x0001)
                            gu16_MbIoOutDebug &= (~0x0001);
                        else
                            gu16_MbIoOutDebug |= 0x0001;
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 95://DEBUG OUT2
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 1))
                            gu16_MbIoOutDebug &= (~(0x0001 << 1));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 1);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 96://DEBUG OUT3
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 2))
                            gu16_MbIoOutDebug &= (~(0x0001 << 2));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 2);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 97://DEBUG OUT4
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 3))
                            gu16_MbIoOutDebug &= (~(0x0001 << 3));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 3);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 98://DEBUG OUT5
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 4))
                            gu16_MbIoOutDebug &= (~(0x0001 << 4));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 4);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 99://DEBUG OUT6
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 5))
                            gu16_MbIoOutDebug &= (~(0x0001 << 5));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 5);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 100://DEBUG OUT7
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 6))
                            gu16_MbIoOutDebug &= (~(0x0001 << 6));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 6);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 101://DEBUG OUT8
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 7))
                            gu16_MbIoOutDebug &= (~(0x0001 << 7));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 7);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 102://DEBUG OUT9
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 8))
                            gu16_MbIoOutDebug &= (~(0x0001 << 8));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 8);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 103://DEBUG OUT10
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 9))
                            gu16_MbIoOutDebug &= (~(0x0001 << 9));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 9);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 104://DEBUG OUT11
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 10))
                            gu16_MbIoOutDebug &= (~(0x0001 << 10));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 10);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 105://DEBUG OUT12
                    if (lu16_WriteData == 0xFF00)
                    {
                        gu16_MbIOOutInit = 1;
                        gu16_MbIoOutDebug = gu16_MbIoOutState;
                        if (gu16_MbIoOutDebug & (0x0001 << 11))
                            gu16_MbIoOutDebug &= (~(0x0001 << 11));
                        else
                            gu16_MbIoOutDebug |= (0x0001 << 11);
                        lb_ResultOKFlg = true;
                    }
                    break;
                    
                case 106://运行
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)
                        {
                            RunStopInit(true);
                            lb_ResultOKFlg = true;
                        }
                        else
                            lb_ErrorFlg = true;
                    }
                    break;
                case 107://急停
                    if (lu16_WriteData == 0xFF00)
                    {
                        Clr_Alarm();
                        RunStopInit(false);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 108://清零
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)
                        {
                            gb_ZeroKeyFlg = true;
                            lb_ResultOKFlg = true;
                        }
                        else
                            lb_ErrorFlg = true;
                    }
                    break;
                case 109://清报警
                    if (lu16_WriteData == 0xFF00)
                    {
                        Clr_Alarm();
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 110:   //选配方
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)
                        {
                            //选配方输入有效调到下目标值不为0的配方
                            gu32_RecNum = NextRecNum(gu32_RecNum);
                            FRAM_WriteDW(FMAddr_RecNum, gu32_RecNum);
                            Read_RecipeData(gu32_RecNum);
                            lb_ResultOKFlg = true;
                        }
                        else
                            lb_ErrorFlg = true;
                    }
                    break;
                case 111:   //夹松袋
                    if (lu16_WriteData == 0xFF00)
                    {
                        PackInput();
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 112://手动卸料
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)
                        {
                            ManualDisc(2);
                            lb_ResultOKFlg = true;
                        }
                        else
                            lb_ErrorFlg = true;
                    }
                    break;
                case 113://手小投
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)
                        {
                            ManualSp3(2);
                            lb_ResultOKFlg = true;
                        }
                        else
                            lb_ErrorFlg = true;
                    }
                    break;
                case 114://打印
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)
                        {
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_PrintAllRecFlg_UART1 = false;//修正Modbus打印所有配方累计后，再打印总累计会打印多条数据的问题 v1.01
                            PrintStart(PRINT_TOTAL_REPORT, 0, 0);
                            lb_ResultOKFlg = true;
                        }
                        else
                            lb_ErrorFlg = true;
                    }
                    break;
                case 115://键盘锁
                    if (lu16_WriteData == 0xFF00)
                    {
                        gb_MbKeyLockFlg = (!gb_MbKeyLockFlg);
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 116://清除累计
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)
                        {
                            ClrSum();
                            lb_ResultOKFlg = true;
                        }
                        else
                        {
                            lb_ErrorFlg = true;
                        }
                    }
                    break;
                case 118://手动运行一次
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)      //停止输入无效时才允许启动
                        {
                            gb_GoFlg = true;
                            RunStopInit(true);
                            lb_ResultOKFlg = true;
                        }
                        else
                        {
                            lb_ErrorFlg = true;
                        }
                    }
                    break;
                case 119://停止
                    if (lu16_WriteData == 0xFF00)
                    {
                        Clr_Alarm();
                        if (gb_RunFlg)
                            gb_EndFlg = true;
                        lb_ResultOKFlg = true;
                    }
                    break;
                case 120://手动大投
                    if (lu16_WriteData == 0xFF00)
                    {
                        if (gb_RunFlg == false)
                        {
                            ManualSp123(2);
                            lb_ResultOKFlg = true;
                        }
                        else
                        {
                            lb_ErrorFlg = true;
                        }
                    }
                    break;
                    
                    
                //预留地址写返回07异常码
                default:
                    if ((lu16_WriteData == 0xFF00)||(lu16_WriteData == 0))
                    {
                        lb_ErrorFlg = true;
                    }
                    break;
            }
        }

        if (lb_ResultOKFlg)
        {
            ModBus_Write_CoilOk(fu8p_Data);
        }
        else if (lb_ErrorFlg)
        {
            Modbus_Error(NOSUCCESS, fu8p_Data);
        }
        else
        {
            Modbus_Error(DATAERRO, fu8p_Data);
        }
    }
    else
    {
        //功能地址错误
        Modbus_Error(DATAPLACEERRO, fu8p_Data);
    }
}

/*********************************************************************
函数名称：u8 BuildModbusLrc(u8 *fuc_Msg, u8 fuc_DataLen)
功    能：modbus ASCII方式下的lrc校验
说    明：
调    用：
入口参数：
返 回 值：检验值
设    计：丛海旭               时    间：2009-9-3
修    改：                     时    间：
*********************************************************************/
u8 BuildModbusLrc(u8 *fuc_Msg, u8 fuc_DataLen)
{
    u8 luc_Lrc = 0;               //LRC 初始化
    
    while (fuc_DataLen--)             //完成整个报文缓冲区
    {
        luc_Lrc += *fuc_Msg++;       //缓冲区字节相加，无进位
    }
    return ((u8)(-((char)luc_Lrc))); //返回二进制补码
}

/***********************************************************
函 数 名：bool CheckModbusLrc(u8 *puchMsg, u32 ulDataLen)
功    能：判断LRC校验
说    明：
入口参数：无
调用程序: 
返 回 值：无
设    计：wbj                  日    期：2008-1-24 9:26
修    改：丛海旭               日    期：2014-1-22
***********************************************************/
bool CheckModbusLrc(u8 *puchMsg, u32 ulDataLen)
{
    if (BuildModbusLrc(puchMsg, ulDataLen) == *(puchMsg+ulDataLen))
    {
        if (puchMsg == gu8_TxdArray_UART0)       //uart0modbus通讯
            gb_ModbusAnswerFlg_Uart0 = true;      //串口参数自动匹配功能中使用
        else                                        //uart1modbus通讯
            gb_ModbusAnswerFlg_Uart1 = true;      //串口参数自动匹配功能中使用
        return true;
    }
    else
    {
        if (puchMsg == gu8_TxdArray_UART0)       //uart0modbus通讯
            gvb_EndCharFlg_UART0 = false;      //
        else                                        //uart1modbus通讯
            gvb_EndCharFlg_UART1 = false;      //
        
        return(false);
    }
}


/*********************************************************************
函数名称: void Com_Modbus(u8 *fu8p_Array)
功    能: modbus通讯处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-22
修    改:                      时    间: 
*********************************************************************/
void Com_Modbus(u8 *fu8p_Array)
{
    /*RTU方式直接把输入缓冲放到输出缓冲，ASCII方式转为hex放入输出缓冲在进行处理
      并且根据RTU和ASCII方式来分配对应的校验、发送等函数指针*/
    if (fu8p_Array == gu8_TxdArray_UART0)       //uart0modbus通讯
    {
        if (gs_Setup.ComMode_2 == COM_BUS_R)    
        {
            RxdArry_To_TxdArry(0, ulFrameLength_UART0);
            FpCheckCrcLrc = CheckModBusCrc16;
            FpBuildCrcLrc = BuildCrc16;
            FpSendRtuAscII = Modbus_Send_RTU;
        }
        else    //Modbus_ASCII方式转成Hex再进行处理 gvu8_RxdCnt_UART0-2是要去掉':' CR LF
        {
            ASCtoHex(&gu8_RxdArray_UART0[1], &gu8_TxdArray_UART0[0], gvu8_RxdCnt_UART0-2);
            FpCheckCrcLrc = CheckModbusLrc;
            FpBuildCrcLrc = BuildLrc;
            FpSendRtuAscII = Modbus_Send_ASCII;
        }
    }
    else                                        //uart1modbus通讯
    {
        if (gs_Setup.ComMode_1 == COM_BUS_R)
        {
            RxdArry_To_TxdArry(1, ulFrameLength_UART1);
            FpCheckCrcLrc = CheckModBusCrc16;
            FpBuildCrcLrc = BuildCrc16;
            FpSendRtuAscII = Modbus_Send_RTU;
        }
        else    //Modbus_ASCII方式转成Hex再进行处理 gvu8_RxdCnt_UART1-2是要去掉':' CR LF
        {
            ASCtoHex(&gu8_RxdArray_UART1[1], &gu8_TxdArray_UART1[0], gvu8_RxdCnt_UART1-2);
            FpCheckCrcLrc = CheckModbusLrc;
            FpBuildCrcLrc = BuildLrc;
            FpSendRtuAscII = Modbus_Send_ASCII;
        }
    }
    
    if ((gs_Setup.TareNumber_2 == fu8p_Array[0] && fu8p_Array == gu8_TxdArray_UART0) ||
        (gs_Setup.TareNumber_1 == fu8p_Array[0] && fu8p_Array == gu8_TxdArray_UART1))
    {
        switch (fu8p_Array[1]) 
        {
            case READREGISTER:      //读寄存器
                if (true == FpCheckCrcLrc(fu8p_Array, 6))        //检验正确
                {
                    ModBus_Read_Register(fu8p_Array);
                }                
                break;
            case WRITEREGISTER:     //写单寄存器
                if (true == FpCheckCrcLrc(fu8p_Array, 6))        //检验正确
                {
                    ModBus_Write_2Byte(fu8p_Array);
                }
                break;
            case WRITEREGISTERS:    //写多寄存器
                if (true == FpCheckCrcLrc(fu8p_Array, 11))        //检验正确
                {
                    ModBus_Write_4Byte(fu8p_Array);
                }
                break;
            case READCOIL:          //读线圈
                if (true == FpCheckCrcLrc(fu8p_Array, 6))        //检验正确
                {
                    Modbus_Read_CoilDeal();
                    Modbus_Read_Coil(fu8p_Array);
                }
                break;
            case WRITECOIL:         //写线圈
                if (true == FpCheckCrcLrc(fu8p_Array, 6))        //检验正确
                {
                    Modbus_Write_CoilDeal(fu8p_Array);
                }
                break;
            default:                //不支持的功能码
                Modbus_Error(FUNCTIONERRO, fu8p_Array);
                break;
        }
    }
}

/***********************************************************************
函 数 名：void IRQ_ModbusRTU_UART0(void)
功    能：串口0中断服务程序
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-7-24 14:11
修    改：丛海旭               日    期：2011-5-11
***********************************************************************/
void IRQ_ModbusRTU_UART0(void)
{
    u32 i;
    u16 UARTStatus;
    u8 u8Data=0;
    u8 u8DataHi = 0;    //存储UART1->RxBUFR高8位，帧错误和校验位
    
    //读取状态寄存器
    UARTStatus=UART_FlagStatus(UART0);
    
    //超时中断
    if (UARTStatus&UART_TimeOutIdle)
    {
        if (DelayT15==DelayType_UART0)
        {
            bOverT15_UART0=true;
            //设置超时非空中断的超时长度为2个字符
            UART_TimeOutPeriodConfig(UART0,22);
            DelayType_UART0=DelayT35;
        }
        else if ((DelayT35==DelayType_UART0)&&(true==bOverT15_UART0))
        {
            bOverT35_UART0=true;
            if (true==bBeginReceive_UART0)
            {
                bBeginReceive_UART0=false;
                bFinishReceive_UART0=true;
                DisableRecIRQ_UART0();      //关闭接收中断
            }
            UART_ItConfig(UART0,UART_TimeOutIdle,DISABLE);
            DelayType_UART0=DelayNo;
        }
    }
    
    //有数据接收
    if (UARTStatus&UART_RxBufNotEmpty)
    {
        u8Data=UART0->RxBUFR;        
        u8DataHi = (UART0->RxBUFR >> 8);
        //启动超时定时，设置超时非空中断的超时长度为1.5个字符
        UART_TimeOutPeriodConfig(UART0,17);
        DelayType_UART0=DelayT15;
        UART_ItConfig(UART0,UART_TimeOutIdle,ENABLE);
        
        if (true==bOverT15_UART0)
        {
            bOverT15_UART0=false;
            if (true==bOverT35_UART0)
            {
                bOverT35_UART0=false;
                bFinishReceive_UART0=false;
                //当地址符合或者是广播地址，则开始接收
//                if ((u8Data==u8ModbusAddr_UART0)||(0==u8Data))   //为多机通讯删除
//                {
                    bBeginReceive_UART0=true;
                    gu8_RxdArray_UART0[0]=u8Data;
                    gu8_DebugRxdArray_UART0[0] = u8DataHi;    //波特率自动匹配中使用
                    ulInsertPos_UART0=1;
                    ulFrameLength_UART0=1;
//                }
            }
            else
            {
                bBeginReceive_UART0=false;
            }
        }
        else
        {
            if (true==bBeginReceive_UART0)
            {
                gu8_RxdArray_UART0[ulInsertPos_UART0]=u8Data;
                gu8_DebugRxdArray_UART0[ulInsertPos_UART0] = u8DataHi;    //波特率自动匹配中使用
                ulInsertPos_UART0=(ulInsertPos_UART0+1)%MAX(RXDLEN_BUS_R,RXDLEN_BUS_A);
                ulFrameLength_UART0+=1;
                
//                do
//              {
//                  gu8_RxdArray_UART0[ulInsertPos]=UART0->RxBUFR;
//                  ulInsertPos=(ulInsertPos+1)%RXBUFSIZE;
//                  ulFrameLength+=1;
//              }
//              while(UART_FlagStatus(UART0)&(u16)UART_RxBufNotEmpty);
            }
        }
    }
    
    //缓冲区半空和缓冲区全空引起的中断
    if (true==bUARTSending_UART0)
    {
        //因为半空所引起的中断
        if (UARTStatus&UART_TxHalfEmpty)
        {
            //发送缓冲区是否有数据
            if (ulTBUFAvailNum_UART0>0)
            {
                if (ulTBUFAvailNum_UART0>=8)
                {
                    for(i=0;i<8;i++)
                    {
                        UART0->TxBUFR=gu8_TxdArray_UART0[ulTBUFBegin_UART0];
                        ulTBUFBegin_UART0=(ulTBUFBegin_UART0+1)%TXDLEN_BUS_R;
                    }
                    ulTBUFAvailNum_UART0-=8;
                }
                else
                {
                    for(i=0;i<ulTBUFAvailNum_UART0;i++)
                    {
                        UART0->TxBUFR=gu8_TxdArray_UART0[ulTBUFBegin_UART0];
                        ulTBUFBegin_UART0=(ulTBUFBegin_UART0+1)%TXDLEN_BUS_R;
                    }
                    ulTBUFAvailNum_UART0=0;
                    
                    UART_ItConfig(UART0,UART_TxHalfEmpty,DISABLE);
                }
            }
            else
            {
                UART_ItConfig(UART0,UART_TxHalfEmpty,DISABLE);
            }
            
            //由于对半空的操作可能会更新状态，所以要更新一下
            UARTStatus=UART_FlagStatus(UART0);
        }
        
        if (UARTStatus&UART_TxEmpty)
        {
            if (ulTBUFAvailNum_UART0==0)
            {
                bUARTSending_UART0=false; //串口没有发送操作
                DisableSendIRQ_UART0();
                EnableRecIRQ_UART0();
                Sio_485_Enable();
                gb_S485Recieve = true;
            }
        }
    }
    else
    {
        DisableSendIRQ_UART0();
        EnableRecIRQ_UART0();
        Sio_485_Enable();
    }
}

/***********************************************************************
函 数 名：void IRQ_Modbus_RTU_UART1(void)
功    能：串口0中断服务程序
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：jzt                  日    期：2007-7-24 14:11
修    改：丛海旭               日    期：2011-5-11
***********************************************************************/
void IRQ_Modbus_RTU_UART1(void)
{
    u32 i;
    u16 UARTStatus;
    u8 u8Data=0;
    u8 u8DataHi = 0;    //存储UART1->RxBUFR高8位，帧错误和校验位
    
    //读取状态寄存器
    UARTStatus=UART_FlagStatus(UART1);
    
    //超时中断
    if (UARTStatus&UART_TimeOutIdle)
    {
        if (DelayT15==DelayType_UART1)
        {
            bOverT15_UART1=true;
            //设置超时非空中断的超时长度为2个字符
            UART_TimeOutPeriodConfig(UART1,22);
            DelayType_UART1=DelayT35;
        }
        else if ((DelayT35==DelayType_UART1)&&(true==bOverT15_UART1))
        {
            bOverT35_UART1=true;
            if (true==bBeginReceive_UART1)
            {
                bBeginReceive_UART1=false;
                bFinishReceive_UART1=true;
                DisableRecIRQ_UART1();      //关闭接收中断
            }
            UART_ItConfig(UART1,UART_TimeOutIdle,DISABLE);
            DelayType_UART1=DelayNo;
        }
    }
    
    //有数据接收
    if (UARTStatus&UART_RxBufNotEmpty)
    {
        u8Data=UART1->RxBUFR;        
        u8DataHi = (UART1->RxBUFR >> 8);
        //启动超时定时，设置超时非空中断的超时长度为1.5个字符
        UART_TimeOutPeriodConfig(UART1,17);
        DelayType_UART1=DelayT15;
        UART_ItConfig(UART1,UART_TimeOutIdle,ENABLE);
        
        if (true==bOverT15_UART1)
        {
            bOverT15_UART1=false;
            if (true==bOverT35_UART1)
            {
                bOverT35_UART1=false;
                bFinishReceive_UART1=false;
                //当地址符合或者是广播地址，则开始接收
//                if ((u8Data==u8ModbusAddr_UART1)||(0==u8Data))   //为多机通讯删除
//                {
                    bBeginReceive_UART1=true;
                    gu8_RxdArray_UART1[0]=u8Data;
                    gu8_DebugRxdArray_UART1[0] = u8DataHi;    //波特率自动匹配中使用
                    ulInsertPos_UART1=1;
                    ulFrameLength_UART1=1;
//                }
            }
            else
            {
                bBeginReceive_UART1=false;
            }
        }
        else
        {
            if (true==bBeginReceive_UART1)
            {
                gu8_RxdArray_UART1[ulInsertPos_UART1]=u8Data;
                gu8_DebugRxdArray_UART1[ulInsertPos_UART1] = u8DataHi;    //波特率自动匹配中使用
                ulInsertPos_UART1=(ulInsertPos_UART1+1)%MAX(RXDLEN_BUS_R,RXDLEN_BUS_A);
                ulFrameLength_UART1+=1;
                
//                do
//              {
//                  gu8_RxdArray_UART1[ulInsertPos]=UART1->RxBUFR;
//                  ulInsertPos=(ulInsertPos+1)%RXBUFSIZE;
//                  ulFrameLength+=1;
//              }
//              while(UART_FlagStatus(UART1)&(u16)UART_RxBufNotEmpty);
            }
        }
    }
    
    //缓冲区半空和缓冲区全空引起的中断
    if (true==bUARTSending_UART1)
    {
        //因为半空所引起的中断
        if (UARTStatus&UART_TxHalfEmpty)
        {
            //发送缓冲区是否有数据
            if (ulTBUFAvailNum_UART1>0)
            {
                if (ulTBUFAvailNum_UART1>=8)
                {
                    for(i=0;i<8;i++)
                    {
                        UART1->TxBUFR=gu8_TxdArray_UART1[ulTBUFBegin_UART1];
                        ulTBUFBegin_UART1=(ulTBUFBegin_UART1+1)%TXDLEN_BUS_R;
                    }
                    ulTBUFAvailNum_UART1-=8;
                }
                else
                {
                    for(i=0;i<ulTBUFAvailNum_UART1;i++)
                    {
                        UART1->TxBUFR=gu8_TxdArray_UART1[ulTBUFBegin_UART1];
                        ulTBUFBegin_UART1=(ulTBUFBegin_UART1+1)%TXDLEN_BUS_R;
                    }
                    ulTBUFAvailNum_UART1=0;
                    
                    UART_ItConfig(UART1,UART_TxHalfEmpty,DISABLE);
                }
            }
            else
            {
                UART_ItConfig(UART1,UART_TxHalfEmpty,DISABLE);
            }
            
            //由于对半空的操作可能会更新状态，所以要更新一下
            UARTStatus=UART_FlagStatus(UART1);
        }
        
        if (UARTStatus&UART_TxEmpty)
        {
            if (ulTBUFAvailNum_UART1==0)
            {
                bUARTSending_UART1=false; //串口没有发送操作
                DisableSendIRQ_UART1();
                EnableRecIRQ_UART1();
//              Sio_485_Enable();
            }
        }
    }
    else
    {
        DisableSendIRQ_UART1();
        EnableRecIRQ_UART1();
//        Sio_485_Enable();
    }
}



