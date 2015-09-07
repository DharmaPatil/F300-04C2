/*
********************************************************************************
* �ļ�˵�� 	:  �����������
* ��  �� 	:  lzs 					ʱ  �� :  2015-6-24
********************************************************************************
*/

#include "weight.h"
#include "TimeTick.h"
#include "LogicAdaptation.h"
#include "LogicConfirm.h"


/*
********************************************************************************
* ��  �� :  ��������ṹ���ʼ��
* ˵  �� :  ��Ӧ����ָ����Ҫָ��ĵ�ַ
* ��  �� :  lzs 					ʱ  �� :  2015-6-24 
* ��  �� :  none					ʱ  �� :  none
********************************************************************************
*/
_strAdapt gstrAdapt =
{
	&(gstrWeightInfo.s32GrossWt),    // ë�ذ�װʱ��ָ��ë������
	&(gsrtProductParam.s32SP1Val),   // �䷽�����Ĵ�Ͷ��ǰ��
	&(gsrtProductParam.s32SP2Val),   // ��Ͷ��ǰ�� 
	&(gsrtProductParam.s32SP3Val),   // ���ֵ 
	&(gsrtProductParam.s32SP2Val),   // ��Ͷ��ǰ�� 
	&(gstrRunResult.u32SlowFeedTime),// ʵ��СͶʱ��    
	0, 
};

/*********************************************************************************
* ������ :  Adapt_InitSP12Temp
* ��  �� :  ��ʼ����/��Ͷ�����ݴ�ֵ
* ˵  �� :  ���δ��������ػ��ϵ������������ɺ��ʼ��
* ��  �� :  none
* ��  �� :  none
* ��  �� :  lzs 					ʱ  �� :  2015-6-24
* ��  �� :  none					ʱ  �� :  none
*********************************************************************************/
void Adapt_InitSP12Temp(void)
{
    gstrAdapt.u32SP12Count = 0;
    gstrAdapt.u32SP12SubCount = 0;
}

/*********************************************************************************
* ������ :  Adapt_InitSP3Temp
* ��  �� :  ��ʼ����������ݴ�ֵ
* ˵  �� :  ���δ��������ػ��ϵ������������ɺ��ʼ��
* ��  �� :  none
* ��  �� :  none
* ��  �� :  lzs 					ʱ  �� :  2015-6-24
* ��  �� :  none					ʱ  �� :  none
*********************************************************************************/
void Adapt_InitSP3Temp(void)
{
    gstrAdapt.u32SP3Count = 0;
}

