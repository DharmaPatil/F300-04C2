/*
Description:            ����״̬����ع��ܺ���
Author:                 �Ժ���
Date:                   2013-12-9
*/
#include "include.h"
#include "MathLogic.h"
#include "Adaptation.h"


u32 gu32_Process;                 //����ʱ�ļ���״̬

bool gb_TimeRunFlg;              //��ʱ�Ѿ�������־λ
u32 gu32_TimeBuf;                //��ʱ��ʱ�����棬�������õ�ʱ��ֵ
u32 gu32_Time1_5;                //T1~T5�ļ�ʱ�����棬��������ʱ�̵�ʱ��

bool gb_FlapBagOutFlg;             //�Ĵ������Ч��־
bool gb_FlapWaitFlg;               //�Ĵ�֮ǰ�ĵȴ���־
bool gb_FlapStartFlg;              //�Ĵ��Ѿ���ʼ��־
bool gb_FlapAllHighFlg;            //�͵�ƽʱ��Ϊ0,�Ĵ��㶨��Ч��־λ
u32 gu32_TimeFlapBuf;               //�Ĵ���ʱ��Ĵ���
u32 gu32_Time8_9;                   //t8��t10����ʱ�Ĵ���
u32 gu32_FlapBagNumCnt;             //�Ĵ�������
bool gb_Sp1OverFlg;              //��Ͷ������Ч��־
bool gb_Sp2OverFlg;              //��Ͷ������Ч��־
bool gb_AddFlapBagFlg;                //�������Ĵ���־λ
bool gb_ExtFlapBagFlg;                //�����Ĵ������־λ
bool gb_AddFlapBagEndFlg;             //�������Ĵ��Ѿ���ɻ�����������־λ
u32 gu32_DiscShakeTimeBuf;   //��ж�����ʱ��Ĵ���
bool gb_TareGetedFlg;       //Ƥ���Ѿ���ȡ��־λ
u32 gu32_OUDelayTimeBuf;         //��Ƿ����ͣ�ر�ʱ�ĳ�Ƿ�����ʱ�Ĵ���
u32 gu32_DiscShakeCnt;   //ж��������
bool gb_Error9Flg;                    //ж��δ�ɾ�����
bool gb_SumFlg;                 //�ۼ�
u32 gu32_SumTemp;           //�ۼƵĻ���ֵ
bool gb_DiscWtGetedFlg;       //��ֵ�����Ѿ���ȡ
u32 gu32_DiscValue;     //����ж��ֵ
u32 gu32_HoldWt;        //��ֵ��������
u32 gu32_WeighTimeCnt;      //��������
u32 gu32_TargetTemp;        //Ŀ��ֵ�Ļ���
bool gb_LiftBagStartFlg;    //�������
bool gb_PushBagStartFlg;    //�����ƴ�
bool gb_ConveyorStartFlg;   //�������ͻ�
bool NO2Wait(u32 fu32_No2PackOffTwoSwitch);

STATISTIC_DATA gs_Statistic; //ͳ������
PREDICTSP3 gs_PredictSp3;
PREDICTSP3 gs_PredictSp3Temp;

DEBUG_RUN_TIME gs_DebugRunTime;

#ifdef DEBUG_MONITORTIME_PRINT
    DEBUG_RUN_TIME gs_DebugRunTimeTemp; //��ӡ��ʱ��ʹ����Ϊgs_DebugRunTime�Ļ���
    u32 gu32_DebugTarget;
    u32 gu32_DebugLarge;
    u32 gu32_DebugMiddle;
    u32 gu32_DebugSmall;
#endif

struct_autostudy_t gs_AutoStd = {0,0,0,0,0,0,0,0,0,0,0};

bool JudgeAutoStudy(void);
void AutoStudy_SP2Start(void);


#ifdef DEBUG_MONITORTIME_PRINT
/*********************************************************************
��������: void RefreshDebugRec(void)
��    ��: ˢ�µ����õĴ���СͶĿ��ֵ����
˵    ��: ÿ�μ��Ͽ�ʼʱˢ��һ�Σ�����ֱ����ϵͳ���䷽������������ʱ�޸��䷽ʱ�п���
          ���ͳ��������ݺ͵��ε��Խ���ǶԲ��ϵ�
��ڲ���: 
�� �� ֵ: 
��    ��: chx                  ʱ    ��: 2014-05-27
��    ��:                      ʱ    ��: 
*********************************************************************/
void RefreshDebugRec(void)
{
    gu32_DebugTarget = gs_Rec.Target;
    gu32_DebugLarge = gs_Rec.Large;
    gu32_DebugMiddle = gs_Rec.Middle;
    gu32_DebugSmall = gs_Rec.Small;
}

#endif
/*********************************************************************
��������: void UpdateFastSetRec(void)
��    ��: ���¿������õ��䷽
˵    ��: 
��ڲ���: fb_UpTotalFlg������Ŀ��ֵ fb_UpSingleFlg���µ���Ŀ��ֵ AorB����A��B�Ĵ���СͶ
�� �� ֵ: s
��    ��: �Ժ���               ʱ    ��: 2013-7-23
��    ��:                      ʱ    ��: 
*********************************************************************/
void UpdateFastSetRec(void)
{
    u8 i;
    
    if (gu8_FastSetSigTemp != 0)
    {
        for (i = 0; i <= 5; i++)
        {
            if (gu8_FastSetSigTemp & (0x01 << i))
            {
                ulParameter.ul = gu32_FastSetTemp[i];
                FRAM_WriteDW(GetRecipePort(i + 3), ulParameter.ul);
            }
        }
        gu8_FastSetSigTemp = 0;
    }
}

