/*
Description:            ���Խ�����غ��� ���ڲ����Զ�ƥ��ȴ���
Author:                 �Ժ���
Date:                   2013-12-9
*/
#include "include.h"

#define DEBUG_UART1             0
#define DEBUG_UART2             1

#define DEBUG_END               1
#define DEBUG_BAUDRATE_NUM      6       //֧�ֵĲ����ʸ���





bool gb_DebugUart1;     //����1���ڵ��Ա�־λ��Ҫ����������ͨѶ����
bool gb_DebugUart0;     //����2���ڵ��Ա�־λ��Ҫ����������ͨѶ����
u32 gu32_DebugTimeBuf;
u32 gu32_DebugError;    //���Թ��̴����־
bool gb_ModbusAnswerFlg_Uart0;   //ModbusЭ���з��ط��ͱ�־λ
bool gb_ModbusAnswerFlg_Uart1;   //ModbusЭ���з��ط��ͱ�־λ
u8 gu8_DebugRxdArray_UART1[RXDLEN];        //��������洢UARTn->RxBUFR��8λ��У��λ��֡����
u8 gu8_DebugRxdArray_UART0[RXDLEN];        //��������洢UARTn->RxBUFR��8λ��У��λ��֡����
u32 gu32_ComModeBak_1;
u32 gu32_ComModeBak_2;


