/*
Description:            ��������״̬����ع��ܺ���
Author:                 �Ժ���
Date:                   2013-12-9
*/

#include "include.h"

u32 gu32_BatchTime;             //������
u32 gu32_BatchTimeCnt;          //ʣ��������



/*********************************************************************
��������: void BatchStateDeal(void)
��    ��: ����������״̬��ش���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-4-2
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_BatchState(void)
{
    if (gb_SetFlg == true)         //����״̬
    {
        gb_FlashFlg = true;
        //�������룬ɢ�ϳ�ģʽ�·�������Ϊ9λ
        ulParameter.ul = Key_NumInput(ulParameter.ul, 4);
        switch (gu32_KeyCode)
        {
            case K_ENTER:               //ȷ�ϼ�����������
                gb_SetFlg = false;
                gu32_BatchTime = ulParameter.ul;
                gu32_BatchTimeCnt = gu32_BatchTime;
                FRAM_WriteDW(FMAddr_BatchTime, gu32_BatchTime);
                FRAM_WriteDW(FMAddr_BatchTimeCnt, gu32_BatchTimeCnt);
                break;
            case K_ESC:                 //�˳�����״̬
                gb_SetFlg = false;
                ulParameter.ul = gu32_BatchTime;
                break;
            default:
                break;
        }
    }
    else
    {
        gb_FlashFlg = false;
        gb_FirstNumKeyFlg = true;
        ulParameter.ul = gu32_BatchTime;
        switch (gu32_KeyCode)
        {
            case K_ESC:
                gu32_SystemState = STOPSTATE;
                break;
            case K_SET:
                gb_SetFlg = true;              //��������״̬
                break;
            default:
                break;
        }
    }
}

/*********************************************************************
��������: void Disp_BatchState(void)
��    ��: ���������������ʾ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2011-4-2
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disp_BatchState(void)
{
    VFD_AuxDisp_Str("BAT");
    Disp_Main_Data(ulParameter.ul, 0, 4, false, false);       //A��ʾֵ
    Flash_MainDisp(0x01);            //��˸����
    VFD_UnitDisp(UNIT_PCS); //pcs��ʾ
}








