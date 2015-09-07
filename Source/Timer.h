#ifndef _TIMER_H
#define _TIMER_H

#include "71x_type.h"

extern vu32 gu32_Sys10ms;
extern vu32 gu32_Sys1ms;   //1ms
extern void Timer0_Init(void);
extern void Timer1_Init(void);
extern void Sp2StopTimeStart(bool* fbp_TimeRunFlg);
extern void Sp2StopTimeStop(bool* fbp_TimeRunFlg);
extern void T1_IRQ(void);

#endif
