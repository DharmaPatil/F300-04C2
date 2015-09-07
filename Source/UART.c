/*
Description:            串口通讯相关功能
Author:                 丛海旭
Date:                   2013-12-9
*/

#include "include.h"
#include "MathLogic.h"


//串口1的相关参数
u8  gu8_RxdArray_UART0[RXDLEN];        //接收数组
u8  gu8_TxdArray_UART0[TXDLEN];        //发送数组
volatile u8  gvu8_TxdCnt_UART0;                 //发送计数
volatile u8  gvu8_RxdCnt_UART0;                 //接收计数
volatile bool  gvb_EndCharFlg_UART0;            //接收到结束符标志
bool  gb_SioConfigedFlg_UART0;        //串口已初始化
volatile bool  gvb_SioTxingFlg_UART0;           //串口正在发送
volatile bool  gvb_SioRxingFlg_UART0;           //串口正在接收

//串口2的相关参数
u8  gu8_RxdArray_UART1[RXDLEN];        //接收数组
u8  gu8_TxdArray_UART1[TXDLEN];        //发送数组
volatile u8  gvu8_TxdCnt_UART1;                 //发送计数
volatile u8  gvu8_RxdCnt_UART1;                 //接收计数
volatile bool  gvb_EndCharFlg_UART1;            //接收到结束符标志
bool  gb_SioConfigedFlg_UART1;        //串口已初始化
volatile bool  gvb_SioTxingFlg_UART1;           //串口正在发送
volatile bool  gvb_SioRxingFlg_UART1;           //串口正在接收


const u32 gu32_BaudRateArry[] = {2400, 4800, 9600, 19200, 38400, 57600, 115200, 128000, 192000, 240000, 256000};
const UARTParity_TypeDef g_ParityArry[] = {UART_EVEN_PARITY,      //18N2
                                           UART_EVEN_PARITY,      //18E1
                                           UART_ODD_PARITY,       //18O1
                                           UART_EVEN_PARITY,      //18N1
                                           UART_EVEN_PARITY,      //17N2
                                           UART_EVEN_PARITY,      //17E1
                                           UART_ODD_PARITY};      //17O1
                             
const UARTStopBits_TypeDef g_StopBitsArry[] = {UART_2_StopBits,     //18N2
                                               UART_1_StopBits,     //18E1
                                               UART_1_StopBits,     //18O1
                                               UART_1_StopBits,     //18N1
                                               UART_1_StopBits,     //17N2
                                               UART_1_StopBits,     //17E1
                                               UART_1_StopBits};    //17O1
                               
const UARTMode_TypeDef g_DataLenArry[] = {UARTM_8D,             //18N2
                                          UARTM_8D_P,           //18E1
                                          UARTM_8D_P,           //18O1
                                          UARTM_8D,             //18N1
                                          UARTM_8D,             //17N2
                                          UARTM_7D_P,           //17E1
                                          UARTM_7D_P};          //17O1

#ifdef DEBUG_WT_SEND
    u8 gu8_DebugState2 = 0;     //调试功能 状态2变量
#endif


bool SRCCheck(u8* u8Arry,u8 u8SRCIndex);
void SRCFrame(u8* u8Arry,u8 u8SRCIndex);
void Deal_Read(u8 *fu8p_Array);
void Com_Read(u8 *fu8p_Array);


//从波特率获得波特率寄存器值
#define getBBR(Baudrate)    (u16)(11059200/2*16/2/16/(Baudrate))


/*********************************************************************
函数名称: void Uart0_Init(void)
功    能: 串口0初始化
说    明: 通讯串口
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-20
修    改:                      时    间: 
*********************************************************************/
void Uart0_Init(void)
{
    //UART0 RS232和RS485可选
    GPIO_Config(GPIO0,(1<<9),GPIO_AF_PP);       //U0.TXD
    GPIO_BitWrite(GPIO0,9,1);
    GPIO_Config(GPIO0,(1<<8),GPIO_IN_TRI_TTL);  //U0.RXD
    GPIO_BitWrite(GPIO0,8,1);
    GPIO_Config(GPIO1,(1<<1),GPIO_OUT_PP);      //U0.S485
    
    DebugInit_UART0();
//    if (gs_Setup.ComMode_2 == COM_BUS_R)          //Modbus-rtu初始化
//        UartModbusInit_UART0();
//    else
//        UartGMInit_UART0();         //GM协议的串口初始化和Modbus-ascII初始化
}


/*********************************************************************
函数名称: void UartGMInit_UART0(void)
功    能: 串口0初始化
说    明: 通讯串口
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-20
修    改:                      时    间: 
*********************************************************************/
void UartGMInit_UART0(void)
{
    gb_SioConfigedFlg_UART0=false;
    Sio_485_Enable();
    UART_Init(UART0);               //复位
    UART_GuardTimeConfig(UART0,0); //配置Txempty置位等待延时
    UART_FifoConfig(UART0,DISABLE);//关闭FIFO
    UART_LoopBackConfig(UART0,DISABLE);//关闭回写

    //根据串口相关设置初始化串口 波特率 校验方式
    UART_Config(UART0,gu32_BaudRateArry[gs_Setup.BaudRate_2],g_ParityArry[gs_Setup.ParityMode_2],g_StopBitsArry[gs_Setup.ParityMode_2],g_DataLenArry[gs_Setup.ParityMode_2]);
    if (gs_Setup.ComMode_2 == COM_CONT || gs_Setup.ComMode_2 == COM_PRINT)
        UART_RxConfig(UART0,DISABLE);//CONT方式不使能接收
    else
        UART_RxConfig(UART0,ENABLE);//使能接收
    UART_FifoReset(UART0, UART_RxFIFO);
    UART_ItConfig(UART0,UART_RxBufNotEmpty,ENABLE);//使能"接收" 中断,发送中断应该等到需要发送的时候再允许
    UART_OnOffConfig(UART0,ENABLE);//使能串口

    EIC_IRQChannelPriorityConfig(UART0_IRQChannel, UART0_SIPL); //中断优先级
    EIC_IRQChannelConfig(UART0_IRQChannel, ENABLE); //开启IRQ0中断输入通道

    gvb_SioTxingFlg_UART0=false;
    gvb_SioRxingFlg_UART0=false;
    gb_SioConfigedFlg_UART0=true;
}


