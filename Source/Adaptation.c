/*************************************************
Project:                GM8804C-2_ARM
File name:              Adaptation.c
Description:            ��������Ӧ��ع���
Author:                 �Ժ���
Version:                1.0
Date:                   2015-8-19
Others:                 
History:                2015-8-21���ú���������������Ӧ��ع��ܣ�Ŀǰֻ���������٣�Ĭ����Ͷ��СͶ����Ӧ
*************************************************/

#include "Adaptation.h"
#include "RunState.h"
#include "Recipe.h"
#include "ADtoWeight.h"
#include "F_RAM.h"
#include "SetupState.h"
#include "MathLogic.h"

#define AD_SPEED        480                     //ADת������
#define ADAPT_SP3_THRESHOLD_TIME        100      //СͶ��������ֵʱ��(ms) СͶʱ�� <= ����ʱ��+��ֵ ������
//#define ADAPT_RUSHWT_SAMPLE_TIME        100     //��Ͷ����������ʱ�� ����ʱ����������ms
#define ADAPT_SP12_THRESHOLD_LIMIT      700     //��Ͷ�����ж�ʱ СͶ��Сʱ�������

#define SLFALLWT    7       //СͶ��Kֵ�������鳤��


//����Ӧ�ṹ����ز�����ʼ��
//��ֲʱ������ָ���ַ
struct_adapt_t gs_Adapt =
{
    &(gs32_WtKalmanFed),            // ë�ذ�װʱ��ָ��ë������
    &(gs_Rec.Middle),               // ָ����Ͷ��ǰ��������ʱ�ã� 
    &(gs_Rec.Small),                // ָ�������ǰ��������ʱ�ã� 
    &(gs_DebugRunTime.u32Sp3Time),  // ָ��ʵ�ʲ�õ�СͶʱ��
    &(gs_DebugRunTime.u32Sp2Time),  // ָ��ʵ�ʲ�õ���Ͷʱ��
    &(gu32_T2bTime),                //ָ��СͶ����ʱ��
    &(gs_DebugRunTime.s32SP3RealStopWt),   //ʵ��СͶֹͣ����
    &(gs_DebugRunTime.s32SP2RealStopWt),   //ʵ����Ͷֹͣ����
    &(gs_PredictSp3.s32KThisTime),  //ָ�򱾴�СͶ���̵��������Kֵ
    &(gs32_SP2KFed),                //������Ͷ��ֹ�㸽�����������Kֵ
    
};

/*********************************************************************
��������: Adapt_InitData
��    ��: ����Ӧ������ز�����ʼ��
˵    ��: ��ֲʱ��� ÿ�μ���ǰ������
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void Adapt_InitData(struct_adapt_t *pfs_Adapt)
{
//    const u32 u32ScopeValue[] = {0, 25, 50, 100};         // �����������
//    const u32 u32MinTime[] = {50, 100, 150, 200, 250, 300, 350, 400};    // ��/��Ͷ�����ȼ���Ӧ��СͶʱ��

    pfs_Adapt->u32AdatpEn         = gs_Setup.AdaptSW;
    pfs_Adapt->s32RushWtLow       = 0;
    pfs_Adapt->s32RushWtMiddle = 0;        //�������е�
    pfs_Adapt->s32RushWtHigh = 0;          //�������ߵ�
    pfs_Adapt->s32RushWtLowProb = 0;       //ͳ�ƹ������͵�
    pfs_Adapt->s32RushWtMiddleProb = 0;    //ͳ�ƹ������е�
    pfs_Adapt->s32RushWtHighProb = 0;      //ͳ�ƹ������ߵ�

    pfs_Adapt->u32SP2En           = gs_Setup.AdaptSP2SW;
    pfs_Adapt->u32SP2Range        = gs_Setup.AdaptSP2LimitRng;
    pfs_Adapt->u32SP2ThresholdMin = gs_Setup.AdaptSP2Level;
    pfs_Adapt->u32SP2ThresholdVal = gs_Setup.AdaptSP2TimeThreshold;
    pfs_Adapt->u32SP2Scope        = gs_Setup.AdaptSP2Scope;
    
    pfs_Adapt->s32ConfirmWt      = 0;
    pfs_Adapt->s32TargetVal      = gs_Rec.Target;

    pfs_Adapt->u32SP3En          = gs_Setup.AdaptSP3SW;
    pfs_Adapt->u32SP3Range       = gs_Setup.AdaptSP3LimitRng;
    pfs_Adapt->u32SP3Scope       = gs_Setup.AdaptSP3Scope;
}

/*********************************************************************
��������: SaveSP2Value
��    ��: ������Ͷ��ǰ��������
˵    ��: ��ֲʱ���ú���
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void SaveSP2Value(struct_adapt_t *pfs_Adapt)
{
    pfs_Adapt->s32SP2Bkp = (*pfs_Adapt->ps32SP2);   //���±���ֵ
    FRAM_WriteDW(FMAddr_Middle(gu32_RecNum), (*pfs_Adapt->ps32SP2));
}

/*********************************************************************
��������: void SaveSP3Value(s32 fs32_Value)
��    ��: ����СͶ��ǰ��������
˵    ��: ��ֲʱ���ú���
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void SaveSP3Value(struct_adapt_t *pfs_Adapt)
{
    pfs_Adapt->s32SP3Bkp = (*pfs_Adapt->ps32SP3);   //���±���ֵ
    FRAM_WriteDW(FMAddr_Small(gu32_RecNum), (*pfs_Adapt->ps32SP3));
}

//��ʼ��СͶK���
void Adapt_InitSP3KTemp(struct_adapt_t *pfs_Adapt)
{
    pfs_Adapt->u32SP3KCnt = 0;
    pfs_Adapt->s32SP3KProb = 0;
}

void Adapt_InitSP2Temp(struct_adapt_t *pfs_Adapt)
{
    pfs_Adapt->s32SP2Bkp = (*pfs_Adapt->ps32SP2);
    pfs_Adapt->u32SP2Count = 0;
    pfs_Adapt->u32SP2SubCount = 0;
}


void Adapt_InitSP3Temp(struct_adapt_t *pfs_Adapt)
{
    pfs_Adapt->s32SP3Bkp = (*pfs_Adapt->ps32SP3);
    pfs_Adapt->u32SP3Count = 0;
}

/*********************************************************************
��������: Adapt_InitDataFirstRun
��    ��: ����Ӧ������ز�����ʼ��
˵    ��: ����������ʱ����һ��
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-21
��    ��:                      ʱ    ��: 
*********************************************************************/
void Adapt_InitDataFirstRun(struct_adapt_t *pfs_Adapt)
{
    pfs_Adapt->bFirstIn = true;
//    pfs_Adapt.s32SP2Bkp = (*pfs_Adapt.ps32SP2);
//    pfs_Adapt.s32SP3Bkp = (*pfs_Adapt.ps32SP3);
    Adapt_InitSP2Temp(pfs_Adapt);
    Adapt_InitSP3Temp(pfs_Adapt);
}

