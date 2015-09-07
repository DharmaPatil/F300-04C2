#include "include.h"

u32  gu32_SystemState;         //ϵͳ����״̬
ULDATA ulParameter;            //�����������
u8  gu8_Item;                  //����ʱ���ָ�����Ŀ
bool gb_PrintCodeIngFlg;        //�����־���������μ��ϵ�
bool gb_StopPrintCodeFlg;       //ֹͣ����

#ifdef DEBUG_ADCNVT_TIMES
    u32 gu32_AdConventTimes;        //������ADת������
#endif


//
/***********************************************************
��������: void PrintCodeFun(u32 fu32_PCSwitch,   //���뿪��
                   u32 fu32_PCTime,     //����ʱ��
                   u32 fu32_PCRDelay,   //����������ʱ
                   u32 fu32_Sys10ms,    //ϵͳʱ�䣬��λ10ms
                   bool fb_PackFlg,     //�д����
                   bool *fbp_StopPCFlg, //ֹͣ���룬�����ڴ���ʱֹͣ�ź�����
                   bool *fbp_PCOutFlg,  //���뿪�����������
                   bool *fbp_RCDelayFlg //����������ʱ��������ʱ��Ч����������ж�ϵ�
               )
��    ��: ���빦��
˵    ��: ���뿪��δ�������ʱ��Ϊ��˹�����Ч
          ������ʱ��δ��,��ʱ���ɴ�,���������������
          ֹͣ����������������
          ��Ϊ�����Ŀ���ֲ������ʹ�ø�������������溯����ڱ���ע��
          chx 2013-9-29
��ڲ���: 
���ó���: 
�� �� ֵ: ��
��    ��: �Ժ���              ʱ    ��:2013-9-29
��    ��:                     ʱ    ��:
***********************************************************/
void PrintCodeFun(u32 fu32_PCSwitch,   //���뿪��
                   u32 fu32_PCTime,     //����ʱ��
                   u32 fu32_PCRDelay,   //����������ʱ
                   u32 fu32_Sys10ms,    //ϵͳʱ�䣬��λ10ms
                   bool fb_PackFlg,     //�д����
                   bool *fbp_StopPCFlg, //ֹͣ���룬�����ڴ���ʱֹͣ�ź�����
                   bool *fbp_PCOutFlg,  //���뿪�����������
                   bool *fbp_RCDelayFlg //����������ʱ��������ʱ��Ч����������ж�ϵ�
               )
{
    static bool  bPrnDelayEn = false;      //������ʱ������־
    static bool  bHadPrn = false;              //�Ѵ�ӡ��־
    static u32 baktime1 = 0;
    static u32 baktime2 = 0;
    
    if((fu32_PCSwitch&0x01)==0 || fu32_PCTime==0) //���عرջ����ʱ��Ϊ����˹�����Ч
    {
        *fbp_RCDelayFlg = false;
        return;
    }
    
    if(*fbp_StopPCFlg)       //ֹͣ�ź�����������������
    {
        *fbp_PCOutFlg = false;
        bPrnDelayEn = false;
        bHadPrn = true;
        *fbp_StopPCFlg = false;
    }
    //�˴�Ҳ��һ����ֵ*fbp_RCDelayFlg����䣬��Ϊ�п����������return�����ĸ�ֵ����߲���
    *fbp_RCDelayFlg = ((bPrnDelayEn)||(*fbp_PCOutFlg)) ? true : false;
    
    if(fb_PackFlg)
    {
        if(bHadPrn)
        {
            return;
        }
        if(!bPrnDelayEn)
        {
            bPrnDelayEn = true;        //��ʱ����
            baktime1 = gu32_Sys10ms;       //����ǰʱ��
        }
        if((*fbp_PCOutFlg == false) && (gu32_Sys10ms - baktime1 >= (10*fu32_PCRDelay)))
        {
            *fbp_PCOutFlg = true;
            baktime2 = gu32_Sys10ms;
        }
        
        if(*fbp_PCOutFlg && (gu32_Sys10ms - baktime2 >= (10*fu32_PCTime)))
        {
            *fbp_PCOutFlg = false;
            bPrnDelayEn = false;
            bHadPrn = true;
        }
    }
    else
    {
        *fbp_PCOutFlg = false;
        bPrnDelayEn = false;
        bHadPrn = false;      //�ָ��Ѵ�ӡ��־
    }
    *fbp_RCDelayFlg = ((bPrnDelayEn)||(*fbp_PCOutFlg)) ? true : false;
}

