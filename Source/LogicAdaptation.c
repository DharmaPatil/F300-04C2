/*
********************************************************************************
* 文件说明 	:  落差修正程序
* 设  计 	:  lzs 					时  间 :  2015-6-24
********************************************************************************
*/

#include "weight.h"
#include "TimeTick.h"
#include "LogicAdaptation.h"
#include "LogicConfirm.h"


/*
********************************************************************************
* 功  能 :  落差修正结构体初始化
* 说  明 :  相应参数指向需要指向的地址
* 设  计 :  lzs 					时  间 :  2015-6-24 
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
_strAdapt gstrAdapt =
{
	&(gstrWeightInfo.s32GrossWt),    // 毛重包装时，指向毛重重量
	&(gsrtProductParam.s32SP1Val),   // 配方参数的大投提前量
	&(gsrtProductParam.s32SP2Val),   // 中投提前量 
	&(gsrtProductParam.s32SP3Val),   // 落差值 
	&(gsrtProductParam.s32SP2Val),   // 中投提前量 
	&(gstrRunResult.u32SlowFeedTime),// 实际小投时间    
	0, 
};

/*********************************************************************************
* 函数名 :  Adapt_InitSP12Temp
* 功  能 :  初始化大/中投修正暂存值
* 说  明 :  初次打开修正开关或上电或修正次数完成后初始化
* 入  参 :  none
* 返  回 :  none
* 设  计 :  lzs 					时  间 :  2015-6-24
* 修  改 :  none					时  间 :  none
*********************************************************************************/
void Adapt_InitSP12Temp(void)
{
    gstrAdapt.u32SP12Count = 0;
    gstrAdapt.u32SP12SubCount = 0;
}

/*********************************************************************************
* 函数名 :  Adapt_InitSP3Temp
* 功  能 :  初始化落差修正暂存值
* 说  明 :  初次打开修正开关或上电或修正次数完成后初始化
* 入  参 :  none
* 返  回 :  none
* 设  计 :  lzs 					时  间 :  2015-6-24
* 修  改 :  none					时  间 :  none
*********************************************************************************/
void Adapt_InitSP3Temp(void)
{
    gstrAdapt.u32SP3Count = 0;
}