/*********************************************************************
��������: BubbleSort
��    ��: ð������
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-19
��    ��:                      ʱ    ��: 
*********************************************************************/
void BubbleSort(s32 *pfs32_Array, u32 n)
{ 
    u32 i, j; 
    s32 temp;

    for(i = 0; i < n - 1; i++) 
    { 
        for(j = 0; i + j < n - 1; j++) 
        { 
            if(pfs32_Array[j] > pfs32_Array[j + 1]) 
            { 
                temp = pfs32_Array[j]; 
                pfs32_Array[j] = pfs32_Array[j + 1]; 
                pfs32_Array[j + 1] = temp; 
            } 
        } 
    } 
}

/*********************************************************************
��������: BubbleSortFloat
��    ��: ð������ float����
˵    ��: 
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-19
��    ��:                      ʱ    ��: 
*********************************************************************/
void BubbleSortFloat(float *pff_Array, u32 n)
{ 
    u32 i, j; 
    s32 temp;

    for(i = 0; i < n - 1; i++) 
    { 
        for(j = 0; i + j < n - 1; j++) 
        { 
            if(pff_Array[j] > pff_Array[j + 1]) 
            { 
                temp = pff_Array[j]; 
                pff_Array[j] = pff_Array[j + 1]; 
                pff_Array[j + 1] = temp; 
            } 
        } 
    } 
}

/*********************************************************************
��������: SP2LinearKProbabilityStatistics
��    ��: ����ͳ�Ʒ�������СͶ�������Kֵ
˵    ��: �ú���һ������FallWtProbabilityStatistics����ǰ��
          ��ΪFallWtProbabilityStatistics�������ж�pfs_Adapt->u32SP3Count++�Ĳ���
��ڲ���: pfs_Adapt=����Ӧ�����ṹ��
          fs32_KThisTime=ʵ�ʵõ��ı���Kֵ
���ڲ���: 
�� �� ֵ: ͳ�ƴ�����Kֵ
��    ��: �Ժ���               ʱ    ��: 2015-8-25
��    ��:                      ʱ    ��: 
*********************************************************************/
s32 SP2LinearKProbabilityStatistics(struct_adapt_t *pfs_Adapt, s32 fs32_KThisTime)
{
    s32 ls32_Result = 0;
    u32 i = 0;
    static s32 lss32_AddFallWTemp = 0;
    static s32 lss32_KRecord[SLFALLWT];
    static s32 lss32_K_Pro[SLFALLWT];
        
    if(pfs_Adapt->u32SP3Count>(SLFALLWT-1))
    {
         for(i=0;i<(SLFALLWT-1);i++)
         {
            lss32_KRecord[i] = lss32_KRecord[i+1];
            lss32_K_Pro[i] = lss32_KRecord[i];
         }
         lss32_KRecord[SLFALLWT-1] = fs32_KThisTime;
         lss32_K_Pro[SLFALLWT-1] = fs32_KThisTime;
    
         BubbleSort(lss32_K_Pro, SLFALLWT); //ð������
         ls32_Result = (lss32_K_Pro[1]+lss32_K_Pro[2]*3+lss32_K_Pro[3]*5+lss32_K_Pro[4]*3+lss32_K_Pro[5])/13;  //����ͳ�Ƴ����ֵƫ��
    }  
    else if(pfs_Adapt->u32SP3Count>=2)
    {
         lss32_KRecord[pfs_Adapt->u32SP3Count] = fs32_KThisTime;
         for(i=0;i<=pfs_Adapt->u32SP3Count;i++)
         {
             lss32_K_Pro[i] = lss32_KRecord[i];
         }       
         BubbleSort(lss32_K_Pro, pfs_Adapt->u32SP3Count + 1); //ð������
         if(pfs_Adapt->u32SP3Count>=4)
         {
             ls32_Result = (lss32_K_Pro[1]+lss32_K_Pro[2]*2+lss32_K_Pro[3])/4;
         }
         else if(pfs_Adapt->u32SP3Count>=3)
         {
             ls32_Result = (lss32_K_Pro[1]+lss32_K_Pro[2])/2;
         }
         else if(pfs_Adapt->u32SP3Count>=2)
         {
             ls32_Result = lss32_K_Pro[1];
         }       
//         pfs_Adapt->u32SP3Count++;
    }
    else
    {
         if(pfs_Adapt->u32SP3Count==0) //����Ǹ�����������
         {
            lss32_AddFallWTemp = 0;
         }
         lss32_KRecord[pfs_Adapt->u32SP3Count] = fs32_KThisTime;
//         pfs_Adapt->u32SP3Count++;       
         lss32_AddFallWTemp += fs32_KThisTime;
         ls32_Result = lss32_AddFallWTemp/(pfs_Adapt->u32SP3Count + 1);       
    }
    
    return ls32_Result;
}


/*********************************************************************
��������: SP2FluxProbabilityStatistics
��    ��: ����ͳ�Ʒ�������СͶ�������Kֵ
˵    ��: �ú���һ������FallWtProbabilityStatistics����ǰ��
          ��ΪFallWtProbabilityStatistics�������ж�pfs_Adapt->u32SP3Count++�Ĳ���
��ڲ���: pfs_Adapt=����Ӧ�����ṹ��
          ff_SP2Flux=���μ�����Ͷ����
���ڲ���: 
�� �� ֵ: ͳ�ƴ�������Ͷ����
��    ��: �Ժ���               ʱ    ��: 2015-8-25
��    ��:                      ʱ    ��: 
*********************************************************************/
float SP2FluxProbabilityStatistics(struct_adapt_t *pfs_Adapt, float ff_SP2Flux)
{
    float lf_Result = 0;
    u32 i = 0;
    static float lsf_AddFallWTemp = 0;
    static float lsf_FluxRecord[SLFALLWT];
    static float lsf_FluxPro[SLFALLWT];
        
    if(pfs_Adapt->u32SP3Count>(SLFALLWT-1))
    {
         for(i=0;i<(SLFALLWT-1);i++)
         {
            lsf_FluxRecord[i] = lsf_FluxRecord[i+1];
            lsf_FluxPro[i] = lsf_FluxRecord[i];
         }
         lsf_FluxRecord[SLFALLWT-1] = ff_SP2Flux;
         lsf_FluxPro[SLFALLWT-1] = ff_SP2Flux;
    
         BubbleSortFloat(lsf_FluxPro, SLFALLWT); //ð������
         lf_Result = (lsf_FluxPro[1]+lsf_FluxPro[2]*3+lsf_FluxPro[3]*5+lsf_FluxPro[4]*3+lsf_FluxPro[5])/13;  //����ͳ�Ƴ����ֵƫ��
    }  
    else if(pfs_Adapt->u32SP3Count>=2)
    {
         lsf_FluxRecord[pfs_Adapt->u32SP3Count] = ff_SP2Flux;
         for(i=0;i<=pfs_Adapt->u32SP3Count;i++)
         {
             lsf_FluxPro[i] = lsf_FluxRecord[i];
         }       
         BubbleSortFloat(lsf_FluxPro, pfs_Adapt->u32SP3Count + 1); //ð������
         if(pfs_Adapt->u32SP3Count>=4)
         {
             lf_Result = (lsf_FluxPro[1]+lsf_FluxPro[2]*2+lsf_FluxPro[3])/4;
         }
         else if(pfs_Adapt->u32SP3Count>=3)
         {
             lf_Result = (lsf_FluxPro[1]+lsf_FluxPro[2])/2;
         }
         else if(pfs_Adapt->u32SP3Count>=2)
         {
             lf_Result = lsf_FluxPro[1];
         }       
//         pfs_Adapt->u32SP3Count++;
    }
    else
    {
         if(pfs_Adapt->u32SP3Count==0) //����Ǹ�����������
         {
            lsf_AddFallWTemp = 0;
         }
         lsf_FluxRecord[pfs_Adapt->u32SP3Count] = ff_SP2Flux;
//         pfs_Adapt->u32SP3Count++;       
         lsf_AddFallWTemp += ff_SP2Flux;
         lf_Result = lsf_AddFallWTemp/(pfs_Adapt->u32SP3Count + 1);       
    }
    
    return lf_Result;
}

