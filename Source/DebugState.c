/*
Description:            调试界面相关函数 串口参数自动匹配等处理
Author:                 丛海旭
Date:                   2013-12-9
*/
#include "include.h"

#define DEBUG_UART1             0
#define DEBUG_UART2             1

#define DEBUG_END               1
#define DEBUG_BAUDRATE_NUM      6       //支持的波特率个数





bool gb_DebugUart1;     //串口1正在调试标志位，要屏蔽正常的通讯函数
bool gb_DebugUart0;     //串口2正在调试标志位，要屏蔽正常的通讯函数
u32 gu32_DebugTimeBuf;
u32 gu32_DebugError;    //调试过程错误标志
bool gb_ModbusAnswerFlg_Uart0;   //Modbus协议有返回发送标志位
bool gb_ModbusAnswerFlg_Uart1;   //Modbus协议有返回发送标志位
u8 gu8_DebugRxdArray_UART1[RXDLEN];        //接收数组存储UARTn->RxBUFR高8位（校验位和帧错误）
u8 gu8_DebugRxdArray_UART0[RXDLEN];        //接收数组存储UARTn->RxBUFR高8位（校验位和帧错误）
u32 gu32_ComModeBak_1;
u32 gu32_ComModeBak_2;


/*********************************************************************
函数名称: void Debug_UartInit(u8 fu8_BR, u8 fu8_P, u8 fu8_UratType )
功    能: 初始化串口自适应所用的串口参数
说    明: 
入口参数: fu8_BR=波特率  fu8_P=校验  fu8_UratType=串口号
返 回 值: 
设    计: 丛海旭               时    间: 2014-2-20
修    改:                      时    间: 
*********************************************************************/
void Debug_UartInit(u8 fu8_BR, u8 fu8_P, u8 fu8_UratType )
{
    if (fu8_UratType == 1)   //串口1
    {
        gb_ModbusAnswerFlg_Uart1 = false;
        //清除判断校验和帧错误的数组 清除接收缓存数组
        memset(gu8_RxdArray_UART1, 0, sizeof(gu8_RxdArray_UART1));
        memset(gu8_DebugRxdArray_UART1, 0, sizeof(gu8_DebugRxdArray_UART1));
        gs_Setup.ComMode_1 = COM_BUS_R;
        switch (fu8_P)
        {
            case 0:
            default:
                gs_Setup.ParityMode_1 = P_18N1;
                break;
            case 1:
                gs_Setup.ParityMode_1 = P_18E1;
                break;
            case 2:
                gs_Setup.ParityMode_1 = P_18O1;
                break;
            case 3:
                gs_Setup.ParityMode_1 = P_18N2;
                break;
            case 4:
                gs_Setup.ParityMode_1 = P_17E1;
                break;
            case 5:
                gs_Setup.ParityMode_1 = P_17O1;
                break;
            case 6:
                gs_Setup.ParityMode_1 = P_17N2;
                break;
        }
        switch (fu8_BR)
        {
            case 0:
            default:
                gs_Setup.BaudRate_1 = 2;    //9600
                break;
            case 1:
                gs_Setup.BaudRate_1 = 3;    //19200
                break;
            case 2:
                gs_Setup.BaudRate_1 = 1;    //4800
                break;
            case 3:
                gs_Setup.BaudRate_1 = 4;    //38400
                break;
            case 4:
                gs_Setup.BaudRate_1 = 5;    //57600
                break;
            case 5:
                gs_Setup.BaudRate_1 = 0;    //2400
                break;
            case 6:
                gs_Setup.BaudRate_1 = 6;    //115200
                break;
        }
        Uart1_Init();
    }
    else    //其他值串口2
    {
        gb_ModbusAnswerFlg_Uart0 = false;
        //清除判断校验和帧错误的数组 清除接收缓存数组
        memset(gu8_RxdArray_UART0, 0, sizeof(gu8_RxdArray_UART0));
        memset(gu8_DebugRxdArray_UART0, 0, sizeof(gu8_DebugRxdArray_UART0));
        
        gs_Setup.ComMode_2 = COM_BUS_R;
        switch (fu8_P)
        {
            case 0:
            default:
                gs_Setup.ParityMode_2 = P_18N1;
                break;
            case 1:
                gs_Setup.ParityMode_2 = P_18E1;
                break;
            case 2:
                gs_Setup.ParityMode_2 = P_18O1;
                break;
            case 3:
                gs_Setup.ParityMode_2 = P_18N2;
                break;
            case 4:
                gs_Setup.ParityMode_2 = P_17E1;
                break;
            case 5:
                gs_Setup.ParityMode_2 = P_17O1;
                break;
            case 6:
                gs_Setup.ParityMode_2 = P_17N2;
                break;
        }
        switch (fu8_BR)
        {
            case 0:
                gs_Setup.BaudRate_2 = 2;    //9600
                break;
            case 1:
                gs_Setup.BaudRate_2 = 3;    //19200
                break;
            case 2:
                gs_Setup.BaudRate_2 = 1;    //4800
                break;
            case 3:
                gs_Setup.BaudRate_2 = 4;    //38400
                break;
            case 4:
                gs_Setup.BaudRate_2 = 5;    //57600
                break;
            case 5:
                gs_Setup.BaudRate_2 = 0;    //2400
                break;
            case 6:
                gs_Setup.BaudRate_2 = 6;    //115200
                break;
        }
        Uart0_Init();
    }
}

