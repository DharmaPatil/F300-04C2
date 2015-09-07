#ifndef _INCLUDE_
#define _INCLUDE_

//以下为调试用功能的宏定义，正式编译时要注释掉      for test

#define DEBUG_ADCNVT_TIMES      //副显示实际AD转换次数/秒

#define DEBUG_WT_SEND           //注意不能和下面的DEBUG_WT_STATE_SEND同时开启
//CONT通讯方式下：串口1不停发送滤波前AD码和滤波后AD码；串口2不停发送滤波前重量和滤波后重量

//#define DEBUG_WT_STATE_SEND     //注意不能和上面的DEBUG_WT_SEND同时开启
//CONT通讯方式下：不停发送当前重量和加料相关数据。

#define DEBUG_MONITORTIME_PRINT
//时间改成ms为单位 2015-7-23
//PRINT方式每次加料过程结束后，发送加料结果相关数据，格式如下：（在Printstate.c中）
//    {"调试、测试的数据   日期:          时间:             AD转换速度:   次/秒 \r\n"},
//    {"-----------------------------------------------------------------------------------------------------\r\n"},
//    {" 快加  中加  慢加 定值完 卸料完   结果   目标值  大投值  中投值  小投值 T2a  T2b T1  T4  包数 总用时 \r\n"},
//    {" (秒)  (秒)  (秒)   (秒)   (秒)  (    )  (    )  (    )  (    )  (    ) (秒)(秒)(秒)(秒)       (秒)  \r\n"} };


#define DEBUG_JUDESTABLE_TIME
//判稳时间参数，最大可以设置0.5s，由b1.5参数设置

#define DEBUG_T2A
//启动时中投禁止比较时间设置不为0，即使重量已经超过中投截止点也启动中投走完中投禁止比较时间

#define DEBUG_RUN_STOP_BY_KEY
//按键控制启动急停 TARE键启动 →键急停

//#define REFRESH_AT_SP23_MAX
//在中投结束后在重量的最高点刷新采样队列

#define DEBUG_SMARTT2B
//智能判断T2B禁止比较时间

#define DEBUG_SMARTT2B_FILTER
//这个的前提是已经使用了上面的DEBUG_SMARTT2B智能判断禁止比较时间
//智能判断禁止比较时间时所用的滤波级数，用F2.3(零点跟踪范围)来设置

#define ADFILTER_02CS
//使用GM8802CS的滤波数组 有120、240、480不同AD速度的滤波

#define DISC_TIME_CONTROL
//时间控制卸料 F1.5单位为ms 小数点无效

//#define PREDICT_SP2_STOP
//智能预测中投截止点    //用B3.3开关来控制

#define DONOT_SAVE_SUM
//不保存累计数据到铁电  防止影响串口发送数据的速度


    #include "string.h"     //为了使用memcpy()内存拷贝,memset()等函数
    #include "71x_lib.h"
    #include "stdlib.h"     //随机数函数srand() rand() abs()等
    #include "IO.h"
    #include "RTC.h"
    #include "Buzzer.h"
    #include "UserBin.h"
    #include "UserLib.h"
    #include "F_RAM.h"
    #include "VFD.h"
    #include "Timer.h"
    #include "AD.h"
    #include "Parameter.h"
    #include "DisplayAndKey.h"
    #include "main.h"
    #include "StopAndRunState.h"
    #include "CalibState.h"
    #include "ADtoWeight.h"
    #include "SetupState.h"
    #include "Recipe.h"
    #include "TimeAndDate.h"
    #include "BatchState.h"
    #include "Sum.h"
    #include "ShutdownState.h"
    #include "RunState.h"
    #include "UART.h"
    #include "Modbus.h"
    #include "DebugState.h"
    #include "PrintState.h"
    #include "PROFIBUS_DP.h"

#endif
