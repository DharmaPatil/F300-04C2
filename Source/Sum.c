/* 
Description:            �ۼ���ع���
Author:                 �Ժ���
Date:                   2013-12-9
*/

#include "include.h"

#define SUM_VIEW_CHECKCODE  0   //�ۼƲ�ѯ����У��
#define DISP_SUM_WT         1   //��ʾ�ۼ�����
#define DISP_SUM_NUM_OUTWT  2   //��ʾ�ۼƴ������շ����ۼ�
#define DISP_SUM_BAT_FLUX   3   //��ʾʣ�����λ�����
#define DISP_SUM_WT_REC     4   //��ʾ�䷽�ۼ�����
#define DISP_SUM_NUM_REC    5   //��ʾ�䷽�ۼƴ���
#define SUM_CLR_CHECKCODE   6   //�ۼ��������У��

u32 gu32_DebugSumNum;
u32 gu32_DebugSumNumTemp;   //��ӡ��ʹ��




u64 gu64_SumWt;                //�ۼ����� ���15λ
u32 gu32_SumNum;               //�ۼƴ���
//u64 gu64_SumWt_Total;          //ϵͳ���ۼ�����
//u32 gu32_SumNum_Total;         //ϵͳ���ۼƴ���
u32 gu32_SumWt_Rec[21];             //�䷽1-20�ۼ�����
u32 gu32_SumNum_Rec[21];            //�䷽1-20�ۼƴ���

//�ۼ�����С����͵�λת��������ݱ���
u32 gu32_SumWt;
u32 gu32_SumWtUnit;             //�ۼƵ�λ
u32 gu32_SumWtPoint;            //�ۼ�С����
//u32 gu32_SumWt_Rec[21];
//u32 gu32_SumWtUnit_Rec[21];             //�ۼƵ�λ
//u32 gu32_SumWtPoint_Rec[21];            //�ۼ�С����


u8 gu8_SumCheckInTime;      //�ۼƲ�ѯ��������������
u8 gu8_RecDispSum;


/*********************************************************************
��������: bool GetSumWt_u32(u64 fu64_SumWt,
                           u32 fu32_Unit,
                           u32 fu32_Point,
                           u32* fu32p_Unit,
                           u32* fu32p_Point)
��    ��: ����4�ֽڵ��ۼ���������
˵    ��: �Ǳ��ۼ������ʾʮ���Ƶ�9λ������ʾ���������Ƚ���С��������λ��������
          ������ʾ��������е�λ�任����kg��Ϊt��Ȼ������ʾ
          ��Ϊ�����Ŀ���ֲ����
��ڲ���: 
�� �� ֵ: true:���  false:δ���
��    ��: �Ժ���               ʱ    ��: 2014-1-15
��    ��:                      ʱ    ��: 
*********************************************************************/
bool GetSumWt_u32(u64 fu64_SumWt,    //��Ҫת���ĵ�ǰ8�ֽ���������
                  u32 fu32_Unit,     //��ǰ������Ӧ�ĵ�λ
                  u32 fu32_Point,    //��ǰ������Ӧ��С����
                  u32* fu32p_SumWt,  //�õ���4�ֽ�Ŀ������
                  u32* fu32p_Unit,   //Ŀ�������ĵ�λ
                  u32* fu32p_Point   //Ŀ��������С����
                )
{   //С����͵�λ��ֵͬϵͳ��ͬ
    *fu32p_Unit = fu32_Unit;
    *fu32p_Point = fu32_Point;
    if (fu64_SumWt <= 999999999)    //С��9λ��ֱ�Ӹ�ֵ4�ֽڵ���������
        *fu32p_SumWt = fu64_SumWt;
    else
    {   //��������ʮ����9λ������ת��
        while(fu64_SumWt > 999999999)
        {   //�Ƚ���С���������ݴӵ�λ��ȥ
            if (*fu32p_Point > 0)
            {
                (*fu32p_Point)--;
                fu64_SumWt /= 10;
            }
            else if (*fu32p_Unit < 2)
            {   //С����Ϊ0����е�λ��ת��
    //          UNIT_g      0
    //          UNIT_kg     1
    //          UNIT_t      2
                (*fu32p_Unit)++;
                (*fu32p_Point) = 3;
            }
            else
            {   //��λ�Ѿ�ת��Ϊt������ʾ��������Ϊ��������¿�ʼ�ۼ�
                return true;
            }
        }
        //��������ת�����������ֵ4�ֽڵ��ۼ�����
        *fu32p_SumWt = fu64_SumWt;
    }
    return false;
}

