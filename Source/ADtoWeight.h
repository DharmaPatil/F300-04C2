#ifndef _ADTOWEIGHT_H
#define _ADTOWEIGHT_H

#include "71x_type.h"


extern u32  gu32_AdFedValue;       //AD数据进行滤波后得到的数据
extern u32  gu32_NetAdFedValue;    //减去标定零点的AD码
extern bool gb_NetAdFlg;           //减去标定零点的AD码符号位
extern u32  gu32_Nquad;            //4倍分度数
extern bool gb_SigNquadFlg;        //4倍分度数的符号位
extern u32  gu32_FloatZero;        //浮动零点
extern bool gb_SigFZeroFlg;        //浮动零点符号
extern u32  gu32_N;                //分度数
extern u32  gu32_Wt;               //重量
extern bool gb_StabFlg;        //稳定
extern bool gb_MvStabFlg;      //毫伏数稳定
extern bool gb_PowerOnZeroFlg;     //上电自动清零
extern bool gb_ZeroKeyFlg;         //按清零键
extern bool gb_ForceClrZeroFlg;    //强制清零标志位 如清零间隔到但是判稳超时
extern bool gb_AlarmFlg;       //报警标志
extern bool gb_ZeroPosition;   //零位标志
extern bool gb_NzFlg;          //零区标志
extern bool gb_DispNetFlg;         //显示净重标志位
extern bool gb_NetSigFlg;          //净重符号
extern u32 gu32_TareNquad;        //皮重值的4倍分度数
extern bool gb_TareSigFlg;         //皮重符号
extern s32 gs32_Wt;                //有符号数的重量值
extern s32 gs32_WtKalmanFed;       //卡尔曼滤波处理后的重量值有符号整形


#ifdef DEBUG_WT_SEND
extern u32 gu32_WtNotFed;          //没有滤波的AD码转换成的重量
extern bool gb_SigWtNotFedFlg;     //没有滤波的AD码转换成的重量的符号
#endif

#ifdef REFRESH_AT_SP23_MAX
    extern bool gb_RefreshAdArrayFlg;
#endif

//#ifdef DEBUG_SMARTT2B
    extern bool gb_T2bRunFlg;  //小投禁止比较时间已经开始
    extern u32 gu32_T2bTime;
//#endif

extern bool gb_Sp2StopTimeRunFlg;  //预测中投停止时间已经开始
extern u32 gu32_Sp2StopTime01ms;       //预测中投停止的时间 单位0.1ms

extern float gf_WtKalmanFed;       //卡尔曼滤波处理后的重量值
//卡尔曼滤波后显示重量值 跟分度有关
#define DISP_WT_KALMAN  (gf_WtKalmanFed > 0 ? ((u32)(gf_WtKalmanFed + (float)gs_Cal.Div / 2)) / gs_Cal.Div * gs_Cal.Div : ((u32)((-gf_WtKalmanFed) + (float)gs_Cal.Div / 2)) / gs_Cal.Div * gs_Cal.Div)
//卡尔曼滤波后重量值 跟分度无关 单位1
//#define WT_KALMAN   (gf_WtKalmanFed > 0 ? (u32)(gf_WtKalmanFed + 0.5) : (u32)((-gf_WtKalmanFed) + 0.5))

extern void GetWt(void);
void GetMaxLinearK(bool fb_StartFlg);

#endif