/*********************************************************************
函数名称: void UartGMInit_UART1(void)
功    能: 串口0初始化
说    明: 通讯串口
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void UartGMInit_UART1(void)
{
    gb_SioConfigedFlg_UART1 = false;
    UART_Init(UART1);               //复位
    UART_GuardTimeConfig(UART1,0); //配置Txempty置位等待延时
    UART_FifoConfig(UART1,DISABLE);//关闭FIFO
    UART_LoopBackConfig(UART1,DISABLE);//关闭回写

    //根据串口相关设置初始化串口 波特率 校验方式
    UART_Config(UART1,gu32_BaudRateArry[gs_Setup.BaudRate_1],g_ParityArry[gs_Setup.ParityMode_1],g_StopBitsArry[gs_Setup.ParityMode_1],g_DataLenArry[gs_Setup.ParityMode_1]);
    if (gs_Setup.ComMode_1 == COM_CONT || gs_Setup.ComMode_1 == COM_PRINT)
        UART_RxConfig(UART1,DISABLE);//CONT方式不使能接收
    else
        UART_RxConfig(UART1,ENABLE);//使能接收
    UART_FifoReset(UART1, UART_RxFIFO);
    UART_ItConfig(UART1,UART_RxBufNotEmpty,ENABLE);//使能"接收" 中断,发送中断应该等到需要发送的时候再允许
    UART_OnOffConfig(UART1,ENABLE);//使能串口

    EIC_IRQChannelPriorityConfig(UART1_IRQChannel, UART1_SIPL);
    EIC_IRQChannelConfig(UART1_IRQChannel, ENABLE);

    gvb_SioTxingFlg_UART1=false;
    gvb_SioRxingFlg_UART1=false;
    gb_SioConfigedFlg_UART1=true;
}

/*********************************************************************
函数名称: void Uart1_Init(void)
功    能: 串口1初始化
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void Uart1_Init(void)
{
    //UART1 RS232
    GPIO_Config(GPIO0,(1<<11),GPIO_AF_PP);      //U1.TXD
    GPIO_BitWrite(GPIO0,11,1);
    GPIO_Config(GPIO0,(1<<10),GPIO_IN_TRI_TTL); //U1.RXD
    GPIO_BitWrite(GPIO0,10,1);
    
    switch (gs_Setup.ComMode_1)
    {
        case COM_BUS_R: //MODBUS-RTU模式通讯
            UartModbusInit_UART1();
            break;
        case COM_READ:
        case COM_READ_O:
        case COM_CONT:
        case COM_PRINT:
        case COM_BUS_A:
            UartGMInit_UART1();         //GM协议的串口初始化
            break;
        case COM_PBUS:
            UARTInit_Pbus();       //PROFIBUS总线串口初始化
            break;
        default:
            break;
    }
}

void SRCFrame(u8* u8Arry,u8 u8SRCIndex)
{
    u8 i;
    u32 u16SRCResult=0;

    for(i=0;i<u8SRCIndex;i++){
        u16SRCResult+=u8Arry[i];
    }

    u16SRCResult%=100;
    u8Arry[i++]=u16SRCResult/10 + '0';
    u8Arry[i++]=u16SRCResult%10 + '0';

    u8Arry[i++]=ENDCHAR1;
    u8Arry[i++]=ENDCHAR2;
}

bool SRCCheck(u8* u8Arry,u8 u8SRCIndex)
{
    u8 i;
    u32 u16SRCResult = 0;
    u32  u8CRCH;
    u32  u8CRCL;

    for(i=0;i<u8SRCIndex;i++){
        u16SRCResult+=u8Arry[i];
    }
    u8CRCH=(u16SRCResult%100)/10 + 0x30;
    u8CRCL=(u16SRCResult%10)     + 0x30;

    if(u8CRCH!=u8Arry[u8SRCIndex])
        return false;
    if(u8CRCL!=u8Arry[u8SRCIndex+1])
        return false;
    if(u8Arry[u8SRCIndex+2]!=ENDCHAR1)
        return false;
    if(u8Arry[u8SRCIndex+3]!=ENDCHAR2)
        return false;
    
    return true;
}

void StartSend_Com(u8 *fu8p_Array)
{
    if (fu8p_Array == gu8_TxdArray_UART0)      //uart0发送
    {
        gvb_EndCharFlg_UART0=false;
        gvb_SioTxingFlg_UART0=true;                          //构造数据
        gvu8_TxdCnt_UART0=0;
        Sio_485_Disable();
        gb_S485Recieve = false; //修正RS485总线Modbus ASCII方式通讯时偶尔出现中断的问题。 
        UART_FifoReset(UART0, UART_TxFIFO);         //清空发送队列
        UART_ItConfig(UART0,UART_TxEmpty,ENABLE);   //允许串口发送中断
    }
    else                    //uart1发送
    {
        gvb_EndCharFlg_UART1=false;
        gvb_SioTxingFlg_UART1=true;                          //构造数据
        gvu8_TxdCnt_UART1=0;
        UART_FifoReset(UART1, UART_TxFIFO);         //清空发送队列
        UART_ItConfig(UART1,UART_TxEmpty,ENABLE);   //允许串口发送中断
    }
}

/*********************************************************************
函数名称: void Com_Cont(u8 *fu8p_Array)
功    能: cont模式通讯处理
说    明: Read模式RS也是用这个函数
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void Com_Cont(u8 *fu8p_Array)
{
    u8 i;
    
    //秤号
    i = (fu8p_Array == gu8_TxdArray_UART0) ? gs_Setup.TareNumber_2 : gs_Setup.TareNumber_1;
    fu8p_Array[0] = STARTCHAR;
    fu8p_Array[1] = 0x30 + i / 10;
    fu8p_Array[2] = 0x30 + i % 10;       //秤号
    //命令方式为R，连续方式为C
    if (fu8p_Array == gu8_TxdArray_UART0)
        fu8p_Array[3] = gs_Setup.ComMode_2 == COM_CONT ? 'C' : 'R';
    else
        fu8p_Array[3] = gs_Setup.ComMode_1 == COM_CONT ? 'C' : 'R';
    fu8p_Array[4] = 'S';
    if (gb_RunFlg == false) //停止状态
        fu8p_Array[5] = '0';
    else
    {
        if (gb_BatchOverFlg)    //批次完成
            fu8p_Array[5] = '8';
        else if (gu32_Process == T1PROCESS) //加料前
            fu8p_Array[5] = '1';
        else if (gu32_Process == LARGEPROCESS)  //快加
            fu8p_Array[5] = '2';
        else if (gu32_Process == MIDDLEPROCESS) //中加
            fu8p_Array[5] = '3';
        else if (gu32_Process == SMALLPROCESS)  //慢加
            fu8p_Array[5] = '4';
        else if (gu32_Process == T3PROCESS || gu32_Process == T4PROCESS)    //定值
        {
            if ((gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B) && (gb_FlapWaitFlg || gb_FlapStartFlg))  //无斗 拍袋
                fu8p_Array[5] = '6';
            else
                fu8p_Array[5] = '5';
        }
        else if (gu32_Process == FALLPROCESS || gu32_Process == T5PROCESS)
        {
            if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)    //有斗 卸料
                fu8p_Array[5] = '6';
        }
    }
    if (gb_OflFlg)
        fu8p_Array[6] = 'O';    //溢出
    else if (gb_StabFlg)
        fu8p_Array[6] = 'S';    //稳定
    else
        fu8p_Array[6] = 'M';    //不稳
    //净重N 毛重G
    fu8p_Array[7] = (gb_DispNetFlg == true) ? 'N' : 'G';
    //正负号
    fu8p_Array[8] = (gb_SigWtFlg == true) ? '-' : '+';
    //7位重量，含小数点，[9~15]中
    //重量转为ASCII码存入fu8p_Array中
    BIN4toNASC_Point(gu32_Wt, &fu8p_Array[9], 6, gs_Cal.Point);
    SRCFrame(fu8p_Array, 16);
}

#ifdef DEBUG_WT_SEND
/*********************************************************************
函数名称: u8 Debug_State1(void)
功    能: 返回测试的状态1
说    明: 
入口参数: 
返 回 值: 
状态1 31H 加料延时
      32H 快加
      33H 中加禁止比较时间
      34H 中加
      35H 慢加禁止比较时间
      36H 慢加
      37H 超欠差检测
      38H 定值
      39H 卸料
      3AH 卸料延时
      30H 其他
      
设    计: 丛海旭               时    间: 2015-07-16
修    改:                      时    间: 
*********************************************************************/
u8 Debug_State1(void)
{
    u8 lu8_Result = '0';
    
    if (gb_RunFlg == false)
        lu8_Result = '0';
    else if (gu32_Process == T1PROCESS)  //加料延时
        lu8_Result = '1';
    else if(gu32_Process == LARGEPROCESS)  //快加
        lu8_Result = '2';
    else if (gu32_Process == MIDDLEPROCESS)
    {
        if (gb_TimeRunFlg)      //中加禁止比较时间
            lu8_Result = '3';
        else
            lu8_Result = '4';   //中加
    }
    else if (gu32_Process == SMALLPROCESS)
    {
    #ifndef DEBUG_SMARTT2B
        if (gb_TimeRunFlg)      //慢加禁止比较时间
            lu8_Result = '5';
    #else
        if (gb_T2bRunFlg)
        {   //智能禁止比较时间判断已经启动
            lu8_Result = '5';
        }
    #endif
        else
            lu8_Result = '6';   //慢加
    }
    else if (gu32_Process == T3PROCESS)    //超欠差检测
        lu8_Result = '7';
    else if (gu32_Process == T4PROCESS)    //定值
        lu8_Result = '8';
    else if (gu32_Process == FALLPROCESS)    //卸料
        lu8_Result = '9';
    else if (gu32_Process == T5PROCESS)    //卸料延时
        lu8_Result = ':';
        
    return lu8_Result;
}

