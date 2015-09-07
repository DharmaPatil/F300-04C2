/*
Description:            ����ת����غ�����AD������ת�����������ݣ����ȡ�׷�㡢����ȴ���
Author:                 �Ժ���
Date:                   2013-12-9
*/

#include "include.h"
#include "MathLogic.h"
#include "Kalman.h"

u32  gu32_AdFedValue;       //AD���ݽ����˲���õ�������
u32  gu32_NetAdFedValue;    //��ȥ�궨����AD��
bool gb_NetAdFlg;           //��ȥ�궨����AD�����λ
u32  gu32_Nquad;            //ë��4���ֶ���
bool gb_SigNquadFlg;        //ë��4���ֶ����ķ���λ

u32  gu32_FloatZero;        //�������
bool gb_SigFZeroFlg;        //����������
u32  gu32_N;                //ë�طֶ���
u32  gu32_Wt;               //ë������

bool gb_DispNetFlg;         //��ʾ���ر�־λ
bool gb_NetSigFlg;          //���ط���
u32 gu32_TareNquad;        //Ƥ��ֵ��4���ֶ���
bool gb_TareSigFlg;         //Ƥ�ط���


bool gb_StabFlg;        //�ȶ�
bool gb_MvStabFlg;      //�������ȶ�
bool gb_PowerOnZeroFlg;     //�ϵ��Զ�����
bool gb_ZeroKeyFlg;         //�������
bool gb_ForceClrZeroFlg;    //ǿ�������־λ �����������������ȳ�ʱ
bool gb_AlarmFlg;       //������־
bool gb_ZeroPosition;   //��λ��־
bool gb_NzFlg;          //������־

#ifdef DEBUG_WT_SEND
    u32 gu32_WtNotFed;          //û���˲���AD��ת���ɵ�����
    bool gb_SigWtNotFedFlg;     //û���˲���AD��ת���ɵ������ķ���
#endif

#ifdef REFRESH_AT_SP23_MAX
    bool gb_RefreshAdArrayFlg = false;
#endif

#ifdef DEBUG_SMARTT2B
    bool gb_T2bRunFlg = false;  //СͶ��ֹ�Ƚ�ʱ���Ѿ���ʼ
    u32 gu32_T2bTime;
#endif

bool gb_Sp2StopTimeRunFlg = false;  //Ԥ����Ͷֹͣʱ���Ѿ���ʼ
u32 gu32_Sp2StopTime01ms;       //Ԥ����Ͷֹͣ��ʱ�� ��λ0.1ms

s32 gs32_Wt;                //�з�����������ֵ
float gf_WtKalmanFed;       //�������˲�����������ֵ������
s32 gs32_WtKalmanFed;       //�������˲�����������ֵ�з�������


/*********************************************************************
��������: void AD_SCal0(void)
��    ��: �������AD��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-4
��    ��:                      ʱ    ��: 
*********************************************************************/
void AD_SCal0(void)
{
    if(gu32_AdFedValue >= gs_Cal.Zero)
    {
        gu32_NetAdFedValue = gu32_AdFedValue - gs_Cal.Zero;
        gb_NetAdFlg = false;
    }
    else
    {
        gu32_NetAdFedValue = gs_Cal.Zero - gu32_AdFedValue;
        gb_NetAdFlg = true;
    }
}

/*********************************************************************
��������: void TareAndWeight(void)
��    ��: ���ش����������ݼ����Լ�������λ�ȱ�־����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-18
��    ��:                      ʱ    ��: 
*********************************************************************/
void TareAndWeight(void)
{
    if (gb_DispNetFlg == true)    //��ʾ���ص���ش���
    {
        if (gb_SigWtFlg != gb_TareSigFlg)
        {
            gu32_Nquad += gu32_TareNquad;
        }
        else
        {
            if (gu32_Nquad >= gu32_TareNquad)
                gu32_Nquad -= gu32_TareNquad;
            else
            {
                gu32_Nquad = gu32_TareNquad - gu32_Nquad;
                gb_SigWtFlg = !gb_SigWtFlg;
            }
        }
    }
    gb_ZeroPosition = false;
    if (gu32_Nquad < 2)        //��λ����
    {
        gb_SigWtFlg = false;
        gb_ZeroPosition = true;
    }
    
    gu32_N = (gu32_Nquad + 2)>> 2;             //����4��������õ�һ���ֶ���
    gu32_Wt = gu32_N * gs_Cal.Div;        //���Էֶ�ֵ�õ���ǰ����
    if(gu32_Wt > gs_Cal.Cap + 9 * gs_Cal.Div)         //����10���ֶ�������������
        gb_OflFlg = true;
    
        gb_NzFlg = false;
        if (gu32_Wt <= gs_Rec.NearZero || gb_SigWtFlg == true)        //������־λ
            gb_NzFlg = true;
}