/*********************************************************************
��������: void Debug_UartInit(u8 fu8_BR, u8 fu8_P, u8 fu8_UratType )
��    ��: ��ʼ����������Ӧ���õĴ��ڲ���
˵    ��: 
��ڲ���: fu8_BR=������  fu8_P=У��  fu8_UratType=���ں�
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-2-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void Debug_UartInit(u8 fu8_BR, u8 fu8_P, u8 fu8_UratType )
{
    if (fu8_UratType == 1)   //����1
    {
        gb_ModbusAnswerFlg_Uart1 = false;
        //����ж�У���֡��������� ������ջ�������
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
    else    //����ֵ����2
    {
        gb_ModbusAnswerFlg_Uart0 = false;
        //����ж�У���֡��������� ������ջ�������
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
��������: u8 Debug_UartJudgeDeal(u8 *fpu8_Array)
��    ��: ���ڽ��յ����ݺ���жϺʹ���
˵    ��: 
��ڲ���: 
�� �� ֵ: ^0=���ݲ�ƥ��  ^1=У�鲻ƥ��(MODBUS)  ^2=֡����(MODBUS)
          ^3=7λ����У�鲻ƥ��(READ) ^4=У�鲻ƥ��(READ) ^5=֡����(READ)
��    ��: �Ժ���               ʱ    ��: 2014-2-20
��    ��:                      ʱ    ��: 
*********************************************************************/
u8 Debug_UartJudgeDeal(u8 *fpu8_Array)
{
    u8 lu8_Result = 0;
    u8 i;
    
    if (fpu8_Array == gu8_TxdArray_UART0)       //uart0modbusͨѶ
    {   //���жϽ��յ��������Ƿ����Modbus_rtuͨѶ�ĸ�ʽ
        gs_Setup.ComMode_2 = COM_BUS_R;
        gb_ModbusAnswerFlg_Uart0 = false;
        Com_Modbus(gu8_TxdArray_UART0);
        if (gb_ModbusAnswerFlg_Uart0 == true)
        {
            gb_ModbusAnswerFlg_Uart0 = false;
            //���ϸ�ʽ���ж��Ƿ���У���֡����
            for (i = 0; i < 6; i++)    //����Ƿ���֡�����У�����
            {
                if ((gu8_DebugRxdArray_UART0[i] & 0x01) != 0)    //У�����
                    lu8_Result |= 0x02;
                if ((gu8_DebugRxdArray_UART0[i] & 0x02) != 0)    //֡����
                     lu8_Result |= 0x04;
            }
        }
        else
        {   //���жϽ��յ��������Ƿ����Modbus_ASCIIͨѶ�ĸ�ʽ
            gs_Setup.ComMode_2 = COM_BUS_A;
            gb_ModbusAnswerFlg_Uart0 = false;
            Com_Modbus(gu8_TxdArray_UART0);
            if (gb_ModbusAnswerFlg_Uart0 == true)
            {
                gb_ModbusAnswerFlg_Uart0 = false;
                //���ϸ�ʽ���ж��Ƿ���У���֡����
                for (i = 0; i < 6; i++)    //����Ƿ���֡�����У�����
                {
                    if ((gu8_DebugRxdArray_UART0[i] & 0x01) != 0)    //У�����
                        lu8_Result |= 0x02;
                    if ((gu8_DebugRxdArray_UART0[i] & 0x02) != 0)    //֡����
                         lu8_Result |= 0x04;
                }
            }
            else
            {
                gs_Setup.ComMode_2 = COM_BUS_R;
                //���ж��Ƿ����Read��ʽ
                RxdArry_To_TxdArry(0, RXDLEN);
                for (i = 0; i < RXDLEN; i++)    //�������λ���п�����У��λ
                    gu8_TxdArray_UART0[i] &= 0x7F;
                if (RxData_Check(gu8_TxdArray_UART0))             //�������ݵ�У��
                {   //���ϸ�ʽ���ж��Ƿ���У���֡����
                    for (i = 0; i < 8; i++)
                    {   //7λ����λʱ���λ�п�����У��λ
                        if ((gu8_RxdArray_UART0[i] & 0x80) != 0)
                            lu8_Result |= 0x08; //У�����
                        if ((gu8_DebugRxdArray_UART0[i] & 0x01) != 0) //У�����
                            lu8_Result |= 0x10;
                        if ((gu8_DebugRxdArray_UART0[i] & 0x02) != 0) //֡����
                             lu8_Result |= 0x20;
                    }
                    if (lu8_Result == 0) //ƥ��
                        gs_Setup.ComMode_2 = COM_READ;
                }
                else
                    lu8_Result |= 0x01;
                //���У��������
                memset(gu8_DebugRxdArray_UART0, 0, sizeof(gu8_DebugRxdArray_UART0));
            }
        }
    }
    else                                        //uart1modbusͨѶ
    {   //���жϽ��յ��������Ƿ����Modbus_rtuͨѶ�ĸ�ʽ
        gs_Setup.ComMode_1 = COM_BUS_R;
        gb_ModbusAnswerFlg_Uart1 = false;
        Com_Modbus(gu8_TxdArray_UART1);
        if (gb_ModbusAnswerFlg_Uart1 == true)
        {
            gb_ModbusAnswerFlg_Uart1 = false;
            //���ϸ�ʽ���ж��Ƿ���У���֡����
            for (i = 0; i < 6; i++)    //����Ƿ���֡�����У�����
            {
                if ((gu8_DebugRxdArray_UART1[i] & 0x01) != 0) //У�����
                    lu8_Result |= 0x02;
                if ((gu8_DebugRxdArray_UART1[i] & 0x02) != 0) //֡����
                     lu8_Result |= 0x04;
            }
        }
        else
        {   //���жϽ��յ��������Ƿ����Modbus_ASCIIͨѶ�ĸ�ʽ
            gs_Setup.ComMode_1 = COM_BUS_A;
            gb_ModbusAnswerFlg_Uart1 = false;
            Com_Modbus(gu8_TxdArray_UART1);
            if (gb_ModbusAnswerFlg_Uart1 == true)
            {
                gb_ModbusAnswerFlg_Uart1 = false;
                //���ϸ�ʽ���ж��Ƿ���У���֡����
                for (i = 0; i < 6; i++)    //����Ƿ���֡�����У�����
                {
                    if ((gu8_DebugRxdArray_UART1[i] & 0x01) != 0)    //У�����
                        lu8_Result |= 0x02;
                    if ((gu8_DebugRxdArray_UART1[i] & 0x02) != 0)    //֡����
                         lu8_Result |= 0x04;
                }
            }
            else
            {
                gs_Setup.ComMode_1 = COM_BUS_R;
                //���ж��Ƿ����Read��ʽ
                RxdArry_To_TxdArry(1, RXDLEN);
                for (i = 0; i < RXDLEN; i++)    //�������λ���п�����У��λ
                    gu8_TxdArray_UART1[i] &= 0x7F;
                if (RxData_Check(gu8_TxdArray_UART1))             //�������ݵ�У��
                {   //���ϸ�ʽ���ж��Ƿ���У���֡����
                    for (i = 0; i < 8; i++)
                    {   //7λ����λʱ���λ�п�����У��λ
                        if ((gu8_RxdArray_UART1[i] & 0x80) != 0)
                            lu8_Result |= 0x08; //У�����
                        if ((gu8_DebugRxdArray_UART1[i] & 0x01) != 0) //У�����
                            lu8_Result |= 0x10;
                        if ((gu8_DebugRxdArray_UART1[i] & 0x02) != 0) //֡����
                             lu8_Result |= 0x20;
                    }
                    if (lu8_Result == 0) //ƥ��
                        gs_Setup.ComMode_1 = COM_READ;
                }
                else
                    lu8_Result |= 0x01;
                //���У��������
                memset(gu8_DebugRxdArray_UART1, 0, sizeof(gu8_DebugRxdArray_UART1));
            }
        }
    }
    return lu8_Result;
}

