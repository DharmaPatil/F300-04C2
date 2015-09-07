/*************************************************
Project:                GM8804C-2_ARM
File name:              Timer.c
Description:            定时器相关函数
Author:                 丛海旭
Version:                1.0
Date:                   2013-12-9
Others:                 
History:
*************************************************/
#include "include.h"

#define T0OCI_SIPL      7   //定时器中断优先级
//#define T0OCMPT_MS      10  //定时器输出比较时间(mS为单位)
//#define T0_CCR          15  //定时器0 预分频寄存器值
#define T0OCMPT_MS      1  //定时器输出比较时间(mS为单位)
#define T0_CCR          0  //定时器0 预分频寄存器值
//定时器0 输出比较A寄存器 初始值
//#define T0_OCAR         (u16)((T0OCM PT_MS)*RCCU_FrequencyValue(RCCU_PCLK2)/(TIM_PrescalerValue(TIM0)+1)/1000)
//44236800/1000=44236 减小误差 +1 = 44237 计数器CNTR是从FFFC开始的所以再加3
#define T0_OCAR         (u16)((T0OCMPT_MS)*RCCU_FrequencyValue(RCCU_PCLK2)/(TIM_PrescalerValue(TIM0)+1)/1000-2)

vu32 gu32_Sys10ms;               //系统计时器，一定要定义为volatile类型，防止编译器优化时出错
vu32 gu32_Sys1ms;   //1ms
vu32 gu32_Sys01ms;  //0.1ms




//TO输出比较A产生1ms中断
void Timer0_Init(void)
{
    gu32_Sys10ms = 0;
    gu32_Sys1ms = 0;

    TIM_Init(TIM0);     //初始化定时器T0
    TIM_ClockSourceConfig(TIM0,TIM_INTERNAL);   //设置定时器时钟源为内部时钟
    TIM_PrescalerConfig(TIM0,T0_CCR);   //配置分频因子值
    TIM_OCMPModeConfig(TIM0, TIM_CHANNEL_A,T0_OCAR, TIM_TIMING, TIM_LOW);
    TIM_ITConfig(TIM0,TIM_OCA_IT,ENABLE);   //输出比较模式
    EIC_IRQChannelPriorityConfig(T0OC1_IRQChannel , T0OCI_SIPL);    //
    EIC_IRQChannelConfig(T0OC1_IRQChannel,ENABLE);  //使能T0中断
    TIM_CounterConfig(TIM0,TIM_START);  //开始计时
}


#define T1OCI_SIPL      6   //定时器中断优先级
#define T1OCMPT_MS      1  //定时器输出比较时间(0.1mS为单位)
#define T1_CCR          0  //定时器1 预分频寄存器值
#define T1_OCAR         (u16)((T1OCMPT_MS)*RCCU_FrequencyValue(RCCU_PCLK2)/(TIM_PrescalerValue(TIM1)+1)/10000-2)


/*********************************************************************
函数名称: void Timer1_Init(void)
功    能: T1初始化
说    明: 供智能预测SP2截至点时间使用 产生0.1ms的中断
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-23
修    改:                      时    间: 
*********************************************************************/
void Timer1_Init(void)
{
    TIM_Init(TIM1);     //初始化定时器T0
    TIM_ClockSourceConfig(TIM1,TIM_INTERNAL);   //设置定时器时钟源为内部时钟
    TIM_PrescalerConfig(TIM1,T1_CCR);   //配置分频因子值
    TIM_OCMPModeConfig(TIM1, TIM_CHANNEL_A, T1_OCAR, TIM_TIMING, TIM_LOW);
    TIM_ITConfig(TIM1,TIM_OCA_IT,ENABLE);   //输出比较模式
    EIC_IRQChannelPriorityConfig(T1TIMI_IRQChannel , T1OCI_SIPL);    //
}

/*********************************************************************
函数名称: void Sp2StopTimeStart(bool* fbp_TimeRunFlg)
功    能: 启动预测SP2截止点的时间计时
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-23
修    改:                      时    间: 
*********************************************************************/
void Sp2StopTimeStart(bool* fbp_TimeRunFlg)
{
    gu32_Sys01ms = 0;
    *fbp_TimeRunFlg = true;
    EIC_IRQChannelConfig(T1TIMI_IRQChannel,ENABLE);  //使能T1中断
    TIM_CounterConfig(TIM1,TIM_START);  //开始计时
    gs_DebugRunTime.u32PredictSp2StopTime = gu32_Sp2StopTime01ms;   //保存预测的中投截至时间
//    BuzzerStart(30);
}