/*********************************************************************
��������: void Save_SumData(u32 fu32_Weight, u32 fu32_Num)
��    ��: �ۼ����ݴ�������
˵    ��: 
��ڲ���: fu32_Weight=������ۼ�����  fu32_Num=������ۼƴ���
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-15
��    ��:                      ʱ    ��: 
*********************************************************************/
void Save_SumData(u32 fu32_Weight, u32 fu32_Num)
{
    gu64_SumWt += fu32_Weight;  //�ۼ�
    gu32_SumNum += fu32_Num;
    
    gu32_SumWt_Rec[gu32_RecNum] += fu32_Weight;     //��Ӧ�䷽�ۼ�
    gu32_SumNum_Rec[gu32_RecNum] += fu32_Num;
    
    //�ۼƹ��� 15λ��9λ
    if (gu64_SumWt > (gs_Setup.SumExtendSwitch ? 999999999999999 : 999999999))
    {
        gu64_SumWt -= (gs_Setup.SumExtendSwitch ? 1000000000000000 : 1000000000);
//        gu32_SumNum = 0;
    }
    if (gu32_SumNum > 999999999)
    {
        gu32_SumNum = 0;
//        gu32_SumWt = 0;
    }
    
    if (gu32_SumWt_Rec[gu32_RecNum] > 999999999)          //�ۼƹ���
    {
        gu32_SumWt_Rec[gu32_RecNum] -= 1000000000;
//        gu32_SumNum_Rec[gu32_RecNum] = 0;
    }
    if (gu32_SumNum_Rec[gu32_RecNum] > 999999999)
    {
//        gu32_SumWt_Rec[gu32_RecNum] = 0;
        gu32_SumNum_Rec[gu32_RecNum] = 0;
    }
    //�������ۼ������ʹ���
    FRAM_Write8Byte(FMAddr_SumWt, gu64_SumWt);
    FRAM_WriteDW(FMAddr_SumNum, gu32_SumNum);
    //�����䷽�ۼ������ʹ���
    FRAM_WriteDW(FMAddr_SumWt_1 + gu32_RecNum * 4, gu32_SumWt_Rec[gu32_RecNum]);
    FRAM_WriteDW(FMAddr_SumNum_1 + gu32_RecNum * 4, gu32_SumNum_Rec[gu32_RecNum]);
    //��ȡ��ʾ��4�ֽڵ������Ͷ�Ӧ�ĵ�λ
    if (GetSumWt_u32(gu64_SumWt,      //��Ҫת���ĵ�ǰ8�ֽ���������
                     gs_Cal.Unit,     //��ǰ������Ӧ�ĵ�λ
                     gs_Cal.Point,    //��ǰ������Ӧ��С����
                     &gu32_SumWt,     //�õ���4�ֽ�Ŀ������
                     &gu32_SumWtUnit,   //Ŀ�������ĵ�λ
                     &gu32_SumWtPoint   //Ŀ��������С����
                    ))
    {   //�����Ѿ��������ۼƵ����ֵ�����������ۼ����¼���
        ClrSum();
    }
}

/*********************************************************************
��������: void ClrSum_Rec(u8 fu8_Rec)
��    ��: ���䷽�ۼ�
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-15
��    ��:                      ʱ    ��: 
*********************************************************************/
void ClrSum_Rec(u8 fu8_Rec)
{
    gu32_SumWt_Rec[fu8_Rec] = 0;
    gu32_SumNum_Rec[fu8_Rec] = 0;
    FRAM_WriteDW(FMAddr_SumWt_1 + fu8_Rec*4, 0);
    FRAM_WriteDW(FMAddr_SumNum_1 + fu8_Rec*4, 0);
}

