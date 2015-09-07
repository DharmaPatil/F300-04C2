/*
Description:            ����ͨѶ��ع���
Author:                 �Ժ���
Date:                   2013-12-9
*/

#include "include.h"
#include "MathLogic.h"


//����1����ز���
u8  gu8_RxdArray_UART0[RXDLEN];        //��������
u8  gu8_TxdArray_UART0[TXDLEN];        //��������
volatile u8  gvu8_TxdCnt_UART0;                 //���ͼ���
volatile u8  gvu8_RxdCnt_UART0;                 //���ռ���
volatile bool  gvb_EndCharFlg_UART0;            //���յ���������־
bool  gb_SioConfigedFlg_UART0;        //�����ѳ�ʼ��
volatile bool  gvb_SioTxingFlg_UART0;           //�������ڷ���
volatile bool  gvb_SioRxingFlg_UART0;           //�������ڽ���

//����2����ز���
u8  gu8_RxdArray_UART1[RXDLEN];        //��������
u8  gu8_TxdArray_UART1[TXDLEN];        //��������
volatile u8  gvu8_TxdCnt_UART1;                 //���ͼ���
volatile u8  gvu8_RxdCnt_UART1;                 //���ռ���
volatile bool  gvb_EndCharFlg_UART1;            //���յ���������־
bool  gb_SioConfigedFlg_UART1;        //�����ѳ�ʼ��
volatile bool  gvb_SioTxingFlg_UART1;           //�������ڷ���
volatile bool  gvb_SioRxingFlg_UART1;           //�������ڽ���


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
    u8 gu8_DebugState2 = 0;     //���Թ��� ״̬2����
#endif


bool SRCCheck(u8* u8Arry,u8 u8SRCIndex);
void SRCFrame(u8* u8Arry,u8 u8SRCIndex);
void Deal_Read(u8 *fu8p_Array);
void Com_Read(u8 *fu8p_Array);


//�Ӳ����ʻ�ò����ʼĴ���ֵ
#define getBBR(Baudrate)    (u16)(11059200/2*16/2/16/(Baudrate))