/*********************************************************************
��������: void Over_Under(u32 ful_Target, u32 ful_Result)
��    ��: ��Ƿ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void Over_Under(u32 ful_Target, u32 ful_Result)
{
    if (gs_Rec.OUSwitch != 0)       //��Ƿ���⿪�ش򿪽��г�Ƿ����
    {
        if (gb_OflFlg)
            gb_OverFlg = true;       //�����Ϊ����
        else if (ful_Result >= ful_Target + gs_Rec.OverValue)
        {
            gb_OverFlg = true;       //��ֵ��������Ŀ��ֵ+����ֵ �򳬲�
            gb_UnderFlg = false;
        }
        else if ((ful_Result <= ful_Target - gs_Rec.UnderValue) && (ful_Target > gs_Rec.UnderValue))
        {
            gb_UnderFlg = true;      //��ֵ����С��Ŀ��ֵ-Ƿ��ֵ ��Ƿ��
            gb_OverFlg = false;
        }
    }
    if (gb_OverFlg || gb_UnderFlg)       //��Ƿ��
    {
        if (gs_Rec.OUStopSwitch == 0)       //��Ƿ����ͣ���عر�
        {
            BuzzerStart(100);   //��������
//            if (gb_BatchOverFlg == 0)         //�������û�ڱ������屨�� for test
//                gb_AlarmFlg = false;

            //�峬Ƿ�� �ó�Ƿ����ͣ�ر�ʱ�ĳ�Ƿ���־
            gb_OUOverFlg = gb_OverFlg;
            gb_OUUnderFlg = gb_UnderFlg;
            gb_OverFlg = false;
            gb_UnderFlg = false;
            gu32_OUDelayTimeBuf = gu32_Sys10ms;        //���汨��ʱ��
        }
        else
        {
            ErrorDeal(8, 0);        //error8
            gb_AlarmFlg = true;
        }
    }
}

/*********************************************************************
��������: void StatisticClrStruct(void)
��    ��: ���ͳ�ƽṹ��
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void StatisticClrStruct(void)
{
    u32* lu32p_Start = &gs_Statistic.u32StartTime;
    u8 i;
    
    for (i = 0; i < sizeof(gs_Statistic) / sizeof(gs_Statistic.u32StartTime); i++)
    {
        *(lu32p_Start+i) = 0;
    }
}

/*********************************************************************
��������: void StatisticSaveStartTime(u32 fu32_Time)
��    ��: ��������ʱ��
˵    ��: 
��ڲ���: fu32_Time=ϵͳʱ��10ms
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void StatisticSaveStartTime(u32 fu32_Time)
{
    gs_Statistic.u32StartTime = fu32_Time;
}

/*********************************************************************
��������: void StatisticDeal(u32 fu32_Result)
��    ��: ͳ�����ݼ��㴦��
˵    ��: 
��ڲ���: fu32_Result=���ӽ��
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-17
��    ��:                      ʱ    ��: 
*********************************************************************/
void StatisticDeal(u32 fu32_Result)
{
    #define TARGET  gs_Rec.Target
    #define OVER    gs_Rec.OverValue
    #define UNDER   gs_Rec.UnderValue
    #define SUMNUM  gu32_DebugSumNum
    #define SYSTIME gu32_Sys10ms
    
    //�ܹ���ʱ
    gs_Statistic.u32TotalTime = SYSTIME - gs_Statistic.u32StartTime;
    //��ɰ���
    gs_Statistic.u32SumNum = SUMNUM;
    //�������
    gs_Statistic.u32SumWt += fu32_Result;
    //ÿСʱ����
    gs_Statistic.u32NumAHour = ((double)360000) / gs_Statistic.u32TotalTime * gs_Statistic.u32SumNum + 0.5;
    //ÿСʱ����
    gs_Statistic.u32WtAHour = gs_Statistic.u32NumAHour * TARGET;
    //ÿ���Ӱ��� 1С����
    gs_Statistic.u32NumAMin = ((double)60000) / gs_Statistic.u32TotalTime * gs_Statistic.u32SumNum + 0.5;
    //ƽ��ÿ������
    gs_Statistic.ResultAverage = gs_Statistic.u32SumWt / gs_Statistic.u32SumNum;
    //��߳���ֵ ��Ӧ����
    if ((fu32_Result > TARGET + OVER) && (fu32_Result > gs_Statistic.u32MostOverResult))
    {
        gs_Statistic.u32MostOverResult = fu32_Result;
        gs_Statistic.u32MostOverNum = SUMNUM;
    }
    //���Ƿ��ֵ ��Ӧ����
    if ((fu32_Result < TARGET - UNDER) && (fu32_Result < gs_Statistic.u32MostUnderResult || gs_Statistic.u32MostUnderResult == 0))
    {
        gs_Statistic.u32MostUnderResult = fu32_Result;
        gs_Statistic.u32MostUnderNum = SUMNUM;
    }
    //���ֵ
    if (fu32_Result > gs_Statistic.u32TheMaxValue)
    {
        gs_Statistic.u32TheMaxValue = fu32_Result;
    }
    //���ֵ
    if (fu32_Result < gs_Statistic.u32TheMinValue || gs_Statistic.u32MostUnderResult == 0)
    {
        gs_Statistic.u32TheMinValue = fu32_Result;
    }
    //����
    if (gs_Statistic.u32TheMaxValue != 0 && gs_Statistic.u32TheMinValue != 0 && gs_Statistic.u32TheMaxValue > gs_Statistic.u32TheMinValue)
    {
        gs_Statistic.u32ErrorBand = gs_Statistic.u32TheMaxValue - gs_Statistic.u32TheMinValue;
    }
    //�����ֲ���� ����
    if (fu32_Result < TARGET - UNDER)
        gs_Statistic.u32Dst_Under_Num++;
    else if (fu32_Result >= TARGET - UNDER && fu32_Result < TARGET - 2 * ONESTEP)
        gs_Statistic.u32Dst_U_2_Num++;
    else if (fu32_Result >= TARGET - 2 * ONESTEP && fu32_Result < TARGET - ONESTEP)
        gs_Statistic.u32Dst_2_1_Num++;
    else if (fu32_Result >= TARGET - ONESTEP && fu32_Result < TARGET)
        gs_Statistic.u32Dst_1_0_Num++;
    else if (fu32_Result >= TARGET && fu32_Result <= TARGET + ONESTEP)
        gs_Statistic.u32Dst_0_1_Num++;
    else if (fu32_Result > TARGET + ONESTEP && fu32_Result <= TARGET + 2 * ONESTEP)
        gs_Statistic.u32Dst_1_2_Num++;
    else if (fu32_Result > TARGET + 2 * ONESTEP && fu32_Result <= TARGET + OVER)
        gs_Statistic.u32Dst_2_O_Num++;
    else if (fu32_Result > TARGET + OVER)
        gs_Statistic.u32Dst_Over_Num++;
    //�ٷֱ�    1λС����
    gs_Statistic.u32Dst_Under_Per = ((float)gs_Statistic.u32Dst_Under_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_U_2_Per = ((float)gs_Statistic.u32Dst_U_2_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_2_1_Per = ((float)gs_Statistic.u32Dst_2_1_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_1_0_Per = ((float)gs_Statistic.u32Dst_1_0_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_0_1_Per = ((float)gs_Statistic.u32Dst_0_1_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_1_2_Per = ((float)gs_Statistic.u32Dst_1_2_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_2_O_Per = ((float)gs_Statistic.u32Dst_2_O_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    gs_Statistic.u32Dst_Over_Per = ((float)gs_Statistic.u32Dst_Over_Num) / gs_Statistic.u32SumNum * 1000 + 0.5;
    
    
    #undef TARGET
    #undef OVER
    #undef UNDER
    #undef SUMNUM
    #undef SYSTIME
}

/*********************************************************************
��������: void OneTareOver(void)
��    ��: һ�ӽ���������һ�ӿ�ʼ֮ǰ�Ĵ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void OneTareOver(void)
{
    if (gu32_BatchTime != 0)
    {   //����������һ�����βż�һ��
        if (gu32_WeighTimeCnt <= 1 || (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B))
        {
            gu32_BatchTimeCnt--;
            if (gb_BatchOverFlg || gu32_BatchTimeCnt == 0 || (gu32_BatchTimeCnt & 0x80000000) != 0)
            {
                ErrorDeal(1, 0);        //error1
                gb_AlarmFlg = true;
                gb_BatchOverFlg = true;
                gu32_BatchTimeCnt = 0;
            }
            FRAM_WriteDW(FMAddr_BatchTimeCnt, gu32_BatchTimeCnt);
        }
    }
    if (gu32_WeighTimeCnt <= 1 || (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B))
        UpdateFastSetRec(); //���¿���������ֵ
    
    if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
    {
        if (gu32_SumTemp != 0 && gu32_WeighTimeCnt <= 1)
        {   
            //ModbusͨѶ��ʹ�ã��ϳӽ��
            gu32_MbLastResult = gu32_SumTemp;
            //����������һ�Ӳ��ۼ�
        #ifndef DONOT_SAVE_SUM
            Save_SumData(gu32_SumTemp, 1);            //�����ۼ�����
        #endif
        #ifndef DEBUG_MONITORTIME_PRINT
            //�Զ���ӡ
            PrintStart(PRINT_AUTO, gu32_SumTemp, 0);
        #endif
            gu32_SumTemp = 0;
        }
        //������ϴ����������¸�ֵ
        if ((--gu32_WeighTimeCnt) == 0 || (gu32_WeighTimeCnt & 0x80000000) != 0)
            gu32_WeighTimeCnt = gs_Rec.CombinedNum;
        if (gu32_WeighTimeCnt == 1 && gs_Rec.CombinedNum > 1)
        {   //������ϴ�������1,������һ������ϵ����һ�ӣ�����ʣ���Ŀ��ֵ
            if (gs_Rec.Target * gs_Rec.CombinedNum > gu32_SumTemp)  //�ۼ�û�г���Ŀ��ֵ
                gu32_TargetTemp = gs_Rec.Target * gs_Rec.CombinedNum - gu32_SumTemp;
            else    //�ۼ��Ѿ�����ֹͣ
                RunStopInit(false);
        }
        else    //���������������ϴ���С�ڵ���1�������û�е����һ�ӣ�
            gu32_TargetTemp = gs_Rec.Target;
    }
    else    //�޶�ģʽ����Ŀ��ֵ v1.01
        gu32_TargetTemp = gs_Rec.Target;
    if (gb_EndFlg)      //����������Ч
    {
        gu32_Process = 0;
        RunStopInit(false);
    }
    gb_AddFlapBagEndFlg = false;
    if (gb_AlarmFlg == false && gu32_Process != 0)
    {
    #ifdef DEBUG_WT_SEND
        gu8_DebugState2 = T1START;
    #endif
        gu32_Process = T1PROCESS;
        gu32_TimeBuf = gs_Rec.TimeT1 * 10;
        gu32_Time1_5 = gu32_Sys10ms;
        gb_TimeRunFlg = false;
        //һ�μ���ʱ��
        gs_DebugRunTime.u32OneTareTime = gu32_Sys1ms - gs_DebugRunTime.u32RunStartTimeTemp;
        gs_DebugRunTime.u32RunStartTimeTemp = gu32_Sys1ms; //��¼����ʱ��
        //��ʱ��
        gs_DebugRunTime.u32TotalTime = gu32_Sys1ms - gs_DebugRunTime.u32FirstRunTimeTemp;
        //����+1
        gs_DebugRunTime.u32SumCnt++;
    #ifdef DEBUG_MONITORTIME_PRINT    //���͵����������
        gs_DebugRunTimeTemp = gs_DebugRunTime;
        gs_PredictSp3Temp = gs_PredictSp3;
        gu32_DebugSumNumTemp = gu32_DebugSumNum;
        
        PrintStart(PRINT_AUTO, gu32_HoldWt, 0);
    #endif
        gs_DebugRunTime.bGetRunCmpTimeFlg = false;
        gs_DebugRunTime.s32MaxKFedReLinear = 0;
        if(gs_DebugRunTime.u32TotalTime > 60000)
        {   //����1�������¼�ʱ����
            gs_DebugRunTime.u32TotalTime = 0;
            gs_DebugRunTime.u32FirstRunTimeTemp = gu32_Sys1ms;
            gs_DebugRunTime.u32SumCnt = 0;
        }
        PredictSp3_Restart();
        StatisticDeal(gu32_HoldWt);
        gu32_DebugSumNum++;
    }
    else
        gu32_Process = 0;
}

/*********************************************************************
��������: void Deal_WeighTime_Sum(void)
��    ��: �д���ɺ�ĳ����������ۼƵ���ش���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void Deal_WeighTime_Sum(void)
{
    if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
    {
        if (gb_SigWtFlg == false)   //��������ʱ���ۼ�
        {   //�п��ܵ�����ϴ�����Ϊ0�����Դ˴���+=
            gu32_SumTemp += gu32_Wt;  //�����ۼ�������ж����ɺ����ۼ�
            gu32_DiscValue = gu32_Wt;  //��ֵ����
        }
        else
            gu32_SumTemp += 0;
    }
    else    //�޶��Ӻ�ɢ�ϳӵ��ۼ�
    {
        if (gb_SigWtFlg == false)   //��������ʱ���ۼ�
        {
            gu32_SumTemp = gu32_Wt;  //�����ۼ�������ж����ɺ����ۼ�
            gu32_DiscValue = gu32_Wt;  //��ֵ����
        }
        else
            gu32_SumTemp = 0;
    }
}

/*********************************************************************
��������: bool CorrectFall(void)
��    ��: �������
˵    ��: 
��ڲ���: 
�� �� ֵ: 0���������ɻ���Ҫ�������  1�������δ���
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
bool CorrectFall(void)
{
    bool  lb_Quit = false;
    bool  lb_SigFallWt;
    u32 lul_CorrectFallRng2Tab[] = {1, 1, 2, 4};
    u32 lul_Temp;
    
    if (gs_Rec.CorrectFallSwitch)          //����������ش�
    {
        if (gb_StabFlg || StabOver(false))
        {   //�ȶ������ȳ�ʱʱ�䵽
            StabOver(true); //��λ���ȳ�ʱ����
            if (gb_OflFlg == false)
            {
                lul_Temp = abs(gu32_TargetTemp - gu32_Wt) * 1000;
                // |Ŀ��ֵ-��ǰֵ|>Ŀ��ֵ*���������Χ �������������Χ������
                if (lul_Temp > gu32_TargetTemp * gs_Rec.CorrectFallRng1)
                    return false;
                    
                //����޼���������
                gu32_CorrectFallWt += (gu32_TargetTemp - gu32_Wt);
                if (gs_Rec.CorrectFallTime != 0)       //�������������Ϊ������������
                {
                    if (--gu32_CorrectFallTimeCnt == 0 || gu32_CorrectFallTimeCnt & 0x80000000)        //�������������
                    {
                        lb_SigFallWt = false;
                        if (gu32_CorrectFallWt & 0x80000000)           //����ۼ�ֵΪ��
                        {
                            gu32_CorrectFallWt = -gu32_CorrectFallWt;
                            lb_SigFallWt = true;
                        }
                        gu32_CorrectFallWt = gu32_CorrectFallWt / (lul_CorrectFallRng2Tab[gs_Rec.CorrectFallRng2] * gs_Rec.CorrectFallTime);
                        if (lb_SigFallWt)
                            gs_Rec.Small += gu32_CorrectFallWt;     //�޸����ֵ
                        else
                            gs_Rec.Small -= gu32_CorrectFallWt;     //�޸����ֵ
                        //������
                        if (gs_Rec.Small & 0x80000000)
                        {
                            gs_Rec.Small = 0;
                        }
                        else if ((gb_Sp2OverFlg == false) && (gs_Rec.Small > gs_Rec.Middle))
                        {
                            gs_Rec.Small = gs_Rec.Middle;     //���ֵ������Ͷֵ����
                        }
                        else if ((gb_Sp1OverFlg == false) && (gs_Rec.Small > gs_Rec.Large))
                        {
                            gs_Rec.Small = gs_Rec.Large;      //��Ͷ������Ч�����ֵ���ڴ�Ͷֵ����
                        }
                        FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);  //����������ʵʱ����
                        //�����������ۼ�ֵ�������������������λ��׼����һ���������
                        gu32_CorrectFallWt = 0;
                        gu32_CorrectFallTimeCnt = gs_Rec.CorrectFallTime;
                    }
                }
                else
                {
                    gu32_CorrectFallWt = 0;
                    gu32_CorrectFallTimeCnt = gs_Rec.CorrectFallTime;
                }
            }
        }
        else    //���ȶ�ʱ��ȴ�
            return true;
    }
    return lb_Quit;
}

/*********************************************************************
��������: void GetTare(u32 fu32_Tare)
��    ��: ȥƤ����
˵    ��: ����Ƥ�ؼ���ȹ��ܴ���Ƥ�ش��ڵ�ǰ����������ȥƤ����������ȥƤ
��ڲ���: fu32_Tare:Ƥ��ֵ��4���ֶ���
          fb_TareGetFlg:�Ƿ����Ƥ�ؼ���Ĵ���
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void GetTare(u32 fu32_Tare, bool fb_SigFlg, bool fb_TareGetFlg)
{
    if (gb_TareGetedFlg == false || fb_TareGetFlg == false)
    {   //���֮ǰû�б����Ƥ��ֵ����ô����ǰ��������Ƥ��ȥƤ
        //fb_TareGetFlg=false��ʾ������Ƥ�ؼ���Ĵ���ֱ�Ӱ���ǰ����ȥƤ
        gu32_TareNquad = fu32_Tare;    //�洢Ƥ��ֵ,4���ֶ���
        gb_TareSigFlg = fb_SigFlg; //�洢Ƥ��ֵ��������
        gb_TareGetedFlg = true;
    }
    else
    {   //֮ǰ�б������Ƥ��
        if (fb_SigFlg == true)
        {   //��ǰë��Ϊ����֮ǰ�洢��Ƥ��ҲΪ����
            if (gb_TareSigFlg == true)
            {   //Ƥ�ظ�����˵����ǰ����С�ڱ����Ƥ�أ�������ȥƤ
                if (gu32_TareNquad < fu32_Tare)
                {
                    gu32_TareNquad = fu32_Tare;    //�洢Ƥ��ֵ,4���ֶ���
                    gb_TareSigFlg = fb_SigFlg; //�洢Ƥ��ֵ��������
                    gb_TareGetedFlg = true;
                }
            }
            else
            {   //��ǰë��Ϊ������֮ǰ�洢��Ƥ��Ϊ��������ǰë��С�ڱ����Ƥ�أ�����ȥƤ
                gu32_TareNquad = fu32_Tare;    //�洢Ƥ��ֵ,4���ֶ���
                gb_TareSigFlg = fb_SigFlg; //�洢Ƥ��ֵ��������
                gb_TareGetedFlg = true;
            }
        }
        else
        {   //��ǰë��Ϊ�������洢��Ƥ��ҲΪ���������ҵ�ǰë��С�ڱ����Ƥ�أ�����ȥƤ
            if (gu32_TareNquad > fu32_Tare)
            {
                gu32_TareNquad = fu32_Tare;    //�洢Ƥ��ֵ,4���ֶ���
                gb_TareSigFlg = fb_SigFlg; //�洢Ƥ��ֵ��������
                gb_TareGetedFlg = true;
            }
        }
    }
}

/*********************************************************************
��������: void FlapBag(void)
��    ��: �Ĵ�����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void FlapBag(void)
{
    if (gb_RunFlg)
    {
        if (gb_AddFlapBagEndFlg == false)     //A�������Ĵ�û������
        {
            //�������Ĵ��������� �Ĵ����ش򿪲��Ҳ������ú�����Ч
            if ((gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B) && (gs_Rec.FlapBagSwitch == PF_ || gs_Rec.FlapBagSwitch == PFD)
                && (gs_Rec.FlapBagAddNum!= 0) && (gs_Rec.FlapBagTimeHi != 0))
            {
                //��Ͷ����Ͷʱ�����ҵ�ǰ���������Ĵ���ʼ����
                if ((gb_Sp1Flg || gb_Sp2Flg) && (gu32_Wt >= gs_Rec.FlapBagWt) && gb_SigWtFlg == false)
                {
                    gb_AddFlapBagFlg = true;
                    gb_FlapWaitFlg = true;         //�Ĵ���ʱ��ʱ������
                    gu32_TimeFlapBuf = gs_Rec.FlapBagDelay * 10; 
                    gu32_Time8_9 = gu32_Sys10ms;
                    gb_AddFlapBagEndFlg = true;
                }
            }
        }
        
        //�Ĵ�����
        if (gb_FlapWaitFlg)         //�Ĵ���ʱ������
        {
            gb_FlapBagOutFlg = false;           //��ʱ�ڼ��Ĵ���Ч
            if (gu32_Sys10ms - gu32_Time8_9 >= gu32_TimeFlapBuf)   //�Ĵ�������ʱ��,�����Ĵ����̸ߵ�ƽ����ʱ��
            {
                gb_FlapWaitFlg = false;
                gb_FlapStartFlg = true;        //�����Ĵ�
                gb_FlapBagOutFlg = true;       //�Ĵ������Ч
                gb_FlapAllHighFlg = false;
                if (gs_Rec.FlapBagTimeLow == 0)
                    gb_FlapAllHighFlg = true;
                //��ֵ�������Ĵ�����Ϻ��Ĵ�����
                gu32_FlapBagNumCnt = (gb_AddFlapBagFlg) ? gs_Rec.FlapBagAddNum : gs_Rec.FlapBagNum;
                gu32_Time8_9 = gu32_Sys10ms;
            }
        }
        else if (gb_FlapStartFlg == true)      //�Ĵ��Ѿ�����
        {
            if (gb_FlapBagOutFlg)           //�Ĵ������Ч
            {
                if (gb_ExtFlapBagFlg == false)
                {
                    if (gu32_Sys10ms - gu32_Time8_9 >= gs_Rec.FlapBagTimeHi * 10)        //�ߵ�ƽʱ�䵽
                    {
                        gb_FlapBagOutFlg = false;
                        gu32_Time8_9 = gu32_Sys10ms;
                    }
                }
                else    //�����Ĵ������Чʱ��
                {
                    if (gu32_Sys10ms - gu32_Time8_9 >= gs_Rec.FlapBagExtTime * 10)        //�ߵ�ƽʱ�䵽
                    {
                        gb_FlapBagOutFlg = false;
                        gu32_Time8_9 = gu32_Sys10ms;
                    }
                }
            }
            else                            //�Ĵ������Чʱ
            {
                if (gu32_Sys10ms - gu32_Time8_9 >= gs_Rec.FlapBagTimeLow * 10)       //�͵�ƽʱ�䵽
                {
                    if (--gu32_FlapBagNumCnt == 0)
                        gu32_FlapBagNumCnt = 0;

                    //�Ĵ������Ѿ���
                    if (gu32_FlapBagNumCnt == 0 || gu32_FlapBagNumCnt & 0x80000000)
                    {
                        //��������Ĵ����ʱ�䲻Ϊ0���Ҳ��Ǽ������Ĵ������������Ĵ����
                        if (gs_Rec.FlapBagExtTime != 0 && gb_ExtFlapBagFlg == false && gb_AddFlapBagFlg == false)
                        {
                            if (gs_Setup.No2PackOffTwoSwitch != OFF)
                            {   //ͬʱ�ɴ��Ļ���ʱ������������
                                gb_AddingOutFlg = false;
                            }
                            if (gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
                            {   //�޶�˫�ӻ���
                                if (NO2Wait(ON1))
                                    return;
                            }
                            gb_FlapBagOutFlg = true;
                            gu32_Time8_9 = gu32_Sys10ms;
                            gb_ExtFlapBagFlg = true;  //�����Ĵ������־λ
                            gu32_FlapBagNumCnt = 1;
                        }
                        else
                        {
                            gu32_FlapBagNumCnt = gs_Rec.FlapBagNum;
                            gb_FlapStartFlg = false;        //�Ĵ�����
                            gb_FlapBagOutFlg = false;
                            gb_FlapWaitFlg = false;
                            gb_FlapAllHighFlg = false;
                            gb_AddFlapBagFlg = false; //����������Ĵ���־λ
                            gb_ExtFlapBagFlg = false; //��������Ĵ������־λ
                        }
                    }
                    else
                    {
                        gb_FlapBagOutFlg = true;
                        gu32_Time8_9 = gu32_Sys10ms;
                    }
                }
            }
        }
    }
    else
    {
        gb_FlapWaitFlg = false;
        gb_FlapStartFlg = false;
        gb_FlapBagOutFlg = false;
        gb_FlapAllHighFlg = false;
        gb_AddFlapBagEndFlg = false;
    }
}

/*********************************************************************
��������: void PackOff(void)
��    ��: �ɴ�����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void PackOff(void)
{
    gb_JSDDelayFlg = true;
    gb_SDDelayFlg = false;
    gb_SDBDelayFlg = true;
    gu32_JSDTime = gu32_Sys10ms;
}

/*********************************************************************
��������: bool NO2Wait(u32 fu32_No2PackOffTwoSwitch)
��    ��: �޶�˫�ӻ����ȴ��Ĵ���
˵    ��: 
��ڲ���: 
�� �� ֵ: true�ȴ� false����ִ��
��    ��: �Ժ���               ʱ    ��: 2015-02-05
��    ��:                      ʱ    ��: 
*********************************************************************/
bool NO2Wait(u32 fu32_No2PackOffTwoSwitch)
{
    bool lb_Result = false;
    if ((gs_Setup.No2PackOffTwoSwitch > 0 && fu32_No2PackOffTwoSwitch == 0) ||
        (gs_Setup.No2PackOffTwoSwitch == 0 && fu32_No2PackOffTwoSwitch > 0))
    {   //���õ�ͬʱ�ɴ���ִ�еĲ�ͬʱ�����ز��ȴ�״̬������ʱ����ִ��������ж�
        //��Ϊ������OFF/ON1/ON2����״̬������ON1/ON2����״̬Ҫ�ȴ������λ����ͬ����ڲ���ֻ����һ��ON1�����������ж�
        //�����ֻ������OFF/ON1,����ͬʱ�ɴ��������������ֵOFF/ON1/ON2�����ON1ʱ����������ON1/ON2ʱ��Ҫ����������ж�
        return false;
    }
    
    switch (gs_Setup.No2PackOffTwoSwitch)
    {
        case OFF:   //ͬʱ�ɴ����عرգ�����ȫ���ɴ������������ͻ�
            if (gb_AddingInFlg)
            {   //��ʱ��һ�ӻ�û���ɴ���ȴ���һ���ɴ����������ͻ�
                lb_Result = true;
            }
            else
            {   //����һ���Ѿ��ɴ�������û�мд������ж�����һ�����
                if (gb_No2PackOffAnother)
                {   //����һ���Ѿ��ɴ����������ͻ�������ȴ�
                    gb_ConveyorStartFlg = true; //�������ͻ�
                    gb_No2PackOffAnother = false;
                    lb_Result = false;
                }
                else
                {
                    lb_Result = true;
                }
            }
            break;
        case ON1:   //ͬʱ�ɴ���ͨģʽ
            if (gb_AddingInFlg)
            {   //��ʱ��һ���ڼ�����ȴ���һ��һ���ɴ�
                lb_Result = true;
            }
            else
            {   //��һ��û�м��ϣ����ɴ����������ͻ�
                gb_ConveyorStartFlg = true; //�������ͻ�
                lb_Result = false;
            }
            break;
        case ON2:   //ͬʱ�ɴ�����ģʽ
            if (gs_Setup.TareMode == NO2_A)
            {
                //�ɴ����������ͻ�
                gb_ConveyorStartFlg = true; //�������ͻ�
                lb_Result = false;
            }
            else if (gs_Setup.TareMode == NO2_B)
            {
                if (gb_AddingInFlg)
                {   //��ʱ��һ���ڼ�����ȴ���һ��һ���ɴ�
                    lb_Result = true;
                }
                else
                {   //��һ��û�м��ϣ����ɴ����������ͻ�
                    gb_ConveyorStartFlg = true; //�������ͻ�
                    lb_Result = false;
                }
            }
            break;
        default:
            break;
    }
    return lb_Result;
}

/*********************************************************************
��������: void WaitT5(void)
��    ��: Aж����ʱ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void WaitT5(void)
{
    if (gb_TimeRunFlg)            //ж����ʱ�Ѿ�����
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)       //ж����ʱ��
        {
            gb_DiscFlg = false;
            gb_TimeRunFlg = false;
            if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)    //�ж��������ɴ�����
            {   //����������һ��ʱ�ɴ�����ϴ���Ϊ0���ù��ɴ�����Ϊѹ���Ͳ��жϼ��ɴ�
                if (gu32_WeighTimeCnt <= 1 && gs_Rec.CombinedNum != 0)
                {
                    PackOff();            //�ɴ�����
                    gb_LiftBagStartFlg = true;
                    gb_PushBagStartFlg = true;
                }
                else if (gs_Rec.CombinedNum == 0 && (gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B))
                {   //˫�ӻ���ģʽ ������üд� ��ô��ʱж�����Ӧ��������һ��ж��
                    gb_DiscEnableOutFlg = true;
                }
            }
        }
    }
    else
    {
        if (gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
        {   //�޶�˫�ӻ���ģʽ ͬʱ�ɴ����عر�ʱ�ĵȴ�
            if (NO2Wait(OFF))
                return;
        }
        OneTareOver();            //���һ�Ӻ�Ĵ���
    }
}

/*********************************************************************
��������: void Disc(void)
��    ��: ж�ϴ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void Disc(void)
{
    if (gb_Error9Flg)     //ж��δ�ɾ�����
        return;
    
    gb_CompFlg = false;
    
    if (gb_DiscFlg == false && (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B))
        gb_DiscFlg = true;
    //�ж�ģʽ������ж������ܴ�
    if ((gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
         && gs_Rec.DiscShakeSwitch != 0 && gs_Rec.DiscShakeNum != 0)
    {
        if (gb_DiscShakeStartFlg)     //ж������Ѿ���ʼ
        {
            if (gb_DiscShakeFlg)      //ж�����ߵ�ƽ
            {
                if (gu32_Sys10ms - gu32_DiscShakeTimeBuf >= gs_Rec.DiscShakeTimeHi * 10)
                {
                    gb_DiscShakeFlg = false;
                    gu32_DiscShakeTimeBuf = gu32_Sys10ms;
                }
            }
            else    //ж�����͵�ƽ
            {
                if (gu32_Sys10ms - gu32_DiscShakeTimeBuf >= gs_Rec.DiscShakeTimeLow * 10)
                {
                    gb_DiscShakeFlg = true;
                    gu32_DiscShakeTimeBuf = gu32_Sys10ms;
                    gu32_DiscShakeCnt++;
                    if (gu32_DiscShakeCnt >= gs_Rec.DiscShakeNum)
                    {
                        gu32_DiscShakeCnt = 0;
                        gb_DiscShakeStartFlg = false;
                        gb_DiscShakeFlg = false;
                        ErrorDeal(9, 0);        //error9ж�����ʱ�䵽��δС������ֵ
                        gb_AlarmFlg = true;
                        gb_Error9Flg = true;
                        return;
                    }
                } 
            }
        }
        //ж����Чʱ�䵽������û�е�������ֵ������ж�����
        else if (gu32_Sys10ms - gu32_DiscShakeTimeBuf >= gs_Rec.DiscEffTime * 10)
        {
            gb_DiscShakeStartFlg = true;  //ж�����ʼ
            gb_DiscShakeFlg = true;       //ж����������Ч
            gu32_DiscShakeTimeBuf = gu32_Sys10ms;     //��������ж�����ʱ��
            gu32_DiscShakeCnt = 0;
        }
        if (Sp_Disc_State(DISC_STATE) == false)
        {   //���ڼд���λ��Ч���µ�ж���ź������Ч����ôֹͣж�����
            gb_DiscShakeStartFlg = false;
            gb_DiscShakeFlg = false;       //ж����������Ч
            gu32_DiscShakeCnt = 0;
            gu32_DiscShakeTimeBuf = gu32_Sys10ms;
        }
    }
    
#ifdef DISC_TIME_CONTROL
    if (gb_TimeRunFlg)            //ж��ʱ���Ѿ�����
    {
        if (gu32_Sys1ms - gu32_Time1_5 >= gu32_TimeBuf)       //ж��ʱ�䵽
        {
            gu32_DiscShakeCnt = 0;
            gb_DiscShakeStartFlg = false;
            gb_DiscShakeFlg = false;
            gb_DispNetFlg = false;
            gu32_Process = T5PROCESS;
            gu32_TimeBuf = gs_Rec.TimeT5 * 10;
            gb_TimeRunFlg = true;
            gu32_Time1_5 = gu32_Sys10ms;
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T5START;
        #endif
        }
    }
#else
    if (gb_NzFlg)         //ж�ϵ�����������ж����ʱt5
    {
        gu32_DiscShakeCnt = 0;
        gb_DiscShakeStartFlg = false;
        gb_DiscShakeFlg = false;
        gb_DispNetFlg = false;
        gu32_Process = T5PROCESS;
        gu32_TimeBuf = gs_Rec.TimeT5 * 10;
        gb_TimeRunFlg = true;
        gu32_Time1_5 = gu32_Sys10ms;
    #ifdef DEBUG_WT_SEND
        gu8_DebugState2 = T5START;
    #endif
    }
#endif
}

/*********************************************************************
��������: void WaitPack(void)
��    ��: ��ֵ���ֽ�����ȴ��д�
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void WaitPack(void)
{
    if (gs_Rec.CombinedNum != 0)  //������ϴ�������0ʱ�����жϼд�
    {   //�Ѿ��д���ɿ���ж��
        if (PACKEND)
        {
            gb_TimeRunFlg = false;
        }
    }
    else
        gb_TimeRunFlg = false;
}

/*********************************************************************
��������: void WaitT4(void)
��    ��: ��ֵ����ʱ��t4����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void WaitT4(void)
{
    if (gb_TimeRunFlg)        //��ֵ����T4ʱ���Ѿ�����
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)       //��ֵ����ʱ�䵽
        {
            if (gs_DebugRunTime.bGetRunCmpTimeFlg == false)
            {
                //��������ֵ����ʱ��
                gs_DebugRunTime.u32RunCmpTime = gu32_Sys1ms - gs_DebugRunTime.u32RunStartTimeTemp;
                gs_DebugRunTime.bGetRunCmpTimeFlg = true;
            }
            
            if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)    
            {//�ж�ģʽ
                if (gb_GoFlg)
                {   //����һ�κ��ֹͣ
                    gb_GoFlg = false;
                    RunStopInit(false);
                }
                if (gb_DiscWtGetedFlg == false)
                {
//                    gu32_HoldWt = gu32_Wt;  //A�Ӷ�ֵ����
                    gu32_HoldWt = DISP_WT_KALMAN;  //A�Ӷ�ֵ����
                    gb_DiscWtGetedFlg = true;
                }
                if (gs_Setup.TareMode == BINYES)
                    WaitPack();         //�ȴ��д�
                else    //˫�ӻ���ģʽ
                {
                    if (gb_DiscEnableInFlg) 
                    {   //��һ�ӹ���������ж���ź���Ч�����Խ�����һ��
                        gb_DiscEnableOutFlg = false;    //����ж�������Ч����ֹ��һ��ж��
                        WaitPack();         //�ȴ��д�
                    }
                    else
                    {/*��һ�ӹ���������ж���ź���Ч�����ұ��ӵ�ж���������Ҳ��Ч����ôֻ��
                       һ�����������СͶ����ʱ�Ļ�����������ź��ж�������ͬʱ���еģ�ͬʱ
                       �жϵ�����ж��������ЧȻ��ͬʱ������ж�������Ϊ��Ч��
                       ��ô��ʱ�������������������ݳ���ģʽ���õ����ȼ���������һ�ӷ��ϣ�
                       Ĭ����BIN2-A�ӷ��ϣ�B�ӵȴ���һ�μд���*/
                        if (gb_DiscEnableOutFlg == false)
                        {   
                            if (gs_Setup.TareMode == BIN2_B)
                            {//BIN2-B������ж�������Ϊ��Ч����ôBIN2-A�ӾͿ���ж����
                                gb_DiscEnableOutFlg = true;
                            }
                        }
                    }
                }
            }
            else if (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)    //�޶�ģʽ��ֱ�ӽ��붨ֵ���ж� 
            {
                if (gb_GoFlg)
                {   //����һ�κ��ֹͣ
                    gb_GoFlg = false;
                    RunStopInit(false);
                }
                gb_TimeRunFlg = false;
                Deal_WeighTime_Sum();
                //���Ϻ��Ĵ��������� �Ĵ����ش򿪲��Ҳ������ú�����Ч
                if ((gs_Rec.FlapBagSwitch == P_D || gs_Rec.FlapBagSwitch == PFD)
                    && (gs_Rec.FlapBagNum!= 0) && (gs_Rec.FlapBagTimeHi != 0))
                {
                    gb_AddFlapBagFlg = false;
                    gb_FlapWaitFlg = true;         //�Ĵ���ʱ��ʱ������
                    gu32_TimeFlapBuf = gs_Rec.FlapBagDelay * 10;
                    gu32_Time8_9 = gu32_Sys10ms;
                }
            }
        }
    }
    else        //��ֵ����ʱ��t4�Ѿ�����
    {
        if (gs_Setup.TareMode == BINYES || gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
        {
            //��������в�����ж�ϵĹ��ܴ���
            if (gb_PrintCodeIngFlg && gs_Rec.SheildDiscSwitch !=  0)
                return;
            gb_CompFlg = false;
            gb_TimeRunFlg = false;
            
            //�Ĵ�����
            if ((gs_Rec.FlapBagSwitch == P_D || gs_Rec.FlapBagSwitch == PFD)
                && (gs_Rec.FlapBagNum!= 0) && (gs_Rec.FlapBagTimeHi != 0))      //�Ĵ����ش򿪲��Ҳ������ú�����Ч
            {
                if (gb_FlapWaitFlg == false && gb_FlapStartFlg == false)        //ԭ�����ڷ��Ĵ��Լ����Ĵ���ʱ״̬,�����Ĵ���ʱ
                {
                    //���ģʽ�����һ�η��ϲ��Ĵ�,��ϴ���Ϊ0���Ĵ�
                    if (gu32_WeighTimeCnt <= 1 && gs_Rec.CombinedNum != 0)
                    {
                        gb_FlapWaitFlg = true;         //�Ĵ���ʱ��ʱ������
                        gu32_TimeFlapBuf = gs_Rec.FlapBagDelay * 10;
                        gu32_Time8_9 = gu32_Sys10ms;
                    }
                }
            }
            else
            {
                gb_FlapWaitFlg = false;
                gb_FlapStartFlg = false;
                gb_FlapBagOutFlg = false;
            }
            
            Deal_WeighTime_Sum();
            Adapt_Process(&gs_Adapt);    //����Ӧ��������
            gu32_Process = FALLPROCESS;
            gb_DiscFlg = true;           //����ж��
            gu32_DiscShakeTimeBuf = gu32_Sys10ms;     //��¼����ж�ϵ�ʱ��
            gb_DiscWtGetedFlg = false;
        #ifdef DISC_TIME_CONTROL
            gu32_TimeBuf = gs_Rec.NearZero;
            gb_TimeRunFlg = true;
            gu32_Time1_5 = gu32_Sys1ms;
        #endif
        }
        else if (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
        {
            gb_CompFlg = false;
            gb_TimeRunFlg = false;
            
            //BINNOģʽ�Ĵ��Ѿ�����������ɴ����������������Ĵ�ʱ�����ɴ�
            //NO2_A/Bģʽ��Ҫ���㻥�����������ɴ�
            if ((gb_FlapStartFlg == false && gb_FlapWaitFlg == false) || gb_ExtFlapBagFlg == true)
            {
                if (gs_Setup.No2PackOffTwoSwitch != OFF)
                {   //ͬʱ�ɴ��Ļ���ʱ������������
                    gb_AddingOutFlg = false;
                }
                if (gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
                {   //�޶�˫�ӻ���
                    if (NO2Wait(ON1))
                        return;
                }
                //�����ɴ���ʱ
                PackOff();  //�ɴ�
                if (gs_Setup.No2PackOffTwoSwitch == OFF)
                {   //����޶�˫�ӻ������
                    gb_AddingOutFlg = false;
                    if (gs_Setup.No2AddAgainDelayTime != 0)
                        gb_No2AddAgainDelay = true;
                }
                gb_LiftBagStartFlg = true;
                gb_PushBagStartFlg = true;
                gu32_Process = FALLPROCESS;
                if (gu32_SumTemp != 0)
                {//�ۼƽ��
                    //ModbusͨѶ��ʹ�ã��ϳӽ��
                    gu32_MbLastResult = gu32_SumTemp;
                    //�����ۼ�����
                    Save_SumData(gu32_SumTemp, 1);            
                #ifndef DEBUG_MONITORTIME_PRINT
                    //�Զ���ӡ
                    PrintStart(PRINT_AUTO, gu32_SumTemp, 0);
                #endif
                    gu32_SumTemp = 0;
                }
                //�޶��ɴ�ֱ�ӽ���T5PROCESS���Ҳ���T5ж����ʱ��ֱ�ӽ������һ�Ӻ�Ĵ���
                gb_DispNetFlg = false;
                gu32_Process = T5PROCESS;
                gb_TimeRunFlg = false;
            }
        }
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = DISCSTART;
        #endif
    }
}

/*********************************************************************
��������: void WaitT3(void)
��    ��: ������ʱ��
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void WaitT3(void)
{
    bool lb_Quit;
    
    if (gb_AlarmFlg)
        return;
    
    if (gb_TimeRunFlg)                //������ʱ���Ѿ�����
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)           //������ʱ�䵽
        {
            lb_Quit = CorrectFall();          //�������
            if (lb_Quit == 0)           //���������ɻ�������������عر�
            {
                if ((gs_Rec.OUSwitch != 0) && (gs_Rec.CorrectFallSwitch == 0) && (gb_StabFlg == false) && (!StabOver(false)))
                {
                    ;       //��������رգ����ܵ��²��ȶ�ʱҲ��λquit�����ҳ�Ƿ��򿪣����Ҳ��ȶ���ȴ���
                }
                else
                {
                    StabOver(true); //��λ���ȳ�ʱ����
                    if (gu32_WeighTimeCnt <= 1 || (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B))
                    {   //������ϵ����һ�Ӳ��жϳ�Ƿ��,�޶�û�е�����ϴ���
                        if (gs_Rec.CombinedNum <= 1 || (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B))    //������ϴ���С�ڵ���1����Ŀ��ֵ�жϳ�Ƿ��
                            Over_Under(gs_Rec.Target, gu32_Wt);         //��Ƿ���
                        else    //����1��Ŀ��ֵ�ı����ж�
                            Over_Under(gs_Rec.Target * gs_Rec.CombinedNum, gu32_SumTemp + gu32_Wt);         //��Ƿ���
                        
                        if (gb_AlarmFlg == false)    //û�б����������һ����
                        {
                            gu32_Process = T4PROCESS;
                            gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                            gb_TimeRunFlg = true;                //��ֵ����ʱ��t4�Ѿ�����
                            gu32_Time1_5 = gu32_Sys10ms;        //ȡ��ǰʱ��
                        }
                    }
                    else
                    {
                        gu32_Process = T4PROCESS;
                        gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                        gb_TimeRunFlg = true;                //��ֵ����ʱ��t4�Ѿ�����
                        gu32_Time1_5 = gu32_Sys10ms;        //ȡ��ǰʱ��
                    #ifdef DEBUG_WT_SEND
                        gu8_DebugState2 = T4START;
                    #endif
                    }
                }
            }
        }
    }
}

/*********************************************************************
��������: u8 PredictSp3_Sample(u32 fu32_Sp3StopWt)
��    ��: СͶԤ���ֹ�� �����ɼ��������������ݼ���
˵    ��: �ú������ڲ�����������Ϻ���LeastLinear ��������������������RoundOff
��ڲ���: fu32_Wt=��ǰ���� fu32_Sp3StopWt=СͶ��ֹ������
���ڲ���: 
�� �� ֵ: //0x00=СͶ��ֹ��Ԥ�⹦��û�д�
          //0x10=ǰ���������Kֵ�ɼ��׶� ��û��Խ������ϲ�������u3InvalidTimes
          //0x11=ǰ���������Kֵ�ɼ��׶� �Ѿ�Խ������ϲ������� ��û�дﵽ������Ч��������u32ValidTimes
          //0x12=ǰ���������Kֵ�ɼ��׶� �Ѿ��ﵽ������Ч��������u32ValidTimes
          //0x20=�������Kֵ�Ѿ��ɼ���� ��û��Խ������ϲ�������u3InvalidTimes
          //0x21=�������Kֵ�Ѿ��ɼ���� �Ѿ�Խ������ϲ������� ��û�дﵽ������Ч��������u32ValidTimes
          //0x22=�������Kֵ�Ѿ��ɼ���� �Ѿ��ﵽ������Ч��������u32ValidTimes ��ʼ�ӹ�СͶ��ֹ�����
          //0x30=������ֲ������ֵ ������ϵõ�YֵΪ����
��    ��: �Ժ���               ʱ    ��: 2015-08-10
��    ��:                      ʱ    ��: 
*********************************************************************/
u8 PredictSp3_Sample(u32 fu32_Sp3StopWt)
{
    s32 ls32_Temp;
    
//    if (gs_PredictSp3.u8Switch != 0)
//    {   //СͶ������Ԥ�⿪�ش�
        if (++gs_PredictSp3.u32CntInvalid > gs_PredictSp3.u32InvalidTimes)
        {   //��ֵ��Խ������ϵĲ�������
            gs_PredictSp3.u32CntInvalid = gs_PredictSp3.u32InvalidTimes;    //��ֹ�����������
            
            if (++gs_PredictSp3.u32CntValid >= gs_PredictSp3.u32ValidTimes)
            {   //��Ч���������Ѿ��������õļ�����ϵ����� ��ʼ���㱾��СͶ���ϵ����Kֵ
                //�ﵽ��������������󳤶Ƚ��д���
                if (gs_PredictSp3.u32CntValid >= LINEARARRAY_LENGTH - 1)
                    gs_PredictSp3.u32CntValid = LINEARARRAY_LENGTH - 1;
                //�������Kֵ
                gs_PredictSp3.s32KThisTime = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_PredictSp3.u32CntValid, 2, 0);
                
                if (gs_PredictSp3.bSampleDoneFlg)
                {
                    if (gs_PredictSp3.u8State != 0x22)
                    {   //�������Kֵ�Ѿ��ɼ���� ���нӹ�СͶ��ֹ�������ؼ���
                        //���ݵ�ǰ����������Yֵ ����ǰ��Ϻ������ֵ ������ϵ�ǰ������Ϊ�������
                        ls32_Temp = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - gs_PredictSp3.u32CntValid, 1, LINEARARRAY_LENGTH);
                        if (ls32_Temp < 0)
                        {   //�������Yֵ������
                            gs_PredictSp3.u8State = 0x30;
                            return gs_PredictSp3.u8State;
                        }
                        ls32_Temp = ((fu32_Sp3StopWt >= ls32_Temp) ? (fu32_Sp3StopWt - ls32_Temp) : (ls32_Temp - fu32_Sp3StopWt)); //���㵽����ǰ���Ĳ�ֵY
                        //��֪Y �Ե�ǰ״̬Ϊ0�� ����ǰ����Kƽ��ֵ��X��ֵ ������Ҫ���ٴ�ADת��������ǰ�� �ӹ�СͶ��ֹ�����
                        gs_PredictSp3.u32PredictTarget = RoundOff(ls32_Temp * 1000, gs_PredictSp3.s32KAverage);
                    }
                    gs_PredictSp3.u8State = 0x22;   //�������Kֵ�Ѿ��ɼ���� �Ѿ��ﵽ������Ч��������u32ValidTimes ��ʼ�ӹ�СͶ��ֹ�����
                }
                else
                    gs_PredictSp3.u8State = 0x12;   //ǰ���������Kֵ�ɼ��׶� �Ѿ��ﵽ������Ч��������u32ValidTimes
            }
            else
            {
                if (gs_PredictSp3.bSampleDoneFlg)
                    gs_PredictSp3.u8State = 0x21;  //�������Kֵ�Ѿ��ɼ���� �Ѿ�Խ������ϲ������� ��û�дﵽ������Ч��������u32ValidTimes
                else
                    gs_PredictSp3.u8State = 0x11; //ǰ���������Kֵ�ɼ��׶� �Ѿ�Խ������ϲ������� ��û�дﵽ������Ч��������u32ValidTimes
            }
        }
        else
        {
            if (gs_PredictSp3.bSampleDoneFlg)
                gs_PredictSp3.u8State = 0x20; //�������Kֵ�Ѿ��ɼ���� ��û��Խ������ϲ�������u3InvalidTimes
            else
                gs_PredictSp3.u8State = 0x10; //ǰ���������Kֵ�ɼ��׶� ��û��Խ������ϲ�������u3InvalidTimes
        }
//    }
//    else  //ע�͵� Ϊ������Ӧ������ʹ��gs_PredictSp3.s32KThisTime����
    if (gs_PredictSp3.u8Switch == 0)
    {
        gs_PredictSp3.u8State = 0;
    }
    
    return gs_PredictSp3.u8State;
}

/*********************************************************************
��������: bool PredictSp3_Control(void)
��    ��: �ӹ�СͶ��ֹ���Ŀ��ƴ���
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: true=СͶ���� false=СͶδ����
��    ��: �Ժ���               ʱ    ��: 2015-08-10
��    ��:                      ʱ    ��: 
*********************************************************************/
bool PredictSp3_Control(void)
{
    bool lb_Result;
    
    if (gs_PredictSp3.u8Switch != 0)
    {   //СͶ��ֹ��Ԥ�⹦�ܴ�
        if (gs_PredictSp3.u8State == 0x22)
        {   //�ӹ�СͶ��ֹ�����
            if (gs_PredictSp3.u32CntTarget >= gs_PredictSp3.u32PredictTarget)
            {   //Ŀ��AD������СͶ����
                gs_DebugRunTime.s32SP3RealStopWt = gs32_WtKalmanFed;    //ʵ��ͣСͶʱ������
                lb_Result = true;
            }
            else
            {
                gs_PredictSp3.u32CntTarget++;
                lb_Result = false;
            }
        }
        else
        {
            lb_Result = false;
        }
    }
    else
    {
        lb_Result = false;
    }
    return lb_Result;
}

/*********************************************************************
��������: void PredictSp3_Init(void)
��    ��: СͶ��ֹ��Ԥ�⹦�ܳ�ʼ��
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-10
��    ��:                      ʱ    ��: 
*********************************************************************/
void PredictSp3_Init(void)
{
    gs_PredictSp3.u8Switch = gs_Setup.DiscSumSwitch;
    gs_PredictSp3.u32KSampleTimes = gs_Rec.TimeT12;
    gs_PredictSp3.u32KCntSample = 0;
    gs_PredictSp3.u8State = 0;
    gs_PredictSp3.bSampleDoneFlg = false;
    gs_PredictSp3.u32PredictTarget = 0;
    gs_PredictSp3.u32CntTarget = 0;
    gs_PredictSp3.s32KThisTime = 0;
    gs_PredictSp3.s32KAverage = 0;
    gs_PredictSp3.u32InvalidTimes = gs_Rec.TimeT6;
    gs_PredictSp3.u32CntInvalid = 0;
    gs_PredictSp3.u32ValidTimes = gs_Rec.TimeT7;
    gs_PredictSp3.u32CntValid = 0;
}


/*********************************************************************
��������: void PredictSp3_Restart(void)
��    ��: СͶ��ֹ��Ԥ�⹦���ٴ���������
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-10
��    ��:                      ʱ    ��: 
*********************************************************************/
void PredictSp3_Restart(void)
{
    gs_PredictSp3.u32PredictTarget = 0;
    gs_PredictSp3.u32CntTarget = 0;
    gs_PredictSp3.s32KThisTime = 0;
    gs_PredictSp3.u32CntInvalid = 0;
    gs_PredictSp3.u32CntValid = 0;
}

/*********************************************************************
��������: s32 PredictSp3_KAverage(void)
��    ��: ���㱣���ƽ��Kֵ
˵    ��: ȥ��һ�����ֵ ȥ��һ����Сֵ�����ƽ��ֵ
��ڲ���: 
���ڲ���: 
�� �� ֵ: ƽ��Kֵ
��    ��: �Ժ���               ʱ    ��: 2015-08-10
��    ��:                      ʱ    ��: 
*********************************************************************/
s32 PredictSp3_KAverage(void)
{
    s32 ls32_KMax;
    s32 ls32_KMin;
    s32 ls32_Sum;
    u8 i;
    
    ls32_KMax = gs_PredictSp3.s32KSampleArray[0];
    ls32_KMin = gs_PredictSp3.s32KSampleArray[0];
    ls32_Sum = 0;
    for (i = 0; i < gs_PredictSp3.u32KSampleTimes; i++)
    {   //��ȡ���ֵ��Сֵ����ֵ
        ls32_Sum += gs_PredictSp3.s32KSampleArray[i];
        if (ls32_KMax < gs_PredictSp3.s32KSampleArray[i])
            ls32_KMax = gs_PredictSp3.s32KSampleArray[i];
        if (ls32_KMin > gs_PredictSp3.s32KSampleArray[i])
            ls32_KMin = gs_PredictSp3.s32KSampleArray[i];
    }
    ls32_Sum -= (ls32_KMax + ls32_KMin);
    ls32_Sum /= (gs_PredictSp3.u32KSampleTimes - 2);    //����ƽ��Kֵ
    return ls32_Sum;
}

/*********************************************************************
��������: void PredictSp3_Calculate(void)
��    ��: СͶ��ֹ��Ԥ�⹦��������ݼ���
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-10
��    ��:                      ʱ    ��: 
*********************************************************************/
void PredictSp3_Calculate(void)
{
    u8 i;
    
    if (gs_PredictSp3.u8Switch != 0)
    {   //СͶ��ֹ��Ԥ�⹦�ܴ�
        if ((gs_PredictSp3.u8State & 0x02) == 0x02)
        {   //�Ѿ��ﵽ������Ч��������u32ValidTimes
            for (i = gs_PredictSp3.u32KSampleTimes - 1; i > 0; i--)
            {   //�������µ�Kֵ������[0]
                gs_PredictSp3.s32KSampleArray[i] = gs_PredictSp3.s32KSampleArray[i-1];
            }
            gs_PredictSp3.s32KSampleArray[0] = gs_PredictSp3.s32KThisTime;
            if (++gs_PredictSp3.u32KCntSample >= gs_PredictSp3.u32KSampleTimes)
            {   //�Ѿ��������õ�Kֵ�������
                gs_PredictSp3.u32KCntSample = gs_PredictSp3.u32KSampleTimes;    //�����
                //����ƽ��Kֵ
                gs_PredictSp3.s32KAverage = PredictSp3_KAverage();
                gs_PredictSp3.bSampleDoneFlg = true;
            }
        }
    }
}


/*********************************************************************
��������: void SmallFeed(void)
��    ��: СͶ����
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void SmallFeed(void)
{
    u8 lu8_State;
    
    gs_DebugRunTime.u32Sp3Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp3TimeTemp;  //СͶʱ��
    gb_Sp1Flg = false;
    gb_Sp2Flg = false;
    //СͶʱǿ��ֹͣ�������Ĵ�
    if (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
    {
        gb_AddFlapBagFlg = false;
        gb_FlapWaitFlg = false;
        gb_FlapStartFlg = false;
        gb_FlapBagOutFlg = false;
    }
    
#ifndef DEBUG_SMARTT2B
    if (gb_TimeRunFlg)            //СͶ��ֹ�Ƚ�ʱ���Ѿ�����
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)       //��ֹ�Ƚ�ʱ�����֮ǰСͶһֱ��Ч
        {
            gb_TimeRunFlg = false;
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T2BEND;
        #endif
        }
    }
#else
    if (gb_T2bRunFlg)
    {   //���ܽ�ֹ�Ƚ�ʱ���ж��Ѿ�����
        ;
    }
#endif
    else
    {   //��ֹ�Ƚ�ʱ���Ѿ�����
        SampleRushWt(&gs_Adapt);    //����Ӧ���� �ɼ���Ͷ������
            
            
        lu8_State = PredictSp3_Sample(gu32_TargetTemp - gs_Rec.Small);
        
        if (lu8_State != 0x22)
        {   //СͶ���ƻ�û�б�Ԥ�⹦�ܽӹ�
            //Ŀ��ֵ<=��� ���� ��ǰ����>=Ŀ��ֵ-��� ����T3
            if (((gf_WtKalmanFed >= gu32_TargetTemp - gs_Rec.Small) && (gb_SigWtFlg == false)) || (gs_Rec.Small >= gu32_TargetTemp))
            {
                PredictSp3_Calculate();
                gs_DebugRunTime.s32SP3RealStopWt = gs32_WtKalmanFed;    //ʵ��ͣСͶʱ������
                //����ʱ��
                gs_DebugRunTime.u32Sp3Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp3TimeTemp;
                gb_Sp3Flg = false;
                gb_CompFlg = true;           //��ֵ
                gb_TimeRunFlg = true;                    //������ʱ���ֵ����ʱ���Ѿ�����
                if (gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
                {   /*˫�ӻ���ģʽ��СͶ���������ж�һ������ж�������״̬�Ƿ���Ч��Ȼ��ı�����ж�����
                      ��ֵ���ֽ����󻹻��ж�һ������״̬��
                      ��Ϊ���ֻ�ڶ�ֵ���ֽ������жϣ���ô����ͬʱ�����Ļ����п��ܵ����Ǳ�ͬʱ�жϵ�����
                      ж��������Ч��Ȼ��ͬʱ������ж�������Ϊ��Ч���������Ӷ����ж���źţ����Ƕ�û�м�
                      �������*/
                    if (gb_DiscEnableInFlg)
                        gb_DiscEnableOutFlg = false;
                }
                if (gs_Rec.OUSwitch != 0 || gs_Rec.CorrectFallSwitch != 0)
                {   //��Ƿ���⿪�ش򿪻�������������ش������T3ʱ��
                    gu32_TimeBuf = gs_Rec.TimeT3 * 10;        //��������ʱ��
                    gu32_Process = T3PROCESS;               //����T3
                    gu32_Time1_5 = gu32_Sys10ms;                //��¼��ǰʱ����ΪT3����ʱ��
                #ifdef DEBUG_WT_SEND
                    gu8_DebugState2 = T3START;
                #endif
                }
                else
                {   //��Ƿ�������������ر�������T3��ֱ����T4
                    gu32_Process = T4PROCESS;
                    gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                    gu32_Time1_5 = gu32_Sys10ms;        //ȡ��ǰʱ��
                #ifdef DEBUG_WT_SEND
                    gu8_DebugState2 = T4START;
                #endif
                }
            }
        }
        else
        {
            if (PredictSp3_Control() == true)
            {   //СͶԤ����ƽ���
                PredictSp3_Calculate();
                //����ʱ��
                gs_DebugRunTime.u32Sp3Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp3TimeTemp;
                gb_Sp3Flg = false;
                gb_CompFlg = true;           //��ֵ
                gb_TimeRunFlg = true;                    //������ʱ���ֵ����ʱ���Ѿ�����
                if (gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
                {   /*˫�ӻ���ģʽ��СͶ���������ж�һ������ж�������״̬�Ƿ���Ч��Ȼ��ı�����ж�����
                      ��ֵ���ֽ����󻹻��ж�һ������״̬��
                      ��Ϊ���ֻ�ڶ�ֵ���ֽ������жϣ���ô����ͬʱ�����Ļ����п��ܵ����Ǳ�ͬʱ�жϵ�����
                      ж��������Ч��Ȼ��ͬʱ������ж�������Ϊ��Ч���������Ӷ����ж���źţ����Ƕ�û�м�
                      �������*/
                    if (gb_DiscEnableInFlg)
                        gb_DiscEnableOutFlg = false;
                }
                if (gs_Rec.OUSwitch != 0 || gs_Rec.CorrectFallSwitch != 0)
                {   //��Ƿ���⿪�ش򿪻�������������ش������T3ʱ��
                    gu32_TimeBuf = gs_Rec.TimeT3 * 10;        //��������ʱ��
                    gu32_Process = T3PROCESS;               //����T3
                    gu32_Time1_5 = gu32_Sys10ms;                //��¼��ǰʱ����ΪT3����ʱ��
                #ifdef DEBUG_WT_SEND
                    gu8_DebugState2 = T3START;
                #endif
                }
                else
                {   //��Ƿ�������������ر�������T3��ֱ����T4
                    gu32_Process = T4PROCESS;
                    gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                    gu32_Time1_5 = gu32_Sys10ms;        //ȡ��ǰʱ��
                #ifdef DEBUG_WT_SEND
                    gu8_DebugState2 = T4START;
                #endif
                }
            }
        }
    }
}

