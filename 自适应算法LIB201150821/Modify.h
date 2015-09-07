#ifndef _MODIFY_H_
#define _MODIFY_H_

	#include <stdint.h>

	
	typedef struct HRUSHMODIFY_DATA		//修正大投提前量参数
	{
		int32_t RealRushweight;   		//采集的过冲量
		uint32_t L_FeedTime;		  	//采集的慢加时间
		uint8_t FlowRateValue;	  		//大投修正等级(1-6)
		int32_t Limit;            		//限制修正幅度
		int32_t Range;             		//限制修正范围
		uint8_t TimeMin;          		//慢加禁判时间(单位ms)
		int32_t MFeed_StopPoint;    	//大投提前量
		int32_t LFeed_StopPoint;    	//小投提前量(落差值)
		uint8_t flag;					//标志位(是否初始化标志位)
	}HRushModifyData;	//数据结构
	
	typedef struct FALLMODIFY_DATA	 	//修正小投提前量(落差值)参数
	{
		int32_t RealFallweight;			//采集的落差值
		uint32_t L_FeedTime;		    //采集的慢加时间
		int32_t Limit;					//限制修正幅度
		int32_t Range;					//限幅修正范围
		uint32_t TimeMin;       		//慢加禁判时间(单位ms)
		int32_t LFeed_StopPoint;    	//小投提前量(落差值)
		uint8_t flag;					//标志位(是否初始化标志位)
	}FALLModifyData;	//数据结构
	
	#define GetInitStatus   0x00    //初始化
	#define GetStartStatus  0x01	//开始

	int32_t ModifyFallWeight(FALLModifyData* FallWM);   	//落差修正函数
	int32_t ModifyHRushWeight(HRushModifyData* HRushWM);	//中投修正函数(模式两料速实际为大投)
	

#endif