/*********************************************************************
��������: void Uart0_Init(void)
��    ��: ����0��ʼ��
˵    ��: ͨѶ����
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void Uart0_Init(void)
{
    //UART0 RS232��RS485��ѡ
    GPIO_Config(GPIO0,(1<<9),GPIO_AF_PP);       //U0.TXD
    GPIO_BitWrite(GPIO0,9,1);
    GPIO_Config(GPIO0,(1<<8),GPIO_IN_TRI_TTL);  //U0.RXD
    GPIO_BitWrite(GPIO0,8,1);
    GPIO_Config(GPIO1,(1<<1),GPIO_OUT_PP);      //U0.S485
    
    DebugInit_UART0();
//    if (gs_Setup.ComMode_2 == COM_BUS_R)          //Modbus-rtu��ʼ��
//        UartModbusInit_UART0();
//    else
//        UartGMInit_UART0();         //GMЭ��Ĵ��ڳ�ʼ����Modbus-ascII��ʼ��
}


/*********************************************************************
��������: void UartGMInit_UART0(void)
��    ��: ����0��ʼ��
˵    ��: ͨѶ����
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void UartGMInit_UART0(void)
{
    gb_SioConfigedFlg_UART0=false;
    Sio_485_Enable();
    UART_Init(UART0);               //��λ
    UART_GuardTimeConfig(UART0,0); //����Txempty��λ�ȴ���ʱ
    UART_FifoConfig(UART0,DISABLE);//�ر�FIFO
    UART_LoopBackConfig(UART0,DISABLE);//�رջ�д

    //���ݴ���������ó�ʼ������ ������ У�鷽ʽ
    UART_Config(UART0,gu32_BaudRateArry[gs_Setup.BaudRate_2],g_ParityArry[gs_Setup.ParityMode_2],g_StopBitsArry[gs_Setup.ParityMode_2],g_DataLenArry[gs_Setup.ParityMode_2]);
    if (gs_Setup.ComMode_2 == COM_CONT || gs_Setup.ComMode_2 == COM_PRINT)
        UART_RxConfig(UART0,DISABLE);//CONT��ʽ��ʹ�ܽ���
    else
        UART_RxConfig(UART0,ENABLE);//ʹ�ܽ���
    UART_FifoReset(UART0, UART_RxFIFO);
    UART_ItConfig(UART0,UART_RxBufNotEmpty,ENABLE);//ʹ��"����" �ж�,�����ж�Ӧ�õȵ���Ҫ���͵�ʱ��������
    UART_OnOffConfig(UART0,ENABLE);//ʹ�ܴ���

    EIC_IRQChannelPriorityConfig(UART0_IRQChannel, UART0_SIPL); //�ж����ȼ�
    EIC_IRQChannelConfig(UART0_IRQChannel, ENABLE); //����IRQ0�ж�����ͨ��

    gvb_SioTxingFlg_UART0=false;
    gvb_SioRxingFlg_UART0=false;
    gb_SioConfigedFlg_UART0=true;
}


/*********************************************************************
��������: void UartGMInit_UART1(void)
��    ��: ����0��ʼ��
˵    ��: ͨѶ����
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void UartGMInit_UART1(void)
{
    gb_SioConfigedFlg_UART1 = false;
    UART_Init(UART1);               //��λ
    UART_GuardTimeConfig(UART1,0); //����Txempty��λ�ȴ���ʱ
    UART_FifoConfig(UART1,DISABLE);//�ر�FIFO
    UART_LoopBackConfig(UART1,DISABLE);//�رջ�д

    //���ݴ���������ó�ʼ������ ������ У�鷽ʽ
    UART_Config(UART1,gu32_BaudRateArry[gs_Setup.BaudRate_1],g_ParityArry[gs_Setup.ParityMode_1],g_StopBitsArry[gs_Setup.ParityMode_1],g_DataLenArry[gs_Setup.ParityMode_1]);
    if (gs_Setup.ComMode_1 == COM_CONT || gs_Setup.ComMode_1 == COM_PRINT)
        UART_RxConfig(UART1,DISABLE);//CONT��ʽ��ʹ�ܽ���
    else
        UART_RxConfig(UART1,ENABLE);//ʹ�ܽ���
    UART_FifoReset(UART1, UART_RxFIFO);
    UART_ItConfig(UART1,UART_RxBufNotEmpty,ENABLE);//ʹ��"����" �ж�,�����ж�Ӧ�õȵ���Ҫ���͵�ʱ��������
    UART_OnOffConfig(UART1,ENABLE);//ʹ�ܴ���

    EIC_IRQChannelPriorityConfig(UART1_IRQChannel, UART1_SIPL);
    EIC_IRQChannelConfig(UART1_IRQChannel, ENABLE);

    gvb_SioTxingFlg_UART1=false;
    gvb_SioRxingFlg_UART1=false;
    gb_SioConfigedFlg_UART1=true;
}

/*********************************************************************
��������: void Uart1_Init(void)
��    ��: ����1��ʼ��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
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
        case COM_BUS_R: //MODBUS-RTUģʽͨѶ
            UartModbusInit_UART1();
            break;
        case COM_READ:
        case COM_READ_O:
        case COM_CONT:
        case COM_PRINT:
        case COM_BUS_A:
            UartGMInit_UART1();         //GMЭ��Ĵ��ڳ�ʼ��
            break;
        case COM_PBUS:
            UARTInit_Pbus();       //PROFIBUS���ߴ��ڳ�ʼ��
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
    if (fu8p_Array == gu8_TxdArray_UART0)      //uart0����
    {
        gvb_EndCharFlg_UART0=false;
        gvb_SioTxingFlg_UART0=true;                          //��������
        gvu8_TxdCnt_UART0=0;
        Sio_485_Disable();
        gb_S485Recieve = false; //����RS485����Modbus ASCII��ʽͨѶʱż�������жϵ����⡣ 
        UART_FifoReset(UART0, UART_TxFIFO);         //��շ��Ͷ���
        UART_ItConfig(UART0,UART_TxEmpty,ENABLE);   //�����ڷ����ж�
    }
    else                    //uart1����
    {
        gvb_EndCharFlg_UART1=false;
        gvb_SioTxingFlg_UART1=true;                          //��������
        gvu8_TxdCnt_UART1=0;
        UART_FifoReset(UART1, UART_TxFIFO);         //��շ��Ͷ���
        UART_ItConfig(UART1,UART_TxEmpty,ENABLE);   //�����ڷ����ж�
    }
}

/*********************************************************************
��������: void Com_Cont(u8 *fu8p_Array)
��    ��: contģʽͨѶ����
˵    ��: ReadģʽRSҲ�����������
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void Com_Cont(u8 *fu8p_Array)
{
    u8 i;
    
    //�Ӻ�
    i = (fu8p_Array == gu8_TxdArray_UART0) ? gs_Setup.TareNumber_2 : gs_Setup.TareNumber_1;
    fu8p_Array[0] = STARTCHAR;
    fu8p_Array[1] = 0x30 + i / 10;
    fu8p_Array[2] = 0x30 + i % 10;       //�Ӻ�
    //���ʽΪR��������ʽΪC
    if (fu8p_Array == gu8_TxdArray_UART0)
        fu8p_Array[3] = gs_Setup.ComMode_2 == COM_CONT ? 'C' : 'R';
    else
        fu8p_Array[3] = gs_Setup.ComMode_1 == COM_CONT ? 'C' : 'R';
    fu8p_Array[4] = 'S';
    if (gb_RunFlg == false) //ֹͣ״̬
        fu8p_Array[5] = '0';
    else
    {
        if (gb_BatchOverFlg)    //�������
            fu8p_Array[5] = '8';
        else if (gu32_Process == T1PROCESS) //����ǰ
            fu8p_Array[5] = '1';
        else if (gu32_Process == LARGEPROCESS)  //���
            fu8p_Array[5] = '2';
        else if (gu32_Process == MIDDLEPROCESS) //�м�
            fu8p_Array[5] = '3';
        else if (gu32_Process == SMALLPROCESS)  //����
            fu8p_Array[5] = '4';
        else if (gu32_Process == T3PROCESS || gu32_Process == T4PROCESS)    //��ֵ
        {
            if ((gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B) && (gb_FlapWaitFlg || gb_FlapStartFlg))  //�޶� �Ĵ�
                fu8p_Array[5] = '6';
            else
                fu8p_Array[5] = '5';
        }
        else if (gu32_Process == FALLPROCESS || gu32_Process == T5PROCESS)
        {
            if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)    //�ж� ж��
                fu8p_Array[5] = '6';
        }
    }
    if (gb_OflFlg)
        fu8p_Array[6] = 'O';    //���
    else if (gb_StabFlg)
        fu8p_Array[6] = 'S';    //�ȶ�
    else
        fu8p_Array[6] = 'M';    //����
    //����N ë��G
    fu8p_Array[7] = (gb_DispNetFlg == true) ? 'N' : 'G';
    //������
    fu8p_Array[8] = (gb_SigWtFlg == true) ? '-' : '+';
    //7λ��������С���㣬[9~15]��
    //����תΪASCII�����fu8p_Array��
    BIN4toNASC_Point(gu32_Wt, &fu8p_Array[9], 6, gs_Cal.Point);
    SRCFrame(fu8p_Array, 16);
}

#ifdef DEBUG_WT_SEND
/*********************************************************************
��������: u8 Debug_State1(void)
��    ��: ���ز��Ե�״̬1
˵    ��: 
��ڲ���: 
�� �� ֵ: 
״̬1 31H ������ʱ
      32H ���
      33H �мӽ�ֹ�Ƚ�ʱ��
      34H �м�
      35H ���ӽ�ֹ�Ƚ�ʱ��
      36H ����
      37H ��Ƿ����
      38H ��ֵ
      39H ж��
      3AH ж����ʱ
      30H ����
      
��    ��: �Ժ���               ʱ    ��: 2015-07-16
��    ��:                      ʱ    ��: 
*********************************************************************/
u8 Debug_State1(void)
{
    u8 lu8_Result = '0';
    
    if (gb_RunFlg == false)
        lu8_Result = '0';
    else if (gu32_Process == T1PROCESS)  //������ʱ
        lu8_Result = '1';
    else if(gu32_Process == LARGEPROCESS)  //���
        lu8_Result = '2';
    else if (gu32_Process == MIDDLEPROCESS)
    {
        if (gb_TimeRunFlg)      //�мӽ�ֹ�Ƚ�ʱ��
            lu8_Result = '3';
        else
            lu8_Result = '4';   //�м�
    }
    else if (gu32_Process == SMALLPROCESS)
    {
    #ifndef DEBUG_SMARTT2B
        if (gb_TimeRunFlg)      //���ӽ�ֹ�Ƚ�ʱ��
            lu8_Result = '5';
    #else
        if (gb_T2bRunFlg)
        {   //���ܽ�ֹ�Ƚ�ʱ���ж��Ѿ�����
            lu8_Result = '5';
        }
    #endif
        else
            lu8_Result = '6';   //����
    }
    else if (gu32_Process == T3PROCESS)    //��Ƿ����
        lu8_Result = '7';
    else if (gu32_Process == T4PROCESS)    //��ֵ
        lu8_Result = '8';
    else if (gu32_Process == FALLPROCESS)    //ж��
        lu8_Result = '9';
    else if (gu32_Process == T5PROCESS)    //ж����ʱ
        lu8_Result = ':';
        
    return lu8_Result;
}