/*********************************************************************
��������: void AD_ADK(void)
��    ��: AD�����ADK�õ�4���ֶ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-4
��    ��:                      ʱ    ��: 
*********************************************************************/
void AD_ADK(void)
{
    //����2��13�η�������Լ��gs_Cal.AD�е�2��13�η���������������߼��㾫��
    //���������������룬�õ�4���ֶ���
//    gu32_Nquad = RoundOff(gu32_NetAdFedValue << 13, gs_Cal.ADK);
    gu32_Nquad = ((float)gu32_NetAdFedValue)/gs_Cal.ADK;
    gb_SigNquadFlg = gb_NetAdFlg;
}


/*********************************************************************
��������: void Clr0(u32 fu32_ClrZeroRng)
��    ��: ���㴦��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void Clr0(u32 fu32_ClrZeroRng)
{
    //�ϵ��Զ���������������ǿ������
    if (gb_PowerOnZeroFlg || gb_ZeroKeyFlg || gb_ForceClrZeroFlg)
    {
        if ((gb_OflFlg == false && gb_StabFlg == true) || gb_ForceClrZeroFlg)      //�ȶ����Ҳ�����ſ�������
        {   //��������õ��ֶ�ֵ�������㷶Χ�ıȽ�
            if ((((gu32_Nquad + 2) >> 2) * gs_Cal.Div * 100) <= (gs_Cal.Cap * fu32_ClrZeroRng))      //��ǰ�����������㷶Χ
            {
                gu32_FloatZero = gu32_Nquad;            //���渡�����Ϊ��ǰ4���ֶ�ֵ
                gb_SigFZeroFlg = gb_SigNquadFlg;         //���渡��������
                gb_PowerOnZeroFlg = false;
            }
            else                //�������㷶Χ�ڣ�����error2
            {
                ErrorDeal(2, 100);        //error2
                gb_AlarmFlg = true;
                gb_PowerOnZeroFlg = false;
            }
            BuzzerStart(30);
        }
        else if (gb_OflFlg == 0 && gb_StabFlg == false && gb_PowerOnZeroFlg == false)       //������ʱ���ȶ�
        {
            ErrorDeal(3, 100);        //error3
            gb_AlarmFlg = true;
        }
        gb_ZeroKeyFlg = false;
        gb_ForceClrZeroFlg = false;
    }
    
    gb_SigWtFlg = gb_SigNquadFlg;            //��ǰ�����ķ���
    if (gb_SigNquadFlg == gb_SigFZeroFlg)
    {
        if (gu32_Nquad >= gu32_FloatZero)
            gu32_Nquad -= gu32_FloatZero;
        else
        {
            gu32_Nquad = gu32_FloatZero - gu32_Nquad;
            gb_SigWtFlg = !gb_SigFZeroFlg;
        }
    }
    else
    {
        gu32_Nquad += gu32_FloatZero;
    }
}

/*********************************************************************
��������: void Trace0(u32 fu32_TraceZeroRng)
��    ��: ׷�㴦��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void Trace0(u32 fu32_TraceZeroRng)
{
    #define TRACE0TIME (cu8_MultiAd[gs_Setup.AdSamplingRate] * 240);
    static u32 sul_TraceZeroTimeBuf;        //׷��ʱ��
    u32 u32temp;
    
    //׷�㣬�궨״̬������ʱ��׷��
    if (fu32_TraceZeroRng != 0 && gu32_SystemState != CALIBSTATE && gu32_SystemState != RUNSTATE)
    {
        //���㵱ǰ�����븡�����֮��Ĳ�ֵ(4���ֶ���)
        if (gb_SigNquadFlg == gb_SigFZeroFlg)
            u32temp = abs(gu32_Nquad - gu32_FloatZero);
        else
            u32temp = gu32_Nquad + gu32_FloatZero;
        
        if (u32temp <= 4 * fu32_TraceZeroRng && gb_StabFlg == true)       //С��׷�㷶Χ���ȶ�����ʼ׷��
        {
            sul_TraceZeroTimeBuf--;
            if (sul_TraceZeroTimeBuf == 0)
            {
                gu32_FloatZero = gu32_Nquad;
                gb_SigFZeroFlg = gb_SigNquadFlg;
                sul_TraceZeroTimeBuf = TRACE0TIME;
            }
        }
        else
            sul_TraceZeroTimeBuf = TRACE0TIME;
    }
}



/*********************************************************************
��������: void JudgeStable(void)
��    ��: �����ͺ���������
˵    ��: 
��ڲ���: fu32_JudgeStabRng:���ȷ�Χ Ϊ0ʱĬ��Ϊһֱ�ȶ�
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void JudgeStable(u32 fu32_JudgeStabRng)
{
    u32  MOTNUM = (cu8_MultiAd[gs_Setup.AdSamplingRate] * 60);      //�������ȵ����г���
    
    static u32 WTTAB[480];              //���ȶ���
    static u32 *lu32p_NewData = &WTTAB[0];
    u16 i;
    u32 lu32_Max, lu32_Min, lu32_TempWt, lu32_TempMv;
    
#ifdef DEBUG_JUDESTABLE_TIME
    //ֻ��С�ڵ���0.5s
    if (gs_Setup.JudgeStabOverTime <= 5)
    {
        MOTNUM = MOTNUM * gs_Setup.JudgeStabOverTime / 5;
    }
#endif
    
    //��ֵ������ ��ָ���������ѭ���ƶ� ��߳���Ч��
    *lu32p_NewData = gu32_AdFedValue;
    lu32p_NewData++;
    if (lu32p_NewData > &WTTAB[ MOTNUM - 1])
        lu32p_NewData = &WTTAB[0];
    
//    for (i = 0; i < MOTNUM - 1; i++)
//        WTTAB[i] = WTTAB[i+1];
//    WTTAB[MOTNUM - 1] = gu32_AdFedValue;
    lu32_Max = WTTAB[0];
    lu32_Min = WTTAB[0];
    
    for (i = 1; i < MOTNUM; i++)
    {
        if (WTTAB[i] > lu32_Max)
            lu32_Max = WTTAB[i];
        if (WTTAB[i] < lu32_Min)
            lu32_Min = WTTAB[i];
    }
    
    lu32_TempWt = lu32_Max - lu32_Min;
    lu32_TempWt = ((float)lu32_TempWt)/gs_Cal.ADK;    //ת����4���ֶ���
    
    lu32_TempMv = lu32_Max - lu32_Min;
    lu32_TempMv = AdToMv(lu32_TempMv);        //ת���ɺ�����
    
    //�������ȴ��� С�����ȷ�Χ�ȶ� ���ȷ�ΧΪ0һֱ�ȶ�
    if (lu32_TempWt <= fu32_JudgeStabRng * 4 || fu32_JudgeStabRng == 0)
        gb_StabFlg = true;
    else
        gb_StabFlg = false;

    //mv�����ȴ���
    if (lu32_TempMv <= 1)   //������������0.001�ڵ����ȶ�
        gb_MvStabFlg = true;
    else
        gb_MvStabFlg = false;
}

/*********************************************************************
��������: void AdToWeight(u32 fu32_AdValue, u32 *fu32p_Wt, bool *fbp_SigFlg)
��    ��: AD��ת������������
˵    ��: ����ʹ��
��ڲ���: fu32_AdValue:��ת����AD�룻*fu32p_Wt��ת�����������ַ��*fbp_SigFlg��ת������������ţ�
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-04-30
��    ��:                      ʱ    ��: 
*********************************************************************/
void AdToWeight(u32 fu32_AdValue, u32 *fu32p_Wt, bool *fbp_SigFlg)
{
    
    if(fu32_AdValue >= gs_Cal.Zero)
    {
        fu32_AdValue = fu32_AdValue - gs_Cal.Zero;
        *fbp_SigFlg = false;
    }
    else
    {
        fu32_AdValue = gs_Cal.Zero - fu32_AdValue;
        *fbp_SigFlg = true;
    }
    //����2��13�η�������Լ��gs_Cal.AD�е�2��13�η���������������߼��㾫��
    //���������������룬�õ�4���ֶ���
    fu32_AdValue = ((float)fu32_AdValue)/gs_Cal.ADK;
    
    if (*fbp_SigFlg == gb_SigFZeroFlg)
    {
        if (fu32_AdValue >= gu32_FloatZero)
            fu32_AdValue -= gu32_FloatZero;
        else
        {
            fu32_AdValue = gu32_FloatZero - fu32_AdValue;
            *fbp_SigFlg = !gb_SigFZeroFlg;
        }
    }
    else
    {
        fu32_AdValue += gu32_FloatZero;
    }
    
    if (gb_DispNetFlg == true)    //��ʾ���ص���ش���
    {
        if (*fbp_SigFlg != gb_TareSigFlg)
        {
            fu32_AdValue += gu32_TareNquad;
        }
        else
        {
            if (fu32_AdValue >= gu32_TareNquad)
                fu32_AdValue -= gu32_TareNquad;
            else
            {
                fu32_AdValue = gu32_TareNquad - fu32_AdValue;
                *fbp_SigFlg = !(*fbp_SigFlg);
            }
        }
    }
    if (fu32_AdValue < 2)        //��λ����
        *fbp_SigFlg = false;
    
    fu32_AdValue = (fu32_AdValue + 2)>> 2;             //����4��������õ�һ���ֶ���
    fu32_AdValue = fu32_AdValue * gs_Cal.Div;        //���Էֶ�ֵ�õ���ǰ����
    *fu32p_Wt = fu32_AdValue;
}


