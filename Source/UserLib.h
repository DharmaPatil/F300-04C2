#ifndef _USERLIB_H
#define _USERLIB_H

#include "71x_type.h"

#define MAX(A,B)    ((A>=B)?A:B)    //返回较大的数


extern void ShortDelay(void);
extern void LongDelay(void);
extern void delay(u32 u32DelayTime);

//extern void POWOn_Buzer(void);
//extern void BuzerS(void);
//extern void BuzerL(void);
//extern void OPBuzer(void);

extern void BIN4toNBCD(u32 ulBin,u8* BcdArry,u8 Length);
extern void BIN4toNASC(u32 ulBin,u8* ASCArry,u8 Length);
extern void BIN4toNHex(u32 ulBin,u8* HexArry,u8 Length);
extern void BIN4toNHexASC(u32 ulBin,u8* HexArry,u8 Length);
extern bool ASCto4Bin(u32* pu32Bin,u8* ASCArry,u8 Length);
extern u32 RoundOff(u32 ful_DataA, u32 ful_DataB);
extern void BIN4toNASC_Point(u32 ulBin,u8* ASCArry,u8 Length, u32 Point);
extern void BIN4toNASC_Echo0(u32 ulBin,u8* ASCArry,u8 Length);
extern void ASCtoHex(u8* ASCArry, u8* HexArry, u8 Length);
extern void HextoASC(u8* HexArry, u8* ASCArry, u8 HexLen);

#endif