/*********************************************************************
��������: EnterSmallFeed
��    ��: ����СͶ����
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-26
��    ��:                      ʱ    ��: 
*********************************************************************/
void EnterSmallFeed(void)
{
    gs_DebugRunTime.s32SP2RealStopWt = gs32_WtKalmanFed;    //��¼ʵ����Ͷ��ֹ����
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
    
}

/*********************************************************************
��������: void MiddleFeed(void)
��    ��: ��Ͷ�������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void MiddleFeed(void)
{
    gb_Sp1Flg = false;
    if (gb_TimeRunFlg)            //��ֹ�Ƚ�ʱ���Ѿ�����
    {
        if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)       //��ֹ�Ƚ�ʱ�䵽
        {
            gb_TimeRunFlg = false;
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T2AEND;
        #endif
        }
    }
    else
    {
        if (gs_AutoStd.u8Process == AUTOSTD_SECEND)
        {   //����ǵڶ�����ѧϰ��������Ͷ��ʱ�����
            if (gu32_Sys1ms - gs_DebugRunTime.u32Sp2TimeTemp >= gs_AutoStd.u32SP2Time)
            {
                EnterSmallFeed();
                gs_AutoStd.u8Process = AUTOSTD_NONE;
                gs_Rec.Middle = gu32_TargetTemp - gs32_WtKalmanFed;       //��ǰ������ֹ�������Ͷ��ǰ��
                FRAM_WriteDW(FMAddr_Middle(gu32_RecNum), gs_Rec.Middle);
                RefreshDebugRec();  //���´�ӡ����Ͷ��ǰ��
                gs_Adapt.s32SP2Bkp = (*gs_Adapt.ps32SP2);   //��������Ӧ����Ͷ��ǰ��
            }
        }
        if ((gf_WtKalmanFed >= gu32_TargetTemp - gs_Rec.Middle) && (gb_SigWtFlg == false))
        {
            EnterSmallFeed();
        }
    }
}

/*********************************************************************
��������: void LargeFeed(void)
��    ��: ��Ͷ�������
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void LargeFeed(void)
{
    if ((gf_WtKalmanFed >= gu32_TargetTemp - gs_Rec.Large) && (gb_SigWtFlg == false))
    {
        //���ʱ��
        gs_DebugRunTime.u32Sp1Time = gu32_Sys1ms - gs_DebugRunTime.u32Sp1TimeTemp;
        //��¼�мӼ�����ʱ��
        gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;
        gb_Sp1Flg = false;
        gb_Sp2Flg = true;
        gu32_Process = MIDDLEPROCESS;
        if (gb_Sp2OverFlg == true)
        {
            gb_Sp2Flg = false;
            gu32_Process = SMALLPROCESS;
            gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
        }
        gb_Sp3Flg = true;
        gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
        gb_TimeRunFlg = true;        //������ֹ�Ƚ�ʱ��
        gu32_TimeBuf = gs_Rec.TimeT2a * 10;
        gu32_Time1_5 = gu32_Sys10ms;            //��¼��ǰϵͳʱ��
    #ifdef DEBUG_WT_SEND
        gu8_DebugState2 = SP2START;
    #endif
    }
}

/*********************************************************************
��������: void WaitT1(void)
��    ��: ������ʱ���̴���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void WaitT1(void)
{
    bool lb_Run = false;            //��ʼִ�м��ϱ�־��Ϊ0ʱ�ȴ�������������
    
    if (gb_BatchOverFlg)        //������������ֹͣ����
    {
        gu32_Process = 0;
        gb_AlarmFlg = true;
        return;
    }
#ifdef DEBUG_MONITORTIME_PRINT    //ˢ���䷽
    RefreshDebugRec();
#endif

    if (gs_Setup.TareMode == BINNO || gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)    //�޶�ģʽ
    {
        if (gb_TimeRunFlg)        //�������ʱ�Ѿ�����
        {
            if (gs_Setup.GorNMode == GROSS) //�޶�ë�� ��ȥƤֱ�Ӽ���
            {
                gb_TimeRunFlg = false;
                lb_Run = true;
            }
            else    //�޶����أ�ȥƤ����ʱһ��ʱ���ٿ�ʼ����
            {
                if (--gu32_TimeBuf == 0 || gu32_TimeBuf & 0x80000000)
                {
                    gb_TimeRunFlg = false;
                    lb_Run = true;
                }
            }
        }
        else
        {
            //�д���������ʱT1
            //�������ʱ����������
            if (PACKEND && (gb_PrintCodeIngFlg == false || gs_Rec.SheildDiscSwitch == false))
            {
                gb_AddingOutFlg = true;
                if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)    //T1������ʱʱ�䵽
                {
                    if (gs_Setup.TareMode == NO2_A || gs_Setup.TareMode == NO2_B)
                    {   //�޶�˫�ӻ������ϵȴ�
                        if (gb_No2AddAgainDelay)
                            return;
                    }
                    
                    if (gs_Setup.GorNMode == GROSS)  //�޶�ë�� ��ȥƤ
                    {
                        gu32_TareNquad = 0;    //�洢Ƥ��ֵ
                        gb_TareSigFlg = false; //�洢Ƥ��ֵ��������
                        gb_TimeRunFlg = true;
                    }
                    else    //�޶�����ģʽ ��Ҫ����ȥƤ
                    {
                        if (gb_StabFlg || StabOver(false))  //���� �ȶ����߳������ȳ�ʱʱ��
                        {
                            StabOver(true); //��λ���ȳ�ʱ����
                            GetTare(gu32_Nquad, gb_SigWtFlg, true);   //ȥƤ
                            BuzzerStart(30);
                            gb_DispNetFlg = true;        //��ʾ����
                            gu32_TimeBuf = 1;    //ȥƤ����ʱ����
                            gb_TimeRunFlg = true;
                        }
                    }
                }
            }
            else
                gu32_Time1_5 = gu32_Sys10ms;
        }
    }
    else    //�ж�ģʽ
    {
        if (gb_TimeRunFlg)        //�������ʱ�Ѿ�����
        {
            if (gb_AlarmFlg)
            {
                gu32_TimeBuf = 2;
            }
            else if (--gu32_TimeBuf == 0)
            {
                gb_TimeRunFlg = false;
                lb_Run = true;
                gu32_TareNquad = 0;
                gb_TareSigFlg = false;
                gb_DispNetFlg = true;        //��ʾ����
            }
        }
        else if (gu32_Sys10ms - gu32_Time1_5 >= gu32_TimeBuf)          //T1������ʱʱ�䵽
        {
            //�Զ��������Ĵ���
            if (gs_Setup.AutoClrZeroInterval == 0)       //������Ϊ0������
            {
                lb_Run = true;
                gu32_TareNquad = 0;
                gb_TareSigFlg = false;
                gb_DispNetFlg = true;        //��ʾ����
            }
            else if (gb_FirstStartFlg)            //������A�ӵ�һ�ӱ�־,��λ��Ч,����ǰ������
            {
                gb_FirstStartFlg = false;     //������к��һ�ӱ�־
                lb_Run = true;                 //���β�����
                gu32_TareNquad = 0;
                gb_TareSigFlg = false;
                gb_DispNetFlg = true;        //��ʾ����
                gu32_AutoClrZeroCnt--;
                if (gu32_AutoClrZeroCnt <= 1)      //����������
                    gu32_AutoClrZeroCnt = 0;
            }
            else if (gu32_AutoClrZeroCnt <= 1)  //�����������ȶ�������
            {
                if (gb_StabFlg || StabOver(false))         //�ȶ������������
                {
                    StabOver(true); //��λ���ȳ�ʱ����
                    gb_TimeRunFlg = true;
                    if (gb_StabFlg)
                        gb_ZeroKeyFlg = true;
                    else
                        gb_ForceClrZeroFlg = true;
                    gu32_TimeBuf = 1;
                    gu32_AutoClrZeroCnt = gs_Setup.AutoClrZeroInterval;//��λ������������
                }
            }
            else    //������δ��
            {
                gu32_AutoClrZeroCnt--;         //������δ��,��������������һ
                lb_Run = true;
                gu32_TareNquad = 0;
                gb_TareSigFlg = false;
                gb_DispNetFlg = true;        //��ʾ����
            }
        }
        
        if (gb_OflFlg || gb_AdOKFlg == false)//������� //����ǰ������� һֱ���ڼ���ǰ�ȴ�״̬
        {
            gb_TimeRunFlg = true;
            gb_OflDelayFlg = true;
            return;
        }
        else if (gb_OflDelayFlg)
        {
            gb_OflDelayFlg = false;
            gb_TimeRunFlg = true;
            gu32_TimeBuf = 120;        //�����ת�������״̬Ҫ�ٽ���120��ADת���ټ���
            return;
        }
    }
    gb_Sp1OverFlg = false;
    gb_Sp2OverFlg = false;
    if ((gu32_TargetTemp <= gs_Rec.Large) || (gs_Rec.Large == 0))          //��Ͷ��ǰ����Ŀ��ֵ�����Ϊ0��Ϊ��Ͷ��Ч
        gb_Sp1OverFlg = true;
    if ((gu32_TargetTemp <= gs_Rec.Middle) || (gs_Rec.Middle == 0))        //��Ͷ��ǰ����Ŀ��ֵ�����Ϊ0��Ϊ��Ͷ��Ч
        gb_Sp2OverFlg = true;
    
    if (lb_Run)
    {
        gb_FirstStartFlg = false;
        if (gb_LowDefineFlg)       //����λ�ж���,������λ������Ч
        {
            if (gb_LowFlg == false)
            {
                gb_TimeRunFlg = true;
                gu32_TimeBuf = 1;
                return;
            }
        }
        
//        if (gs_Setup.TareMode == BINYES)    //�ж�ģʽ���жϼ�����ϵ�����
//        {
            //��ϼ�����ɺ����¸�ֵ���ϴ�����Ϊ��ֹmodbus����ʱ�޸��䷽����Ŀ��ֵ�͵���Ŀ��ֵ��ƥ���������жϣ���ƥ����ֹͣ
//            if (gu32_WeighTimeCnt == 0 && (gu32_WorkMode == A_ADD || gu32_WorkMode == A_ADD_B))
//            {
//                lu32_WeighTimeTemp = gs_Rec.FlapBagWt / gu32_SingleTarget;
//                            
//                if (lu32_WeighTimeTemp < 1)          //��Ŀ��ֵС�ڵ���Ŀ��ֵ����
//                {
//                    RunStopInit(false);
//                    ErrorDeal(MDP_A, 6, 100);     //��ʾ����6
//                    return;
//                }
//                else if (gs_Rec.FlapBagWt % gu32_SingleTarget != 0)       //��Ŀ��ֵ���ǵ���Ŀ��ֵ������������
//                {
//                    RunStopInit(false);
//                    ErrorDeal(MDP_A, 6, 100);     //��ʾ����6
//                    return;
//                }
//                else if (lu32_WeighTimeTemp == 1)
//                {
//                    if (gu32_WorkMode == A_ADD_B)    //A+Bģʽ����Ŀ��ֵ���ܵ��ڵ���Ŀ��ֵ
//                    {
//                        RunStopInit(false);
//                        ErrorDeal(MDP_A, 6, 100);     //��ʾ����6
//                        return;
//                    }
//                }
//            }
//        }
        Adapt_InitData(&gs_Adapt);   //����Ӧÿ�����д���
        
        if (JudgeAutoStudy())
        {   //���½�����ѧϰ
            AutoStudy_SP2Start();
            return;
        }
        if (gs_AutoStd.u8Process == AUTOSTD_SECEND)
        {   //��ѧϰ�ڶ������� ֱ�ӽ�����Ͷ
            
            gu32_Process = MIDDLEPROCESS;
            gb_TimeRunFlg = false;
            gb_Sp1Flg = false;
            gb_Sp2Flg = true;
            gb_Sp3Flg = true;
            gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
            gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;  //��¼�м�����ʱ��
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = SP2START;
        #endif
        #ifdef DEBUG_T2A
            //��Ͷ��ֹ�Ƚ�ʱ�䲻Ϊ0�������Ͷ�߽�ֹ�Ƚ�ʱ��
            if (gs_Rec.TimeT2a != 0)
            {
                //��¼�мӼ�����ʱ��
                gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;
                gb_Sp1Flg = false;
                gb_Sp2Flg = true;
                gb_Sp3Flg = true;
                gu32_Process = MIDDLEPROCESS;
                gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
                gb_TimeRunFlg = true;        //������ֹ�Ƚ�ʱ��
                gu32_TimeBuf = gs_Rec.TimeT2a * 10;
                gu32_Time1_5 = gu32_Sys10ms;            //��¼��ǰϵͳʱ��
                
                gs_DebugRunTime.u32Sp3TimeTemp = gu32_Sys1ms;  //��¼��������ʱ��
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = SP2START;
            #endif
            }
        #endif
            return;
        }

        //����������Ч�Ե��ж�
        if (gu32_TargetTemp <= gs_Rec.Small)
        {   //���ֵ���ڵ���Ŀ��ֱֵ�ӽ��붨ֵ״̬
            gu32_Process = T3PROCESS;
            gb_TimeRunFlg = true;
            gb_Sp1Flg = false;
            gb_Sp2Flg = false;
            gb_Sp3Flg = false;
            gb_CompFlg = true;           //��ֵ
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T4START;
        #endif
            return;
        }
        else if ((gb_Sp1OverFlg == true) && (gb_Sp2OverFlg == true))
        {   //��Ͷ��Ͷ����Ч������СͶ
            EnterSmallFeed();
        }
        else if (gb_Sp1OverFlg == true)          //ֻ�д�Ͷ��Ч��������Ͷ
        {
            gu32_Process = MIDDLEPROCESS;
            gb_TimeRunFlg = false;
            gb_Sp1Flg = false;
            gb_Sp2Flg = true;
            gb_Sp3Flg = true;
            gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
            gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;  //��¼�м�����ʱ��
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = SP2START;
        #endif
        #ifdef DEBUG_T2A
            //��Ͷ��ֹ�Ƚ�ʱ�䲻Ϊ0�������Ͷ�߽�ֹ�Ƚ�ʱ��
            if (gs_Rec.TimeT2a != 0)
            {
                //��¼�мӼ�����ʱ��
                gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;
                gb_Sp1Flg = false;
                gb_Sp2Flg = true;
                gb_Sp3Flg = true;
                gu32_Process = MIDDLEPROCESS;
                gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
                gb_TimeRunFlg = true;        //������ֹ�Ƚ�ʱ��
                gu32_TimeBuf = gs_Rec.TimeT2a * 10;
                gu32_Time1_5 = gu32_Sys10ms;            //��¼��ǰϵͳʱ��
                
                gs_DebugRunTime.u32Sp3TimeTemp = gu32_Sys1ms;  //��¼��������ʱ��
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = SP2START;
            #endif
            }
        #endif
        }
        else        //��Ͷ��Ч�������Ͷ����Ͷ������Ч��������Ͷʱ���ж�
        {
            gu32_Process = LARGEPROCESS;
            gb_TimeRunFlg = false;
            gb_Sp1Flg = true;
            gb_Sp2Flg = true;
            gb_Sp3Flg = true;
            gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
            if (gb_Sp2OverFlg == true)       //��Ͷ��Ч�ر���Ͷ
                gb_Sp2Flg = false;
            gs_DebugRunTime.u32Sp1TimeTemp = gu32_Sys1ms;  //��¼�������ʱ��
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = SP1START;
        #endif
        }
        
        //��ǰ�������䷽�������ù�ϵ���жϣ����������ĸ����ƹ���
        if ((gu32_Wt >= gu32_TargetTemp - gs_Rec.Small) && (gb_SigWtFlg == false))
        {   //��ǰ�����Ѿ�����Ŀ��ֵ-��ֱ�ӽ��붨ֵ
            gu32_Process = T3PROCESS;
            gb_TimeRunFlg = true;
            gb_Sp1Flg = false;
            gb_Sp2Flg = false;
            gb_Sp3Flg = false;
            gb_CompFlg = true;           //��ֵ
            if (gs_Rec.OUSwitch != 0 || gs_Rec.CorrectFallSwitch != 0)
            {   //��Ƿ���⿪�ش򿪻�������������ش������T3ʱ��
                gu32_TimeBuf = gs_Rec.TimeT3 * 10;        //��������ʱ��
                gu32_Process = T3PROCESS;               //����T3
                gu32_Time1_5 = gu32_Sys10ms;                //��¼��ǰʱ����ΪT3����ʱ��
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = T3START;
            #endif
            }
            else
            {   //��Ƿ�������������ر�������T3��ֱ����T4
                gu32_Process = T4PROCESS;
                gu32_TimeBuf = gs_Rec.TimeT4 * 10;
                gu32_Time1_5 = gu32_Sys10ms;        //ȡ��ǰʱ��
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = T4START;
            #endif
            }
            if (gs_Setup.TareMode == BIN2_A || gs_Setup.TareMode == BIN2_B)
            {   /*˫�ӻ���ģʽ��СͶ���������ж�һ������ж�������״̬�Ƿ���Ч��Ȼ��ı�����ж�����
                  ��ֵ���ֽ����󻹻��ж�һ������״̬��
                  ��Ϊ���ֻ�ڶ�ֵ���ֽ������жϣ���ô����ͬʱ�����Ļ����п��ܵ����Ǳ�ͬʱ�жϵ�����
                  ж��������Ч��Ȼ��ͬʱ������ж�������Ϊ��Ч���������Ӷ����ж���źţ����Ƕ�û�м�
                  �������*/
                if (gb_DiscEnableInFlg)
                    gb_DiscEnableOutFlg = false;
            }
            
            //СͶʱǿ��ֹͣ�������Ĵ�
            gb_AddFlapBagFlg = false;
            gb_FlapWaitFlg = false;
            gb_FlapStartFlg = false;
            gb_FlapBagOutFlg = false;
        }
        else if ((gb_Sp2OverFlg == false) && (gu32_Wt >= gu32_TargetTemp - gs_Rec.Middle) && (gb_SigWtFlg == false))
        {   //��Ͷ������Ч������������Ŀ��-��Ͷ��ǰ����ֱ�ӽ���СͶ
        #ifdef DEBUG_T2A
            //��Ͷ��ֹ�Ƚ�ʱ�䲻Ϊ0�������Ͷ�߽�ֹ�Ƚ�ʱ��
            if (gs_Rec.TimeT2a != 0)
            {
                //��¼�мӼ�����ʱ��
                gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;
                gb_Sp1Flg = false;
                gb_Sp2Flg = true;
                gb_Sp3Flg = true;
                gu32_Process = MIDDLEPROCESS;
                gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
                gb_TimeRunFlg = true;        //������ֹ�Ƚ�ʱ��
                gu32_TimeBuf = gs_Rec.TimeT2a * 10;
                gu32_Time1_5 = gu32_Sys10ms;            //��¼��ǰϵͳʱ��
                
                gs_DebugRunTime.u32Sp3TimeTemp = gu32_Sys1ms;  //��¼��������ʱ��
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = SP2START;
            #endif
            }
            else if (gu32_Process < SMALLPROCESS)
            {
                EnterSmallFeed();
            }
        #else
            if (gu32_Process < SMALLPROCESS)
            {
                EnterSmallFeed();
            }
        #endif
        }
        else if ((gb_Sp1OverFlg == false) && (gu32_Wt >= gu32_TargetTemp - gs_Rec.Large) && (gb_SigWtFlg == false))
        {   //��Ͷ������Ч������������Ŀ��-��Ͷ��ǰ�����������Ͷ����
            if (gu32_Process < MIDDLEPROCESS)
            {
                gu32_Process = MIDDLEPROCESS;
                gb_TimeRunFlg = false;
                gb_Sp1Flg = false;
                gb_Sp2Flg = true;
                gs_DebugRunTime.u32Sp2TimeTemp = gu32_Sys1ms;  //��¼�м�����ʱ��
            #ifdef DEBUG_WT_SEND
                gu8_DebugState2 = SP2START;
            #endif
                //��Ͷ������Ч��ֱ�ӽ���СͶ
                if (gb_Sp2OverFlg == true)
                {
                    EnterSmallFeed();
                }
                gb_Sp3Flg = true;
                gu32_SmallPluseTimeBuf = gu32_Sys10ms;      //СͶ�㶯ʱ�䴦��
            }
        }
    }
}

