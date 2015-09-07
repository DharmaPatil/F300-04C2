#ifndef _RUNSTATE_H
#define _RUNSTATE_H

#define     T1PROCESS               1        /*过程控制的T1延时阶段*/
#define     SUPERLARGEPROCESS       2        /*过程控制的超大投阶段*/
#define     LARGEPROCESS            3        /*过程控制的大投阶段*/
#define     MIDDLEPROCESS           4        /*过程控制的中投阶段*/
#define     SMALLPROCESS            5        /*过程控制的小投阶段*/
#define     T3PROCESS               6        /*过程控制的T3延时阶段*/
#define     T4PROCESS               7        /*过程控制的T4延时阶段*/
#define     FALLPROCESS             8        /*过程控制的卸料阶段*/
#define     T5PROCESS               9        /*过程控制的T5延时阶段*/
#define     AUTOSTUDYPROCESS        10      //自学习过程

//夹袋到位标志
#define     PACKOK  ((!gb_EnPackOKFlg) || gb_PackOKFlg)
//夹袋已经完成标志
#define     PACKEND  (gb_SDDelayFlg && gb_PackFlg && (!gb_JSDDelayFlg) && PACKOK)



extern u32 gu32_Process;                 //运行时的加料状态
extern bool gb_TimeRunFlg;              //延时已经启动标志位
extern u32 gu32_TimeBuf;                //延时计时器缓存，保存设置的时间值
extern u32 gu32_Time1_5;                //T1~T5的计时器缓存，保存启动时刻的时间
extern bool  gb_FlapBagOutFlg;             //拍袋输出有效标志
extern bool  gb_FlapWaitFlg;               //拍袋之前的等待标志
extern bool  gb_FlapStartFlg;              //拍袋已经开始标志
extern bool  gb_FlapAllHighFlg;            //低电平时间为0,拍袋恒定有效标志位
extern u32 gu32_TimeFlapBuf;               //拍袋用时间寄存器
extern u32 gu32_Time8_9;                   //t8和t10用延时寄存器
extern u32 gu32_FlapBagNumCnt;             //拍袋计数器
extern bool gb_TareGetedFlg;            //皮重已经获取标志位
extern u32 gu32_OUDelayTimeBuf;         //超欠差暂停关闭时的超欠差报警计时寄存器
extern bool gb_AddFlapBagFlg;                //加料中拍袋标志位
extern bool gb_ExtFlapBagFlg;                //额外拍袋输出标志位
extern u32 gu32_DiscShakeCnt;           //卸料震打计数
extern bool gb_Error9Flg;                    //卸料未干净报警
extern bool gb_SumFlg;                 //累计
extern bool gb_DiscWtGetedFlg;       //定值重量已经获取
extern u32 gu32_DiscValue;     //本次卸料值
extern u32 gu32_HoldWt;        //定值保持重量
extern u32 gu32_WeighTimeCnt;      //称量次数
extern u32 gu32_TargetTemp;        //目标值的缓存
extern bool gb_LiftBagStartFlg;    //启动提袋
extern bool gb_PushBagStartFlg;    //启动推袋
extern u32 gu32_SumTemp;           //累计的缓存值
extern bool gb_ConveyorStartFlg;   //启动输送机


typedef struct
{
    u8  u8Process;          //自学习过程
    u32 u32StartMoment;     //加料开始时间
    u32 u32SP2Time;         //中投加料时间
    u32 u32SP3Time;         //小投加料时间
    float fSP2Flux;         //中投流量
    s32 s32SP2StopResult;   //中投投料结果
    s32 s32SP3StopMomentWt; //小头停止时刻重量
    s32 s32SP3StopResult;   //小投投料结果
    s32 s32SP3K;            //小投线性拟合K
    u32 u32ForbidTime;      //自学习的禁止比较时间
    
    u32 u32SP3Val;          //自学习的小投落差值
}struct_autostudy_t;

extern struct_autostudy_t gs_AutoStd;
#define AUTOSTD_NONE        0   //
#define AUTOSTD_SP2         1   //自学习中投已启动
#define AUTOSTD_SP2STOP     2   //自学习中投已停止
#define AUTOSTD_SP3         3   //自学习小投已启动
#define AUTOSTD_SP3STOP     4   //自学习小投已停止
#define AUTOSTD_SP3FILL     5   //再次启动SP3完成加料
#define AUTOSTD_SECEND      6   //自学习第二次运行


#define SP3LINEARKLEN   10
typedef struct
{
    u8 u8Switch;    //小投截止点预测功能的开关
    s32 s32KSampleArray[SP3LINEARKLEN];   //保存小投最近斜率的数组 
    u32 u32KSampleTimes;    //设置的K值保存个数
    u32 u32KCntSample;      //已经保存的K值个数计数
    u8  u8State;    //0x00=小投截止点预测功能没有打开
                    //0x10=前期线性拟合K值采集阶段 还没有越过不拟合采样次数u3InvalidTimes
                    //0x11=前期线性拟合K值采集阶段 已经越过不拟合采样次数 还没有达到最少有效采样次数u32ValidTimes
                    //0x12=前期线性拟合K值采集阶段 已经达到最少有效采样次数u32ValidTimes
                    //0x20=线性拟合K值已经采集完毕 还没有越过不拟合采样次数u3InvalidTimes
                    //0x21=线性拟合K值已经采集完毕 已经越过不拟合采样次数 还没有达到最少有效采样次数u32ValidTimes
                    //0x22=线性拟合K值已经采集完毕 已经达到最少有效采样次数u32ValidTimes 开始接管小投截止点控制
    bool bSampleDoneFlg;    //前期线性拟合K值采集已经完毕
    u32 u32PredictTarget;   //预测的小投截止点还需要多少次AD转换
    s32 s32RealStopWt;      //小投停止时对应的实际重量
    u32 u32CntTarget;       //预测的小投截止点之后的计数变量
    s32 s32KThisTime;       //本次加料的小投K值
    s32 s32KAverage;        //计算的平均K值
    u32 u32InvalidTimes;     //谷值后不拟合的无效采样次数
    u32 u32CntInvalid;      //谷值后不记采样次数计数变量
    u32 u32ValidTimes;      //开始拟合的最少有效采样次数
    u32 u32CntValid;        //开始记录采样次数的计数变量
    
}PREDICTSP3;    //预测小投结束相关参数结构体
extern PREDICTSP3 gs_PredictSp3;
extern PREDICTSP3 gs_PredictSp3Temp;   //打印的时候使用作为缓存