/*********************************************************************
��������: void Key_DebugState(void)
��    ��: ���Խ��洦��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-2-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_DebugState(void)
{
    static u8 su8_FailCnt;
    static bool sb_RecedDataFlg;    //�����ݽ��չ���־�������ж�ƥ��ʧ�ܺ���E01����E02
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
            {   //��ʼ��������Ӧ����
                /*�ô�������Ӧԭ������ö�ٷ���������ΪModbus��ʽ��Ȼ�����������ԣ�
                  9600 19200 4800 38400 57600 2400 115200��˳��仯�����ȽϽ��յ��������Ƿ�����
                  ÿ���������½���3���������ж����ĸ�ͨѶ��ʽ������5��û�����ݽ�������һ��������
                  ��ΪMODBUSͨѶ���õ���ʱ�������ж�һ֡�����Ƿ������ɣ����Լ�ʹ��READͨѶ
                  ��ʽҲ����ȷ���յ����ݡ�
                  �Խ��յ������ݽ��бȶԣ��Ϳ���ȷ��MODBUS����READ������Э��Ͳ����ʾͶ�ȷ���ˣ�
                  Ȼ�����ж�У�鷽ʽ��
                */
                
                if (gu32_Sys10ms - gu32_DebugTimeBuf >= 500)
                {   //����5��û�����ݽ���
                    gu32_DebugTimeBuf = gu32_Sys10ms;   //���¸�ֵ��ʱʱ�仺��
                    if (++ulParameter.ui[0] > DEBUG_BAUDRATE_NUM)
                    {   //�Զ�ƥ��ʧ��
                        //�н��յ������ݱ���E02����E01
                        gu32_DebugError = (sb_RecedDataFlg ? 2 : 1);
                        gb_DebugUart1 = false;
                        gb_FlashFlg = false;
                        gu32_DebugTimeBuf = gu32_Sys10ms;
                        BuzzerStart(100);
                    }
                    else
                        Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 1);
                }
                
                //�Ѿ���ʼ����ɣ����д�������Ӧ�����ж�
                if (bFinishReceive_UART1 == true)
                {   //���յ�����
                    sb_RecedDataFlg = true;
                    gu32_DebugTimeBuf = gu32_Sys10ms;   //���¸�ֵ��ʱʱ�仺��
                    i = Debug_UartJudgeDeal(gu8_TxdArray_UART1);
                    if (i & 0x01)
                    {   //�ȶԽ��յ������ݲ���
                        if (++su8_FailCnt >= 3)
                        {   //���յ������ݽ��бȶԣ�����3��ʧ���������һ��������
                            su8_FailCnt = 0;
                            if (++ulParameter.ui[0] > DEBUG_BAUDRATE_NUM)
                            {   //�Զ�ƥ��ʧ��
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
                        {   //modbusУ������֡����
                            //modbusֻ��8λ����λ
                            if (++ulParameter.ui[1] > 3)
                            {//�Զ�ƥ��ʧ��,��֧�ֵ�У��
                                gu32_DebugError = 3;
                                gb_DebugUart1 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                        }
                        else if (i & 0x08)
                        {   //������read��ʽ7λ����
                            if (ulParameter.ui[1] < 4)
                            {
                                ulParameter.ui[1] = 4;  //17E1У��
                            }
                            else
                            {
                                if (++ulParameter.ui[1] > 6)
                                {//�Զ�ƥ��ʧ��,��֧�ֵ�У��
                                    gu32_DebugError = 3;
                                    gb_DebugUart1 = false;
                                    gb_FlashFlg = false;
                                    BuzzerStart(100);
                                }
                            }
                        }
                        else
                        {   //readУ������֡����
                            //read��8λ����λ��7λ����λ
                            if (++ulParameter.ui[1] > 6)
                            {//�Զ�ƥ��ʧ��,��֧�ֵ�У��
                                gu32_DebugError = 3;
                                gb_DebugUart1 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                        }
                        Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 1);
                    }
                    else
                    {   //���յ�������ȷ��������ز���ֵ
                        FRAM_WriteDW(FMAddr_ComMode_1, gs_Setup.ComMode_1);
                        FRAM_WriteDW(FMAddr_BaudRate_1, gs_Setup.BaudRate_1);
                        FRAM_WriteDW(FMAddr_ParityMode_1, gs_Setup.ParityMode_1);
                        Uart1_Init();
                        //��ʾFiniSh����ƥ��ɹ�
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
            {   //��ʼ��������Ӧ����
                /*�ô�������Ӧԭ������ö�ٷ���������ΪModbus��ʽ��Ȼ�����������ԣ�
                  9600 19200 4800 38400 57600 2400 115200��˳��仯�����ȽϽ��յ��������Ƿ�����
                  ÿ���������½���3���������ж����ĸ�ͨѶ��ʽ������5��û�����ݽ�������һ��������
                  ��ΪMODBUSͨѶ���õ���ʱ�������ж�һ֡�����Ƿ������ɣ����Լ�ʹ��READͨѶ
                  ��ʽҲ����ȷ���յ����ݡ�
                  �Խ��յ������ݽ��бȶԣ��Ϳ���ȷ��MODBUS����READ������Э��Ͳ����ʾͶ�ȷ���ˣ�
                  Ȼ�����ж�У�鷽ʽ��
                */
                
                if (gu32_Sys10ms - gu32_DebugTimeBuf >= 500)
                {   //����5��û�����ݽ���
                    gu32_DebugTimeBuf = gu32_Sys10ms;   //���¸�ֵ��ʱʱ�仺��
                    if (++ulParameter.ui[0] > DEBUG_BAUDRATE_NUM)
                    {   //�Զ�ƥ��ʧ��
                        //�н��յ������ݱ���E02����E01
                        gu32_DebugError = (sb_RecedDataFlg ? 2 : 1);
                        gb_DebugUart0 = false;
                        gb_FlashFlg = false;
                        gu32_DebugTimeBuf = gu32_Sys10ms;
                        BuzzerStart(100);
                    }
                    else
                        Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 0);
                }
                
                //�Ѿ���ʼ����ɣ����д�������Ӧ�����ж�
                if (bFinishReceive_UART0 == true)
                {   //���յ�����
                    sb_RecedDataFlg = true;
                    gu32_DebugTimeBuf = gu32_Sys10ms;   //���¸�ֵ��ʱʱ�仺��
                    i = Debug_UartJudgeDeal(gu8_TxdArray_UART0);
                    if (i & 0x01)
                    {   //�ȶԽ��յ������ݲ���
                        if (++su8_FailCnt >= 3)
                        {   //���յ������ݽ��бȶԣ�����3��ʧ���������һ��������
                            su8_FailCnt = 0;
                            if (++ulParameter.ui[0] > DEBUG_BAUDRATE_NUM)
                            {   //�Զ�ƥ��ʧ��
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
                        {   //modbusУ������֡����
                            //modbusֻ��8λ����λ
                            if (++ulParameter.ui[1] > 3)
                            {//�Զ�ƥ��ʧ��,��֧�ֵ�У��
                                gu32_DebugError = 3;
                                gb_DebugUart0 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                        }
                        else if (i & 0x08)
                        {   //������read��ʽ7λ����
                            if (ulParameter.ui[1] < 4)
                            {
                                ulParameter.ui[1] = 4;  //17E1У��
                            }
                            else
                            {
                                if (++ulParameter.ui[1] > 6)
                                {//�Զ�ƥ��ʧ��,��֧�ֵ�У��
                                    gu32_DebugError = 3;
                                    gb_DebugUart0 = false;
                                    gb_FlashFlg = false;
                                    BuzzerStart(100);
                                }
                            }
                        }
                        else
                        {   //readУ������֡����
                            //read��8λ����λ��7λ����λ
                            if (++ulParameter.ui[1] > 6)
                            {//�Զ�ƥ��ʧ��,��֧�ֵ�У��
                                gu32_DebugError = 3;
                                gb_DebugUart0 = false;
                                gb_FlashFlg = false;
                                BuzzerStart(100);
                            }
                        }
                        Debug_UartInit(ulParameter.ui[0], ulParameter.ui[1], 0);
                    }
                    else
                    {   //���յ�������ȷ��������ز���ֵ
                        FRAM_WriteDW(FMAddr_ComMode_2, gs_Setup.ComMode_2);
                        FRAM_WriteDW(FMAddr_BaudRate_2, gs_Setup.BaudRate_2);
                        FRAM_WriteDW(FMAddr_ParityMode_2, gs_Setup.ParityMode_2);
                        Uart0_Init();
                        //��ʾFiniSh����ƥ��ɹ�
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
                Read_SetupData();   //���¶�ȡ��������ֵ����Ҫ�Ǵ������
                Uart0_Init();                //����0��ʼ��
                Uart1_Init();                //����1��ʼ��
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
��������: void Disp_SetupState(void)
��    ��: ���Խ�����ʾ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-2-17
��    ��:                      ʱ    ��: 
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
                {   //6�ֲ����ʣ���Ӧ���ڼ��־���������-��
                    if (i <= 5)
                        gu8_MainDispArray[i] = SEG_1LINE;
                    else    //ulParameter.ui[0]���6
                        gu8_MainDispArray[5] = SEG_1LINE;
                }
                if (ulParameter.ui[0] <= 5)
                    Flash_MainDisp(0x20 >> ulParameter.ui[0]);                //��˸����
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
                {   //6�ֲ����ʣ���Ӧ���ڼ��־���������-��
                    if (i <= 5)
                        gu8_MainDispArray[i] = SEG_1LINE;
                    else    //ulParameter.ui[0]���6
                        gu8_MainDispArray[5] = SEG_1LINE;
                }
                if (ulParameter.ui[0] <= 5)
                    Flash_MainDisp(0x20 >> ulParameter.ui[0]);                //��˸����
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