/*********************************************************************
��������: u8 Debug_State2(u8* fu8p_State2)
��    ��: ���ز��Ե�״̬2
˵    ��: 
��ڲ���: 
�� �� ֵ: 
״̬2 31H ������ʱ��ʼ��
      32H �����ʼ��
      33H �м���ʼ��
      34H �мӽ�ֹ�Ƚ�ʱ�����ʱ�̵�
      35H ������ʼ��
      36H ���ӽ�ֹ�Ƚ�ʱ�����ʱ�̵�
      37H ��Ƿ������ʼ��
      38H ��ֵ��ʼ��
      39H ж�������� �����ӽ��
      3AH ж����ʱ��ʼ��
      30H ����
      
��    ��: �Ժ���               ʱ    ��: 2015-07-16
��    ��:                      ʱ    ��: 
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
��������: void Debug_Com_Cont_Wt(u8 *fu8p_Array)
��    ��: �����ù��ܣ������˲�ǰ���˲�������
˵    ��: 
STX ״̬1 ״̬2  +/-δ�˲�������ֵ             +/-�˲��������ֵ           CR  LF
02H  30H   30H  2BH 30H 30H 30H 30H 30H 30H  2BH 31H 31H 31H 31H 31H 31H  0DH 0AH

״̬1 31H ������ʱ
      32H ���
      33H �мӽ�ֹ�Ƚ�ʱ��
      34H �м�
      35H ���ӽ�ֹ�Ƚ�ʱ��
      36H ����
      37H ��ֵʱ��
      38H ж��
      39H ж����ʱ
      30H ����

״̬2 31H ������ʱ��ʼ��
      32H �����ʼ��
      33H �м���ʼ��
      34H �мӽ�ֹ�Ƚ�ʱ�����ʱ�̵�
      35H ������ʼ��
      36H ���ӽ�ֹ�Ƚ�ʱ�����ʱ�̵�
      37H ��Ƿ������ʼ��
      38H ��ֵ��ʼ��
      39H ж�������� �����ӽ��
      3AH ж����ʱ��ʼ��
      30H ����
      
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-4-30
��    ��:                      ʱ    ��: 
*********************************************************************/
void Debug_Com_Cont_Wt(u8 *fu8p_Array)
{
    u32 lu32_Temp;
    
    fu8p_Array[0] = STARTCHAR;
        
    fu8p_Array[1] = Debug_State1(); //״̬1
    fu8p_Array[2] = Debug_State2(&gu8_DebugState2); //״̬2
        
    fu8p_Array[3] = (gb_SigWtNotFedFlg ? '-' : '+');
    //����תΪASCII�����fu8p_Array�� 5λ
    BIN4toNASC_Echo0(gu32_WtNotFed, &fu8p_Array[4], 5);
    
    fu8p_Array[9] = (gb_SigWtFlg ? '-' : '+');
    //����תΪASCII�����fu8p_Array��
    BIN4toNASC_Echo0(gu32_Wt, &fu8p_Array[10], 5);
    
    //�˲�ǰ�������kֵ 2λС����
//    fu8p_Array[15] = (gs32_LinearKNotFed < 0 ? '-' : '+');
//    lu32_Temp = (gs32_LinearKNotFed < 0 ? (-gs32_LinearKNotFed) : gs32_LinearKNotFed) / 10;
//    BIN4toNASC_Echo0(lu32_Temp, &fu8p_Array[16], 5);
    
    //�˲����������kֵ
    fu8p_Array[21] = (gs32_LinearKFed < 0 ? '-' : '+');
    lu32_Temp = (gs32_LinearKFed < 0 ? (-gs32_LinearKFed) : gs32_LinearKFed) / 10;
    BIN4toNASC_Echo0(lu32_Temp, &fu8p_Array[22], 5);
    
    fu8p_Array[27] = ENDCHAR1;
    fu8p_Array[28] = ENDCHAR2;
}