/*
********************************************************************************
* 函数名 :  Adapt_InitData
* 功  能 :  初始化落差修正参数
* 说  明 :  none
* 入  参 :  none
* 返  回 :  none
* 设  计 :  lzs 					时  间 :  2015-6-24
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
void Adapt_InitData(void)
{
	const uint32_t u32ScopeValue[] = {100, 75, 50, 25};    		// 落差修正幅度
	const uint32_t u32SP12Time[] = {1800, 2000, 2200, 2500}; 	// 大/中投修正等级的时间上限   

	gstrAdapt.u32AdatpEn		= gsrtProductParam.u32SelfAdaptionSW;
	gstrAdapt.s32RushWt			= 0;
	gstrAdapt.s32SP1Bkp 		= gsrtProductParam.s32SP1Val;
	gstrAdapt.s32SP2Bkp 		= gsrtProductParam.s32SP2Val;

	gstrAdapt.u32SP12En 		= gsrtProductParam.u32SP12AffcSW;
    gstrAdapt.u32SP12Range 		= gsrtProductParam.u32SP12AffcRange;
	gstrAdapt.u32SP12Scope 		= u32SP12Time[(gsrtProductParam.u32SP12AffcScope < 3) ? gsrtProductParam.u32SP12AffcScope : 2];


	gstrAdapt.s32ConfirmWt 		= 0;
	gstrAdapt.s32TargetVal		= gsrtProductParam.s32Target;
	gstrAdapt.s32SP3Bkp 		= gsrtProductParam.s32SP3Val;

	gstrAdapt.u32SP3ForbidTime 	= gsrtProductParam.u32ForbidSP3Time;
	gstrAdapt.u32SP3En 			= gsrtProductParam.u32SP3AffcSW;
    gstrAdapt.u32SP3Range 		= gsrtProductParam.u32SP3AffcRange;
	gstrAdapt.u32SP3Scope 		= u32ScopeValue[(gsrtProductParam.u32SP3AffcScope <= 3) ? gsrtProductParam.u32SP3AffcScope : 0];
}
	
/*
********************************************************************************
* 函数名 :  Bubble_Sort
* 功  能 :  冒泡排序
* 说  明 :  none
* 入  参 :  *num	: 待排序序列
*			n		: 数据个数
* 返  回 :  none
* 设  计 :  lzs 					时  间 :  2015-7-7
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
void Bubble_Sort(int32_t *num, uint32_t n) 
{ 
    uint32_t i, j; 
	int32_t temp;

    for(i = 0; i < n - 1; i++) 
    { 
        for(j = 0; i + j < n - 1; j++) 
        { 
            if(num[j] > num[j + 1]) 
            { 
                temp = num[j]; 
                num[j] = num[j + 1]; 
                num[j + 1] = temp; 
            } 
        } 
    } 
}

/*
********************************************************************************
* 函数名 :  FallWtProbabilityStatistics
* 功  能 :  概率统计方法处理落差值
* 说  明 :  采用概率统计处理方法处理落差，避免个别大误差造成误修正
* 入  参 :  GFallTimes : 落差修正次数
*			RealFallweight : 实际得到的落差
* 返  回 :  统计处理后的落差值
* 设  计 :  czh 					时  间 :  2014-09-17
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
int32_t FallWtProbabilityStatistics(uint32_t GFallTimes, int32_t RealFallweight)
{
#define SLFALLWT 	7
   int32_t BestFall = 0;
   int32_t slFallWTemp = 0;
   uint32_t i = 0;
   uint32_t j = 0;
   static int32_t AddFallWTemp = 0;
   static int32_t SlFallWRecord[SLFALLWT];
   static int32_t SlFallW_Pro[SLFALLWT];

   if(GFallTimes>(SLFALLWT-1))
   {
   	  	for(i=0;i<(SLFALLWT-1);i++)
		{
		   SlFallWRecord[i] = SlFallWRecord[i+1];
		   SlFallW_Pro[i] = SlFallWRecord[i];
		}
		SlFallWRecord[SLFALLWT-1] = RealFallweight;
		SlFallW_Pro[SLFALLWT-1] = RealFallweight;

		Bubble_Sort(SlFallW_Pro, SLFALLWT); //冒泡排序
		BestFall = (SlFallW_Pro[1]+SlFallW_Pro[2]*3+SlFallW_Pro[3]*5+SlFallW_Pro[4]*3+SlFallW_Pro[5])/13;  //由于统计出落差值偏大
   }  
   else if(GFallTimes>=2)
   {
   		SlFallWRecord[GFallTimes] = RealFallweight;
   	  	for(i=0;i<=GFallTimes;i++)
		{
			SlFallW_Pro[i] = SlFallWRecord[i];
		}		
		Bubble_Sort(SlFallW_Pro, GFallTimes + 1); //冒泡排序
		if(GFallTimes>=4)
		{
		  	BestFall = (SlFallW_Pro[1]+SlFallW_Pro[2]*2+SlFallW_Pro[3])/4;
		}
		else if(GFallTimes>=3)
		{
		   	BestFall = (SlFallW_Pro[1]+SlFallW_Pro[2])/2;
		}
		else if(GFallTimes>=2)
		{
		   	BestFall = SlFallW_Pro[1];
		}		
		GFallTimes++;
   }
   else
   {
   		if(GFallTimes==0) //如果是刚启动，清零
		{
		   AddFallWTemp = 0;
		}
		SlFallWRecord[GFallTimes] = RealFallweight;
   	  	GFallTimes++;		
		AddFallWTemp += RealFallweight;
		BestFall = AddFallWTemp/GFallTimes;		  
   }

   return BestFall;
   
#undef SLFALLWT
}

/*
********************************************************************************
* 函数名 :  ModifyFallWeight
* 功  能 :  落差修正函数
* 说  明 :  none
* 入  参 :  RealFallweight : 实际得到的落差
* 返  回 :  修正量(有符号)
* 设  计 :  czh 					时  间 :  2014-03-21
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
int32_t ModifyFallWeight(int32_t RealFallweight)
{
	int32_t ModifyFall = 0;
	int32_t RealFallweightTemp = 0;
	
	RealFallweightTemp = gstrAdapt.s32TargetVal * (int32_t)gstrAdapt.u32SP3Range / (int32_t)100;
	if(RealFallweight > RealFallweightTemp)	 //限幅
	{
	   	RealFallweight = RealFallweightTemp;
	}	
	else if(RealFallweight < (0 - RealFallweightTemp))	 //限幅
	{
	   	RealFallweight = 0 - RealFallweightTemp;
	}	
	ModifyFall = (RealFallweight * ((int32_t)gstrAdapt.u32SP3Scope) / (int32_t)100);

	return ModifyFall;
}


/*
********************************************************************************
* 函数名 :  Adapt_SP3
* 功  能 :  落差修正  
* 说  明 :  若未进行欠差补料，且高速定值前，使用当前定值重量修正，否则，使用补料后的或高速定值的值来进行修正
* 入  参 :  *pAdapt : 落差修正处理指针
* 返  回 :  1 : 完成    0 : 未完成
* 设  计 :  lzs 					时  间 :  2015-7-7 
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
uint32_t Adapt_SP3(_strAdapt *pAdapt)
{
	int32_t  s32Temp = 0;		// 临时变量
	int32_t	 s32FallTemp = 0;	// 落差值临时变量
	
	if((0 == pAdapt->u32SP3En) || (0 == pAdapt->u32SP3Range))	// 开关关闭 / 修正次数为0 / 修正范围为0
	{
		Adapt_InitSP3Temp();
	}
	else
	{
		if(pAdapt->s32SP3Bkp != (*pAdapt->ps32SP3))	// 落差被修改
		{
			Adapt_InitSP3Temp();
		}
		else if(*pAdapt->pu32SP3RealTime <= (pAdapt->u32SP3ForbidTime * PRODUCT_PARAM_DELAY_TIME_UNIT + 20)) // 小投时间 <= 禁判时间+20ms阈值  不修正
		{
			// 不修
		}
		else
		{
			if(0 == pAdapt->s32ConfirmWt)	// 未进行过欠差补料或高速定值前10次学习的定值重量
			{
				pAdapt->s32ConfirmWt = *pAdapt->ps32Weight;
			}
			
			s32Temp = FallWtProbabilityStatistics(pAdapt->u32SP3Count, (pAdapt->s32ConfirmWt - pAdapt->s32SP3RealWt));
			s32Temp -= *pAdapt->ps32SP3;
			s32FallTemp = ModifyFallWeight(s32Temp);
			*pAdapt->ps32SP3 += s32FallTemp;
		}
	}
	return 1;
}

/***************************************************
函    数：s32 RushStdError(s32 StdErrorDelta)
函数功能： 过冲量概率统计函数		
参数说明： s32  RealRushweightDelta 实际过冲量值变化量
		   	
返 回 值： s32 返回在90%的最大幅值
设计时间： 2014-09-09
*****************************************************/
int32_t RushStdError(int32_t RealStdErrorDelta)
{
#define SLRUSHERRWT 50
	int32_t StdError = 0;
	int32_t slRushWDeltaTemp = 0;
	static int32_t SlRushRecord[SLRUSHERRWT];
	static int32_t SlRushRecord_pro[SLRUSHERRWT];
	int32_t SumErrorTemp = 0;
	uint32_t i = 0;
	uint32_t j = 0;
		

	if(gstrAdapt.u32SP12SubCount>SLRUSHERRWT-1)
	{
		for(i=0;i<SLRUSHERRWT-1;i++)
		{
			SlRushRecord[i] = SlRushRecord[i+1];
			SlRushRecord_pro[i] = SlRushRecord[i];
		}
		SlRushRecord[SLRUSHERRWT-1] = RealStdErrorDelta;
		SlRushRecord_pro[SLRUSHERRWT-1] = RealStdErrorDelta;
		
		Bubble_Sort(SlRushRecord_pro, SLRUSHERRWT); //冒泡排序
		
		SumErrorTemp = 0;			//用于做判断的值
		for(i=2;i<SLRUSHERRWT-2;i++)
		{
			SumErrorTemp += SlRushRecord_pro[i]; //可以有效去掉极值 
		}
		SumErrorTemp = SumErrorTemp/(SLRUSHERRWT-4);
		
		StdError = (SlRushRecord_pro[SLRUSHERRWT-2]+SlRushRecord_pro[SLRUSHERRWT-3]*6+SlRushRecord_pro[SLRUSHERRWT-4]*3)/10;
		if(gstrAdapt.u32SP12SubCount>SLRUSHERRWT)
			gstrAdapt.u32SP12SubCount = SLRUSHERRWT;
	}
	else
	{
		SlRushRecord[gstrAdapt.u32SP12SubCount] = RealStdErrorDelta;
		SlRushRecord_pro[gstrAdapt.u32SP12SubCount] = RealStdErrorDelta;
		
		Bubble_Sort(SlRushRecord_pro, gstrAdapt.u32SP12SubCount + 1); //冒泡排序

		SumErrorTemp = 0;			//用于做判断的值 
		if(gstrAdapt.u32SP12SubCount<3)
		{			
			for(i=0;i<gstrAdapt.u32SP12SubCount+1;i++)
			{
				SumErrorTemp += SlRushRecord_pro[i]; //可以有效去掉极值 
			}				
			SumErrorTemp = SumErrorTemp/(gstrAdapt.u32SP12SubCount+1);
			StdError = SlRushRecord_pro[gstrAdapt.u32SP12SubCount]*11/10;
		}
		else if(gstrAdapt.u32SP12SubCount<10)
		{
			for(i=1;i<gstrAdapt.u32SP12SubCount-1;i++)
			{
				SumErrorTemp += SlRushRecord_pro[i]; //可以有效去掉极值 
			}
			SumErrorTemp = SumErrorTemp/(gstrAdapt.u32SP12SubCount-2);
			StdError = SlRushRecord_pro[gstrAdapt.u32SP12SubCount-1]*11/10;
		}	
		else if(gstrAdapt.u32SP12SubCount<SLRUSHERRWT)
		{
			for(i=1;i<gstrAdapt.u32SP12SubCount-2;i++)
			{
				SumErrorTemp += SlRushRecord_pro[i]; //可以有效去掉极值 
			}
			SumErrorTemp = SumErrorTemp/(gstrAdapt.u32SP12SubCount-3);
			StdError = (SlRushRecord_pro[gstrAdapt.u32SP12SubCount-1]*3+SlRushRecord_pro[gstrAdapt.u32SP12SubCount-2]*6+SlRushRecord_pro[gstrAdapt.u32SP12SubCount-3])/10;

		}
		gstrAdapt.u32SP12SubCount++;		
	}	
	return StdError;
	
#undef SLRUSHERRWT
}