/*********************************************************************
��������: SP3LinearKProbabilityStatistics
��    ��: ����ͳ�Ʒ�������СͶ�������Kֵ
˵    ��: 
��ڲ���: pfs_Adapt=����Ӧ�����ṹ��
          fs32_KThisTime=ʵ�ʵõ��ı���Kֵ
���ڲ���: 
�� �� ֵ: ͳ�ƴ�����Kֵ
��    ��: �Ժ���               ʱ    ��: 2015-8-25
��    ��:                      ʱ    ��: 
*********************************************************************/
s32 SP3LinearKProbabilityStatistics(struct_adapt_t *pfs_Adapt, s32 fs32_KThisTime)
{
    #define DEFAULT_SP3K    1000    //������KΪ0ʱ Ĭ�ϸ�ֵ��ΪKֵ
    
    s32 ls32_Result = 0;
    u32 i = 0;
    static s32 lss32_AddFallWTemp = 0;
    static s32 lss32_KRecord[SLFALLWT];
    static s32 lss32_K_Pro[SLFALLWT];
    //СͶKֵû�вɼ��� ������ΪСͶû�дﵽ�ɼ���ʱ�� ��Ĭ�����ϴμ����Kֵ
    if (fs32_KThisTime == 0)    
    {
        if (pfs_Adapt->u32SP3KCnt == 0)
            fs32_KThisTime = DEFAULT_SP3K;
        else
            fs32_KThisTime = pfs_Adapt->s32SP3KProb;
    }
        
    if(pfs_Adapt->u32SP3KCnt>(SLFALLWT-1))
    {
         for(i=0;i<(SLFALLWT-1);i++)
         {
            lss32_KRecord[i] = lss32_KRecord[i+1];
            lss32_K_Pro[i] = lss32_KRecord[i];
         }
         lss32_KRecord[SLFALLWT-1] = fs32_KThisTime;
         lss32_K_Pro[SLFALLWT-1] = fs32_KThisTime;
    
         BubbleSort(lss32_K_Pro, SLFALLWT); //ð������
         ls32_Result = (lss32_K_Pro[1]+lss32_K_Pro[2]*3+lss32_K_Pro[3]*5+lss32_K_Pro[4]*3+lss32_K_Pro[5])/13;  //����ͳ�Ƴ����ֵƫ��
    }  
    else if(pfs_Adapt->u32SP3KCnt>=2)
    {
         lss32_KRecord[pfs_Adapt->u32SP3KCnt] = fs32_KThisTime;
         for(i=0;i<=pfs_Adapt->u32SP3KCnt;i++)
         {
             lss32_K_Pro[i] = lss32_KRecord[i];
         }       
         BubbleSort(lss32_K_Pro, pfs_Adapt->u32SP3KCnt + 1); //ð������
         if(pfs_Adapt->u32SP3KCnt>=4)
         {
             ls32_Result = (lss32_K_Pro[1]+lss32_K_Pro[2]*2+lss32_K_Pro[3])/4;
         }
         else if(pfs_Adapt->u32SP3KCnt>=3)
         {
             ls32_Result = (lss32_K_Pro[1]+lss32_K_Pro[2])/2;
         }
         else if(pfs_Adapt->u32SP3KCnt>=2)
         {
             ls32_Result = lss32_K_Pro[1];
         }       
         pfs_Adapt->u32SP3KCnt++;
    }
    else
    {
         if(pfs_Adapt->u32SP3KCnt==0) //����Ǹ�����������
         {
            lss32_AddFallWTemp = 0;
         }
         lss32_KRecord[pfs_Adapt->u32SP3KCnt] = fs32_KThisTime;
         pfs_Adapt->u32SP3KCnt++;       
         lss32_AddFallWTemp += fs32_KThisTime;
         ls32_Result = lss32_AddFallWTemp/(pfs_Adapt->u32SP3KCnt + 1);       
    }
    
    return ls32_Result;
    
    #undef DEFAULT_SP3K
}

/*********************************************************************
��������: FallWtProbabilityStatistics
��    ��: ����ͳ�Ʒ����������ֵ
˵    ��: ���ø���ͳ�ƴ�������������������������������
��ڲ���: pfs_Adapt : ����Ӧ�����ṹ��
          fs32_RealFallWt : ʵ�ʵõ������
���ڲ���: 
�� �� ֵ: ͳ�ƴ��������ֵ
��    ��: �Ժ���               ʱ    ��: 2015-08-19
��    ��:                      ʱ    ��: 
*********************************************************************/
s32 FallWtProbabilityStatistics(struct_adapt_t *pfs_Adapt, s32 fs32_RealFallWt)
{
    s32 ls32_BestFall = 0;
    u32 i = 0;
    static s32 lss32_AddFallWTemp = 0;
    static s32 lss32_FallWRecord[SLFALLWT];
    static s32 lss32_FallW_Pro[SLFALLWT];
    
    if(pfs_Adapt->u32SP3Count>(SLFALLWT-1))
    {
         for(i=0;i<(SLFALLWT-1);i++)
         {
            lss32_FallWRecord[i] = lss32_FallWRecord[i+1];
            lss32_FallW_Pro[i] = lss32_FallWRecord[i];
         }
         lss32_FallWRecord[SLFALLWT-1] = fs32_RealFallWt;
         lss32_FallW_Pro[SLFALLWT-1] = fs32_RealFallWt;
    
         BubbleSort(lss32_FallW_Pro, SLFALLWT); //ð������
         ls32_BestFall = (lss32_FallW_Pro[1]+lss32_FallW_Pro[2]*3+lss32_FallW_Pro[3]*5+lss32_FallW_Pro[4]*3+lss32_FallW_Pro[5])/13;  //����ͳ�Ƴ����ֵƫ��
    }  
    else if(pfs_Adapt->u32SP3Count>=2)
    {
         lss32_FallWRecord[pfs_Adapt->u32SP3Count] = fs32_RealFallWt;
         for(i=0;i<=pfs_Adapt->u32SP3Count;i++)
         {
             lss32_FallW_Pro[i] = lss32_FallWRecord[i];
         }       
         BubbleSort(lss32_FallW_Pro, pfs_Adapt->u32SP3Count + 1); //ð������
         if(pfs_Adapt->u32SP3Count>=4)
         {
             ls32_BestFall = (lss32_FallW_Pro[1]+lss32_FallW_Pro[2]*2+lss32_FallW_Pro[3])/4;
         }
         else if(pfs_Adapt->u32SP3Count>=3)
         {
             ls32_BestFall = (lss32_FallW_Pro[1]+lss32_FallW_Pro[2])/2;
         }
         else if(pfs_Adapt->u32SP3Count>=2)
         {
             ls32_BestFall = lss32_FallW_Pro[1];
         }       
         pfs_Adapt->u32SP3Count++;
    }
    else
    {
         if(pfs_Adapt->u32SP3Count==0) //����Ǹ�����������
         {
            lss32_AddFallWTemp = 0;
         }
         lss32_FallWRecord[pfs_Adapt->u32SP3Count] = fs32_RealFallWt;
         pfs_Adapt->u32SP3Count++;       
         lss32_AddFallWTemp += fs32_RealFallWt;
         ls32_BestFall = lss32_AddFallWTemp/pfs_Adapt->u32SP3Count;       
    }
    
    return ls32_BestFall;
   
}

