#ifndef _MATHLOGIC_H
#define _MATHLOGIC_H

#include "71x_type.h"

#define LINEARARRAY_LENGTH      241

//extern s32 gs32_WtNotFedArray[LINEARARRAY_LENGTH+1];   //�˲�ǰ��·���� ���µ������
extern s32 gs32_WtFedArray[LINEARARRAY_LENGTH+1];     //�˲����������� ���µ������
extern s32 gs32_LinearKFed;        //�˲����������������kֵ
extern s32 gs32_SP2KFed;       //��Ͷ��ֹ�㸽�����������Kֵ ����F3.9����.

void SaveLinearArray(s32* fs32p_Array, s32 fs32_Data);
s32 LeastLinear(s32* fs32p_Array, u16 ucWTNum,u16 CheckNum, u8 ucXorYTempFlag, s32 XorYtemp);

#endif
