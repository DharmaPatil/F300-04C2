#ifndef _SETUP_H
#define _SETUP_H

#include "71x_type.h"

#define DISP_CURR   0       //显示重量
#define DISP_TOL1   1       //显示总累计
#define DISP_TOL2   2       //显示收发货累计
#define DISP_FLU    3       //显示流量

#define BINYES      0       //有计量斗模式
#define BINNO       1       //无计量斗模式
#define BIN2_A      2       //双秤A
#define BIN2_B      3       //双秤B
#define NO2_A       4       //双无斗A
#define NO2_B       5       //双无斗B

//#define BULK        2       //散料累计模式

#define GROSS       0       //无斗毛重
#define NET         1       //无斗净重

#define CO          0       //组合给料
#define SIN         1       //单独给料

#define AUX_REC     0       //副显示配方
#define AUX_SUM     1       //副显示总累计次数
#define AUX_SUM_R   2       //副显示配方累计次数


#define OFF         0
#define ON1         1
#define ON2         2


struct SETUP
{   //B1
    u32 TareMode;           //秤体模式
    u32 GorNMode;           //无斗毛/净重方式
    u32 FeedMode;           //给料方式
    u32 AutoClrZeroInterval;//自动清零间隔
    u32 JudgeStabOverTime;  //判稳超时时间
    u32 AuxDispType;        //副显示内容
    //B2
    u32 ClrZeroRng;         //清零范围
    u32 JudgeStabRng;       //判稳范围
    u32 TraceZeroRng;       //零点跟踪范围
    u32 AdFilterLevel;      //AD数字滤波参数
    u32 AdFilterLevelDisc;  //卸料滤波参数
    u32 AdFilterLevelComp;  //定值滤波参数
    u32 AdSamplingRate;     //AD采样速度
    u32 ExtType;            //扩展接口类型
    //B3
    u32 PowerOnZeroSwitch;  //上电自动清零开关
    u32 DiscSumSwitch;      //手动卸料累计开关
    u32 ResultHoldSwitch;   //定值重量保持开关
    u32 SumCheckCodeSwitch; //累计查询密码开关
    u32 SumClrCodeSwitch;   //累计清除密码开关
    u32 SumExtendSwitch;    //累计范围扩展开关
    //B4
    u32 ComMode_1;          //通讯方式
    u32 TareNumber_1;       //设备号
    u32 BaudRate_1;         //波特率
    u32 ParityMode_1;       //数据格式
    u32 HiLo_1;             //modbus高低字顺序
    u32 PrintSwitch_1;      //自动打印开关
    u32 PrintType_1;        //打印格式
    u32 PrintLanguage_1;    //打印语言
    u32 PrintBlankRows_1;   //打印走纸行数
    //B5
    u32 ComMode_2;          //通讯方式        
    u32 TareNumber_2;       //设备号          
    u32 BaudRate_2;         //波特率          
    u32 ParityMode_2;       //数据格式        
    u32 HiLo_2;             //modbus高低字顺序
    u32 PrintSwitch_2;      //自动打印开关    
    u32 PrintType_2;        //打印格式
    u32 PrintLanguage_2;    //打印语言
    u32 PrintBlankRows_2;   //打印走纸行数
    //B6
    u32 No2PackOffTwoSwitch;    //双无斗秤互锁模式同时松袋开关
    u32 No2ConveyorDelayTime;   //输送机启动延时时间          
    u32 No2ConveyorRunTime;     //输送机运行时间              
    u32 No2AddAgainDelayTime;   //再次夹袋延时启动加料时间    
    //B7
    u32 AdaptSW;        //自适应开关
    u32 AdaptSP2SW;     //
    u32 AdaptSP2LimitRng;
    u32 AdaptSP2Level;
    u32 AdaptSP2TimeThreshold;
    u32 AdaptSP3SW;
    u32 AdaptSP3LimitRng;
    u32 AdaptSP3Scope;
    u32 AdaptSP2Scope;
    u32 Adapt7_9;
    
};
extern struct SETUP gs_Setup;
extern u32 gu32_SetupCodeSwitch;              //工作参数密码开关
extern u32 gu32_SetupCode;                    //工作参数密码
extern u8 gu8_SetupInTime;                //工作参数密码输入错误次数


extern void Key_SetupState(void);
extern void Disp_SetupState(void);
extern void TareModeChange(u32 fu32_NewTareMode);


#endif