/*********************************************************************
��������: ModifyFallWeight
��    ��: �����������
˵    ��: 
��ڲ���: RealFallweightSub=��ʵ�����������Ĳ�ֵ
          *pfs_Adapt ����Ӧ�����ṹ��
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-19
��    ��:                      ʱ    ��: 
*********************************************************************/
s32 ModifyFallWeight(s32 RealFallweightSub, struct_adapt_t *pfs_Adapt)
{
    s32 ls32_ModifyFall = 0;
    s32 ls32_RealFallweightTemp = 0;
    //�����ж����������Χ
    ls32_RealFallweightTemp = pfs_Adapt->s32TargetVal * (s32)pfs_Adapt->u32SP3Range / (s32)1000;
    if(RealFallweightSub > ls32_RealFallweightTemp)  //�޷�
    {
        RealFallweightSub = ls32_RealFallweightTemp;
    }   
    else if(RealFallweightSub < (0 - ls32_RealFallweightTemp))   //�޷�
    {
        RealFallweightSub = 0 - ls32_RealFallweightTemp;
    }   
    ls32_ModifyFall = (RealFallweightSub * ((s32)pfs_Adapt->u32SP3Scope) / (s32)100);

    return ls32_ModifyFall;
}


/*********************************************************************
��������: Adapt_SP3
��    ��: СͶ��ǰ������Ӧ
˵    ��: 
��ڲ���: *pfs_Adapt ����Ӧ�����ṹ��
���ڲ���: 
�� �� ֵ: 1=��� 0=δ���
��    ��: �Ժ���               ʱ    ��: 2015-08-19
��    ��:                      ʱ    ��: 
*********************************************************************/
u32 Adapt_SP3(struct_adapt_t *pfs_Adapt)
{
    s32  ls32_Temp = 0;       // ��ʱ����
    s32  ls32_FallTemp = 0;   // ���ֵ��ʱ����
    
    if((0 == pfs_Adapt->u32SP3En) || (0 == pfs_Adapt->u32SP3Range))   // ���عر� / ������ΧΪ0 ������
    {
        Adapt_InitSP3Temp(pfs_Adapt);
    }
    else
    {
        if(pfs_Adapt->s32SP3Bkp != (*pfs_Adapt->ps32SP3)) // ���޸�
        {
            Adapt_InitSP3Temp(pfs_Adapt);
        }
        else if((*pfs_Adapt->pu32SP3RealTime) <= ((*pfs_Adapt->pu32SP3ForbidTime) + ADAPT_SP3_THRESHOLD_TIME))
        {   // СͶʱ�� <= ����ʱ��+��ֵ  ������
            // ����
        }
        else
        {
            if(0 == pfs_Adapt->s32ConfirmWt)   // δ���й�Ƿ��ϻ���ٶ�ֵǰ10��ѧϰ�Ķ�ֵ����
            {
                pfs_Adapt->s32ConfirmWt = (*pfs_Adapt->ps32Weight);
            }
            //����ͳ�Ƶó����ֵ
            ls32_Temp = FallWtProbabilityStatistics(pfs_Adapt, (pfs_Adapt->s32ConfirmWt - (*pfs_Adapt->ps32SP3RealWt)));
            pfs_Adapt->u32SP3Prob = ls32_Temp;  //ͳ�����ֵ ��ӡ��
            ls32_Temp -= *pfs_Adapt->ps32SP3;
            ls32_FallTemp = ModifyFallWeight(ls32_Temp, pfs_Adapt);
            pfs_Adapt->s32SP3AdaptVal = ls32_FallTemp; //����ֵ ��ӡ��
            *pfs_Adapt->ps32SP3 += ls32_FallTemp;
            SaveSP3Value(pfs_Adapt);  //�������ֵ������
        }
    }
    return 1;
}


