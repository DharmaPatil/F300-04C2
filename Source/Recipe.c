/*
Description:            �䷽����������غ���
Author:                 �Ժ���
Date:                   2013-12-9
*/

#include "include.h"



struct RECIPE gs_Rec;
u32 gu32_RecCodeSwitch;
u32 gu32_RecCode;
u32 gu32_RecNum;

u8 gu8_RecInTime;



const u16 gu16_RecRealItem[]=
{0, 1,
//F1  F1.1 F1.2 F1.3 F1.4 F1.5 F1.6
0x10, 0x11,0x12,0x13,0x14,0x15,0x16,
//F2 F2.1 F2.2 F2.3
0x20,0x21,0x22,0x23,
//F3 F3.1 F3.2 F3.3 F3.4 F3.5 F3.6 F3.7 F3.8 F3.9 F3.10 F3.11 F3.12
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x310,0x311,0x312,
//F4 F4.1 F4.2 F4.3
0x40,0x41,0x42,0x43,
//F5 F5.1 F5.2 F5.3 F5.4 F5.5 F5.6
0x50,0x51,0x52,0x53,0x54,0x55,0x56,
//F6 F6.1 F6.2
0x60,0x61,0x62,
//F7
0x70,
//F8 F8.1 F8.2 F8.3 F8.4
0x80,0x81,0x82,0x83,0x84,
//F9 F9.1 F9.2 F9.3
0x90,0x91,0x92,0x93,
//FA FA.1
0xA0,0xA1};
const u8 gu8_RecRightItem[] =        //�����������һ����
{0, 1,
//F1    F1.1      F1.2      F1.3      F1.4      F1.5      F1.6
REC_F2, REC_F1_2, REC_F1_3, REC_F1_4, REC_F1_5, REC_F1_6, REC_F1_1,
//F2    F2.1      F2.2      F2.3
REC_F3, REC_F2_2, REC_F2_3, REC_F2_1,
//F3    F3.1      F3.2      F3.3      F3.4      F3.5      F3.6      F3.7      F3.8      F3.9       F3.10      F3.11      F3.12    
REC_F4, REC_F3_2, REC_F3_3, REC_F3_4, REC_F3_5, REC_F3_6, REC_F3_7, REC_F3_8, REC_F3_9, REC_F3_10, REC_F3_11, REC_F3_12, REC_F3_1,
//F4    F4.1      F4.2      F4.3
REC_F5, REC_F4_2, REC_F4_3, REC_F4_1,
//F5    F5.1      F5.2      F5.3      F5.4      F5.5      F5.6
REC_F6, REC_F5_2, REC_F5_3, REC_F5_4, REC_F5_5, REC_F5_6, REC_F5_1,
//F6    F6.1      F6.2
REC_F7, REC_F6_2, REC_F6_1,
//F7
REC_F8,
//F8    F8.1      F8.2      F8.3      F8.4
REC_F9, REC_F8_2, REC_F8_3, REC_F8_4, REC_F8_1,
//F9    F9.1      F9.2      F9.3
REC_FA, REC_F9_2, REC_F9_3, REC_F9_1,
//FA    FA.1
REC_F1, REC_FA_1};

const u8 gu8_RecEnterItem[] =        //��ENTER�������һ����
{0,       REC_F1,
//F1      F1.1      F1.2      F1.3      F1.4      F1.5      F1.6
REC_F1_1, REC_F1_2, REC_F1_3, REC_F1_4, REC_F1_5, REC_F1_6, REC_F1_1,
//F2      F2.1      F2.2      F2.3
REC_F2_1, REC_F2_2, REC_F2_3, REC_F2_1,
//F3      F3.1      F3.2      F3.3      F3.4      F3.5      F3.6      F3.7      F3.8      F3.9       F3.10      F3.11      F3.12     
REC_F3_1, REC_F3_2, REC_F3_3, REC_F3_4, REC_F3_5, REC_F3_6, REC_F3_7, REC_F3_8, REC_F3_9, REC_F3_10, REC_F3_11, REC_F3_12, REC_F3_1,
//F4      F4.1      F4.2      F4.3
REC_F4_1, REC_F4_2, REC_F4_3, REC_F4_1,
//F5      F5.1      F5.2      F5.3      F5.4      F5.5      F5.6
REC_F5_1, REC_F5_2, REC_F5_3, REC_F5_4, REC_F5_5, REC_F5_6, REC_F5_1,
//F6      F6.1      F6.2
REC_F6_1, REC_F6_2, REC_F6_1,
//F7
REC_F8,
//F8      F8.1      F8.2      F8.3      F8.4
REC_F8_1, REC_F8_2, REC_F8_3, REC_F8_4, REC_F8_1,
//F9      F9.1      F9.2      F9.3
REC_F9_1, REC_F9_2, REC_F9_3, REC_F9_1,
//FA      FA.1
REC_FA_1, REC_FA};



