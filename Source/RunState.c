/*
Description:            运行状态下相关功能函数
Author:                 丛海旭
Date:                   2013-12-9
*/
#include "include.h"
#include "MathLogic.h"
#include "Adaptation.h"


u32 gu32_Process;                 //运行时的加料状态

bool gb_TimeRunFlg;              //延时已经启动标志位
u32 gu32_TimeBuf;                //延时计时器缓存，保存设置的时间值
u32 gu32_Time1_5;                //T1~T5的计时器缓存，保存启动时刻的时间

bool gb_FlapBagOutFlg;             //拍袋输出有效标志
bool gb_FlapWaitFlg;               //拍袋之前的等待标志
bool gb_FlapStartFlg;              //拍袋已经开始标志
bool gb_FlapAllHighFlg;            //低电平时间为0,拍袋恒定有效标志位
u32 gu32_TimeFlapBuf;               //拍袋用时间寄存器
u32 gu32_Time8_9;                   //t8和t10用延时寄存器
u32 gu32_FlapBagNumCnt;             //拍袋计数器
bool gb_Sp1OverFlg;              //大投设置无效标志
bool gb_Sp2OverFlg;              //中投设置无效标志
bool gb_AddFlapBagFlg;                //加料中拍袋标志位
bool gb_ExtFlapBagFlg;                //额外拍袋输出标志位
bool gb_AddFlapBagEndFlg;             //加料中拍袋已经完成或者已启动标志位
u32 gu32_DiscShakeTimeBuf;   //秤卸料震打时间寄存器
bool gb_TareGetedFlg;       //皮重已经获取标志位
u32 gu32_OUDelayTimeBuf;         //超欠差暂停关闭时的超欠差报警计时寄存器
u32 gu32_DiscShakeCnt;   //卸料震打计数
bool gb_Error9Flg;                    //卸料未干净报警
bool gb_SumFlg;                 //累计
u32 gu32_SumTemp;           //累计的缓存值
bool gb_DiscWtGetedFlg;       //定值重量已经获取
u32 gu32_DiscValue;     //本次卸料值
u32 gu32_HoldWt;        //定值保持重量
u32 gu32_WeighTimeCnt;      //称量次数
u32 gu32_TargetTemp;        //目标值的缓存
bool gb_LiftBagStartFlg;    //启动提袋
bool gb_PushBagStartFlg;    //启动推袋
bool gb_ConveyorStartFlg;   //启动输送机
bool NO2Wait(u32 fu32_No2PackOffTwoSwitch);

STATISTIC_DATA gs_Statistic; //统计数据
PREDICTSP3 gs_PredictSp3;
PREDICTSP3 gs_PredictSp3Temp;

DEBUG_RUN_TIME gs_DebugRunTime;

#ifdef DEBUG_MONITORTIME_PRINT
    DEBUG_RUN_TIME gs_DebugRunTimeTemp; //打印的时候使用作为gs_DebugRunTime的缓存
    u32 gu32_DebugTarget;
    u32 gu32_DebugLarge;
    u32 gu32_DebugMiddle;
    u32 gu32_DebugSmall;
#endif

struct_autostudy_t gs_AutoStd = {0,0,0,0,0,0,0,0,0,0,0};

bool JudgeAutoStudy(void);
void AutoStudy_SP2Start(void);


#ifdef DEBUG_MONITORTIME_PRINT
/*********************************************************************
函数名称: void RefreshDebugRec(void)
功    能: 刷新调试用的大中小投目标值数据
说    明: 每次加料开始时刷新一次，否则直接用系统的配方变量，在运行时修改配方时有可能
          发送出来的数据和当次调试结果是对不上的
入口参数: 
返 回 值: 
设    计: chx                  时    间: 2014-05-27
修    改:                      时    间: 
*********************************************************************/
void RefreshDebugRec(void)
{
    gu32_DebugTarget = gs_Rec.Target;
    gu32_DebugLarge = gs_Rec.Large;
    gu32_DebugMiddle = gs_Rec.Middle;
    gu32_DebugSmall = gs_Rec.Small;
}

#endif
/*********************************************************************
函数名称: void UpdateFastSetRec(void)
功    能: 更新快速设置的配方
说    明: 
入口参数: fb_UpTotalFlg更新总目标值 fb_UpSingleFlg更新单秤目标值 AorB更新A或B的大中小投
返 回 值: s
设    计: 丛海旭               时    间: 2013-7-23
修    改:                      时    间: 
*********************************************************************/
void UpdateFastSetRec(void)
{
    u8 i;
    
    if (gu8_FastSetSigTemp != 0)
    {
        for (i = 0; i <= 5; i++)
        {
            if (gu8_FastSetSigTemp & (0x01 << i))
            {
                ulParameter.ul = gu32_FastSetTemp[i];
                FRAM_WriteDW(GetRecipePort(i + 3), ulParameter.ul);
            }
        }
        gu8_FastSetSigTemp = 0;
    }
}

/*********************************************************************
函数名称: void Over_Under(u32 ful_Target, u32 ful_Result)
功    能: 超欠差处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void Over_Under(u32 ful_Target, u32 ful_Result)
{
    if (gs_Rec.OUSwitch != 0)       //超欠差检测开关打开进行超欠差检测
    {
        if (gb_OflFlg)
            gb_OverFlg = true;       //溢出认为超差
        else if (ful_Result >= ful_Target + gs_Rec.OverValue)
        {
            gb_OverFlg = true;       //定值重量大于目标值+超量值 则超差
            gb_UnderFlg = false;
        }
        else if ((ful_Result <= ful_Target - gs_Rec.UnderValue) && (ful_Target > gs_Rec.UnderValue))
        {
            gb_UnderFlg = true;      //定值重量小于目标值-欠量值 则欠差
            gb_OverFlg = false;
        }
    }
    if (gb_OverFlg || gb_UnderFlg)       //超欠差
    {
        if (gs_Rec.OUStopSwitch == 0)       //超欠差暂停开关关闭
        {
            BuzzerStart(100);   //蜂鸣器响
//            if (gb_BatchOverFlg == 0)         //批次完成没在报警则清报警 for test
//                gb_AlarmFlg = false;

            //清超欠差 置超欠差暂停关闭时的超欠差标志
            gb_OUOverFlg = gb_OverFlg;
            gb_OUUnderFlg = gb_UnderFlg;
            gb_OverFlg = false;
            gb_UnderFlg = false;
            gu32_OUDelayTimeBuf = gu32_Sys10ms;        //保存报警时间
        }
        else
        {
            ErrorDeal(8, 0);        //error8
            gb_AlarmFlg = true;
        }
    }
}

/*********************************************************************
函数名称: void StatisticClrStruct(void)
功    能: 清除统计结构体
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-17
修    改:                      时    间: 
*********************************************************************/
void StatisticClrStruct(void)
{
    u32* lu32p_Start = &gs_Statistic.u32StartTime;
    u8 i;
    
    for (i = 0; i < sizeof(gs_Statistic) / sizeof(gs_Statistic.u32StartTime); i++)
    {
        *(lu32p_Start+i) = 0;
    }
}

/*********************************************************************
函数名称: void StatisticSaveStartTime(u32 fu32_Time)
功    能: 保存启动时刻
说    明: 
入口参数: fu32_Time=系统时间10ms
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-17
修    改:                      时    间: 
*********************************************************************/
void StatisticSaveStartTime(u32 fu32_Time)
{
    gs_Statistic.u32StartTime = fu32_Time;
}

