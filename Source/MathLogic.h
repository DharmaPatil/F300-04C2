#ifndef _MATHLOGIC_H
#define _MATHLOGIC_H

#include "71x_type.h"

#define LINEARARRAY_LENGTH      241

//extern s32 gs32_WtNotFedArray[LINEARARRAY_LENGTH+1];   //滤波前中路数组 最新的在最后
extern s32 gs32_WtFedArray[LINEARARRAY_LENGTH+1];     //滤波后重量数组 最新的在最后
extern s32 gs32_LinearKFed;        //滤波后重量的线性拟合k值
extern s32 gs32_SP2KFed;       //中投截止点附近的线性拟合K值 个数F3.9设置.

void SaveLinearArray(s32* fs32p_Array, s32 fs32_Data);
s32 LeastLinear(s32* fs32p_Array, u16 ucWTNum,u16 CheckNum, u8 ucXorYTempFlag, s32 XorYtemp);

#endif
