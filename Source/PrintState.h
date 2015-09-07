#ifndef _PRINTSTATE_H
#define _PRINTSTATE_H

#include "71x_Type.h"

#define PRTTYPE_80      2         //80�д�ӡ
#define PRTTYPE_32      1         //32�д�ӡ
#define PRTTYPE_16      0         //16�д�ӡ

#define PRINTER_CHN       0         //���Ĵ�ӡ
#define PRINTER_ENG       1         //Ӣ�Ĵ�ӡ

#define PRINT_AUTO          0       //�Զ���ӡ
#define PRINT_TOTAL_REPORT  1       //���ۼƱ����ӡ
#define PRINT_REC_REPORT    2       //�䷽�����ӡ
#define PRINT_REC_SET       3       //�䷽���ô�ӡ
#define PRINT_AUTO_DEBUG    4       //���Դ�ӡ����
#define PRINT_STATISTIC     5       //ͳ������




extern volatile bool gvb_PrintingFlg_UART0;        //�������ڴ�ӡ��־
extern volatile u8 gvu8_PrintCnt_UART0;            //��ӡ������
extern bool gb_PrintAllRecFlg_UART0;      //��ӡ�����䷽��ʼ��־
extern bool gb_FirstPrintFlg_UART0;      //��һ�δ�ӡ��־ ������ӡһЩ̧ͷ˵��

extern volatile bool gvb_PrintingFlg_UART1;        //�������ڴ�ӡ��־
extern volatile u8 gvu8_PrintCnt_UART1;            //��ӡ������
extern bool gb_PrintAllRecFlg_UART1;      //��ӡ�����䷽��ʼ��־
extern bool gb_FirstPrintFlg_UART1;      //��һ�δ�ӡ��־ ������ӡһЩ̨ͷ˵��

extern bool gb_PrintAllFlg;        //��ӡ�����䷽����
extern u32 gu32_PrintRecNumStart;


extern void Key_PrintState(void);
extern void Disp_PrintState(void);
extern void PrintSum(void);
extern void PrintStart(u8 fu8_PrintMode, u32 fu32_Result, u8 fu8_ComNum);
extern void PrintDeal(u8 *fu8p_Array);
extern void PredictSp3_Init(void);



#endif