/*********************************************************************
��������: AutoStudy_SP2Start
��    ��: ��ѧϰ���ܿ�ʼ��ͶͶ��
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-31
��    ��:                      ʱ    ��: 
*********************************************************************/
void AutoStudy_SP2Start(void)
{
    gb_Sp1Flg = false;
    gb_Sp2Flg = true;
    gb_Sp3Flg = true;
    gs_AutoStd.u32StartMoment = gu32_Sys1ms;  //��¼��Ͷ����ʱ��
    gs_AutoStd.u8Process = AUTOSTD_SP2;
    gs_AutoStd.u32SP2Time = gs_Setup.Adapt7_9;      //B7.9������ѧϰ��Ͷ����ʱ��
    gu32_Process = AUTOSTUDYPROCESS;
    Adapt_InitSP3KTemp(&gs_Adapt);  //���СͶ�������K
}

/*********************************************************************
��������: AutoStudy_SP2ing
��    ��: ��ѧϰ��Ͷ���̴���
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-31
��    ��:                      ʱ    ��: 
*********************************************************************/
void AutoStudy_SP2ing(void)
{
    if (gu32_Sys1ms - gs_AutoStd.u32StartMoment >= gs_AutoStd.u32SP2Time)
    {
        gb_Sp1Flg = false;
        gb_Sp2Flg = false;
        gb_Sp3Flg = false;
        gs_AutoStd.u8Process = AUTOSTD_SP2STOP;
        gs_AutoStd.u32StartMoment = gu32_Sys1ms;    //��¼ֹͣ����ʱ��
    }
}

