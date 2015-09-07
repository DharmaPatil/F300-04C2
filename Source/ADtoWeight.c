/*
Description:            重量转换相关函数，AD码数据转换成重量数据，判稳、追零、清零等处理
Author:                 丛海旭
Date:                   2013-12-9
*/

#include "include.h"
#include "MathLogic.h"
#include "Kalman.h"

u32  gu32_AdFedValue;       //AD数据进行滤波后得到的数据
u32  gu32_NetAdFedValue;    //减去标定零点的AD码
bool gb_NetAdFlg;           //减去标定零点的AD码符号位
u32  gu32_Nquad;            //毛重4倍分度数
bool gb_SigNquadFlg;        //毛重4倍分度数的符号位

u32  gu32_FloatZero;        //浮动零点
bool gb_SigFZeroFlg;        //浮动零点符号
u32  gu32_N;                //毛重分度数
u32  gu32_Wt;               //毛重重量

bool gb_DispNetFlg;         //显示净重标志位
bool gb_NetSigFlg;          //净重符号
u32 gu32_TareNquad;        //皮重值的4倍分度数
bool gb_TareSigFlg;         //皮重符号


bool gb_StabFlg;        //稳定
bool gb_MvStabFlg;      //毫伏数稳定
bool gb_PowerOnZeroFlg;     //上电自动清零
bool gb_ZeroKeyFlg;         //按清零键
bool gb_ForceClrZeroFlg;    //强制清零标志位 如清零间隔到但是判稳超时
bool gb_AlarmFlg;       //报警标志
bool gb_ZeroPosition;   //零位标志
bool gb_NzFlg;          //零区标志

#ifdef DEBUG_WT_SEND
    u32 gu32_WtNotFed;          //没有滤波的AD码转换成的重量
    bool gb_SigWtNotFedFlg;     //没有滤波的AD码转换成的重量的符号
#endif

#ifdef REFRESH_AT_SP23_MAX
    bool gb_RefreshAdArrayFlg = false;
#endif

#ifdef DEBUG_SMARTT2B
    bool gb_T2bRunFlg = false;  //小投禁止比较时间已经开始
    u32 gu32_T2bTime;
#endif

bool gb_Sp2StopTimeRunFlg = false;  //预测中投停止时间已经开始
u32 gu32_Sp2StopTime01ms;       //预测中投停止的时间 单位0.1ms

s32 gs32_Wt;                //有符号数的重量值
float gf_WtKalmanFed;       //卡尔曼滤波处理后的重量值浮点数
s32 gs32_WtKalmanFed;       //卡尔曼滤波处理后的重量值有符号整形


/*********************************************************************
函数名称: void AD_SCal0(void)
功    能: 相对零点的AD码
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-4
修    改:                      时    间: 
*********************************************************************/
void AD_SCal0(void)
{
    if(gu32_AdFedValue >= gs_Cal.Zero)
    {
        gu32_NetAdFedValue = gu32_AdFedValue - gs_Cal.Zero;
        gb_NetAdFlg = false;
    }
    else
    {
        gu32_NetAdFedValue = gs_Cal.Zero - gu32_AdFedValue;
        gb_NetAdFlg = true;
    }
}

/*********************************************************************
函数名称: void TareAndWeight(void)
功    能: 净重处理，重量数据计算以及零区零位等标志处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-18
修    改:                      时    间: 
*********************************************************************/
void TareAndWeight(void)
{
    if (gb_DispNetFlg == true)    //显示净重的相关处理
    {
        if (gb_SigWtFlg != gb_TareSigFlg)
        {
            gu32_Nquad += gu32_TareNquad;
        }
        else
        {
            if (gu32_Nquad >= gu32_TareNquad)
                gu32_Nquad -= gu32_TareNquad;
            else
            {
                gu32_Nquad = gu32_TareNquad - gu32_Nquad;
                gb_SigWtFlg = !gb_SigWtFlg;
            }
        }
    }
    gb_ZeroPosition = false;
    if (gu32_Nquad < 2)        //零位处理
    {
        gb_SigWtFlg = false;
        gb_ZeroPosition = true;
    }
    
    gu32_N = (gu32_Nquad + 2)>> 2;             //除以4四舍五入得到一倍分度数
    gu32_Wt = gu32_N * gs_Cal.Div;        //乘以分度值得到当前重量
    if(gu32_Wt > gs_Cal.Cap + 9 * gs_Cal.Div)         //大于10个分度最大量程则溢出
        gb_OflFlg = true;
    
        gb_NzFlg = false;
        if (gu32_Wt <= gs_Rec.NearZero || gb_SigWtFlg == true)        //零区标志位
            gb_NzFlg = true;
}


