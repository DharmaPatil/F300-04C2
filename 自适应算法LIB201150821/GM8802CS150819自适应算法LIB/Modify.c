/*********************************************************************************
说明

*********************************************************************************/
#include <stdbool.h>

#include "Modify.h"
#define GetInitStatus   0x00    //初始化
#define GetStartStatus  0x01	//开始

/***************************************************
函    数：int32_t ModifyFallWeightDelta(int32_t FallweightDelta,int32_t Target,uint8_t Range,uint8_t Limit)
函数功能： 落差幅值范围修正函数
参数说明： bool Fix_FeedingTimeFlag 是否有补料修正标志位
		   uint32_t  RealFallweight 实际落差值	
返 回 值：  uint32_t 修正量
设计时间： 2014-03-21
*****************************************************/
int32_t ModifyFallWeightDelta(int32_t FallweightDelta,int32_t Range,int32_t Limit)
{
	int32_t ModifyDelta = 0;	
	if(FallweightDelta>Range)	 //限幅
	{
	   	FallweightDelta = Range;
	}	
	ModifyDelta = (FallweightDelta*Limit/100);

	return ModifyDelta;
}
/*******************************************************************************
	落差修正函数
	参数：
	int32_t RealFallweight;		//采集的落差值
	uint32_t L_FeedTime;		    //采集的慢加时间
	int32_t Limit;				//限制幅度
	uint8_t Range;				//限幅范围
	uint32_t TimeMin;       		//慢加禁判时间
	int32_t LFeed_StopPoint;    //落差值
	uint8_t flag;		
********************************************************************************/
#define SLFALLWT 7
int32_t ModifyFallWeight(FALLModifyData* FallWM)
{
	int32_t BestFall = 0;
	int32_t slFallWTemp = 0;
	uint8_t i = 0;
	uint8_t j = 0;
	static int32_t AddFallWTemp = 0;
	static int32_t SlFallWRecord[SLFALLWT];
	static int32_t SlFallW_Pro[SLFALLWT];
	static uint8_t GetFallTimes = 0;
	int32_t FallWeightDeltaTemp = 0;
	static int32_t LastFallWeight = 0;
	
	if(GetInitStatus==FallWM->flag) 
	{
		GetFallTimes = 0;   //启动前必须初始化
		return 0;
	}
	if((FallWM->TimeMin+20)<FallWM->L_FeedTime)
	{
		if(LastFallWeight!=FallWM->LFeed_StopPoint) GetFallTimes = 0;//检测是否有手动修改
		
		//************************前期采集去极值平均开始***********************************
		if(GetFallTimes>(SLFALLWT-1))
		{
			for(i=0;i<(SLFALLWT-1);i++)
			{
			   SlFallWRecord[i] = SlFallWRecord[i+1];
			   SlFallW_Pro[i] = SlFallWRecord[i];
			}
			SlFallWRecord[SLFALLWT-1] = FallWM->RealFallweight;
			SlFallW_Pro[SLFALLWT-1] = FallWM->RealFallweight;

			for(j=0;j<(SLFALLWT-1);j++)//冒泡排序
			{
				for(i=0;i<SLFALLWT-1-j;i++)
				{
					if(SlFallW_Pro[i]>SlFallW_Pro[i+1])
					{
						slFallWTemp = SlFallW_Pro[i];
						SlFallW_Pro[i]= SlFallW_Pro[i+1];
						SlFallW_Pro[i+1]=slFallWTemp;
					}
				}				
			} 
			BestFall = (SlFallW_Pro[1]+SlFallW_Pro[2]*4+SlFallW_Pro[3]*5+SlFallW_Pro[4]*3+SlFallW_Pro[5])/14;  //由于统计出落差值偏大
		}  
		else if(GetFallTimes>=2)
		{
			SlFallWRecord[GetFallTimes] = FallWM->RealFallweight;
			for(i=0;i<=GetFallTimes;i++)
			{
				SlFallW_Pro[i] = SlFallWRecord[i];
			}		
			for(j=0;j<(GetFallTimes);j++)//冒泡排序
			{
				for(i=0;i<GetFallTimes-j;i++)
				{
					if(SlFallW_Pro[i]>SlFallW_Pro[i+1])
					{
						slFallWTemp = SlFallW_Pro[i];
						SlFallW_Pro[i]= SlFallW_Pro[i+1];
						SlFallW_Pro[i+1]=slFallWTemp;
					}
				}				
			} 
			if(GetFallTimes>=4)
			{
				BestFall = (SlFallW_Pro[1]+SlFallW_Pro[2]*2+SlFallW_Pro[3])/4;
			}
			else if(GetFallTimes>=3)
			{
				BestFall = (SlFallW_Pro[1]+SlFallW_Pro[2])/2;
			}
			else if(GetFallTimes>=2)
			{
				BestFall = SlFallW_Pro[1];
			}		
			GetFallTimes++;
		}
		else
		{
			if(GetFallTimes==0) //如果是刚启动，清零
			{
			   AddFallWTemp = 0;
			}
			SlFallWRecord[GetFallTimes] = FallWM->RealFallweight;
			GetFallTimes++;		
			AddFallWTemp += FallWM->RealFallweight;
			BestFall = AddFallWTemp/GetFallTimes;		  
		}
		//************************前期采集去极值平均结束***********************************
		
		//*************************后期限幅开始********************************************
						
		if(GetFallTimes>1)
		{
			if(LastFallWeight>=BestFall)	//往负修
			{			
				FallWeightDeltaTemp = ModifyFallWeightDelta((LastFallWeight - BestFall),FallWM->Range,FallWM->Limit);
				BestFall -= FallWeightDeltaTemp;
			}
			else 
			{
				FallWeightDeltaTemp = ModifyFallWeightDelta((BestFall - LastFallWeight),FallWM->Range,FallWM->Limit);
				BestFall += FallWeightDeltaTemp;
			}
		}	
		//*************************后期限幅结束********************************************	
		LastFallWeight = BestFall;		
	}
	else
	{
		BestFall = FallWM->LFeed_StopPoint;
	}
	
	return BestFall;

}