/*********************************************************************
��������: u32 GetRecipeItemValue(u8 fuc_Item)
��    ��: �õ���Ӧ���䷽ֵ
˵    ��: 
��ڲ���: fuc_Item = ������
�� �� ֵ: �䷽����ֵ
��    ��: �Ժ���               ʱ    ��: 2011-3-23
��    ��:                      ʱ    ��: 
*********************************************************************/
u32 GetRecipeItemValue(u8 fuc_Item)
{
    u32 ful_Value;
    
    switch(fuc_Item)
    {
        case DISP_RECNUM:
            ful_Value = gu32_RecNum; break;
        case REC_F1_1:
            ful_Value = gs_Rec.Target; break;
        case REC_F1_2:
            ful_Value = gs_Rec.Large; break;
        case REC_F1_3:
            ful_Value = gs_Rec.Middle; break;
        case REC_F1_4:
            ful_Value = gs_Rec.Small; break;
        case REC_F1_5:
            ful_Value = gs_Rec.NearZero; break;
        case REC_F1_6:
            ful_Value = gs_Rec.FlapBagWt; break;
        case REC_F2:
            ful_Value = gs_Rec.OUSwitch; break;
        case REC_F2_1:
            ful_Value = gs_Rec.OverValue; break;
        case REC_F2_2:
            ful_Value = gs_Rec.UnderValue; break;
        case REC_F2_3:
            ful_Value = gs_Rec.OUStopSwitch; break;
        case REC_F3_1:
            ful_Value = gs_Rec.TimeT1; break;
        case REC_F3_2:
            ful_Value = gs_Rec.TimeT2b; break;
        case REC_F3_3:
            ful_Value = gs_Rec.TimeT3; break;
        case REC_F3_4:
            ful_Value = gs_Rec.TimeT4; break;
        case REC_F3_5:
            ful_Value = gs_Rec.TimeT5; break;
        case REC_F3_6:
            ful_Value = gs_Rec.TimeT6; break;
        case REC_F3_7:
            ful_Value = gs_Rec.TimeT7; break;
        case REC_F3_8:
            ful_Value = gs_Rec.TimeT2a; break;
        case REC_F3_9:
            ful_Value = gs_Rec.TimeT9; break;
        case REC_F3_10:
            ful_Value = gs_Rec.TimeT10; break;
        case REC_F3_11:
            ful_Value = gs_Rec.TimeT11; break;
        case REC_F3_12:
            ful_Value = gs_Rec.TimeT12; break;
        case REC_F4:
            ful_Value = gs_Rec.CorrectFallSwitch; break;
        case REC_F4_1:
            ful_Value = gs_Rec.CorrectFallTime; break;
        case REC_F4_2:
            ful_Value = gs_Rec.CorrectFallRng1; break;
        case REC_F4_3:
            ful_Value = gs_Rec.CorrectFallRng2; break;
        case REC_F5:
            ful_Value = gs_Rec.FlapBagSwitch; break;
        case REC_F5_1:
            ful_Value = gs_Rec.FlapBagAddNum; break;
        case REC_F5_2:
            ful_Value = gs_Rec.FlapBagNum; break;
        case REC_F5_3:
            ful_Value = gs_Rec.FlapBagDelay; break;
        case REC_F5_4:
            ful_Value = gs_Rec.FlapBagTimeHi; break;
        case REC_F5_5:
            ful_Value = gs_Rec.FlapBagTimeLow; break;
        case REC_F5_6:
            ful_Value = gs_Rec.FlapBagExtTime; break;
        case REC_F6:
            ful_Value = gs_Rec.SmallPluseSwitch; break;
        case REC_F6_1:
            ful_Value = gs_Rec.SmallPluseHi; break;
        case REC_F6_2:
            ful_Value = gs_Rec.SmallPluseLow; break;
        case REC_F7:
            ful_Value = gs_Rec.CombinedNum; break;
        case REC_F8:
            ful_Value = gs_Rec.DiscShakeSwitch; break;
        case REC_F8_1:
            ful_Value = gs_Rec.DiscEffTime; break;
        case REC_F8_2:
            ful_Value = gs_Rec.DiscShakeTimeHi; break;
        case REC_F8_3:
            ful_Value = gs_Rec.DiscShakeTimeLow; break;
        case REC_F8_4:
            ful_Value = gs_Rec.DiscShakeNum; break;
        case REC_F9:
            ful_Value = gs_Rec.PrintCodeSwitch; break;
        case REC_F9_1:
            ful_Value = gs_Rec.PrintCodeRunDelay; break;
        case REC_F9_2:
            ful_Value = gs_Rec.PrintCodeTime; break;
        case REC_F9_3:
            ful_Value = gs_Rec.SheildDiscSwitch; break;
        case REC_FA:
            ful_Value = gu32_RecCodeSwitch; break;
        case REC_FA_1:
            ful_Value = gu32_RecCode; break;
        default:
            ful_Value = 0;
            break;
    }
    return ful_Value;
}