typedef struct
{
    u32 u32Sp1Time;         //快加时间
    u32 u32Sp1TimeTemp;     //快加时间缓存
    u32 u32Sp2Time;         //中加时间
    u32 u32Sp2TimeTemp;     //中加时间缓存
    u32 u32Sp3Time;         //慢加时间
    u32 u32Sp3TimeTemp;     //慢加时间缓存
    u32 u32RunCmpTime;      //启动到定值结束时间
    bool bGetRunCmpTimeFlg; //启动到定值结束时间已经获取到标志位
    u32 u32OneTareTime;     //一次加料过程时间
    u32 u32RunStartTimeTemp;     //启动时刻缓存
    u32 u32SumCnt;      //包数
    u32 u32TotalTime;   //总用时
    u32 u32FirstRunTimeTemp;
    bool u32HadPrintedMin;       //已经打印总结数据的分钟数    
    u32 u32PredictSp2StopTime;  //预测的中投停止时间
    u32 u32MaxWt;       //峰值
    u32 u32MinWt;       //谷值
    u32 u32Sp2StopToMaxTime;    //中投截止点到峰值时间
    u32 u32MaxToMinTime;        //峰值到谷值时间
    u32 u32MaxToMinTimeTemp;
    s32 s32Sp2MaxKNotFed;       //中投滤波前最大k
    s32 s32Sp2MaxKFed;          //中投滤波后最大k
    s32 s32MaxKFedReLinear;     //最大k附近点的重新拟合斜率F3.11设置拟合点数
    s32 s32MaxBFedReLinear;     //最大K附近点的重新拟合b值
    
    s32 s32SP3RealStopWt;       //实际停小投时的重量值
    s32 s32SP2RealStopWt;       //实际停中投时的重量值
}DEBUG_RUN_TIME;

typedef struct 
{
//    u32 u32Result[3000];    //加料结果
    u32 u32StartTime;       //开始时间记录 及总共用时记录
    u32 u32TotalTime;       //总共用时记录
    u32 u32SumNum;          //完成包数
    u32 u32SumWt;           //完成重量
    u32 u32NumAHour;        //每小时包数
    u32 u32WtAHour;         //每小时重量
    u32 u32NumAMin;         //每分钟包数
    u32 ResultAverage;      //平均每包重量
    u32 u32MostOverResult;  //最高超差值
    u32 u32MostOverNum;     //最高超差值对应包数
    u32 u32MostUnderResult; //最低欠差值
    u32 u32MostUnderNum;    //最低欠差值对应包数
    u32 u32TheMaxValue;     //最高值
    u32 u32TheMinValue;     //最低值
    u32 u32ErrorBand;       //误差带
    
    u32 u32Dst_Under_Num;   //分布情况 欠差包数
    u32 u32Dst_Under_Per;   //百分数
    u32 u32Dst_U_2_Num;   //分布情况
    u32 u32Dst_U_2_Per;   //百分数
    u32 u32Dst_2_1_Num;    //分布情况
    u32 u32Dst_2_1_Per;    //百分数
    u32 u32Dst_1_0_Num;     //分布情况
    u32 u32Dst_1_0_Per;     //百分数
    u32 u32Dst_0_1_Num;     //分布情况
    u32 u32Dst_0_1_Per;     //百分数
    u32 u32Dst_1_2_Num;    //分布情况
    u32 u32Dst_1_2_Per;    //百分数
    u32 u32Dst_2_O_Num;   //分布情况
    u32 u32Dst_2_O_Per;   //百分数
    u32 u32Dst_Over_Num;    //分布情况 超差包数
    u32 u32Dst_Over_Per;    //百分数
    
}STATISTIC_DATA;

#define ONESTEP 5

extern STATISTIC_DATA gs_Statistic; //统计数据
extern DEBUG_RUN_TIME gs_DebugRunTime;

#ifdef DEBUG_MONITORTIME_PRINT
    extern DEBUG_RUN_TIME gs_DebugRunTimeTemp; //打印的时候使用作为gs_DebugRunTime的缓存
    extern u32 gu32_DebugTarget;
    extern u32 gu32_DebugLarge;
    extern u32 gu32_DebugMiddle;
    extern u32 gu32_DebugSmall;
#endif


extern void Key_RunState(void);
extern void FlapBag(void);
extern void UpdateFastSetRec(void);
extern bool StabOver(bool lb_Reset);
void PredictSp3_Restart(void);
void StatisticClrStruct(void);
void StatisticSaveStartTime(u32 fu32_Time);

#endif