/*********************************************************************
��������: SampleRushWt
��    ��: �ɼ���ӻ���СͶ����ʱ�����С���е㡢��� ������������
˵    ��: ��ֲʱ���ú������ڽ���ʱ��֮��
��ڲ���: *pfs_Adapt ����Ӧ�����ṹ��
���ڲ���: 
�� �� ֵ: true=��Ͷ�������ɼ����
��    ��: �Ժ���               ʱ    ��: 2015-08-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void SampleRushWt(struct_adapt_t *pfs_Adapt)
{
    if((*pfs_Adapt->pu32SP3RealTime) <= ((*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin))  
    {   //С�ڵ��ڽ���ʱ��+�ж�������Ͷ��СͶ���к���Сʱ��
        pfs_Adapt->s32RushWtLow = (*pfs_Adapt->ps32Weight) - (*pfs_Adapt->ps32SP2RealWt);//��ǰ���� - ��Ͷ��ֹ��ʵ������;
//        pfs_Adapt->u32SP2ResultWt = (*pfs_Adapt->ps32Weight);
    }
    if ((*pfs_Adapt->pu32SP3RealTime) <= ((*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin + pfs_Adapt->u32SP2ThresholdVal / 2))
    {   //С�ڵ��ڽ���ʱ��+�ж�������Ͷ��СͶ���к���Сʱ������ʱ����е�ʱ��
        pfs_Adapt->s32RushWtMiddle = (*pfs_Adapt->ps32Weight) - (*pfs_Adapt->ps32SP2RealWt);//��ǰ���� - ��Ͷ��ֹ��ʵ������;
    }
    if ((*pfs_Adapt->pu32SP3RealTime) <= ((*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin + pfs_Adapt->u32SP2ThresholdVal))
    {   //С�ڵ��ڽ���ʱ��+�ж�������Ͷ��СͶ���к����ʱ��
        pfs_Adapt->s32RushWtHigh = (*pfs_Adapt->ps32Weight) - (*pfs_Adapt->ps32SP2RealWt);//��ǰ���� - ��Ͷ��ֹ��ʵ������;
    }
    
    
//    if(false == pfs_Adapt->bFirstIn) // �������һ������
//    {
//        if(gstrRunResult.u32MidFeedTime > 0)    // ʵ����Ͷʱ�� > 0 
//        {
//            //���Ĭ������Ͷ
//            if((*pfs_Adapt->pu32SP3RealTime) <= ((*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin))  
//            {   //С�ڵ��ڽ���ʱ��+�ж�������Ͷ��СͶ���к���Сʱ��
//                pfs_Adapt->s32RushWtLow = (*pfs_Adapt->ps32Weight) - (*pfs_Adapt->ps32SP2RealWt);//��ǰ���� - ��Ͷ��ֹ��ʵ������;
//            }
//        }
//        else if(gstrRunResult.u32FastFeedTime > 0) // ʵ�ʴ�Ͷʱ�� > 0 
//        {
//            if(gstrRunResult.pu32SP3RealTime == gstrRunResult.u32SP3RealForbTime)//ʵ��СͶʱ����ڽ���ʱ��
//            {
//                pfs_Adapt.s32RushWtLow = ��ǰ���� - ��Ͷ��ֹ��ʵ������;
//            }
//            else if(gstrRunResult.pu32SP3RealTime == (gstrRunResult.u32SP3RealForbTime + 100))//���ڽ���ʱ��+100
//            {
//                pfs_Adapt.s32RushWtLow = ��ǰ���� - ��Ͷ��ֹ��ʵ������;
//            }
//            else if(gstrRunResult.pu32SP3RealTime < (gstrRunResult.u32SP3RealForbTime + 100))//С�ڽ���ʱ��+100
//            {
//                pfs_Adapt.s32RushWtLow = СͶ��ֹ������ - ��Ͷ��ֹ��ʵ������;
//            }
//        }
//    }
}

/*********************************************************************
��������: RushStdError
��    ��: ����������ͳ�ƺ���
˵    ��: 
��ڲ���: fs32_RealStdErrorDelta=ʵ�ʹ���ֵ�仯��
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-20
��    ��:                      ʱ    ��: 
*********************************************************************/
s32 RushStdError(struct_adapt_t *pfs_Adapt, s32 fs32_RealStdErrorDelta)
{
    #define SLRUSHERRWT 50
    s32 ls32_StdError = 0;
    static s32 lss32_SlRushRecord[SLRUSHERRWT];
    static s32 lss32_SlRushRecord_pro[SLRUSHERRWT];
    s32 ls32_SumErrorTemp = 0;
    u32 i = 0;
        
    if(pfs_Adapt->u32SP2SubCount>SLRUSHERRWT-1)
    {
        for(i=0;i<SLRUSHERRWT-1;i++)
        {
            lss32_SlRushRecord[i] = lss32_SlRushRecord[i+1];
            lss32_SlRushRecord_pro[i] = lss32_SlRushRecord[i];
        }
        lss32_SlRushRecord[SLRUSHERRWT-1] = fs32_RealStdErrorDelta;
        lss32_SlRushRecord_pro[SLRUSHERRWT-1] = fs32_RealStdErrorDelta;
        
        BubbleSort(lss32_SlRushRecord_pro, SLRUSHERRWT); //ð������
        
        ls32_SumErrorTemp = 0;           //�������жϵ�ֵ
        for(i=2;i<SLRUSHERRWT-2;i++)
        {
            ls32_SumErrorTemp += lss32_SlRushRecord_pro[i]; //������Чȥ����ֵ 
        }
        ls32_SumErrorTemp = ls32_SumErrorTemp/(SLRUSHERRWT-4);
        
        ls32_StdError = (lss32_SlRushRecord_pro[SLRUSHERRWT-2]+lss32_SlRushRecord_pro[SLRUSHERRWT-3]*6+lss32_SlRushRecord_pro[SLRUSHERRWT-4]*3)/10;
        if(pfs_Adapt->u32SP2SubCount>SLRUSHERRWT)
            pfs_Adapt->u32SP2SubCount = SLRUSHERRWT;
    }
    else
    {
        lss32_SlRushRecord[pfs_Adapt->u32SP2SubCount] = fs32_RealStdErrorDelta;
        lss32_SlRushRecord_pro[pfs_Adapt->u32SP2SubCount] = fs32_RealStdErrorDelta;
        
        BubbleSort(lss32_SlRushRecord_pro, pfs_Adapt->u32SP2SubCount + 1); //ð������

        ls32_SumErrorTemp = 0;           //�������жϵ�ֵ 
        if(pfs_Adapt->u32SP2SubCount<3)
        {           
            for(i=0;i<pfs_Adapt->u32SP2SubCount+1;i++)
            {
                ls32_SumErrorTemp += lss32_SlRushRecord_pro[i]; //������Чȥ����ֵ 
            }               
            ls32_SumErrorTemp = ls32_SumErrorTemp/(pfs_Adapt->u32SP2SubCount+1);
            ls32_StdError = lss32_SlRushRecord_pro[pfs_Adapt->u32SP2SubCount]*11/10;
        }
        else if(pfs_Adapt->u32SP2SubCount<10)
        {
            for(i=1;i<pfs_Adapt->u32SP2SubCount-1;i++)
            {
                ls32_SumErrorTemp += lss32_SlRushRecord_pro[i]; //������Чȥ����ֵ 
            }
            ls32_SumErrorTemp = ls32_SumErrorTemp/(pfs_Adapt->u32SP2SubCount-2);
            ls32_StdError = lss32_SlRushRecord_pro[pfs_Adapt->u32SP2SubCount-1]*11/10;
        }   
        else if(pfs_Adapt->u32SP2SubCount<SLRUSHERRWT)
        {
            for(i=1;i<pfs_Adapt->u32SP2SubCount-2;i++)
            {
                ls32_SumErrorTemp += lss32_SlRushRecord_pro[i]; //������Чȥ����ֵ 
            }
            ls32_SumErrorTemp = ls32_SumErrorTemp/(pfs_Adapt->u32SP2SubCount-3);
            ls32_StdError = (lss32_SlRushRecord_pro[pfs_Adapt->u32SP2SubCount-1]*3+lss32_SlRushRecord_pro[pfs_Adapt->u32SP2SubCount-2]*6+lss32_SlRushRecord_pro[pfs_Adapt->u32SP2SubCount-3])/10;

        }
        pfs_Adapt->u32SP2SubCount++;        
    }   
    return ls32_StdError;
    
#undef SLRUSHERRWT
}