/*********************************************************************
函数名称: u8 Debug_State2(u8* fu8p_State2)
功    能: 返回测试的状态2
说    明: 
入口参数: 
返 回 值: 
状态2 31H 加料延时起始点
      32H 快加起始点
      33H 中加起始点
      34H 中加禁止比较时间结束时刻点
      35H 慢加起始点
      36H 慢加禁止比较时间结束时刻点
      37H 超欠差检测起始点
      38H 定值起始点
      39H 卸料启动点 即本秤结果
      3AH 卸料延时起始点
      30H 其他
      
设    计: 丛海旭               时    间: 2015-07-16
修    改:                      时    间: 
*********************************************************************/
u8 Debug_State2(u8* fu8_State2)
{
    u8 lu8_Result = '0';
    
    if (*fu8_State2 != 0)
    {
        lu8_Result = (*fu8_State2);
        (*fu8_State2) = 0;
    }
    
    return lu8_Result;
}

/*********************************************************************
函数名称: void Debug_Com_Cont_Wt(u8 *fu8p_Array)
功    能: 调试用功能，发送滤波前和滤波后重量
说    明: 
STX 状态1 状态2  +/-未滤波的重量值             +/-滤波后的重量值           CR  LF
02H  30H   30H  2BH 30H 30H 30H 30H 30H 30H  2BH 31H 31H 31H 31H 31H 31H  0DH 0AH

状态1 31H 加料延时
      32H 快加
      33H 中加禁止比较时间
      34H 中加
      35H 慢加禁止比较时间
      36H 慢加
      37H 定值时间
      38H 卸料
      39H 卸料延时
      30H 其他

状态2 31H 加料延时起始点
      32H 快加起始点
      33H 中加起始点
      34H 中加禁止比较时间结束时刻点
      35H 慢加起始点
      36H 慢加禁止比较时间结束时刻点
      37H 超欠差检测起始点
      38H 定值起始点
      39H 卸料启动点 即本秤结果
      3AH 卸料延时起始点
      30H 其他
      
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-4-30
修    改:                      时    间: 
*********************************************************************/
void Debug_Com_Cont_Wt(u8 *fu8p_Array)
{
    u32 lu32_Temp;
    
    fu8p_Array[0] = STARTCHAR;
        
    fu8p_Array[1] = Debug_State1(); //状态1
    fu8p_Array[2] = Debug_State2(&gu8_DebugState2); //状态2
        
    fu8p_Array[3] = (gb_SigWtNotFedFlg ? '-' : '+');
    //重量转为ASCII码存入fu8p_Array中 5位
    BIN4toNASC_Echo0(gu32_WtNotFed, &fu8p_Array[4], 5);
    
    fu8p_Array[9] = (gb_SigWtFlg ? '-' : '+');
    //重量转为ASCII码存入fu8p_Array中
    BIN4toNASC_Echo0(gu32_Wt, &fu8p_Array[10], 5);
    
    //滤波前线性拟合k值 2位小数点
//    fu8p_Array[15] = (gs32_LinearKNotFed < 0 ? '-' : '+');
//    lu32_Temp = (gs32_LinearKNotFed < 0 ? (-gs32_LinearKNotFed) : gs32_LinearKNotFed) / 10;
//    BIN4toNASC_Echo0(lu32_Temp, &fu8p_Array[16], 5);
    
    //滤波后线性拟合k值
    fu8p_Array[21] = (gs32_LinearKFed < 0 ? '-' : '+');
    lu32_Temp = (gs32_LinearKFed < 0 ? (-gs32_LinearKFed) : gs32_LinearKFed) / 10;
    BIN4toNASC_Echo0(lu32_Temp, &fu8p_Array[22], 5);
    
    fu8p_Array[27] = ENDCHAR1;
    fu8p_Array[28] = ENDCHAR2;
}

