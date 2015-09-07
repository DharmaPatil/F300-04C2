/*
Description:            �Ǳ������غ����������д����ʼ���Ȳ���
Author:                 �Ժ���
Date:                   2013-12-9
*/



#include "include.h"
#include "Kalman.h"

/*����һ����������������ж��Ǳ��Ƿ��б���ʼ�������б��ݹ���
�����ʼ�����򱸷ݹ����ڶ�Ӧ��ַ�д������*/
#define INITED  0x12345678  

bool  gb_ExitFlg;              //�˳��ָ���������
u8 gu8_ResetInTime;              //�ָ�����������������������
u8 gu8_BorRInTime;              //���ݻָ���������������������
u8 gu8_ModelSetInTime;          //�ͺ��Զ�����������������
u32 gu32_ModelSetSwitch;            //�ͺ��Զ��忪��
u32 gu32_ModelArry[7];              //�ͺ���ʾ����������� 0-logo���أ�1~6��6λ���ͺ���ʾ



/*********************************************************************
��������: void Read_SetupData(void)
��    ��: ��ȡ����������ز���ֵ
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void Read_SetupData(void)
{
    u8 i;
    //��ȡgs_Setup�ı�������
    i = sizeof(gs_Setup) / sizeof(gs_Setup.TareMode);
    
    gu32_SetupCodeSwitch = FRAM_ReadDW(FMAddr_SetupCodeSwitch); //�������������뱣������
    gu32_SetupCode = FRAM_ReadDW(FMAddr_SetupCode);     //��������������
    FRAM_ContReadDW(FMAddr_TareMode, &gs_Setup.TareMode, i);       //��������������
    gu32_GSDType = FRAM_ReadDW(FMAddr_GSDType);     //��GSD�ļ�����
    gu32_FollowMasterSwitch = FRAM_ReadDW(FMAddr_FollowMasterSwitch);     //���������궨��������վ���ƿ���
    gu32_IoTestCodeSwitch = FRAM_ReadDW(FMAddr_IoTestCodeSwitch);     //�����������Բ�������
    gu32_IoTestCode = FRAM_ReadDW(FMAddr_IoTestCode);     //�����������Բ�������
}

/*********************************************************************
��������: void Read_SumData(void)
��    ��: ���ۼ������ʹ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-15
��    ��:                      ʱ    ��: 
*********************************************************************/
void Read_SumData(void)
{
    u8 i;
    
    for (i = 1; i <= 20; i++)
    {
        gu32_SumWt_Rec[i] = FRAM_ReadDW(FMAddr_SumWt_1 + i * 4);
        gu32_SumNum_Rec[i] = FRAM_ReadDW(FMAddr_SumNum_1 + i * 4);
    }
    
    //�ۼ��������������շ����ۼ�
    gu64_SumWt = FRAM_Read8Byte(FMAddr_SumWt);
    gu32_SumNum = FRAM_ReadDW(FMAddr_SumNum);
    
//    gu64_SumWt = 10000203; //for test
//    gu32_SumNum = 987654321;    //for test
    
    //��ȡ��ʾ��4�ֽڵ������Ͷ�Ӧ�ĵ�λ
    if (GetSumWt_u32(gu64_SumWt,      //��Ҫת���ĵ�ǰ8�ֽ���������
                     gs_Cal.Unit,     //��ǰ������Ӧ�ĵ�λ
                     gs_Cal.Point,    //��ǰ������Ӧ��С����
                     &gu32_SumWt,     //�õ���4�ֽ�Ŀ������
                     &gu32_SumWtUnit,   //Ŀ�������ĵ�λ
                     &gu32_SumWtPoint   //Ŀ��������С����
                    ))
    {
        ClrSum();
    }
}

/*********************************************************************
��������: void Read_RecipeData(u32 ful_RecNum)
��    ��: ��ȡ�䷽������ز���ֵ
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void Read_RecipeData(u32 ful_RecNum)
{
    u8 i;
    //��ȡgs_Cal�ı�������
    i = sizeof(gs_Rec) / sizeof(gs_Rec.Target);
    
    gu32_RecCodeSwitch = FRAM_ReadDW(FMAddr_RecCodeSwitch); //���䷽�������뱣������
    gu32_RecCode = FRAM_ReadDW(FMAddr_RecCode);     //���䷽��������
    
    FRAM_ContReadDW(FMAddr_Target(ful_RecNum), &gs_Rec.Target, i);       //���䷽��������
}

/*********************************************************************
��������: void Read_CalibData(void)
��    ��: ��ȡ�궨��ز���ֵ
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-13
��    ��:                      ʱ    ��: 
*********************************************************************/
void Read_CalibData(void)
{
    u8 i;
    //��ȡgs_Cal�ı�������
    i = sizeof(gs_Cal) / sizeof(gs_Cal.Unit) - 1;   //ע��ADK������ ����-1
    
    gu32_CalCode = FRAM_ReadDW(FMAddr_CalCode);     //���궨����
    FRAM_ContReadDW(FMAddr_CalUnit, &gs_Cal.Unit, i);       //���궨��������
    gs_Cal.ADK = FRAM_ReadFloat(FMAddr_CalADK);
}