/*********************************************************************
��������: void RCCU_Config(void)
��    ��: ϵͳʱ������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2010-12-3
��    ��:                      ʱ    ��: 
*********************************************************************/
void RCCU_Config(void)
{
    RCCU_Div2Config(ENABLE);                        // ʹ��2��Ƶ//RCCU_CFR�е�DIV2����λ��λ
    RCCU_MCLKConfig(RCCU_DEFAULT);                  // Configure MCLK = RCLK�ں˺ʹ洢��ʱ��
    RCCU_PCLK1Config(RCCU_DEFAULT);                  // Configure FCLK = RCLK
    RCCU_PCLK2Config(RCCU_DEFAULT);                  // Configure PCLK = RCLK
    RCCU_PLL1Config(RCCU_PLL1_Mul_16, RCCU_Div_2) ;      // Configure the PLL1 (* 16 , / 2)  
    while(RCCU_FlagStatus(RCCU_PLL1_LOCK) == RESET);// Wait PLL to lock
    RCCU_RCLKSourceConfig(RCCU_PLL1_Output) ;       // Select PLL1_Output as RCLK clock
    //����ʱ��������ϣ�MCLK = FCLK = PCLK = 11.0592/2*16/2 = 44.2368MHz
}

/*********************************************************************
��������: void Flash_All3t(void)
��    ��: �ϵ���˸3��
˵    ��: �����а����ָ��������õĴ���
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-11
��    ��:                      ʱ    ��: 
*********************************************************************/
void Flash_All3t(void)
{
    u16 i, j;
    u32 lu32_Key;
    u16 lu16_PressedKey = 0;
    
    CmpDay();   //��ȡgb_SysClosedFlg������ֵ
FLASH_3T:
    lu32_Key = 0;
    for (i = 0; i <= 5; i++)
    {
        //��˸3��
        if (i % 2 == 0)
            VFD_FillDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);
        else
            VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);
            
        VFD_WriteDispRamArry();
        
        //��˸ʱ���AD��ʼ��δ�ɹ��������ʼ��
        if (!gb_AdOKFlg)
            gb_AdOKFlg = AD_Init(FRAM_ReadDW(FMAddr_AdSamplingRate));            //AD��ʼ��
        
        //��˸ʱ����ADת�������AD���У���ֹ��˸��ɺ�������0Ѹ�ٱ仯����ǰ�����Ĺ���
        //���Ᵽ֤���ݲ�ͬADת������ʱ����ͬ
        for (j = 0; j < 60 * cu8_MultiAd[FRAM_ReadDW(FMAddr_AdSamplingRate) <= 3 ? FRAM_ReadDW(FMAddr_AdSamplingRate) : 1]; j++)
        {
            lu32_Key = Key_Deal();             //������
            OPBuzzer();
            if (lu32_Key != K_NONE)
            {
                switch(lu32_Key)
                {
                    case K_7:       //��7�ż����ݺͻָ�����
                        lu16_PressedKey = K_7;
                        break;
//                    case K_1:       //��1���ָ�����
//                        lu16_PressedKey = K_1;
//                        break;
//                    case K_4:       //��4�����ݲ���
//                        lu16_PressedKey = K_4;
//                        break;
//                    case K_8:       //��8���ٰ�5��������ʾ����
//                        lu16_PressedKey = K_8;
//                        break;
//                    case K_5:       //��8���ٰ�5��������ʾ����
//                        if (lu16_PressedKey == K_8)
//                            lu16_PressedKey = K_5;
//                        else if (lu16_PressedKey == K_5)
//                            lu16_PressedKey = K_5;
//                        else 
//                            lu16_PressedKey = K_NONE;
//                        break;
//                    case K_AZERO_BZERO:     //ͬʱ��AB��������봮����������
//                        lu16_PressedKey = K_AZERO_BZERO;
//                        break;
                    case K_ENTER:   //��ENTER�������ʼ������
                        lu16_PressedKey = K_ENTER;
                        break;
                    default:
                        lu16_PressedKey = K_NONE;
                        break;
                }
            }
            
            if (gb_AdOKFlg)
                AD_GetFedVal(gs_Setup.AdFilterLevel);
            else
                delay(4);
            if (lu16_PressedKey == K_ENTER || lu16_PressedKey == K_7)  //�а�����������ѭ��
                break;
        }
        if (lu16_PressedKey == K_ENTER || lu16_PressedKey == K_7)  //�а�����������ѭ��
            break;
    }
    if (lu16_PressedKey == K_ENTER && gb_SysClosedFlg == false)     //��ʼ��
    {
        gb_KeyLockFlg = false;
        gu32_SystemState = RESETSTATE;
        Reset_Parameter();      //�ָ��������ò���
        lu16_PressedKey = 0;
        goto FLASH_3T;          //������˸3��
    }
    if (lu16_PressedKey == K_7 && gb_SysClosedFlg == false)     //�������ݺͻָ�
    {
        gb_KeyLockFlg = false;
        gu32_SystemState = BORRSTATE;
        BorR_Parameter();      //�������ݺͻָ�����
        lu16_PressedKey = 0;
        goto FLASH_3T;          //������˸3��
    }
}