//�����˲�ǰ���������������˲���������λС���㣩
void Debug_Com_Cont_Wt_Kalman(u8 *fu8p_Array)
{
    u32 lu32_Temp;
        
    fu8p_Array[0] = Debug_State1(); //״̬1
    fu8p_Array[1] = 9; //TAB
    fu8p_Array[2] = Debug_State2(&gu8_DebugState2); //״̬2
    fu8p_Array[3] = 9; //TAB
        
    fu8p_Array[4] = (gb_SigWtNotFedFlg ? '-' : '+');
    //����תΪASCII�����fu8p_Array�� 5λ
    BIN4toNASC_Echo0(gu32_WtNotFed, &fu8p_Array[5], 5);
    fu8p_Array[10] = 9; //TAB
    fu8p_Array[11] = (gb_SigWtFlg ? '-' : '+');
    //����תΪASCII�����fu8p_Array��
    BIN4toNASC_Echo0(gu32_Wt, &fu8p_Array[12], 5);
    fu8p_Array[17] = 9; //TAB
    //�������˲����� 1С��
    fu8p_Array[18] = (gf_WtKalmanFed < 0 ? '-' : '+');
    lu32_Temp = (gf_WtKalmanFed < 0 ? ((-gf_WtKalmanFed) * 10 + 0.5) : gf_WtKalmanFed * 10 + 0.5);
    BIN4toNASC_Point(lu32_Temp, &fu8p_Array[19], 6, 1);
    fu8p_Array[26] = 9; //TAB
    //�˲����������kֵ
    fu8p_Array[27] = (gs32_LinearKFed < 0 ? '-' : '+');
    lu32_Temp = (gs32_LinearKFed < 0 ? (-gs32_LinearKFed) : gs32_LinearKFed);
    BIN4toNASC_Echo0(lu32_Temp, &fu8p_Array[28], 6);
    fu8p_Array[34] = 9; //TAB
    //����
    BIN4toNASC_Echo0(gu32_DebugSumNum, &fu8p_Array[35], 3);
    
    fu8p_Array[38] = ENDCHAR1;
    fu8p_Array[39] = ENDCHAR2;
    Debug_Send(fu8p_Array, 40);
}

/*********************************************************************
��������: void Debug_Com_Cont_AD(u8 *fu8p_Array)
��    ��: �����ù��ܣ������˲�ǰ���˲���AD��
˵    ��: 
STX ״̬1 ״̬2  +/-δ�˲�������ֵ             +/-�˲��������ֵ           CR  LF
02H  30H   30H  2BH 30H 30H 30H 30H 30H 30H  2BH 31H 31H 31H 31H 31H 31H  0DH 0AH

״̬1 31H ������ʱ
      32H ���
      33H �мӽ�ֹ�Ƚ�ʱ��
      34H �м�
      35H ���ӽ�ֹ�Ƚ�ʱ��
      36H ����
      37H ��ֵʱ��
      38H ж��
      39H ж����ʱ
      30H ����

״̬2 31H ������ʱ��ʼ��
      32H �����ʼ��
      33H �м���ʼ��
      34H �мӽ�ֹ�Ƚ�ʱ�����ʱ�̵�
      35H ������ʼ��
      36H ���ӽ�ֹ�Ƚ�ʱ�����ʱ�̵�
      37H ��Ƿ������ʼ��
      38H ��ֵ��ʼ��
      39H ж�������� �����ӽ��
      3AH ж����ʱ��ʼ��
      30H ����
      
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-5-13
��    ��:                      ʱ    ��: 
*********************************************************************/
void Debug_Com_Cont_AD(u8 *fu8p_Array)
{
    fu8p_Array[0] = STARTCHAR;
    fu8p_Array[1] = Debug_State1(); //״̬1
    fu8p_Array[2] = Debug_State2(&gu8_DebugState2); //״̬2
        
    fu8p_Array[3] = '+';
    //����תΪASCII�����fu8p_Array��
    BIN4toNASC_Echo0(gu32_AdValue, &fu8p_Array[4], 6);
    
    fu8p_Array[10] = '+';
    //����תΪASCII�����fu8p_Array��
    BIN4toNASC_Echo0(gu32_AdFedValue, &fu8p_Array[11], 6);
    
    fu8p_Array[17]=ENDCHAR1;
    fu8p_Array[18]=ENDCHAR2;
}

#endif


#ifdef DEBUG_WT_STATE_SEND
/*********************************************************************
��������: void Debug_Com_Cont_Wt_State(u8 *fu8p_Array)
��    ��: �����ù��ܣ������˲�ǰ���˲�������
˵    ��: 
STX  ʱ��    ״̬   +/-����ֵ                    CR  LF
02H (30~39H)  30H   2BH 30H 30H 30H 30H 30H 30H  0DH 0AH
 
ʱ���Ǵ�30~39Hѭ��

״̬ 31H ���
     32H �м�
     33H ����
     34H ��ֵ
     35H ж��
     30H ����

����ֵ ��λ0Ϊ�ո�20H
     
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-6-26
��    ��:                      ʱ    ��: 
*********************************************************************/
void Debug_Com_Cont_Wt_State(u8 *fu8p_Array)
{
    static u8 i = 0;
    
    fu8p_Array[0] = STARTCHAR;
    
    fu8p_Array[1] = 0x30 + i;
    i++;
    if (i > 9)
        i = 0;
    
    if (gu32_Process == LARGEPROCESS)  //���
        fu8p_Array[2] = '1';
    else if (gu32_Process == MIDDLEPROCESS) //�м�
        fu8p_Array[2] = '2';
    else if (gu32_Process == SMALLPROCESS)  //����
        fu8p_Array[2] = '3';
    else if (gu32_Process == T3PROCESS)
         fu8p_Array[2] = '4';
    else if (gu32_Process == T4PROCESS)    //��ֵ
    {
        if (gu32_Sys10ms - gu32_Time1_5 < gu32_TimeBuf)
            fu8p_Array[2] = '4';
        else
            fu8p_Array[2] = '0';
    }
    else if (gu32_Process == FALLPROCESS || gu32_Process == T5PROCESS)     //ж��
        fu8p_Array[2] = '5';
    else
        fu8p_Array[2] = '0';
        
        
    fu8p_Array[3] = (gb_SigWtFlg ? '-' : '+');
    //����תΪASCII�����fu8p_Array��
    BIN4toNASC_Echo0(gu32_Wt, &fu8p_Array[4], 6);
    
    fu8p_Array[10] = ENDCHAR1;
    fu8p_Array[11] = ENDCHAR2;
}
#endif

