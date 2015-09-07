#ifndef _MODIFY_H_
#define _MODIFY_H_

	#include <stdint.h>

	
	typedef struct HRUSHMODIFY_DATA		//������Ͷ��ǰ������
	{
		int32_t RealRushweight;   		//�ɼ��Ĺ�����
		uint32_t L_FeedTime;		  	//�ɼ�������ʱ��
		uint8_t FlowRateValue;	  		//��Ͷ�����ȼ�(1-6)
		int32_t Limit;            		//������������
		int32_t Range;             		//����������Χ
		uint8_t TimeMin;          		//���ӽ���ʱ��(��λms)
		int32_t MFeed_StopPoint;    	//��Ͷ��ǰ��
		int32_t LFeed_StopPoint;    	//СͶ��ǰ��(���ֵ)
		uint8_t flag;					//��־λ(�Ƿ��ʼ����־λ)
	}HRushModifyData;	//���ݽṹ
	
	typedef struct FALLMODIFY_DATA	 	//����СͶ��ǰ��(���ֵ)����
	{
		int32_t RealFallweight;			//�ɼ������ֵ
		uint32_t L_FeedTime;		    //�ɼ�������ʱ��
		int32_t Limit;					//������������
		int32_t Range;					//�޷�������Χ
		uint32_t TimeMin;       		//���ӽ���ʱ��(��λms)
		int32_t LFeed_StopPoint;    	//СͶ��ǰ��(���ֵ)
		uint8_t flag;					//��־λ(�Ƿ��ʼ����־λ)
	}FALLModifyData;	//���ݽṹ
	
	#define GetInitStatus   0x00    //��ʼ��
	#define GetStartStatus  0x01	//��ʼ

	int32_t ModifyFallWeight(FALLModifyData* FallWM);   	//�����������
	int32_t ModifyHRushWeight(HRushModifyData* HRushWM);	//��Ͷ��������(ģʽ������ʵ��Ϊ��Ͷ)
	

#endif