/*********************************************************************
��������: void System_Init(void)
��    ��: ϵͳ��ʼ��
˵    ��: 
��    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2010-12-3
��    ��:                      ʱ    ��: 
*********************************************************************/
void System_Init(void)
{
    APB_ClockConfig(APB1,ENABLE,APB1_ALL_Periph);
    APB_ClockConfig(APB2,ENABLE,APB2_ALL_Periph);       //������
    RCCU_Config();              //ϵͳʱ������
    IO_Init();      //�������ڳ�ʼ��
    Buzzer_Init();  //��������ʼ��
    RTC_Init();     //ʱ��оƬ��ʼ��
    FRAM_Init();    //FRAMоƬ��ʼ��
    VFD_Init();     //VFD��ʼ��
    VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);
    VFD_WriteDispRamArry();     //����ʾ
    EIC_Init();                 //��ʼ���жϼĴ���
    Timer0_Init();              //��ʱ��0��ʼ����������ʱ��
    Timer1_Init();      //Ԥ��SP2��ֹ��ʹ�ö�ʱ��
    
    gb_AdOKFlg = AD_Init(FRAM_ReadDW(FMAddr_AdSamplingRate));            //AD��ʼ��
    Buzzer_PowerOn();
    EIC_IRQConfig(ENABLE);      //�������ж�
    //��ʱ������ر���
    gu32_GetedRandom = FRAM_ReadDW(FMAddr_GetedRandom);         //��ȡ������Ѿ���ȡ����־
    gu32_ShutDownSwitch = FRAM_ReadDW(FMAddr_ShutDownSwitch);   //��ȡ��ʱ��������
    gu32_ShutDownCode = FRAM_ReadDW(FMAddr_ShutDownCode);       //��ȡ��ʱ��������
    gu32_BuyerID = FRAM_ReadDW(FMAddr_BuyerID);                 //��ȡ�û�ID
    gu32_LeaveDays = FRAM_ReadDW(FMAddr_LeaveDays);             //��ȡʣ������
    gu32_RandomID =  FRAM_ReadDW(FMAddr_RandomID);              //��ȡ���ID
    UpdataGrandShutCode();
    
    gu32_ModelSetSwitch = FRAM_ReadDW(FMAddr_ModelSetSwitch);   //�ͺ��Զ��忪��
    FRAM_ContReadDW(FMAddr_ModelSet0, gu32_ModelArry, 7);        //���ͺ����ú�logo����
    
    Flash_All3t();              //�ϵ���˸
    Init_Parameter();           //��ʼ������
    
    if (Pbus_CheckExt())
    {   //��⵽���ӵ���DP+���ڰ�
        gu8_ExtType = PBUS_UART;
        if (gs_Setup.ComMode_1 != COM_PBUS)
        {   //ͨѶģʽ�ĳ�PROFIBUS
            gs_Setup.ComMode_1 = COM_PBUS;
            FRAM_WriteDW(FMAddr_ComMode_1, gs_Setup.ComMode_1);
        }
    }
    else
    {   //��⵽���ӵ���˫���ڰ�
        gu8_ExtType = UART_UART;
        if (gs_Setup.ComMode_1 == COM_PBUS)
        {   //���֮ǰ��PROFIBUS�İ壬�ĳ�˫���ں�ͨѶ��ʽĬ��ΪPRINT
            gs_Setup.ComMode_1 = COM_PRINT;
            FRAM_WriteDW(FMAddr_ComMode_1, gs_Setup.ComMode_1);
        }
    }
    
    Uart0_Init();                //����0��ʼ��
    Uart1_Init();                //����1��ʼ��
    
    gu32_SystemState = STOPSTATE;
}