/*********************************************************************
��������: u16 GetRecipePort(u8 fuc_Item)
��    ��: �����䷽������Ӧ�Ĵ����ַ
˵    ��: 
��ڲ���: fuc_Item = ������
�� �� ֵ: �䷽�����洢��ַ
��    ��: �Ժ���               ʱ    ��: 2013-12-24
��    ��:                      ʱ    ��: 
*********************************************************************/
u16 GetRecipePort(u8 fuc_Item)
{
    u16 lui_FMAddress;
    
    switch (fuc_Item)
    {
        case DISP_RECNUM:
            gu32_RecNum = ulParameter.ul;
            lui_FMAddress = FMAddr_RecNum;
            break;
        case REC_F1_1:
            gs_Rec.Target = ulParameter.ul;
            lui_FMAddress = FMAddr_Target(gu32_RecNum);
            break;
        case REC_F1_2:
            gs_Rec.Large = ulParameter.ul;
            lui_FMAddress = FMAddr_Large(gu32_RecNum);
            break;
        case REC_F1_3:
            gs_Rec.Middle = ulParameter.ul;
            lui_FMAddress = FMAddr_Middle(gu32_RecNum);
            break;
        case REC_F1_4:
            gs_Rec.Small = ulParameter.ul;
            lui_FMAddress = FMAddr_Small(gu32_RecNum);
            break;
        case REC_F1_5:
            gs_Rec.NearZero = ulParameter.ul;
            lui_FMAddress = FMAddr_NearZero(gu32_RecNum);
            break;
        case REC_F1_6:
            gs_Rec.FlapBagWt = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagWt(gu32_RecNum);
            break;
        case REC_F2:
            gs_Rec.OUSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_OUSwitch(gu32_RecNum);
            break;
        case REC_F2_1:
            gs_Rec.OverValue = ulParameter.ul;
            lui_FMAddress = FMAddr_OverValue(gu32_RecNum);
            break;
        case REC_F2_2:
            gs_Rec.UnderValue = ulParameter.ul;
            lui_FMAddress = FMAddr_UnderValue(gu32_RecNum);
            break;
        case REC_F2_3:
            gs_Rec.OUStopSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_OUStopSwitch(gu32_RecNum);
            break;
        case REC_F3_1:
            gs_Rec.TimeT1 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT1(gu32_RecNum);
            break;
        case REC_F3_2:
            gs_Rec.TimeT2b = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT2b(gu32_RecNum);
            break;
        case REC_F3_3:
            gs_Rec.TimeT3 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT3(gu32_RecNum);
            break;
        case REC_F3_4:
            gs_Rec.TimeT4 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT4(gu32_RecNum);
            break;
        case REC_F3_5:
            gs_Rec.TimeT5 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT5(gu32_RecNum);
            break;
        case REC_F3_6:
            gs_Rec.TimeT6 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT6(gu32_RecNum);
            break;
        case REC_F3_7:
            gs_Rec.TimeT7 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT7(gu32_RecNum);
            break;
        case REC_F3_8:
            gs_Rec.TimeT2a = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT2a(gu32_RecNum);
            break;
        case REC_F3_9:
            gs_Rec.TimeT9 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT9(gu32_RecNum);
            break;
        case REC_F3_10:
            gs_Rec.TimeT10 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT10(gu32_RecNum);
            break;
        case REC_F3_11:
            gs_Rec.TimeT11 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT11(gu32_RecNum);
            break;
        case REC_F3_12:
            gs_Rec.TimeT12 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT12(gu32_RecNum);
            break;
        case REC_F4:
            gs_Rec.CorrectFallSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_CorrectFallSwitch(gu32_RecNum);
            break;
        case REC_F4_1:
            gs_Rec.CorrectFallTime = ulParameter.ul;
            lui_FMAddress = FMAddr_CorrectFallTime(gu32_RecNum);
            break;
        case REC_F4_2:
            gs_Rec.CorrectFallRng1 = ulParameter.ul;
            lui_FMAddress = FMAddr_CorrectFallRng1(gu32_RecNum);
            break;
        case REC_F4_3:
            gs_Rec.CorrectFallRng2 = ulParameter.ul;
            lui_FMAddress = FMAddr_CorrectFallRng2(gu32_RecNum);
            break;
        case REC_F5:
            gs_Rec.FlapBagSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagSwitch(gu32_RecNum);
            break;
        case REC_F5_1:
            gs_Rec.FlapBagAddNum = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagAddNum(gu32_RecNum);
            break;
        case REC_F5_2:
            gs_Rec.FlapBagNum = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagNum(gu32_RecNum);
            break;
        case REC_F5_3:
            gs_Rec.FlapBagDelay = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagDelay(gu32_RecNum);
            break;
        case REC_F5_4:
            gs_Rec.FlapBagTimeHi = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagTimeHi(gu32_RecNum);
            break;
        case REC_F5_5:
            gs_Rec.FlapBagTimeLow = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagTimeLow(gu32_RecNum);
            break;
        case REC_F5_6:
            gs_Rec.FlapBagExtTime = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagExtTime(gu32_RecNum);
            break;
        case REC_F6:
            gs_Rec.SmallPluseSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_SmallPluseSwitch(gu32_RecNum);
            break;
        case REC_F6_1:
            gs_Rec.SmallPluseHi = ulParameter.ul;
            lui_FMAddress = FMAddr_SmallPluseHi(gu32_RecNum);
            break;
        case REC_F6_2:
            gs_Rec.SmallPluseLow = ulParameter.ul;
            lui_FMAddress = FMAddr_SmallPluseLow(gu32_RecNum);
            break;
        case REC_F7:
            gs_Rec.CombinedNum = ulParameter.ul;
            lui_FMAddress = FMAddr_CombinedNum(gu32_RecNum);
            break;
        case REC_F8:
            gs_Rec.DiscShakeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscShakeSwitch(gu32_RecNum);
            break;
        case REC_F8_1:
            gs_Rec.DiscEffTime = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscEffTime(gu32_RecNum);
            break;
        case REC_F8_2:
            gs_Rec.DiscShakeTimeHi = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscShakeTimeHi(gu32_RecNum);
            break;
        case REC_F8_3:
            gs_Rec.DiscShakeTimeLow = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscShakeTimeLow(gu32_RecNum);
            break;
        case REC_F8_4:
            gs_Rec.DiscShakeNum = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscShakeNum(gu32_RecNum);
            break;
        case REC_F9:
            gs_Rec.PrintCodeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintCodeSwitch(gu32_RecNum);
            break;
        case REC_F9_1:
            gs_Rec.PrintCodeRunDelay = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintCodeRunDelay(gu32_RecNum);
            break;
        case REC_F9_2:
            gs_Rec.PrintCodeTime = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintCodeTime(gu32_RecNum);
            break;
        case REC_F9_3:
            gs_Rec.SheildDiscSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_SheildDiscSwitch(gu32_RecNum);
            break;
        case REC_FA:
            gu32_RecCodeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_RecCodeSwitch;
            break;
        case REC_FA_1:
            gu32_RecCode = ulParameter.ul;
            lui_FMAddress = FMAddr_RecCode;
            break;
        default:
            lui_FMAddress = 0;
            break;
    }
    return lui_FMAddress;
}