//发送滤波前后重量及卡尔曼滤波重量（两位小数点）
void Debug_Com_Cont_Wt_Kalman(u8 *fu8p_Array)
{
    u32 lu32_Temp;
        
    fu8p_Array[0] = Debug_State1(); //状态1
    fu8p_Array[1] = 9; //TAB
    fu8p_Array[2] = Debug_State2(&gu8_DebugState2); //状态2
    fu8p_Array[3] = 9; //TAB
        
    fu8p_Array[4] = (gb_SigWtNotFedFlg ? '-' : '+');
    //重量转为ASCII码存入fu8p_Array中 5位
    BIN4toNASC_Echo0(gu32_WtNotFed, &fu8p_Array[5], 5);
    fu8p_Array[10] = 9; //TAB
    fu8p_Array[11] = (gb_SigWtFlg ? '-' : '+');
    //重量转为ASCII码存入fu8p_Array中
    BIN4toNASC_Echo0(gu32_Wt, &fu8p_Array[12], 5);
    fu8p_Array[17] = 9; //TAB
    //卡尔曼滤波重量 1小数
    fu8p_Array[18] = (gf_WtKalmanFed < 0 ? '-' : '+');
    lu32_Temp = (gf_WtKalmanFed < 0 ? ((-gf_WtKalmanFed) * 10 + 0.5) : gf_WtKalmanFed * 10 + 0.5);
    BIN4toNASC_Point(lu32_Temp, &fu8p_Array[19], 6, 1);
    fu8p_Array[26] = 9; //TAB
    //滤波后线性拟合k值
    fu8p_Array[27] = (gs32_LinearKFed < 0 ? '-' : '+');
    lu32_Temp = (gs32_LinearKFed < 0 ? (-gs32_LinearKFed) : gs32_LinearKFed);
    BIN4toNASC_Echo0(lu32_Temp, &fu8p_Array[28], 6);
    fu8p_Array[34] = 9; //TAB
    //包数
    BIN4toNASC_Echo0(gu32_DebugSumNum, &fu8p_Array[35], 3);
    
    fu8p_Array[38] = ENDCHAR1;
    fu8p_Array[39] = ENDCHAR2;
    Debug_Send(fu8p_Array, 40);
}

/*********************************************************************
函数名称: void Debug_Com_Cont_AD(u8 *fu8p_Array)
功    能: 调试用功能，发送滤波前和滤波后AD码
说    明: 
STX 状态1 状态2  +/-未滤波的重量值             +/-滤波后的重量值           CR  LF
02H  30H   30H  2BH 30H 30H 30H 30H 30H 30H  2BH 31H 31H 31H 31H 31H 31H  0DH 0AH

状态1 31H 加料延时
      32H 快加
      33H 中加禁止比较时间
      34H 中加
      35H 慢加禁止比较时间
      36H 慢加
      37H 定值时间
      38H 卸料
      39H 卸料延时
      30H 其他

状态2 31H 加料延时起始点
      32H 快加起始点
      33H 中加起始点
      34H 中加禁止比较时间结束时刻点
      35H 慢加起始点
      36H 慢加禁止比较时间结束时刻点
      37H 超欠差检测起始点
      38H 定值起始点
      39H 卸料启动点 即本秤结果
      3AH 卸料延时起始点
      30H 其他
      
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-5-13
修    改:                      时    间: 
*********************************************************************/
void Debug_Com_Cont_AD(u8 *fu8p_Array)
{
    fu8p_Array[0] = STARTCHAR;
    fu8p_Array[1] = Debug_State1(); //状态1
    fu8p_Array[2] = Debug_State2(&gu8_DebugState2); //状态2
        
    fu8p_Array[3] = '+';
    //重量转为ASCII码存入fu8p_Array中
    BIN4toNASC_Echo0(gu32_AdValue, &fu8p_Array[4], 6);
    
    fu8p_Array[10] = '+';
    //重量转为ASCII码存入fu8p_Array中
    BIN4toNASC_Echo0(gu32_AdFedValue, &fu8p_Array[11], 6);
    
    fu8p_Array[17]=ENDCHAR1;
    fu8p_Array[18]=ENDCHAR2;
}

#endif


#ifdef DEBUG_WT_STATE_SEND
/*********************************************************************
函数名称: void Debug_Com_Cont_Wt_State(u8 *fu8p_Array)
功    能: 调试用功能，发送滤波前和滤波后重量
说    明: 
STX  时标    状态   +/-重量值                    CR  LF
02H (30~39H)  30H   2BH 30H 30H 30H 30H 30H 30H  0DH 0AH
 
时标是从30~39H循环

状态 31H 快加
     32H 中加
     33H 慢加
     34H 定值
     35H 卸料
     30H 其他

重量值 高位0为空格20H
     
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-6-26
修    改:                      时    间: 
*********************************************************************/
void Debug_Com_Cont_Wt_State(u8 *fu8p_Array)
{
    static u8 i = 0;
    
    fu8p_Array[0] = STARTCHAR;
    
    fu8p_Array[1] = 0x30 + i;
    i++;
    if (i > 9)
        i = 0;
    
    if (gu32_Process == LARGEPROCESS)  //快加
        fu8p_Array[2] = '1';
    else if (gu32_Process == MIDDLEPROCESS) //中加
        fu8p_Array[2] = '2';
    else if (gu32_Process == SMALLPROCESS)  //慢加
        fu8p_Array[2] = '3';
    else if (gu32_Process == T3PROCESS)
         fu8p_Array[2] = '4';
    else if (gu32_Process == T4PROCESS)    //定值
    {
        if (gu32_Sys10ms - gu32_Time1_5 < gu32_TimeBuf)
            fu8p_Array[2] = '4';
        else
            fu8p_Array[2] = '0';
    }
    else if (gu32_Process == FALLPROCESS || gu32_Process == T5PROCESS)     //卸料
        fu8p_Array[2] = '5';
    else
        fu8p_Array[2] = '0';
        
        
    fu8p_Array[3] = (gb_SigWtFlg ? '-' : '+');
    //重量转为ASCII码存入fu8p_Array中
    BIN4toNASC_Echo0(gu32_Wt, &fu8p_Array[4], 6);
    
    fu8p_Array[10] = ENDCHAR1;
    fu8p_Array[11] = ENDCHAR2;
}
#endif

