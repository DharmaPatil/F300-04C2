#ifndef _SHUTDOWN_H
#define _SHUTDOWN_H


#define SHUTDOWN_CHECKCODE      0
#define SHUTDOWN_SWITCH         1
#define SHUTDOWN_DAYNUM         2
#define SHUTDOWN_INPUTCODE      3
#define SHUTDOWN_BUYID          4
#define SHUTDOWN_UNLOCK         5

extern u8  gu8_ShutDownInTime;         //密码输入错误次数
extern u32 gu32_BuyerID;               //客户代码 00~99 对应不同的ID算法.
extern u32 gu32_RandomID;              //随机数ID
extern u32 gu32_ShutDownCode;          //定时锁机用户密码
extern u32 gu32_RandomUnlockCode;      //随机解锁密码
extern u32 gu32_Random30DayCode;       //随机30天密码
extern u32 gu32_Random60DayCode;       //随机60天密码
extern u32 gu32_ShutDownSwitch;        //定时锁机开关
extern u32 gu32_LeaveDays;             //剩余锁机天数
extern u32 gu32_GetedRandom;           //随机数已经获取标志
extern bool gb_ReCmpFlg;               //已锁机标志，重新比较锁机时间
extern bool gb_SysClosedFlg;           //系统已经锁机标志
extern bool gb_JudgeShutDownFlg;       //判断锁机标志位


extern void Disp_ShutdownState(void);
extern void Key_ShutdownState(void);
extern void UpdataGrandShutCode(void);
extern u32 GetRandom(void);
extern bool GradeShutCodeEQ(void);
extern void Sys_Closed_Set(void);
extern void CmpDay(void);
extern void Auto_ShutDown(void);


#endif
