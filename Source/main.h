#ifndef _MAIN_H
#define _MAIN_H

#include "71x_type.h"




extern u32  gu32_SystemState;          //系统工作状态
extern ULDATA ulParameter;
extern u8  gu8_Item;
extern bool  gb_PrintCodeIngFlg;      //打码延时启动标志
extern bool gb_StopPrintCodeFlg;       //停止打码

#ifdef DEBUG_ADCNVT_TIMES
    extern u32 gu32_AdConventTimes;        //调试用AD转换次数
#endif

#define RESETSTATE              0x00
#define RUNSTATE                0x01            /**/
#define STOPSTATE               0x02            /**/
#define RECIPESTATE             0x03            /**/
#define SETUPSTATE              0x04            /**/
#define IOTESTSTATE             0x05            /**/
#define IODEFINESTATE           0x06            /**/
#define CALIBSTATE              0x07            /**/
#define TIMESTATE               0x08            /**/
#define DATESTATE               0x09            /**/
#define BATCHSTATE              0x0A
#define SUMSTATE                0x0B
#define SHUTDOWNSTATE           0x0C
#define BORRSTATE               0x0D
#define DEBUGSTATE              0x0E
#define SIMULATIONSTATE         0x0F
#define MODELSETSTATE           0x10
#define PRINTSTATE              0x11


//#define FASTSETSTATE            











#endif