/*********************************************************************
��������: void Read_IoDefData(void)
��    ��: ��ȡ�������Զ�������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-25
��    ��:                      ʱ    ��: 
*********************************************************************/
void Read_IoDefData(void)
{
    //��8�������Զ����12������Զ�����뵽��Ӧ��������
    FRAM_ContReadDW(FMAddr_IoInBuf1, &gu32_IoInBuf_BINYES[1] , 8);
    FRAM_ContReadDW(FMAddr_IoOutBuf1, &gu32_IoOutBuf_BINYES[1], 12);
    //BINNO��8�������Զ����12������Զ�����뵽��Ӧ��������
    FRAM_ContReadDW( FMAddr_IoInBuf1_BINNO, &gu32_IoInBuf_BINNO[1] , 8 );
    FRAM_ContReadDW( FMAddr_IoOutBuf1_BINNO, &gu32_IoOutBuf_BINNO[1], 12 );
    //BIN_2��8�������Զ����12������Զ�����뵽��Ӧ��������
    FRAM_ContReadDW( FMAddr_IoInBuf1_BIN_2, &gu32_IoInBuf_BIN_2[1] , 8 );
    FRAM_ContReadDW( FMAddr_IoOutBuf1_BIN_2, &gu32_IoOutBuf_BIN_2[1], 12 );
    //NO_2��8�������Զ����12������Զ�����뵽��Ӧ��������
    FRAM_ContReadDW( FMAddr_IoInBuf1_NO_2, &gu32_IoInBuf_NO_2[1] , 8 );
    FRAM_ContReadDW( FMAddr_IoOutBuf1_NO_2, &gu32_IoOutBuf_NO_2[1], 12 );
    
    gu32_IoDefCodeSwitch = FRAM_ReadDW(FMAddr_IoDefCodeSwitch);     //���������Զ������뿪��
    gu32_IoDefCode = FRAM_ReadDW(FMAddr_IoDefCode);     //���������Զ�������
    IoDefPointerRefresh(gs_Setup.TareMode);  //���¿������Զ�������ָ��
}

/*********************************************************************
��������: void Init_Parameter(void)
��    ��: ����ֵ�Ķ�ȡ
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-2-18
��    ��:                      ʱ    ��: 
*********************************************************************/
void Init_Parameter(void)
{
    gb_ConveyorRunFlg = false;
    gb_FlashFlg = false;
    gb_ZeroKeyFlg = false;
    gb_ForceClrZeroFlg = false;
    gb_DispErrorFlg = false;
    gb_FlapWaitFlg = false;         //�Ĵ���ʱ������
    gb_FlapStartFlg = false;        //�Ĵ��Ѿ�����
    gb_FlapBagOutFlg = false;       //�Ĵ�����ź�
    gb_FlapAllHighFlg = false;
    gb_LackFlg = false;
    gb_FillFlg = false;
    gb_ExtFlapBagFlg = false;
    gb_AddFlapBagFlg = false;
    gb_PrintAllRecFlg_UART0 = false;
    gb_PrintAllRecFlg_UART1 = false;
    gb_DiscEnableOutFlg = true;
    gu16_MbIOOutInit = 0;
    gb_GoFlg = false;
    gu16_MbIoTestSwitch = 0;
    gu16_MbIoOutTest = 0;
    gb_BatchOverFlg = false;
    gu8_DispErrorNumber = 0;
//    gu8_DispErrorNumberBackup = 0;
    gb_DiscFlg = false; 
    gb_PackFlg = false;
    gb_JSDDelayFlg = false;                 //������ɴ���ʱ��־
    gb_SDDelayFlg = false;
    gb_SDBDelayFlg = false;
    gb_DispNetFlg = false;  
    gb_DiscShakeFlg = false;     //A��ж��������
    gb_DiscShakeStartFlg = false;     //A��ж�����ʼ
    gb_Error9Flg = false;                    //A��ж��δ�ɾ�����
    gb_AddFlapBagFlg = false;                //A�������Ĵ���־λ
    gb_ExtFlapBagFlg = false;                //A�����Ĵ������־λ
    gb_TareGetedFlg = false;
    gb_PrintCodeIngFlg = false;
    gu16_MbIoTestSwitch = 0;
    gu16_MbIoInState = 0;           //����������״̬
    gu16_MbIoOutState = 0;          //���������״̬
    gb_MbKeyLockFlg = false;
    gb_EndFlg = false;              //��������
    gb_DebugUart1 = false;
    gb_DebugUart0 = false;
    gb_LiftBagStartFlg = false;
    gb_PushBagStartFlg = false;
    gb_ConveyorStartFlg = false;
    gb_AddingOutFlg = false;
    
    gu32_BatchTime = FRAM_ReadDW(FMAddr_BatchTime);      //������
    gu32_BatchTimeCnt = FRAM_ReadDW(FMAddr_BatchTimeCnt);      //ʣ��������
    Read_CalibData();           //���궨����ֵ
    Read_SetupData();           //����������ֵ
    gu32_RecNum = FRAM_ReadDW(FMAddr_RecNum);        //���䷽��
    Read_RecipeData(gu32_RecNum);                  //���䷽������ֵ
    Read_IoDefData();           //���������Զ���
    Read_SumData();             //���ۼƲ���
    kalman1_init(&gs_KalmanState, XLAST, PLAST);
}

/*********************************************************************
��������: void Reset_Cal(void)
��    ��: �궨�����ָ���������
˵    ��: ����һ����ʼ���������飬��������д��˫�ֵĺ���д���Ӧ�����Ĵ洢��ַ����
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-30
��    ��:                      ʱ    ��: 
*********************************************************************/
void Reset_Cal(void)
{                            //���� ��λ  С���� �ֶ� ����  ������ ��� 
    const u32 cu32_CalData[] = {0,  UNIT_kg, 2,   1,  10000, 2,    0x2000};
    FRAM_ContWriteDW(FMAddr_CalCode, (u32 *)cu32_CalData, sizeof(cu32_CalData)/sizeof(cu32_CalData[0]));           //����������д���Ӧ�Ĵ洢��ַ
    //�����ֵ
    FRAM_WriteFloat(FMAddr_CalADK, 10.0f);
}

