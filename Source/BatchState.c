/*
Description:            批次设置状态下相关功能函数
Author:                 丛海旭
Date:                   2013-12-9
*/

#include "include.h"

u32 gu32_BatchTime;             //批次数
u32 gu32_BatchTimeCnt;          //剩余批次数



/*********************************************************************
函数名称: void BatchStateDeal(void)
功    能: 批次数设置状态相关处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-4-2
修    改:                      时    间: 
*********************************************************************/
void Key_BatchState(void)
{
    if (gb_SetFlg == true)         //设置状态
    {
        gb_FlashFlg = true;
        //数据输入，散料秤模式下发货总量为9位
        ulParameter.ul = Key_NumInput(ulParameter.ul, 4);
        switch (gu32_KeyCode)
        {
            case K_ENTER:               //确认键保存批次数
                gb_SetFlg = false;
                gu32_BatchTime = ulParameter.ul;
                gu32_BatchTimeCnt = gu32_BatchTime;
                FRAM_WriteDW(FMAddr_BatchTime, gu32_BatchTime);
                FRAM_WriteDW(FMAddr_BatchTimeCnt, gu32_BatchTimeCnt);
                break;
            case K_ESC:                 //退出设置状态
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
                gb_SetFlg = true;              //进入设置状态
                break;
            default:
                break;
        }
    }
}

/*********************************************************************
函数名称: void Disp_BatchState(void)
功    能: 批次数设置相关显示处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-4-2
修    改:                      时    间: 
*********************************************************************/
void Disp_BatchState(void)
{
    VFD_AuxDisp_Str("BAT");
    Disp_Main_Data(ulParameter.ul, 0, 4, false, false);       //A显示值
    Flash_MainDisp(0x01);            //闪烁处理
    VFD_UnitDisp(UNIT_PCS); //pcs显示
}