/*********************************************************************
函数名称: void Com_Cont_o(u8 *fu8p_Array, u8 fu8_Uart)
功    能: cont_o模式通讯处理
说    明: 为兼容旧版本04CD程序，累计次数4位，累计重量9位（包含1位小数点）
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void Read_RT(u8 *fu8p_Array)
{
    u8 i;
    
    //秤号
    i = (fu8p_Array == gu8_TxdArray_UART0) ? gs_Setup.TareNumber_2 : gs_Setup.TareNumber_1;
    fu8p_Array[0] = STARTCHAR;
    fu8p_Array[1] = 0x30 + i / 10;
    fu8p_Array[2] = 0x30 + i % 10;       //秤号
    fu8p_Array[3] = 'R';
    fu8p_Array[4] = 'T';
    //累计次数转为ASCII码存入fpu8_TxdArry中，9位，存入[5~13]中
    BIN4toNASC(gu32_SumNum, &fu8p_Array[5], 9);
    for (i = 5; i < 13; i++)        //消隐
    {
        if (fu8p_Array[i] == 0x30)
            fu8p_Array[i] = ' ';
        else
            break;
    }
    fu8p_Array[14] = ',';
    fu8p_Array[15] = 0x20;
    //累计重量转为ASCII码存入gu8_SioSendArry_Pnt中，10位包含一位小数点，消隐后存入[15~24]中
    BIN4toNASC(gu32_SumWt, &fu8p_Array[16], 9);
    if (gs_Cal.Point != 0)          //小数点处理
    {
        for (i = 0; i < 9 - gs_Cal.Point; i++)
            fu8p_Array[15+i] = fu8p_Array[16+i];
        fu8p_Array[15+i] = '.';
    }
    for (i = 15; i < 24; i++)        //消隐
    {
        if (fu8p_Array[i] == 0x30 && fu8p_Array[i+1] != '.')
            fu8p_Array[i] = ' ';
        else
            break;
    }
    SRCFrame(fu8p_Array, 25);
}

/*********************************************************************
函数名称: void Com_Cont_o(u8 *fu8p_Array, u8 fu8_Uart)
功    能: cont_o模式通讯处理
说    明: 为兼容旧版本04CD程序，累计次数4位，累计重量9位（包含1位小数点）
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void Read_RT_O(u8 *fu8p_Array)
{
    u8 i;
    
    //秤号
    i = (fu8p_Array == gu8_TxdArray_UART0) ? gs_Setup.TareNumber_2 : gs_Setup.TareNumber_1;
    fu8p_Array[0] = STARTCHAR;
    fu8p_Array[1] = 0x30 + i / 10;
    fu8p_Array[2] = 0x30 + i % 10;       //秤号
    fu8p_Array[3] = 'R';
    fu8p_Array[4] = 'T';
    //累计次数转为ASCII码存入fpu8_TxdArry中，4位，存入[5~8]中
    BIN4toNASC_Echo0(gu32_SumNum, &fu8p_Array[5], 4);
    fu8p_Array[9] = ',';
    //累计重量转为ASCII码存入gu8_SioSendArry_Pnt中，9位包含一位小数点，消隐后存入[10~18]中
    BIN4toNASC_Point(gu32_SumWt, &fu8p_Array[10], 8, gu32_SumWtPoint);   //包装重量9字节
    SRCFrame(fu8p_Array, 19);
}

/*********************************************************************
函数名称: void Commumication(void)
功    能: 通讯处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void Commumication(void)
{
#ifndef DEBUG_WT_SEND
    static u32 su32_TimeTemp_1 = 0;
    static u32 su32_TimeTemp_2 = 0;
#endif

    if (bUARTSending_UART0 == false)    //连续通讯模式下数据发送
    {
        Debug_Com_Cont_Wt_Kalman(gu8_TxdArray_UART0);   //发送卡尔曼滤波重量
    }
    else
    {   //不能保证每次转换的数据都能被发送出去，会响蜂鸣器提示
        BuzzerStart(5);
    }
    PrintDeal(gu8_TxdArray_UART1);    //打印相关处理
    
//    if (gb_DebugUart1 == false)
//    {
//        switch (gs_Setup.ComMode_1)
//        {
//            case COM_READ:
//            case COM_READ_O:
//                Com_Read(gu8_TxdArray_UART1);
//                break;
//            case COM_CONT:
//            #ifdef DEBUG_WT_SEND    //调试用，串口发送重量数据
//                if (gvb_SioTxingFlg_UART1 == false)    //连续通讯模式下数据发送
//                {
//                    Debug_Com_Cont_AD(gu8_TxdArray_UART1);
//                    StartSend_Com(gu8_TxdArray_UART1);
//                }
//                else
//                {   //不能保证每次转换的数据都能被发送出去，会响蜂鸣器提示
//                    BuzzerStart(5);
//                }
//            #else
//                #ifdef DEBUG_WT_STATE_SEND
//                    if (gvb_SioTxingFlg_UART1 == false)    //连续通讯模式下数据发送
//                    {
//                        Debug_Com_Cont_Wt_State(gu8_TxdArray_UART1);
//                        StartSend_Com(gu8_TxdArray_UART1);
//                    }
//                    else
//                    {   //不能保证每次转换的数据都能被发送出去，会响蜂鸣器提示
//                        BuzzerStart(5);
//                    }
//                #else   //正常运行
//                    if (gu32_Sys10ms - su32_TimeTemp_1 > 1)        //发送间隔大于10ms v1.01
//                    {
//                        if (gvb_SioTxingFlg_UART1 == false)    //连续通讯模式下数据发送
//                        {   //串口未在发送数据才发送
//                            Com_Cont(gu8_TxdArray_UART1);
//                            StartSend_Com(gu8_TxdArray_UART1);
//                            su32_TimeTemp_1 = gu32_Sys10ms;
//                        }
//                    }
//                #endif
//            #endif
//                break;
//            case COM_BUS_R:
//            case COM_BUS_A:
//                if (bFinishReceive_UART1 || gvb_EndCharFlg_UART1)
//                {
//                    Com_Modbus(gu8_TxdArray_UART1);
//                    memset(gu8_DebugRxdArray_UART1, 0, MAX(RXDLEN_BUS_R,RXDLEN_BUS_A));
//                    bFinishReceive_UART1 = false;
//                }
//                break;
//            case COM_PRINT:
//                PrintDeal(gu8_TxdArray_UART1);    //打印相关处理
//                break;
//            case COM_PBUS:
//                Com_Pbus_DP(); //PROFIBUS处理
//                break;
//        }
//    }
//    if (gb_DebugUart0 == false)
//    {
//        switch (gs_Setup.ComMode_2)
//        {
//            case COM_READ:
//            case COM_READ_O:
//                Com_Read(gu8_TxdArray_UART0);
//                break;
//            case COM_CONT:
//            #ifdef DEBUG_WT_SEND    //调试用，串口发送重量数据
//                if (bUARTSending_UART0 == false)    //连续通讯模式下数据发送
//                {
//                    Debug_Com_Cont_Wt_Kalman(gu8_TxdArray_UART0);   //发送卡尔曼滤波重量
////                    Debug_Com_Cont_Wt(gu8_TxdArray_UART0);
////                    StartSend_Com(gu8_TxdArray_UART0);
//                }
//                else
//                {   //不能保证每次转换的数据都能被发送出去，会响蜂鸣器提示
//                    BuzzerStart(5);
//                }
//            #else
//                #ifdef DEBUG_WT_STATE_SEND
//                    if (gvb_SioTxingFlg_UART0 == false)    //连续通讯模式下数据发送
//                    {
//                        Debug_Com_Cont_Wt_State(gu8_TxdArray_UART0);
//                        StartSend_Com(gu8_TxdArray_UART0);
//                    }
//                    else
//                    {   //不能保证每次转换的数据都能被发送出去，会响蜂鸣器提示
//                        BuzzerStart(5);
//                    }
//                #else   //正常运行
//                    if (gu32_Sys10ms - su32_TimeTemp_2 > 1)        //发送间隔大于10ms v1.01
//                    {
//                        if (gvb_SioTxingFlg_UART0 == false)    //连续通讯模式下数据发送
//                        {   //串口未在发送数据才发送
//                            Com_Cont(gu8_TxdArray_UART0);
//                            StartSend_Com(gu8_TxdArray_UART0);
//                            su32_TimeTemp_2 = gu32_Sys10ms;
//                        }
//                    }
//                #endif
//            #endif
//                break;
//            case COM_BUS_R:
//            case COM_BUS_A:
//                if (bFinishReceive_UART0 || gvb_EndCharFlg_UART0)
//                {
//                    Com_Modbus(gu8_TxdArray_UART0);
//                    memset(gu8_DebugRxdArray_UART0, 0, MAX(RXDLEN_BUS_R,RXDLEN_BUS_A));
//                    bFinishReceive_UART0 = false;
//                }
//                break;
//            case COM_PRINT:
//                PrintDeal(gu8_TxdArray_UART0);    //打印相关处理
//                break;
//        }
//    }
}

/*********************************************************************
函数名称: void RxdArry_To_TxdArry(u8 fu8_Uart, u8 fu8_Num)
功    能: 接收数据数据存入发送数据
说    明: 
入口参数: fu8_Num = 复制数据的个数
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void RxdArry_To_TxdArry(u8 fu8_Uart, u8 fu8_Num)
{
    if (fu8_Uart == 0)
        memcpy(gu8_TxdArray_UART0, gu8_RxdArray_UART0, fu8_Num);
    else
        memcpy(gu8_TxdArray_UART1, gu8_RxdArray_UART1, fu8_Num);
}

/*********************************************************************
函数名称: bool RxData_Check(u8 *fu8p_Array)
功    能: 串口接收数据检查和校验
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
bool RxData_Check(u8 *fu8p_Array)
{
    u32 lu32_Temp;
    
    lu32_Temp = (fu8p_Array[1] - 0x30) * 10 + (fu8p_Array[2] - 0x30);     //得到秤号
    
    if (lu32_Temp != ((fu8p_Array == gu8_TxdArray_UART0) ? gs_Setup.TareNumber_2 : gs_Setup.TareNumber_1))
        return false;
    if (fu8p_Array[3] == 'R' && (fu8p_Array[4] == 'S' || fu8p_Array[4] == 'T'))
    {
        if (SRCCheck(fu8p_Array, 5))
            return true;
        else
        {
            Read_CE(fu8p_Array);      //校验错误
            StartSend_Com(fu8p_Array);
            return false;
        }
    }
    else
    {
        Read_CE(fu8p_Array);      //非法命令
        StartSend_Com(fu8p_Array);
        return false;
    }
}

/*********************************************************************
函数名称: bool JudgeFrame(u8 *fu8p_Array, u8 i, u8 j, u8 k)
功    能: 判断接收帧的操作对象
说    明: 操作对象由3字节组成，从fu8p_Arry[4]开始
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
bool JudgeFrame(u8 *fu8p_Array, u8 i, u8 j, u8 k)
{
    if (fu8p_Array[4] == i && fu8p_Array[5] == j && fu8p_Array[6] == k)
        return true;
    else
        return false;
}

/*********************************************************************
函数名称: void Read_CE(u8 *fu8p_Array)
功    能: read协议返回CE，接收到数据不在列表内或校验和错误
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void Read_CE(u8 *fu8p_Array)
{
    fu8p_Array[3] = 'C';
    fu8p_Array[4] = 'E';
    SRCFrame(fu8p_Array, 5);
}

/*********************************************************************
函数名称: void Deal_Read_R(u8 *fu8p_Array)
功    能: 功能码R的命令处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-8-19
修    改:                      时    间: 
*********************************************************************/
void Deal_Read_R(u8 *fu8p_Array)
{
    if (fu8p_Array[4] == 'S')        //读状态
        Com_Cont(fu8p_Array);
    else if (fu8p_Array[4] == 'T')        //读累计
    {
        //如果是旧协议累计次数4位 累计重量8位+1位小数点
        if ((fu8p_Array == gu8_TxdArray_UART0 && gs_Setup.ComMode_2 == COM_READ_O) || 
            (fu8p_Array == gu8_TxdArray_UART1 && gs_Setup.ComMode_1 == COM_READ_O))
            Read_RT_O(fu8p_Array);
        else
            Read_RT(fu8p_Array);
    }
//    else if (JudgeFrame(fu8p_Array, 'R', 'I', 'D'))       //配方号
//        Read_RRID(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'A', 'L', 'A'))  //A报警信息
//        Read_RALA(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'A', 'L', 'B'))  //B报警信息
//        Read_RALB(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'R', 'P', 'C'))  //批次数
//        Read_RRPC(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'S', 'P', 'C'))  //剩余批次数
//        Read_RSPC(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'O', 'T', 'R'))  //发货总量
//        Read_ROTR(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'W', 'M', 'D'))  //工作模式
//        Read_RWMD(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'F', 'L', 'U'))  //工作模式
//        Read_RFLU(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'T', ' ', ' '))  //累计
//        Com_Cont(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'S', 'A', ' '))  //A状态
//        Read_RSA_(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'S', 'B', ' '))  //B状态
//        Read_RSB_(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'P', 'T'))  //小数点
//        Read_RCPT(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'U', 'N'))  //单位
//        Read_RCUN(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'D', 'D'))  //分度值
//        Read_RCDD(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'A', 'P'))  //最大量程
//        Read_RCAP(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'P', 'S'))  //标定密码
//        Read_RCPS(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'V', 'A'))  //A绝对毫伏数
//        Read_RCVA(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'V', 'B'))  //B绝对毫伏数
//        Read_RCVB(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'Z', 'A'))  //A零点毫伏数
//        Read_RCZA(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'Z', 'B'))  //B零点毫伏数
//        Read_RCZB(fu8p_Array);
//    else if (fu8p_Array[4] == 'B')                     //工作参数
//        Read_RBXX(fu8p_Array);
//    else if (fu8p_Array[4] == 'F')                     //配方参数
//        Read_RFXX(fu8p_Array);
//    else if (fu8p_Array[4] == 'T')                     //配方累计
//        Read_RTXX(fu8p_Array);
    else
        Read_CE(fu8p_Array);
}