/*********************************************************************
��������: void Refresh_SampleWt(u32 fu32_Wt, bool* fbp_RefreshFlg, bool fb_StartFlg)
��    ��: ��ߵ�ˢ�²������� �ɼ������ж���ߵ�
˵    ��: �����ȳ����ϴ�����3������ֵ һ�������������������ڵ�һ��������ˢ�²�������
��ڲ���: fu32_Wt=����
          fbp_RefreshFlg=ˢ�������־
          fb_StartFlg=��ʼ�ɼ�������־
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-08
��    ��:                      ʱ    ��: 
*********************************************************************/
#ifdef REFRESH_AT_SP23_MAX
void Refresh_SampleWt(u32 fu32_Wt, bool* fbp_RefreshFlg, bool fb_StartFlg)
{
    static bool sb_DoneFlg = false;
    static u32 su32_SaveWt[3] = {0, 0, 0};
    u8 i;
    
    if (fb_StartFlg)
    {   //��ʼ��������
        if (sb_DoneFlg == false)
        {
            su32_SaveWt[2] = su32_SaveWt[1];
            su32_SaveWt[1] = su32_SaveWt[0];
            su32_SaveWt[0] = fu32_Wt;
            if (su32_SaveWt[1] < su32_SaveWt[2] && su32_SaveWt[0] < su32_SaveWt[1])
            {   //������������������ǰһ������ ��λˢ�²��������־λ
                *fbp_RefreshFlg = true;
                sb_DoneFlg = true;
            }
        }
    }
    else
    {
        for (i = 0; i < sizeof(su32_SaveWt)/sizeof(su32_SaveWt[0]); i++)
        {
            su32_SaveWt[i] = 0;
            sb_DoneFlg = false;
        }
    }
}
#endif