/*********************************************************************
��������: AutoStudy_SP2Stop
��    ��: ��ѧϰ��Ͷֹͣ����
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-31
��    ��:                      ʱ    ��: 
*********************************************************************/
void AutoStudy_SP2Stop(void)
{
    #define STADY_DELAY     2000    //��ѧϰ��Ͷ�����ȴ�ʱ��
    #define STADY_SP3TIME   2000    //��ѧϰʱСͶ����ʱ��
    
    if (gu32_Sys1ms - gs_AutoStd.u32StartMoment >= STADY_DELAY)
    {
        if (gb_StabFlg)
        {   //���Ⱥ��������������
            gs_AutoStd.s32SP2StopResult = gs32_WtKalmanFed;     //��¼��ͶͶ�Ͻ��
            gs_AutoStd.fSP2Flux = (float)gs32_WtKalmanFed / gs_AutoStd.u32SP2Time;   //������Ͷ��������
            gb_Sp1Flg = false;
            gb_Sp2Flg = false;
            gb_Sp3Flg = true;
            gs_AutoStd.u8Process = AUTOSTD_SP3;
            gs_AutoStd.u32SP3Time = STADY_SP3TIME;
            gs_AutoStd.u32StartMoment = gu32_Sys1ms;  //��¼СͶ����ʱ��
        }
    }
    #undef STADY_DELAY
    #undef STADY_SP3TIME
}