/*********************************************************************
函数名称: u8 Debug_UartJudgeDeal(u8 *fpu8_Array)
功    能: 串口接收到数据后的判断和处理
说    明: 
入口参数: 
返 回 值: ^0=数据不匹配  ^1=校验不匹配(MODBUS)  ^2=帧错误(MODBUS)
          ^3=7位数据校验不匹配(READ) ^4=校验不匹配(READ) ^5=帧错误(READ)
设    计: 丛海旭               时    间: 2014-2-20
修    改:                      时    间: 
*********************************************************************/
u8 Debug_UartJudgeDeal(u8 *fpu8_Array)
{
    u8 lu8_Result = 0;
    u8 i;
    
    if (fpu8_Array == gu8_TxdArray_UART0)       //uart0modbus通讯
    {   //先判断接收到的数据是否符合Modbus_rtu通讯的格式
        gs_Setup.ComMode_2 = COM_BUS_R;
        gb_ModbusAnswerFlg_Uart0 = false;
        Com_Modbus(gu8_TxdArray_UART0);
        if (gb_ModbusAnswerFlg_Uart0 == true)
        {
            gb_ModbusAnswerFlg_Uart0 = false;
            //符合格式再判断是否有校验和帧错误
            for (i = 0; i < 6; i++)    //检查是否有帧错误或校验错误
            {
                if ((gu8_DebugRxdArray_UART0[i] & 0x01) != 0)    //校验错误
                    lu8_Result |= 0x02;
                if ((gu8_DebugRxdArray_UART0[i] & 0x02) != 0)    //帧错误
                     lu8_Result |= 0x04;
            }
        }
        else
        {   //再判断接收到的数据是否符合Modbus_ASCII通讯的格式
            gs_Setup.ComMode_2 = COM_BUS_A;
            gb_ModbusAnswerFlg_Uart0 = false;
            Com_Modbus(gu8_TxdArray_UART0);
            if (gb_ModbusAnswerFlg_Uart0 == true)
            {
                gb_ModbusAnswerFlg_Uart0 = false;
                //符合格式再判断是否有校验和帧错误
                for (i = 0; i < 6; i++)    //检查是否有帧错误或校验错误
                {
                    if ((gu8_DebugRxdArray_UART0[i] & 0x01) != 0)    //校验错误
                        lu8_Result |= 0x02;
                    if ((gu8_DebugRxdArray_UART0[i] & 0x02) != 0)    //帧错误
                         lu8_Result |= 0x04;
                }
            }
            else
            {
                gs_Setup.ComMode_2 = COM_BUS_R;
                //再判断是否符合Read方式
                RxdArry_To_TxdArry(0, RXDLEN);
                for (i = 0; i < RXDLEN; i++)    //屏蔽最高位，有可能是校验位
                    gu8_TxdArray_UART0[i] &= 0x7F;
                if (RxData_Check(gu8_TxdArray_UART0))             //接收数据的校验
                {   //符合格式再判断是否有校验和帧错误
                    for (i = 0; i < 8; i++)
                    {   //7位数据位时最高位有可能是校验位
                        if ((gu8_RxdArray_UART0[i] & 0x80) != 0)
                            lu8_Result |= 0x08; //校验错误
                        if ((gu8_DebugRxdArray_UART0[i] & 0x01) != 0) //校验错误
                            lu8_Result |= 0x10;
                        if ((gu8_DebugRxdArray_UART0[i] & 0x02) != 0) //帧错误
                             lu8_Result |= 0x20;
                    }
                    if (lu8_Result == 0) //匹配
                        gs_Setup.ComMode_2 = COM_READ;
                }
                else
                    lu8_Result |= 0x01;
                //清除校验用数组
                memset(gu8_DebugRxdArray_UART0, 0, sizeof(gu8_DebugRxdArray_UART0));
            }
        }
    }
    else                                        //uart1modbus通讯
    {   //先判断接收到的数据是否符合Modbus_rtu通讯的格式
        gs_Setup.ComMode_1 = COM_BUS_R;
        gb_ModbusAnswerFlg_Uart1 = false;
        Com_Modbus(gu8_TxdArray_UART1);
        if (gb_ModbusAnswerFlg_Uart1 == true)
        {
            gb_ModbusAnswerFlg_Uart1 = false;
            //符合格式再判断是否有校验和帧错误
            for (i = 0; i < 6; i++)    //检查是否有帧错误或校验错误
            {
                if ((gu8_DebugRxdArray_UART1[i] & 0x01) != 0) //校验错误
                    lu8_Result |= 0x02;
                if ((gu8_DebugRxdArray_UART1[i] & 0x02) != 0) //帧错误
                     lu8_Result |= 0x04;
            }
        }
        else
        {   //再判断接收到的数据是否符合Modbus_ASCII通讯的格式
            gs_Setup.ComMode_1 = COM_BUS_A;
            gb_ModbusAnswerFlg_Uart1 = false;
            Com_Modbus(gu8_TxdArray_UART1);
            if (gb_ModbusAnswerFlg_Uart1 == true)
            {
                gb_ModbusAnswerFlg_Uart1 = false;
                //符合格式再判断是否有校验和帧错误
                for (i = 0; i < 6; i++)    //检查是否有帧错误或校验错误
                {
                    if ((gu8_DebugRxdArray_UART1[i] & 0x01) != 0)    //校验错误
                        lu8_Result |= 0x02;
                    if ((gu8_DebugRxdArray_UART1[i] & 0x02) != 0)    //帧错误
                         lu8_Result |= 0x04;
                }
            }
            else
            {
                gs_Setup.ComMode_1 = COM_BUS_R;
                //再判断是否符合Read方式
                RxdArry_To_TxdArry(1, RXDLEN);
                for (i = 0; i < RXDLEN; i++)    //屏蔽最高位，有可能是校验位
                    gu8_TxdArray_UART1[i] &= 0x7F;
                if (RxData_Check(gu8_TxdArray_UART1))             //接收数据的校验
                {   //符合格式再判断是否有校验和帧错误
                    for (i = 0; i < 8; i++)
                    {   //7位数据位时最高位有可能是校验位
                        if ((gu8_RxdArray_UART1[i] & 0x80) != 0)
                            lu8_Result |= 0x08; //校验错误
                        if ((gu8_DebugRxdArray_UART1[i] & 0x01) != 0) //校验错误
                            lu8_Result |= 0x10;
                        if ((gu8_DebugRxdArray_UART1[i] & 0x02) != 0) //帧错误
                             lu8_Result |= 0x20;
                    }
                    if (lu8_Result == 0) //匹配
                        gs_Setup.ComMode_1 = COM_READ;
                }
                else
                    lu8_Result |= 0x01;
                //清除校验用数组
                memset(gu8_DebugRxdArray_UART1, 0, sizeof(gu8_DebugRxdArray_UART1));
            }
        }
    }
    return lu8_Result;
}