/***************************************************
函    数：s32 RushWeightProbability(s32 RealRushweightDelta)
函数功能： 过冲量概率统计函数		
参数说明： s32  RealRushweightDelta 实际过冲量值变化量
		   	
返 回 值： s32 返回在90%的最大幅值
设计时间： 2014-09-09
*****************************************************/
int32_t RushWeightProbability(int32_t RealRushweightDelta)
{
#define SLRUSHWTP 	7
	int32_t StdError = 0;
	int32_t slRushWDeltaTemp = 0;
	int32_t AverMaxToMin = 0;
	static int32_t SlRRecord[10];
	static int32_t SlRRecord_pro[10];
	uint32_t i = 0;
	uint32_t j = 0;
	int32_t MaxToMin = 0;
	int32_t StdRushW = 0;
	int32_t AddStdRushW = 0;	
	int32_t AverRushWeight = 0;

	if(gstrAdapt.u32SP12Count>SLRUSHWTP-1)
	{
		for(i=0;i<SLRUSHWTP-1;i++)
		{
			SlRRecord[i] = SlRRecord[i+1];
			SlRRecord_pro[i] = SlRRecord[i];
		}
		SlRRecord[SLRUSHWTP-1] = RealRushweightDelta;
		SlRRecord_pro[SLRUSHWTP-1] = RealRushweightDelta;
		Bubble_Sort(SlRRecord_pro, SLRUSHWTP); //冒泡排序

		StdRushW = (SlRRecord_pro[2]+SlRRecord_pro[3]*2+SlRRecord_pro[4])/4;
		AverRushWeight = StdRushW;
		MaxToMin = SlRRecord_pro[SLRUSHWTP-1] - StdRushW;
		AverMaxToMin = RushStdError(MaxToMin);	  //重点
		StdError = AverMaxToMin;
		StdRushW = StdRushW + StdError;
	}
	else
	{		
		SlRRecord[gstrAdapt.u32SP12Count] = RealRushweightDelta;
		for(i=0;i<gstrAdapt.u32SP12Count+1;i++)
		{
			AddStdRushW += SlRRecord[i];
		}
		StdRushW = AddStdRushW/(gstrAdapt.u32SP12Count+1);
		StdRushW = StdRushW + ((*gstrAdapt.ps32SP3) * (int32_t)3 / (int32_t)2);
		gstrAdapt.u32SP12Count++;
		
	}
//	StdRushW = StdRushW + GetRDataPointer()->EXFallWeight;//过冲预留量	
		
	return StdRushW;
	
#undef SLRUSHWTP
}

