/*
Description:            ��ѧ�����߼��Ⱥ���
Author:                 �Ժ���
Date:                   2015-7-29
*/

#include "71x_type.h"
#include "MathLogic.h"


//s32 gs32_WtNotFedArray[LINEARARRAY_LENGTH+1];   //�˲�ǰ��·���� ���µ������
s32 gs32_WtFedArray[LINEARARRAY_LENGTH+1];     //�˲����������� ���µ������
s32 gs32_LinearKFed;        //�˲����������������kֵ
s32 gs32_SP2KFed;       //��Ͷ��ֹ�㸽�����������Kֵ ����F3.9����


/*********************************************************************
��������: SaveLinearArray
��    ��: ����ֵ��������������ȫ�ֱ���������
˵    ��: 
��ڲ���: fs32p_Array=�������ȫ�ֱ��������׵�ַ
          fs32_Data=�µ���������
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-30
��    ��:                      ʱ    ��: 
*********************************************************************/
void SaveLinearArray(s32* fs32p_Array, s32 fs32_Data)
{
    u16 i;
    
    for (i = 0; i < LINEARARRAY_LENGTH; i++)
    {
        fs32p_Array[i] = fs32p_Array[i+1];
    }
    fs32p_Array[LINEARARRAY_LENGTH] = fs32_Data;
}


/*********************************************************************
��������: s32 LeastLinear(s32* fs32p_Array, u16 ucWTNum,u16 CheckNum, u8 ucXorYTempFlag, s32 XorYtemp)
��    ��: ������Ϻ���
˵    ��: ʹ��ȫ�ֱ������ݲ���
��ڲ���: 
�� �� ֵ: ucWTNum=ȫ�ֱ������� �������������� ������
          CheckNum=ȫ�ֱ������� ��ʼ�������ʼ�� ������
          ucXorYTempFlag=0��֪y��x 1��֪x��y 2б��kֵ 3����bֵ
          XorYtemp=��֪��x��yֵ
��    ��: �Ժ���               ʱ    ��: 2015-07-29
��    ��:                      ʱ    ��: 
*********************************************************************/
s32 LeastLinear(s32* fs32p_Array, u16 ucWTNum,u16 CheckNum, u8 ucXorYTempFlag, s32 XorYtemp)
{
    #define LINEAR_X    0
    #define LINEAR_Y    1
    #define LINEAR_K    2
    #define LINEAR_B    3
    
    s32 YourTemp = 0;   //���ؽ��
    s32 K_Slope = 0;    //б��kֵ
    s32 b_Init = 0;     //����bֵ

    u16 i;    
    s64 sllTSum;            //ʱ��ĺ�
    s64 sllTSum2;           //ʱ��ĺ͵�ƽ��

    s64 sllWSum;            //�����ĺ�
    s64 sllWTSum;           //����*ʱ��ĺ�   
    s64 sllNT2Sum;          //ʱ���ƽ���ĺ�*N
    
    if (ucWTNum <= CheckNum || ucXorYTempFlag > LINEAR_B)
    {   //����
        return 0;
    }
    
    // N = ucWTNum;
    //sum(Y)
    sllWSum=0;
    for(i=CheckNum;i<=ucWTNum;i++)  //����ͳһʹ�����ֵ
    {
        sllWSum+=(fs32p_Array[ucWTNum]-fs32p_Array[i]);
    }       
    //sum(XY)
    sllWTSum=0;
    for(i=CheckNum;i<=ucWTNum;i++)
    {
        sllWTSum+=(i*(fs32p_Array[ucWTNum]-fs32p_Array[i]));
    }               
    //sum(X) sum(X^2)
    sllTSum =0;
    sllNT2Sum=0;
    for(i=CheckNum;i<=ucWTNum;i++)
    {
        sllTSum+=i;
        sllNT2Sum+=(i*i);
    }
    //(sum(X))^2
    sllTSum2=sllTSum*sllTSum;

    K_Slope = 1000*(sllTSum*sllWSum - sllWTSum*(ucWTNum-CheckNum+1)) /(sllNT2Sum*(ucWTNum-CheckNum+1)-sllTSum2);
    b_Init = (sllTSum*sllWTSum - sllNT2Sum*sllWSum) /(sllNT2Sum*(ucWTNum-CheckNum+1)-sllTSum2) + fs32p_Array[ucWTNum];

    if(ucXorYTempFlag==LINEAR_X)
    {
        YourTemp = (s64)((XorYtemp - b_Init)*1000)/K_Slope;    //��֪Y��X      
    }
    else if(ucXorYTempFlag==LINEAR_Y)
    {
        YourTemp = (s64)(K_Slope*XorYtemp)/1000 + b_Init;            //��֪X��Y
    }
    else if(ucXorYTempFlag==LINEAR_K)
    {
        YourTemp = K_Slope;
    }
    else if(ucXorYTempFlag==LINEAR_B)
    {
        YourTemp = b_Init;
    }

    return YourTemp;
}