/*********************************************************************
函数名称: void StatisticDeal(u32 fu32_Result)
功    能: 统计数据计算处理
说    明: 
入口参数: fu32_Result=本秤结果
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-17
修    改:                      时    间: 
*********************************************************************/
void StatisticDeal(u32 fu32_Result)
{
    #define TARGET  gs_Rec.Target
    #define OVER    gs_Rec.OverValue
    #define UNDER   gs_Rec.UnderValue
    #define SUMNUM  gu32_DebugSumNum
    #define SYSTIME gu32_Sys10ms
    
    //总共用时
    gs_Statistic.u32TotalTime = SYSTIME - gs_Statistic.u32StartTime;
    //完成包数
    gs_Statistic.u32SumNum = SUMNUM;
    //完成重量
    gs_Statistic.u32SumWt += fu32_Result;
    //每小时包数
    gs_Statistic.u32NumAHour = ((double)360000) / gs_Statistic.u32TotalTime * gs_Statistic.u32SumNum + 0.5;
    //每小时重量
    gs_Statistic.u32WtAHour = gs_Statistic.u32NumAHour * TARGET;
    //每分钟包数 1小数点
    gs_Statistic.u32NumAMin = ((double)60000) / gs_Statistic.u32TotalTime * gs_Statistic.u32SumNum + 0.5;
    //平均每包重量
    gs_Statistic.ResultAverage = gs_Statistic.u32SumWt / gs_Statistic.u32SumNum;
    //最高超差值 对应包数
    if ((fu32_Result > TARGET + OVER) && (fu32_Result > gs_Statistic.u32MostOverResult))
    {
        gs_Statistic.u32MostOverResult = fu32_Result;
        gs_Statistic.u32MostOverNum = SUMNUM;
    }
    //最低欠差值 对应包数
    if ((fu32_Result < TARGET - UNDER) && (fu32_Result < gs_Statistic.u32MostUnderResult || gs_Statistic.u32MostUnderResult == 0))
    {
        gs_Statistic.u32MostUnderResult = fu32_Result;
        gs_Statistic.u32MostUnderNum = SUMNUM;
    }
    //最高值
    if (fu32_Result > gs_Statistic.u32TheMaxValue)
    {
        gs_Statistic.u32TheMaxValue = fu32_Result;
    }
    //最低值
    if (fu32_Result < gs_Statistic.u32TheMinValue || gs_Statistic.u32MostUnderResult == 0)
    {
        gs_Statistic.u32TheMinValue = fu32_Result;
    }
    //误差带
    if (gs_Statistic.u32TheMaxValue != 0 && gs_Statistic.u32TheMinValue != 0 && gs_Statistic.u32TheMaxValue > gs_Statistic.u32TheMinValue)
    {
        gs_Statistic.u32ErrorBand = gs_Statistic.u32TheMaxValue - gs_Statistic.u32TheMinValue;
    }
    //重量分布情况 包数
    if (fu32_Result < TARGET - UNDER)
        gs_Statistic.u32Dst_Under_Num++;
    else if (fu32_Result >= TARGET - UNDER && fu32_Result < TARGET - 2 * ONESTEP)
        gs_Statistic.u32Dst_U_2_Num++;
    else if (fu32_Result >= TARGET - 2 * ONESTEP && fu32_Result < TARGET - ONESTEP)
        gs_Statistic.u32Dst_2_1_Num++;
    else if (fu32_Result >= TARGET - ONESTEP && fu32_Result < TARGET)
        gs_Statistic.u32Dst_1_0_Num++;
    else if (fu32_Result >= TARGET && fu32_Result <= TARGET + ONESTEP)
        gs_Statistic.u32Dst_0_1_Num++;
    else if (fu32_Result > TARGET + ONESTEP && fu32_Result <= TARGET + 2 * ONESTEP)
        gs_Statistic.u32Dst_1_2_Num++;
    else if (fu32_Result > TARGET + 2 * ONESTEP && fu32_Result <= TARGET + OVER)
        gs_Statistic.u32Dst_2_O_Num++;
    else if (fu32_Result > TARGET + OVER)
        gs_Statistic.u32Dst_Over_Num++;
    //百分比    1位小数点
    gs_Statistic.u32Dst_Under_Per = ((float)gs_Statistic.u32Dst_Under_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_U_2_Per = ((float)gs_Statistic.u32Dst_U_2_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_2_1_Per = ((float)gs_Statistic.u32Dst_2_1_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_1_0_Per = ((float)gs_Statistic.u32Dst_1_0_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_0_1_Per = ((float)gs_Statistic.u32Dst_0_1_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_1_2_Per = ((float)gs_Statistic.u32Dst_1_2_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_2_O_Per = ((float)gs_Statistic.u32Dst_2_O_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_Over_Per = ((float)gs_Statistic.u32Dst_Over_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    
    
    #undef TARGET
    #undef OVER
    #undef UNDER
    #undef SUMNUM
    #undef SYSTIME
}

/*********************************************************************
函数名称: void OneTareOver(void)
功    能: 一秤结束，在下一秤开始之前的处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void OneTareOver(void)
{
    if (gu32_BatchTime != 0)
    {   //单秤组合最后一秤批次才减一次
        if (gu32_WeighTimeCnt <= 1 || (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B))
        {
            gu32_BatchTimeCnt--;
            if (gb_BatchOverFlg || gu32_BatchTimeCnt == 0 || (gu32_BatchTimeCnt & 0x80000000) != 0)
            {
                ErrorDeal(1, 0);        //error1
                gb_AlarmFlg = true;
                gb_BatchOverFlg = true;
                gu32_BatchTimeCnt = 0;
            }
            FRAM_WriteDW(FMAddr_BatchTimeCnt, gu32_BatchTimeCnt);
        }
    }
    if (gu32_WeighTimeCnt <= 1 || (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B))
        UpdateFastSetRec(); //更新快速设置数值
    
    if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
    {
        if (gu32_SumTemp != 0 && gu32_WeighTimeCnt <= 1)
        {   
            //Modbus通讯中使用，上秤结果
            gu32_MbLastResult = gu32_SumTemp;
            //单秤组合最后一秤才累计
        #ifndef DONOT_SAVE_SUM
            Save_SumData(gu32_SumTemp, 1);            //保存累计数据
        #endif
        #ifndef DEBUG_MONITORTIME_PRINT
            //自动打印
            PrintStart(PRINT_AUTO, gu32_SumTemp, 0);
        #endif
            gu32_SumTemp = 0;
        }
        //单秤组合次数到后重新赋值
        if ((--gu32_WeighTimeCnt) == 0 || (gu32_WeighTimeCnt & 0x80000000) != 0)
            gu32_WeighTimeCnt = gs_Rec.CombinedNum;
        if (gu32_WeighTimeCnt == 1 && gs_Rec.CombinedNum > 1)
        {   //单秤组合次数大于1,并且下一秤是组合的最后一秤，计算剩余的目标值
            if (gs_Rec.Target * gs_Rec.CombinedNum > gu32_SumTemp)  //累计没有超过目标值
                gu32_TargetTemp = gs_Rec.Target * gs_Rec.CombinedNum - gu32_SumTemp;
            else    //累计已经过量停止
                RunStopInit(false);
        }
        else    //其他情况（单秤组合次数小于等于1或者组合没有到最后一秤）
            gu32_TargetTemp = gs_Rec.Target;
    }
    else    //无斗模式更新目标值 v1.01
        gu32_TargetTemp = gs_Rec.Target;
    if (gb_EndFlg)      //结束输入有效
    {
        gu32_Process = 0;
        RunStopInit(false);
    }
    gb_AddFlapBagEndFlg = false;
    if (gb_AlarmFlg == false && gu32_Process != 0)
    {
    #ifdef DEBUG_WT_SEND
        gu8_DebugState2 = T1START;
    #endif
        gu32_Process = T1PROCESS;
        gu32_TimeBuf = gs_Rec.TimeT1 * 10;
        gu32_Time1_5 = gu32_Sys10ms;
        gb_TimeRunFlg = false;
        //一次加料时间
        gs_DebugRunTime.u32OneTareTime = gu32_Sys1ms - gs_DebugRunTime.u32RunStartTimeTemp;
        gs_DebugRunTime.u32RunStartTimeTemp = gu32_Sys1ms; //记录启动时刻
        //总时间
        gs_DebugRunTime.u32TotalTime = gu32_Sys1ms - gs_DebugRunTime.u32FirstRunTimeTemp;
        //次数+1
        gs_DebugRunTime.u32SumCnt++;
    #ifdef DEBUG_MONITORTIME_PRINT    //发送调试相关数据
        gs_DebugRunTimeTemp = gs_DebugRunTime;
        gs_PredictSp3Temp = gs_PredictSp3;
        gu32_DebugSumNumTemp = gu32_DebugSumNum;
        
        PrintStart(PRINT_AUTO, gu32_HoldWt, 0);
    #endif
        gs_DebugRunTime.bGetRunCmpTimeFlg = false;
        gs_DebugRunTime.s32MaxKFedReLinear = 0;
        if(gs_DebugRunTime.u32TotalTime > 60000)
        {   //超过1分钟重新计时计数
            gs_DebugRunTime.u32TotalTime = 0;
            gs_DebugRunTime.u32FirstRunTimeTemp = gu32_Sys1ms;
            gs_DebugRunTime.u32SumCnt = 0;
        }
        PredictSp3_Restart();
        StatisticDeal(gu32_HoldWt);
        gu32_DebugSumNum++;
    }
    else
        gu32_Process = 0;
}

/*********************************************************************
函数名称: void Deal_WeighTime_Sum(void)
功    能: 夹袋完成后的称量次数和累计的相关处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void Deal_WeighTime_Sum(void)
{
    if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
    {
        if (gb_SigWtFlg == false)   //重量正数时才累计
        {   //有可能单秤组合次数不为0，所以此处用+=
            gu32_SumTemp += gu32_Wt;  //保存累计重量，卸料完成后再累计
            gu32_DiscValue = gu32_Wt;  //定值重量
        }
        else
            gu32_SumTemp += 0;
    }
    else    //无斗秤和散料秤的累计
    {
        if (gb_SigWtFlg == false)   //重量正数时才累计
        {
            gu32_SumTemp = gu32_Wt;  //保存累计重量，卸料完成后再累计
            gu32_DiscValue = gu32_Wt;  //定值重量
        }
        else
            gu32_SumTemp = 0;
    }
}

/*********************************************************************
函数名称: bool CorrectFall(void)
功    能: 落差修正
说    明: 
入口参数: 
返 回 值: 0落差修正完成或不需要落差修正  1落差修正未完成
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
bool CorrectFall(void)
{
    bool  lb_Quit = false;
    bool  lb_SigFallWt;
    u32 lul_CorrectFallRng2Tab[] = {1, 1, 2, 4};
    u32 lul_Temp;
    
    if (gs_Rec.CorrectFallSwitch)          //落差修正开关打开
    {
        if (gb_StabFlg || StabOver(false))
        {   //稳定或判稳超时时间到
            StabOver(true); //复位判稳超时参数
            if (gb_OflFlg == false)
            {
                lul_Temp = abs(gu32_TargetTemp - gu32_Wt) * 1000;
                // |目标值-当前值|>目标值*落差修正范围 超过落差修正范围不修正
                if (lul_Temp > gu32_TargetTemp * gs_Rec.CorrectFallRng1)
                    return false;
                    
                //落差修计算正处理
                gu32_CorrectFallWt += (gu32_TargetTemp - gu32_Wt);
                if (gs_Rec.CorrectFallTime != 0)       //落差修正次数不为零进行落差修正
                {
                    if (--gu32_CorrectFallTimeCnt == 0 || gu32_CorrectFallTimeCnt & 0x80000000)        //落差修正次数到
                    {
                        lb_SigFallWt = false;
                        if (gu32_CorrectFallWt & 0x80000000)           //落差累计值为负
                        {
                            gu32_CorrectFallWt = -gu32_CorrectFallWt;
                            lb_SigFallWt = true;
                        }
                        gu32_CorrectFallWt = gu32_CorrectFallWt / (lul_CorrectFallRng2Tab[gs_Rec.CorrectFallRng2] * gs_Rec.CorrectFallTime);
                        if (lb_SigFallWt)
                            gs_Rec.Small += gu32_CorrectFallWt;     //修改落差值
                        else
                            gs_Rec.Small -= gu32_CorrectFallWt;     //修改落差值
                        //负落差处理
                        if (gs_Rec.Small & 0x80000000)
                        {
                            gs_Rec.Small = 0;
                        }
                        else if ((gb_Sp2OverFlg == false) && (gs_Rec.Small > gs_Rec.Middle))
                        {
                            gs_Rec.Small = gs_Rec.Middle;     //落差值大于中投值处理
                        }
                        else if ((gb_Sp1OverFlg == false) && (gs_Rec.Small > gs_Rec.Large))
                        {
                            gs_Rec.Small = gs_Rec.Large;      //中投设置无效，落差值大于大投值处理
                        }
                        FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);  //落差存入铁电实时保存
                        //清除落差修正累计值，落差修正检测计数器复位，准备下一轮落差修正
                        gu32_CorrectFallWt = 0;
                        gu32_CorrectFallTimeCnt = gs_Rec.CorrectFallTime;
                    }
                }
                else
                {
                    gu32_CorrectFallWt = 0;
                    gu32_CorrectFallTimeCnt = gs_Rec.CorrectFallTime;
                }
            }
        }
        else    //不稳定时则等待
            return true;
    }
    return lb_Quit;
}

/*********************************************************************
函数名称: void GetTare(u32 fu32_Tare)
功    能: 去皮操作
说    明: 包含皮重记忆等功能处理，皮重大于当前重量则重新去皮，否则不重新去皮
入口参数: fu32_Tare:皮重值，4倍分度数
          fb_TareGetFlg:是否进行皮重记忆的处理
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void GetTare(u32 fu32_Tare, bool fb_SigFlg, bool fb_TareGetFlg)
{
    if (gb_TareGetedFlg == false || fb_TareGetFlg == false)
    {   //如果之前没有保存过皮重值，那么按当前重量当做皮重去皮
        //fb_TareGetFlg=false表示不进行皮重记忆的处理，直接按当前重量去皮
        gu32_TareNquad = fu32_Tare;    //存储皮重值,4倍分度数
        gb_TareSigFlg = fb_SigFlg; //存储皮重值的正负号
        gb_TareGetedFlg = true;
    }
    else
    {   //之前有保存过的皮重
        if (fb_SigFlg == true)
        {   //当前毛重为负数之前存储的皮重也为负数
            if (gb_TareSigFlg == true)
            {   //皮重负的少说明当前重量小于保存的皮重，则重新去皮
                if (gu32_TareNquad < fu32_Tare)
                {
                    gu32_TareNquad = fu32_Tare;    //存储皮重值,4倍分度数
                    gb_TareSigFlg = fb_SigFlg; //存储皮重值的正负号
                    gb_TareGetedFlg = true;
                }
            }
            else
            {   //当前毛重为负数，之前存储的皮重为正数，则当前毛重小于保存的皮重，重新去皮
                gu32_TareNquad = fu32_Tare;    //存储皮重值,4倍分度数
                gb_TareSigFlg = fb_SigFlg; //存储皮重值的正负号
                gb_TareGetedFlg = true;
            }
        }
        else
        {   //当前毛重为正数，存储的皮重也为正数，并且当前毛重小于保存的皮重，重新去皮
            if (gu32_TareNquad > fu32_Tare)
            {
                gu32_TareNquad = fu32_Tare;    //存储皮重值,4倍分度数
                gb_TareSigFlg = fb_SigFlg; //存储皮重值的正负号
                gb_TareGetedFlg = true;
            }
        }
    }
}

/*********************************************************************
函数名称: void FlapBag(void)
功    能: 拍袋处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void FlapBag(void)
{
    if (gb_RunFlg)
    {
        if (gb_AddFlapBagEndFlg == false)     //A加料中拍袋没有启动
        {
            //加料中拍袋启动处理 拍袋开关打开并且参数设置合理有效
            if ((gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B) && (gs_Rec.FlapBagSwitch == PF_ || gs_Rec.FlapBagSwitch == PFD)
                && (gs_Rec.FlapBagAddNum!= 0) && (gs_Rec.FlapBagTimeHi != 0))
            {
                //大投或中投时，并且当前重量大于拍袋起始重量
                if ((gb_Sp1Flg || gb_Sp2Flg) && (gu32_Wt >= gs_Rec.FlapBagWt) && gb_SigWtFlg == false)
                {
                    gb_AddFlapBagFlg = true;
                    gb_FlapWaitFlg = true;         //拍袋延时计时器启动
                    gu32_TimeFlapBuf = gs_Rec.FlapBagDelay * 10; 
                    gu32_Time8_9 = gu32_Sys10ms;
                    gb_AddFlapBagEndFlg = true;
                }
            }
        }
        
        //拍袋处理
        if (gb_FlapWaitFlg)         //拍袋延时已启动
        {
            gb_FlapBagOutFlg = false;           //延时期间拍袋无效
            if (gu32_Sys10ms - gu32_Time8_9 >= gu32_TimeFlapBuf)   //拍袋启动延时到,启动拍袋过程高电平持续时间
            {
                gb_FlapWaitFlg = false;
                gb_FlapStartFlg = true;        //启动拍袋
                gb_FlapBagOutFlg = true;       //拍袋输出有效
                gb_FlapAllHighFlg = false;
                if (gs_Rec.FlapBagTimeLow == 0)
                    gb_FlapAllHighFlg = true;
                //赋值加料中拍袋或加料后拍袋次数
                gu32_FlapBagNumCnt = (gb_AddFlapBagFlg) ? gs_Rec.FlapBagAddNum : gs_Rec.FlapBagNum;
                gu32_Time8_9 = gu32_Sys10ms;
            }
        }
        else if (gb_FlapStartFlg == true)      //拍袋已经启动
        {
            if (gb_FlapBagOutFlg)           //拍袋输出有效
            {
                if (gb_ExtFlapBagFlg == false)
                {
                    if (gu32_Sys10ms - gu32_Time8_9 >= gs_Rec.FlapBagTimeHi * 10)        //高电平时间到
                    {
                        gb_FlapBagOutFlg = false;
                        gu32_Time8_9 = gu32_Sys10ms;
                    }
                }
                else    //额外拍袋输出有效时间
                {
                    if (gu32_Sys10ms - gu32_Time8_9 >= gs_Rec.FlapBagExtTime * 10)        //高电平时间到
                    {
                        gb_FlapBagOutFlg = false;
                        gu32_Time8_9 = gu32_Sys10ms;
                    }
                }
            }
            else                            //拍袋输出无效时
            {
                if (gu32_Sys10ms - gu32_Time8_9 >= gs_Rec.FlapBagTimeLow * 10)       //低电平时间到
                {
                    if (--gu32_FlapBagNumCnt == 0)
                        gu32_FlapBagNumCnt = 0;

                    //拍袋次数已经到
                    if (gu32_FlapBagNumCnt == 0 || gu32_FlapBagNumCnt & 0x80000000)
                    {
                        //如果额外拍袋输出时间不为0并且不是加料中拍袋则启动额外拍袋输出
                        if (gs_Rec.FlapBagExtTime != 0 && gb_ExtFlapBagFlg == false && gb_AddFlapBagFlg == false)
                        {
                            if (gs_Setup.No2PackOffTwoSwitch != OFF)
                            {   //同时松袋的话此时就清除互锁输出
                                gb_AddingOutFlg = false;
                            }
                            if (gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
                            {   //无斗双秤互锁
                                if (NO2Wait(ON1))
                                    return;
                            }
                            gb_FlapBagOutFlg = true;
                            gu32_Time8_9 = gu32_Sys10ms;
                            gb_ExtFlapBagFlg = true;  //额外拍袋输出标志位
                            gu32_FlapBagNumCnt = 1;
                        }
                        else
                        {
                            gu32_FlapBagNumCnt = gs_Rec.FlapBagNum;
                            gb_FlapStartFlg = false;        //拍袋结束
                            gb_FlapBagOutFlg = false;
                            gb_FlapWaitFlg = false;
                            gb_FlapAllHighFlg = false;
                            gb_AddFlapBagFlg = false; //清除加料中拍袋标志位
                            gb_ExtFlapBagFlg = false; //清除额外拍袋输出标志位
                        }
                    }
                    else
                    {
                        gb_FlapBagOutFlg = true;
                        gu32_Time8_9 = gu32_Sys10ms;
                    }
                }
            }
        }
    }
    else
    {
        gb_FlapWaitFlg = false;
        gb_FlapStartFlg = false;
        gb_FlapBagOutFlg = false;
        gb_FlapAllHighFlg = false;
        gb_AddFlapBagEndFlg = false;
    }
}

/*********************************************************************
函数名称: void PackOff(void)
功    能: 松袋处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void PackOff(void)
{
    gb_JSDDelayFlg = true;
    gb_SDDelayFlg = false;
    gb_SDBDelayFlg = true;
    gu32_JSDTime = gu32_Sys10ms;
}

/*********************************************************************
函数名称: bool NO2Wait(u32 fu32_No2PackOffTwoSwitch)
功    能: 无斗双秤互锁等待的处理
说    明: 
入口参数: 
返 回 值: true等待 false往下执行
设    计: 丛海旭               时    间: 2015-02-05
修    改:                      时    间: 
*********************************************************************/
bool NO2Wait(u32 fu32_No2PackOffTwoSwitch)
{
    bool lb_Result = false;
    if ((gs_Setup.No2PackOffTwoSwitch > 0 && fu32_No2PackOffTwoSwitch == 0) ||
        (gs_Setup.No2PackOffTwoSwitch == 0 && fu32_No2PackOffTwoSwitch > 0))
    {   //设置的同时松袋和执行的不同时，返回不等待状态，即此时不用执行下面的判断
        //因为开关有OFF/ON1/ON2三种状态，但是ON1/ON2两种状态要等待处理的位置相同，入口参数只能是一种ON1，所以这样判断
        //即入口只有两种OFF/ON1,但是同时松袋这个参数有三种值OFF/ON1/ON2，入口ON1时，参数设置ON1/ON2时都要进行下面的判断
        return false;
    }
    
    switch (gs_Setup.No2PackOffTwoSwitch)
    {
        case OFF:   //同时松袋开关关闭，两秤全部松袋后再启动输送机
            if (gb_AddingInFlg)
            {   //此时另一秤还没有松袋则等待另一秤松袋后启动输送机
                lb_Result = true;
            }
            else
            {   //另外一秤已经松袋或者是没有夹袋，则判断是哪一种情况
                if (gb_No2PackOffAnother)
                {   //另外一秤已经松袋则启动输送机，否则等待
                    gb_ConveyorStartFlg = true; //启动输送机
                    gb_No2PackOffAnother = false;
                    lb_Result = false;
                }
                else
                {
                    lb_Result = true;
                }
            }
            break;
        case ON1:   //同时松袋普通模式
            if (gb_AddingInFlg)
            {   //此时另一秤在加料则等待另一秤一起松袋
                lb_Result = true;
            }
            else
            {   //另一秤没有加料，则松袋，启动输送机
                gb_ConveyorStartFlg = true; //启动输送机
                lb_Result = false;
            }
            break;
        case ON2:   //同时松袋快速模式
            if (gs_Setup.TareMode == NO2_A)
            {
                //松袋，启动输送机
                gb_ConveyorStartFlg = true; //启动输送机
                lb_Result = false;
            }
            else if (gs_Setup.TareMode == NO2_B)
            {
                if (gb_AddingInFlg)
                {   //此时另一秤在加料则等待另一秤一起松袋
                    lb_Result = true;
                }
                else
                {   //另一秤没有加料，则松袋，启动输送机
                    gb_ConveyorStartFlg = true; //启动输送机
                    lb_Result = false;
                }
            }
            break;
        default:
            break;
    }
    return lb_Result;
}

/*********************************************************************
函数名称: void WaitT5(void)
功    能: A卸料延时处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void WaitT5(void)
{
    if (gb_TimeRunFlg)            //卸料延时已经启动
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)       //卸料延时到
        {
            gb_DiscFlg = false;
            gb_TimeRunFlg = false;
            if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)    //有斗秤启动松袋处理
            {   //单秤组合最后一秤时松袋，组合次数为0不用管松袋，因为压根就不判断夹松袋
                if (gu32_WeighTimeCnt <= 1 && gs_Rec.CombinedNum != 0)
                {
                    PackOff();            //松袋处理
                    gb_LiftBagStartFlg = true;
                    gb_PushBagStartFlg = true;
                }
                else if (gs_Rec.CombinedNum == 0 && (gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B))
                {   //双秤互锁模式 如果不用夹袋 那么此时卸料完成应该允许另一秤卸料
                    gb_DiscEnableOutFlg = true;
                }
            }
        }
    }
    else
    {
        if (gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
        {   //无斗双秤互锁模式 同时松袋开关关闭时的等待
            if (NO2Wait(OFF))
                return;
        }
        OneTareOver();            //完成一秤后的处理
    }
}

/*********************************************************************
函数名称: void Disc(void)
功    能: 卸料处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void Disc(void)
{
    if (gb_Error9Flg)     //卸料未干净报警
        return;
    
    gb_CompFlg = false;
    
    if (gb_DiscFlg == false && (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B))
        gb_DiscFlg = true;
    //有斗模式，并且卸料震打功能打开
    if ((gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
         && gs_Rec.DiscShakeSwitch != 0 && gs_Rec.DiscShakeNum != 0)
    {
        if (gb_DiscShakeStartFlg)     //卸料震打已经开始
        {
            if (gb_DiscShakeFlg)      //卸料震打高电平
            {
                if (gu32_Sys10ms - gu32_DiscShakeTimeBuf >= gs_Rec.DiscShakeTimeHi * 10)
                {
                    gb_DiscShakeFlg = false;
                    gu32_DiscShakeTimeBuf = gu32_Sys10ms;
                }
            }
            else    //卸料震打低电平
            {
                if (gu32_Sys10ms - gu32_DiscShakeTimeBuf >= gs_Rec.DiscShakeTimeLow * 10)
                {
                    gb_DiscShakeFlg = true;
                    gu32_DiscShakeTimeBuf = gu32_Sys10ms;
                    gu32_DiscShakeCnt++;
                    if (gu32_DiscShakeCnt >= gs_Rec.DiscShakeNum)
                    {
                        gu32_DiscShakeCnt = 0;
                        gb_DiscShakeStartFlg = false;
                        gb_DiscShakeFlg = false;
                        ErrorDeal(9, 0);        //error9卸料震打时间到还未小于零区值
                        gb_AlarmFlg = true;
                        gb_Error9Flg = true;
                        return;
                    }
                } 
            }
        }
        //卸料有效时间到重量还没有低于零区值，启动卸料震打
        else if (gu32_Sys10ms - gu32_DiscShakeTimeBuf >= gs_Rec.DiscEffTime * 10)
        {
            gb_DiscShakeStartFlg = true;  //卸料震打开始
            gb_DiscShakeFlg = true;       //卸料震打输出有效
            gu32_DiscShakeTimeBuf = gu32_Sys10ms;     //保存启动卸料震打时刻
            gu32_DiscShakeCnt = 0;
        }
        if (Sp_Disc_State(DISC_STATE) == false)
        {   //由于夹袋到位无效导致的卸料信号输出无效，那么停止卸料震打
            gb_DiscShakeStartFlg = false;
            gb_DiscShakeFlg = false;       //卸料震打输出无效
            gu32_DiscShakeCnt = 0;
            gu32_DiscShakeTimeBuf = gu32_Sys10ms;
        }
    }
    
#ifdef DISC_TIME_CONTROL
    if (gb_TimeRunFlg)            //卸料时间已经启动
    {
        if (gu32_Sys1ms - gu32_Time1_5 >= gu32_TimeBuf)       //卸料时间到
        {
            gu32_DiscShakeCnt = 0;
            gb_DiscShakeStartFlg = false;
            gb_DiscShakeFlg = false;
            gb_DispNetFlg = false;
            gu32_Process = T5PROCESS;
            gu32_TimeBuf = gs_Rec.TimeT5 * 10;
            gb_TimeRunFlg = true;
            gu32_Time1_5 = gu32_Sys10ms;
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T5START;
        #endif
        }
    }
#else
    if (gb_NzFlg)         //卸料到零区后启动卸料延时t5
    {
        gu32_DiscShakeCnt = 0;
        gb_DiscShakeStartFlg = false;
        gb_DiscShakeFlg = false;
        gb_DispNetFlg = false;
        gu32_Process = T5PROCESS;
        gu32_TimeBuf = gs_Rec.TimeT5 * 10;
        gb_TimeRunFlg = true;
        gu32_Time1_5 = gu32_Sys10ms;
    #ifdef DEBUG_WT_SEND
        gu8_DebugState2 = T5START;
    #endif
    }
#endif
}

/*********************************************************************
函数名称: void WaitPack(void)
功    能: 定值保持结束后等待夹袋
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void WaitPack(void)
{
    if (gs_Rec.CombinedNum != 0)  //单秤组合次数等于0时不需判断夹袋
    {   //已经夹袋完成可以卸料
        if (PACKEND)
        {
            gb_TimeRunFlg = false;
        }
    }
    else
        gb_TimeRunFlg = false;
}

/*********************************************************************
函数名称: void WaitT4(void)
功    能: 定值保持时间t4处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void WaitT4(void)
{
    if (gb_TimeRunFlg)        //定值保持T4时间已经启动
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)       //定值保持时间到
        {
            if (gs_DebugRunTime.bGetRunCmpTimeFlg == false)
            {
                //启动到定值结束时间
                gs_DebugRunTime.u32RunCmpTime = gu32_Sys1ms - gs_DebugRunTime.u32RunStartTimeTemp;
                gs_DebugRunTime.bGetRunCmpTimeFlg = true;
            }
            
            if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)    
            {//有斗模式
                if (gb_GoFlg)
                {   //运行一次后的停止
                    gb_GoFlg = false;
                    RunStopInit(false);
                }
                if (gb_DiscWtGetedFlg == false)
                {
//                    gu32_HoldWt = gu32_Wt;  //A秤定值重量
                    gu32_HoldWt = DISP_WT_KALMAN;  //A秤定值重量
                    gb_DiscWtGetedFlg = true;
                }
                if (gs_Setup.TareMode == BINYES)
                    WaitPack();         //等待夹袋
                else    //双秤互锁模式
                {
                    if (gb_DiscEnableInFlg) 
                    {   //另一秤过来的允许卸料信号有效，可以进行下一步
                        gb_DiscEnableOutFlg = false;    //允许卸料输出无效，禁止另一秤卸料
                        WaitPack();         //等待夹袋
                    }
                    else
                    {/*另一秤过来的允许卸料信号无效，并且本秤的卸料允许输出也无效，那么只有
                       一种情况，就是小投结束时的互锁输入输出信号判断两秤是同时进行的，同时
                       判断到允许卸料输入有效然后同时将允许卸料输出变为无效。
                       那么此时如果是这种情况，将根据秤体模式设置的优先级来决定哪一秤放料，
                       默认是BIN2-A秤放料，B秤等待下一次夹袋。*/
                        if (gb_DiscEnableOutFlg == false)
                        {   
                            if (gs_Setup.TareMode == BIN2_B)
                            {//BIN2-B将允许卸料输出变为有效，那么BIN2-A秤就可以卸料了
                                gb_DiscEnableOutFlg = true;
                            }
                        }
                    }
                }
            }
            else if (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)    //无斗模式，直接进入定值后判断 
            {
                if (gb_GoFlg)
                {   //运行一次后的停止
                    gb_GoFlg = false;
                    RunStopInit(false);
                }
                gb_TimeRunFlg = false;
                Deal_WeighTime_Sum();
                //加料后拍袋启动处理 拍袋开关打开并且参数设置合理有效
                if ((gs_Rec.FlapBagSwitch == P_D || gs_Rec.FlapBagSwitch == PFD)
                    && (gs_Rec.FlapBagNum!= 0) && (gs_Rec.FlapBagTimeHi != 0))
                {
                    gb_AddFlapBagFlg = false;
                    gb_FlapWaitFlg = true;         //拍袋延时计时器启动
                    gu32_TimeFlapBuf = gs_Rec.FlapBagDelay * 10;
                    gu32_Time8_9 = gu32_Sys10ms;
                }
            }
        }
    }
    else        //定值保持时间t4已经结束
    {
        if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
        {
            //打码过程中不允许卸料的功能处理
            if (gb_PrintCodeIngFlg && gs_Rec.SheildDiscSwitch !=  0)
                return;
            gb_CompFlg = false;
            gb_TimeRunFlg = false;
            
            //拍袋处理
            if ((gs_Rec.FlapBagSwitch == P_D || gs_Rec.FlapBagSwitch == PFD)
                && (gs_Rec.FlapBagNum!= 0) && (gs_Rec.FlapBagTimeHi != 0))      //拍袋开关打开并且参数设置合理有效
            {
                if (gb_FlapWaitFlg == false && gb_FlapStartFlg == false)        //原来处于非拍袋以及非拍袋延时状态,启动拍袋延时
                {
                    //组合模式下最后一次放料才拍袋,组合次数为0不拍袋
                    if (gu32_WeighTimeCnt <= 1 && gs_Rec.CombinedNum != 0)
                    {
                        gb_FlapWaitFlg = true;         //拍袋延时计时器启动
                        gu32_TimeFlapBuf = gs_Rec.FlapBagDelay * 10;
                        gu32_Time8_9 = gu32_Sys10ms;
                    }
                }
            }
            else
            {
                gb_FlapWaitFlg = false;
                gb_FlapStartFlg = false;
                gb_FlapBagOutFlg = false;
            }
            
            Deal_WeighTime_Sum();
            Adapt_Process(&gs_Adapt);    //自适应参数处理
            gu32_Process = FALLPROCESS;
            gb_DiscFlg = true;           //启动卸料
            gu32_DiscShakeTimeBuf = gu32_Sys10ms;     //记录启动卸料的时刻
            gb_DiscWtGetedFlg = false;
        #ifdef DISC_TIME_CONTROL
            gu32_TimeBuf = gs_Rec.NearZero;
            gb_TimeRunFlg = true;
            gu32_Time1_5 = gu32_Sys1ms;
        #endif
        }
        else if (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
        {
            gb_CompFlg = false;
            gb_TimeRunFlg = false;
            
            //BINNO模式拍袋已经完成则启动松袋，或者启动额外拍袋时启动松袋
            //NO2_A/B模式需要满足互锁等条件才松袋
            if ((gb_FlapStartFlg == false && gb_FlapWaitFlg == false) || gb_ExtFlapBagFlg == true)
            {
                if (gs_Setup.No2PackOffTwoSwitch != OFF)
                {   //同时松袋的话此时就清除互锁输出
                    gb_AddingOutFlg = false;
                }
                if (gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
                {   //无斗双秤互锁
                    if (NO2Wait(ON1))
                        return;
                }
                //启动松袋延时
                PackOff();  //松袋
                if (gs_Setup.No2PackOffTwoSwitch == OFF)
                {   //清除无斗双秤互锁输出
                    gb_AddingOutFlg = false;
                    if (gs_Setup.No2AddAgainDelayTime != 0)
                        gb_No2AddAgainDelay = true;
                }
                gb_LiftBagStartFlg = true;
                gb_PushBagStartFlg = true;
                gu32_Process = FALLPROCESS;
                if (gu32_SumTemp != 0)
                {//累计结果
                    //Modbus通讯中使用，上秤结果
                    gu32_MbLastResult = gu32_SumTemp;
                    //保存累计数据
                    Save_SumData(gu32_SumTemp, 1);            
                #ifndef DEBUG_MONITORTIME_PRINT
                    //自动打印
                    PrintStart(PRINT_AUTO, gu32_SumTemp, 0);
                #endif
                    gu32_SumTemp = 0;
                }
                //无斗松袋直接进入T5PROCESS并且不走T5卸料延时，直接进行完成一秤后的处理
                gb_DispNetFlg = false;
                gu32_Process = T5PROCESS;
                gb_TimeRunFlg = false;
            }
        }
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = DISCSTART;
        #endif
    }
}

/*********************************************************************
函数名称: void WaitT3(void)
功    能: 超差检测时间
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void WaitT3(void)
{
    bool lb_Quit;
    
    if (gb_AlarmFlg)
        return;
    
    if (gb_TimeRunFlg)                //超差检测时间已经启动
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)           //超差检测时间到
        {
            lb_Quit = CorrectFall();          //落差修正
            if (lb_Quit == 0)           //落差修正完成或者落差修正开关关闭
            {
                if ((gs_Rec.OUSwitch != 0) && (gs_Rec.CorrectFallSwitch == 0) && (gb_StabFlg == false) && (!StabOver(false)))
                {
                    ;       //落差修正关闭（可能导致不稳定时也置位quit），且超欠差打开，并且不稳定则等待。
                }
                else
                {
                    StabOver(true); //复位判稳超时参数
                    if (gu32_WeighTimeCnt <= 1 || (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B))
                    {   //单秤组合到最后一秤才判断超欠差,无斗没有单秤组合次数
                        if (gs_Rec.CombinedNum <= 1 || (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B))    //单秤组合次数小于等于1，用目标值判断超欠差
                            Over_Under(gs_Rec.Target, gu32_Wt);         //超欠差处理
                        else    //大于1和目标值的倍数判断
                            Over_Under(gs_Rec.Target * gs_Rec.CombinedNum, gu32_SumTemp + gu32_Wt);         //超欠差处理
                        
                        if (gb_AlarmFlg == false)    //没有报警则继续下一过程
                        {
                            gu32_Process = T4PROCESS;
                            gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                            gb_TimeRunFlg = true;                //定值保持时间t4已经启动
                            gu32_Time1_5 = gu32_Sys10ms;        //取当前时间
                        }
                    }
                    else
                    {
                        gu32_Process = T4PROCESS;
                        gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                        gb_TimeRunFlg = true;                //定值保持时间t4已经启动
                        gu32_Time1_5 = gu32_Sys10ms;        //取当前时间
                    #ifdef DEBUG_WT_SEND
                        gu8_DebugState2 = T4START;
                    #endif
                    }
                }
            }
        }
    }
}

/*********************************************************************
函数名称: u8 PredictSp3_Sample(u32 fu32_Sp3StopWt)
功    能: 小投预测截止点 重量采集及线拟合相关数据计算
说    明: 该函数会内部调用线性拟合函数LeastLinear 和整型数除法四舍五入RoundOff
入口参数: fu32_Wt=当前重量 fu32_Sp3StopWt=小投截止点重量
出口参数: 
返 回 值: //0x00=小投截止点预测功能没有打开
          //0x10=前期线性拟合K值采集阶段 还没有越过不拟合采样次数u3InvalidTimes
          //0x11=前期线性拟合K值采集阶段 已经越过不拟合采样次数 还没有达到最少有效采样次数u32ValidTimes
          //0x12=前期线性拟合K值采集阶段 已经达到最少有效采样次数u32ValidTimes
          //0x20=线性拟合K值已经采集完毕 还没有越过不拟合采样次数u3InvalidTimes
          //0x21=线性拟合K值已经采集完毕 已经越过不拟合采样次数 还没有达到最少有效采样次数u32ValidTimes
          //0x22=线性拟合K值已经采集完毕 已经达到最少有效采样次数u32ValidTimes 开始接管小投截止点控制
          //0x30=计算出现不合理的值 线性拟合得到Y值为负数
设    计: 丛海旭               时    间: 2015-08-10
修    改:                      时    间: 
*********************************************************************/
u8 PredictSp3_Sample(u32 fu32_Sp3StopWt)
{
    s32 ls32_Temp;
    
//    if (gs_PredictSp3.u8Switch != 0)
//    {   //小投结束点预测开关打开
        if (++gs_PredictSp3.u32CntInvalid > gs_PredictSp3.u32InvalidTimes)
        {   //谷值后越过不拟合的采样次数
            gs_PredictSp3.u32CntInvalid = gs_PredictSp3.u32InvalidTimes;    //防止变量溢出处理
            
            if (++gs_PredictSp3.u32CntValid >= gs_PredictSp3.u32ValidTimes)
            {   //有效计数次数已经到达设置的计算拟合的数量 开始计算本次小投加料的拟合K值
                //达到计算拟合数组的最大长度进行处理
                if (gs_PredictSp3.u32CntValid >= LINEARARRAY_LENGTH - 1)
                    gs_PredictSp3.u32CntValid = LINEARARRAY_LENGTH - 1;
                //本次拟合K值
                gs_PredictSp3.s32KThisTime = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_PredictSp3.u32CntValid, 2, 0);
                
                if (gs_PredictSp3.bSampleDoneFlg)
                {
                    if (gs_PredictSp3.u8State != 0x22)
                    {   //线性拟合K值已经采集完毕 进行接管小投截止点控制相关计算
                        //根据当前线性拟合求得Y值 即当前拟合后的重量值 计算拟合当前重量是为减少误差
                        ls32_Temp = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_PredictSp3.u32CntValid, 1, LINEARARRAY_LENGTH);
                        if (ls32_Temp < 0)
                        {   //线性拟合Y值不合理
                            gs_PredictSp3.u8State = 0x30;
                            return gs_PredictSp3.u8State;
                        }
                        ls32_Temp = ((fu32_Sp3StopWt >= ls32_Temp) ? (fu32_Sp3StopWt - ls32_Temp) : (ls32_Temp - fu32_Sp3StopWt)); //计算到达提前量的差值Y
                        //已知Y 以当前状态为0点 根据前几次K平均值求X的值 即还需要多少次AD转换到达提前量 接管小投截止点控制
                        gs_PredictSp3.u32PredictTarget = RoundOff(ls32_Temp * 1000, gs_PredictSp3.s32KAverage);
                    }
                    gs_PredictSp3.u8State = 0x22;   //线性拟合K值已经采集完毕 已经达到最少有效采样次数u32ValidTimes 开始接管小投截止点控制
                }
                else
                    gs_PredictSp3.u8State = 0x12;   //前期线性拟合K值采集阶段 已经达到最少有效采样次数u32ValidTimes
            }
            else
            {
                if (gs_PredictSp3.bSampleDoneFlg)
                    gs_PredictSp3.u8State = 0x21;  //线性拟合K值已经采集完毕 已经越过不拟合采样次数 还没有达到最少有效采样次数u32ValidTimes
                else
                    gs_PredictSp3.u8State = 0x11; //前期线性拟合K值采集阶段 已经越过不拟合采样次数 还没有达到最少有效采样次数u32ValidTimes
            }
        }
        else
        {
            if (gs_PredictSp3.bSampleDoneFlg)
                gs_PredictSp3.u8State = 0x20; //线性拟合K值已经采集完毕 还没有越过不拟合采样次数u3InvalidTimes
            else
                gs_PredictSp3.u8State = 0x10; //前期线性拟合K值采集阶段 还没有越过不拟合采样次数u3InvalidTimes
        }
//    }
//    else  //注释掉 为了自适应功能中使用gs_PredictSp3.s32KThisTime变量
    if (gs_PredictSp3.u8Switch == 0)
    {
        gs_PredictSp3.u8State = 0;
    }
    
    return gs_PredictSp3.u8State;
}

/*********************************************************************
函数名称: bool PredictSp3_Control(void)
功    能: 接管小投截止点后的控制处理
说    明: 
入口参数: 
出口参数: 
返 回 值: true=小投结束 false=小投未结束
设    计: 丛海旭               时    间: 2015-08-10
修    改:                      时    间: 
*********************************************************************/
bool PredictSp3_Control(void)
{
    bool lb_Result;
    
    if (gs_PredictSp3.u8Switch != 0)
    {   //小投截止点预测功能打开
        if (gs_PredictSp3.u8State == 0x22)
        {   //接管小投截止点控制
            if (gs_PredictSp3.u32CntTarget >= gs_PredictSp3.u32PredictTarget)
            {   //目标AD次数到小投结束
                gs_DebugRunTime.s32SP3RealStopWt = gs32_WtKalmanFed;    //实际停小投时的重量
                lb_Result = true;
            }
            else
            {
                gs_PredictSp3.u32CntTarget++;
                lb_Result = false;
            }
        }
        else
        {
            lb_Result = false;
        }
    }
    else
    {
        lb_Result = false;
    }
    return lb_Result;
}

/*********************************************************************
函数名称: void PredictSp3_Init(void)
功    能: 小投截止点预测功能初始化
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-10
修    改:                      时    间: 
*********************************************************************/
void PredictSp3_Init(void)
{
    gs_PredictSp3.u8Switch = gs_Setup.DiscSumSwitch;
    gs_PredictSp3.u32KSampleTimes = gs_Rec.TimeT12;
    gs_PredictSp3.u32KCntSample = 0;
    gs_PredictSp3.u8State = 0;
    gs_PredictSp3.bSampleDoneFlg = false;
    gs_PredictSp3.u32PredictTarget = 0;
    gs_PredictSp3.u32CntTarget = 0;
    gs_PredictSp3.s32KThisTime = 0;
    gs_PredictSp3.s32KAverage = 0;
    gs_PredictSp3.u32InvalidTimes = gs_Rec.TimeT6;
    gs_PredictSp3.u32CntInvalid = 0;
    gs_PredictSp3.u32ValidTimes = gs_Rec.TimeT7;
    gs_PredictSp3.u32CntValid = 0;
}


/*********************************************************************
函数名称: void PredictSp3_Restart(void)
功    能: 小投截止点预测功能再次启动处理
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-10
修    改:                      时    间: 
*********************************************************************/
void PredictSp3_Restart(void)
{
    gs_PredictSp3.u32PredictTarget = 0;
    gs_PredictSp3.u32CntTarget = 0;
    gs_PredictSp3.s32KThisTime = 0;
    gs_PredictSp3.u32CntInvalid = 0;
    gs_PredictSp3.u32CntValid = 0;
}

/*********************************************************************
函数名称: s32 PredictSp3_KAverage(void)
功    能: 计算保存的平均K值
说    明: 去掉一个最大值 去掉一个最小值后计算平均值
入口参数: 
出口参数: 
返 回 值: 平均K值
设    计: 丛海旭               时    间: 2015-08-10
修    改:                      时    间: 
*********************************************************************/
s32 PredictSp3_KAverage(void)
{
    s32 ls32_KMax;
    s32 ls32_KMin;
    s32 ls32_Sum;
    u8 i;
    
    ls32_KMax = gs_PredictSp3.s32KSampleArray[0];
    ls32_KMin = gs_PredictSp3.s32KSampleArray[0];
    ls32_Sum = 0;
    for (i = 0; i < gs_PredictSp3.u32KSampleTimes; i++)
    {   //获取最大值最小值和总值
        ls32_Sum += gs_PredictSp3.s32KSampleArray[i];
        if (ls32_KMax < gs_PredictSp3.s32KSampleArray[i])
            ls32_KMax = gs_PredictSp3.s32KSampleArray[i];
        if (ls32_KMin > gs_PredictSp3.s32KSampleArray[i])
            ls32_KMin = gs_PredictSp3.s32KSampleArray[i];
    }
    ls32_Sum -= (ls32_KMax + ls32_KMin);
    ls32_Sum /= (gs_PredictSp3.u32KSampleTimes - 2);    //计算平均K值
    return ls32_Sum;
}

/*********************************************************************
函数名称: void PredictSp3_Calculate(void)
功    能: 小投截止点预测功能相关数据计算
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-10
修    改:                      时    间: 
*********************************************************************/
void PredictSp3_Calculate(void)
{
    u8 i;
    
    if (gs_PredictSp3.u8Switch != 0)
    {   //小投截止点预测功能打开
        if ((gs_PredictSp3.u8State & 0x02) == 0x02)
        {   //已经达到最少有效采样次数u32ValidTimes
            for (i = gs_PredictSp3.u32KSampleTimes - 1; i > 0; i--)
            {   //存入最新的K值到数组[0]
                gs_PredictSp3.s32KSampleArray[i] = gs_PredictSp3.s32KSampleArray[i-1];
            }
            gs_PredictSp3.s32KSampleArray[0] = gs_PredictSp3.s32KThisTime;
            if (++gs_PredictSp3.u32KCntSample >= gs_PredictSp3.u32KSampleTimes)
            {   //已经到达设置的K值保存个数
                gs_PredictSp3.u32KCntSample = gs_PredictSp3.u32KSampleTimes;    //防溢出
                //计算平均K值
                gs_PredictSp3.s32KAverage = PredictSp3_KAverage();
                gs_PredictSp3.bSampleDoneFlg = true;
            }
        }
    }
}


/*********************************************************************
函数名称: void SmallFeed(void)
功    能: 小投过程
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void SmallFeed(void)
{
    u8 lu8_State;
    
    gs_DebugRunTime.u32Sp3Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp3TimeTemp;  //小投时间
    gb_Sp1Flg = false;
    gb_Sp2Flg = false;
    //小投时强制停止加料中拍袋
    if (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
    {
        gb_AddFlapBagFlg = false;
        gb_FlapWaitFlg = false;
        gb_FlapStartFlg = false;
        gb_FlapBagOutFlg = false;
    }
    
#ifndef DEBUG_SMARTT2B
    if (gb_TimeRunFlg)            //小投禁止比较时间已经启动
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)       //禁止比较时间结束之前小投一直有效
        {
            gb_TimeRunFlg = false;
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T2BEND;
        #endif
        }
    }
#else
    if (gb_T2bRunFlg)
    {   //智能禁止比较时间判断已经启动
        ;
    }
#endif
    else
    {   //禁止比较时间已经结束
        SampleRushWt(&gs_Adapt);    //自适应功能 采集中投过冲量
            
            
        lu8_State = PredictSp3_Sample(gu32_TargetTemp - gs_Rec.Small);
        
        if (lu8_State != 0x22)
        {   //小投控制还没有被预测功能接管
            //目标值<=落差 或者 当前重量>=目标值-落差 进入T3
            if (((gf_WtKalmanFed >= gu32_TargetTemp - gs_Rec.Small) && (gb_SigWtFlg == false)) || (gs_Rec.Small >= gu32_TargetTemp))
            {
                PredictSp3_Calculate();
                gs_DebugRunTime.s32SP3RealStopWt = gs32_WtKalmanFed;    //实际停小投时的重量
                //慢加时间
                gs_DebugRunTime.u32Sp3Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp3TimeTemp;
                gb_Sp3Flg = false;
                gb_CompFlg = true;           //定值
                gb_TimeRunFlg = true;                    //超差检测时间或定值保持时间已经启动
                if (gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
                {   /*双秤互锁模式，小投结束后先判断一次允许卸料输入的状态是否有效，然后改变允许卸料输出
                      定值保持结束后还会判断一次输入状态。
                      因为如果只在定值保持结束后判断，那么两秤同时启动的话，有可能导致仪表同时判断到允许
                      卸料输入有效，然后同时将允许卸料输出变为无效，导致两秤都输出卸料信号，但是都没有夹
                      袋的情况*/
                    if (gb_DiscEnableInFlg)
                        gb_DiscEnableOutFlg = false;
                }
                if (gs_Rec.OUSwitch != 0 || gs_Rec.CorrectFallSwitch != 0)
                {   //超欠差检测开关打开或者落差修正开关打开则进入T3时间
                    gu32_TimeBuf = gs_Rec.TimeT3 * 10;        //读超差检测时间
                    gu32_Process = T3PROCESS;               //进入T3
                    gu32_Time1_5 = gu32_Sys10ms;                //记录当前时间作为T3启动时间
                #ifdef DEBUG_WT_SEND
                    gu8_DebugState2 = T3START;
                #endif
                }
                else
                {   //超欠差和落差修正都关闭则跳过T3，直接走T4
                    gu32_Process = T4PROCESS;
                    gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                    gu32_Time1_5 = gu32_Sys10ms;        //取当前时间
                #ifdef DEBUG_WT_SEND
                    gu8_DebugState2 = T4START;
                #endif
                }
            }
        }
        else
        {
            if (PredictSp3_Control() == true)
            {   //小投预测控制结束
                PredictSp3_Calculate();
                //慢加时间
                gs_DebugRunTime.u32Sp3Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp3TimeTemp;
                gb_Sp3Flg = false;
                gb_CompFlg = true;           //定值
                gb_TimeRunFlg = true;                    //超差检测时间或定值保持时间已经启动
                if (gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
                {   /*双秤互锁模式，小投结束后先判断一次允许卸料输入的状态是否有效，然后改变允许卸料输出
                      定值保持结束后还会判断一次输入状态。
                      因为如果只在定值保持结束后判断，那么两秤同时启动的话，有可能导致仪表同时判断到允许
                      卸料输入有效，然后同时将允许卸料输出变为无效，导致两秤都输出卸料信号，但是都没有夹
                      袋的情况*/
                    if (gb_DiscEnableInFlg)
                        gb_DiscEnableOutFlg = false;
                }
                if (gs_Rec.OUSwitch != 0 || gs_Rec.CorrectFallSwitch != 0)
                {   //超欠差检测开关打开或者落差修正开关打开则进入T3时间
                    gu32_TimeBuf = gs_Rec.TimeT3 * 10;        //读超差检测时间
                    gu32_Process = T3PROCESS;               //进入T3
                    gu32_Time1_5 = gu32_Sys10ms;                //记录当前时间作为T3启动时间
                #ifdef DEBUG_WT_SEND
                    gu8_DebugState2 = T3START;
                #endif
                }
                else
                {   //超欠差和落差修正都关闭则跳过T3，直接走T4
                    gu32_Process = T4PROCESS;
                    gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                    gu32_Time1_5 = gu32_Sys10ms;        //取当前时间
                #ifdef DEBUG_WT_SEND
                    gu8_DebugState2 = T4START;
                #endif
                }
            }
        }
    }
}

