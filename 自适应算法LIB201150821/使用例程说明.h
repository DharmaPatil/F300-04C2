
     以下是修正函数的头文件：
/*******************************************************************************************
*************************************头文件：Modify.h***************************************/
#ifndef _MODIFY_H_
#define _MODIFY_H_

	#include <stdint.h>

	//此函数只做两料速修正
	
	typedef struct HRUSHMODIFY_DATA	     //修正大投提前量参数
	{
		int32_t RealRushweight;   	//采集到的过冲量
		uint32_t L_FeedTime;		//采集的慢加时间
		uint8_t FlowRateValue;	  	//大投修正等级(1-6)
		int32_t Limit;            	//限制修正幅度
		int32_t Range;             	//限制修正范围
		uint8_t TimeMin;          	//慢加禁判时间(单位ms)
		int32_t MFeed_StopPoint;        //大投提前量
		int32_t LFeed_StopPoint;    	//小投提前量(落差值)
		uint8_t flag;			//标志位(是否初始化标志位)
	}HRushModifyData;	
	
	typedef struct FALLMODIFY_DATA	 //修正小投提前量(落差值)参数
	{
		int32_t RealFallweight;		//采集到的落差值
		uint32_t L_FeedTime;		//采集的慢加时间
		int32_t Limit;			//限制修正幅度
		int32_t Range;			//限制修正范围
		uint32_t TimeMin;       	//慢加禁判时间(单位ms)
		int32_t LFeed_StopPoint;    	//小投提前量(落差值)
		uint8_t flag;			//标志位(是否初始化标志位)
	}FALLModifyData;
	
	
	#define GetInitStatus   0x00    //初始化标志位
	#define GetStartStatus  0x01	//开始修正标志位

	int32_t ModifyFallWeight(FALLModifyData* FallWM);   	//小投提前量(落差)修正函数
	int32_t ModifyHRushWeight(HRushModifyData* HRushWM);	//中投提前量修正函数
	

#endif
/***********************************头文件：Modify.h****************************************
*******************************************************************************************/


/*******************************************************************************************
*************************************案例开始**************************************************/
使用案例：
Logic_Pack.c
以下包装流程：

Step 1:StartPackFlag = true; //开始启动包装

Step 2:InitPackPara();       //初始化包装参数
	InitPackPara()
	{
	    ......
	    ModifyFunLibInit(); //初始化修正函数，下文有函数体
	    ......
	}

Step 3:HFeedFunction();      //大投供料
	HFeedFunction()
	{   //CurrentWeight为实时重量
	    RecordHWeight =  CurrentWeight;   //记下大投信号结束时的重量点	
	}

Step 4:LFeedFunction();      //小投供料
	LFeedFunction()
	{
		//HRushWeight  	   记为大投过冲值
		//LDisjudge_T	   为慢加禁盘时间
		//TargetWT	   目标值
		//HWeightStopPoint 大投提前量
	    ......
	    if(ReadTimerCount()<=(LDisjudge_T+100))
	    {   		
	        HRushWeight = CurrentWeight - RecordHWeight;
		(也可以：RushWeight = CurrentWeight - (TargetWT - HWeightStopPoint);)
            }
	    ......
	    //小投结束时
	    RecordFallWeight = CurrentWeight;   //记下小投信号结束时的重量点
	}

Step 5:FixValueFunction();   //定值函数
	FixValueFunction()
	{
		//LRushWeight 记为小投过冲值
		//FixValue_Time	   为慢加禁盘时间
		//TargetWT	   目标值
		//LWeightStopPoint 小投提前量(落差值)
	    ......
	    if(ReadTimerCount()>=FixValue_Time) //定值时间一到，结束，记下采集的落差值
	    {      
		......
		LRushWeight = CurrentWeight - RecordFallWeight;	
		(也可以：LRushWeight = CurrentWeight - (TargetWT - LWeightStopPoint);)
		......
		ModifyFunLibStart(); //可以参看下文函数体
	    }
	    ......
	}

Step 5:DiscFunction();       //卸料函数

Step 6:NextPack 	     //重新开始下一包


/***********************************案例结束****************************************************
*******************************************************************************************/


/***************************************************
函    数 :    void ModifyFunLibInit(void)
参数说明 ：   初始化自适应修正函数
*****************************************************/
void ModifyFunLibInit(void)
{
	FALLModifyData  FallWMTP;
	HRushModifyData HRushWM;
	
	s32 LFeed_StopPointTemp;
	s32 MFeed_StopPointTemp;

	//落差修正函数初始化	
	FallWMTP.flag 	    = GetInitStatus;						
	LFeed_StopPointTemp = ModifyFallWeight(&FallWMTP);
	
	//中投修正函数初始化	
	HRushWM.flag	    = GetInitStatus;
	MFeed_StopPointTemp = ModifyHRushWeight(&HRushWM);

}
/***************************************************
函    数 :    void ModifyFunLib(void)
参数说明 ：   初始化自适应修正函数
*****************************************************/
void ModifyFunLib(void)
{
	FALLModifyData  FallWMTP;
	HRushModifyData HRushWM;
	
	u32 ulFreeFallExtent = 0;
	//FallWeightLimit(可设0-3，选取范围)
	switch(FallWeightLimit)
	{
		case 0: ulFreeFallExtent = 100 ;  break;
		case 1: ulFreeFallExtent = 75 ;  break;
		case 2: ulFreeFallExtent = 50 ;   break;
		case 3: ulFreeFallExtent = 25 ;   break;
		default : ulFreeFallExtent = 50 ; break;
	}
	
	
	//落差修正
	FallWMTP.RealFallweight  	= LRushWeight;	
	FallWMTP.L_FeedTime 		= RunResult_Info.L_FeedTime;
	FallWMTP.Limit 			= ulFreeFallExtent;
	FallWMTP.Range 			= TargetWT*FallWeightDealRange/100;//(可设置百分比)
	FallWMTP.TimeMin 		= LDisjudge_T;
	FallWMTP.LFeed_StopPoint   	= LWeightStopPoint;
	FallWMTP.flag 			= GetStartStatus;						
	GetRDataPointer()->LFeed_StopPoint = ModifyFallWeight(&FallWMTP);
	
	//大投提前量修正
	HRushWM.RealRushweight		= HRushWeight;   
	HRushWM.L_FeedTime		= RunResult_Info.L_FeedTime;		  
	HRushWM.FlowRateValue	  	= FlowRateValue;//(1-6)可设
	HRushWM.Limit			= MRushWeightLimit; 		         
	HRushWM.Range              	= TargetWT*MRushWeightDealRange/100;//(可设置百分比)	
	HRushWM.TimeMin        		= LDisjudge_T;
	HRushWM.MFeed_StopPoint 	= HWeightStopPoint; 
	HRushWM.LFeed_StopPoint    	= LWeightStopPoint;
	HRushWM.flag			= GetStartStatus;
	GetRDataPointer()->MFeed_StopPoint = ModifyHRushWeight(&HRushWM);
	
}