#ifdef DEBUG_SMARTT2B
/*********************************************************************
��������: void SmartT2b_SampleWt(u32 fu32_Wt, bool* fbp_T2bRunFlg, bool fb_StartFlg, u32 fu32_MinT2b)
��    ��: �����ж�T2B��ֹ�Ƚ�ʱ��
˵    ��: �����ȳ����ϴ�����2������ֵ һ���е��ڵ�һ�����������Ѿ�������ߵ�
          Ȼ�����ж�����һ���д���ǰһ������������ֹ�Ƚ�ʱ�����
��ڲ���: fu32_Wt=����
          fbp_T2bRunFlg=���ܽ�ֹ�Ƚ�ʱ�俪ʼ��־
          fb_StartFlg=��ʼ�ɼ�������־
          fu32_MinT2b=��С����ʱ������1ms��λ
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void SmartT2b_SampleWt(u32 fu32_Wt, bool* fbp_T2bRunFlg, bool fb_StartFlg, u32 fu32_MinT2b)
{
    static bool sb_DoneFlg = false;
    static bool sb_HighestFlg = false;
    static u32 su32_SaveWt[2] = {0, 0};
    static u32 su32_Time;
    const u8 cu8_PerAdTime[] = {8, 4, 2, 1};    //120~960�ε�ÿ��ADת��ʱ�� ms
    u8 i;
    
    if (fb_StartFlg)
    {   //��ʼ��������
        if (sb_DoneFlg == false)
        {   //�ɼ�����
            su32_SaveWt[1] = su32_SaveWt[0];
            su32_SaveWt[0] = fu32_Wt;
            *fbp_T2bRunFlg = true;
            if (sb_HighestFlg == false)
            {   //��δ����ߵ�
                if (su32_SaveWt[0] < su32_SaveWt[1])
                {   //һ���е��ڵ�һ�����������Ѿ�������ߵ�
                    sb_HighestFlg = true;
                    //�����ֵ
                    gs_DebugRunTime.u32MaxWt = su32_SaveWt[1];
                    //��Ͷ��ֹ�㵽��ֵʱ�� ��Ϊ�ϴ�ADת�����Ƿ�ֵ ����Ҫ��ȥ1��ADת��ʱ��
                    gs_DebugRunTime.u32Sp2StopToMaxTime = gu32_Sys1ms - gs_DebugRunTime.u32Sp3TimeTemp - cu8_PerAdTime[gs_Setup.AdSamplingRate];
                    //�����ֵʱ�� �������ֵ����ֵʱ��
                    gs_DebugRunTime.u32MaxToMinTimeTemp = gu32_Sys1ms - cu8_PerAdTime[gs_Setup.AdSamplingRate];
                }
            }
            else
            {   //�Ѿ�������ߵ����½�
                if (su32_SaveWt[0] > su32_SaveWt[1])
                {   //һ���д��ڵ���ǰһ������������ֹ�Ƚ�ʱ�����
                    if (gu32_Sys1ms - su32_Time > fu32_MinT2b)
                    {   //���ܼ�����ʱ�䳬��fu32_MinT2b��Ϊ����
                        sb_DoneFlg = true;
                        *fbp_T2bRunFlg = false;
                        //�����ֵ
                        gs_DebugRunTime.u32MinWt = su32_SaveWt[1];
                        //��ֵ����ֵʱ�� ��Ϊ�ϴ�ADת�����ǹ�ֵ ����Ҫ��ȥ1��ADת��ʱ��
                        gs_DebugRunTime.u32MaxToMinTime = gu32_Sys1ms - gs_DebugRunTime.u32MaxToMinTimeTemp - cu8_PerAdTime[gs_Setup.AdSamplingRate];
    
                    #ifdef DEBUG_WT_SEND
                        gu8_DebugState2 = T2BEND;
                        gu32_T2bTime = gu32_Sys1ms - gu32_Time1_5;  //��¼��ֹ�Ƚ�ʱ��T2bʱ��
                    #endif
                    }
                    else
                    {   //���ܼ�����ʱ��̫�̣����ܳ����̫�� ���¼���ʱ��
                        sb_HighestFlg = false;
                    }
                }
            }
        }
    }
    else
    {
        for (i = 0; i < sizeof(su32_SaveWt)/sizeof(su32_SaveWt[0]); i++)
        {
            su32_SaveWt[i] = 0;
        }
        sb_DoneFlg = false;
        sb_HighestFlg = false;
        *fbp_T2bRunFlg = false;
        su32_Time = gu32_Sys1ms;    //��������ǰһʱ�̵�ʱ��
    }
}
#endif

/*********************************************************************
��������: LinearFitting_SP2K
��    ��: �ɼ���Ͷ��ֹ�㸽�����������Kֵ
˵    ��: 
��ڲ���: fb_StartFlg=��ʼ�ɼ���־ СͶ��ʼ
          fu32_Num=�ɼ���ĸ���
          pfs32_K=����õ���K
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-8-26
��    ��:                      ʱ    ��: 
*********************************************************************/
void LinearFitting_SP2K(bool fb_StartFlg, u32 fu32_Num, s32 *pfs32_K)
{
    static bool sb_DoneFlg = false;
    static u32 lsu32_Cnt = 0;
    
    if (fu32_Num <= 2)
    {
        *pfs32_K = 0;
        return;
    }
    
    if (fb_StartFlg)
    {   //��ʼ��������
        if (sb_DoneFlg == false)
        {   //��ʼ�ɼ� fu32_Num/2����
            lsu32_Cnt++;
            if (lsu32_Cnt >= fu32_Num / 2)
            {   //�ﵽ���õĲ�������
                *pfs32_K = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT9, 2, 0);
                sb_DoneFlg = true;
            }
        }
    }
    else
    {
        sb_DoneFlg = false;
        lsu32_Cnt = 0;
    }
}

