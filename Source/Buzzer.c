/*************************************************
Project:                GM8804C-2_ARM
File name:              Buzzer.c
Description:            蜂鸣器相关函数
Author:                 丛海旭
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
函数名称: void Buzzer_Init(void)
功    能: 蜂鸣器初始化
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-4
修    改:                      时    间: 
*********************************************************************/
void Buzzer_Init(void)
{
    GPIO_Config(GPIO1, (1<<0), GPIO_OUT_PP);
    GPIO_BitWrite(GPIO1, 0, 0);               //蜂鸣器关
    gb_BuzzerAlwaysOnFlg = false;
    u8BuzerMode = 0;
    u32BuzerHoldTime = 0;
    u32BuzerHoldTime = 0;
}

/*********************************************************************
函数名称: void Buzzer_PowerOn(void)
功    能: 上电短响两声
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-11
修    改:                      时    间: 
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
函数名称: void BuzzerStart(u32 fu32_HoldTime)
功    能: 启动蜂鸣器响fu32_HoldTime×10ms
说    明: 
入口参数: fu32_HoldTime:响声时间，为0时一直响报警
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-11
修    改:                      时    间: 
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