/*********************************************************************
��������: AutoStudy_SP3ing
��    ��: ��ѧϰСͶ�������
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-09-01
��    ��:                      ʱ    ��: 
*********************************************************************/
void AutoStudy_SP3ing(void)
{
    if (gu32_Sys1ms - gs_AutoStd.u32StartMoment >= gs_AutoStd.u32SP3Time)
    {
        gb_Sp1Flg = false;
        gb_Sp2Flg = false;
        gb_Sp3Flg = false;
        gs_AutoStd.u8Process = AUTOSTD_SP3STOP;
        gs_AutoStd.s32SP3StopMomentWt = gs32_WtKalmanFed;     //��¼СͶֹͣʱ������
        //ȡ240��(0.5s)�������СͶб��K
        gs_AutoStd.s32SP3K = LeastLinear(gs32_WtFedArray, LINEARARRAY_LENGTH, LINEARARRAY_LENGTH - 240, 2, 0);
        gs_AutoStd.u32StartMoment = gu32_Sys1ms;    //��¼ֹͣ����ʱ��
        gs_Adapt.s32SP3KProb = SP3LinearKProbabilityStatistics(&gs_Adapt, gs_AutoStd.s32SP3K);  //СͶKͳ��
    }
}

/*********************************************************************
��������: AutoStudy_SP3Stop
��    ��: ��ѧϰСͶֹͣ����
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-09-01
��    ��:                      ʱ    ��: 
*********************************************************************/
void AutoStudy_SP3Stop(void)
{
    #define STADY_DELAY     2000    //��ѧϰСͶ�����ȴ�ʱ��
    
    if (gu32_Sys1ms - gs_AutoStd.u32StartMoment >= STADY_DELAY)
    {
        if (gb_StabFlg)
        {   //���Ⱥ��������������
            gs_AutoStd.s32SP3StopResult = gs32_WtKalmanFed;     //��¼��ͶͶ�Ͻ��
            gs_AutoStd.u32SP3Val = gs_AutoStd.s32SP3StopResult - gs_AutoStd.s32SP3StopMomentWt; //������ѧϰ��СͶ���ֵ
            gs_Rec.Small = gs_AutoStd.u32SP3Val;
            FRAM_WriteDW(FMAddr_Small(gu32_RecNum), gs_Rec.Small);  //����СͶ���ֵ
            //�ٴ�����СͶ������ɼ���
            gb_Sp1Flg = false;
            gb_Sp2Flg = false;
            gb_Sp3Flg = true;
            gs_AutoStd.u8Process = AUTOSTD_SP3FILL;
        }
    }
    #undef STADY_DELAY
}

