#ifndef _AD_H
#define _AD_H

#include "71x_type.h"

extern bool gb_AdErrFlg;           //AD�����־
extern bool gb_AdOKFlg;            //AD��ʼ���ɹ���־λ
extern bool gb_AdOflFlg;           //AD�����־λ
extern bool gb_OflFlg;             //���������־λ
extern bool gb_SigWtFlg;             //���ű�־

extern u32 gu32_AdValue;           //ADת���õ�������


extern bool AD_Init(u8 i);
extern u32 AD_GetFedVal(u32 fu32_FLevel);
extern u32 AD_ConvertTimes(u32 fu32_SysTime);

#endif