/*********************************************************************
��������: void Com_Cont_o(u8 *fu8p_Array, u8 fu8_Uart)
��    ��: cont_oģʽͨѶ����
˵    ��: Ϊ���ݾɰ汾04CD�����ۼƴ���4λ���ۼ�����9λ������1λС���㣩
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void Read_RT(u8 *fu8p_Array)
{
    u8 i;
    
    //�Ӻ�
    i = (fu8p_Array == gu8_TxdArray_UART0) ? gs_Setup.TareNumber_2 : gs_Setup.TareNumber_1;
    fu8p_Array[0] = STARTCHAR;
    fu8p_Array[1] = 0x30 + i / 10;
    fu8p_Array[2] = 0x30 + i % 10;       //�Ӻ�
    fu8p_Array[3] = 'R';
    fu8p_Array[4] = 'T';
    //�ۼƴ���תΪASCII�����fpu8_TxdArry�У�9λ������[5~13]��
    BIN4toNASC(gu32_SumNum, &fu8p_Array[5], 9);
    for (i = 5; i < 13; i++)        //����
    {
        if (fu8p_Array[i] == 0x30)
            fu8p_Array[i] = ' ';
        else
            break;
    }
    fu8p_Array[14] = ',';
    fu8p_Array[15] = 0x20;
    //�ۼ�����תΪASCII�����gu8_SioSendArry_Pnt�У�10λ����һλС���㣬���������[15~24]��
    BIN4toNASC(gu32_SumWt, &fu8p_Array[16], 9);
    if (gs_Cal.Point != 0)          //С���㴦��
    {
        for (i = 0; i < 9 - gs_Cal.Point; i++)
            fu8p_Array[15+i] = fu8p_Array[16+i];
        fu8p_Array[15+i] = '.';
    }
    for (i = 15; i < 24; i++)        //����
    {
        if (fu8p_Array[i] == 0x30 && fu8p_Array[i+1] != '.')
            fu8p_Array[i] = ' ';
        else
            break;
    }
    SRCFrame(fu8p_Array, 25);
}

/*********************************************************************
��������: void Com_Cont_o(u8 *fu8p_Array, u8 fu8_Uart)
��    ��: cont_oģʽͨѶ����
˵    ��: Ϊ���ݾɰ汾04CD�����ۼƴ���4λ���ۼ�����9λ������1λС���㣩
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void Read_RT_O(u8 *fu8p_Array)
{
    u8 i;
    
    //�Ӻ�
    i = (fu8p_Array == gu8_TxdArray_UART0) ? gs_Setup.TareNumber_2 : gs_Setup.TareNumber_1;
    fu8p_Array[0] = STARTCHAR;
    fu8p_Array[1] = 0x30 + i / 10;
    fu8p_Array[2] = 0x30 + i % 10;       //�Ӻ�
    fu8p_Array[3] = 'R';
    fu8p_Array[4] = 'T';
    //�ۼƴ���תΪASCII�����fpu8_TxdArry�У�4λ������[5~8]��
    BIN4toNASC_Echo0(gu32_SumNum, &fu8p_Array[5], 4);
    fu8p_Array[9] = ',';
    //�ۼ�����תΪASCII�����gu8_SioSendArry_Pnt�У�9λ����һλС���㣬���������[10~18]��
    BIN4toNASC_Point(gu32_SumWt, &fu8p_Array[10], 8, gu32_SumWtPoint);   //��װ����9�ֽ�
    SRCFrame(fu8p_Array, 19);
}

/*********************************************************************
��������: void Commumication(void)
��    ��: ͨѶ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void Commumication(void)
{
#ifndef DEBUG_WT_SEND
    static u32 su32_TimeTemp_1 = 0;
    static u32 su32_TimeTemp_2 = 0;
#endif

    if (bUARTSending_UART0 == false)    //����ͨѶģʽ�����ݷ���
    {
        Debug_Com_Cont_Wt_Kalman(gu8_TxdArray_UART0);   //���Ϳ������˲�����
    }
    else
    {   //���ܱ�֤ÿ��ת�������ݶ��ܱ����ͳ�ȥ�������������ʾ
        BuzzerStart(5);
    }
    PrintDeal(gu8_TxdArray_UART1);    //��ӡ��ش���
    
//    if (gb_DebugUart1 == false)
//    {
//        switch (gs_Setup.ComMode_1)
//        {
//            case COM_READ:
//            case COM_READ_O:
//                Com_Read(gu8_TxdArray_UART1);
//                break;
//            case COM_CONT:
//            #ifdef DEBUG_WT_SEND    //�����ã����ڷ�����������
//                if (gvb_SioTxingFlg_UART1 == false)    //����ͨѶģʽ�����ݷ���
//                {
//                    Debug_Com_Cont_AD(gu8_TxdArray_UART1);
//                    StartSend_Com(gu8_TxdArray_UART1);
//                }
//                else
//                {   //���ܱ�֤ÿ��ת�������ݶ��ܱ����ͳ�ȥ�������������ʾ
//                    BuzzerStart(5);
//                }
//            #else
//                #ifdef DEBUG_WT_STATE_SEND
//                    if (gvb_SioTxingFlg_UART1 == false)    //����ͨѶģʽ�����ݷ���
//                    {
//                        Debug_Com_Cont_Wt_State(gu8_TxdArray_UART1);
//                        StartSend_Com(gu8_TxdArray_UART1);
//                    }
//                    else
//                    {   //���ܱ�֤ÿ��ת�������ݶ��ܱ����ͳ�ȥ�������������ʾ
//                        BuzzerStart(5);
//                    }
//                #else   //��������
//                    if (gu32_Sys10ms - su32_TimeTemp_1 > 1)        //���ͼ������10ms v1.01
//                    {
//                        if (gvb_SioTxingFlg_UART1 == false)    //����ͨѶģʽ�����ݷ���
//                        {   //����δ�ڷ������ݲŷ���
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
//                PrintDeal(gu8_TxdArray_UART1);    //��ӡ��ش���
//                break;
//            case COM_PBUS:
//                Com_Pbus_DP(); //PROFIBUS����
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
//            #ifdef DEBUG_WT_SEND    //�����ã����ڷ�����������
//                if (bUARTSending_UART0 == false)    //����ͨѶģʽ�����ݷ���
//                {
//                    Debug_Com_Cont_Wt_Kalman(gu8_TxdArray_UART0);   //���Ϳ������˲�����
////                    Debug_Com_Cont_Wt(gu8_TxdArray_UART0);
////                    StartSend_Com(gu8_TxdArray_UART0);
//                }
//                else
//                {   //���ܱ�֤ÿ��ת�������ݶ��ܱ����ͳ�ȥ�������������ʾ
//                    BuzzerStart(5);
//                }
//            #else
//                #ifdef DEBUG_WT_STATE_SEND
//                    if (gvb_SioTxingFlg_UART0 == false)    //����ͨѶģʽ�����ݷ���
//                    {
//                        Debug_Com_Cont_Wt_State(gu8_TxdArray_UART0);
//                        StartSend_Com(gu8_TxdArray_UART0);
//                    }
//                    else
//                    {   //���ܱ�֤ÿ��ת�������ݶ��ܱ����ͳ�ȥ�������������ʾ
//                        BuzzerStart(5);
//                    }
//                #else   //��������
//                    if (gu32_Sys10ms - su32_TimeTemp_2 > 1)        //���ͼ������10ms v1.01
//                    {
//                        if (gvb_SioTxingFlg_UART0 == false)    //����ͨѶģʽ�����ݷ���
//                        {   //����δ�ڷ������ݲŷ���
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
//                PrintDeal(gu8_TxdArray_UART0);    //��ӡ��ش���
//                break;
//        }
//    }
}

/*********************************************************************
��������: void RxdArry_To_TxdArry(u8 fu8_Uart, u8 fu8_Num)
��    ��: �����������ݴ��뷢������
˵    ��: 
��ڲ���: fu8_Num = �������ݵĸ���
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void RxdArry_To_TxdArry(u8 fu8_Uart, u8 fu8_Num)
{
    if (fu8_Uart == 0)
        memcpy(gu8_TxdArray_UART0, gu8_RxdArray_UART0, fu8_Num);
    else
        memcpy(gu8_TxdArray_UART1, gu8_RxdArray_UART1, fu8_Num);
}

/*********************************************************************
��������: bool RxData_Check(u8 *fu8p_Array)
��    ��: ���ڽ������ݼ���У��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
bool RxData_Check(u8 *fu8p_Array)
{
    u32 lu32_Temp;
    
    lu32_Temp = (fu8p_Array[1] - 0x30) * 10 + (fu8p_Array[2] - 0x30);     //�õ��Ӻ�
    
    if (lu32_Temp != ((fu8p_Array == gu8_TxdArray_UART0) ? gs_Setup.TareNumber_2 : gs_Setup.TareNumber_1))
        return false;
    if (fu8p_Array[3] == 'R' && (fu8p_Array[4] == 'S' || fu8p_Array[4] == 'T'))
    {
        if (SRCCheck(fu8p_Array, 5))
            return true;
        else
        {
            Read_CE(fu8p_Array);      //У�����
            StartSend_Com(fu8p_Array);
            return false;
        }
    }
    else
    {
        Read_CE(fu8p_Array);      //�Ƿ�����
        StartSend_Com(fu8p_Array);
        return false;
    }
}

/*********************************************************************
��������: bool JudgeFrame(u8 *fu8p_Array, u8 i, u8 j, u8 k)
��    ��: �жϽ���֡�Ĳ�������
˵    ��: ����������3�ֽ���ɣ���fu8p_Arry[4]��ʼ
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
bool JudgeFrame(u8 *fu8p_Array, u8 i, u8 j, u8 k)
{
    if (fu8p_Array[4] == i && fu8p_Array[5] == j && fu8p_Array[6] == k)
        return true;
    else
        return false;
}

/*********************************************************************
��������: void Read_CE(u8 *fu8p_Array)
��    ��: readЭ�鷵��CE�����յ����ݲ����б��ڻ�У��ʹ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void Read_CE(u8 *fu8p_Array)
{
    fu8p_Array[3] = 'C';
    fu8p_Array[4] = 'E';
    SRCFrame(fu8p_Array, 5);
}

/*********************************************************************
��������: void Deal_Read_R(u8 *fu8p_Array)
��    ��: ������R�������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-8-19
��    ��:                      ʱ    ��: 
*********************************************************************/
void Deal_Read_R(u8 *fu8p_Array)
{
    if (fu8p_Array[4] == 'S')        //��״̬
        Com_Cont(fu8p_Array);
    else if (fu8p_Array[4] == 'T')        //���ۼ�
    {
        //����Ǿ�Э���ۼƴ���4λ �ۼ�����8λ+1λС����
        if ((fu8p_Array == gu8_TxdArray_UART0 && gs_Setup.ComMode_2 == COM_READ_O) || 
            (fu8p_Array == gu8_TxdArray_UART1 && gs_Setup.ComMode_1 == COM_READ_O))
            Read_RT_O(fu8p_Array);
        else
            Read_RT(fu8p_Array);
    }
//    else if (JudgeFrame(fu8p_Array, 'R', 'I', 'D'))       //�䷽��
//        Read_RRID(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'A', 'L', 'A'))  //A������Ϣ
//        Read_RALA(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'A', 'L', 'B'))  //B������Ϣ
//        Read_RALB(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'R', 'P', 'C'))  //������
//        Read_RRPC(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'S', 'P', 'C'))  //ʣ��������
//        Read_RSPC(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'O', 'T', 'R'))  //��������
//        Read_ROTR(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'W', 'M', 'D'))  //����ģʽ
//        Read_RWMD(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'F', 'L', 'U'))  //����ģʽ
//        Read_RFLU(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'T', ' ', ' '))  //�ۼ�
//        Com_Cont(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'S', 'A', ' '))  //A״̬
//        Read_RSA_(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'S', 'B', ' '))  //B״̬
//        Read_RSB_(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'P', 'T'))  //С����
//        Read_RCPT(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'U', 'N'))  //��λ
//        Read_RCUN(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'D', 'D'))  //�ֶ�ֵ
//        Read_RCDD(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'A', 'P'))  //�������
//        Read_RCAP(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'P', 'S'))  //�궨����
//        Read_RCPS(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'V', 'A'))  //A���Ժ�����
//        Read_RCVA(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'V', 'B'))  //B���Ժ�����
//        Read_RCVB(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'Z', 'A'))  //A��������
//        Read_RCZA(fu8p_Array);
//    else if (JudgeFrame(fu8p_Array, 'C', 'Z', 'B'))  //B��������
//        Read_RCZB(fu8p_Array);
//    else if (fu8p_Array[4] == 'B')                     //��������
//        Read_RBXX(fu8p_Array);
//    else if (fu8p_Array[4] == 'F')                     //�䷽����
//        Read_RFXX(fu8p_Array);
//    else if (fu8p_Array[4] == 'T')                     //�䷽�ۼ�
//        Read_RTXX(fu8p_Array);
    else
        Read_CE(fu8p_Array);
}

