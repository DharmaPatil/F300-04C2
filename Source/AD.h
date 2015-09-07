#ifndef _AD_H
#define _AD_H

#include "71x_type.h"

extern bool gb_AdErrFlg;           //AD错误标志
extern bool gb_AdOKFlg;            //AD初始化成功标志位
extern bool gb_AdOflFlg;           //AD溢出标志位
extern bool gb_OflFlg;             //重量溢出标志位
extern bool gb_SigWtFlg;             //符号标志

extern u32 gu32_AdValue;           //AD转换得到的数据


extern bool AD_Init(u8 i);
extern u32 AD_GetFedVal(u32 fu32_FLevel);
extern u32 AD_ConvertTimes(u32 fu32_SysTime);

#endif
