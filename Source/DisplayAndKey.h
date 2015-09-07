#ifndef _DISPLAYANDKEY_H
#define _DISPLAYANDKEY_H

#include "71x_type.h"

#define MODELSET_CHECKCODE      0
#define MODELSET_SWITCH         1
#define MODELSET_SETUP_0        2
#define MODELSET_SETUP_1        3
#define MODELSET_SETUP_2        4
#define MODELSET_SETUP_3        5
#define MODELSET_SETUP_4        6
#define MODELSET_SETUP_5        7
#define MODELSET_SETUP_6        8

extern u32 gu32_KeyCode;       //按键值
extern u32 gu32_FlashDelay;           //闪烁延时
extern u8 gu8_DataInLength;       //数据输入长度
extern bool gb_SetFlg;          //SET按键标志
extern bool gb_FirstNumKeyFlg;   //第一次按键清零标志
extern u8 gu8_DispErrorNumber;         //错误号
//extern u8 gu8_DispErrorNumberBackup;   //错误号备份
extern bool gb_DispErrorFlg;
extern bool gb_HandClrErrorFlg;
extern u32 gu32_DispErrorTime;
extern u32 gu32_ErrTimeDelay;
extern bool gb_FlashFlg;       //闪烁标志位



extern void Disp_Version(void);
extern u32 Key_Deal(void);
extern void Disp_Main_Data(u32 ful_MainDispVal, u32 lul_Point, u8 fuc_DispNumber, bool fb_Echo0, bool fb_Ofl);
extern u32 Key_NumInput(u32 ful_Value, u8 fuc_Len);
extern void Disp_Error(void);
extern void ErrorDeal(u8 fuc_Number, u32 ful_Time);
extern void Flash_MainDisp(u8 fuc_Postion);
extern void Disp_InputCode(void);
extern u32 Key_IncInput(u32 ful_Value, u8 fuc_Min, u8 fuc_Max);
extern void Disp_OnOff(u32 ful_Data);
extern void Disp_FlapOnOff(u32 ful_Data);
extern void Disp_TareMode(u32 ful_Data);
extern void Disp_GorNMode(u32 ful_Data);
extern void Disp_BaudRate(u32 ful_Data);
extern void Disp_ComMode(u32 ful_Data);
extern void Disp_FeedMode(u32 ful_Data);
extern void Disp_ParityMode(u32 ful_Data);
extern void Disp_HiLo(u32 ful_Data);
extern void Disp_AdSamplingRate(u32 ful_Data);
extern void Disp_ExtType(u32 ful_Data);
extern void Disp_Aux_Rec(u32 fu32_Num);
extern void Flash_AuxDisp(u8 fuc_Postion);
extern u32 Key_T9Input(u32 ful_Value);
extern void Disp_FlgLight(void);
extern void Disp_Echo0(u32 lul_Point, u8 *fucp_DataArry, u8 i);
extern bool Flash_Flg(bool fb_State);
extern void Disp_PrintLanguage(u32 ful_Data);
extern void Disp_PrintType(u32 ful_Data);
extern void Disp_GSDType(u32 ful_Data);
extern void Disp_No2PackOffTwoSwitch(u32 fu32_Data);


#endif