/*********************************************************************
函数名称: EnterSmallFeed
功    能: 进入小投过程
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-26
修    改:                      时    间: 
*********************************************************************/
void EnterSmallFeed(void)
{
    gs_DebugRunTime.s32SP2RealStopWt = gs32_WtKalmanFed;    //记录实际中投截止重量
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
    
}

/*********************************************************************
函数名称: void MiddleFeed(void)
功    能: 中投处理过程
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void MiddleFeed(void)
{
    gb_Sp1Flg = false;
    if (gb_TimeRunFlg)            //禁止比较时间已经启动
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)       //禁止比较时间到
        {
            gb_TimeRunFlg = false;
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T2AEND;
        #endif
        }
    }
    else
    {
        if (gs_AutoStd.u8Process == AUTOSTD_SECEND)
        {   //如果是第二次自学习过程则中投由时间控制
            if (gu32_Sys1ms - gs_DebugRunTime.u32Sp2TimeTemp >= gs_AutoStd.u32SP2Time)
            {
                EnterSmallFeed();
                gs_AutoStd.u8Process = AUTOSTD_NONE;
                gs_Rec.Middle = gu32_TargetTemp - gs32_WtKalmanFed;       //当前重量截止点计算中投提前量
                FRAM_WriteDW(FMAddr_Middle(gu32_RecNum), gs_Rec.Middle);
                RefreshDebugRec();  //更新打印的中投提前量
                gs_Adapt.s32SP2Bkp = (*gs_Adapt.ps32SP2);   //更新自适应的中投提前量
            }
        }
        if ((gf_WtKalmanFed >= gu32_TargetTemp - gs_Rec.Middle) && (gb_SigWtFlg == false))
        {
            EnterSmallFeed();
        }
    }
}

/*********************************************************************
函数名称: void LargeFeed(void)
功    能: 大投处理过程
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void LargeFeed(void)
{
    if ((gf_WtKalmanFed >= gu32_TargetTemp - gs_Rec.Large) && (gb_SigWtFlg == false))
    {
        //快加时间
        gs_DebugRunTime.u32Sp1Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp1TimeTemp;
        //记录中加加启动时刻
        gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;
        gb_Sp1Flg = false;
        gb_Sp2Flg = true;
        gu32_Process = MIDDLEPROCESS;
        if (gb_Sp2OverFlg == true)
        {
            gb_Sp2Flg = false;
            gu32_Process = SMALLPROCESS;
            gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
        }
        gb_Sp3Flg = true;
        gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
        gb_TimeRunFlg = true;        //启动禁止比较时间
        gu32_TimeBuf = gs_Rec.TimeT2a * 10;
        gu32_Time1_5 = gu32_Sys10ms;            //记录当前系统时间
    #ifdef DEBUG_WT_SEND
        gu8_DebugState2 = SP2START;
    #endif
    }
}

/*********************************************************************
函数名称: void WaitT1(void)
功    能: 启动延时过程处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void WaitT1(void)
{
    bool lb_Run = false;            //开始执行加料标志，为0时等待清零结束后加料
    
    if (gb_BatchOverFlg)        //如果批次完成则停止加料
    {
        gu32_Process = 0;
        gb_AlarmFlg = true;
        return;
    }
#ifdef DEBUG_MONITORTIME_PRINT    //刷新配方
    RefreshDebugRec();
#endif

    if (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)    //无斗模式
    {
        if (gb_TimeRunFlg)        //清零后延时已经启动
        {
            if (gs_Setup.GorNMode == GROSS) //无斗毛重 不去皮直接加料
            {
                gb_TimeRunFlg = false;
                lb_Run = true;
            }
            else    //无斗净重，去皮后延时一点时间再开始加料
            {
                if (--gu32_TimeBuf == 0 || gu32_TimeBuf & 0x80000000)
                {
                    gb_TimeRunFlg = false;
                    lb_Run = true;
                }
            }
        }
        else
        {
            //夹袋后启动延时T1
            //打码输出时不启动加料
            if (PACKEND && (gb_PrintCodeIngFlg == false || gs_Rec.SheildDiscSwitch == false))
            {
                gb_AddingOutFlg = true;
                if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)    //T1启动延时时间到
                {
                    if (gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
                    {   //无斗双秤互锁加料等待
                        if (gb_No2AddAgainDelay)
                            return;
                    }
                    
                    if (gs_Setup.GorNMode == GROSS)  //无斗毛重 不去皮
                    {
                        gu32_TareNquad = 0;    //存储皮重值
                        gb_TareSigFlg = false; //存储皮重值的正负号
                        gb_TimeRunFlg = true;
                    }
                    else    //无斗净重模式 需要判稳去皮
                    {
                        if (gb_StabFlg || StabOver(false))  //判稳 稳定或者超过判稳超时时间
                        {
                            StabOver(true); //复位判稳超时参数
                            GetTare(gu32_Nquad, gb_SigWtFlg, true);   //去皮
                            BuzzerStart(30);
                            gb_DispNetFlg = true;        //显示净重
                            gu32_TimeBuf = 1;    //去皮后延时启动
                            gb_TimeRunFlg = true;
                        }
                    }
                }
            }
            else
                gu32_Time1_5 = gu32_Sys10ms;
        }
    }
    else    //有斗模式
    {
        if (gb_TimeRunFlg)        //清零后延时已经启动
        {
            if (gb_AlarmFlg)
            {
                gu32_TimeBuf = 2;
            }
            else if (--gu32_TimeBuf == 0)
            {
                gb_TimeRunFlg = false;
                lb_Run = true;
                gu32_TareNquad = 0;
                gb_TareSigFlg = false;
                gb_DispNetFlg = true;        //显示净重
            }
        }
        else if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)          //T1启动延时时间到
        {
            //自动清零间隔的处理
            if (gs_Setup.AutoClrZeroInterval == 0)       //清零间隔为0，启动
            {
                lb_Run = true;
                gu32_TareNquad = 0;
                gb_TareSigFlg = false;
                gb_DispNetFlg = true;        //显示净重
            }
            else if (gb_FirstStartFlg)            //启动后A秤第一秤标志,该位有效,启动前不清零
            {
                gb_FirstStartFlg = false;     //清除运行后第一秤标志
                lb_Run = true;                 //本次不清零
                gu32_TareNquad = 0;
                gb_TareSigFlg = false;
                gb_DispNetFlg = true;        //显示净重
                gu32_AutoClrZeroCnt--;
                if (gu32_AutoClrZeroCnt <= 1)      //处理清零间隔
                    gu32_AutoClrZeroCnt = 0;
            }
            else if (gu32_AutoClrZeroCnt <= 1)  //清零间隔到，稳定后清零
            {
                if (gb_StabFlg || StabOver(false))         //稳定，清零计数器
                {
                    StabOver(true); //复位判稳超时参数
                    gb_TimeRunFlg = true;
                    if (gb_StabFlg)
                        gb_ZeroKeyFlg = true;
                    else
                        gb_ForceClrZeroFlg = true;
                    gu32_TimeBuf = 1;
                    gu32_AutoClrZeroCnt = gs_Setup.AutoClrZeroInterval;//复位清零间隔计数器
                }
            }
            else    //清零间隔未到
            {
                gu32_AutoClrZeroCnt--;         //清零间隔未到,清零间隔计数器减一
                lb_Run = true;
                gu32_TareNquad = 0;
                gb_TareSigFlg = false;
                gb_DispNetFlg = true;        //显示净重
            }
        }
        
        if (gb_OflFlg || gb_AdOKFlg == false)//溢出处理 //加料前溢出处理 一直处于加料前等待状态
        {
            gb_TimeRunFlg = true;
            gb_OflDelayFlg = true;
            return;
        }
        else if (gb_OflDelayFlg)
        {
            gb_OflDelayFlg = false;
            gb_TimeRunFlg = true;
            gu32_TimeBuf = 120;        //由溢出转到非溢出状态要再进行120次AD转换再加料
            return;
        }
    }
    gb_Sp1OverFlg = false;
    gb_Sp2OverFlg = false;
    if ((gu32_TargetTemp <= gs_Rec.Large) || (gs_Rec.Large == 0))          //大投提前量比目标值大或者为0认为大投无效
        gb_Sp1OverFlg = true;
    if ((gu32_TargetTemp <= gs_Rec.Middle) || (gs_Rec.Middle == 0))        //中投提前量比目标值大或者为0认为中投无效
        gb_Sp2OverFlg = true;
    
    if (lb_Run)
    {
        gb_FirstStartFlg = false;
        if (gb_LowDefineFlg)       //下料位有定义,且下料位输入无效
        {
            if (gb_LowFlg == false)
            {
                gb_TimeRunFlg = true;
                gu32_TimeBuf = 1;
                return;
            }
        }
        
//        if (gs_Setup.TareMode == BINYES)    //有斗模式才判断加料组合等问题
//        {
            //组合加料完成后重新赋值加料次数，为防止modbus运行时修改配方后总目标值和单秤目标值不匹配需重新判断，不匹配则停止
//            if (gu32_WeighTimeCnt == 0 && (gu32_WorkMode == A_ADD || gu32_WorkMode == A_ADD_B))
//            {
//                lu32_WeighTimeTemp = gs_Rec.FlapBagWt / gu32_SingleTarget;
//                            
//                if (lu32_WeighTimeTemp < 1)          //总目标值小于单称目标值报错
//                {
//                    RunStopInit(false);
//                    ErrorDeal(MDP_A, 6, 100);     //提示错误6
//                    return;
//                }
//                else if (gs_Rec.FlapBagWt % gu32_SingleTarget != 0)       //总目标值不是单称目标值的整数倍报错
//                {
//                    RunStopInit(false);
//                    ErrorDeal(MDP_A, 6, 100);     //提示错误6
//                    return;
//                }
//                else if (lu32_WeighTimeTemp == 1)
//                {
//                    if (gu32_WorkMode == A_ADD_B)    //A+B模式下总目标值不能等于单称目标值
//                    {
//                        RunStopInit(false);
//                        ErrorDeal(MDP_A, 6, 100);     //提示错误6
//                        return;
//                    }
//                }
//            }
//        }
        Adapt_InitData(&gs_Adapt);   //自适应每次运行处理
        
        if (JudgeAutoStudy())
        {   //重新进行自学习
            AutoStudy_SP2Start();
            return;
        }
        if (gs_AutoStd.u8Process == AUTOSTD_SECEND)
        {   //自学习第二次运行 直接进入中投
            
            gu32_Process = MIDDLEPROCESS;
            gb_TimeRunFlg = false;
            gb_Sp1Flg = false;
            gb_Sp2Flg = true;
            gb_Sp3Flg = true;
            gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
            gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;  //记录中加启动时刻
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = SP2START;
        #endif
        #ifdef DEBUG_T2A
            //中投禁止比较时间不为0则进入中投走禁止比较时间
            if (gs_Rec.TimeT2a != 0)
            {
                //记录中加加启动时刻
                gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;
                gb_Sp1Flg = false;
                gb_Sp2Flg = true;
                gb_Sp3Flg = true;
                gu32_Process = MIDDLEPROCESS;
                gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
                gb_TimeRunFlg = true;        //启动禁止比较时间
                gu32_TimeBuf = gs_Rec.TimeT2a * 10;
                gu32_Time1_5 = gu32_Sys10ms;            //记录当前系统时间
                
                gs_DebugRunTime.u32Sp3TimeTemp = gu32_Sys1ms;  //记录慢加启动时刻
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = SP2START;
            #endif
            }
        #endif
            return;
        }

        //参数设置有效性的判断
        if (gu32_TargetTemp <= gs_Rec.Small)
        {   //落差值大于等于目标值直接进入定值状态
            gu32_Process = T3PROCESS;
            gb_TimeRunFlg = true;
            gb_Sp1Flg = false;
            gb_Sp2Flg = false;
            gb_Sp3Flg = false;
            gb_CompFlg = true;           //定值
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T4START;
        #endif
            return;
        }
        else if ((gb_Sp1OverFlg == true) && (gb_Sp2OverFlg == true))
        {   //大投中投均无效，进入小投
            EnterSmallFeed();
        }
        else if (gb_Sp1OverFlg == true)          //只有大投无效，进入中投
        {
            gu32_Process = MIDDLEPROCESS;
            gb_TimeRunFlg = false;
            gb_Sp1Flg = false;
            gb_Sp2Flg = true;
            gb_Sp3Flg = true;
            gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
            gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;  //记录中加启动时刻
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = SP2START;
        #endif
        #ifdef DEBUG_T2A
            //中投禁止比较时间不为0则进入中投走禁止比较时间
            if (gs_Rec.TimeT2a != 0)
            {
                //记录中加加启动时刻
                gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;
                gb_Sp1Flg = false;
                gb_Sp2Flg = true;
                gb_Sp3Flg = true;
                gu32_Process = MIDDLEPROCESS;
                gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
                gb_TimeRunFlg = true;        //启动禁止比较时间
                gu32_TimeBuf = gs_Rec.TimeT2a * 10;
                gu32_Time1_5 = gu32_Sys10ms;            //记录当前系统时间
                
                gs_DebugRunTime.u32Sp3TimeTemp = gu32_Sys1ms;  //记录慢加启动时刻
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = SP2START;
            #endif
            }
        #endif
        }
        else        //大投有效，进入大投，中投可能无效，进入中投时再判断
        {
            gu32_Process = LARGEPROCESS;
            gb_TimeRunFlg = false;
            gb_Sp1Flg = true;
            gb_Sp2Flg = true;
            gb_Sp3Flg = true;
            gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
            if (gb_Sp2OverFlg == true)       //中投无效关闭中投
                gb_Sp2Flg = false;
            gs_DebugRunTime.u32Sp1TimeTemp = gu32_Sys1ms;  //记录快加启动时刻
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = SP1START;
        #endif
        }
        
        //当前重量和配方参数设置关系的判断，决定进入哪个控制过程
        if ((gu32_Wt >= gu32_TargetTemp - gs_Rec.Small) && (gb_SigWtFlg == false))
        {   //当前重量已经超过目标值-落差，直接进入定值
            gu32_Process = T3PROCESS;
            gb_TimeRunFlg = true;
            gb_Sp1Flg = false;
            gb_Sp2Flg = false;
            gb_Sp3Flg = false;
            gb_CompFlg = true;           //定值
            if (gs_Rec.OUSwitch != 0 || gs_Rec.CorrectFallSwitch != 0)
            {   //超欠差检测开关打开或者落差修正开关打开则进入T3时间
                gu32_TimeBuf = gs_Rec.TimeT3 * 10;        //读超差检测时间
                gu32_Process = T3PROCESS;               //进入T3
                gu32_Time1_5 = gu32_Sys10ms;                //记录当前时间作为T3启动时间
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = T3START;
            #endif
            }
            else
            {   //超欠差和落差修正都关闭则跳过T3，直接走T4
                gu32_Process = T4PROCESS;
                gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                gu32_Time1_5 = gu32_Sys10ms;        //取当前时间
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = T4START;
            #endif
            }
            if (gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
            {   /*双秤互锁模式，小投结束后先判断一次允许卸料输入的状态是否有效，然后改变允许卸料输出
                  定值保持结束后还会判断一次输入状态。
                  因为如果只在定值保持结束后判断，那么两秤同时启动的话，有可能导致仪表同时判断到允许
                  卸料输入有效，然后同时将允许卸料输出变为无效，导致两秤都输出卸料信号，但是都没有夹
                  袋的情况*/
                if (gb_DiscEnableInFlg)
                    gb_DiscEnableOutFlg = false;
            }
            
            //小投时强制停止加料中拍袋
            gb_AddFlapBagFlg = false;
            gb_FlapWaitFlg = false;
            gb_FlapStartFlg = false;
            gb_FlapBagOutFlg = false;
        }
        else if ((gb_Sp2OverFlg == false) && (gu32_Wt >= gu32_TargetTemp - gs_Rec.Middle) && (gb_SigWtFlg == false))
        {   //中投设置有效并且重量大于目标-中投提前量则直接进入小投
        #ifdef DEBUG_T2A
            //中投禁止比较时间不为0则进入中投走禁止比较时间
            if (gs_Rec.TimeT2a != 0)
            {
                //记录中加加启动时刻
                gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;
                gb_Sp1Flg = false;
                gb_Sp2Flg = true;
                gb_Sp3Flg = true;
                gu32_Process = MIDDLEPROCESS;
                gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
                gb_TimeRunFlg = true;        //启动禁止比较时间
                gu32_TimeBuf = gs_Rec.TimeT2a * 10;
                gu32_Time1_5 = gu32_Sys10ms;            //记录当前系统时间
                
                gs_DebugRunTime.u32Sp3TimeTemp = gu32_Sys1ms;  //记录慢加启动时刻
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = SP2START;
            #endif
            }
            else if (gu32_Process < SMALLPROCESS)
            {
                EnterSmallFeed();
            }
        #else
            if (gu32_Process < SMALLPROCESS)
            {
                EnterSmallFeed();
            }
        #endif
        }
        else if ((gb_Sp1OverFlg == false) && (gu32_Wt >= gu32_TargetTemp - gs_Rec.Large) && (gb_SigWtFlg == false))
        {   //大投设置有效并且重量大于目标-大投提前量，则进入中投处理
            if (gu32_Process < MIDDLEPROCESS)
            {
                gu32_Process = MIDDLEPROCESS;
                gb_TimeRunFlg = false;
                gb_Sp1Flg = false;
                gb_Sp2Flg = true;
                gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;  //记录中加启动时刻
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = SP2START;
            #endif
                //中投设置无效则直接进入小投
                if (gb_Sp2OverFlg == true)
                {
                    EnterSmallFeed();
                }
                gb_Sp3Flg = true;
                gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //小投点动时间处理
            }
        }
    }
}