/*********************************************************************
��������: JudgeWtOK
��    ��: �ж����������Ƿ�����������״̬
˵    ��: 
��ڲ���: 
�� �� ֵ: true=��������״̬ false=������
��    ��: �Ժ���               ʱ    ��: 2015-07-23
��    ��:                      ʱ    ��: 
*********************************************************************/
bool JudgeWtOK(s32* pfs32_Array, u8 fu8_Num)
{
    u8 i;
    
    for(i = 0; i < fu8_Num; i++)
    {
        if (pfs32_Array[i] <= pfs32_Array[i+1] || pfs32_Array[i] <= 0)
            return false;
    }
    return true;
}

/*********************************************************************
��������: Sp2WillStop
��    ��: �ж��Ƿ��������ʱ����ֹͣSP2
˵    ��: 
��ڲ���: su32_SaveWt=������������
          fu32_Sp2StopWt=��Ͷ��ֹ��
          lu8_Num=������������ĳ���
          fu32p_StopTime=ֹͣʱ��0.1msΪ��λ
�� �� ֵ: true=������ʱ�� false=��������ʱ��
��    ��: �Ժ���               ʱ    ��: 2015-07-23
��    ��:                      ʱ    ��: 
*********************************************************************/
bool Sp2WillStop(s32* pfs32_SaveWt, u32 fu32_Sp2StopWt, u8 lu8_Num, u32* fu32p_StopTime)
{
    const float cf_AdSpeed[] = {120, 240, 480, 960};
    
//    u8 i;
    s32 ls32_DataTemp = 0;
    float lf_Time = (float)10000 / cf_AdSpeed[gs_Setup.AdSamplingRate];//ÿ��ADת����೤ʱ�� ��λ0.1ms
    
    if (lu8_Num < 1)
    {
        return false;
    }
    //������� ��֪xֵԤ��yֵ�Ƿ񳬹���Ͷ��ֹ��
    ls32_DataTemp = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT10, 1, LINEARARRAY_LENGTH + 1);
    if (ls32_DataTemp >= fu32_Sp2StopWt)
    {   //Ԥ��ֵ���ڴ�Ͷ��ֹ��
        //������ʱ�� ���㶨ʱʱ��
        lf_Time = lf_Time * (float)(fu32_Sp2StopWt - pfs32_SaveWt[0]) / (float)(ls32_DataTemp - pfs32_SaveWt[0]); //���ӵ���Ͷ��ֹ������Ҫ�೤ʱ�� ��λ0.1ms
        //�����������
        *fu32p_StopTime = (u32)(lf_Time * 10);
        *fu32p_StopTime = RoundOff(*fu32p_StopTime , 10);
        return true;
    }
    else
    {
        return false;
    }
    
