#ifndef _RTC_H
#define _RTC_H


//实时时钟日期时间,符合RTC数据格式的,用于RTC
typedef struct
{
	u8 year;
	u8 month;
	u8 date;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
}sTime;

//十六进制日期时间,用于显示
typedef struct
{
	u32 hexdate;
	u32 hextime;
}dTime;

//实际日期时间,用于计算
typedef struct
{
	u8 year;
	u8 month;
	u8 date;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
}rTime;



extern sTime g_sSysDateTime;
extern dTime g_dSysDateTime;
extern void RTC_ReadDateTime(sTime* sReadDate);
extern void RTC_sDatetoASC(sTime* psTime,u8* a8Arry);
extern void RTC_sTimetoASC(sTime* psTime,u8* a8Arry);
extern void RTC_WriteDateTime(sTime* sWriteDateTime);
extern bool RTC_FrameTime(sTime sTargTime);
extern bool RTC_FrameDate(sTime sTargTime);
extern bool LeapYear(u16 year);
extern void RTC_DateToASC_Full(u8* a8Arry);
extern void RTC_TimeToASC_Full(u8* a8Arry);

extern void RTC_Init(void);

#endif