/*********************************************************************
函数名称: AutoStudy_SP2Start
功    能: 自学习功能开始中投投料
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-31
修    改:                      时    间: 
*********************************************************************/
void AutoStudy_SP2Start(void)
{
    gb_Sp1Flg = false;
    gb_Sp2Flg = true;
    gb_Sp3Flg = true;
    gs_AutoStd.u32StartMoment = gu32_Sys1ms;  //记录中投启动时刻
    gs_AutoStd.u8Process = AUTOSTD_SP2;
    gs_AutoStd.u32SP2Time = gs_Setup.Adapt7_9;      //B7.9设置自学习中投加料时间
    gu32_Process = AUTOSTUDYPROCESS;
    Adapt_InitSP3KTemp(&gs_Adapt);  //清除小投线性拟合K
}

/*********************************************************************
函数名称: AutoStudy_SP2ing
功    能: 自学习中投过程处理
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-31
修    改:                      时    间: 
*********************************************************************/
void AutoStudy_SP2ing(void)
{
    if (gu32_Sys1ms - gs_AutoStd.u32StartMoment >= gs_AutoStd.u32SP2Time)
    {
        gb_Sp1Flg = false;
        gb_Sp2Flg = false;
        gb_Sp3Flg = false;
        gs_AutoStd.u8Process = AUTOSTD_SP2STOP;
        gs_AutoStd.u32StartMoment = gu32_Sys1ms;    //记录停止加料时刻
    }
}