/*********************************************************************
函数名称: void AD_ADK(void)
功    能: AD码除以ADK得到4倍分度数
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-4
修    改:                      时    间: 
*********************************************************************/
void AD_ADK(void)
{
    //扩大2的13次方，用来约掉gs_Cal.AD中的2的13次方，这样做可以提高计算精度
    //余数处理，四舍五入，得到4倍分度数
//    gu32_Nquad = RoundOff(gu32_NetAdFedValue << 13, gs_Cal.ADK);
    gu32_Nquad = ((float)gu32_NetAdFedValue)/gs_Cal.ADK;
    gb_SigNquadFlg = gb_NetAdFlg;
}


/*********************************************************************
函数名称: void Clr0(u32 fu32_ClrZeroRng)
功    能: 清零处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-17
修    改:                      时    间: 
*********************************************************************/
void Clr0(u32 fu32_ClrZeroRng)
{
    //上电自动清零或按了清零键或强制清零
    if (gb_PowerOnZeroFlg || gb_ZeroKeyFlg || gb_ForceClrZeroFlg)
    {
        if ((gb_OflFlg == false && gb_StabFlg == true) || gb_ForceClrZeroFlg)      //稳定并且不溢出才可以清零
        {   //四舍五入得到分度值进行清零范围的比较
            if ((((gu32_Nquad + 2) >> 2) * gs_Cal.Div * 100) <= (gs_Cal.Cap * fu32_ClrZeroRng))      //当前重量符合清零范围
            {
                gu32_FloatZero = gu32_Nquad;            //保存浮动零点为当前4倍分度值
                gb_SigFZeroFlg = gb_SigNquadFlg;         //保存浮动零点符号
                gb_PowerOnZeroFlg = false;
            }
            else                //不在清零范围内，报警error2
            {
                ErrorDeal(2, 100);        //error2
                gb_AlarmFlg = true;
                gb_PowerOnZeroFlg = false;
            }
            BuzzerStart(30);
        }
        else if (gb_OflFlg == 0 && gb_StabFlg == false && gb_PowerOnZeroFlg == false)       //按请零时不稳定
        {
            ErrorDeal(3, 100);        //error3
            gb_AlarmFlg = true;
        }
        gb_ZeroKeyFlg = false;
        gb_ForceClrZeroFlg = false;
    }
    
    gb_SigWtFlg = gb_SigNquadFlg;            //当前重量的符号
    if (gb_SigNquadFlg == gb_SigFZeroFlg)
    {
        if (gu32_Nquad >= gu32_FloatZero)
            gu32_Nquad -= gu32_FloatZero;
        else
        {
            gu32_Nquad = gu32_FloatZero - gu32_Nquad;
            gb_SigWtFlg = !gb_SigFZeroFlg;
        }
    }
    else
    {
        gu32_Nquad += gu32_FloatZero;
    }
}

/*********************************************************************
函数名称: void Trace0(u32 fu32_TraceZeroRng)
功    能: 追零处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-17
修    改:                      时    间: 
*********************************************************************/
void Trace0(u32 fu32_TraceZeroRng)
{
    #define TRACE0TIME (cu8_MultiAd[gs_Setup.AdSamplingRate] * 240);
    static u32 sul_TraceZeroTimeBuf;        //追零时间
    u32 u32temp;
    
    //追零，标定状态和运行时不追零
    if (fu32_TraceZeroRng != 0 && gu32_SystemState != CALIBSTATE && gu32_SystemState != RUNSTATE)
    {
        //计算当前重量与浮动零点之间的差值(4倍分度数)
        if (gb_SigNquadFlg == gb_SigFZeroFlg)
            u32temp = abs(gu32_Nquad - gu32_FloatZero);
        else
            u32temp = gu32_Nquad + gu32_FloatZero;
        
        if (u32temp <= 4 * fu32_TraceZeroRng && gb_StabFlg == true)       //小于追零范围且稳定，开始追零
        {
            sul_TraceZeroTimeBuf--;
            if (sul_TraceZeroTimeBuf == 0)
            {
                gu32_FloatZero = gu32_Nquad;
                gb_SigFZeroFlg = gb_SigNquadFlg;
                sul_TraceZeroTimeBuf = TRACE0TIME;
            }
        }
        else
            sul_TraceZeroTimeBuf = TRACE0TIME;
    }
}