/*********************************************************************
函数名称: void Key_DebugState(void)
功    能: 调试界面处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-2-17
修    改:                      时    间: 
*********************************************************************/
void Key_DebugState(void)
{
    static u8 su8_FailCnt;
    static bool sb_RecedDataFlg;    //有数据接收过标志，用来判断匹配失败后是E01还是E02
    u8 i;
    
    if (gu32_KeyCode == K_RIGHT)
    {
        if (gu32_DebugError == 0 && gb_DebugUart1 == false && gb_DebugUart0 == false)
        {
            gu8_Item++;
            if (gu8_Item > DEBUG_END)
                gu8_Item = DEBUG_UART1;
        }
    }
    
    switch (gu8_Item)
    {
        case DEBUG_UART1:
            if (gb_DebugUart1 == true)
            {   //开始串口自适应过程
                /*该串口自适应原理类似枚举法，先设置为Modbus方式，然后波特率依次以：
                  9600 19200 4800 38400 57600 2400 115200的顺序变化，来比较接收到的数据是否正常
                  每个波特率下接收3次数据来判断是哪个通讯方式。连续5秒没有数据接收则换下一个波特率
                  因为MODBUS通讯采用的是时间间隔来判断一帧数据是否接收完成，所以即使是READ通讯
                  方式也能正确接收到数据。
                  对接收到的数据进行比对，就可以确认MODBUS还是READ，这样协议和波特率就都确定了，
                  然后在判断校验方式。
                */
                
                if (gu32_Sys10ms - gu32_DebugTimeBuf >= 500)
                {   //超过5秒没有数据接收
                    gu32_DebugTimeBuf = gu32_Sys10ms;   //重新赋值超时时间缓存
                    if (++ulParameter.ui[0] > DEBUG_BAUDRATE_NUM)
                    {   //自动匹配失败
                        //有接收到过数据报警E02否则E01
                        gu32_DebugError = (sb_RecedDataFlg ? 2 : 1);
                        gb_DebugUart1 = false;
                        gb_FlashFlg = false;
                        gu32_DebugTimeBuf = gu32_Sys10ms;
                        BuzzerStart(100);
                    }
                    else
                        Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 1);
                }
                
                //已经初始化完成，进行串口自适应过程判断
                if (bFinishReceive_UART1 == true)
                {   //接收到数据
                    sb_RecedDataFlg = true;
                    gu32_DebugTimeBuf = gu32_Sys10ms;   //重新赋值超时时间缓存
                    i = Debug_UartJudgeDeal(gu8_TxdArray_UART1);
                    if (i & 0x01)
                    {   //比对接收到的数据不对
                        if (++su8_FailCnt >= 3)
                        {   //接收到的数据进行比对，连续3次失败则进行下一个波特率
                            su8_FailCnt = 0;
                            if (++ulParameter.ui[0] > DEBUG_BAUDRATE_NUM)
                            {   //自动匹配失败
                                gu32_DebugError = 2;
                                gb_DebugUart1 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                            else
                            {
                                ulParameter.ui[1] = 0;
                                Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 1);
                            }
                        }
                    }
                    else if (i & 0x3E)
                    {   
                        if (i & 0x06)
                        {   //modbus校验错误或帧错误
                            //modbus只有8位数据位
                            if (++ulParameter.ui[1] > 3)
                            {//自动匹配失败,不支持的校验
                                gu32_DebugError = 3;
                                gb_DebugUart1 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                        }
                        else if (i & 0x08)
                        {   //数据是read格式7位数据
                            if (ulParameter.ui[1] < 4)
                            {
                                ulParameter.ui[1] = 4;  //17E1校验
                            }
                            else
                            {
                                if (++ulParameter.ui[1] > 6)
                                {//自动匹配失败,不支持的校验
                                    gu32_DebugError = 3;
                                    gb_DebugUart1 = false;
                                    gb_FlashFlg = false;
                                    BuzzerStart(100);
                                }
                            }
                        }
                        else
                        {   //read校验错误或帧错误
                            //read有8位数据位和7位数据位
                            if (++ulParameter.ui[1] > 6)
                            {//自动匹配失败,不支持的校验
                                gu32_DebugError = 3;
                                gb_DebugUart1 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                        }
                        Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 1);
                    }
                    else
                    {   //接收到数据正确，保存相关参数值
                        FRAM_WriteDW(FMAddr_ComMode_1, gs_Setup.ComMode_1);
                        FRAM_WriteDW(FMAddr_BaudRate_1, gs_Setup.BaudRate_1);
                        FRAM_WriteDW(FMAddr_ParityMode_1, gs_Setup.ParityMode_1);
                        Uart1_Init();
                        //显示FiniSh代表匹配成功
                        gu32_DebugError = 10;
                        gb_DebugUart1 = false;
                        gb_FlashFlg = false;
                        BuzzerStart(100);
                    }
                    bFinishReceive_UART1 = false;
                }
            }
            break;
        case DEBUG_UART2:
            if (gb_DebugUart0 == true)
            {   //开始串口自适应过程
                /*该串口自适应原理类似枚举法，先设置为Modbus方式，然后波特率依次以：
                  9600 19200 4800 38400 57600 2400 115200的顺序变化，来比较接收到的数据是否正常
                  每个波特率下接收3次数据来判断是哪个通讯方式。连续5秒没有数据接收则换下一个波特率
                  因为MODBUS通讯采用的是时间间隔来判断一帧数据是否接收完成，所以即使是READ通讯
                  方式也能正确接收到数据。
                  对接收到的数据进行比对，就可以确认MODBUS还是READ，这样协议和波特率就都确定了，
                  然后在判断校验方式。
                */
                
                if (gu32_Sys10ms - gu32_DebugTimeBuf >= 500)
                {   //超过5秒没有数据接收
                    gu32_DebugTimeBuf = gu32_Sys10ms;   //重新赋值超时时间缓存
                    if (++ulParameter.ui[0] > DEBUG_BAUDRATE_NUM)
                    {   //自动匹配失败
                        //有接收到过数据报警E02否则E01
                        gu32_DebugError = (sb_RecedDataFlg ? 2 : 1);
                        gb_DebugUart0 = false;
                        gb_FlashFlg = false;
                        gu32_DebugTimeBuf = gu32_Sys10ms;
                        BuzzerStart(100);
                    }
                    else
                        Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 0);
                }
                
                //已经初始化完成，进行串口自适应过程判断
                if (bFinishReceive_UART0 == true)
                {   //接收到数据
                    sb_RecedDataFlg = true;
                    gu32_DebugTimeBuf = gu32_Sys10ms;   //重新赋值超时时间缓存
                    i = Debug_UartJudgeDeal(gu8_TxdArray_UART0);
                    if (i & 0x01)
                    {   //比对接收到的数据不对
                        if (++su8_FailCnt >= 3)
                        {   //接收到的数据进行比对，连续3次失败则进行下一个波特率
                            su8_FailCnt = 0;
                            if (++ulParameter.ui[0] > DEBUG_BAUDRATE_NUM)
                            {   //自动匹配失败
                                gu32_DebugError = 2;
                                gb_DebugUart0 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                            else
                            {
                                ulParameter.ui[1] = 0;
                                Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 0);
                            }
                        }
                    }
                    else if (i & 0x3E)
                    {   
                        if (i & 0x06)
                        {   //modbus校验错误或帧错误
                            //modbus只有8位数据位
                            if (++ulParameter.ui[1] > 3)
                            {//自动匹配失败,不支持的校验
                                gu32_DebugError = 3;
                                gb_DebugUart0 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                        }
                        else if (i & 0x08)
                        {   //数据是read格式7位数据
                            if (ulParameter.ui[1] < 4)
                            {
                                ulParameter.ui[1] = 4;  //17E1校验
                            }
                            else
                            {
                                if (++ulParameter.ui[1] > 6)
                                {//自动匹配失败,不支持的校验
                                    gu32_DebugError = 3;
                                    gb_DebugUart0 = false;
                                    gb_FlashFlg = false;
                                    BuzzerStart(100);
                                }
                            }
                        }
                        else
                        {   //read校验错误或帧错误
                            //read有8位数据位和7位数据位
                            if (++ulParameter.ui[1] > 6)
                            {//自动匹配失败,不支持的校验
                                gu32_DebugError = 3;
                                gb_DebugUart0 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                        }
                        Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 0);
                    }
                    else
                    {   //接收到数据正确，保存相关参数值
                        FRAM_WriteDW(FMAddr_ComMode_2, gs_Setup.ComMode_2);
                        FRAM_WriteDW(FMAddr_BaudRate_2, gs_Setup.BaudRate_2);
                        FRAM_WriteDW(FMAddr_ParityMode_2, gs_Setup.ParityMode_2);
                        Uart0_Init();
                        //显示FiniSh代表匹配成功
                        gu32_DebugError = 10;
                        gb_DebugUart0 = false;
                        gb_FlashFlg = false;
                        BuzzerStart(100);
                    }
                    bFinishReceive_UART0 = false;
                }
            }
            break;
    }
    
    
    switch (gu32_KeyCode)
    {
        case K_ESC:
            if (gu32_DebugError != 0)
                gu32_DebugError = 0;
            else if (gb_DebugUart0 == true || gb_DebugUart1 == true)
            {
                gb_DebugUart0 = false;
                gb_DebugUart1 = false;
            }
            else
            {
                gb_DebugUart1 = false;
                gb_DebugUart0 = false;
                gb_FlashFlg = false;
                Read_SetupData();   //重新读取工作参数值，主要是串口相关
                Uart0_Init();                //串口0初始化
                Uart1_Init();                //串口1初始化
                gu32_SystemState = STOPSTATE;
            }
            break;
        case K_ENTER:
            if (gu8_Item == DEBUG_UART1)
            {
                sb_RecedDataFlg = false;
                gb_DebugUart1 = true;
                ulParameter.ui[0] = 0;
                ulParameter.ui[1] = 0;
                su8_FailCnt = 0;
                Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 1);
                gu32_DebugTimeBuf = gu32_Sys10ms;
            }
            else if (gu8_Item == DEBUG_UART2)
            {
                sb_RecedDataFlg = false;
                gb_DebugUart0 = true;
                ulParameter.ui[0] = 0;
                ulParameter.ui[1] = 0;
                su8_FailCnt = 0;
                Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 0);
                gu32_DebugTimeBuf = gu32_Sys10ms;
            }
            break;
        default:
            break;
    }
    
}