/*********************************************************************
函数名称: AutoStudy_SP2Stop
功    能: 自学习中投停止后处理
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-31
修    改:                      时    间: 
*********************************************************************/
void AutoStudy_SP2Stop(void)
{
    #define STADY_DELAY     2000    //自学习中投后间隔等待时间
    #define STADY_SP3TIME   2000    //自学习时小投持续时间
    
    if (gu32_Sys1ms - gs_AutoStd.u32StartMoment >= STADY_DELAY)
    {
        if (gb_StabFlg)
        {   //判稳后计算流量等数据
            gs_AutoStd.s32SP2StopResult = gs32_WtKalmanFed;     //记录中投投料结果
            gs_AutoStd.fSP2Flux = (float)gs32_WtKalmanFed / gs_AutoStd.u32SP2Time;   //计算中投加料流量
            gb_Sp1Flg = false;
            gb_Sp2Flg = false;
            gb_Sp3Flg = true;
            gs_AutoStd.u8Process = AUTOSTD_SP3;
            gs_AutoStd.u32SP3Time = STADY_SP3TIME;
            gs_AutoStd.u32StartMoment = gu32_Sys1ms;  //记录小投启动时刻
        }
    }
    #undef STADY_DELAY
    #undef STADY_SP3TIME
}

/*********************************************************************
函数名称: AutoStudy_SP3ing
功    能: 自学习小投处理过程
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-09-01
修    改:                      时    间: 
*********************************************************************/
void AutoStudy_SP3ing(void)
{
    if (gu32_Sys1ms - gs_AutoStd.u32StartMoment >= gs_AutoStd.u32SP3Time)
    {
        gb_Sp1Flg = false;
        gb_Sp2Flg = false;
        gb_Sp3Flg = false;
        gs_AutoStd.u8Process = AUTOSTD_SP3STOP;
        gs_AutoStd.s32SP3StopMomentWt = gs32_WtKalmanFed;     //记录小投停止时刻重量
        //取240次(0.5s)线性拟合小投斜率K
        gs_AutoStd.s32SP3K = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - 240, 2, 0);
        gs_AutoStd.u32StartMoment = gu32_Sys1ms;    //记录停止加料时刻
        gs_Adapt.s32SP3KProb = SP3LinearKProbabilityStatistics(&gs_Adapt, gs_AutoStd.s32SP3K);  //小投K统计
    }
}