/*********************************************************************
函数名称: void Sp2StopTimeStop(bool* fbp_TimeRunFlg)
功    能: 停止SP2预测截止点的计时
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-24
修    改:                      时    间: 
*********************************************************************/
void Sp2StopTimeStop(bool* fbp_TimeRunFlg)
{
    gu32_Sys01ms = 0;
    gu32_Sp2StopTime01ms = 0;
    *fbp_TimeRunFlg = false;
    TIM_CounterConfig(TIM1,TIM_STOP);  //停止计时
    TIM_CounterConfig(TIM1,TIM_CLEAR);  //清除计数器
    TIM_FlagClear(TIM1,TIM_OCFA);   //清中断标志
    EIC_IRQChannelConfig(T1TIMI_IRQChannel,DISABLE);  //不使能T1中断
}

/*********************************************************************
函数名称: void StopSp2ByT1(void)
功    能: 预测中投截止点的时间到，停止SP2
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-27
修    改:                      时    间: 
*********************************************************************/
void StopSp2ByT1(void)
{
    u8 i;
    gs_DebugRunTime.s32SP2RealStopWt = gs_Rec.Target - gs_Rec.Middle;   //实际中投截止重量 预测停止认为和设置的截止点相同
    //中加加时间
    gs_DebugRunTime.u32Sp2Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp2TimeTemp;
    //记录慢加启动时刻
    gs_DebugRunTime.u32Sp3TimeTemp = gu32_Sys1ms;
    gb_Sp2Flg = false;
    gb_Sp3Flg = true;
    gu32_Process = SMALLPROCESS;
    gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
#ifdef DEBUG_SMARTT2B
    gu32_Time1_5 = gu32_Sys1ms;                //记录当前时间作为T2启动时间
#else
    gb_TimeRunFlg = true;
    gu32_TimeBuf = gs_Rec.TimeT2b * 10;          //读禁止比较时间
    gu32_Time1_5 = gu32_Sys10ms;                //记录当前时间作为T2启动时间
#endif

#ifdef DEBUG_WT_SEND
    gu8_DebugState2 = SP3START;
#endif
    for (i = 1; i <= IO_PORT_OUTMAX; i++)
    {
        switch (*gu32p_IoOutBuf[i])
        {
            case 4:         //中投
                switch (i)
                {
                    case 1:
                        OUT1_CLR;
                        break;
                    case 2:
                        OUT2_CLR;
                        break;
                    case 3:
                        OUT3_CLR;
                        break;
                    case 4:
                        OUT4_CLR;
                        break;
                    case 5:
                        OUT5_CLR;
                        break;
                    case 6:
                        OUT6_CLR;
                        break;
                    case 7:
                        OUT7_CLR;
                        break;
                    case 8:
                        OUT8_CLR;
                        break;
                    case 9:
                        OUT9_CLR;
                        break;
                    case 10:
                        OUT10_CLR;
                        break;
                    case 11:
                        OUT11_CLR;
                        break;
                    case 12:
                        OUT12_CLR;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
        
        
        
        
    
    
}

/*********************************************************************
函数名称: void T1_IRQ(void)
功    能: 
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-23
修    改:                      时    间: 
*********************************************************************/
void T1_IRQ(void)
{
    gu32_Sys01ms++;
    TIM_CounterConfig(TIM1,TIM_CLEAR);  //清除计数器
    TIM_FlagClear(TIM1,TIM_OCFA);   //清中断标志
    if (gu32_Sys01ms >= gu32_Sp2StopTime01ms)
    {   //时间到
        TIM_CounterConfig(TIM1,TIM_STOP);  //停止计时
        TIM_CounterConfig(TIM1,TIM_CLEAR);  //清除计数器
        EIC_IRQChannelConfig(T1TIMI_IRQChannel,DISABLE);  //不使能T1中断
        StopSp2ByT1();
        gu32_Sys01ms = 0;
        gu32_Sp2StopTime01ms = 0;
        gb_Sp2StopTimeRunFlg = false;
    }
}