/*********************************************************************
函数名称: void Disp_SetupState(void)
功    能: 调试界面显示处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-2-17
修    改:                      时    间: 
*********************************************************************/
void Disp_DebugState(void)
{
    u8 i;
    
    switch (gu8_Item)
    {
        case DEBUG_UART1:
            VFD_AuxDisp_Str("-1-");
            if (gb_DebugUart1 == true)
            {
                gb_FlashFlg = true;
                for (i = 0; i <= ulParameter.ui[0]; i++)
                {   //6种波特率，适应到第几种就亮几个“-”
                    if (i <= 5)
                        gu8_MainDispArray[i] = SEG_1LINE;
                    else    //ulParameter.ui[0]最大到6
                        gu8_MainDispArray[5] = SEG_1LINE;
                }
                if (ulParameter.ui[0] <= 5)
                    Flash_MainDisp(0x20 >> ulParameter.ui[0]);                //闪烁处理
                else
                    Flash_MainDisp(0x20 >> 5);
            }
            else
            {
                if (gu32_DebugError != 0)
                {
                    switch (gu32_DebugError)
                    {
                        case 1:
                            VFD_MainDisp_Str("   E01");
                            break;
                        case 2:
                            VFD_MainDisp_Str("   E02");
                            break;
                        case 3:
                            VFD_MainDisp_Str("   E03");
                            break;
                        case 10:
                            VFD_MainDisp_Str("FINISH");
                    }
                }
                else
                    VFD_MainDisp_Str(" UART1");
            }
            break;
        case DEBUG_UART2:
            VFD_AuxDisp_Str("-2-");
            if (gb_DebugUart0 == true)
            {
                gb_FlashFlg = true;
                for (i = 0; i <= ulParameter.ui[0]; i++)
                {   //6种波特率，适应到第几种就亮几个“-”
                    if (i <= 5)
                        gu8_MainDispArray[i] = SEG_1LINE;
                    else    //ulParameter.ui[0]最大到6
                        gu8_MainDispArray[5] = SEG_1LINE;
                }
                if (ulParameter.ui[0] <= 5)
                    Flash_MainDisp(0x20 >> ulParameter.ui[0]);                //闪烁处理
                else
                    Flash_MainDisp(0x20 >> 5);
            }
            else
            {
                if (gu32_DebugError != 0)
                {
                    switch (gu32_DebugError)
                    {
                        case 1:
                            VFD_MainDisp_Str("   E01");
                            break;
                        case 2:
                            VFD_MainDisp_Str("   E02");
                            break;
                        case 3:
                            VFD_MainDisp_Str("   E03");
                            break;
                        case 10:
                            VFD_MainDisp_Str("FINISH");
                    }
                }
                else
                    VFD_MainDisp_Str(" UART2");
            }
            break;
    }
}