/*********************************************************************
函数名称: AutoStudy_SP3Stop
功    能: 自学习小投停止后处理
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-09-01
修    改:                      时    间: 
*********************************************************************/
void AutoStudy_SP3Stop(void)
{
    #define STADY_DELAY     2000    //自学习小投后间隔等待时间
    
    if (gu32_Sys1ms - gs_AutoStd.u32StartMoment >= STADY_DELAY)
    {
        if (gb_StabFlg)
        {   //判稳后计算流量等数据
            gs_AutoStd.s32SP3StopResult = gs32_WtKalmanFed;     //记录中投投料结果
            gs_AutoStd.u32SP3Val = gs_AutoStd.s32SP3StopResult - gs_AutoStd.s32SP3StopMomentWt; //计算自学习的小投落差值
            gs_Rec.Small = gs_AutoStd.u32SP3Val;
            FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);  //保存小投落差值
            //再次启动小投继续完成加料
            gb_Sp1Flg = false;
            gb_Sp2Flg = false;
            gb_Sp3Flg = true;
            gs_AutoStd.u8Process = AUTOSTD_SP3FILL;
        }
    }
    #undef STADY_DELAY
}

/*********************************************************************
函数名称: AutoStudy_SP3Fill
功    能: 自学习小投加料完成处理
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-09-01
修    改:                      时    间: 
*********************************************************************/
void AutoStudy_SP3Fill(void)
{
    #define FORBIDTIME_25 700   //默认禁止比较时间
    #define FORBIDTIME_15 750
    #define FORBIDTIME_05 800
    #define STUDY_CONSERVATIVE 0.9  //自学习中投保守百分比
    
    u32 lu32_Temp;
    
    if ((gf_WtKalmanFed >= gu32_TargetTemp - gs_Rec.Small) && (gb_SigWtFlg == false))
    {   //加料完成
        gb_Sp3Flg = false;
        //计算自学习的中投时间 以小投时间为禁止比较时间+自适应判断小投时间依据的最长时间为基准
        //小投加的重量
        lu32_Temp = gs_AutoStd.s32SP3K * 480 * (FORBIDTIME_25 + gs_Setup.AdaptSP2Level + gs_Setup.AdaptSP2TimeThreshold) / 1000000;
        //中投加的重量
        lu32_Temp = (float)(gu32_TargetTemp - lu32_Temp) * STUDY_CONSERVATIVE;
        //中投加的时间
        gs_AutoStd.u32SP2Time = (float)lu32_Temp / gs_AutoStd.fSP2Flux;
        gs_DebugRunTime.u32Sp2Time = gs_AutoStd.u32SP2Time; //打印用
        
        gs_AutoStd.u8Process = AUTOSTD_SECEND;
        
        gb_CompFlg = true;           //定值
        gb_TimeRunFlg = true;                    //超差检测时间或定值保持时间已经启动
        if (gs_Rec.OUSwitch != 0 || gs_Rec.CorrectFallSwitch != 0)
        {   //超欠差检测开关打开或者落差修正开关打开则进入T3时间
            gu32_TimeBuf = gs_Rec.TimeT3 * 10;        //读超差检测时间
            gu32_Process = T3PROCESS;               //进入T3
            gu32_Time1_5 = gu32_Sys10ms;                //记录当前时间作为T3启动时间
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T3START;
        #endif
        }
        else
        {   //超欠差和落差修正都关闭则跳过T3，直接走T4
            gu32_Process = T4PROCESS;
            gu32_TimeBuf = gs_Rec.TimeT4 * 10;
            gu32_Time1_5 = gu32_Sys10ms;        //取当前时间
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T4START;
        #endif
        }
    }
}