/*********************************************************************
��������: void Deal_Read(u8 *fu8p_Array)
��    ��: ����read��ʽ�µķ�������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
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
��������: void Com_Read(u8 *fu8p_Array)
��    ��: readģʽͨѶ����
˵    ��: 
��ڲ���: fu8p_Array���������׵�ַ
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void Com_Read(u8 *fu8p_Array)
{
    if (fu8p_Array == gu8_TxdArray_UART0 && gvb_EndCharFlg_UART0)            //����0���յ�����������
    {
        if (gvb_SioTxingFlg_UART0 == false)    //����δ��������
        {
            RxdArry_To_TxdArry(0, RXDLEN_READ);
            if (RxData_Check(gu8_TxdArray_UART0))             //�������ݵ�У��
                Deal_Read(gu8_TxdArray_UART0);            //����read��ʽ��������
            else
                gvb_EndCharFlg_UART0 = false;    //���Խ�����һ֡����
        }
    }
    if (fu8p_Array == gu8_TxdArray_UART1 && gvb_EndCharFlg_UART1)            //����1���յ�����������
    {
        if (gvb_SioTxingFlg_UART1 == false)    //����δ��������
        {
            RxdArry_To_TxdArry(1, RXDLEN_READ);
            if (RxData_Check(gu8_TxdArray_UART1))             //�������ݵ�У��
                Deal_Read(gu8_TxdArray_UART1);            //����read��ʽ��������
            else
                gvb_EndCharFlg_UART1 = false;    //���Խ�����һ֡����
       }
    }
}


/***********************************************************************
�� �� ��: void DebugInit_UART0(UartSet_Typedef *pUartSet)
��    �ܣ�
˵    ����
��ڲ�����
������Դ��
�� �� ֵ����
��    �ƣ��Ժ���               ��    �ڣ�
��    �ģ�                     ��    �ڣ�
***********************************************************************/
void DebugInit_UART0(void)
{
    UART_Init(UART0);
    Sio_485_Enable();
    
    //������ʱ�򣬷��Ͷ�����Ч����Ϊ0����ָ�붼ָ�������ͷ����
    ulTBUFAvailNum_UART0=0;
    ulTBUFBegin_UART0=0;
    bUARTSending_UART0=false;       //����û�з��Ͳ���
    
    //ʹ��UART0����ս���FIFO����շ���FIFO��ʹ��UART0��FIFO
    UART_OnOffConfig(UART0,ENABLE);
    UART_FifoReset(UART0,UART_RxFIFO);
    UART_FifoReset(UART0,UART_TxFIFO);
    UART_FifoConfig(UART0,ENABLE);
    
    //��д��ʽ��
    UART_LoopBackConfig(UART0,DISABLE);
    
    //����0�����ʣ�У�鷽ʽ������
    UART_Config(UART0,gu32_BaudRateArry[gs_Setup.BaudRate_2],g_ParityArry[gs_Setup.ParityMode_2],g_StopBitsArry[gs_Setup.ParityMode_2],g_DataLenArry[gs_Setup.ParityMode_2]);
    
    //�����жϵ����ȼ�������ʹ��UART0�ж�
    EIC_IRQChannelPriorityConfig(UART0_IRQChannel,UART0_SIPL);
    EIC_IRQChannelConfig(UART0_IRQChannel,ENABLE);//
}