/*********************************************************************
��������: void ClrSum(void)
��    ��: ���ۼ�
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-15
��    ��:                      ʱ    ��: 
*********************************************************************/
void ClrSum(void)
{
    u8 i;
    for (i = 1; i <= 20; i++)    //����䷽�ۼ�
        ClrSum_Rec(i);
    gu64_SumWt = 0;
    gu32_SumNum = 0;
    Save_SumData(0, 0);
    GetSumWt_u32(gu64_SumWt,      //��Ҫת���ĵ�ǰ8�ֽ���������
                 gs_Cal.Unit,     //��ǰ������Ӧ�ĵ�λ
                 gs_Cal.Point,    //��ǰ������Ӧ��С����
                 &gu32_SumWt,     //�õ���4�ֽ�Ŀ������
                 &gu32_SumWtUnit,   //Ŀ�������ĵ�λ
                 &gu32_SumWtPoint   //Ŀ��������С����
                 );
}

/*********************************************************************
��������: void Key_SumState(void)
��    ��: �ۼƲ�ѯ״̬��ش���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-27
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_SumState(void)
{
    static u8 su8_ItemBak;
     
    if (gu8_Item != SUM_VIEW_CHECKCODE && gu8_Item != SUM_CLR_CHECKCODE)
    {
        if (gb_FlashFlg == true)             //�ۼ���˸
        {
            switch (gu32_KeyCode)        //��������
            {
                case K_ESC:
                    gb_FlashFlg = false;
                    break;
                case K_ENTER:           //����ۼ��������ۼƴ���
                    if (gs_Setup.SumClrCodeSwitch != 0)     //����ۼ���Ҫ���뿪��
                    {
            			su8_ItemBak = gu8_Item;
            			gu8_Item = SUM_CLR_CHECKCODE;
                        gu32_KeyCode = 0;
            			gb_SetFlg = false;
            			ulParameter.ul = 0xFFFFFFFF;
                        gb_FlashFlg = false;
            			gu8_DataInLength = 0;
                    }
                    else
                    {
                        if (gu8_Item <= DISP_SUM_BAT_FLUX)  //��������ۼ�
                        {
                            ClrSum();
                            gu8_Item = DISP_SUM_WT;
                        }
                        else    //����䷽�ۼ�
                        {
                            ClrSum_Rec(gu8_RecDispSum);
                            gu8_Item = DISP_SUM_NUM_REC;
                        }
                        gb_FlashFlg = false;
                    }
                    break;
                default:
                    break;
            }
        }
        else                        //�ۼ���ʾ״̬
        {
            switch (gu32_KeyCode)       //��������
            {
                case K_7:   //�鿴�ۼ�ʱ����ӡ���Ĵ���
                    if (gs_Setup.ComMode_1 == COM_PRINT || gs_Setup.ComMode_2 == COM_PRINT)
                    {
                        if (gu8_Item == DISP_SUM_WT || gu8_Item == DISP_SUM_NUM_OUTWT)
                        {   //�鿴���ۼƵ�ʱ�򰴴�ӡ����ӡ���ۼ�����
                            PrintStart(PRINT_TOTAL_REPORT, 0, 0);
                        }
                        else if (gu8_Item == DISP_SUM_WT_REC || gu8_Item == DISP_SUM_NUM_REC)
                        {   //�鿴�䷽�ۼƵ�ʱ���ӡ�䷽�ۼƱ���
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_REPORT, gu8_RecDispSum, 0);
                        }
                    }
                    else    //����û�ж���ɴ�ӡ����ʾERROR10
                        ErrorDeal(10, 100);
                    break;
                case K_6:           //
                case K_RIGHT:
                    if (gu8_Item >= DISP_SUM_WT_REC)
                    {
                        if (++gu8_Item > DISP_SUM_NUM_REC)
                            gu8_Item = DISP_SUM_WT_REC;
                    }
                    else
                    {   //����������Ϊ0ʱ����ʾʣ������
                        if (++gu8_Item > ((gu32_BatchTime == 0) ? DISP_SUM_NUM_OUTWT : DISP_SUM_BAT_FLUX))
                        {
                            if (gu32_SystemState != RUNSTATE)  //δ��������״̬
                                gu32_SystemState = STOPSTATE;
                            gb_FlashFlg = false;
                            gb_SumFlg = false;
                        }
                    }
                    break;
                case K_INC:
                    if (gu8_Item <= DISP_SUM_BAT_FLUX) //�鿴���ۼ�ʱ������������䷽�ۼƲ鿴
                    {
                        gu8_Item = DISP_SUM_WT_REC;
                        gu8_RecDispSum = gu32_RecNum;
                    }
                    else if (gu8_Item >= DISP_SUM_WT_REC) //���ڲ鿴�䷽�ۼ�״̬�����������䷽��ѭ����ʾÿ���䷽���ۼ�
                    {
                        gu8_Item = DISP_SUM_WT_REC;
                        if (++gu8_RecDispSum > 20)
                            gu8_RecDispSum = 1;
                        ulParameter.ul = gu32_SumWt_Rec[gu8_RecDispSum];
                    }
                    break;
                case K_ESC:
                    if (gu8_Item >= DISP_SUM_WT_REC)
                    {
                        gu8_Item = DISP_SUM_WT;
                    }
                    else
                    {
                        gb_FlashFlg = false;
                        gb_SumFlg = false;
                        if (gu32_SystemState != RUNSTATE)  //δ��������״̬
                            gu32_SystemState = STOPSTATE;
                    }
                    break;
                case K_ZERO:
                    if (gu8_Item != DISP_SUM_BAT_FLUX && gb_RunFlg == false)   //��ʾ�ۼ������ʹ������԰�ZERO
                        gb_FlashFlg = true;
                    break;
                default:
                    break;
            }
        }
    }
    
    switch (gu8_Item)
    {
        case SUM_VIEW_CHECKCODE:    //��ѯ����
        case SUM_CLR_CHECKCODE:     //�������
            gb_FirstNumKeyFlg = false;
            //��������
            ulParameter.ul = Key_NumInput(ulParameter.ul, 6);
            if (gu32_KeyCode == K_ESC)
            {
                if (gu8_Item == SUM_VIEW_CHECKCODE)
                {
                    gb_FlashFlg = false;
                    if (gu32_SystemState != RUNSTATE)  //δ��������״̬
                        gu32_SystemState = STOPSTATE;
                    gb_SumFlg = false;
                }
                else if (gu8_Item == SUM_CLR_CHECKCODE)
                {
                    gu8_Item = su8_ItemBak;
                    gb_FlashFlg = false;
                }
            }
            else if (gu32_KeyCode == K_ENTER)
            {
                gu8_SumCheckInTime++;
                if ((ulParameter.ul == gu32_SetupCode && gu8_DataInLength ==6) || 
                    (ulParameter.ul == SUPERCODE && gu8_DataInLength ==6))        //������������趨����򳬼�����
                {
                    if (gu8_Item == SUM_VIEW_CHECKCODE)      //�ۼƲ�ѯ����������ȷ
                    {
                        gu8_SumCheckInTime = 0;
                        gb_FlashFlg = false;
                		ulParameter.ul = gu32_SumWt;
                		gu8_Item = DISP_SUM_WT;
                		gb_SumFlg = true;      //sumָʾ����
                	}
                	else if (gu8_Item == SUM_CLR_CHECKCODE)     //�ۼ��������������ȷ
                	{
                	    if (su8_ItemBak <= DISP_SUM_NUM_OUTWT)  //��������ۼ�
                        {
                            ClrSum();
                    		ulParameter.ul = gu32_SumWt;
                    		gu8_Item = DISP_SUM_WT;
                        }
                        else    //����䷽�ۼ�
                        {
                            ClrSum_Rec(gu8_RecDispSum);
                    		ulParameter.ul = gu32_SumWt_Rec[gu8_RecDispSum];
                    		gu8_Item = DISP_SUM_NUM_REC;
                        }
                        gb_FlashFlg = false;
                        gu8_SumCheckInTime = 0;
                		gb_SumFlg = true;      //sumָʾ����
                    }
                }
                else if (gu8_SumCheckInTime >= 3)      //����������󳬹�3��
                {
                    ErrorDeal(4, 0);        //��ʾerror4
                    Disp_Error();
                    VFD_CopyDispArryToDispRam();
                    VFD_WriteDispRamArry();                  //д��ʾ����
                    while(1);
                }
                else            //�������������ʾerror
                {
                    ErrorDeal(0, 100);
                    ulParameter.ul = 0;
                    gu8_DataInLength = 0;
                    return;
                }
            }
            break;
        case DISP_SUM_WT:             //��ʾ�ۼ�����
            ulParameter.ul = gu32_SumWt;
            break;
        case DISP_SUM_NUM_OUTWT:             //��ʾ�ۼƴ�����ɢ�ϳ�ģʽ��ʾ�շ����ۼ�
            ulParameter.ul = gu32_SumNum;
            break;
        case DISP_SUM_BAT_FLUX:             //����ֵ������
            ulParameter.ul = gu32_BatchTimeCnt;
            break;
        case DISP_SUM_WT_REC:             //�䷽�ۼ�����
            ulParameter.ul = gu32_SumWt_Rec[gu8_RecDispSum];
            break;
        case DISP_SUM_NUM_REC:             //�䷽�ۼƴ���
            ulParameter.ul = gu32_SumNum_Rec[gu8_RecDispSum];
        default:
            break;
    }
}

/*********************************************************************
��������: void Disp_SumState(void)
��    ��: ���������������ʾ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-27
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_SumState(void)
{
    bool lb_Echo0 = true;
    u32 lul_Point = 0;
    static u32 su32_TimeTemp;
    
    if (gu8_Item == SUM_VIEW_CHECKCODE || gu8_Item == SUM_CLR_CHECKCODE)      //��������
    {
        Disp_InputCode();       //����������ʾ����
        gb_FlashFlg = true;
        Flash_MainDisp(0x01);                //��˸����
    }
    else if (gu8_Item == DISP_SUM_WT)
    {
        VFD_UnitDisp(gu32_SumWtUnit);               //�ۼ�������ʾ��λ
        lul_Point = gu32_SumWtPoint;               //�ۼ�������ʾϵͳС����
    }
    else if (gu8_Item == DISP_SUM_WT_REC)
    {
        VFD_UnitDisp(gs_Cal.Unit);               //�ۼ�������ʾ��λ
        lul_Point = gs_Cal.Point;               //�ۼ�������ʾϵͳС����
    }
    else if (gu8_Item == DISP_SUM_BAT_FLUX)
    {
        VFD_UnitDisp(UNIT_PCS);               //ʣ��������ʾPCS
        lul_Point = 0;                          //�ۼƴ�������ʾС����
    }
    else
    {
        VFD_UnitDisp(UNIT_N);               //�ۼƴ�����ʾN
        lul_Point = 0;                          //�ۼƴ�������ʾС����
    }
    
    
    //�ۼ��������ۼƴ�������ʾ�����9λ�������6λ�����3λ
    if (gu8_Item == DISP_SUM_WT || gu8_Item == DISP_SUM_NUM_OUTWT || gu8_Item == DISP_SUM_BAT_FLUX || 
        gu8_Item == DISP_SUM_WT_REC || gu8_Item == DISP_SUM_NUM_REC)
    {
        //�ۼƴ���6λ����������ʾ������
        if (ulParameter.ul / 1000000 > 0)
        {
            lb_Echo0 = false;
            
            if (gu8_Item == DISP_SUM_WT_REC || gu8_Item == DISP_SUM_NUM_REC)
            {
                //��ʾ�䷽�ۼ�ʱ������6λҪռ�ø���ʾ������ʾ���ۼƸ�3λ���䷽��֮����˸�л�
                if(gu32_Sys10ms - su32_TimeTemp > 40)
                {
                    VFD_AuxDisp_u32(ulParameter.ul/1000000);       //����ʾֵ ����
                    Disp_Echo0(0, gu8_AuxDispArray, 3);
                    if(gu32_Sys10ms - su32_TimeTemp > 80)
                        su32_TimeTemp = gu32_Sys10ms;
                }
                else
                    Disp_Aux_Rec(gu8_RecDispSum);
            }
            else
            {
                VFD_AuxDisp_u32(ulParameter.ul/1000000);       //����ʾֵ ����
                Disp_Echo0(0, gu8_AuxDispArray, 3);
            }
        }
        else if (gu8_Item == DISP_SUM_WT_REC || gu8_Item == DISP_SUM_NUM_REC)
        {
            Disp_Aux_Rec(gu8_RecDispSum);
        }
        
        Disp_Main_Data(ulParameter.ul % 1000000, lul_Point, 6, lb_Echo0, false);       //����ʾֵ
        if (lb_Echo0 == true)  //�˴���ֹ���ۼ���˸ʱ0����ʧЧ������
            Disp_Echo0(lul_Point, gu8_MainDispArray, 6);
        if (gb_RunFlg)
        {   //����ʱ�鿴�ۼ�ʱ����ָʾ�ƶ���ʾ v1.01
            Disp_FlgLight();
        }
        else
        {//ֹͣʱ�鿴�ۼ�ʱΪ����ʾ������ֻ࣬��SUMָʾ���� v1.01
            VFD_FlgDisp_SUM(gb_SumFlg);     //SUMָʾ����
            //LEDָʾ�Ƶ���ʾ
            //�д����ʱ����������˼д���λ���Ǽд���λ��Ч��PACK��˸
            if (gb_EnPackOKFlg && (!gb_PackOKFlg))
                VFD_LEDDisp(!gb_DispNetFlg, gb_DispNetFlg, false, Flash_Flg(gb_PackFlg));  //LED����ʾ
            else
                VFD_LEDDisp(!gb_DispNetFlg, gb_DispNetFlg, false, gb_PackFlg);  //LED����ʾ
        }
        Flash_MainDisp(0x3F);   //����ʾ��˸
        Flash_AuxDisp(0x07);    //����ʾ��˸
    }
}

/*********************************************************************
��������: void DiscSumDeal(void)
��    ��: �ֶ�ж���ۼ�������ݴ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-16
��    ��:                      ʱ    ��: 
*********************************************************************/
void DiscSumDeal(void)
{
    static u32  su32_SavedWt;
    static bool sb_SavedWtSigFlg;
    u32 lu32_SumTemp = 0;
    
    if (gb_RunFlg == false)     //ֹͣ״̬�²ſ��Խ����ֶ�ж��
    {
        if (gb_DiscFlg == false)     //δ�����ֶ�ж��
        {
            if (gb_DiscSumOnFlg == true)
            {
                /*�ֶ�ж���ۼ��ź���Ч�󣬵ȴ��Ǳ��ȶ�������ж��ǰ���ȶ�������
                ������ж�Ϻ�Ĳ�ֵ�Ӷ������ۼƼ��㣬�����ж���ź�*/
                if (gb_StabFlg == true)
                {
                    su32_SavedWt = gu32_Wt;       //����ж��ǰ����
                    sb_SavedWtSigFlg = gb_SigWtFlg;       //����ж��ǰ��������+��-
                    gb_DiscFlg = true;        //����ж��
                    gb_DiscSumOnFlg = false;
                }
            }
        }
        else    //�ֶ�ж���Ѿ�����
        {
            if (gb_DiscSumOffFlg == true)
            {
                /*�ֶ�ж���ۼ��źŹرգ��ȴ��Ǳ��ȶ���Ƚ�ж��ǰ��ж�Ϻ��������ֵ��
                �Ӷ�������ۼƵ����������ر�ж���źš�*/
                if (gb_StabFlg == true)
                {
                    if (gb_SigWtFlg == sb_SavedWtSigFlg)  //ж��ǰ�������ͬ
                    {
                        if (gb_SigWtFlg == false)    //ж��ǰ��Ϊ+��
                        {
                            if (su32_SavedWt >= gu32_Wt)    //ж��ǰ��������ж�Ϻ�����������ֵ��Ϊж��ֵ���������ڲ�����ж�ϲ������
                            {
                                lu32_SumTemp = (su32_SavedWt - gu32_Wt);
                            }
                        }
                        else        //ж��ǰ��Ϊ-��
                        {
                            if (gu32_Wt >= su32_SavedWt)    //ж��ǰ��������ж�Ϻ�����������ֵ��Ϊж��ֵ���������ڲ�����ж�ϲ������
                            {
                                lu32_SumTemp = (gu32_Wt - su32_SavedWt);
                            }
                        }
                    }
                    else    //ж��ǰ����Ų�ͬ
                    {
                        if (gb_SigWtFlg == true)    //ж��ǰ+ж�Ϻ�-����ж��
                        {
                            lu32_SumTemp = (gu32_Wt + su32_SavedWt);
                        }
                    }
                    if (lu32_SumTemp != 0)
                    {
                        Save_SumData(lu32_SumTemp, 1);    //�����ۼ�����
                    #ifndef DEBUG_MONITORTIME_PRINT
                        //�Զ���ӡ
                        PrintStart(PRINT_AUTO, lu32_SumTemp, 0);
                    #endif
                    }
                    gb_DiscFlg = false;   //�ر�ж��
                    gb_DiscSumOffFlg = false;
                }
            }
        }
    }
}