/*********************************************************************
��������: void Reset_Setup(bool fb_MbFlg)
��    ��: �ָ���������
˵    ��: ����һ����ʼ���������飬��������д��˫�ֵĺ���д���Ӧ�����Ĵ洢��ַ����
��ڲ���: fb_MbFlg =false���̳�ʼ��  =true ModbusͨѶ��ʼ��������ʼ��������ز���
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-30
��    ��:                      ʱ    ��: 
*********************************************************************/
void Reset_Setup(bool fb_MbFlg)
{                             
    const u32 cu32_SetupData[] = {
    //���뿪�� ����
       0,       0,
    //����ģʽ ë���� ���Ϸ�ʽ ������ ���ȳ�ʱ ��������
       BINYES,  NET,     CO,    0,       0,       0,
    //���㷶Χ ���ȷ�Χ ׷�㷶Χ �˲�  ж���˲� ��ֵ�˲� AD���� ��չ�ӿ�����
       50,       1,      1,       5,    3,       4,       0,      0,        
    //�ϵ����� ж���ۼ� ��ֵ���� �ۼƲ�ѯ���� �ۼ�������� �ۼ���չ
       0,       0,       0,       0,            0,          1,
    //ͨѶ��ʽ �Ӻ� ������ У��     HiLo �Զ���ӡ ��ӡ���� ��ӡ����    ��ӡ��ֽ����
      COM_PRINT,  1,   2,  P_18N1,   0,   0,       1,      PRINTER_ENG,      3,
    //ͨѶ��ʽ �Ӻ� ������ У��     HiLo �Զ���ӡ ��ӡ���� ��ӡ���� ��ӡ��ֽ����
      COM_CONT, 1,   2,    P_17E1,   0,   0,       1,      PRINTER_ENG,      3,
    //�޶�ͬʱ�ɴ� ���ͻ�������ʱ ���ͻ�����ʱ�� �ٴμд���ʱ����
      0,            5,             40,            20,
    //����Ӧ���� SP2 ������� �ȼ� ʱ��ֲ� SP3 ������� ���� Ԥ�� Ԥ��
      0,         0,  100,     1,   500,     0,  100,     1,   0,   0,
    };
    
    u8 j;
    if (fb_MbFlg)   //ModbusͨѶ����ʼ���������
    {   //���� B1 B2 B3
        FRAM_ContWriteDW(FMAddr_SetupCodeSwitch, (u32 *)cu32_SetupData, 22);           //����������д���Ӧ�Ĵ洢��ַ
        //B6
        FRAM_ContWriteDW(FMAddr_No2PackOffTwoSwitch, (u32 *)&cu32_SetupData[40], 4);           //����������д���Ӧ�Ĵ洢��ַ
    }
    else    //�ָ�����
    {
        j = sizeof(cu32_SetupData) / sizeof(cu32_SetupData[0]);
        FRAM_ContWriteDW(FMAddr_SetupCodeSwitch, (u32 *)cu32_SetupData, j);           //����������д���Ӧ�Ĵ洢��ַ
    }
    
    FRAM_WriteDW(FMAddr_GSDType, 0);               //GSD�ļ�
    FRAM_WriteDW(FMAddr_FollowMasterSwitch, 0);               //���������궨��������վ���ƿ���
    FRAM_WriteDW(FMAddr_IoTestCodeSwitch, 0);         //IO�������뿪��
    FRAM_WriteDW(FMAddr_IoTestCode, 0);               //IO��������
}

/*********************************************************************
��������: void Reset_Rec(void)
��    ��: �ָ��䷽����
˵    ��: ����һ����ʼ���������飬��������д��˫�ֵĺ���д���Ӧ�����Ĵ洢��ַ����
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-4-12
��    ��:                      ʱ    ��: 
*********************************************************************/
void Reset_Rec(void)
{
    const u32 cu32_RecData[] = {
    //Ŀ�� ��Ͷ ��Ͷ ��� ���� �Ĵ���ʼ
       0,  0,    0,   0,   0,   0,
    //��Ƿ���� Over under ��Ƿ��ͣ
       0,      0,   0,    0,
    //T1 T2A T3 T4 T5 T6 T7 T2B T9 T10 T11 T12
       5,9,  5,  5, 5, 5, 5, 9, 0,  0, 0,  0,
    //�������� ���� ��Χ ����
       0,      1,   20,  2,
    //�Ĵ�ģʽ �д��� ����� �Ĵ���ʱ ��Чʱ�� ��Чʱ�� �����Ĵ�ʱ��
       0,      0,     4,      5,       5,        5,         0,
    //СͶ�㶯 ��Чʱ�� ��Чʱ��
       0,      5,       5,
    //������ϴ���
       1,
    //ж����� ж����Ч ��ʱ�� ��ʱ�� ����
       0,      20,      5,     5,     10,
    //���뿪�� ������ʱ ����ʱ�� ����ж��
       0,      5,       5,       0
       };
       
    u8 i, j;
    
    j = sizeof(cu32_RecData) / sizeof(cu32_RecData[0]);
    for (i = 1; i <= 20; i++)            //�ָ������䷽����
        FRAM_ContWriteDW(FMAddr_Target(i), (u32 *)cu32_RecData, j);           //����������д���Ӧ�Ĵ洢��ַ
    FRAM_WriteDW(FMAddr_RecNum, 1);                   //�ָ��䷽��Ϊ1
    FRAM_WriteDW(FMAddr_RecCodeSwitch, 0);            //�䷽�������뿪�عر�
    FRAM_WriteDW(FMAddr_RecCode, 0);                  //�䷽��������000000
}