/*********************************************************************
函数名称: void Deal_Read(u8 *fu8p_Array)
功    能: 构造read方式下的返回数据
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void Deal_Read(u8 *fu8p_Array)
{
    switch (fu8p_Array[3])
    {
        case 'R':
            Deal_Read_R(fu8p_Array);
            break;
//        case 'W':
//            Deal_Read_W(fu8p_Array);
//            break;
//        case 'C':
//            Deal_Read_C(fu8p_Array);
//            break;
//        case 'D':
//            Deal_Read_D(fu8p_Array);
//            break;
        default:
            Read_CE(fu8p_Array);
            break;
    }
    StartSend_Com(fu8p_Array);
}

/*********************************************************************
函数名称: void Com_Read(u8 *fu8p_Array)
功    能: read模式通讯处理
说    明: 
入口参数: fu8p_Array发送数组首地址
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-21
修    改:                      时    间: 
*********************************************************************/
void Com_Read(u8 *fu8p_Array)
{
    if (fu8p_Array == gu8_TxdArray_UART0 && gvb_EndCharFlg_UART0)            //串口0接收到完整的数据
    {
        if (gvb_SioTxingFlg_UART0 == false)    //串口未发送数据
        {
            RxdArry_To_TxdArry(0, RXDLEN_READ);
            if (RxData_Check(gu8_TxdArray_UART0))             //接收数据的校验
                Deal_Read(gu8_TxdArray_UART0);            //构造read方式返回数据
            else
                gvb_EndCharFlg_UART0 = false;    //可以接收下一帧数据
        }
    }
    if (fu8p_Array == gu8_TxdArray_UART1 && gvb_EndCharFlg_UART1)            //串口1接收到完整的数据
    {
        if (gvb_SioTxingFlg_UART1 == false)    //串口未发送数据
        {
            RxdArry_To_TxdArry(1, RXDLEN_READ);
            if (RxData_Check(gu8_TxdArray_UART1))             //接收数据的校验
                Deal_Read(gu8_TxdArray_UART1);            //构造read方式返回数据
            else
                gvb_EndCharFlg_UART1 = false;    //可以接收下一帧数据
       }
    }
}