/*************************************************************
�� �� ��: bool Debug_Send(u8 *fu8p_Data, u32 fu32_Num)
��    �ܣ����ͺ���
˵    ����
��ڲ�����
������Դ��
�� �� ֵ����
��    �ƣ��Ժ���               ��    �ڣ�2014-1-22
��    �ģ�                     ��    �ڣ�
*************************************************************/
bool Debug_Send(u8 *fu8p_Data, u32 fu32_Num)
{
    if (fu8p_Data == gu8_TxdArray_UART0)
    {
        if (true==bUARTSending_UART0){
            return(false);
        }
        
        //����������ڷ��͵Ļ��������ǲ����н������ݵ�
        DisableSendIRQ_UART0();
        DisableRecIRQ_UART0();      //�رս����ж�
    
        Sio_485_Disable();
        gb_S485Recieve = false;     //����RS485����Modbus ASCII��ʽͨѶʱż�������жϵ����⡣ 
    
        ulTBUFAvailNum_UART0 = fu32_Num; //�������ݸ����Ĵ���   
        ulTBUFBegin_UART0 = 0; 
        
        bUARTSending_UART0=true;          //�������ڷ���֮��
        
        //ע��һ�����⣬ǧ��Ҫ��bUARTSending_UART0��λ��true֮ǰ���ж�
        //�����ж�һ����Ȼ�����Ͻ����պͿ��жϣ�bUARTSending_UART0
        //û�����ϣ������ϱ��ж���λFalse�ˣ��൱��û�п���
        //ͬ���ulTBUFAvailNum_UART0��ulTBUFBegin_UART0Ҳһ����
        
        EnableSendIRQ_UART0();      //�򿪷����ж�
        
        return(true);
    }
    return false;
}