/*********************************************************************
函数名称: void JudgeStable(void)
功    能: 重量和毫伏数判稳
说    明: 
入口参数: fu32_JudgeStabRng:判稳范围 为0时默认为一直稳定
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-17
修    改:                      时    间: 
*********************************************************************/
void JudgeStable(u32 fu32_JudgeStabRng)
{
    u32  MOTNUM = (cu8_MultiAd[gs_Setup.AdSamplingRate] * 60);      //用于判稳的序列长度
    
    static u32 WTTAB[480];              //判稳队列
    static u32 *lu32p_NewData = &WTTAB[0];
    u16 i;
    u32 lu32_Max, lu32_Min, lu32_TempWt, lu32_TempMv;
    
#ifdef DEBUG_JUDESTABLE_TIME
    //只能小于等于0.5s
    if (gs_Setup.JudgeStabOverTime <= 5)
    {
        MOTNUM = MOTNUM * gs_Setup.JudgeStabOverTime / 5;
    }
#endif
    
    //赋值新数据 用指针代替数组循环移动 提高程序效率
    *lu32p_NewData = gu32_AdFedValue;
    lu32p_NewData++;
    if (lu32p_NewData > &WTTAB[ MOTNUM - 1])
        lu32p_NewData = &WTTAB[0];
    
//    for (i = 0; i < MOTNUM - 1; i++)
//        WTTAB[i] = WTTAB[i+1];
//    WTTAB[MOTNUM - 1] = gu32_AdFedValue;
    lu32_Max = WTTAB[0];
    lu32_Min = WTTAB[0];
    
    for (i = 1; i < MOTNUM; i++)
    {
        if (WTTAB[i] > lu32_Max)
            lu32_Max = WTTAB[i];
        if (WTTAB[i] < lu32_Min)
            lu32_Min = WTTAB[i];
    }
    
    lu32_TempWt = lu32_Max - lu32_Min;
    lu32_TempWt = ((float)lu32_TempWt)/gs_Cal.ADK;    //转换成4倍分度数
    
    lu32_TempMv = lu32_Max - lu32_Min;
    lu32_TempMv = AdToMv(lu32_TempMv);        //转换成毫伏数
    
    //重量判稳处理 小于判稳范围稳定 判稳范围为0一直稳定
    if (lu32_TempWt <= fu32_JudgeStabRng * 4 || fu32_JudgeStabRng == 0)
        gb_StabFlg = true;
    else
        gb_StabFlg = false;

    //mv数判稳处理
    if (lu32_TempMv <= 1)   //毫伏数跳动在0.001内当做稳定
        gb_MvStabFlg = true;
    else
        gb_MvStabFlg = false;
}