/***********************************************************************
函 数 名: void DebugInit_UART0(UartSet_Typedef *pUartSet)
功    能：
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：丛海旭               日    期：
修    改：                     日    期：
***********************************************************************/
void DebugInit_UART0(void)
{
    UART_Init(UART0);
    Sio_485_Enable();
    
    //启动的时候，发送队列有效个数为0，首指针都指向数组的头部。
    ulTBUFAvailNum_UART0=0;
    ulTBUFBegin_UART0=0;
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
    
    //设置中断的优先级，并且使能UART0中断
    EIC_IRQChannelPriorityConfig(UART0_IRQChannel,UART0_SIPL);
    EIC_IRQChannelConfig(UART0_IRQChannel,ENABLE);//
}

/*************************************************************
函 数 名: bool Debug_Send(u8 *fu8p_Data, u32 fu32_Num)
功    能：发送函数
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：丛海旭               日    期：2014-1-22
修    改：                     日    期：
*************************************************************/
bool Debug_Send(u8 *fu8p_Data, u32 fu32_Num)
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
        
        bUARTSending_UART0=true;          //串口正在发送之中
        
        //注意一个问题，千万不要在bUARTSending_UART0置位成true之前开中断
        //否则，中断一开，然后马上进入半空和空中断，bUARTSending_UART0
        //没有置上，就马上被中断置位False了，相当于没有开。
        //同理对ulTBUFAvailNum_UART0，ulTBUFBegin_UART0也一样。
        
        EnableSendIRQ_UART0();      //打开发送中断
        
        return(true);
    }
    return false;
}