/*********************************************************************
��������: void Reset_IoDef(void)
��    ��: 
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-4-12
��    ��:                      ʱ    ��: 
*********************************************************************/
void Reset_IoDef(void)
{
    //��������������������������
    const u32 cu32_IoDefDatap[] = {1, 2, 3, 4, 5, 6, 7, 8,
                                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                   1, 2, 3, 4, 5, 6, 7, 8,
                                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                   1, 2, 3, 4, 5, 6, 7, 23,
                                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 21,
                                   1, 2, 3, 4, 5, 6, 7, 23,
                                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 22, 21
                                   };
    FRAM_ContWriteDW(FMAddr_IoInBuf1, (u32 *)cu32_IoDefDatap, sizeof(cu32_IoDefDatap)/sizeof(cu32_IoDefDatap[0]));           //����������д���Ӧ�Ĵ洢��ַ
    FRAM_WriteDW(FMAddr_IoDefCodeSwitch, 1);          //IO�Զ������뿪��
    FRAM_WriteDW(FMAddr_IoDefCode, 0);                //IO�Զ�������
}

/*********************************************************************
��������: void Reset_All(void)
��    ��: �ָ�ȫ������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-4-12
��    ��:                      ʱ    ��: 
*********************************************************************/
void Reset_All(void)
{
    if (INITED != FRAM_ReadDW(FMAddr_HadBeenInited))
    {   //�ж��Ǳ��Ƿ��б���ʼ���������û����������´���
        
        //��ʱ����������������Ϊ000000,���عر�
        gu32_ShutDownSwitch = 0;
        FRAM_WriteDW(FMAddr_ShutDownCode, 0);
        gu32_ShutDownCode = 0;
        FRAM_WriteDW(FMAddr_ShutDownSwitch, 0);
        //����ۼ�����
        ClrSum();
        //�ر��ͺ��Զ��幦��
        gu32_ModelSetSwitch = 0;
        gu32_ModelArry[0] = 1;
        gu32_ModelArry[1] = 8;
        gu32_ModelArry[2] = 8;
        gu32_ModelArry[3] = 0;
        gu32_ModelArry[4] = 4;
        gu32_ModelArry[5] = 12;
        gu32_ModelArry[6] = 2;
        FRAM_WriteDW(FMAddr_ModelSetSwitch, gu32_ModelSetSwitch);   //�ͺ��Զ��忪��
        FRAM_ContWriteDW(FMAddr_ModelSet0, gu32_ModelArry, 7);        //���ͺ����ú�logo����
        
        //д���ʼ���жϵ�ַ����INITED����ʾ�Ѿ����й���ʼ��
        FRAM_WriteDW(FMAddr_HadBeenInited, INITED);
    }
    
    Reset_Cal();
    Reset_Setup(false);
    Reset_Rec();
    Reset_IoDef();
    
    FRAM_WriteDW(FMAddr_BatchTime, 0);
    FRAM_WriteDW(FMAddr_BatchTimeCnt, 0);
}

