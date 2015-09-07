/*************************************************
Project:                GM8804C-2_ARM
File name:              Timer.c
Description:            ��ʱ����غ���
Author:                 �Ժ���
Version:                1.0
Date:                   2013-12-9
Others:                 
History:
*************************************************/
#include "include.h"

#define T0OCI_SIPL      7   //��ʱ���ж����ȼ�
//#define T0OCMPT_MS      10  //��ʱ������Ƚ�ʱ��(mSΪ��λ)
//#define T0_CCR          15  //��ʱ��0 Ԥ��Ƶ�Ĵ���ֵ
#define T0OCMPT_MS      1  //��ʱ������Ƚ�ʱ��(mSΪ��λ)
#define T0_CCR          0  //��ʱ��0 Ԥ��Ƶ�Ĵ���ֵ
//��ʱ��0 ����Ƚ�A�Ĵ��� ��ʼֵ
//#define T0_OCAR         (u16)((T0OCM PT_MS)*RCCU_FrequencyValue(RCCU_PCLK2)/(TIM_PrescalerValue(TIM0)+1)/1000)
//44236800/1000=44236 ��С��� +1 = 44237 ������CNTR�Ǵ�FFFC��ʼ�������ټ�3
#define T0_OCAR         (u16)((T0OCMPT_MS)*RCCU_FrequencyValue(RCCU_PCLK2)/(TIM_PrescalerValue(TIM0)+1)/1000-2)

vu32 gu32_Sys10ms;               //ϵͳ��ʱ����һ��Ҫ����Ϊvolatile���ͣ���ֹ�������Ż�ʱ����
vu32 gu32_Sys1ms;   //1ms
vu32 gu32_Sys01ms;  //0.1ms




//TO����Ƚ�A����1ms�ж�
void Timer0_Init(void)
{
    gu32_Sys10ms = 0;
    gu32_Sys1ms = 0;

    TIM_Init(TIM0);     //��ʼ����ʱ��T0
    TIM_ClockSourceConfig(TIM0,TIM_INTERNAL);   //���ö�ʱ��ʱ��ԴΪ�ڲ�ʱ��
    TIM_PrescalerConfig(TIM0,T0_CCR);   //���÷�Ƶ����ֵ
    TIM_OCMPModeConfig(TIM0, TIM_CHANNEL_A,T0_OCAR, TIM_TIMING, TIM_LOW);
    TIM_ITConfig(TIM0,TIM_OCA_IT,ENABLE);   //����Ƚ�ģʽ
    EIC_IRQChannelPriorityConfig(T0OC1_IRQChannel , T0OCI_SIPL);    //
    EIC_IRQChannelConfig(T0OC1_IRQChannel,ENABLE);  //ʹ��T0�ж�
    TIM_CounterConfig(TIM0,TIM_START);  //��ʼ��ʱ
}


#define T1OCI_SIPL      6   //��ʱ���ж����ȼ�
#define T1OCMPT_MS      1  //��ʱ������Ƚ�ʱ��(0.1mSΪ��λ)
#define T1_CCR          0  //��ʱ��1 Ԥ��Ƶ�Ĵ���ֵ
#define T1_OCAR         (u16)((T1OCMPT_MS)*RCCU_FrequencyValue(RCCU_PCLK2)/(TIM_PrescalerValue(TIM1)+1)/10000-2)


/*********************************************************************
��������: void Timer1_Init(void)
��    ��: T1��ʼ��
˵    ��: ������Ԥ��SP2������ʱ��ʹ�� ����0.1ms���ж�
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-23
��    ��:                      ʱ    ��: 
*********************************************************************/
void Timer1_Init(void)
{
    TIM_Init(TIM1);     //��ʼ����ʱ��T0
    TIM_ClockSourceConfig(TIM1,TIM_INTERNAL);   //���ö�ʱ��ʱ��ԴΪ�ڲ�ʱ��
    TIM_PrescalerConfig(TIM1,T1_CCR);   //���÷�Ƶ����ֵ
    TIM_OCMPModeConfig(TIM1, TIM_CHANNEL_A, T1_OCAR, TIM_TIMING, TIM_LOW);
    TIM_ITConfig(TIM1,TIM_OCA_IT,ENABLE);   //����Ƚ�ģʽ
    EIC_IRQChannelPriorityConfig(T1TIMI_IRQChannel , T1OCI_SIPL);    //
}