/***********************************************************************
函 数 名: void IRQ_Debug_UART0(void)
功    能：串口0中断服务程序
说    明：
入口参数：
调用资源：
返 回 值：无
设    计：                     日    期：
修    改：                     日    期：
***********************************************************************/
void IRQ_Debug_UART0(void)
{
    u32 i;
    u16 UARTStatus;
    
    //读取状态寄存器
    UARTStatus=UART_FlagStatus(UART0);
    
    //缓冲区半空和缓冲区全空引起的中断
    if (true==bUARTSending_UART0)
    {
        //因为全空引起的中断
        if (UARTStatus&UART_TxEmpty)
        {
            if (ulTBUFAvailNum_UART0==0)
            {   //发送完毕
                bUARTSending_UART0=false; //串口没有发送操作
                DisableSendIRQ_UART0();
                EnableRecIRQ_UART0();
                Sio_485_Enable();
                gb_S485Recieve = true;
            }
            else
            {   //第一次发送
                if (ulTBUFAvailNum_UART0>0)
                {
                    if (ulTBUFAvailNum_UART0>=16)
                    {
                        for(i=0;i<16;i++)
                        {
                            UART0->TxBUFR=gu8_TxdArray_UART0[ulTBUFBegin_UART0];
                            ulTBUFBegin_UART0=(ulTBUFBegin_UART0+1)%TXDLEN_BUS_R;
                        }
                        ulTBUFAvailNum_UART0-=16;
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
            }
            UARTStatus=UART_FlagStatus(UART0);
        }
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
        
    }
    else
    {
        DisableSendIRQ_UART0();
        Sio_485_Enable();
    }
}

/*********************************************************************
函数名称: void IRQ_GM_ModbusASCII_UART0(void)
功    能: READ CONT PRIINT MODBUS_ASCII串口通讯的中断处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-4-3
修    改:                      时    间: 
*********************************************************************/
void IRQ_GM_ModbusASCII_UART0(void)
{
    u8 lu8_TxdData;
    u8 lu8_RxdData;
    u8 lu8_MaxLen;
    static bool sb_StartCharFlg_UART0 = false;
    switch (gs_Setup.ComMode_2)
    {
        case COM_BUS_A:
            lu8_MaxLen = TXDLEN_BUS_A;
            break;
        case COM_READ:
        case COM_READ_O:
            lu8_MaxLen = TXDLEN_READ_CONT;
            break;
        case COM_PRINT:
        default:
            lu8_MaxLen = TXDLEN_PRINT;
            break;
    }
    //关超时中断，防止正在通讯Modbus rtu时切换通讯方式引起的死机 v1.01
    if (UART_FlagStatus(UART0)&UART_TimeOutIdle)
        UART_ItConfig(UART0,UART_TimeOutIdle,DISABLE);
    //发送中断
    if(UART_FlagStatus(UART0) & (UART_TxEmpty|UART_TxHalfEmpty))
    {
        if(!(gb_SioConfigedFlg_UART0&&gvb_SioTxingFlg_UART0))
        {
            UART_ItConfig(UART0,UART_TxEmpty,DISABLE);
            Sio_485_Enable();
            gb_S485Recieve = true;  //修正RS485总线Modbus ASCII方式通讯时偶尔出现中断的问题。 v1.01
        }
        else
        {
            lu8_TxdData=gu8_TxdArray_UART0[gvu8_TxdCnt_UART0++];
            UART_ByteSend(UART0,&lu8_TxdData);

            if(lu8_TxdData == ENDCHAR2)
            {
                gvb_SioTxingFlg_UART0=false;
                if (gvu8_PrintCnt_UART0 > 0) //打印中使用，打印台头等
                    gvu8_PrintCnt_UART0--;
                gvb_PrintingFlg_UART0 = false;
            }
            if (gvu8_TxdCnt_UART0 > lu8_MaxLen)
            {
                gvb_SioTxingFlg_UART0=false;
                if (gvu8_PrintCnt_UART0 > 0) //打印中使用，打印台头等
                    gvu8_PrintCnt_UART0--;
                gvb_PrintingFlg_UART0 = false;
            }
        }
    }
    //接收中断
    if(UART_FlagStatus(UART0) & (UART_RxBufNotEmpty))
    {
        lu8_RxdData= (u8)(UART0->RxBUFR);
        UART_FifoReset(UART0, UART_RxFIFO);

        if(!gb_SioConfigedFlg_UART0)
            return;

        switch(lu8_RxdData)
        {
            case STARTCHAR: //Read方式起始符
            case MB_ASCII_STARTCHAR:    //Modbus_ASCII起始符
                if(gvb_EndCharFlg_UART0==true)     //还有数据未处理完成
                {
                    sb_StartCharFlg_UART0=false;
                }
                else
                {
                    sb_StartCharFlg_UART0=true;
                    gu8_RxdArray_UART0[0]=lu8_RxdData;
                    gvu8_RxdCnt_UART0=1;
                }
                break;
            case ENDCHAR2:
                if(sb_StartCharFlg_UART0)
                {
                    sb_StartCharFlg_UART0=false;
                    gvb_EndCharFlg_UART0=true;
                    if(gvu8_RxdCnt_UART0 < ((gs_Setup.ComMode_2 == COM_BUS_A) ? RXDLEN_BUS_A : RXDLEN_READ))
                        gu8_RxdArray_UART0[gvu8_RxdCnt_UART0]=lu8_RxdData;
                }
                break;

            default:
                if((sb_StartCharFlg_UART0) && (!gvb_EndCharFlg_UART0))
                {
                    if(gvu8_RxdCnt_UART0> ((gs_Setup.ComMode_2 == COM_BUS_A) ? RXDLEN_BUS_A : RXDLEN_READ))
                    {
                        sb_StartCharFlg_UART0=false;
                        gvu8_RxdCnt_UART0=0;
                    }
                    else
                    {
                        gu8_RxdArray_UART0[gvu8_RxdCnt_UART0++]=lu8_RxdData;
                    }
                }
                break;
        }
    }
}

/*********************************************************************
函数名称: void IRQ_GM_ModbusASCII_UART1(void)
功    能: READ CONT PRIINT MODBUS_ASCII串口通讯的中断处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-4-3
修    改:                      时    间: 
*********************************************************************/
void IRQ_GM_ModbusASCII_UART1(void)
{
    u8 lu8_RxdData;
    u8 lu8_TxdData;
    u8 lu8_MaxLen;
    static bool sb_StartCharFlg_UART1 = false;
    
    switch (gs_Setup.ComMode_1)
    {
        case COM_BUS_A:
            lu8_MaxLen = TXDLEN_BUS_A;
            break;
        case COM_READ:
        case COM_READ_O:
            lu8_MaxLen = TXDLEN_READ_CONT;
            break;
        case COM_PRINT:
        default:
            lu8_MaxLen = TXDLEN_PRINT;
            break;
    }
    
    //关超时中断，防止正在通讯Modbus rtu时切换通讯方式引起的死机 v1.01
    if (UART_FlagStatus(UART1)&UART_TimeOutIdle)
        UART_ItConfig(UART1,UART_TimeOutIdle,DISABLE);
        
    //发送中断
    if(UART_FlagStatus(UART1) & (UART_TxEmpty|UART_TxHalfEmpty))
    {
        if(!(gb_SioConfigedFlg_UART1&&gvb_SioTxingFlg_UART1))
        {
            UART_ItConfig(UART1,UART_TxEmpty,DISABLE);
        }
        else
        {
            lu8_TxdData=gu8_TxdArray_UART1[gvu8_TxdCnt_UART1++];
            UART_ByteSend(UART1,&lu8_TxdData);

            if(lu8_TxdData==ENDCHAR2)
            {
                gvb_SioTxingFlg_UART1=false;
                if (gvu8_PrintCnt_UART1 > 0) //打印中使用，打印台头等
                    gvu8_PrintCnt_UART1--;
                gvb_PrintingFlg_UART1 = false;
            }
            if (gvu8_TxdCnt_UART1 > lu8_MaxLen)
            {
                gvb_SioTxingFlg_UART1=false;
                if (gvu8_PrintCnt_UART1 > 0) //打印中使用，打印台头等
                    gvu8_PrintCnt_UART1--;
                gvb_PrintingFlg_UART1 = false;
            }
        }
    }
    //接收中断
    if(UART_FlagStatus(UART1) & (UART_RxBufNotEmpty))
    {
        lu8_RxdData= (u8)(UART1->RxBUFR);
        UART_FifoReset(UART1, UART_RxFIFO);

        if(!gb_SioConfigedFlg_UART1)
            return;

        switch(lu8_RxdData)
        {
            case STARTCHAR: //Read方式起始符
            case MB_ASCII_STARTCHAR:    //Modbus_ASCII起始符
                if(gvb_EndCharFlg_UART1==true)     //还有数据未处理完成
                {
                    sb_StartCharFlg_UART1=false;
                }
                else
                {
                    sb_StartCharFlg_UART1=true;
                    gu8_RxdArray_UART1[0]=lu8_RxdData;
                    gvu8_RxdCnt_UART1=1;
                }
                break;
            case ENDCHAR2:
                if(sb_StartCharFlg_UART1)
                {
                    sb_StartCharFlg_UART1=false;
                    gvb_EndCharFlg_UART1=true;
                    if(gvu8_RxdCnt_UART1 < ((gs_Setup.ComMode_1 == COM_BUS_A) ? RXDLEN_BUS_A : RXDLEN_READ))
                        gu8_RxdArray_UART1[gvu8_RxdCnt_UART1]=lu8_RxdData;
                }
                break;

            default:
                if((sb_StartCharFlg_UART1) && (!gvb_EndCharFlg_UART1))
                {
                    if(gvu8_RxdCnt_UART1 > ((gs_Setup.ComMode_1 == COM_BUS_A) ? RXDLEN_BUS_A : RXDLEN_READ))
                    {
                        sb_StartCharFlg_UART1=false;
                        gvu8_RxdCnt_UART1=0;
                    }
                    else
                    {
                        gu8_RxdArray_UART1[gvu8_RxdCnt_UART1++]=lu8_RxdData;
                    }
                }
                break;
        }
    }
}