/*********************************************************************
��������: RushWtProbabilityStatistics
��    ��: ����ͳ�Ʒ��������ӹ�����
˵    ��: 
��ڲ���: *pfs_Adapt ����Ӧ�����ṹ��
���ڲ���: 
�� �� ֵ: ������90%������ֵ
��    ��: �Ժ���               ʱ    ��: 2015-08-20
��    ��:                      ʱ    ��: 
*********************************************************************/
void RushWtProbabilityStatistics(struct_adapt_t *pfs_Adapt)
{
    #define SLRUSHWT   7
//    s32 ls32_StdError = 0;
    static s32 lss32_RushWtLow[SLRUSHWT];
    static s32 lss32_RushWtLowPro[SLRUSHWT];
    static s32 lss32_RushWtMiddle[SLRUSHWT];
    static s32 lss32_RushWtMiddlePro[SLRUSHWT];
    static s32 lss32_RushWtHigh[SLRUSHWT];
    static s32 lss32_RushWtHighPro[SLRUSHWT];
    
    
    u32 i = 0;
//    s32 ls32_MaxToMin = 0;
//    s32 ls32_StdRushW = 0;
    u32 lu32_AddStdRushWtLow = 0;    
    u32 lu32_AddStdRushWtMiddle = 0;    
    u32 lu32_AddStdRushWtHigh = 0;    

    if(pfs_Adapt->u32SP2Count>SLRUSHWT-1)
    {
        for(i=0;i<SLRUSHWT-1;i++)
        {
            lss32_RushWtLow[i] = lss32_RushWtLow[i+1];
            lss32_RushWtLowPro[i] = lss32_RushWtLow[i];
            
            lss32_RushWtMiddle[i] = lss32_RushWtMiddle[i+1];
            lss32_RushWtMiddlePro[i] = lss32_RushWtMiddle[i];
            
            lss32_RushWtHigh[i] = lss32_RushWtHigh[i+1];
            lss32_RushWtHighPro[i] = lss32_RushWtHigh[i];
        }
        lss32_RushWtLow[SLRUSHWT-1] = pfs_Adapt->s32RushWtLow;
        lss32_RushWtLowPro[SLRUSHWT-1] = pfs_Adapt->s32RushWtLow;
        
        lss32_RushWtMiddle[SLRUSHWT-1] = pfs_Adapt->s32RushWtMiddle;
        lss32_RushWtMiddlePro[SLRUSHWT-1] = pfs_Adapt->s32RushWtMiddle;
        
        lss32_RushWtHigh[SLRUSHWT-1] = pfs_Adapt->s32RushWtHigh;
        lss32_RushWtHighPro[SLRUSHWT-1] = pfs_Adapt->s32RushWtHigh;
        
        BubbleSort(lss32_RushWtLowPro, SLRUSHWT); //ð������
        BubbleSort(lss32_RushWtMiddlePro, SLRUSHWT); //ð������
        BubbleSort(lss32_RushWtHighPro, SLRUSHWT); //ð������

        pfs_Adapt->s32RushWtLowProb = (lss32_RushWtLowPro[2]+lss32_RushWtLowPro[3]*2+lss32_RushWtLowPro[4])/4;
        pfs_Adapt->s32RushWtMiddleProb = (lss32_RushWtMiddlePro[2]+lss32_RushWtMiddlePro[3]*2+lss32_RushWtMiddlePro[4])/4;
        pfs_Adapt->s32RushWtHighProb = (lss32_RushWtHighPro[2]+lss32_RushWtHighPro[3]*2+lss32_RushWtHighPro[4])/4;
//        ls32_MaxToMin = lss32_RushWtLowPro[SLRUSHWT-1] - ls32_StdRushW;
//        ls32_StdError = RushStdError(pfs_Adapt, ls32_MaxToMin);    //�ص�
//        ls32_StdRushW = ls32_StdRushW + ls32_StdError;
    }
    else
    {       
        lss32_RushWtLow[pfs_Adapt->u32SP2Count] = pfs_Adapt->s32RushWtLow;
        lss32_RushWtMiddle[pfs_Adapt->u32SP2Count] = pfs_Adapt->s32RushWtMiddle;
        lss32_RushWtHigh[pfs_Adapt->u32SP2Count] = pfs_Adapt->s32RushWtHigh;
        for(i=0;i<pfs_Adapt->u32SP2Count+1;i++)
        {
            lu32_AddStdRushWtLow += lss32_RushWtLow[i];
            lu32_AddStdRushWtMiddle += lss32_RushWtMiddle[i];
            lu32_AddStdRushWtHigh += lss32_RushWtHigh[i];
        }
        lu32_AddStdRushWtLow /= (pfs_Adapt->u32SP2Count+1);
        pfs_Adapt->s32RushWtLowProb = lu32_AddStdRushWtLow/* + ((*pfs_Adapt->ps32SP3) * (s32)3 / (s32)2)*/;
        
        lu32_AddStdRushWtMiddle /= (pfs_Adapt->u32SP2Count+1);
        pfs_Adapt->s32RushWtMiddleProb = lu32_AddStdRushWtMiddle/* + ((*pfs_Adapt->ps32SP3) * (s32)3 / (s32)2)*/;
        
        lu32_AddStdRushWtHigh /= (pfs_Adapt->u32SP2Count+1);
        pfs_Adapt->s32RushWtHighProb = lu32_AddStdRushWtHigh/* + ((*pfs_Adapt->ps32SP3) * (s32)3 / (s32)2)*/;
        
        pfs_Adapt->u32SP2Count++;
    }
//  ls32_StdRushW = ls32_StdRushW + GetRDataPointer()->EXFallWeight;//����Ԥ����  
        
    #undef SLRUSHWT
}

/*********************************************************************
��������: ModifyRushWeight
��    ��: ��Ͷ��������
˵    ��: 
��ڲ���: fs32_RealRushweight=�õ�����Ͷ���
          *pfs_Adapt ����Ӧ�����ṹ��
���ڲ���: 
�� �� ֵ: ���������Ͷ���
��    ��: �Ժ���               ʱ    ��: 2015-08-20
��    ��:                      ʱ    ��: 
*********************************************************************/
s32 ModifyRushWeight(s32 fs32_RealRushweight, struct_adapt_t *pfs_Adapt)
{
    s32 ls32_ModifyRushW = 0;
    s32 ls32_RealRushweightTemp = 0;
    
    ls32_RealRushweightTemp = pfs_Adapt->s32TargetVal * (s32)pfs_Adapt->u32SP2Range / (s32)1000;

    if(fs32_RealRushweight > ls32_RealRushweightTemp)    //�޷�
    {
        fs32_RealRushweight = ls32_RealRushweightTemp;
    }   
    else if(fs32_RealRushweight < (0 - ls32_RealRushweightTemp))     //�޷�
    {
        fs32_RealRushweight = 0 - ls32_RealRushweightTemp;
    }   
    ls32_ModifyRushW = (fs32_RealRushweight * ((s32)pfs_Adapt->u32SP2Scope) / (s32)100);

    return ls32_ModifyRushW;
}