/***********************************************************************
�� �� ��: void IRQ_Debug_UART0(void)
��    �ܣ�����0�жϷ������
˵    ����
��ڲ�����
������Դ��
�� �� ֵ����
��    �ƣ�                     ��    �ڣ�
��    �ģ�                     ��    �ڣ�
***********************************************************************/
void IRQ_Debug_UART0(void)
{
    u32 i;
    u16 UARTStatus;
    
    //��ȡ״̬�Ĵ���
    UARTStatus=UART_FlagStatus(UART0);
    
    //��������պͻ�����ȫ��������ж�
    if (true==bUARTSending_UART0)
    {
        //��Ϊȫ��������ж�
        if (UARTStatus&UART_TxEmpty)
        {
            if (ulTBUFAvailNum_UART0==0)
            {   //�������
                bUARTSending_UART0=false; //����û�з��Ͳ���
                DisableSendIRQ_UART0();
                EnableRecIRQ_UART0();
                Sio_485_Enable();
                gb_S485Recieve = true;
            }
            else
            {   //��һ�η���
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
        //��Ϊ�����������ж�
        if (UARTStatus&UART_TxHalfEmpty)
        {
            //���ͻ������Ƿ�������
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
            
            //���ڶ԰�յĲ������ܻ����״̬������Ҫ����һ��
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
��������: void IRQ_GM_ModbusASCII_UART0(void)
��    ��: READ CONT PRIINT MODBUS_ASCII����ͨѶ���жϴ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-4-3
��    ��:                      ʱ    ��: 
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
    //�س�ʱ�жϣ���ֹ����ͨѶModbus rtuʱ�л�ͨѶ��ʽ��������� v1.01
    if (UART_FlagStatus(UART0)&UART_TimeOutIdle)
        UART_ItConfig(UART0,UART_TimeOutIdle,DISABLE);
    //�����ж�
    if(UART_FlagStatus(UART0) & (UART_TxEmpty|UART_TxHalfEmpty))
    {
        if(!(gb_SioConfigedFlg_UART0&&gvb_SioTxingFlg_UART0))
        {
            UART_ItConfig(UART0,UART_TxEmpty,DISABLE);
            Sio_485_Enable();
            gb_S485Recieve = true;  //����RS485����Modbus ASCII��ʽͨѶʱż�������жϵ����⡣ v1.01
        }
        else
        {
            lu8_TxdData=gu8_TxdArray_UART0[gvu8_TxdCnt_UART0++];
            UART_ByteSend(UART0,&lu8_TxdData);

            if(lu8_TxdData == ENDCHAR2)
            {
                gvb_SioTxingFlg_UART0=false;
                if (gvu8_PrintCnt_UART0 > 0) //��ӡ��ʹ�ã���ӡ̨ͷ��
                    gvu8_PrintCnt_UART0--;
                gvb_PrintingFlg_UART0 = false;
            }
            if (gvu8_TxdCnt_UART0 > lu8_MaxLen)
            {
                gvb_SioTxingFlg_UART0=false;
                if (gvu8_PrintCnt_UART0 > 0) //��ӡ��ʹ�ã���ӡ̨ͷ��
                    gvu8_PrintCnt_UART0--;
                gvb_PrintingFlg_UART0 = false;
            }
        }
    }
    //�����ж�
    if(UART_FlagStatus(UART0) & (UART_RxBufNotEmpty))
    {
        lu8_RxdData= (u8)(UART0->RxBUFR);
        UART_FifoReset(UART0, UART_RxFIFO);

        if(!gb_SioConfigedFlg_UART0)
            return;

        switch(lu8_RxdData)
        {
            case STARTCHAR: //Read��ʽ��ʼ��
            case MB_ASCII_STARTCHAR:    //Modbus_ASCII��ʼ��
                if(gvb_EndCharFlg_UART0==true)     //��������δ�������
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
��������: void IRQ_GM_ModbusASCII_UART1(void)
��    ��: READ CONT PRIINT MODBUS_ASCII����ͨѶ���жϴ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-4-3
��    ��:                      ʱ    ��: 
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
    
    //�س�ʱ�жϣ���ֹ����ͨѶModbus rtuʱ�л�ͨѶ��ʽ��������� v1.01
    if (UART_FlagStatus(UART1)&UART_TimeOutIdle)
        UART_ItConfig(UART1,UART_TimeOutIdle,DISABLE);
        
    //�����ж�
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
                if (gvu8_PrintCnt_UART1 > 0) //��ӡ��ʹ�ã���ӡ̨ͷ��
                    gvu8_PrintCnt_UART1--;
                gvb_PrintingFlg_UART1 = false;
            }
            if (gvu8_TxdCnt_UART1 > lu8_MaxLen)
            {
                gvb_SioTxingFlg_UART1=false;
                if (gvu8_PrintCnt_UART1 > 0) //��ӡ��ʹ�ã���ӡ̨ͷ��
                    gvu8_PrintCnt_UART1--;
                gvb_PrintingFlg_UART1 = false;
            }
        }
    }
    //�����ж�
    if(UART_FlagStatus(UART1) & (UART_RxBufNotEmpty))
    {
        lu8_RxdData= (u8)(UART1->RxBUFR);
        UART_FifoReset(UART1, UART_RxFIFO);

        if(!gb_SioConfigedFlg_UART1)
            return;

        switch(lu8_RxdData)
        {
            case STARTCHAR: //Read��ʽ��ʼ��
            case MB_ASCII_STARTCHAR:    //Modbus_ASCII��ʼ��
                if(gvb_EndCharFlg_UART1==true)     //��������δ�������
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