///*********************************************************************
//��������: void WorkStateDeal(void)
//��    ��: ��������״̬�µİ�������
//˵    ��: 
//��ڲ���: 
//�� �� ֵ: 
//��    ��: �Ժ���               ʱ    ��: 2010-12-30
//��    ��:                      ʱ    ��: 
//*********************************************************************/
void WorkStateDeal(void)
{
    gu32_KeyCode = Key_Deal();
    //��ֹ����ͨѶ�ж��ж�485s�����ý��պ������жϲ���SPI����GPIO1�˿ڣ����������÷���,VFD_KeyDeal�����л����SPI���ߣ�
    //�п���Ӱ��485S�����Է��ڸú�������    chx 2011-12-29
    if (gb_S485Recieve == true)
    {
        Sio_485_Enable();
        gb_S485Recieve = false;
    }
    OPBuzzer();      //��������������
    switch (gu32_SystemState)
    {
        case RUNSTATE:              //����״̬
            IO_InScan();            //���뿪����ɨ��
            Key_RunState();
            break;
        case STOPSTATE:             //ֹͣ״̬
            IO_InScan();            //���뿪����ɨ��
            Key_StopState();
            break;
        case SETUPSTATE:            //������������
            Key_SetupState();
            break;
        case CALIBSTATE:            //�궨
            Key_CalibState();
            break;
        case BATCHSTATE:            //����������
            Key_BatchState();
            break;
        case TIMESTATE:             //ʱ��
            Key_DateTimeState();
            break;
        case DATESTATE:             //����
            Key_DateTimeState();
            break;
        case RECIPESTATE:           //�䷽����
            Key_RecipeState();
            break;
        case SUMSTATE:              //�ۼƲ�ѯ
            Key_SumState();
            break;
        case IOTESTSTATE:           //IO����
            Key_IoTestState();
            break;
        case IODEFINESTATE:         //IO�Զ���
            Key_IoDefState();
            break;
        case SHUTDOWNSTATE:         //��ʱ��������
            Key_ShutdownState();
            break;
        case DEBUGSTATE:            //���Խ��� ����ƥ��
            Key_DebugState();
            break;
        case PRINTSTATE:            //��ӡ����
            Key_PrintState();
            break;
        default:
            break;
    }
}