/*********************************************************************
函数名称: void AdToWeight(u32 fu32_AdValue, u32 *fu32p_Wt, bool *fbp_SigFlg)
功    能: AD码转换成重量数据
说    明: 调试使用
入口参数: fu32_AdValue:需转换的AD码；*fu32p_Wt：转化后的重量地址；*fbp_SigFlg：转换后的重量符号；
返 回 值: 
设    计: 丛海旭               时    间: 2014-04-30
修    改:                      时    间: 
*********************************************************************/
void AdToWeight(u32 fu32_AdValue, u32 *fu32p_Wt, bool *fbp_SigFlg)
{
    
    if(fu32_AdValue >= gs_Cal.Zero)
    {
        fu32_AdValue = fu32_AdValue - gs_Cal.Zero;
        *fbp_SigFlg = false;
    }
    else
    {
        fu32_AdValue = gs_Cal.Zero - fu32_AdValue;
        *fbp_SigFlg = true;
    }
    //扩大2的13次方，用来约掉gs_Cal.AD中的2的13次方，这样做可以提高计算精度
    //余数处理，四舍五入，得到4倍分度数
    fu32_AdValue = ((float)fu32_AdValue)/gs_Cal.ADK;
    
    if (*fbp_SigFlg == gb_SigFZeroFlg)
    {
        if (fu32_AdValue >= gu32_FloatZero)
            fu32_AdValue -= gu32_FloatZero;
        else
        {
            fu32_AdValue = gu32_FloatZero - fu32_AdValue;
            *fbp_SigFlg = !gb_SigFZeroFlg;
        }
    }
    else
    {
        fu32_AdValue += gu32_FloatZero;
    }
    
    if (gb_DispNetFlg == true)    //显示净重的相关处理
    {
        if (*fbp_SigFlg != gb_TareSigFlg)
        {
            fu32_AdValue += gu32_TareNquad;
        }
        else
        {
            if (fu32_AdValue >= gu32_TareNquad)
                fu32_AdValue -= gu32_TareNquad;
            else
            {
                fu32_AdValue = gu32_TareNquad - fu32_AdValue;
                *fbp_SigFlg = !(*fbp_SigFlg);
            }
        }
    }
    if (fu32_AdValue < 2)        //零位处理
        *fbp_SigFlg = false;
    
    fu32_AdValue = (fu32_AdValue + 2)>> 2;             //除以4四舍五入得到一倍分度数
    fu32_AdValue = fu32_AdValue * gs_Cal.Div;        //乘以分度值得到当前重量
    *fu32p_Wt = fu32_AdValue;
}


/*********************************************************************
函数名称: void Refresh_SampleWt(u32 fu32_Wt, bool* fbp_RefreshFlg, bool fb_StartFlg)
功    能: 最高点刷新采样数组 采集重量判断最高点
说    明: 先入先出不断存重量3个重量值 一旦有连续两个重量低于第一个重量测刷新采样数组
入口参数: fu32_Wt=重量
          fbp_RefreshFlg=刷新数组标志
          fb_StartFlg=开始采集重量标志
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-08
修    改:                      时    间: 
*********************************************************************/
#ifdef REFRESH_AT_SP23_MAX
void Refresh_SampleWt(u32 fu32_Wt, bool* fbp_RefreshFlg, bool fb_StartFlg)
{
    static bool sb_DoneFlg = false;
    static u32 su32_SaveWt[3] = {0, 0, 0};
    u8 i;
    
    if (fb_StartFlg)
    {   //开始采样重量
        if (sb_DoneFlg == false)
        {
            su32_SaveWt[2] = su32_SaveWt[1];
            su32_SaveWt[1] = su32_SaveWt[0];
            su32_SaveWt[0] = fu32_Wt;
            if (su32_SaveWt[1] < su32_SaveWt[2] && su32_SaveWt[0] < su32_SaveWt[1])
            {   //连续有两个重量低于前一个重量 置位刷新采样数组标志位
                *fbp_RefreshFlg = true;
                sb_DoneFlg = true;
            }
        }
    }
    else
    {
        for (i = 0; i < sizeof(su32_SaveWt)/sizeof(su32_SaveWt[0]); i++)
        {
            su32_SaveWt[i] = 0;
            sb_DoneFlg = false;
        }
    }
}
#endif