/*********************************************************************
函数名称: JudgeAutoStudy
功    能: 判断是否执行自学习过程
说    明: 
入口参数: 
出口参数: 
返 回 值: true=执行自学习 false=不执行
设    计: 丛海旭               时    间: 2015-09-01
修    改:                      时    间: 
*********************************************************************/
bool JudgeAutoStudy(void)
{
    if (gs_Rec.Middle == 0 && gs_Rec.Small == 0)
    {   //中投和小投提前量都为0 并且没有运行过自学习 则重新进行自学习
        return true;
    }
    else
        return false;
}

/*********************************************************************
函数名称: AutoStudy
功    能: 自学习过程控制
说    明: 
入口参数: 
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-31
修    改:                      时    间: 
*********************************************************************/
void AutoStudy(void)
{
    switch (gs_AutoStd.u8Process)
    {
        case AUTOSTD_SP2:
            AutoStudy_SP2ing();
            break;
        case AUTOSTD_SP2STOP:
            AutoStudy_SP2Stop();
            break;
        case AUTOSTD_SP3:
            AutoStudy_SP3ing();
            break;
        case AUTOSTD_SP3STOP:
            AutoStudy_SP3Stop();
            break;
        case AUTOSTD_SP3FILL:
            AutoStudy_SP3Fill();
            break;
        default:
            break;
    }
}