/***************************************************
函    数：uint32_t ModifyRushWeight(uint32_t RealFallweight)
函数功能： 中投幅值范围修正函数
参数说明： bool Fix_FeedingTimeFlag 是否有补料修正标志位
		   uint32_t  RealFallweight 实际落差值	
返 回 值：  uint32_t 修正量
设计时间： 2014-03-21
*****************************************************/
int32_t ModifyRushWeightDelta(int32_t RealRushweightDelta,int32_t Range,int32_t Limit)
{
	int32_t ModifyRushW = 0;

	if(RealRushweightDelta>Range)	 //限幅
	{
	   	RealRushweightDelta = Range;
	}	
	ModifyRushW = (RealRushweightDelta*Limit/100);

	return ModifyRushW;
}

/*******************************************************************************
	大投修正函数
	参数：
	int32_t RealRushweight;   	//采集的过冲量
	uint32_t L_FeedTime;		  	//采集的慢加时间
	uint8_t FlowRateValue;	  	//中投滤波等级(1-4)
	int32_t Limit;            	//限制幅度
	uint8_t Range;             	//限制范围
	uint32_t TimeMin;          	//慢加禁判时间(单位ms)
	int32_t MFeed_StopPoint;    //中投提前量
	int32_t LFeed_StopPoint;    //落差值
	uint8_t flag;
********************************************************************************/
#define SLRUSHWTP 7
int32_t ModifyHRushWeight(HRushModifyData* HRushWM)
{
	int32_t slRushWDeltaTemp = 0;
	static int32_t SlRRecord[10];
	static int32_t SlRRecord_pro[10];
	uint8_t i = 0;
	uint8_t j = 0;
	int32_t StdRushW = 0;
	int32_t AddStdRushW = 0;
	static uint8_t GetRushTimes = 0;
	static int32_t LastRushWeight = 0;
	uint32_t JudgePointTime = 0;
	uint32_t MPointModifyTime = 0;
	int32_t ModifyMStopPointTmp1 = 0;
	int32_t ModifyMStopPointTmp2 = 0;
	static int32_t GotoModifyTimes = 0;
	
	if(GetInitStatus==HRushWM->flag) 
	{
		GetRushTimes = 0;   //启动前必须初始化
		GotoModifyTimes = 0;
		return 0;
	}
	
	//************************前期采集去极值平均开始***********************************
	if(GetRushTimes>SLRUSHWTP-1)
	{
		for(i=0;i<SLRUSHWTP-1;i++)
		{
			SlRRecord[i] = SlRRecord[i+1];
			SlRRecord_pro[i] = SlRRecord[i];
		}
		SlRRecord[SLRUSHWTP-1] = HRushWM->RealRushweight;
		SlRRecord_pro[SLRUSHWTP-1] = HRushWM->RealRushweight;
		for(j=0;j<SLRUSHWTP-1;j++)//冒泡排序
		{
			for(i=0;i<SLRUSHWTP-1-j;i++)
			{
			   	if(SlRRecord_pro[i]>SlRRecord_pro[i+1])
			   	{
			   		slRushWDeltaTemp = SlRRecord_pro[i];
					SlRRecord_pro[i] = SlRRecord_pro[i+1];
					SlRRecord_pro[i+1] = slRushWDeltaTemp;
			   	}
			}				
		} 
		StdRushW = (SlRRecord_pro[2]+SlRRecord_pro[3]*2+SlRRecord_pro[4])/4;
	}
	else
	{		
		SlRRecord[GetRushTimes] = HRushWM->RealRushweight;
		for(i=0;i<GetRushTimes+1;i++)
		{
			AddStdRushW += SlRRecord[i];
		}
		StdRushW = AddStdRushW/(GetRushTimes+1);
		StdRushW = StdRushW + HRushWM->LFeed_StopPoint*3/2;
		GetRushTimes++;
		
	}
	//************************前期采集去极值平均结束***********************************
	
	//*************************后期限幅开始********************************************

	if(StdRushW!=0)//如果为0，不统计
	{	
		if(LastRushWeight!=HRushWM->MFeed_StopPoint) GetRushTimes = 0; //如果遇到手动改变，则归零
		
		JudgePointTime = HRushWM->TimeMin+200;
		
		//if(JudgePointTime<1200)	JudgePointTime = 1200;   //时间点限制
		
		switch(HRushWM->FlowRateValue)   //中投修正等级
		{
			case 1: MPointModifyTime = HRushWM->TimeMin + 500; break;
			case 2: MPointModifyTime = HRushWM->TimeMin + 600 ; break;
			case 3: MPointModifyTime = HRushWM->TimeMin + 800 ; break;
			case 4: MPointModifyTime = HRushWM->TimeMin + 1000 ;break;
			case 5: MPointModifyTime = HRushWM->TimeMin + 1200 ;break;
			case 6: MPointModifyTime = HRushWM->TimeMin + 1400 ;break;
			default : MPointModifyTime = HRushWM->TimeMin + 800 ; break;
		}
		if(HRushWM->L_FeedTime<JudgePointTime||HRushWM->L_FeedTime>MPointModifyTime)
		{
			ModifyMStopPointTmp1 = HRushWM->MFeed_StopPoint;
			ModifyMStopPointTmp2 = StdRushW + HRushWM->LFeed_StopPoint; //注意加上小投落差值
														
			if((ModifyMStopPointTmp1>ModifyMStopPointTmp2)&&(HRushWM->L_FeedTime>MPointModifyTime))
			{
				ModifyMStopPointTmp2 = ModifyRushWeightDelta(ModifyMStopPointTmp1-ModifyMStopPointTmp2,HRushWM->Range,HRushWM->Limit);
				StdRushW = ModifyMStopPointTmp1 - ModifyMStopPointTmp2;
				GetRushTimes = 0;
			}
			else if((ModifyMStopPointTmp1<ModifyMStopPointTmp2)&&(HRushWM->L_FeedTime<JudgePointTime))
			{									   
				ModifyMStopPointTmp2 = ModifyRushWeightDelta(ModifyMStopPointTmp2-ModifyMStopPointTmp1,HRushWM->Range,HRushWM->Limit);
				StdRushW = ModifyMStopPointTmp1 + ModifyMStopPointTmp2;
				GotoModifyTimes ++;   //连续修正会重新开始
				if(GotoModifyTimes>2) 
				{
					GetRushTimes = 0;
					GotoModifyTimes = 0;
				}
				
			}
			else
			{
				GetRushTimes = 0;
			}
		}
		else
		{
			GetRushTimes = 0;
			StdRushW = HRushWM->MFeed_StopPoint;
		}
	
	}
	else
	{
		StdRushW = HRushWM->MFeed_StopPoint;
	}
		
	//*************************后期限幅结束********************************************	
	LastRushWeight = StdRushW;
	
	return StdRushW;
}

/*******************************************************************************
	高速定值函数
********************************************************************************/