/*********************************************************************
��������: AutoStudy_SP3Fill
��    ��: ��ѧϰСͶ������ɴ���
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-09-01
��    ��:                      ʱ    ��: 
*********************************************************************/
void AutoStudy_SP3Fill(void)
{
    #define FORBIDTIME_25 700   //Ĭ�Ͻ�ֹ�Ƚ�ʱ��
    #define FORBIDTIME_15 750
    #define FORBIDTIME_05 800
    #define STUDY_CONSERVATIVE 0.9  //��ѧϰ��Ͷ���ذٷֱ�
    
    u32 lu32_Temp;
    
    if ((gf_WtKalmanFed >= gu32_TargetTemp - gs_Rec.Small) && (gb_SigWtFlg == false))
    {   //�������
        gb_Sp3Flg = false;
        //������ѧϰ����Ͷʱ�� ��СͶʱ��Ϊ��ֹ�Ƚ�ʱ��+����Ӧ�ж�СͶʱ�����ݵ��ʱ��Ϊ��׼
        //СͶ�ӵ�����
        lu32_Temp = gs_AutoStd.s32SP3K * 480 * (FORBIDTIME_25 + gs_Setup.AdaptSP2Level + gs_Setup.AdaptSP2TimeThreshold) / 1000000;
        //��Ͷ�ӵ�����
        lu32_Temp = (float)(gu32_TargetTemp - lu32_Temp) * STUDY_CONSERVATIVE;
        //��Ͷ�ӵ�ʱ��
        gs_AutoStd.u32SP2Time = (float)lu32_Temp / gs_AutoStd.fSP2Flux;
        gs_DebugRunTime.u32Sp2Time = gs_AutoStd.u32SP2Time; //��ӡ��
        
        gs_AutoStd.u8Process = AUTOSTD_SECEND;
        
        gb_CompFlg = true;           //��ֵ
        gb_TimeRunFlg = true;                    //������ʱ���ֵ����ʱ���Ѿ�����
        if (gs_Rec.OUSwitch != 0 || gs_Rec.CorrectFallSwitch != 0)
        {   //��Ƿ���⿪�ش򿪻�������������ش������T3ʱ��
            gu32_TimeBuf = gs_Rec.TimeT3 * 10;        //��������ʱ��
            gu32_Process = T3PROCESS;               //����T3
            gu32_Time1_5 = gu32_Sys10ms;                //��¼��ǰʱ����ΪT3����ʱ��
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T3START;
        #endif
        }
        else
        {   //��Ƿ�������������ر�������T3��ֱ����T4
            gu32_Process = T4PROCESS;
            gu32_TimeBuf = gs_Rec.TimeT4 * 10;
            gu32_Time1_5 = gu32_Sys10ms;        //ȡ��ǰʱ��
        #ifdef DEBUG_WT_SEND
            gu8_DebugState2 = T4START;
        #endif
        }
    }
}