/*********************************************************************
函数名称: void Cmpul(void)
功    能: 加料过程
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void Cmpul(void)
{
    switch(gu32_Process)
    {
        case T1PROCESS:             //加料延时
            WaitT1();
            break;
        case AUTOSTUDYPROCESS:      //自学习过程
            AutoStudy();
            break;
        case LARGEPROCESS:          //大投
            LargeFeed();
            break;
        case MIDDLEPROCESS:         //中投
            MiddleFeed();
            break;
        case SMALLPROCESS:          //小投
            SmallFeed();
            break;
        case T3PROCESS:             //超欠差检测延时
            WaitT3();
            break;
        case T4PROCESS:             //定值结束输出
            WaitT4();
            break;
        case FALLPROCESS:           //卸料
            Disc();
            break;
        case T5PROCESS:             //卸料延时
            WaitT5();
            break;
        default:
            gu32_Process = 0;
            break;
    }

    if (gb_AlarmFlg || gb_AdOKFlg == false)
    {
        gb_Sp1Flg = false;
        gb_Sp2Flg = false;
        gb_Sp3Flg = false;
        gb_DiscFlg = false;
    }
}

/*********************************************************************
函数名称: void Key_RunState(void)
功    能: 仪表运行时的处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-8
修    改:                      时    间: 
*********************************************************************/
void Key_RunState(void)
{
    u8 i;
    
    if (gb_SumFlg)
    {
        Key_SumState();
    }
    else if (gb_FastSetFlg)              //快速设置状态下的按键处理
    {
        Key_FastSetState();
    }
    else
    {
        if (gb_AlarmFlg)     //报警状态下只能按ESC ENTER ZERO
        {
            if ((gu32_KeyCode != K_ZERO) && (gu32_KeyCode != K_ESC) && (gu32_KeyCode != K_ENTER) && (gu32_KeyCode != K_NONE))
                return;
        }
        switch (gu32_KeyCode)
        {
            case K_6:   //运行按SUM键查看累计次数和累计重量
                 ulParameter.ul = 0xFFFFFFFF;
                gu8_Item = 0;
                gu8_DataInLength = 0;
                gb_SetFlg = false;
                gb_FirstNumKeyFlg = true;
                gb_FlashFlg = false;
                gu32_KeyCode = 0;
                if(gs_Setup.SumCheckCodeSwitch != 0)   //自定义密码开关打开
                {
                    gu8_Item = 0;
                    gb_SumFlg = true;
                }
                else
                {
                    ulParameter.ul = gu32_SumWt;
                    gu8_Item = 1;
                    gb_SumFlg = true;
                }
                Key_SumState();
                break;
            case K_ENTER:
                ulParameter.ul = 0xFFFFFFFF;
                gu8_Item = 0;
                gu8_DataInLength = 0;
                gb_SetFlg = false;
                gb_FirstNumKeyFlg = true;
                gb_FlashFlg = false;
                
                gb_FastSetFlg = true;
                gu32_KeyCode = 0;
                gu8_Item = REC_F1_1;
                gu8_FastSetSigTemp = 0;
                for (i = 0; i <= 5; i++)        //将F1.x读入到快速设置暂存
                    gu32_FastSetTemp[i] = GetRecipeItemValue(REC_F1_1 + i);
                ulParameter.ul = gu32_FastSetTemp[gu8_Item-3];
                break;
            case K_ESC:
                Clr_Alarm();                //清报警
                if (gb_GoFlg)   //手动运行一次时按ESC键停止
                    RunStopInit(false);
                break;
            default:
                break;
        #ifdef DEBUG_RUN_STOP_BY_KEY
            case K_RIGHT:     //→键急停
                Clr_Alarm();
                RunStopInit(false);
                break;
        #endif
        }
    }
    
    if (gu32_SystemState == RUNSTATE)
    {
        Cmpul();            //加料过程处理
    }
    
    if(gb_OUOverFlg)
    {
        if(gu32_Sys10ms - gu32_OUDelayTimeBuf >= 200)
            gb_OUOverFlg = false;
    }
    if(gb_OUUnderFlg)
    {
        if(gu32_Sys10ms - gu32_OUDelayTimeBuf >= 200)
            gb_OUUnderFlg = false;
    }
}


/*********************************************************************
函数名称: bool StabOver(bool lb_Reset)
功    能: 仪表是否判稳超时
说    明: 
入口参数: lb_Reset 重新获取判断时间
返 回 值: 判稳超时时间到
设    计: 丛海旭               时    间: 2014-2-14
修    改:                      时    间: 
*********************************************************************/
bool StabOver(bool lb_Reset)
{
#ifdef DEBUG_JUDESTABLE_TIME
    return false;
#else
    static bool sb_TimeGetedFlg = false;
    static u32 su32_TimeTemp;
    
    if (lb_Reset)
    {
        sb_TimeGetedFlg = false;
        return false;
    }
    if (gs_Setup.JudgeStabOverTime == 0) //判稳超时时间设置为0该功能不起作用
        return false;
    
    if (sb_TimeGetedFlg == false)
    {
        sb_TimeGetedFlg = true;
        su32_TimeTemp = gu32_Sys10ms;
    }
    if (gu32_Sys10ms - su32_TimeTemp >= gs_Setup.JudgeStabOverTime * 10)
    {   //判稳超时时间到
        ErrorDeal(7, 70);        //error7 判稳超时时间到但是秤体还不稳
        return true;
    }
    else
        return false;
#endif
}