/*********************************************************************
��������: void Sp2StopTimeStart(bool* fbp_TimeRunFlg)
��    ��: ����Ԥ��SP2��ֹ���ʱ���ʱ
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-23
��    ��:                      ʱ    ��: 
*********************************************************************/
void Sp2StopTimeStart(bool* fbp_TimeRunFlg)
{
    gu32_Sys01ms = 0;
    *fbp_TimeRunFlg = true;
    EIC_IRQChannelConfig(T1TIMI_IRQChannel,ENABLE);  //ʹ��T1�ж�
    TIM_CounterConfig(TIM1,TIM_START);  //��ʼ��ʱ
    gs_DebugRunTime.u32PredictSp2StopTime = gu32_Sp2StopTime01ms;   //����Ԥ�����Ͷ����ʱ��
//    BuzzerStart(30);
}

/*********************************************************************
��������: void Sp2StopTimeStop(bool* fbp_TimeRunFlg)
��    ��: ֹͣSP2Ԥ���ֹ��ļ�ʱ
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-24
��    ��:                      ʱ    ��: 
*********************************************************************/
void Sp2StopTimeStop(bool* fbp_TimeRunFlg)
{
    gu32_Sys01ms = 0;
    gu32_Sp2StopTime01ms = 0;
    *fbp_TimeRunFlg = false;
    TIM_CounterConfig(TIM1,TIM_STOP);  //ֹͣ��ʱ
    TIM_CounterConfig(TIM1,TIM_CLEAR);  //���������
    TIM_FlagClear(TIM1,TIM_OCFA);   //���жϱ�־
    EIC_IRQChannelConfig(T1TIMI_IRQChannel,DISABLE);  //��ʹ��T1�ж�
}

/*********************************************************************
��������: void StopSp2ByT1(void)
��    ��: Ԥ����Ͷ��ֹ���ʱ�䵽��ֹͣSP2
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-27
��    ��:                      ʱ    ��: 
*********************************************************************/
void StopSp2ByT1(void)
{
    u8 i;
    gs_DebugRunTime.s32SP2RealStopWt = gs_Rec.Target - gs_Rec.Middle;   //ʵ����Ͷ��ֹ���� Ԥ��ֹͣ��Ϊ�����õĽ�ֹ����ͬ
    //�мӼ�ʱ��
    gs_DebugRunTime.u32Sp2Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp2TimeTemp;
    //��¼��������ʱ��
    gs_DebugRunTime.u32Sp3TimeTemp = gu32_Sys1ms;
    gb_Sp2Flg = false;
    gb_Sp3Flg = true;
    gu32_Process = SMALLPROCESS;
    gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
#ifdef DEBUG_SMARTT2B
    gu32_Time1_5 = gu32_Sys1ms;                //��¼��ǰʱ����ΪT2����ʱ��
#else
    gb_TimeRunFlg = true;
    gu32_TimeBuf = gs_Rec.TimeT2b * 10;          //����ֹ�Ƚ�ʱ��
    gu32_Time1_5 = gu32_Sys10ms;                //��¼��ǰʱ����ΪT2����ʱ��
#endif

#ifdef DEBUG_WT_SEND
    gu8_DebugState2 = SP3START;
#endif
    for (i = 1; i <= IO_PORT_OUTMAX; i++)
    {
        switch (*gu32p_IoOutBuf[i])
        {
            case 4:         //��Ͷ
                switch (i)
                {
                    case 1:
                        OUT1_CLR;
                        break;
                    case 2:
                        OUT2_CLR;
                        break;
                    case 3:
                        OUT3_CLR;
                        break;
                    case 4:
                        OUT4_CLR;
                        break;
                    case 5:
                        OUT5_CLR;
                        break;
                    case 6:
                        OUT6_CLR;
                        break;
                    case 7:
                        OUT7_CLR;
                        break;
                    case 8:
                        OUT8_CLR;
                        break;
                    case 9:
                        OUT9_CLR;
                        break;
                    case 10:
                        OUT10_CLR;
                        break;
                    case 11:
                        OUT11_CLR;
                        break;
                    case 12:
                        OUT12_CLR;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
        
        
        
        
    
    
}

/*********************************************************************
��������: void T1_IRQ(void)
��    ��: 
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-23
��    ��:                      ʱ    ��: 
*********************************************************************/
void T1_IRQ(void)
{
    gu32_Sys01ms++;
    TIM_CounterConfig(TIM1,TIM_CLEAR);  //���������
    TIM_FlagClear(TIM1,TIM_OCFA);   //���жϱ�־
    if (gu32_Sys01ms >= gu32_Sp2StopTime01ms)
    {   //ʱ�䵽
        TIM_CounterConfig(TIM1,TIM_STOP);  //ֹͣ��ʱ
        TIM_CounterConfig(TIM1,TIM_CLEAR);  //���������
        EIC_IRQChannelConfig(T1TIMI_IRQChannel,DISABLE);  //��ʹ��T1�ж�
        StopSp2ByT1();
        gu32_Sys01ms = 0;
        gu32_Sp2StopTime01ms = 0;
        gb_Sp2StopTimeRunFlg = false;
    }
}