/*********************************************************************
��������: void Disp_MainFun(void)
��    ��: ��ѭ����ʾ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-11
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_MainFun(void)
{
    static u32 su32_TimeTemp = 0;
    
    if (gu32_Sys10ms - su32_TimeTemp < 4)  //40msˢ��һ����ʾ v1.01
        return;
    su32_TimeTemp = gu32_Sys10ms;
    VFD_Init();     //VFD��ʼ�� v1.01
    VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);       //����ʾ����
    switch(gu32_SystemState)
    {
        case RUNSTATE:
        case STOPSTATE:
            Disp_StopRunState();         //ֹͣ״̬�µ���ʾ����
            break;
        case RECIPESTATE:
            Disp_RecipeState();          //�䷽����״̬�µ���ʾ����
            break;
        case SETUPSTATE:
            Disp_SetupState();           //������������״̬�µ���ʾ����
            break;
        case IOTESTSTATE:
            Disp_IoTestState();          //����������״̬�µ���ʾ����
            break;
        case IODEFINESTATE:
            Disp_IoDefState();           //�������Զ���״̬�µ���ʾ����
            break;
        case CALIBSTATE:
            Disp_CalibState();           //�궨״̬�µ���ʾ����
            break;
        case TIMESTATE:
            Disp_DateTimeState();            //ʱ������״̬�µ���ʾ����
            break;
        case DATESTATE:
            Disp_DateTimeState();            //��������״̬�µ���ʾ����
            break;
        case SHUTDOWNSTATE:
            Disp_ShutdownState();            //�ػ�״̬�µ���ʾ����
            break;
        case BATCHSTATE:
            Disp_BatchState();           //��������״̬�µ���ʾ����
            break;
        case SUMSTATE:
            Disp_SumState();            //�ۼƲ鿴��ʾ����
            break;
        case DEBUGSTATE:            //���Խ��� ����ƥ��
            Disp_DebugState();
            break;
        case PRINTSTATE:            //��ӡ����
            Disp_PrintState();
            break;
        default:
            break;
    }
    
    if (gb_DispErrorFlg)        //��ʾerror
    {   //���󱨾�ʱ����ʾ��λ
        Disp_Error();           //error������ʾ����
        VFD_CopyMainDispArryToDispRam();    //����ʾ����
        VFD_CopyAuxDispArryToDispRam();     //����ʾ����
        VFD_CopyFlgDispArryToDispRam();     //��־�ƻ���
    }
    else
        VFD_CopyDispArryToDispRam();         //����ʾ���������ʾ����
    VFD_WriteDispRamArry();                    //д��ʾ���� 
}

/*********************************************************************
��������: void Lift_Push_Bag(bool *fbp_LiftStart, bool *fbp_PushStart)
��    ��: ����ƴ�����
˵    ��: 
��ڲ���: *fbp_LiftStart ���������־λ  
          *fbp_PushStart �ƴ�������־λ
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-2-27
��    ��:                      ʱ    ��: 
*********************************************************************/
void Lift_Push_Bag(bool *fbp_LiftStart, bool *fbp_PushStart)
{
    static u32 su32_LiftTimeTemp;
    static bool sb_LiftDelayFlg = false;    //�����ʱ������
    static u32 su32_PushTimeTemp;
    static bool sb_PushDelayFlg = false;    //�ƴ���ʱ������
    
    //�������
    if (*fbp_LiftStart == true)
    {   //��������Чʱ�䲻Ϊ0 �����������
        if (gs_Rec.TimeT10 != 0)
        {
            if (sb_LiftDelayFlg)
            {   //�����ʱ�Ѿ�����
                if (gu32_Sys10ms - su32_LiftTimeTemp >= gs_Rec.TimeT9 * 10)
                {   //�����ʱʱ�䵽����������
                    sb_LiftDelayFlg = false;
                    gb_LiftBagFlg = true;
                    su32_LiftTimeTemp = gu32_Sys10ms;
                }
            }
            else if (gb_LiftBagFlg)
            {   //�������Ѿ���Ч
                if (gu32_Sys10ms - su32_LiftTimeTemp >= gs_Rec.TimeT10 * 10)
                {   //����������
                    gb_LiftBagFlg = false;
                    *fbp_LiftStart = false;
                }
            }
            else
            {
                //�����ʱʱ�䲻Ϊ0������ʱ
                if (gs_Rec.TimeT9 != 0)
                {
                    su32_LiftTimeTemp = gu32_Sys10ms;
                    sb_LiftDelayFlg = true;
                    gb_LiftBagFlg = false;
                }
                else
                {
                    sb_LiftDelayFlg = false;
                    gb_LiftBagFlg = true;
                    su32_LiftTimeTemp = gu32_Sys10ms;
                }
            }
        }
        else
            *fbp_LiftStart = false;
    }
    //�ƴ�����
    if (*fbp_PushStart == true)
    {   //�ƴ������Чʱ�䲻Ϊ0 �����������
        if (gs_Rec.TimeT12 != 0)
        {
            if (sb_PushDelayFlg)
            {   //�ƴ���ʱ�Ѿ�����
                if (gu32_Sys10ms - su32_PushTimeTemp >= gs_Rec.TimeT11 * 10)
                {   //�ƴ���ʱʱ�䵽����������
                    sb_PushDelayFlg = false;
                    gb_PushBagFlg = true;
                    su32_PushTimeTemp = gu32_Sys10ms;
                }
            }
            else if (gb_PushBagFlg)
            {   //�ƴ�����Ѿ���Ч
                if (gu32_Sys10ms - su32_PushTimeTemp >= gs_Rec.TimeT12 * 10)
                {   //�ƴ��������
                    gb_PushBagFlg = false;
                    *fbp_PushStart = false;
                }
            }
            else
            {
                //�ƴ���ʱʱ�䲻Ϊ0������ʱ
                if (gs_Rec.TimeT11 != 0)
                {
                    su32_PushTimeTemp = gu32_Sys10ms;
                    sb_PushDelayFlg = true;
                    gb_PushBagFlg = false;
                }
                else
                {
                    sb_PushDelayFlg = false;
                    gb_PushBagFlg = true;
                    su32_PushTimeTemp = gu32_Sys10ms;
                }
            }
        }
        else
            *fbp_PushStart = false;
    }
}

