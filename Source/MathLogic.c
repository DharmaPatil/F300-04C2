/*
Description:            数学运算逻辑等函数
Author:                 丛海旭
Date:                   2015-7-29
*/

#include "71x_type.h"
#include "MathLogic.h"


//s32 gs32_WtNotFedArray[LINEARARRAY_LENGTH+1];   //滤波前中路数组 最新的在最后
s32 gs32_WtFedArray[LINEARARRAY_LENGTH+1];     //滤波后重量数组 最新的在最后
s32 gs32_LinearKFed;        //滤波后重量的线性拟合k值
s32 gs32_SP2KFed;       //中投截止点附近的线性拟合K值 个数F3.9设置


/*********************************************************************
函数名称: SaveLinearArray
功    能: 重量值存入计算线性拟合全局变量的数组
说    明: 
入口参数: fs32p_Array=线性拟合全局变量数组首地址
          fs32_Data=新的重量数据
返 回 值: 
设    计: 丛海旭               时    间: 2015-07-30
修    改:                      时    间: 
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
函数名称: s32 LeastLinear(s32* fs32p_Array, u16 ucWTNum,u16 CheckNum, u8 ucXorYTempFlag, s32 XorYtemp)
功    能: 线性拟合函数
说    明: 使用全局变量传递参数
入口参数: 
返 回 值: ucWTNum=全局变量数组 结束计算的数组号 新重量
          CheckNum=全局变量数组 开始计算的起始号 旧重量
          ucXorYTempFlag=0已知y求x 1已知x求y 2斜率k值 3常数b值
          XorYtemp=已知的x或y值
设    计: 丛海旭               时    间: 2015-07-29
修    改:                      时    间: 
*********************************************************************/
s32 LeastLinear(s32* fs32p_Array, u16 ucWTNum,u16 CheckNum, u8 ucXorYTempFlag, s32 XorYtemp)
{
    #define LINEAR_X    0
    #define LINEAR_Y    1
    #define LINEAR_K    2
    #define LINEAR_B    3
    
    s32 YourTemp = 0;   //返回结果
    s32 K_Slope = 0;    //斜率k值
    s32 b_Init = 0;     //常数b值

    u16 i;    
    s64 sllTSum;            //时间的和
    s64 sllTSum2;           //时间的和的平方

    s64 sllWSum;            //重量的和
    s64 sllWTSum;           //重量*时间的和   
    s64 sllNT2Sum;          //时间的平方的和*N
    
    if (ucWTNum <= CheckNum || ucXorYTempFlag > LINEAR_B)
    {   //错误
        return 0;
    }
    
    // N = ucWTNum;
    //sum(Y)
    sllWSum=0;
    for(i=CheckNum;i<=ucWTNum;i++)  //这里统一使用相对值
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
        YourTemp = (s64)((XorYtemp - b_Init)*1000)/K_Slope;    //得知Y求X      
    }
    else if(ucXorYTempFlag==LINEAR_Y)
    {
        YourTemp = (s64)(K_Slope*XorYtemp)/1000 + b_Init;            //已知X求Y
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