#ifdef DEBUG_SMARTT2B
/*********************************************************************
函数名称: void SmartT2b_SampleWt(u32 fu32_Wt, bool* fbp_T2bRunFlg, bool fb_StartFlg, u32 fu32_MinT2b)
功    能: 智能判断T2B禁止比较时间
说    明: 先入先出不断存重量2个重量值 一旦有低于第一个重量代表已经到达最高点
          然后再判断重量一旦有大于前一个重量则代表禁止比较时间结束
入口参数: fu32_Wt=重量
          fbp_T2bRunFlg=智能禁止比较时间开始标志
          fb_StartFlg=开始采集重量标志
          fu32_MinT2b=最小禁比时间限制1ms单位
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-20
修    改:                      时    间: 
*********************************************************************/
void SmartT2b_SampleWt(u32 fu32_Wt, bool* fbp_T2bRunFlg, bool fb_StartFlg, u32 fu32_MinT2b)
{
    static bool sb_DoneFlg = false;
    static bool sb_HighestFlg = false;
    static u32 su32_SaveWt[2] = {0, 0};
    static u32 su32_Time;
    const u8 cu8_PerAdTime[] = {8, 4, 2, 1};    //120~960次的每次AD转换时间 ms
    u8 i;
    
    if (fb_StartFlg)
    {   //开始采样重量
        if (sb_DoneFlg == false)
        {   //采集重量
            su32_SaveWt[1] = su32_SaveWt[0];
            su32_SaveWt[0] = fu32_Wt;
            *fbp_T2bRunFlg = true;
            if (sb_HighestFlg == false)
            {   //还未到最高点
                if (su32_SaveWt[0] < su32_SaveWt[1])
                {   //一旦有低于第一个重量代表已经到达最高点
                    sb_HighestFlg = true;
                    //保存峰值
                    gs_DebugRunTime.u32MaxWt = su32_SaveWt[1];
                    //中投截止点到峰值时间 因为上次AD转换才是峰值 所以要减去1次AD转的时间
                    gs_DebugRunTime.u32Sp2StopToMaxTime = gu32_Sys1ms - gs_DebugRunTime.u32Sp3TimeTemp - cu8_PerAdTime[gs_Setup.AdSamplingRate];
                    //保存峰值时间 供计算峰值到谷值时间
                    gs_DebugRunTime.u32MaxToMinTimeTemp = gu32_Sys1ms - cu8_PerAdTime[gs_Setup.AdSamplingRate];
                }
            }
            else
            {   //已经到达最高点往下降
                if (su32_SaveWt[0] > su32_SaveWt[1])
                {   //一旦有大于等于前一个重量则代表禁止比较时间结束
                    if (gu32_Sys1ms - su32_Time > fu32_MinT2b)
                    {   //智能检测禁比时间超过fu32_MinT2b认为正常
                        sb_DoneFlg = true;
                        *fbp_T2bRunFlg = false;
                        //保存谷值
                        gs_DebugRunTime.u32MinWt = su32_SaveWt[1];
                        //峰值到谷值时间 因为上次AD转换才是谷值 所以要减去1次AD转的时间
                        gs_DebugRunTime.u32MaxToMinTime = gu32_Sys1ms - gs_DebugRunTime.u32MaxToMinTimeTemp - cu8_PerAdTime[gs_Setup.AdSamplingRate];
    
                    #ifdef DEBUG_WT_SEND
                        gu8_DebugState2 = T2BEND;
                        gu32_T2bTime = gu32_Sys1ms - gu32_Time1_5;  //记录禁止比较时间T2b时间
                    #endif
                    }
                    else
                    {   //智能检测禁比时间太短，可能冲击力太大 重新计算时间
                        sb_HighestFlg = false;
                    }
                }
            }
        }
    }
    else
    {
        for (i = 0; i < sizeof(su32_SaveWt)/sizeof(su32_SaveWt[0]); i++)
        {
            su32_SaveWt[i] = 0;
        }
        sb_DoneFlg = false;
        sb_HighestFlg = false;
        *fbp_T2bRunFlg = false;
        su32_Time = gu32_Sys1ms;    //保存启动前一时刻的时间
    }
}
#endif

/*********************************************************************
函数名称: LinearFitting_SP2K
功    能: 采集中投截止点附近的线性拟合K值
说    明: 
入口参数: fb_StartFlg=开始采集标志 小投开始
          fu32_Num=采集点的个数
          pfs32_K=计算得到的K
返 回 值: 
设    计: 丛海旭               时    间: 2015-8-26
修    改:                      时    间: 
*********************************************************************/
void LinearFitting_SP2K(bool fb_StartFlg, u32 fu32_Num, s32 *pfs32_K)
{
    static bool sb_DoneFlg = false;
    static u32 lsu32_Cnt = 0;
    
    if (fu32_Num <= 2)
    {
        *pfs32_K = 0;
        return;
    }
    
    if (fb_StartFlg)
    {   //开始采样重量
        if (sb_DoneFlg == false)
        {   //开始采集 fu32_Num/2个点
            lsu32_Cnt++;
            if (lsu32_Cnt >= fu32_Num / 2)
            {   //达到设置的采样个数
                *pfs32_K = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT9, 2, 0);
                sb_DoneFlg = true;
            }
        }
    }
    else
    {
        sb_DoneFlg = false;
        lsu32_Cnt = 0;
    }
}