/*
********************************************************************************
* 函数名 :  ModifyRushWeight
* 功  能 :  大/中投修正函数
* 说  明 :  none
* 入  参 :  RealFallweight : 实际得到的落差
* 返  回 :  修正量(有符号)
* 设  计 :  czh 					时  间 :  2014-03-21
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
int32_t ModifyRushWeight(int32_t RealRushweight)
{
	int32_t ModifyRushW = 0;
	int32_t RealRushweightTemp = 0;
	
	RealRushweightTemp = gstrAdapt.s32TargetVal * (int32_t)gstrAdapt.u32SP12Range / (int32_t)100;

	if(RealRushweight > RealRushweightTemp)	 //限幅
	{
	   	RealRushweight = RealRushweightTemp;
	}	
	else if(RealRushweight < (0 - RealRushweightTemp))	 //限幅
	{
	   	RealRushweight = 0 - RealRushweightTemp;
	}	
	ModifyRushW = (RealRushweight * ((int32_t)gstrAdapt.u32SP3Scope) / (int32_t)100);

	return ModifyRushW;
}

/*
********************************************************************************
* 函数名 :  Adapt_SP12
* 功  能 :  落差修正  
* 说  明 :  none
* 入  参 :  *pAdapt : 落差修正处理指针
* 返  回 :  1 : 完成    0 : 未完成
* 设  计 :  lzs 					时  间 :  2015-7-7
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
uint32_t Adapt_SP12(_strAdapt *pAdapt)
{
	#define BUF_SIZE		11
	#define CHANNEL_SP1		1
	#define CHANNEL_SP2		2
	int32_t  s32Temp = 0;		// 临时变量
	int32_t	 s32SumTemp = 0;	// 落差值临时变量
	int32_t InterestWeight = 0;
	uint32_t i = 0;
	uint32_t channel = 0;		// 大投或中投
	uint32_t xmintime = 0;
	static int32_t buff[BUF_SIZE] = {0,};
	static int32_t buff_sort[BUF_SIZE] = {0,};

	if((0 == pAdapt->u32SP12En) || (0 == pAdapt->u32SP12Range))	// 开关关闭 / 修正次数为0 / 修正范围为0
	{
		Adapt_InitSP12Temp();
		pAdapt->bFirstIn = true;
	}
	else
	{
		if(gstrRunResult.u32MidFeedTime > 0)			// 中投有效，则修中投
		{
			pAdapt->ps32SP12 = &gsrtProductParam.s32SP2Val;
			channel = CHANNEL_SP2;
		}
		else if(gstrRunResult.u32FastFeedTime > 0)		// 中投无效，大投有效，则修大投
		{
			pAdapt->ps32SP12 = &gsrtProductParam.s32SP1Val;
			channel = CHANNEL_SP1;
		}
		
		if((pAdapt->s32SP2Bkp != (*pAdapt->ps32SP2)) && (CHANNEL_SP2 == channel))			// 修中投，且中投提前量被修改
		{
			Adapt_InitSP12Temp();
			pAdapt->bFirstIn = true;
		}
		else if((pAdapt->s32SP1Bkp != (*pAdapt->ps32SP1)) && (CHANNEL_SP1 == channel))	// 修大投，且大投提前量被修改
		{
			Adapt_InitSP12Temp();
			pAdapt->bFirstIn = true;
		}
		else
		{
			xmintime = gsrtProductParam.u32ForbidSP3Time + 100;			// 大投修正时，小投下限时间
			if(xmintime < 1100)
			{
				xmintime = 1100;
			}

			InterestWeight = RushWeightProbability(pAdapt->s32RushWt);	// 对过冲量进行概率统计处理
			
			if(InterestWeight != 0)
			{	
				if((*pAdapt->pu32SP3RealTime < xmintime)|| (*pAdapt->pu32SP3RealTime > pAdapt->u32SP12Scope))
				{

					s32Temp = InterestWeight + *pAdapt->ps32SP3;					
	
					if((*pAdapt->ps32SP12 > s32Temp) && (*pAdapt->pu32SP3RealTime > pAdapt->u32SP12Scope))	// 大于2.2秒，不能增大提前量
					{
						s32Temp = ModifyRushWeight(*pAdapt->ps32SP12 - s32Temp);
						InterestWeight = *pAdapt->ps32SP12 - s32Temp;
						if((InterestWeight > 0) && (InterestWeight < pAdapt->s32TargetVal))
						{
							*pAdapt->ps32SP12 = InterestWeight;
						}
					}
					else if((*pAdapt->ps32SP12 < s32Temp) && (*pAdapt->pu32SP3RealTime < xmintime))	// 小于1.1秒，不能减小提前量
					{									   
						s32Temp = ModifyRushWeight(s32Temp - *pAdapt->ps32SP12);
						InterestWeight = *pAdapt->ps32SP12 + s32Temp;
						if((InterestWeight > 0) && (InterestWeight < pAdapt->s32TargetVal))
						{
							*pAdapt->ps32SP12 = InterestWeight;
						}
					} 	
					
					if(*pAdapt->ps32SP12 > pAdapt->s32TargetVal - 100)		// 上限
					{
						*pAdapt->ps32SP12 = pAdapt->s32TargetVal - 100;
					}
					else if(*pAdapt->ps32SP12 < 100)						// 下限
					{
						*pAdapt->ps32SP12 = 100;
					}
					
					if(CHANNEL_SP1 == channel)					// 大投
					{
//						ProductParam_WriteIndex(GET_PRODUCT_PARAM_OFFSET(s32SP1Val));	// 保存到铁电
					}
					else if(CHANNEL_SP2 == channel)				// 中投
					{
//						ProductParam_WriteIndex(GET_PRODUCT_PARAM_OFFSET(s32SP2Val));	// 保存到铁电
					}
				}	
			}	
		}
	}
	return 1;
	#undef BUF_SIZE
	#undef CHANNEL_SP1		
	#undef CHANNEL_SP2		
}


/*
********************************************************************************
* 函数名 :  Adapt_Process
* 功  能 :  自适应处理  
* 说  明 :  none
* 入  参 :  *pAFFC : 落差修正处理指针
* 返  回 :  1 : 完成    0 : 未完成
* 设  计 :  lzs 					时  间 :  2015-7-7
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
uint32_t Adapt_Process(_strAdapt *pAdapt)
{
	if(0 != pAdapt->u32AdatpEn)	
	{
		if(true == pAdapt->bFirstIn)	// 第一包不修
		{
			pAdapt->bFirstIn = false;
		}
		else
		{
			gstrAdapt.u32SP3ForbidTime 	= gstrRunResult.u32SP3RealForbTime;	// 使用实测小投禁判时间
			Adapt_SP3(pAdapt);
			Adapt_SP12(pAdapt);
		}
	}
	else
	{
		pAdapt->bFirstIn = true;
	}
	
	return 1;
}

/***************************************The End Of This File***************************************/




