/*********************************************************************************
˵��

*********************************************************************************/
#include <stdbool.h>

#include "Modify.h"
#define GetInitStatus   0x00    //��ʼ��
#define GetStartStatus  0x01	//��ʼ

/***************************************************
��    ����int32_t ModifyFallWeightDelta(int32_t FallweightDelta,int32_t Target,uint8_t Range,uint8_t Limit)
�������ܣ� ����ֵ��Χ��������
����˵���� bool Fix_FeedingTimeFlag �Ƿ��в���������־λ
		   uint32_t  RealFallweight ʵ�����ֵ	
�� �� ֵ��  uint32_t ������
���ʱ�䣺 2014-03-21
*****************************************************/
int32_t ModifyFallWeightDelta(int32_t FallweightDelta,int32_t Range,int32_t Limit)
{
	int32_t ModifyDelta = 0;	
	if(FallweightDelta>Range)	 //�޷�
	{
	   	FallweightDelta = Range;
	}	
	ModifyDelta = (FallweightDelta*Limit/100);

	return ModifyDelta;
}
/*******************************************************************************
	�����������
	������
	int32_t RealFallweight;		//�ɼ������ֵ
	uint32_t L_FeedTime;		    //�ɼ�������ʱ��
	int32_t Limit;				//���Ʒ���
	uint8_t Range;				//�޷���Χ
	uint32_t TimeMin;       		//���ӽ���ʱ��
	int32_t LFeed_StopPoint;    //���ֵ
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
		GetFallTimes = 0;   //����ǰ�����ʼ��
		return 0;
	}
	if((FallWM->TimeMin+20)<FallWM->L_FeedTime)
	{
		if(LastFallWeight!=FallWM->LFeed_StopPoint) GetFallTimes = 0;//����Ƿ����ֶ��޸�
		
		//************************ǰ�ڲɼ�ȥ��ֵƽ����ʼ***********************************
		if(GetFallTimes>(SLFALLWT-1))
		{
			for(i=0;i<(SLFALLWT-1);i++)
			{
			   SlFallWRecord[i] = SlFallWRecord[i+1];
			   SlFallW_Pro[i] = SlFallWRecord[i];
			}
			SlFallWRecord[SLFALLWT-1] = FallWM->RealFallweight;
			SlFallW_Pro[SLFALLWT-1] = FallWM->RealFallweight;

			for(j=0;j<(SLFALLWT-1);j++)//ð������
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
			BestFall = (SlFallW_Pro[1]+SlFallW_Pro[2]*4+SlFallW_Pro[3]*5+SlFallW_Pro[4]*3+SlFallW_Pro[5])/14;  //����ͳ�Ƴ����ֵƫ��
		}  
		else if(GetFallTimes>=2)
		{
			SlFallWRecord[GetFallTimes] = FallWM->RealFallweight;
			for(i=0;i<=GetFallTimes;i++)
			{
				SlFallW_Pro[i] = SlFallWRecord[i];
			}		
			for(j=0;j<(GetFallTimes);j++)//ð������
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
			if(GetFallTimes==0) //����Ǹ�����������
			{
			   AddFallWTemp = 0;
			}
			SlFallWRecord[GetFallTimes] = FallWM->RealFallweight;
			GetFallTimes++;		
			AddFallWTemp += FallWM->RealFallweight;
			BestFall = AddFallWTemp/GetFallTimes;		  
		}
		//************************ǰ�ڲɼ�ȥ��ֵƽ������***********************************
		
		//*************************�����޷���ʼ********************************************
						
		if(GetFallTimes>1)
		{
			if(LastFallWeight>=BestFall)	//������
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
		//*************************�����޷�����********************************************	
		LastFallWeight = BestFall;		
	}
	else
	{
		BestFall = FallWM->LFeed_StopPoint;
	}
	
	return BestFall;

}

/***************************************************
��    ����uint32_t ModifyRushWeight(uint32_t RealFallweight)
�������ܣ� ��Ͷ��ֵ��Χ��������
����˵���� bool Fix_FeedingTimeFlag �Ƿ��в���������־λ
		   uint32_t  RealFallweight ʵ�����ֵ	
�� �� ֵ��  uint32_t ������
���ʱ�䣺 2014-03-21
*****************************************************/
int32_t ModifyRushWeightDelta(int32_t RealRushweightDelta,int32_t Range,int32_t Limit)
{
	int32_t ModifyRushW = 0;

	if(RealRushweightDelta>Range)	 //�޷�
	{
	   	RealRushweightDelta = Range;
	}	
	ModifyRushW = (RealRushweightDelta*Limit/100);

	return ModifyRushW;
}

/*******************************************************************************
	��Ͷ��������
	������
	int32_t RealRushweight;   	//�ɼ��Ĺ�����
	uint32_t L_FeedTime;		  	//�ɼ�������ʱ��
	uint8_t FlowRateValue;	  	//��Ͷ�˲��ȼ�(1-4)
	int32_t Limit;            	//���Ʒ���
	uint8_t Range;             	//���Ʒ�Χ
	uint32_t TimeMin;          	//���ӽ���ʱ��(��λms)
	int32_t MFeed_StopPoint;    //��Ͷ��ǰ��
	int32_t LFeed_StopPoint;    //���ֵ
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
		GetRushTimes = 0;   //����ǰ�����ʼ��
		GotoModifyTimes = 0;
		return 0;
	}
	
	//************************ǰ�ڲɼ�ȥ��ֵƽ����ʼ***********************************
	if(GetRushTimes>SLRUSHWTP-1)
	{
		for(i=0;i<SLRUSHWTP-1;i++)
		{
			SlRRecord[i] = SlRRecord[i+1];
			SlRRecord_pro[i] = SlRRecord[i];
		}
		SlRRecord[SLRUSHWTP-1] = HRushWM->RealRushweight;
		SlRRecord_pro[SLRUSHWTP-1] = HRushWM->RealRushweight;
		for(j=0;j<SLRUSHWTP-1;j++)//ð������
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
	//************************ǰ�ڲɼ�ȥ��ֵƽ������***********************************
	
	//*************************�����޷���ʼ********************************************

	if(StdRushW!=0)//���Ϊ0����ͳ��
	{	
		if(LastRushWeight!=HRushWM->MFeed_StopPoint) GetRushTimes = 0; //��������ֶ��ı䣬�����
		
		JudgePointTime = HRushWM->TimeMin+200;
		
		//if(JudgePointTime<1200)	JudgePointTime = 1200;   //ʱ�������
		
		switch(HRushWM->FlowRateValue)   //��Ͷ�����ȼ�
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
			ModifyMStopPointTmp2 = StdRushW + HRushWM->LFeed_StopPoint; //ע�����СͶ���ֵ
														
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
				GotoModifyTimes ++;   //�������������¿�ʼ
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
		
	//*************************�����޷�����********************************************	
	LastRushWeight = StdRushW;
	
	return StdRushW;
}

/*******************************************************************************
	���ٶ�ֵ����
********************************************************************************/