/*********************************************************************
函数名称: JudgeWtOK
功    能: 判断重量数组是否处于正常上升状态
说    明: 
入口参数: 
返 回 值: true=正常上升状态 false=不正常
设    计: 丛海旭               时    间: 2015-07-23
修    改:                      时    间: 
*********************************************************************/
bool JudgeWtOK(s32* pfs32_Array, u8 fu8_Num)
{
    u8 i;
    
    for(i = 0; i < fu8_Num; i++)
    {
        if (pfs32_Array[i] <= pfs32_Array[i+1] || pfs32_Array[i] <= 0)
            return false;
    }
    return true;
}

/*********************************************************************
函数名称: Sp2WillStop
功    能: 判断是否该启动定时器来停止SP2
说    明: 
入口参数: su32_SaveWt=保存重量数组
          fu32_Sp2StopWt=中投截止点
          lu8_Num=保存重量数组的长度
          fu32p_StopTime=停止时间0.1ms为单位
返 回 值: true=启动定时器 false=不启动定时器
设    计: 丛海旭               时    间: 2015-07-23
修    改:                      时    间: 
*********************************************************************/
bool Sp2WillStop(s32* pfs32_SaveWt, u32 fu32_Sp2StopWt, u8 lu8_Num, u32* fu32p_StopTime)
{
    const float cf_AdSpeed[] = {120, 240, 480, 960};
    
//    u8 i;
    s32 ls32_DataTemp = 0;
    float lf_Time = (float)10000 / cf_AdSpeed[gs_Setup.AdSamplingRate];//每次AD转换需多长时间 单位0.1ms
    
    if (lu8_Num < 1)
    {
        return false;
    }
    //线性拟合 已知x值预测y值是否超过大投截止点
    ls32_DataTemp = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT10, 1, LINEARARRAY_LENGTH + 1);
    if (ls32_DataTemp >= fu32_Sp2StopWt)
    {   //预测值大于大投截止点
        //启动定时器 计算定时时长
        lf_Time = lf_Time * (float)(fu32_Sp2StopWt - pfs32_SaveWt[0]) / (float)(ls32_DataTemp - pfs32_SaveWt[0]); //增加到大投截止点重量要多长时间 单位0.1ms
        //四舍五入计算
        *fu32p_StopTime = (u32)(lf_Time * 10);
        *fu32p_StopTime = RoundOff(*fu32p_StopTime , 10);
        return true;
    }
    else
    {
        return false;
    }
    
//    for (i = 0; i < lu8_Num - 1; i++)
//    {
//        ls32_DataTemp += (pfs32_SaveWt[i] - pfs32_SaveWt[i+1]);   //计算每个重量之间的差值和
//    }
//    ls32_DataTemp /= (lu8_Num - 1);   //得到平均差值
//    
//    if (pfs32_SaveWt[0] + ls32_DataTemp >= fu32_Sp2StopWt)
//    {   //启动定时器 计算定时时长
//        lf_Time = lf_Time * (float)(fu32_Sp2StopWt - pfs32_SaveWt[0]) / (float)ls32_DataTemp; //增加到大投截止点重量要多长时间 单位0.1ms
//        //四舍五入计算
//        *fu32p_StopTime = (u32)(lf_Time * 10);
//        *fu32p_StopTime = RoundOff(*fu32p_StopTime , 10);
//        return true;
//    }
//    else
//    {
//        return false;
//    }
}