/*********************************************************************
��������: void Key_ResetState(void)
��    ��: �ָ��������ò���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-30
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_ResetState(void)
{
    u8 luc_DataLen = 0;      //��������λ��
    
    if (gu32_KeyCode == K_RIGHT || gu32_KeyCode == K_INC)
    {
        if (gu8_Item != RESET_CHECKCODE)        //��������ʱ������������һ��
            gu8_Item++;
        if (gu8_Item > RESET_IODEF)
            gu8_Item = RESET_ALL;
    }
    switch (gu8_Item)
    {
        case RESET_CHECKCODE:       //��������
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            break;
        default:
            luc_DataLen = 0;
            break;
    }
    
    //��������
    ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
    
    //ÿһ���µİ����������
    if(gu32_KeyCode ==K_ESC)           //�˳���������
    {
        gu32_SystemState = STOPSTATE;
        gb_FirstNumKeyFlg = true;
        gu8_Item = 0;
        gb_FlashFlg = false;
        gb_ExitFlg = true;            //�˳��ָ���������״̬
    }
    if (gu32_KeyCode == K_ENTER)         //ȷ�ϼ�����
    {
        gb_FirstNumKeyFlg = true;
        gu8_DataInLength = 0;
        switch(gu8_Item)
        {
            case RESET_CHECKCODE:       //��������
                gu8_ResetInTime++;
                if (ulParameter.ul == RESETCODE)        //����������ڻָ�������������
                {
                    gu8_ResetInTime = 0;            //�����������������������
                    ulParameter.ul = 0;
                }
                else if (gu8_ResetInTime >= 3)      //����������󳬹�3��
                {
                    ErrorDeal(4, 0);        //��ʾerror4
                    Disp_Error();
                    VFD_CopyDispArryToDispRam();         //����ʾ���������ʾ����
                    VFD_WriteDispRamArry();                    //д��ʾ���� 
                    while(1);
                }
                else            //�������������ʾerror
                {
                    ErrorDeal(0, 100);
                    ulParameter.ul = 0xFFFFFFFF;
                    return;
                }
                break;
            case RESET_ALL:
                Reset_All();            //�ָ�ȫ������
                Disp_Delay();
                gu32_SystemState = STOPSTATE;
                gb_FirstNumKeyFlg = true;
                gu8_Item = 0;
                gb_FlashFlg = false;
                gb_ExitFlg = true;            //�˳��ָ���������״̬
                break;
            case RESET_CAL:
                Reset_Cal();            //�ָ��궨����
                Disp_Delay();
                break;
            case RESET_SET:
                Reset_Setup(false);          //�ָ���������
                Disp_Delay();
                break;
            case RESET_REC:
                Reset_Rec();            //�ָ��䷽����
                Disp_Delay();
                break;
            case RESET_IODEF:
                Reset_IoDef();          //�ָ��������Զ���
                Disp_Delay();
                break;
            default:
                break;
        }
        if (++gu8_Item > RESET_IODEF)
            gu8_Item = RESET_ALL;
    }
}

/*********************************************************************
��������: void Disp_ResetState(void)
��    ��: �ָ�����������ʾ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-30
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_ResetState(void)
{
    gb_FlashFlg = false;
    
    if (gu8_Item != RESET_CHECKCODE)
        VFD_AuxDisp_Str("ini");

    switch (gu8_Item)
    {
        case RESET_CHECKCODE:       //��������
            Disp_InputCode();       //����������ʾ����
            gb_FlashFlg = true;
            VFD_AuxDisp_Str("INI");
            Flash_MainDisp(0x01);                //��˸����
            break;
        case RESET_CAL:
            VFD_MainDisp_Str("   CAL");
            break;
        case RESET_SET:
            VFD_MainDisp_Str("   SET");
            break;
        case RESET_REC:
            VFD_MainDisp_Str("   REC");
            break;
        case RESET_IODEF:
            VFD_MainDisp_Str("    IO");
            break;
        case RESET_ALL:
            VFD_MainDisp_Str("   ALL");
            break;
    }
}

/*********************************************************************
��������: void Reset_Parameter(void)
��    ��: �ָ���������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-4-11
��    ��:                      ʱ    ��: 
*********************************************************************/
void Reset_Parameter(void)
{
    gb_ExitFlg = false;
    ulParameter.ul = 0xFFFFFFFF;
    gu32_KeyCode = 0;
    gu8_Item = 0;
    gu8_DataInLength = 0;

    while (gb_ExitFlg == false)
    {
        gu32_KeyCode = Key_Deal();      //������
        OPBuzzer();
        Key_ResetState();               //�ָ��������ò���
        VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);       //����ʾ����
        Disp_ResetState();              //�ָ�����������ʾ����
        if (gb_DispErrorFlg)        //��ʾerrorʱ��������������ʾ����
            Disp_Error();           //error������ʾ����
        VFD_CopyDispArryToDispRam();    //����ʾ���������ʾ����
        VFD_WriteDispRamArry();         //д��ʾ���� 
    }
}

/*********************************************************************
��������: Disp_Delay
��    ��: �������ݺͻָ�ʱ�Ľ�����ʾ
˵    ��: 
��    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2010-8-16
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_Delay(void)
{
    u8 i;
    
    memset(gu8_MainDispArray, SEG_BLK, sizeof(gu8_MainDispArray));
    for (i = 0; i < 6; i++)
    {
        gu8_MainDispArray[i] = SEG_SIG;
        VFD_CopyDispArryToDispRam();         //����ʾ���������ʾ����
        VFD_WriteDispRamArry();                    //д��ʾ���� 
        delay(60);
    }
}

/*********************************************************************
��������: void Backup_Parameter(void)
��    ��: �����Ǳ����
˵    ��: ���������������䷽���������Զ���
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-5-27
��    ��:                      ʱ    ��: 
*********************************************************************/
void Backup_Parameter(void)
{
    u32 lu32_ParameterAddr = SETUP_BASE;        //���ò�����ʼ��ַ
    u32 lu32_BackupAddr = BACKUPSTART;          //������ʼ��ַ
    u32 i = (BACKUPSTART - SETUP_BASE) / 4;         //�������ݵ�����
    
    for (; i != 0; i--, lu32_ParameterAddr += 4, lu32_BackupAddr += 4)
    {
        FRAM_WriteDW_Backup(lu32_BackupAddr, FRAM_ReadDW(lu32_ParameterAddr));
    }
    //�����ݵ�ʱ����б��棬������INITED�����Ѿ����й�����
    RTC_ReadDateTime(&SysDataTime);      //��ϵͳ���ں�ʱ��
    FRAM_WriteDW(FMAddr_BackupYear, SysDataTime.year);
    FRAM_WriteDW(FMAddr_BackupMonth, SysDataTime.month);
    FRAM_WriteDW(FMAddr_BackupDate, SysDataTime.date);
    FRAM_WriteDW(FMAddr_Backuped, INITED);
}

/*********************************************************************
��������: void Reload_Parameter(void)
��    ��: �ָ��Ǳ����
˵    ��: ���������������䷽���������Զ���
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-5-28
��    ��:                      ʱ    ��: 
*********************************************************************/
void Reload_Parameter(void)
{
    u32 lu32_ParameterAddr = SETUP_BASE;    //���ò�����ʼ��ַ
    u32 lu32_BackupAddr = BACKUPSTART;        //������ʼ��ַ
    u32 i = (BACKUPSTART - SETUP_BASE) / 4;         //�������ݵ�����
    
    for (; i != 0; i--, lu32_ParameterAddr += 4, lu32_BackupAddr += 4)
    {
        FRAM_WriteDW(lu32_ParameterAddr, FRAM_ReadDW(lu32_BackupAddr));
    }
}