/*********************************************************************
��������: JudgeAutoStudy
��    ��: �ж��Ƿ�ִ����ѧϰ����
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: true=ִ����ѧϰ false=��ִ��
��    ��: �Ժ���               ʱ    ��: 2015-09-01
��    ��:                      ʱ    ��: 
*********************************************************************/
bool JudgeAutoStudy(void)
{
    if (gs_Rec.Middle == 0 && gs_Rec.Small == 0)
    {   //��Ͷ��СͶ��ǰ����Ϊ0 ����û�����й���ѧϰ �����½�����ѧϰ
        return true;
    }
    else
        return false;
}

/*********************************************************************
��������: AutoStudy
��    ��: ��ѧϰ���̿���
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-31
��    ��:                      ʱ    ��: 
*********************************************************************/
void AutoStudy(void)
{
    switch (gs_AutoStd.u8Process)
    {
        case AUTOSTD_SP2:
            AutoStudy_SP2ing();
            break;
        case AUTOSTD_SP2STOP:
            AutoStudy_SP2Stop();
            break;
        case AUTOSTD_SP3:
            AutoStudy_SP3ing();
            break;
        case AUTOSTD_SP3STOP:
            AutoStudy_SP3Stop();
            break;
        case AUTOSTD_SP3FILL:
            AutoStudy_SP3Fill();
            break;
        default:
            break;
    }
}

/*********************************************************************
��������: void Cmpul(void)
��    ��: ���Ϲ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void Cmpul(void)
{
    switch(gu32_Process)
    {
        case T1PROCESS:             //������ʱ
            WaitT1();
            break;
        case AUTOSTUDYPROCESS:      //��ѧϰ����
            AutoStudy();
            break;
        case LARGEPROCESS:          //��Ͷ
            LargeFeed();
            break;
        case MIDDLEPROCESS:         //��Ͷ
            MiddleFeed();
            break;
        case SMALLPROCESS:          //СͶ
            SmallFeed();
            break;
        case T3PROCESS:             //��Ƿ������ʱ
            WaitT3();
            break;
        case T4PROCESS:             //��ֵ�������
            WaitT4();
            break;
        case FALLPROCESS:           //ж��
            Disc();
            break;
        case T5PROCESS:             //ж����ʱ
            WaitT5();
            break;
        default:
            gu32_Process = 0;
            break;
    }

    if (gb_AlarmFlg || gb_AdOKFlg == false)
    {
        gb_Sp1Flg = false;
        gb_Sp2Flg = false;
        gb_Sp3Flg = false;
        gb_DiscFlg = false;
    }
}

/*********************************************************************
��������: void Key_RunState(void)
��    ��: �Ǳ�����ʱ�Ĵ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2014-1-8
��    ��:                      ʱ    ��: 
*********************************************************************/
void Key_RunState(void)
{
    u8 i;
    
    if (gb_SumFlg)
    {
        Key_SumState();
    }
    else if (gb_FastSetFlg)              //��������״̬�µİ�������
    {
        Key_FastSetState();
    }
    else
    {
        if (gb_AlarmFlg)     //����״̬��ֻ�ܰ�ESC ENTER ZERO
        {
            if ((gu32_KeyCode != K_ZERO) && (gu32_KeyCode != K_ESC) && (gu32_KeyCode != K_ENTER) && (gu32_KeyCode != K_NONE))
                return;
        }
        switch (gu32_KeyCode)
        {
            case K_6:   //���а�SUM���鿴�ۼƴ������ۼ�����
                 ulParameter.ul = 0xFFFFFFFF;
                gu8_Item = 0;
                gu8_DataInLength = 0;
                gb_SetFlg = false;
                gb_FirstNumKeyFlg = true;
                gb_FlashFlg = false;
                gu32_KeyCode = 0;
                if(gs_Setup.SumCheckCodeSwitch != 0)   //�Զ������뿪�ش�
                {
                    gu8_Item = 0;
                    gb_SumFlg = true;
                }
                else
                {
                    ulParameter.ul = gu32_SumWt;
                    gu8_Item = 1;
                    gb_SumFlg = true;
                }
                Key_SumState();
                break;
            case K_ENTER:
                ulParameter.ul = 0xFFFFFFFF;
                gu8_Item = 0;
                gu8_DataInLength = 0;
                gb_SetFlg = false;
                gb_FirstNumKeyFlg = true;
                gb_FlashFlg = false;
                
                gb_FastSetFlg = true;
                gu32_KeyCode = 0;
                gu8_Item = REC_F1_1;
                gu8_FastSetSigTemp = 0;
                for (i = 0; i <= 5; i++)        //��F1.x���뵽���������ݴ�
                    gu32_FastSetTemp[i] = GetRecipeItemValue(REC_F1_1 + i);
                ulParameter.ul = gu32_FastSetTemp[gu8_Item-3];
                break;
            case K_ESC:
                Clr_Alarm();                //�屨��
                if (gb_GoFlg)   //�ֶ�����һ��ʱ��ESC��ֹͣ
                    RunStopInit(false);
                break;
            default:
                break;
        #ifdef DEBUG_RUN_STOP_BY_KEY
            case K_RIGHT:     //������ͣ
                Clr_Alarm();
                RunStopInit(false);
                break;
        #endif
        }
    }
    
    if (gu32_SystemState == RUNSTATE)
    {
        Cmpul();            //���Ϲ��̴���
    }
    
    if(gb_OUOverFlg)
    {
        if(gu32_Sys10ms - gu32_OUDelayTimeBuf >= 200)
            gb_OUOverFlg = false;
    }
    if(gb_OUUnderFlg)
    {
        if(gu32_Sys10ms - gu32_OUDelayTimeBuf >= 200)
            gb_OUUnderFlg = false;
    }
}


/*********************************************************************
��������: bool StabOver(bool lb_Reset)
��    ��: �Ǳ��Ƿ����ȳ�ʱ
˵    ��: 
��ڲ���: lb_Reset ���»�ȡ�ж�ʱ��
�� �� ֵ: ���ȳ�ʱʱ�䵽
��    ��: �Ժ���               ʱ    ��: 2014-2-14
��    ��:                      ʱ    ��: 
*********************************************************************/
bool StabOver(bool lb_Reset)
{
#ifdef DEBUG_JUDESTABLE_TIME
    return false;
#else
    static bool sb_TimeGetedFlg = false;
    static u32 su32_TimeTemp;
    
    if (lb_Reset)
    {
        sb_TimeGetedFlg = false;
        return false;
    }
    if (gs_Setup.JudgeStabOverTime == 0) //���ȳ�ʱʱ������Ϊ0�ù��ܲ�������
        return false;
    
    if (sb_TimeGetedFlg == false)
    {
        sb_TimeGetedFlg = true;
        su32_TimeTemp = gu32_Sys10ms;
    }
    if (gu32_Sys10ms - su32_TimeTemp >= gs_Setup.JudgeStabOverTime * 10)
    {   //���ȳ�ʱʱ�䵽
        ErrorDeal(7, 70);        //error7 ���ȳ�ʱʱ�䵽���ǳ��廹����
        return true;
    }
    else
        return false;
#endif
}