/*
********************************************************************************
* ������ :  Adapt_InitData
* ��  �� :  ��ʼ�������������
* ˵  �� :  none
* ��  �� :  none
* ��  �� :  none
* ��  �� :  lzs 					ʱ  �� :  2015-6-24
* ��  �� :  none					ʱ  �� :  none
********************************************************************************
*/
void Adapt_InitData(void)
{
	const uint32_t u32ScopeValue[] = {100, 75, 50, 25};    		// �����������
	const uint32_t u32SP12Time[] = {1800, 2000, 2200, 2500}; 	// ��/��Ͷ�����ȼ���ʱ������   

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
* ������ :  Bubble_Sort
* ��  �� :  ð������
* ˵  �� :  none
* ��  �� :  *num	: ����������
*			n		: ���ݸ���
* ��  �� :  none
* ��  �� :  lzs 					ʱ  �� :  2015-7-7
* ��  �� :  none					ʱ  �� :  none
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
* ������ :  FallWtProbabilityStatistics
* ��  �� :  ����ͳ�Ʒ����������ֵ
* ˵  �� :  ���ø���ͳ�ƴ�������������������������������
* ��  �� :  GFallTimes : �����������
*			RealFallweight : ʵ�ʵõ������
* ��  �� :  ͳ�ƴ��������ֵ
* ��  �� :  czh 					ʱ  �� :  2014-09-17
* ��  �� :  none					ʱ  �� :  none
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

		Bubble_Sort(SlFallW_Pro, SLFALLWT); //ð������
		BestFall = (SlFallW_Pro[1]+SlFallW_Pro[2]*3+SlFallW_Pro[3]*5+SlFallW_Pro[4]*3+SlFallW_Pro[5])/13;  //����ͳ�Ƴ����ֵƫ��
   }  
   else if(GFallTimes>=2)
   {
   		SlFallWRecord[GFallTimes] = RealFallweight;
   	  	for(i=0;i<=GFallTimes;i++)
		{
			SlFallW_Pro[i] = SlFallWRecord[i];
		}		
		Bubble_Sort(SlFallW_Pro, GFallTimes + 1); //ð������
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
   		if(GFallTimes==0) //����Ǹ�����������
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
* ������ :  ModifyFallWeight
* ��  �� :  �����������
* ˵  �� :  none
* ��  �� :  RealFallweight : ʵ�ʵõ������
* ��  �� :  ������(�з���)
* ��  �� :  czh 					ʱ  �� :  2014-03-21
* ��  �� :  none					ʱ  �� :  none
********************************************************************************
*/
int32_t ModifyFallWeight(int32_t RealFallweight)
{
	int32_t ModifyFall = 0;
	int32_t RealFallweightTemp = 0;
	
	RealFallweightTemp = gstrAdapt.s32TargetVal * (int32_t)gstrAdapt.u32SP3Range / (int32_t)100;
	if(RealFallweight > RealFallweightTemp)	 //�޷�
	{
	   	RealFallweight = RealFallweightTemp;
	}	
	else if(RealFallweight < (0 - RealFallweightTemp))	 //�޷�
	{
	   	RealFallweight = 0 - RealFallweightTemp;
	}	
	ModifyFall = (RealFallweight * ((int32_t)gstrAdapt.u32SP3Scope) / (int32_t)100);

	return ModifyFall;
}


/*
********************************************************************************
* ������ :  Adapt_SP3
* ��  �� :  �������  
* ˵  �� :  ��δ����Ƿ��ϣ��Ҹ��ٶ�ֵǰ��ʹ�õ�ǰ��ֵ��������������ʹ�ò��Ϻ�Ļ���ٶ�ֵ��ֵ����������
* ��  �� :  *pAdapt : �����������ָ��
* ��  �� :  1 : ���    0 : δ���
* ��  �� :  lzs 					ʱ  �� :  2015-7-7 
* ��  �� :  none					ʱ  �� :  none
********************************************************************************
*/
uint32_t Adapt_SP3(_strAdapt *pAdapt)
{
	int32_t  s32Temp = 0;		// ��ʱ����
	int32_t	 s32FallTemp = 0;	// ���ֵ��ʱ����
	
	if((0 == pAdapt->u32SP3En) || (0 == pAdapt->u32SP3Range))	// ���عر� / ��������Ϊ0 / ������ΧΪ0
	{
		Adapt_InitSP3Temp();
	}
	else
	{
		if(pAdapt->s32SP3Bkp != (*pAdapt->ps32SP3))	// ���޸�
		{
			Adapt_InitSP3Temp();
		}
		else if(*pAdapt->pu32SP3RealTime <= (pAdapt->u32SP3ForbidTime * PRODUCT_PARAM_DELAY_TIME_UNIT + 20)) // СͶʱ�� <= ����ʱ��+20ms��ֵ  ������
		{
			// ����
		}
		else
		{
			if(0 == pAdapt->s32ConfirmWt)	// δ���й�Ƿ��ϻ���ٶ�ֵǰ10��ѧϰ�Ķ�ֵ����
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
��    ����s32 RushStdError(s32 StdErrorDelta)
�������ܣ� ����������ͳ�ƺ���		
����˵���� s32  RealRushweightDelta ʵ�ʹ�����ֵ�仯��
		   	
�� �� ֵ�� s32 ������90%������ֵ
���ʱ�䣺 2014-09-09
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
		
		Bubble_Sort(SlRushRecord_pro, SLRUSHERRWT); //ð������
		
		SumErrorTemp = 0;			//�������жϵ�ֵ
		for(i=2;i<SLRUSHERRWT-2;i++)
		{
			SumErrorTemp += SlRushRecord_pro[i]; //������Чȥ����ֵ 
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
		
		Bubble_Sort(SlRushRecord_pro, gstrAdapt.u32SP12SubCount + 1); //ð������

		SumErrorTemp = 0;			//�������жϵ�ֵ 
		if(gstrAdapt.u32SP12SubCount<3)
		{			
			for(i=0;i<gstrAdapt.u32SP12SubCount+1;i++)
			{
				SumErrorTemp += SlRushRecord_pro[i]; //������Чȥ����ֵ 
			}				
			SumErrorTemp = SumErrorTemp/(gstrAdapt.u32SP12SubCount+1);
			StdError = SlRushRecord_pro[gstrAdapt.u32SP12SubCount]*11/10;
		}
		else if(gstrAdapt.u32SP12SubCount<10)
		{
			for(i=1;i<gstrAdapt.u32SP12SubCount-1;i++)
			{
				SumErrorTemp += SlRushRecord_pro[i]; //������Чȥ����ֵ 
			}
			SumErrorTemp = SumErrorTemp/(gstrAdapt.u32SP12SubCount-2);
			StdError = SlRushRecord_pro[gstrAdapt.u32SP12SubCount-1]*11/10;
		}	
		else if(gstrAdapt.u32SP12SubCount<SLRUSHERRWT)
		{
			for(i=1;i<gstrAdapt.u32SP12SubCount-2;i++)
			{
				SumErrorTemp += SlRushRecord_pro[i]; //������Чȥ����ֵ 
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
��    ����s32 RushWeightProbability(s32 RealRushweightDelta)
�������ܣ� ����������ͳ�ƺ���		
����˵���� s32  RealRushweightDelta ʵ�ʹ�����ֵ�仯��
		   	
�� �� ֵ�� s32 ������90%������ֵ
���ʱ�䣺 2014-09-09
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
		Bubble_Sort(SlRRecord_pro, SLRUSHWTP); //ð������

		StdRushW = (SlRRecord_pro[2]+SlRRecord_pro[3]*2+SlRRecord_pro[4])/4;
		AverRushWeight = StdRushW;
		MaxToMin = SlRRecord_pro[SLRUSHWTP-1] - StdRushW;
		AverMaxToMin = RushStdError(MaxToMin);	  //�ص�
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
//	StdRushW = StdRushW + GetRDataPointer()->EXFallWeight;//����Ԥ����	
		
	return StdRushW;
	
#undef SLRUSHWTP
}

/*
********************************************************************************
* ������ :  ModifyRushWeight
* ��  �� :  ��/��Ͷ��������
* ˵  �� :  none
* ��  �� :  RealFallweight : ʵ�ʵõ������
* ��  �� :  ������(�з���)
* ��  �� :  czh 					ʱ  �� :  2014-03-21
* ��  �� :  none					ʱ  �� :  none
********************************************************************************
*/
int32_t ModifyRushWeight(int32_t RealRushweight)
{
	int32_t ModifyRushW = 0;
	int32_t RealRushweightTemp = 0;
	
	RealRushweightTemp = gstrAdapt.s32TargetVal * (int32_t)gstrAdapt.u32SP12Range / (int32_t)100;

	if(RealRushweight > RealRushweightTemp)	 //�޷�
	{
	   	RealRushweight = RealRushweightTemp;
	}	
	else if(RealRushweight < (0 - RealRushweightTemp))	 //�޷�
	{
	   	RealRushweight = 0 - RealRushweightTemp;
	}	
	ModifyRushW = (RealRushweight * ((int32_t)gstrAdapt.u32SP3Scope) / (int32_t)100);

	return ModifyRushW;
}

/*
********************************************************************************
* ������ :  Adapt_SP12
* ��  �� :  �������  
* ˵  �� :  none
* ��  �� :  *pAdapt : �����������ָ��
* ��  �� :  1 : ���    0 : δ���
* ��  �� :  lzs 					ʱ  �� :  2015-7-7
* ��  �� :  none					ʱ  �� :  none
********************************************************************************
*/
uint32_t Adapt_SP12(_strAdapt *pAdapt)
{
	#define BUF_SIZE		11
	#define CHANNEL_SP1		1
	#define CHANNEL_SP2		2
	int32_t  s32Temp = 0;		// ��ʱ����
	int32_t	 s32SumTemp = 0;	// ���ֵ��ʱ����
	int32_t InterestWeight = 0;
	uint32_t i = 0;
	uint32_t channel = 0;		// ��Ͷ����Ͷ
	uint32_t xmintime = 0;
	static int32_t buff[BUF_SIZE] = {0,};
	static int32_t buff_sort[BUF_SIZE] = {0,};

	if((0 == pAdapt->u32SP12En) || (0 == pAdapt->u32SP12Range))	// ���عر� / ��������Ϊ0 / ������ΧΪ0
	{
		Adapt_InitSP12Temp();
		pAdapt->bFirstIn = true;
	}
	else
	{
		if(gstrRunResult.u32MidFeedTime > 0)			// ��Ͷ��Ч��������Ͷ
		{
			pAdapt->ps32SP12 = &gsrtProductParam.s32SP2Val;
			channel = CHANNEL_SP2;
		}
		else if(gstrRunResult.u32FastFeedTime > 0)		// ��Ͷ��Ч����Ͷ��Ч�����޴�Ͷ
		{
			pAdapt->ps32SP12 = &gsrtProductParam.s32SP1Val;
			channel = CHANNEL_SP1;
		}
		
		if((pAdapt->s32SP2Bkp != (*pAdapt->ps32SP2)) && (CHANNEL_SP2 == channel))			// ����Ͷ������Ͷ��ǰ�����޸�
		{
			Adapt_InitSP12Temp();
			pAdapt->bFirstIn = true;
		}
		else if((pAdapt->s32SP1Bkp != (*pAdapt->ps32SP1)) && (CHANNEL_SP1 == channel))	// �޴�Ͷ���Ҵ�Ͷ��ǰ�����޸�
		{
			Adapt_InitSP12Temp();
			pAdapt->bFirstIn = true;
		}
		else
		{
			xmintime = gsrtProductParam.u32ForbidSP3Time + 100;			// ��Ͷ����ʱ��СͶ����ʱ��
			if(xmintime < 1100)
			{
				xmintime = 1100;
			}

			InterestWeight = RushWeightProbability(pAdapt->s32RushWt);	// �Թ��������и���ͳ�ƴ���
			
			if(InterestWeight != 0)
			{	
				if((*pAdapt->pu32SP3RealTime < xmintime)|| (*pAdapt->pu32SP3RealTime > pAdapt->u32SP12Scope))
				{

					s32Temp = InterestWeight + *pAdapt->ps32SP3;					
	
					if((*pAdapt->ps32SP12 > s32Temp) && (*pAdapt->pu32SP3RealTime > pAdapt->u32SP12Scope))	// ����2.2�룬����������ǰ��
					{
						s32Temp = ModifyRushWeight(*pAdapt->ps32SP12 - s32Temp);
						InterestWeight = *pAdapt->ps32SP12 - s32Temp;
						if((InterestWeight > 0) && (InterestWeight < pAdapt->s32TargetVal))
						{
							*pAdapt->ps32SP12 = InterestWeight;
						}
					}
					else if((*pAdapt->ps32SP12 < s32Temp) && (*pAdapt->pu32SP3RealTime < xmintime))	// С��1.1�룬���ܼ�С��ǰ��
					{									   
						s32Temp = ModifyRushWeight(s32Temp - *pAdapt->ps32SP12);
						InterestWeight = *pAdapt->ps32SP12 + s32Temp;
						if((InterestWeight > 0) && (InterestWeight < pAdapt->s32TargetVal))
						{
							*pAdapt->ps32SP12 = InterestWeight;
						}
					} 	
					
					if(*pAdapt->ps32SP12 > pAdapt->s32TargetVal - 100)		// ����
					{
						*pAdapt->ps32SP12 = pAdapt->s32TargetVal - 100;
					}
					else if(*pAdapt->ps32SP12 < 100)						// ����
					{
						*pAdapt->ps32SP12 = 100;
					}
					
					if(CHANNEL_SP1 == channel)					// ��Ͷ
					{
//						ProductParam_WriteIndex(GET_PRODUCT_PARAM_OFFSET(s32SP1Val));	// ���浽����
					}
					else if(CHANNEL_SP2 == channel)				// ��Ͷ
					{
//						ProductParam_WriteIndex(GET_PRODUCT_PARAM_OFFSET(s32SP2Val));	// ���浽����
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
* ������ :  Adapt_Process
* ��  �� :  ����Ӧ����  
* ˵  �� :  none
* ��  �� :  *pAFFC : �����������ָ��
* ��  �� :  1 : ���    0 : δ���
* ��  �� :  lzs 					ʱ  �� :  2015-7-7
* ��  �� :  none					ʱ  �� :  none
********************************************************************************
*/
uint32_t Adapt_Process(_strAdapt *pAdapt)
{
	if(0 != pAdapt->u32AdatpEn)	
	{
		if(true == pAdapt->bFirstIn)	// ��һ������
		{
			pAdapt->bFirstIn = false;
		}
		else
		{
			gstrAdapt.u32SP3ForbidTime 	= gstrRunResult.u32SP3RealForbTime;	// ʹ��ʵ��СͶ����ʱ��
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
* ������ :  SampleRushWt
* ��  �� :  �ɼ����������� ����СͶ���Ϲ����е�СͶ��ֹ����
* ˵  �� :  none
* ��  �� :  *pFeed : ��Ҫ�����ļ���ָ��
* ��  �� :  none
* ��  �� :  lzs 					ʱ  �� :  2015-6-23
* ��  �� :  none					ʱ  �� :  none
********************************************************************************
*/
static void SampleRushWt(void)
{
	if(false == gstrAdapt.bFirstIn)	// �������һ������
	{
		if(gstrRunResult.u32MidFeedTime > 0)	// ʵ����Ͷʱ�� > 0 
		{
			if(gstrRunResult.u32SlowFeedTime == gstrRunResult.u32SP3RealForbTime)//СͶʱ����ڽ���ʱ��
			{
				gstrAdapt.s32RushWt = ��ǰ���� - ��Ͷ��ֹ��ʵ������;
			}
			else if(gstrRunResult.u32SlowFeedTime == (gstrRunResult.u32SP3RealForbTime + 100))//���ڽ���ʱ��+100
			{
				gstrAdapt.s32RushWt = ��ǰ���� - ��Ͷ��ֹ��ʵ������;
			}
			else if(gstrRunResult.u32SlowFeedTime < (gstrRunResult.u32SP3RealForbTime + 100))//С�ڽ���ʱ��+100
			{
				gstrAdapt.s32RushWt = СͶ��ֹ������ - ��Ͷ��ֹ��ʵ������;
			}
		}
		else if(gstrRunResult.u32FastFeedTime > 0) // ʵ�ʴ�Ͷʱ�� > 0 
		{
			if(gstrRunResult.u32SlowFeedTime == gstrRunResult.u32SP3RealForbTime)//ʵ��СͶʱ����ڽ���ʱ��
			{
				gstrAdapt.s32RushWt = ��ǰ���� - ��Ͷ��ֹ��ʵ������;
			}
			else if(gstrRunResult.u32SlowFeedTime == (gstrRunResult.u32SP3RealForbTime + 100))//���ڽ���ʱ��+100
			{
				gstrAdapt.s32RushWt = ��ǰ���� - ��Ͷ��ֹ��ʵ������;
			}
			else if(gstrRunResult.u32SlowFeedTime < (gstrRunResult.u32SP3RealForbTime + 100))//С�ڽ���ʱ��+100
			{
				gstrAdapt.s32RushWt = СͶ��ֹ������ - ��Ͷ��ֹ��ʵ������;
			}
		}
	}
}