/*********************************************************************
��������: void RushWtCompensate(struct_adapt_t *pfs_Adapt)
��    ��: ����ɼ����Ĳ���
˵    ��: ����ɼ���ʱ��û�е�����Ѿ�ֹͣСͶ���� ��ʱ�Թ��������в��� 
          ���ϸ���СͶ�������Kֵ�����СͶ������
��ڲ���: 
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-27
��    ��:                      ʱ    ��: 
*********************************************************************/
void RushWtCompensate(struct_adapt_t *pfs_Adapt)
{
    u32 lu32_TimeOrWt;
    
    if((*pfs_Adapt->pu32SP3RealTime) < ((*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin))
    {   //СͶʱ��С�ڹ������ɼ���ʱ�� ���в���
        //����ʱ��
        lu32_TimeOrWt = (*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin - (*pfs_Adapt->pu32SP3RealTime);
        //����СͶͳ�Ƶ�kֵ����������� ע��kֵ�ĵ�λ��1��ADת��(3λС����) Ҫ�����ʱ��
        lu32_TimeOrWt = lu32_TimeOrWt * pfs_Adapt->s32SP3KProb * AD_SPEED / 1000 / 1000;
        //�޸Ĺ�����
        pfs_Adapt->s32RushWtLow += lu32_TimeOrWt;
    }
    if((*pfs_Adapt->pu32SP3RealTime) < ((*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin + pfs_Adapt->u32SP2ThresholdVal / 2))
    {   //СͶʱ��С�ڹ������ɼ���ʱ�� ���в���
        //����ʱ��
        lu32_TimeOrWt = (*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin + pfs_Adapt->u32SP2ThresholdVal / 2 - (*pfs_Adapt->pu32SP3RealTime);
        //����СͶͳ�Ƶ�kֵ����������� ע��kֵ�ĵ�λ��1��ADת��(3λС����) Ҫ�����ʱ��
        lu32_TimeOrWt = lu32_TimeOrWt * pfs_Adapt->s32SP3KProb * AD_SPEED / 1000 / 1000;
        //�޸Ĺ�����
        pfs_Adapt->s32RushWtMiddle += lu32_TimeOrWt;
    }
    if((*pfs_Adapt->pu32SP3RealTime) < ((*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin + pfs_Adapt->u32SP2ThresholdVal))
    {   //СͶʱ��С�ڹ������ɼ���ʱ�� ���в���
        //����ʱ��
        lu32_TimeOrWt = (*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin + pfs_Adapt->u32SP2ThresholdVal - (*pfs_Adapt->pu32SP3RealTime);
        //����СͶͳ�Ƶ�kֵ����������� ע��kֵ�ĵ�λ��1��ADת��(3λС����) Ҫ�����ʱ��
        lu32_TimeOrWt = lu32_TimeOrWt * pfs_Adapt->s32SP3KProb * AD_SPEED / 1000 / 1000;
        //�޸Ĺ�����
        pfs_Adapt->s32RushWtHigh += lu32_TimeOrWt;
    }
}

/*********************************************************************
��������: Adapt_SP12
��    ��: ����Ͷ��ǰ������ �˳���ֻ����Ͷ����
˵    ��: 
    ��Ͷ����ԭ�� ����ԭ�����Ч��������δʹ��
    1.��Ͷ����ɼ���ʱ����
    2.�������������
    3.�ɼ���Ͷʱ��
    4.�ɼ���Ͷ��ֹ�㸽���������Kֵ
    5.�ɼ�СͶ�������Kֵ
    6.��Ͷ��ֹ�� �� ����ɼ����ʱ��
    7.��Ͷ��ֹ�� �� ����ɼ��� СͶ�ӵ����� = СͶK �� ʱ�� 
    8.��Ͷ�ӵ����� = ��Ͷ����ɼ���ʱ���� - СͶ�ӵ�����
    9.��Ͷ���� = ��Ͷ�ӵ����� / ��Ͷʱ��
    10.��Ͷ����ʱ�� = ���������� / ��Ͷ����
    11.��Ͷ������� = ����ʱ�� �� ��Ͷ�������K
��ڲ���: *pfs_Adapt ����Ӧ�����ṹ��
���ڲ���: 
�� �� ֵ: 1=��� 0=δ���
��    ��: �Ժ���               ʱ    ��: 2015-08-19
��    ��:                      ʱ    ��: 
*********************************************************************/
u32 Adapt_SP12(struct_adapt_t *pfs_Adapt)
{
    s32 ls32_Temp = 0;       // ��ʱ����
    s32 ls32_InterestWeight = 0;
    u32 lu32_XMinTime = 0;

    if((0 == pfs_Adapt->u32SP2En) || (0 == pfs_Adapt->u32SP2Range)) // ���عر�/ ������ΧΪ0
    {
        Adapt_InitSP2Temp(pfs_Adapt);
        pfs_Adapt->bFirstIn = true;
    }
    else
    {
        if((pfs_Adapt->s32SP2Bkp != (*pfs_Adapt->ps32SP2)))           // ����Ͷ������Ͷ��ǰ�����޸�
        {
            Adapt_InitSP2Temp(pfs_Adapt);
            pfs_Adapt->bFirstIn = true;
        }
        else
        {
            //ͳ�Ƶõ�СͶ�������Kֵ
            pfs_Adapt->s32SP3KProb = SP3LinearKProbabilityStatistics(pfs_Adapt, *pfs_Adapt->ps32SP3KThisTime);
            //ͳ�Ƶõ���Ͷ��ֹ�㸽���������Kֵ ע��ú���Ҫ����FallWtProbabilityStatistics֮ǰ
            pfs_Adapt->s32SP2KProb = SP2LinearKProbabilityStatistics(pfs_Adapt, *pfs_Adapt->ps32SP2KThisTime);
            
            lu32_XMinTime = (*pfs_Adapt->pu32SP3ForbidTime) + pfs_Adapt->u32SP2ThresholdMin;         // ��Ͷ����ʱ��СͶ����ʱ��
            if(lu32_XMinTime < ADAPT_SP12_THRESHOLD_LIMIT)
            {
                lu32_XMinTime = ADAPT_SP12_THRESHOLD_LIMIT;
            }
            pfs_Adapt->u32SP2ThresholdMax = lu32_XMinTime + pfs_Adapt->u32SP2ThresholdVal;  //�õ�СͶ����ʱ��
            
            RushWtCompensate(pfs_Adapt);    //��������������
            RushWtProbabilityStatistics(pfs_Adapt);  // �Թ��������и���ͳ�ƴ���
            
            //��Ͷ����ɼ���ʱ������ = ������+��Ͷ��ֹ��
            pfs_Adapt->u32SP2ResultWt = pfs_Adapt->s32RushWtMiddleProb + (*pfs_Adapt->ps32SP2RealWt);
            //��Ͷ�ӵ����� = ��Ͷ����ɼ���ʱ���� - СͶ�ӵ�������k*t��
            pfs_Adapt->u32SP2WT = pfs_Adapt->u32SP2ResultWt - (pfs_Adapt->s32SP3KProb * (((*pfs_Adapt->pu32SP3ForbidTime) + \
                                  pfs_Adapt->u32SP2ThresholdMin + pfs_Adapt->u32SP2ThresholdVal / 2) * 480 / 1000 / 1000));
            //��Ͷ���� = ��Ͷ�ӵ����� / ��Ͷʱ��(ms)
            pfs_Adapt->fSP2Flux = (float)pfs_Adapt->u32SP2WT  / ((float)(*pfs_Adapt->pu32SP2RealTime));
            //ͳ�ƴ�����Ͷ����
            pfs_Adapt->fSP2Flux = SP2FluxProbabilityStatistics(pfs_Adapt, pfs_Adapt->fSP2Flux);
            
            pfs_Adapt->s32SP2AdaptSig = 0;  //��ӡ�������
            
            ls32_InterestWeight = pfs_Adapt->s32RushWtMiddleProb;   //ʹ��ʱ���е����ֵ�ж�
            if(ls32_InterestWeight != 0)
            {
                if((*pfs_Adapt->pu32SP3RealTime < lu32_XMinTime)|| (*pfs_Adapt->pu32SP3RealTime > pfs_Adapt->u32SP2ThresholdMax))
                {   //СͶʱ����̻���� ��������

                    ls32_Temp = ls32_InterestWeight + *pfs_Adapt->ps32SP3;  //������+СͶ���ֵ
                    
                    if((*pfs_Adapt->ps32SP2 > ls32_Temp) && (*pfs_Adapt->pu32SP3RealTime > pfs_Adapt->u32SP2ThresholdMax))  // ����2.2�룬����������ǰ��
                    {   //���õ���Ͷ��ǰ������������&&����ʱ������������ ��С��Ͷ��ǰ��
                        pfs_Adapt->s32SP2AdaptSig = -1;
                        //�õ�����������
//                        ls32_Temp = ModifyRushWeight(*pfs_Adapt->ps32SP2 - ls32_Temp, pfs_Adapt);
                        ls32_Temp = (*pfs_Adapt->ps32SP2 - ls32_Temp);
                        //��Ͷ����ʱ�� = ���������� / ��Ͷ����
                        //��Ͷ��ǰ��������� = ��Ͷ����ʱ�� �� ��Ͷ�������K(ת����ms��λ)
                        ls32_Temp = (float)(ls32_Temp) * ((float)pfs_Adapt->s32SP2KProb * AD_SPEED / 1000 /1000) / pfs_Adapt->fSP2Flux;
                        ls32_Temp = ModifyRushWeight(ls32_Temp, pfs_Adapt);
                        
                        pfs_Adapt->s32SP2AdaptVal = ls32_Temp;  //��Ͷ����ֵ ��ӡ��
                        
                        ls32_InterestWeight = *pfs_Adapt->ps32SP2 - ls32_Temp;
                        if((ls32_InterestWeight > 0) && (ls32_InterestWeight < pfs_Adapt->s32TargetVal))
                        {
                            *pfs_Adapt->ps32SP2 = ls32_InterestWeight;
                        }
                        pfs_Adapt->u32SP2Count = 0; //���������¼��������
                    }
                    else if((*pfs_Adapt->ps32SP2 < ls32_Temp) && (*pfs_Adapt->pu32SP3RealTime < lu32_XMinTime)) // С��1.1�룬���ܼ�С��ǰ��
                    {   //���õ���Ͷ��ǰ��С��������&&����ʱ��С��������� ������Ͷ��ǰ��
                        pfs_Adapt->s32SP2AdaptSig = 1;
                        //�õ�����������
//                        ls32_Temp = ModifyRushWeight(ls32_Temp - *pfs_Adapt->ps32SP2, pfs_Adapt);
                        ls32_Temp = (ls32_Temp - *pfs_Adapt->ps32SP2);
                        //��Ͷ����ʱ��(AD����) = ���������� / ��Ͷ����
                        //��Ͷ��ǰ��������� = ����ʱ�� �� ��Ͷ�������K
                        ls32_Temp = (float)(ls32_Temp) * ((float)pfs_Adapt->s32SP2KProb * AD_SPEED / 1000 /1000) / pfs_Adapt->fSP2Flux;
                        ls32_Temp = ModifyRushWeight(ls32_Temp, pfs_Adapt);
                        
                        pfs_Adapt->s32SP2AdaptVal = ls32_Temp;  //��Ͷ����ֵ ��ӡ��
                        
                        ls32_InterestWeight = *pfs_Adapt->ps32SP2 + ls32_Temp;
                        if((ls32_InterestWeight > 0) && (ls32_InterestWeight < pfs_Adapt->s32TargetVal))
                        {
                            *pfs_Adapt->ps32SP2 = ls32_InterestWeight;
                        }
                        pfs_Adapt->u32SP2Count = 0; //���������¼��������
                    }   
                    
                    if(*pfs_Adapt->ps32SP2 > pfs_Adapt->s32TargetVal - 100)      // ����
                    {
                        *pfs_Adapt->ps32SP2 = pfs_Adapt->s32TargetVal - 100;
                    }
                    else if(*pfs_Adapt->ps32SP2 < 100)                        // ����
                    {
                        *pfs_Adapt->ps32SP2 = 100;
                    }
                    SaveSP2Value(pfs_Adapt);   // ���浽����
                }   
            }   
        }
    }
    
    return 1;
}

/*********************************************************************
��������: ClrPrintMessage
��    ��: �����ӡ����������Ϣ
˵    ��: 
��ڲ���: *pfs_Adapt ����Ӧ�����ṹ��
���ڲ���: 
�� �� ֵ: 
��    ��: �Ժ���               ʱ    ��: 2015-08-25
��    ��:                      ʱ    ��: 
*********************************************************************/
void ClrPrintMessage(struct_adapt_t *pfs_Adapt)
{
    pfs_Adapt->s32SP2AdaptVal = 0;
    pfs_Adapt->u32SP3Prob = 0;
    pfs_Adapt->s32SP3AdaptVal = 0;
}

/*********************************************************************
��������: Adapt_Process
��    ��: ����Ӧ�������
˵    ��: ���ڼ��϶�ֵ��ɺ����
��ڲ���: *pfs_Adapt ����Ӧ�����ṹ��
���ڲ���: 
�� �� ֵ: 1=��� 0=δ���
��    ��: �Ժ���               ʱ    ��: 2015-08-19
��    ��:                      ʱ    ��: 
*********************************************************************/
u32 Adapt_Process(struct_adapt_t *pfs_Adapt)
{
    ClrPrintMessage(pfs_Adapt); //�����ӡ�����Ϣ
    if(0 != pfs_Adapt->u32AdatpEn)
    {   //����Ӧ���ش�
        if(true == pfs_Adapt->bFirstIn) // ��һ������
        {
            pfs_Adapt->bFirstIn = false;
        }
        else
        {
            Adapt_SP12(pfs_Adapt);  //����Ͷ����Ӧ����
            Adapt_SP3(pfs_Adapt);   //СͶ����Ӧ����
        }
    }
    else
    {
        pfs_Adapt->bFirstIn = true;
    }
    
    return 1;
}



