#ifndef _STOPANDRUN_H
#define _STOPANDRUN_H

#include "71x_type.h"


extern bool gb_FastSetFlg;         //�������ñ�־λ
extern u32 gu32_FastSetTemp[6];    //���������䷽�������ݴ�
extern bool gb_UpdateRecFlg;       //��Ҫ�����䷽��־
extern u8 gu8_FastSetSigTemp;      //���������޸Ĳ����ı�־
extern bool gb_FastCalFlg;         //���ٱ궨��־λ

extern void Key_StopState(void);
extern void Disp_StopRunState(void);
extern void Key_FastSetState(void);


#endif