/*
********************************************************************************
* 函数名 :  SampleRushWt
* 功  能 :  采集过冲重量， 放于小投加料过程中的小投截止后处理
* 说  明 :  none
* 入  参 :  *pFeed : 需要动作的加料指针
* 返  回 :  none
* 设  计 :  lzs 					时  间 :  2015-6-23
* 修  改 :  none					时  间 :  none
********************************************************************************
*/
static void SampleRushWt(void)
{
	if(false == gstrAdapt.bFirstIn)	// 启动后第一包不修
	{
		if(gstrRunResult.u32MidFeedTime > 0)	// 实际中投时间 > 0 
		{
			if(gstrRunResult.u32SlowFeedTime == gstrRunResult.u32SP3RealForbTime)//小投时间等于禁判时间
			{
				gstrAdapt.s32RushWt = 当前重量 - 中投截止点实际重量;
			}
			else if(gstrRunResult.u32SlowFeedTime == (gstrRunResult.u32SP3RealForbTime + 100))//等于禁判时间+100
			{
				gstrAdapt.s32RushWt = 当前重量 - 中投截止点实际重量;
			}
			else if(gstrRunResult.u32SlowFeedTime < (gstrRunResult.u32SP3RealForbTime + 100))//小于禁判时间+100
			{
				gstrAdapt.s32RushWt = 小投截止点重量 - 中投截止点实际重量;
			}
		}
		else if(gstrRunResult.u32FastFeedTime > 0) // 实际大投时间 > 0 
		{
			if(gstrRunResult.u32SlowFeedTime == gstrRunResult.u32SP3RealForbTime)//实际小投时间等于禁判时间
			{
				gstrAdapt.s32RushWt = 当前重量 - 大投截止点实际重量;
			}
			else if(gstrRunResult.u32SlowFeedTime == (gstrRunResult.u32SP3RealForbTime + 100))//等于禁判时间+100
			{
				gstrAdapt.s32RushWt = 当前重量 - 大投截止点实际重量;
			}
			else if(gstrRunResult.u32SlowFeedTime < (gstrRunResult.u32SP3RealForbTime + 100))//小于禁判时间+100
			{
				gstrAdapt.s32RushWt = 小投截止点重量 - 大投截止点实际重量;
			}
		}
	}
}