/*********************************************************************
函数名称: PredictSp2Stop
功    能: 智能预测中投截止点
说    明: 先入先出不断存3个重量值计算差值，增加的幅度，预测下一个数值，是否超过中投截止点，如超过
          则计算出当前值到中投截止点所用的时间，开中断，时间到后结束中投
入口参数: fu32_Wt=重量
          fbp_TimeRunFlg=中断计时已经开始标志
          fb_StartFlg=开始采集重量标志
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-20
修    改:                      时    间: 
*********************************************************************/
void PredictSp2Stop(s32 fs32_Wt, u32 fu32_Sp2StopWt, bool* fbp_TimeRunFlg, u32* fu32p_StopTime, bool fb_StartFlg)
{
    #define WT_ARRAY_LENGTH 3
    
    static bool sb_DoneFlg = false;
    static s32 lss32_SaveWt[WT_ARRAY_LENGTH] = {0, 0, 0};
    u8 i;
    
    if (fb_StartFlg)
    {   //开始采样重量
        if (sb_DoneFlg == false)
        {   //采集重量
            for (i = WT_ARRAY_LENGTH-1; i > 0; i--)
            {
                lss32_SaveWt[i] = lss32_SaveWt[i-1];
            }
            //最新数据存入[0]中
            lss32_SaveWt[0] = fs32_Wt;
            if (JudgeWtOK(lss32_SaveWt, WT_ARRAY_LENGTH))
            {   //最新数据都比前一个数据大，并且都大于0，才认为是已经开始中投加料过程
                if (fs32_Wt >= fu32_Sp2StopWt)
                {   //重量已经超过中投值则不判断
                    sb_DoneFlg = true;
                    gs_DebugRunTime.u32PredictSp2StopTime = 0;
                }
                else if (Sp2WillStop(lss32_SaveWt, fu32_Sp2StopWt, WT_ARRAY_LENGTH, fu32p_StopTime))
                {
                    sb_DoneFlg = true;
                    Sp2StopTimeStart(fbp_TimeRunFlg);
                }
            }
        }
        else
        {   //已经完成中投截至点的判断
            
        }
    }
    else
    {
        memset(lss32_SaveWt, 0, WT_ARRAY_LENGTH);
        sb_DoneFlg = false;
        *fbp_TimeRunFlg = false;
    }
}


/*********************************************************************
函数名称: void GetWt(void)
功    能: 得到重量数据
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-17
修    改:                      时    间: 
*********************************************************************/
void GetWt(void)
{
    u32 lu32_AdFilterTemp;
    
    if (gu32_SystemState == CALIBSTATE) //标定状态固定最大滤波，便于稳定
        lu32_AdFilterTemp = 9;
#ifdef DEBUG_SMARTT2B_FILTER    //智能禁止比较时间过程中的滤波
    else if (gb_RunFlg && gb_T2bRunFlg)
        lu32_AdFilterTemp = gs_Setup.TraceZeroRng;
#endif
    else if (gb_RunFlg && (gu32_Process == FALLPROCESS || gu32_Process == T5PROCESS))   //卸料滤波
        lu32_AdFilterTemp = gs_Setup.AdFilterLevelDisc;
    else if (gb_RunFlg && (gu32_Process == T3PROCESS || gu32_Process == T4PROCESS))     //定值滤波
        lu32_AdFilterTemp = gs_Setup.AdFilterLevelComp;
    else    //一般情况下滤波
        lu32_AdFilterTemp = gs_Setup.AdFilterLevel;
    
    gu32_AdFedValue = AD_GetFedVal(lu32_AdFilterTemp);     //得到滤波后AD码
    JudgeStable(gs_Setup.JudgeStabRng);     //重量和毫伏数判稳
    AD_SCal0();     //减标定零点得到相对零点的AD码
    AD_ADK();       //除以ADK得到4倍分度数
    Trace0(gs_Setup.TraceZeroRng);      //追零处理
    Clr0(gs_Setup.ClrZeroRng);          //清零处理
    TareAndWeight();                //得到重量数据以及净重转换、零区、零位标志等处理
#ifdef DEBUG_WT_SEND
    AdToWeight(gu32_AdValue, &gu32_WtNotFed, &gb_SigWtNotFedFlg);
#endif

    //用上电自动清零间隔开关复用作为卡尔曼滤波处理功能
    gs32_Wt = (gb_SigWtFlg ? (-gu32_Wt) : gu32_Wt);     //有符号重量
    if (gs_Setup.PowerOnZeroSwitch != 0)
    {   //卡尔曼滤波处理
        gf_WtKalmanFed = kalman1_filter(&gs_KalmanState, gs32_Wt);
    }
    else
    {   
        gf_WtKalmanFed = gs32_Wt;
    }
    gs32_WtKalmanFed = (gf_WtKalmanFed > 0 ? (s32)(gf_WtKalmanFed + 0.5) : (s32)(gf_WtKalmanFed - 0.5));

//    //存入滤波前线性拟合计算数组
//    SaveLinearArray(gs32_WtNotFedArray, gu32_WtNotFed, gb_SigWtNotFedFlg);
//    //计算滤波前重量k值 取最新的几个数据
//    gs32_LinearKNotFed = LeastLinear(gs32_WtNotFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT9, 2, 0);

    //存入滤波后线性拟合计算数组
    SaveLinearArray(gs32_WtFedArray, gs32_WtKalmanFed);
    //计算滤波后重量k值 取最新的几个数据
//    gs32_LinearKFed = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT10, 2, 0);
    //获取加料过程中的最大k
//    GetMaxLinearK(((gu32_Process == MIDDLEPROCESS) || (gu32_Process == SMALLPROCESS)) && gb_RunFlg);

#ifdef REFRESH_AT_SP23_MAX
    Refresh_SampleWt(gu32_Wt, &gb_RefreshAdArrayFlg, (gu32_Process == SMALLPROCESS ? true : false) && gb_RunFlg);
#endif

#ifdef DEBUG_SMARTT2B
    SmartT2b_SampleWt(gs32_WtKalmanFed, &gb_T2bRunFlg, (gu32_Process == SMALLPROCESS ? true : false) && gb_RunFlg, gs_Rec.TimeT2b * 100);
#endif
    //计算中投截止点附近的K值
    LinearFitting_SP2K((gu32_Process == SMALLPROCESS ? true : false) && gb_RunFlg, gs_Rec.TimeT9, &gs32_SP2KFed);

    if (gs_Setup.ResultHoldSwitch != 0)
    {
        if (gs_AutoStd.u8Process != AUTOSTD_SECEND)
        {   //第二次自学习过程不参与预测
            //运行状态 中投过程 并且中投禁止比较时间结束后才判断
            PredictSp2Stop(gs32_WtKalmanFed, gs_Rec.Target - gs_Rec.Middle, &gb_Sp2StopTimeRunFlg, &gu32_Sp2StopTime01ms, (gu32_Process == MIDDLEPROCESS ? true : false) && gb_RunFlg && (gb_TimeRunFlg == false));
        }
    }
}