//    for (i = 0; i < lu8_Num - 1; i++)
//    {
//        ls32_DataTemp += (pfs32_SaveWt[i] - pfs32_SaveWt[i+1]);   //����ÿ������֮��Ĳ�ֵ��
//    }
//    ls32_DataTemp /= (lu8_Num - 1);   //�õ�ƽ����ֵ
//    
//    if (pfs32_SaveWt[0] + ls32_DataTemp >= fu32_Sp2StopWt)
//    {   //������ʱ�� ���㶨ʱʱ��
//        lf_Time = lf_Time * (float)(fu32_Sp2StopWt - pfs32_SaveWt[0]) / (float)ls32_DataTemp; //���ӵ���Ͷ��ֹ������Ҫ�೤ʱ�� ��λ0.1ms
//        //�����������
//        *fu32p_StopTime = (u32)(lf_Time * 10);
//        *fu32p_StopTime = RoundOff(*fu32p_StopTime , 10);
//        return true;
//    }
//    else
//    {
//        return false;
//    }
}

/*********************************************************************
��������: PredictSp2Stop
��    ��: ����Ԥ����Ͷ��ֹ��
˵    ��: �����ȳ����ϴ�3������ֵ�����ֵ�����ӵķ��ȣ�Ԥ����һ����ֵ���Ƿ񳬹���Ͷ��ֹ�㣬�糬��
          ��������ǰֵ����Ͷ��ֹ�����õ�ʱ�䣬���жϣ�ʱ�䵽�������Ͷ
��ڲ���: fu32_Wt=����
          fbp_TimeRunFlg=�жϼ�ʱ�Ѿ���ʼ��־
          fb_StartFlg=��ʼ�ɼ�������־
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-07-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void PredictSp2Stop(s32 fs32_Wt, u32 fu32_Sp2StopWt, bool* fbp_TimeRunFlg, u32* fu32p_StopTime, bool fb_StartFlg)
{
    #define WT_ARRAY_LENGTH 3
    
    static bool sb_DoneFlg = false;
    static s32 lss32_SaveWt[WT_ARRAY_LENGTH] = {0, 0, 0};
    u8 i;
    
    if (fb_StartFlg)
    {   //��ʼ��������
        if (sb_DoneFlg == false)
        {   //�ɼ�����
            for (i = WT_ARRAY_LENGTH-1; i > 0; i--)
            {
                lss32_SaveWt[i] = lss32_SaveWt[i-1];
            }
            //�������ݴ���[0]��
            lss32_SaveWt[0] = fs32_Wt;
            if (JudgeWtOK(lss32_SaveWt, WT_ARRAY_LENGTH))
            {   //�������ݶ���ǰһ�����ݴ󣬲��Ҷ�����0������Ϊ���Ѿ���ʼ��Ͷ���Ϲ���
                if (fs32_Wt >= fu32_Sp2StopWt)
                {   //�����Ѿ�������Ͷֵ���ж�
                    sb_DoneFlg = true;
                    gs_DebugRunTime.u32PredictSp2StopTime = 0;
                }
                else if (Sp2WillStop(lss32_SaveWt, fu32_Sp2StopWt, WT_ARRAY_LENGTH, fu32p_StopTime))
                {
                    sb_DoneFlg = true;
                    Sp2StopTimeStart(fbp_TimeRunFlg);
                }
            }
        }
        else
        {   //�Ѿ������Ͷ��������ж�
            
        }
    }
    else
    {
        memset(lss32_SaveWt, 0, WT_ARRAY_LENGTH);
        sb_DoneFlg = false;
        *fbp_TimeRunFlg = false;
    }
}


/*********************************************************************
��������: void GetWt(void)
��    ��: �õ���������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2013-12-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void GetWt(void)
{
    u32 lu32_AdFilterTemp;
    
    if (gu32_SystemState == CALIBSTATE) //�궨״̬�̶�����˲��������ȶ�
        lu32_AdFilterTemp = 9;
#ifdef DEBUG_SMARTT2B_FILTER    //���ܽ�ֹ�Ƚ�ʱ������е��˲�
    else if (gb_RunFlg && gb_T2bRunFlg)
        lu32_AdFilterTemp = gs_Setup.TraceZeroRng;
#endif
    else if (gb_RunFlg && (gu32_Process == FALLPROCESS || gu32_Process == T5PROCESS))   //ж���˲�
        lu32_AdFilterTemp = gs_Setup.AdFilterLevelDisc;
    else if (gb_RunFlg && (gu32_Process == T3PROCESS || gu32_Process == T4PROCESS))     //��ֵ�˲�
        lu32_AdFilterTemp = gs_Setup.AdFilterLevelComp;
    else    //һ��������˲�
        lu32_AdFilterTemp = gs_Setup.AdFilterLevel;
    
    gu32_AdFedValue = AD_GetFedVal(lu32_AdFilterTemp);     //�õ��˲���AD��
    JudgeStable(gs_Setup.JudgeStabRng);     //�����ͺ���������
    AD_SCal0();     //���궨���õ��������AD��
    AD_ADK();       //����ADK�õ�4���ֶ���
    Trace0(gs_Setup.TraceZeroRng);      //׷�㴦��
    Clr0(gs_Setup.ClrZeroRng);          //���㴦��
    TareAndWeight();                //�õ����������Լ�����ת������������λ��־�ȴ���
#ifdef DEBUG_WT_SEND
    AdToWeight(gu32_AdValue, &gu32_WtNotFed, &gb_SigWtNotFedFlg);
#endif

    //���ϵ��Զ����������ظ�����Ϊ�������˲�������
    gs32_Wt = (gb_SigWtFlg ? (-gu32_Wt) : gu32_Wt);     //�з�������
    if (gs_Setup.PowerOnZeroSwitch != 0)
    {   //�������˲�����
        gf_WtKalmanFed = kalman1_filter(&gs_KalmanState, gs32_Wt);
    }
    else
    {   
        gf_WtKalmanFed = gs32_Wt;
    }
    gs32_WtKalmanFed = (gf_WtKalmanFed > 0 ? (s32)(gf_WtKalmanFed + 0.5) : (s32)(gf_WtKalmanFed - 0.5));

//    //�����˲�ǰ������ϼ�������
//    SaveLinearArray(gs32_WtNotFedArray, gu32_WtNotFed, gb_SigWtNotFedFlg);
//    //�����˲�ǰ����kֵ ȡ���µļ�������
//    gs32_LinearKNotFed = LeastLinear(gs32_WtNotFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT9, 2, 0);

    //�����˲���������ϼ�������
    SaveLinearArray(gs32_WtFedArray, gs32_WtKalmanFed);
    //�����˲�������kֵ ȡ���µļ�������
//    gs32_LinearKFed = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT10, 2, 0);
    //��ȡ���Ϲ����е����k
//    GetMaxLinearK(((gu32_Process == MIDDLEPROCESS) || (gu32_Process == SMALLPROCESS)) && gb_RunFlg);

#ifdef REFRESH_AT_SP23_MAX
    Refresh_SampleWt(gu32_Wt, &gb_RefreshAdArrayFlg, (gu32_Process == SMALLPROCESS ? true : false) && gb_RunFlg);
#endif

#ifdef DEBUG_SMARTT2B
    SmartT2b_SampleWt(gs32_WtKalmanFed, &gb_T2bRunFlg, (gu32_Process == SMALLPROCESS ? true : false) && gb_RunFlg, gs_Rec.TimeT2b * 100);
#endif
    //������Ͷ��ֹ�㸽����Kֵ
    LinearFitting_SP2K((gu32_Process == SMALLPROCESS ? true : false) && gb_RunFlg, gs_Rec.TimeT9, &gs32_SP2KFed);

    if (gs_Setup.ResultHoldSwitch != 0)
    {
        if (gs_AutoStd.u8Process != AUTOSTD_SECEND)
        {   //�ڶ�����ѧϰ���̲�����Ԥ��
            //����״̬ ��Ͷ���� ������Ͷ��ֹ�Ƚ�ʱ���������ж�
            PredictSp2Stop(gs32_WtKalmanFed, gs_Rec.Target - gs_Rec.Middle, &gb_Sp2StopTimeRunFlg, &gu32_Sp2StopTime01ms, (gu32_Process == MIDDLEPROCESS ? true : false) && gb_RunFlg && (gb_TimeRunFlg == false));
        }
    }
}

/*********************************************************************
��������: void GetMaxLinearK(bool fb_StartFlg)
��    ��: ���������������Kֵ ֻ�������0��Kֵ
˵    ��: 
��ڲ���: fb_StartFlg=��ʼ��¼���ֵ
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-06
��    ��:                      ʱ    ��: 
*********************************************************************/
void GetMaxLinearK(bool fb_StartFlg)
{
    static bool sb_DoneFlg = false;
    static bool sb_InflexionFlg = false;    //�Ѿ����յ��־
    static s32 ss32_KFed;
    static u32 su32_Cnt;
    
    if (fb_StartFlg)
    {
        if (gs32_LinearKFed > ss32_KFed)
        {   //ȡ�˲������kֵ
            ss32_KFed = gs32_LinearKFed;
        }
        else
        {   
            if (sb_InflexionFlg == false)
            {   //��û�е���յ�
                //kֵ��С����Ϊ�ǵ���յ�
                //������ ����1kg ��ֹ���Źرյ��𶯵�������
                if (gs32_WtKalmanFed > 1000)
                {   //�µ����kֵС���ʾ�˴β����Ѿ���ʼ��С���ƣ��ϴβ�����������kֵ�յ�
                    //�ڹյ㸽��ȡ��F3.11���õĸ������ݼ���kֵ
                    if (gs_Rec.TimeT11 >= 2)
                    {   //ȡ�����Ҫ���ڵ���2
                        su32_Cnt = gs_Rec.TimeT11 / 2 - 1;  //��ȡ��Ҫ��ȡ�ĵ���� ��Ϊ��ʱ�̵���Ѿ��ǹյ�+1������Ҫ-1
                        sb_InflexionFlg = true;
                    }
                    #ifdef DEBUG_WT_SEND
                        gu8_DebugState2 = MAXK;
                    #endif
                }
            }
        }
        gs_DebugRunTime.s32Sp2MaxKFed = ss32_KFed;
        if (sb_InflexionFlg && sb_DoneFlg == false)
        {   //�Ѿ����յ�
            if ((su32_Cnt--) <= 0)
            {   //�Ѿ�ȡ��������Ҫ��������� ����յ㸽��gs_Rec.TimeT11������k
                gs_DebugRunTime.s32MaxKFedReLinear = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT11, 2, 0);
                //�������bֵ �Կ�ʼ��ϵĵ�Ϊx��0�� ����bֵ����x=LINEARARRAY_LENGTH - gs_Rec.TimeT11ʱ��Yֵ
                gs_DebugRunTime.s32MaxBFedReLinear = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_Rec.TimeT11, 1, LINEARARRAY_LENGTH - gs_Rec.TimeT11);
                sb_DoneFlg = true;
            }
        }
    }
    else
    {
        ss32_KFed = 0;
        sb_InflexionFlg = false;
        sb_DoneFlg = false;
        su32_Cnt = 0;
    }
}