/*********************************************************************
��������: void Key_BorRState(void)
��    ��: ���ݺͻָ���������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-31
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_BorRState(void)
{
    u8 luc_DataLen = 0;      //��������λ��
    
    switch (gu8_Item)
    {
        case BORR_RELOAD_CHECKCODE:       //��������
        case BORR_BACKUP_CHECKCODE:
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            //��������
            ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
            break;
        default:
            break;
    }
    
    //ÿһ���µİ����������
    switch (gu32_KeyCode)
    {
        case K_ESC:
            if (gu8_Item == BORR_RELOAD || gu8_Item == BORR_BACKUP)
            {
                gu32_SystemState = STOPSTATE;
                gb_FirstNumKeyFlg = true;
                gu8_Item = 0;
                gb_FlashFlg = false;
                gb_ExitFlg = true;            //�˳��ָ���������״̬
            }
            else
                gu8_Item--;
            if (gu8_Item== BORR_RELOAD_DATE ||
                gu8_Item == BORR_BACKUP_DATE)      //����������ʾ
            {
                ulParameter.ul = FRAM_ReadDW(FMAddr_Backuped);
                if (ulParameter.ul == INITED)   //�б��ݹ�
                {   //�������ݵ���������Ϣ
                    SysDataTime.year = FRAM_ReadDW(FMAddr_BackupYear);
                    SysDataTime.month = FRAM_ReadDW(FMAddr_BackupMonth);
                    SysDataTime.date = FRAM_ReadDW(FMAddr_BackupDate);
                }
            }
            break;
        case K_ENTER:
            if (gu8_Item == BORR_BACKUP_CHECKCODE || gu8_Item == BORR_RELOAD_CHECKCODE)
            {
                gu8_BorRInTime++;
                if ((ulParameter.ul == BACKUPCODE && gu8_Item == BORR_BACKUP_CHECKCODE) ||
                    (ulParameter.ul == RELOADCODE && gu8_Item == BORR_RELOAD_CHECKCODE))       //����������ڻָ�������������
                {
                    if (gu8_Item == BORR_BACKUP_CHECKCODE)
                        Backup_Parameter();
                    else
                        Reload_Parameter();
                    Disp_Delay();
                    gu8_BorRInTime = 0;            //�����������������������
                    ulParameter.ul = 0;
                    gu8_Item -= 2;
                }
                else if (gu8_BorRInTime >= 3)      //����������󳬹�3��
                {
                    ErrorDeal(4, 0);        //��ʾerror4
                    Disp_Error();
                    VFD_CopyDispArryToDispRam();         //����ʾ���������ʾ����
                    VFD_WriteDispRamArry();                    //д��ʾ���� 
                    while(1);
                }
                else            //�������������ʾerror
                {
                    ErrorDeal(0, 100);
                    ulParameter.ul = 0xFFFFFFFF;
                    return;
                }
                break;
            }
            else
                gu8_Item++;
            gu8_DataInLength = 0;
            if (gu8_Item== BORR_RELOAD_DATE ||
                gu8_Item == BORR_BACKUP_DATE)      //����������ʾ
            {
                ulParameter.ul = FRAM_ReadDW(FMAddr_Backuped);
                if (ulParameter.ul == INITED)   //�б��ݹ�
                {   //�������ݵ���������Ϣ
                    SysDataTime.year = FRAM_ReadDW(FMAddr_BackupYear);
                    SysDataTime.month = FRAM_ReadDW(FMAddr_BackupMonth);
                    SysDataTime.date = FRAM_ReadDW(FMAddr_BackupDate);
                }
            }
            break;
        case K_RIGHT:
            if (gu8_Item == BORR_RELOAD)
                gu8_Item = BORR_BACKUP;
            else if (gu8_Item == BORR_BACKUP)
                gu8_Item = BORR_RELOAD;
            break;
    }
//    if(gu32_KeyCode ==K_ESC)           //�˳���������
//    {
//        gu32_SystemState = STOPSTATE;
//        gb_FirstNumKeyFlg = true;
//        gu8_Item = 0;
//        gb_FlashFlg = false;
//        gb_ExitFlg = true;            //�˳��ָ���������״̬
//    }
//    if (gu32_KeyCode == K_ENTER)         //ȷ�ϼ�����
//    {
//        gb_FirstNumKeyFlg = true;
//        gu8_DataInLength = 0;
//        switch(gu8_Item)
//        {
//            case BORR_CHECKCODE:       //��������
//                gu8_BorRInTime++;
//                if ((ulParameter.ul == BACKUPCODE && gu32_SystemState == BORRSTATE) ||
//                    (ulParameter.ul == RELOADCODE && gu32_SystemState == RELOADSTATE))       //����������ڻָ�������������
//                {
//                    gu8_BorRInTime = 0;            //�����������������������
//                    ulParameter.ul = 0;
//                    gu8_Item = BORR_DEAL;
//                }
//                else if (gu8_BorRInTime >= 3)      //����������󳬹�3��
//                {
//                    ErrorDeal(MDP_A, 4, 0);        //��ʾerror4
//                    Disp_Error();
//                    VFD_WriteUpMainDispArry(g_UCurrDisp.ALL);         //����ʾ���������ʾ����
//                    VFD_Write();                    //д��ʾ����
//                    while(1);
//                }
//                else            //�������������ʾerror
//                {
//                    ErrorDeal(MDP_A, 0, 100);
//                    ulParameter.ul = 0xFFFFFFFF;
//                    return;
//                }
//                break;
//            case BORR_DEAL:         //ִ�в������ݻ�ָ�
//                if (gu32_SystemState == BORRSTATE)
//                {
//                    Backup_Parameter();         //���ݲ���
//                    BuzzerOff();
//                    Disp_Delay();
//                }
//                else
//                {
//                    Reload_Parameter();         //�ָ�����'
//                    BuzzerOff();
//                    Disp_Delay();
//                }
//                
//                gu32_SystemState = STOPSTATE;
//                gb_FirstNumKeyFlg = true;
//                gu8_Item = 0;
//                gb_FlashFlg = false;
//                gb_ExitFlg = true;            //�˳��ָ���������״̬
//                break;
//            default:
//                break;
//        }
//    }
}

/*********************************************************************
��������: void Disp_BorRState(void)
��    ��: ���ݺͻָ���������ʾ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-31
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_BorRState(void)
{
    gb_FlashFlg = false;

    switch  (gu8_Item)
    {
        case BORR_RELOAD_CHECKCODE:       //��������
        case BORR_BACKUP_CHECKCODE:
            Disp_InputCode();       //����������ʾ����
            gb_FlashFlg = true;
            Flash_MainDisp(0x01);                //��˸����
            break;
        case BORR_RELOAD:
            VFD_MainDisp_Str("   RLD");
            VFD_AuxDisp_Str("-1-");
            break;
        case BORR_RELOAD_DATE:
            if (ulParameter.ul == INITED)
            {
                RTC_sDatetoASC(&SysDataTime, gu8_MainDispArray);
                VFD_MainDisp_ASCII(gu8_MainDispArray);
                gu8_MainDispArray[1] |= 0x80;
                gu8_MainDispArray[3] |= 0x80;
            }
            else
            VFD_MainDisp_Str("  none");
            VFD_AuxDisp_Str("-1-");
            break;
        case BORR_BACKUP:
            VFD_MainDisp_Str("   BAC");
            VFD_AuxDisp_Str("-2-");
            break;
        case BORR_BACKUP_DATE:
            if (ulParameter.ul == INITED)
            {
                RTC_sDatetoASC(&SysDataTime, gu8_MainDispArray);
                VFD_MainDisp_ASCII(gu8_MainDispArray);
                gu8_MainDispArray[1] |= 0x80;
                gu8_MainDispArray[3] |= 0x80;
            }
            else
                VFD_MainDisp_Str("  none");
            VFD_AuxDisp_Str("-2-");
            break;
        default:
            break;
    }
}

/*********************************************************************
��������: void BorR_Parameter(void)
��    ��: 
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-2
��    ��:                      ʱ    ��: 
*********************************************************************/
void BorR_Parameter(void)
{
    gb_ExitFlg = false;
    ulParameter.ul = 0xFFFFFFFF;
    gu32_KeyCode = 0;
    gu8_Item = 0;
    gu8_DataInLength = 0;

    while (gb_ExitFlg == false)
    {
        gu32_KeyCode = Key_Deal();                     //������
        OPBuzzer();
        Key_BorRState();                //���ݺͻָ���������
        VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);       //����ʾ����
        Disp_BorRState();               //���ݺͻָ�������ʾ����
        if (gb_DispErrorFlg)        //��ʾerrorʱ��������������ʾ����
            Disp_Error();           //error������ʾ����
        VFD_CopyDispArryToDispRam();    //����ʾ���������ʾ����
        VFD_WriteDispRamArry();         //д��ʾ���� 
    }
}

