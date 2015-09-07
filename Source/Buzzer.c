/*************************************************
Project:                GM8804C-2_ARM
File name:              Buzzer.c
Description:            ��������غ���
Author:                 �Ժ���
Version:                1.0
Date:                   2013-12-4
Others: 
History:
*************************************************/
#include "include.h"

u8  u8BuzerMode;
u32 u32BuzerHoldTime;
u32 u32BuzerStarTime;
bool gb_BuzzerAlwaysOnFlg;


/*********************************************************************
��������: void Buzzer_Init(void)
��    ��: ��������ʼ��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-4
��    ��:                      ʱ    ��: 
*********************************************************************/
void Buzzer_Init(void)
{
    GPIO_Config(GPIO1, (1<<0), GPIO_OUT_PP);
    GPIO_BitWrite(GPIO1, 0, 0);               //��������
    gb_BuzzerAlwaysOnFlg = false;
    u8BuzerMode = 0;
    u32BuzerHoldTime = 0;
    u32BuzerHoldTime = 0;
}

/*********************************************************************
��������: void Buzzer_PowerOn(void)
��    ��: �ϵ��������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-11
��    ��:                      ʱ    ��: 
*********************************************************************/
void Buzzer_PowerOn(void)
{
    BuzzerOn();
    delay(40);
    BuzzerOff();
    delay(15);

    BuzzerOn();
    delay(40);
    BuzzerOff();
}

void BuzzerS(void)
{
    BuzzerOn();
    delay(200);
    BuzzerOff();
}

//void BuzerL(void)
//{
//    BuzzerOn();
//    delay(400);
//    BuzzerOff();
//}

void OPBuzzer(void)
{
    if (gb_BuzzerAlwaysOnFlg == true)
    {
        BuzzerOn();
    }
    else if (u8BuzerMode != 0)
    {
        BuzzerOn();
        if (gu32_Sys10ms - u32BuzerStarTime > u32BuzerHoldTime)
        {
            BuzzerOff();
            u8BuzerMode = 0;
        }
    }
    else if (gb_BatchOverFlg || gb_Error9Flg || ((gb_OverFlg || gb_UnderFlg) && gb_RunFlg))
        BuzzerOn();
    else
        BuzzerOff();
}

/*********************************************************************
��������: void BuzzerStart(u32 fu32_HoldTime)
��    ��: ������������fu32_HoldTime��10ms
˵    ��: 
��ڲ���: fu32_HoldTime:����ʱ�䣬Ϊ0ʱһֱ�챨��
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-11
��    ��:                      ʱ    ��: 
*********************************************************************/
void BuzzerStart(u32 fu32_HoldTime)
{
    BuzzerOn();
    if (fu32_HoldTime != 0)
    {
        u8BuzerMode = 1;
        u32BuzerHoldTime = fu32_HoldTime;
        u32BuzerStarTime = gu32_Sys10ms;
    }
    else
    {
        gb_BuzzerAlwaysOnFlg = true;
    }
}


