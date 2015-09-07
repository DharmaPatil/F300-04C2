#ifndef _PRINTSTATE_H
#define _PRINTSTATE_H

#include "71x_Type.h"

#define PRTTYPE_80      2         //80列打印
#define PRTTYPE_32      1         //32列打印
#define PRTTYPE_16      0         //16列打印

#define PRINTER_CHN       0         //中文打印
#define PRINTER_ENG       1         //英文打印

#define PRINT_AUTO          0       //自动打印
#define PRINT_TOTAL_REPORT  1       //总累计报表打印
#define PRINT_REC_REPORT    2       //配方报表打印
#define PRINT_REC_SET       3       //配方设置打印
#define PRINT_AUTO_DEBUG    4       //调试打印数据
#define PRINT_STATISTIC     5       //统计数据




extern volatile bool gvb_PrintingFlg_UART0;        //串口正在打印标志
extern volatile u8 gvu8_PrintCnt_UART0;            //打印多少行
extern bool gb_PrintAllRecFlg_UART0;      //打印所有配方开始标志
extern bool gb_FirstPrintFlg_UART0;      //第一次打印标志 用来打印一些抬头说明

extern volatile bool gvb_PrintingFlg_UART1;        //串口正在打印标志
extern volatile u8 gvu8_PrintCnt_UART1;            //打印多少行
extern bool gb_PrintAllRecFlg_UART1;      //打印所有配方开始标志
extern bool gb_FirstPrintFlg_UART1;      //第一次打印标志 用来打印一些台头说明

extern bool gb_PrintAllFlg;        //打印所有配方数据
extern u32 gu32_PrintRecNumStart;


extern void Key_PrintState(void);
extern void Disp_PrintState(void);
extern void PrintSum(void);
extern void PrintStart(u8 fu8_PrintMode, u32 fu32_Result, u8 fu8_ComNum);
extern void PrintDeal(u8 *fu8p_Array);
extern void PredictSp3_Init(void);



#endif