/*********************************************************************
��������: void SpecialFun(void)
��    ��: ���⹦�ܴ��� ��ѭ���� ������������ͬ�����еĹ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: chx                  ʱ    ��: 2015-2-3
��    ��:                      ʱ    ��: 
*********************************************************************/
void SpecialFun(void)
{
    //�ֶ�ж���ۼƵ���ؼ��㴦��
    DiscSumDeal();
    //�Ĵ�����
    FlapBag();
    //���ɴ�����
    Deal_Pack();
    //������ƴ��Ĵ���
    Lift_Push_Bag(&gb_LiftBagStartFlg, &gb_PushBagStartFlg);
    //���봦��
    PrintCodeFun(gs_Rec.PrintCodeSwitch,    //���뿪��                                    
                 gs_Rec.PrintCodeTime,      //����ʱ��                                    
                 gs_Rec.PrintCodeRunDelay,  //����������ʱ                                
                 gu32_Sys10ms,            //ϵͳʱ�䣬��λ10ms                          
                 gb_PackFlg,              //�д����                                    
                 &gb_StopPrintCodeFlg,    //ֹͣ���룬�����ڴ���ʱֹͣ�ź�����          
                 &gb_PrintCodeOutFlg,     //���뿪�����������                          
                 &gb_PrintCodeIngFlg);    //����������ʱ��������ʱ��Ч����������ж�ϵ�
    ConveyorDeal(&gb_ConveyorStartFlg);
}


int main(void) 
{
    System_Init();          //ϵͳ��ʼ��
    Disp_Version();         //��ʾ�汾��
    do
	{
     	CmpDay();
     	Sys_Closed_Set();
	}while(gb_ReCmpFlg);       //��ʱ�����ж�
//�����ۼ�
//    gu32_SumWt = 123456789;                //�ۼ����� ���15λ
//    gu32_SumNum = 123456789;               //�ۼƴ���
//    gu32_Sp2StopTime01ms = 50000;
//    Sp2StopTimeStart();             //for test
    while(1)
    {
    #ifdef DEBUG_ADCNVT_TIMES
        //������ ����ʾADת������/��
        gu32_AdConventTimes = AD_ConvertTimes(gu32_Sys10ms);
    #endif
        
        //ADת���õ�����ֵ
        GetWt();
        //ͨѶ����
        Commumication();
        //��������״̬�µİ����ȴ���
        WorkStateDeal();
        //��ʾ����
        Disp_MainFun();
        //���⹦�ܴ��� ������������ͬ�����еĹ���
//        SpecialFun();
        //�������������
        if (gu32_SystemState != IOTESTSTATE)
            IO_OutPut();
        //��ʱ�����Ĵ���
//        Auto_ShutDown();
    }
}