/*********************************************************************
��������: void ModelSetStateDeal(void)
��    ��: �ͺ�����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-2
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_ModelSetState(void)
{
    u8 luc_DataLen = 0;      //��������λ��
    
    switch (gu8_Item)
    {
        case MODELSET_CHECKCODE:       //��������
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            //��������
            ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
            break;
        case MODELSET_SWITCH:
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //�����л����á����޸�
            break;
        case MODELSET_SETUP_0:      //��Դ��
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //�����л����á����޸�
            break;
        case MODELSET_SETUP_1:
        case MODELSET_SETUP_2:
        case MODELSET_SETUP_3:
        case MODELSET_SETUP_4:
        case MODELSET_SETUP_5:
        case MODELSET_SETUP_6:
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 36);
            ulParameter.ul = Key_T9Input(ulParameter.ul);        //T9���뷨
            break;
        default:
            luc_DataLen = 0;
            break;
    }
    
    //ÿһ���µİ����������
    switch (gu32_KeyCode)
    {
        case K_RIGHT:
            if (gu8_Item != MODELSET_CHECKCODE && gu8_Item != MODELSET_SWITCH)        //��������ʱ������������һ��
            {
                gu8_Item++;
                if (gu8_Item == MODELSET_SETUP_1)
                    gu32_ModelArry[0] = FRAM_ReadDW(FMAddr_ModelSet0);
                if (gu8_Item > MODELSET_SETUP_6)
                    gu8_Item = MODELSET_SETUP_0;
                ulParameter.ul = gu32_ModelArry[gu8_Item-2];
            }
            break;
        case K_ESC:
            gu32_SystemState = STOPSTATE;
            gb_FirstNumKeyFlg = true;
            gu8_Item = 0;
            gb_FlashFlg = false;
            gb_ExitFlg = true;            //�˳��ָ���������״̬
            break;
        case K_ENTER:
            gb_FirstNumKeyFlg = true;
            gu8_DataInLength = 0;
            switch(gu8_Item)
            {
                case MODELSET_CHECKCODE:       //��������
                    gu8_ModelSetInTime++;
                    if (ulParameter.ul == SUPERCODE)        //����������ڻָ�������������
                    {
                        gu8_ModelSetInTime = 0;            //�����������������������
                        ulParameter.ul = gu32_ModelSetSwitch;
                    }
                    else if (gu8_ModelSetInTime >= 3)      //����������󳬹�3��
                    {
                        ErrorDeal(4, 0);        //��ʾerror4
                        Disp_Error();
                        VFD_CopyDispArryToDispRam();         //����ʾ���������ʾ����
                        VFD_WriteDispRamArry();                    //д��ʾ���� 
                        while(1);
                    }
                    else            //�������������ʾerror
                    {
                        ErrorDeal(0, 100);
                        ulParameter.ul = 0xFFFFFFFF;
                        return;
                    }
                    break;
                case MODELSET_SWITCH:
                    gu32_ModelSetSwitch = ulParameter.ul;
                    FRAM_WriteDW(FMAddr_ModelSetSwitch, gu32_ModelSetSwitch);
                    gu32_FlashDelay = gu32_Sys10ms;
                    if (gu32_ModelSetSwitch == 0)
                        return;
                    break;
                case MODELSET_SETUP_0:      //��Դ��
                case MODELSET_SETUP_1:
                case MODELSET_SETUP_2:
                case MODELSET_SETUP_3:
                case MODELSET_SETUP_4:
                case MODELSET_SETUP_5:
                case MODELSET_SETUP_6:
                    gu32_ModelArry[gu8_Item-2] = ulParameter.ul;
                    FRAM_WriteDW(FMAddr_ModelSet0+(gu8_Item-2)*4, gu32_ModelArry[gu8_Item-2]);
                    break;
                default:
                    break;
            }
            if (++gu8_Item > MODELSET_SETUP_6)
            {
                gu8_Item = MODELSET_SETUP_0;
                gu32_FlashDelay = gu32_Sys10ms;
            }
            if (gu8_Item >= MODELSET_SETUP_0)
            {
                ulParameter.ul = FRAM_ReadDW(FMAddr_ModelSet0+(gu8_Item-2)*4);
            }
            break;
    }
}

/*********************************************************************
��������: void Disp_ModelSetState(void)
��    ��: �ͺ�����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-2
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_ModelSetState(void)
{
//    u8 lu8_FlashBit = 0;
    
    gb_FlashFlg = false;
    
    if (gu8_Item >= MODELSET_SETUP_0 && gu8_Item <= MODELSET_SETUP_6)
    {
        gu8_MainDispArray[0] = Value_To_SEG[gu32_ModelArry[1]];
        gu8_MainDispArray[1] = Value_To_SEG[gu32_ModelArry[2]];
        gu8_MainDispArray[2] = Value_To_SEG[gu32_ModelArry[3]];
        gu8_MainDispArray[3] = Value_To_SEG[gu32_ModelArry[4]];
        gu8_MainDispArray[4] = Value_To_SEG[gu32_ModelArry[5]];
        gu8_MainDispArray[5] = Value_To_SEG[gu32_ModelArry[6]];
        if (gu8_Item >= MODELSET_SETUP_1)
            gu8_MainDispArray[gu8_Item-3] = Value_To_SEG[ulParameter.ul];
    }
    
    switch  (gu8_Item)
    {
        case MODELSET_CHECKCODE:       //��������
            Disp_InputCode();       //����������ʾ����
            gb_FlashFlg = true;
            break;
        case MODELSET_SWITCH:
            Disp_OnOff(ulParameter.ul);           //��ʾ����
            break;
        case MODELSET_SETUP_0:      //��Դ��
            gu32_ModelArry[0] = ulParameter.ul;
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_ZERO(true);
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_ZERO(false);
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_1:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_STAB(true);            //���
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_STAB(false);            //��
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_2:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_RUN(true);            //���
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_RUN(false);            //��
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_3:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_SUM(true);            //���
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_SUM(false);            //��
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_4:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_OVER(true);            //���
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_OVER(false);            //��
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_5:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_UNDER(true);            //���
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_UNDER(false);            //��
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_6:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                gu8_AuxDispArray[0] = SEG_TOPLINE;
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                gu8_AuxDispArray[0] = SEG_BLK;
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
    }
}

/*********************************************************************
��������: void ModelSet(void)
��    ��: �ͺ�����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-1-7
��    ��:                      ʱ    ��: 
*********************************************************************/
void ModelSet(void)
{
    gb_ExitFlg = false;
    ulParameter.ul = 0xFFFFFFFF;
    gu32_KeyCode = 0;
    gu8_Item = 0;
    gu8_DataInLength = 0;

    while (gb_ExitFlg == false)
    {
        gu32_KeyCode = Key_Deal();                     //������
        OPBuzzer();
        Key_ModelSetState();               //�ָ��������ò���
        VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);       //����ʾ����
        Disp_ModelSetState();          //�ָ�����������ʾ����
        if (gb_DispErrorFlg)        //��ʾerrorʱ��������������ʾ����
            Disp_Error();           //error������ʾ����
        VFD_CopyDispArryToDispRam();    //����ʾ���������ʾ����
        VFD_WriteDispRamArry();         //д��ʾ���� 
    }
}