/*********************************************************************
函数名称: void GetMaxLinearK(bool fb_StartFlg)
功    能: 保存最大的线性拟合K值 只保存大于0的K值
说    明: 
入口参数: fb_StartFlg=开始记录最大值
出口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2015-08-06
修    改:                      时    间: 
*********************************************************************/
void GetMaxLinearK(bool fb_StartFlg)
{
    static bool sb_DoneFlg = false;
    static bool sb_InflexionFlg = false;    //已经到拐点标志
    static s32 ss32_KFed;
    static u32 su32_Cnt;
    
    if (fb_StartFlg)
    {
        if (gs32_LinearKFed > ss32_KFed)
        {   //取滤波后最大k值
            ss32_KFed = gs32_LinearKFed;
        }
        else
        {   
            if (sb_InflexionFlg == false)
            {   //还没有到达拐点
                //k值减小则认为是到达拐点
                //防错处理 大于1kg 防止料门关闭的震动导致误判
                if (gs32_WtKalmanFed > 1000)
                {   //新的拟合k值小则表示此次采样已经开始减小趋势，上次采样的数据是k值拐点
                    //在拐点附近取共F3.11设置的个数数据计算k值
                    if (gs_Rec.TimeT11 >= 2)
                    {   //取点个数要大于等于2
                        su32_Cnt = gs_Rec.TimeT11 / 2 - 1;  //获取需要再取的点个数 因为此时刻点点已经是拐点+1，所以要-1
                        sb_InflexionFlg = true;
                    }
                    #ifdef DEBUG_WT_SEND
                        gu8_DebugState2 = MAXK;
                    #endif
                }
            }
        }
        gs_DebugRunTime.s32Sp2MaxKFed = ss32_KFed;
        if (sb_InflexionFlg && sb_DoneFlg == false)
        {   //已经过拐点
            if ((su32_Cnt--) <= 0)
            {   //已经取够了所需要计算的数据 计算拐点附近gs_Rec.TimeT11个数的k
                gs_DebugRunTime.s32MaxKFedReLinear = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT11, 2, 0);
                //线性拟合b值 以开始拟合的点为x的0点 这样b值就是x=LINEARARRAY_LENGTH - gs_Rec.TimeT11时的Y值
                gs_DebugRunTime.s32MaxBFedReLinear = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT11, 1, LINEARARRAY_LENGTH - gs_Rec.TimeT11);
                sb_DoneFlg = true;
            }
        }
    }
    else
    {
        ss32_KFed = 0;
        sb_InflexionFlg = false;
        sb_DoneFlg = false;
        su32_Cnt = 0;
    }
}