/*********************************************************************
��������: bool RecDataInputCheck(void)
��    ��: ������������Լ��
˵    ��: 
��ڲ���: 
�� �� ֵ: ���ݴ���
��    ��: �Ժ���               ʱ    ��: 2013-12-24
��    ��:                      ʱ    ��: 
*********************************************************************/
bool RecDataInputCheck(void)
{
    bool lb_err = false;
    
    switch(gu8_Item)
    {
        case DISP_RECNUM:
            if (ulParameter.ul > 20 || ulParameter.ul == 0)
                lb_err = true;
            break;
        case REC_F1_1:
        case REC_F1_2:
        case REC_F1_3:
        case REC_F1_4:
        case REC_F1_5:
        case REC_F1_6:
        case REC_F2_1:              //����ֵ
        case REC_F2_2:              //Ƿ��ֵ
            if (ulParameter.ul > gs_Cal.Cap)            //����ֵ���ò��ܳ����������
                lb_err = true;
            break;
        default:
            break;
    }
    
    return lb_err;
}


/*********************************************************************
��������: void Key_RecipeState(void)
��    ��: �䷽������ذ��������ݴ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-24
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_RecipeState(void)
{
    u8 i;
    u8 luc_DataLen = 0;      //��������λ��
        
    if (gu32_KeyCode == K_RIGHT)
    {
        gb_FirstNumKeyFlg = true;
        gu8_Item  = gu8_RecRightItem[gu8_Item ];                  //�õ�����������һ�����
        ulParameter.ul = GetRecipeItemValue(gu8_Item);        //��ȡ��Ӧ��Ŀ�Ĳ���ֵ
    }
    switch (gu8_Item)
    {
        case DISP_RECNUM:           //�䷽������
            luc_DataLen = 2;
            if (gu32_KeyCode == K_SET)               //��set�������䷽����
            {
                if (gu32_RecCodeSwitch == 0)         //�����������뿪�عر���ֱ�ӽ���F1
                {
                    gu8_Item  = REC_F1;
                    luc_DataLen = 0;
                    gb_FirstNumKeyFlg = true;
                }
                else
                {
                    gu8_Item = REC_CHECKCODE;       //������������״̬
                    luc_DataLen = 6;
                    gb_FirstNumKeyFlg = false;
                    ulParameter.ul = 0;
                }
                gu8_DataInLength = 0;
                gb_FlashFlg = false;
            }
            else if (gu32_KeyCode != K_NONE && gu32_KeyCode != K_RIGHT)        //������������ʱ�䷽����˸
            {
                gb_FlashFlg = true;
            }
            break;
        case REC_CHECKCODE:         //��������
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            break;
        case REC_F1_1:
        case REC_F1_2:
        case REC_F1_3:
        case REC_F1_4:
        case REC_F1_5:
        case REC_F1_6:
        case REC_F2_1:              //����ֵ
        case REC_F2_2:              //Ƿ��ֵ
             luc_DataLen = 6;
             break;
        case REC_F2:                //��Ƿ�������
        case REC_F2_3:              //��Ƿ����ͣ����
        case REC_F4:                //�����������
        case REC_F6:                //СͶ�㶯�������
        case REC_F8:                //ж����򿪹�
        case REC_F9:                //���뿪��
        case REC_F9_3:              //��������ж�Ͽ���
        case REC_FA:                //�䷽�����������뿪��
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //�����л����á����޸�
            break;
        case REC_F5:                //�Ĵ�����
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 3);      //4���л����á����޸�
            break;
        case REC_F4_1:              //�����������
        case REC_F4_2:              //���������Χ
        case REC_F5_1:              //�������Ĵ�����
        case REC_F5_2:              //��ֵ���Ĵ�����
        case REC_F7:                //������ϴ���
        case REC_F8_4:              //ж��������
            luc_DataLen = 2;
            break;
        case REC_F3_1:              //������ʱT1
        case REC_F3_2:              //��ֹ�Ƚ�ʱ��T2A
        case REC_F3_3:              //��ֹ�Ƚ�ʱ��T2B
        case REC_F3_4:              //������ʱ��T3
        case REC_F3_5:              //��ֵ����ʱ��T4
        case REC_F3_6:              //ж����ʱT5
        case REC_F3_7:              //�д���ʱT6
        case REC_F3_8:              //�ɴ���ʱT7
        case REC_F3_9:              //�����ʱT9         
        case REC_F3_10:             //��������Чʱ��T10
        case REC_F3_11:             //�ƴ���ʱT11        
        case REC_F3_12:             //�ƴ������Чʱ��T12
        case REC_F5_3:              //�Ĵ���ʱ
        case REC_F5_4:              //�Ĵ���Ч��ƽʱ��
        case REC_F5_5:              //�Ĵ���Ч��ƽʱ��
        case REC_F5_6:              //�����Ĵ������Чʱ��
        case REC_F6_1:              //�㶯��Ч��ƽʱ��
        case REC_F6_2:              //�㶯��Ч��ƽʱ��
        case REC_F8_1:              //ж����Чʱ��
        case REC_F8_2:              //ж�����ߵ�ƽ
        case REC_F8_3:              //ж�����͵�ƽ
        case REC_F9_1:              //����������ʱ
        case REC_F9_2:              //����ʱ��
            luc_DataLen = 3;
            break;
        case REC_F4_3:              //�����������
            ulParameter.ul = Key_IncInput(ulParameter.ul, 1, 3);      //�����л����á����޸�
            break;
        case REC_FA_1:
            gb_FirstNumKeyFlg = false;
            if (gu32_KeyCode == K_SET)           //������������״̬
                gb_SetFlg = true;
            if (gb_SetFlg)
            {
                if (gu32_KeyCode == K_ESC)       //�˳���������״̬
                {
                    gb_SetFlg = false;
                    gu32_KeyCode = 0;
                    gu8_RecInTime = 0;
                    gu8_DataInLength = 0;
                }
                else
                    luc_DataLen = 6;
            }
            break;
        default:
            break;
    }
    
    //��������
    ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
    
    //ÿһ���µİ�����������
    switch(gu32_KeyCode)
    {
        case K_ESC:             //�˳�������
            gb_FlashFlg = false;
            gb_SetFlg = false;
            gb_FirstNumKeyFlg = true;
            gu8_DataInLength = 0;
            if ((gu8_Item > REC_F1) && 
               (gu16_RecRealItem[gu8_Item] % 0x10 != 0))    //С���˳�����Ӧ�Ĵ���
            {
                for (i = REC_F1; i < REC_END; i++)
                {
                    if (gu16_RecRealItem[i] == (gu16_RecRealItem[gu8_Item]/0x10*0x10))  //�õ������Ӧ�Ĵ���ֵ
                    {
                        gu8_Item = i;               //�˳�����Ӧ����
                        ulParameter.ul = GetRecipeItemValue(gu8_Item);
                    }
                }
            }
            else            //����ʱ���˳����˳�������״̬
                gu32_SystemState = STOPSTATE;
            break;
        case K_ENTER:           //ȷ�ϼ�����
            gb_FirstNumKeyFlg = true;
            if (RecDataInputCheck())            //�������ݷ�Χ���
            {
                ErrorDeal(0, 100);
                ulParameter.ul = GetRecipeItemValue(gu8_Item);
                return;
            }
            switch (gu8_Item)
            {
                case DISP_RECNUM:
                    if (gu32_RecNum != ulParameter.ul)       //�޸��䷽�����¶�ȡ�䷽����ֵ
                        Read_RecipeData(ulParameter.ul);
                    gb_FlashFlg = false;
                    break;
                case REC_CHECKCODE:       //��������
                    gu8_RecInTime++;
                    if ((ulParameter.ul == gu32_RecCode && gu8_DataInLength == 6) || 
                        (ulParameter.ul == SUPERCODE && gu8_DataInLength == 6))        //������������趨����򳬼�����
                    {
                        gu8_RecInTime = 0;            //�����������������������
                        ulParameter.ul = 0;
                    }
                    else if (gu8_RecInTime >= 3)      //����������󳬹�3��
                    {
                        ErrorDeal(4, 0);        //��ʾerror4
                        Disp_Error();
                        VFD_CopyDispArryToDispRam();
                        VFD_WriteDispRamArry();
                        while(1);
                    }
                    else            //�������������ʾerror
                    {
                        ErrorDeal(0, 100);
                        ulParameter.ul = 0;
                        gu8_DataInLength = 0;
                        return;
                    }
                    break;
                case REC_FA_1:           //��������״̬
                    if (gb_SetFlg == true)
                    {
                        gu8_RecInTime++;
                        if (gu8_RecInTime >= 2)
                        {
                            if (ulParameter.ul == gu32_DataTemp && gu8_DataInLength == 6)         //�������������һ��
                            {
                                gu8_RecInTime = 0;
                                gb_SetFlg = false;
                                gu32_RecCode = ulParameter.ul;
                                FRAM_WriteDW(FMAddr_RecCode, gu32_RecCode);          //��������
                            }
                            else
                            {
                                gu8_RecInTime = 0;
                                ErrorDeal(0, 100);
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                        }
                        else
                        {
                            if (gu8_DataInLength == 6)      //�����������6λ
                            {
                                gu32_DataTemp = ulParameter.ul;      //�ݴ��һ����������ֵ
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                            else
                            {
                                gu8_RecInTime = 0;
                                ErrorDeal(0, 100);
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
            if (gu8_Item != REC_CHECKCODE)              //����У��ʱ������
                FRAM_WriteDW(GetRecipePort(gu8_Item), ulParameter.ul);         //�������õĹ�������ֵ
            if (gu8_Item == REC_F2 || gu8_Item == REC_F4 || gu8_Item == REC_F5 
             || gu8_Item == REC_F6 || gu8_Item == REC_F7 || gu8_Item == REC_F8 
             || gu8_Item == REC_F9 || gu8_Item == REC_FA)
            {
                if (ulParameter.ul != 0)                //���ش�ʱ����С��
                    gu8_Item = gu8_RecEnterItem[gu8_Item];
                else                                    //���عر�ʱ������һ������
                    gu8_Item = gu8_RecRightItem[gu8_Item];
            }
            else
            {
                gu8_Item  = gu8_RecEnterItem[gu8_Item ];
            }
            ulParameter.ul = GetRecipeItemValue(gu8_Item);           //�õ���Ӧ������ֵ
            gu8_DataInLength = 0;
            break;
    }
}


/*********************************************************************
��������: void Disp_RecipeState(void)
��    ��: �䷽������ʾ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-24
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_RecipeState(void)
{
    u8 luc_DispNumber = 0;       //������ʾλ��
    u8 luc_Point = 0;            //С����
    u32 luc_Unit = UNIT_none;             //��λ
    u16 lu16_RealRecItem = gu16_RecRealItem[gu8_Item];
    
    if (gu8_Item != 0)
        gb_FlashFlg = false;
        
    //Fxx����ʾ
    if (gu8_Item != REC_CHECKCODE && gu8_Item != DISP_RECNUM)
    {
        if (lu16_RealRecItem / 0x100 != 0)  //3.10 3.11��...
        {
            gu8_AuxDispArray[0] = Value_To_SEG[lu16_RealRecItem/0x100] | 0x80;
            gu8_AuxDispArray[1] = Value_To_SEG[lu16_RealRecItem%0x100/0x10];
            gu8_AuxDispArray[2] = Value_To_SEG[lu16_RealRecItem%0x10];
        }
        else if (lu16_RealRecItem % 0x10 == 0)  //�����F1/F2/F3...
        {
            gu8_AuxDispArray[1] = SEG_F;
            gu8_AuxDispArray[2] = Value_To_SEG[lu16_RealRecItem/0x10];
        }
        else    //С�� ��F1.1/F1.2...
        {
            gu8_AuxDispArray[0] = SEG_F;
            gu8_AuxDispArray[1] = Value_To_SEG[lu16_RealRecItem/0x10] | 0x80;
            gu8_AuxDispArray[2] = Value_To_SEG[lu16_RealRecItem%0x10];
        }
    }
    
    switch (gu8_Item)
    {
        case DISP_RECNUM:            //�䷽��
            VFD_AuxDisp_Str("REC");
            luc_Point = 0;
            luc_DispNumber = 2;
            break;
        case REC_CHECKCODE:         //��������
            Disp_InputCode();       //����������ʾ����
            gb_FlashFlg = true;
            break;
        case REC_F1:
            VFD_MainDisp_Str("U_LSET");
            break;
        case REC_F3:
            VFD_MainDisp_Str("TI SET");
            break;
        case REC_F1_1:              //
        case REC_F1_2:              //
        case REC_F1_3:              //
        case REC_F1_4:              //
        case REC_F1_5:              //
        case REC_F1_6:              //
        case REC_F2_1:
        case REC_F2_2:
            luc_Point = gs_Cal.Point;
            gb_FlashFlg = true;
            luc_DispNumber = 6;
            luc_Unit = gs_Cal.Unit;
            break;
        case REC_F2:
        case REC_F2_3:
        case REC_F4:
        case REC_F6:
        case REC_F8:
        case REC_F9:                //���뿪��
        case REC_F9_3:              //��������ж�Ͽ���
        case REC_FA:
            Disp_OnOff(ulParameter.ul);           //��ʾ����
            break;
        case REC_F5:
            Disp_FlapOnOff(ulParameter.ul);           //��ʾ����
            break;
        case REC_F4_2:              //���������Χ
            luc_DispNumber = 2;     // x.x ��λ����С����Ϊ1
            luc_Point = 1;
            gb_FlashFlg = true;
            break;
        case REC_F3_1:              //������ʱT1
        case REC_F3_2:              //��Ͷ��ֹ�Ƚ�ʱ��T2A
        case REC_F3_3:              //СͶ��ֹ�Ƚ�ʱ��T2B
        case REC_F3_4:              //������ʱ��T3
        case REC_F3_5:              //��ֵ����ʱ��T4
        case REC_F3_6:              //ж����ʱT5
        case REC_F3_7:              //�д���ʱT6
        case REC_F3_8:              //�ɴ���ʱT7
        case REC_F3_9:              //�����ʱT9         
        case REC_F3_10:             //��������Чʱ��T10
        case REC_F3_11:             //�ƴ���ʱT11        
        case REC_F3_12:             //�ƴ������Чʱ��T12
        case REC_F5_3:              //�Ĵ���ʱ
        case REC_F5_4:              //�Ĵ���Ч��ƽʱ��
        case REC_F5_5:              //�Ĵ���Ч��ƽʱ��
        case REC_F5_6:              //�����Ĵ������Чʱ��
        case REC_F6_1:              //�㶯��Ч��ƽʱ��
        case REC_F6_2:              //�㶯��Ч��ƽʱ��
        case REC_F8_1:              //ж����Чʱ��
        case REC_F8_2:              //ж�����ߵ�ƽ
        case REC_F8_3:              //ж�����͵�ƽ
        case REC_F9_1:              //����������ʱ
        case REC_F9_2:              //����ʱ��
            luc_DispNumber = 3;     //ʱ������ xx.x ��λ����С����Ϊ1
            luc_Point = 1;
            gb_FlashFlg = true;
            break;
        case REC_F5_1:              //�������Ĵ�����
        case REC_F5_2:              //��ֵ���Ĵ�����
        case REC_F4_1:              //�����������
        case REC_F7:                //������ϴ���
        case REC_F8_4:              //ж��������
            luc_DispNumber = 2;     //��λ��,С����λ��Ϊ0
            luc_Point = 0;
            gb_FlashFlg = true;
            break;
        case REC_F4_3:              //�����������
            luc_DispNumber = 1;     //��ʾ1λ�������á����޸ģ�
            break;
        case REC_FA_1:
            Disp_InputCode();       //����������ʾ����
            gu8_AuxDispArray[0] = SEG_F;
            gu8_AuxDispArray[1] = SEG_A | 0x80;
            gu8_AuxDispArray[2] = SEG_1;
            if (gb_SetFlg == true)
                gb_FlashFlg = true;
            break;
        default:
            break;
    }
    
    switch (gu8_Item)
    {
        case REC_CHECKCODE:
        case REC_FA_1:
            Flash_MainDisp(0x01);                //��˸����
        case REC_F1:
        case REC_F2:
        case REC_F2_3:
        case REC_F3:
        case REC_F4:
        case REC_F5:
        case REC_F6:
        case REC_F8:
        case REC_F9:
        case REC_F9_3:
        case REC_FA: 
            break;
        default:
            Disp_Main_Data(ulParameter.ul, luc_Point, luc_DispNumber, false, false);       //��ʾֵ
            Flash_MainDisp(0x01);            //��˸����
            VFD_UnitDisp(luc_Unit);              //��λ��ʾ
            break;
    }
}