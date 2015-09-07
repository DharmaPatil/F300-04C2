
     ����������������ͷ�ļ���
/*******************************************************************************************
*************************************ͷ�ļ���Modify.h***************************************/
#ifndef _MODIFY_H_
#define _MODIFY_H_

	#include <stdint.h>

	//�˺���ֻ������������
	
	typedef struct HRUSHMODIFY_DATA	     //������Ͷ��ǰ������
	{
		int32_t RealRushweight;   	//�ɼ����Ĺ�����
		uint32_t L_FeedTime;		//�ɼ�������ʱ��
		uint8_t FlowRateValue;	  	//��Ͷ�����ȼ�(1-6)
		int32_t Limit;            	//������������
		int32_t Range;             	//����������Χ
		uint8_t TimeMin;          	//���ӽ���ʱ��(��λms)
		int32_t MFeed_StopPoint;        //��Ͷ��ǰ��
		int32_t LFeed_StopPoint;    	//СͶ��ǰ��(���ֵ)
		uint8_t flag;			//��־λ(�Ƿ��ʼ����־λ)
	}HRushModifyData;	
	
	typedef struct FALLMODIFY_DATA	 //����СͶ��ǰ��(���ֵ)����
	{
		int32_t RealFallweight;		//�ɼ��������ֵ
		uint32_t L_FeedTime;		//�ɼ�������ʱ��
		int32_t Limit;			//������������
		int32_t Range;			//����������Χ
		uint32_t TimeMin;       	//���ӽ���ʱ��(��λms)
		int32_t LFeed_StopPoint;    	//СͶ��ǰ��(���ֵ)
		uint8_t flag;			//��־λ(�Ƿ��ʼ����־λ)
	}FALLModifyData;
	
	
	#define GetInitStatus   0x00    //��ʼ����־λ
	#define GetStartStatus  0x01	//��ʼ������־λ

	int32_t ModifyFallWeight(FALLModifyData* FallWM);   	//СͶ��ǰ��(���)��������
	int32_t ModifyHRushWeight(HRushModifyData* HRushWM);	//��Ͷ��ǰ����������
	

#endif
/***********************************ͷ�ļ���Modify.h****************************************
*******************************************************************************************/


/*******************************************************************************************
*************************************������ʼ**************************************************/
ʹ�ð�����
Logic_Pack.c
���°�װ���̣�

Step 1:StartPackFlag = true; //��ʼ������װ

Step 2:InitPackPara();       //��ʼ����װ����
	InitPackPara()
	{
	    ......
	    ModifyFunLibInit(); //��ʼ�����������������к�����
	    ......
	}

Step 3:HFeedFunction();      //��Ͷ����
	HFeedFunction()
	{   //CurrentWeightΪʵʱ����
	    RecordHWeight =  CurrentWeight;   //���´�Ͷ�źŽ���ʱ��������	
	}

Step 4:LFeedFunction();      //СͶ����
	LFeedFunction()
	{
		//HRushWeight  	   ��Ϊ��Ͷ����ֵ
		//LDisjudge_T	   Ϊ���ӽ���ʱ��
		//TargetWT	   Ŀ��ֵ
		//HWeightStopPoint ��Ͷ��ǰ��
	    ......
	    if(ReadTimerCount()<=(LDisjudge_T+100))
	    {   		
	        HRushWeight = CurrentWeight - RecordHWeight;
		(Ҳ���ԣ�RushWeight = CurrentWeight - (TargetWT - HWeightStopPoint);)
            }
	    ......
	    //СͶ����ʱ
	    RecordFallWeight = CurrentWeight;   //����СͶ�źŽ���ʱ��������
	}

Step 5:FixValueFunction();   //��ֵ����
	FixValueFunction()
	{
		//LRushWeight ��ΪСͶ����ֵ
		//FixValue_Time	   Ϊ���ӽ���ʱ��
		//TargetWT	   Ŀ��ֵ
		//LWeightStopPoint СͶ��ǰ��(���ֵ)
	    ......
	    if(ReadTimerCount()>=FixValue_Time) //��ֵʱ��һ�������������²ɼ������ֵ
	    {      
		......
		LRushWeight = CurrentWeight - RecordFallWeight;	
		(Ҳ���ԣ�LRushWeight = CurrentWeight - (TargetWT - LWeightStopPoint);)
		......
		ModifyFunLibStart(); //���Բο����ĺ�����
	    }
	    ......
	}

Step 5:DiscFunction();       //ж�Ϻ���

Step 6:NextPack 	     //���¿�ʼ��һ��


/***********************************��������****************************************************
*******************************************************************************************/


/***************************************************
��    �� :    void ModifyFunLibInit(void)
����˵�� ��   ��ʼ������Ӧ��������
*****************************************************/
void ModifyFunLibInit(void)
{
	FALLModifyData  FallWMTP;
	HRushModifyData HRushWM;
	
	s32 LFeed_StopPointTemp;
	s32 MFeed_StopPointTemp;

	//�������������ʼ��	
	FallWMTP.flag 	    = GetInitStatus;						
	LFeed_StopPointTemp = ModifyFallWeight(&FallWMTP);
	
	//��Ͷ����������ʼ��	
	HRushWM.flag	    = GetInitStatus;
	MFeed_StopPointTemp = ModifyHRushWeight(&HRushWM);

}
/***************************************************
��    �� :    void ModifyFunLib(void)
����˵�� ��   ��ʼ������Ӧ��������
*****************************************************/
void ModifyFunLib(void)
{
	FALLModifyData  FallWMTP;
	HRushModifyData HRushWM;
	
	u32 ulFreeFallExtent = 0;
	//FallWeightLimit(����0-3��ѡȡ��Χ)
	switch(FallWeightLimit)
	{
		case 0: ulFreeFallExtent = 100 ;  break;
		case 1: ulFreeFallExtent = 75 ;  break;
		case 2: ulFreeFallExtent = 50 ;   break;
		case 3: ulFreeFallExtent = 25 ;   break;
		default : ulFreeFallExtent = 50 ; break;
	}
	
	
	//�������
	FallWMTP.RealFallweight  	= LRushWeight;	
	FallWMTP.L_FeedTime 		= RunResult_Info.L_FeedTime;
	FallWMTP.Limit 			= ulFreeFallExtent;
	FallWMTP.Range 			= TargetWT*FallWeightDealRange/100;//(�����ðٷֱ�)
	FallWMTP.TimeMin 		= LDisjudge_T;
	FallWMTP.LFeed_StopPoint   	= LWeightStopPoint;
	FallWMTP.flag 			= GetStartStatus;						
	GetRDataPointer()->LFeed_StopPoint = ModifyFallWeight(&FallWMTP);
	
	//��Ͷ��ǰ������
	HRushWM.RealRushweight		= HRushWeight;   
	HRushWM.L_FeedTime		= RunResult_Info.L_FeedTime;		  
	HRushWM.FlowRateValue	  	= FlowRateValue;//(1-6)����
	HRushWM.Limit			= MRushWeightLimit; 		         
	HRushWM.Range              	= TargetWT*MRushWeightDealRange/100;//(�����ðٷֱ�)	
	HRushWM.TimeMin        		= LDisjudge_T;
	HRushWM.MFeed_StopPoint 	= HWeightStopPoint; 
	HRushWM.LFeed_StopPoint    	= LWeightStopPoint;
	HRushWM.flag			= GetStartStatus;
	GetRDataPointer()->MFeed_StopPoint = ModifyHRushWeight(&HRushWM);
	
}