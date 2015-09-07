#ifndef _RUNSTATE_H
#define _RUNSTATE_H

#define     T1PROCESS               1        /*���̿��Ƶ�T1��ʱ�׶�*/
#define     SUPERLARGEPROCESS       2        /*���̿��Ƶĳ���Ͷ�׶�*/
#define     LARGEPROCESS            3        /*���̿��ƵĴ�Ͷ�׶�*/
#define     MIDDLEPROCESS           4        /*���̿��Ƶ���Ͷ�׶�*/
#define     SMALLPROCESS            5        /*���̿��Ƶ�СͶ�׶�*/
#define     T3PROCESS               6        /*���̿��Ƶ�T3��ʱ�׶�*/
#define     T4PROCESS               7        /*���̿��Ƶ�T4��ʱ�׶�*/
#define     FALLPROCESS             8        /*���̿��Ƶ�ж�Ͻ׶�*/
#define     T5PROCESS               9        /*���̿��Ƶ�T5��ʱ�׶�*/
#define     AUTOSTUDYPROCESS        10      //��ѧϰ����

//�д���λ��־
#define     PACKOK  ((!gb_EnPackOKFlg) || gb_PackOKFlg)
//�д��Ѿ���ɱ�־
#define     PACKEND  (gb_SDDelayFlg && gb_PackFlg && (!gb_JSDDelayFlg) && PACKOK)



extern u32 gu32_Process;                 //����ʱ�ļ���״̬
extern bool gb_TimeRunFlg;              //��ʱ�Ѿ�������־λ
extern u32 gu32_TimeBuf;                //��ʱ��ʱ�����棬�������õ�ʱ��ֵ
extern u32 gu32_Time1_5;                //T1~T5�ļ�ʱ�����棬��������ʱ�̵�ʱ��
extern bool  gb_FlapBagOutFlg;             //�Ĵ������Ч��־
extern bool  gb_FlapWaitFlg;               //�Ĵ�֮ǰ�ĵȴ���־
extern bool  gb_FlapStartFlg;              //�Ĵ��Ѿ���ʼ��־
extern bool  gb_FlapAllHighFlg;            //�͵�ƽʱ��Ϊ0,�Ĵ��㶨��Ч��־λ
extern u32 gu32_TimeFlapBuf;               //�Ĵ���ʱ��Ĵ���
extern u32 gu32_Time8_9;                   //t8��t10����ʱ�Ĵ���
extern u32 gu32_FlapBagNumCnt;             //�Ĵ�������
extern bool gb_TareGetedFlg;            //Ƥ���Ѿ���ȡ��־λ
extern u32 gu32_OUDelayTimeBuf;         //��Ƿ����ͣ�ر�ʱ�ĳ�Ƿ�����ʱ�Ĵ���
extern bool gb_AddFlapBagFlg;                //�������Ĵ���־λ
extern bool gb_ExtFlapBagFlg;                //�����Ĵ������־λ
extern u32 gu32_DiscShakeCnt;           //ж��������
extern bool gb_Error9Flg;                    //ж��δ�ɾ�����
extern bool gb_SumFlg;                 //�ۼ�
extern bool gb_DiscWtGetedFlg;       //��ֵ�����Ѿ���ȡ
extern u32 gu32_DiscValue;     //����ж��ֵ
extern u32 gu32_HoldWt;        //��ֵ��������
extern u32 gu32_WeighTimeCnt;      //��������
extern u32 gu32_TargetTemp;        //Ŀ��ֵ�Ļ���
extern bool gb_LiftBagStartFlg;    //�������
extern bool gb_PushBagStartFlg;    //�����ƴ�
extern u32 gu32_SumTemp;           //�ۼƵĻ���ֵ
extern bool gb_ConveyorStartFlg;   //�������ͻ�


typedef struct
{
    u8  u8Process;          //��ѧϰ����
    u32 u32StartMoment;     //���Ͽ�ʼʱ��
    u32 u32SP2Time;         //��Ͷ����ʱ��
    u32 u32SP3Time;         //СͶ����ʱ��
    float fSP2Flux;         //��Ͷ����
    s32 s32SP2StopResult;   //��ͶͶ�Ͻ��
    s32 s32SP3StopMomentWt; //Сͷֹͣʱ������
    s32 s32SP3StopResult;   //СͶͶ�Ͻ��
    s32 s32SP3K;            //СͶ�������K
    u32 u32ForbidTime;      //��ѧϰ�Ľ�ֹ�Ƚ�ʱ��
    
    u32 u32SP3Val;          //��ѧϰ��СͶ���ֵ
}struct_autostudy_t;

extern struct_autostudy_t gs_AutoStd;
#define AUTOSTD_NONE        0   //
#define AUTOSTD_SP2         1   //��ѧϰ��Ͷ������
#define AUTOSTD_SP2STOP     2   //��ѧϰ��Ͷ��ֹͣ
#define AUTOSTD_SP3         3   //��ѧϰСͶ������
#define AUTOSTD_SP3STOP     4   //��ѧϰСͶ��ֹͣ
#define AUTOSTD_SP3FILL     5   //�ٴ�����SP3��ɼ���
#define AUTOSTD_SECEND      6   //��ѧϰ�ڶ�������


#define SP3LINEARKLEN   10
typedef struct
{
    u8 u8Switch;    //СͶ��ֹ��Ԥ�⹦�ܵĿ���
    s32 s32KSampleArray[SP3LINEARKLEN];   //����СͶ���б�ʵ����� 
    u32 u32KSampleTimes;    //���õ�Kֵ�������
    u32 u32KCntSample;      //�Ѿ������Kֵ��������
    u8  u8State;    //0x00=СͶ��ֹ��Ԥ�⹦��û�д�
                    //0x10=ǰ���������Kֵ�ɼ��׶� ��û��Խ������ϲ�������u3InvalidTimes
                    //0x11=ǰ���������Kֵ�ɼ��׶� �Ѿ�Խ������ϲ������� ��û�дﵽ������Ч��������u32ValidTimes
                    //0x12=ǰ���������Kֵ�ɼ��׶� �Ѿ��ﵽ������Ч��������u32ValidTimes
                    //0x20=�������Kֵ�Ѿ��ɼ���� ��û��Խ������ϲ�������u3InvalidTimes
                    //0x21=�������Kֵ�Ѿ��ɼ���� �Ѿ�Խ������ϲ������� ��û�дﵽ������Ч��������u32ValidTimes
                    //0x22=�������Kֵ�Ѿ��ɼ���� �Ѿ��ﵽ������Ч��������u32ValidTimes ��ʼ�ӹ�СͶ��ֹ�����
    bool bSampleDoneFlg;    //ǰ���������Kֵ�ɼ��Ѿ����
    u32 u32PredictTarget;   //Ԥ���СͶ��ֹ�㻹��Ҫ���ٴ�ADת��
    s32 s32RealStopWt;      //СͶֹͣʱ��Ӧ��ʵ������
    u32 u32CntTarget;       //Ԥ���СͶ��ֹ��֮��ļ�������
    s32 s32KThisTime;       //���μ��ϵ�СͶKֵ
    s32 s32KAverage;        //�����ƽ��Kֵ
    u32 u32InvalidTimes;     //��ֵ����ϵ���Ч��������
    u32 u32CntInvalid;      //��ֵ�󲻼ǲ���������������
    u32 u32ValidTimes;      //��ʼ��ϵ�������Ч��������
    u32 u32CntValid;        //��ʼ��¼���������ļ�������
    
}PREDICTSP3;    //Ԥ��СͶ������ز����ṹ��
extern PREDICTSP3 gs_PredictSp3;
extern PREDICTSP3 gs_PredictSp3Temp;   //��ӡ��ʱ��ʹ����Ϊ����

typedef struct
{
    u32 u32Sp1Time;         //���ʱ��
    u32 u32Sp1TimeTemp;     //���ʱ�仺��
    u32 u32Sp2Time;         //�м�ʱ��
    u32 u32Sp2TimeTemp;     //�м�ʱ�仺��
    u32 u32Sp3Time;         //����ʱ��
    u32 u32Sp3TimeTemp;     //����ʱ�仺��
    u32 u32RunCmpTime;      //��������ֵ����ʱ��
    bool bGetRunCmpTimeFlg; //��������ֵ����ʱ���Ѿ���ȡ����־λ
    u32 u32OneTareTime;     //һ�μ��Ϲ���ʱ��
    u32 u32RunStartTimeTemp;     //����ʱ�̻���
    u32 u32SumCnt;      //����
    u32 u32TotalTime;   //����ʱ
    u32 u32FirstRunTimeTemp;
    bool u32HadPrintedMin;       //�Ѿ���ӡ�ܽ����ݵķ�����    
    u32 u32PredictSp2StopTime;  //Ԥ�����Ͷֹͣʱ��
    u32 u32MaxWt;       //��ֵ
    u32 u32MinWt;       //��ֵ
    u32 u32Sp2StopToMaxTime;    //��Ͷ��ֹ�㵽��ֵʱ��
    u32 u32MaxToMinTime;        //��ֵ����ֵʱ��
    u32 u32MaxToMinTimeTemp;
    s32 s32Sp2MaxKNotFed;       //��Ͷ�˲�ǰ���k
    s32 s32Sp2MaxKFed;          //��Ͷ�˲������k
    s32 s32MaxKFedReLinear;     //���k��������������б��F3.11������ϵ���
    s32 s32MaxBFedReLinear;     //���K��������������bֵ
    
    s32 s32SP3RealStopWt;       //ʵ��ͣСͶʱ������ֵ
    s32 s32SP2RealStopWt;       //ʵ��ͣ��Ͷʱ������ֵ
}DEBUG_RUN_TIME;

typedef struct 
{
//    u32 u32Result[3000];    //���Ͻ��
    u32 u32StartTime;       //��ʼʱ���¼ ���ܹ���ʱ��¼
    u32 u32TotalTime;       //�ܹ���ʱ��¼
    u32 u32SumNum;          //��ɰ���
    u32 u32SumWt;           //�������
    u32 u32NumAHour;        //ÿСʱ����
    u32 u32WtAHour;         //ÿСʱ����
    u32 u32NumAMin;         //ÿ���Ӱ���
    u32 ResultAverage;      //ƽ��ÿ������
    u32 u32MostOverResult;  //��߳���ֵ
    u32 u32MostOverNum;     //��߳���ֵ��Ӧ����
    u32 u32MostUnderResult; //���Ƿ��ֵ
    u32 u32MostUnderNum;    //���Ƿ��ֵ��Ӧ����
    u32 u32TheMaxValue;     //���ֵ
    u32 u32TheMinValue;     //���ֵ
    u32 u32ErrorBand;       //����
    
    u32 u32Dst_Under_Num;   //�ֲ���� Ƿ�����
    u32 u32Dst_Under_Per;   //�ٷ���
    u32 u32Dst_U_2_Num;   //�ֲ����
    u32 u32Dst_U_2_Per;   //�ٷ���
    u32 u32Dst_2_1_Num;    //�ֲ����
    u32 u32Dst_2_1_Per;    //�ٷ���
    u32 u32Dst_1_0_Num;     //�ֲ����
    u32 u32Dst_1_0_Per;     //�ٷ���
    u32 u32Dst_0_1_Num;     //�ֲ����
    u32 u32Dst_0_1_Per;     //�ٷ���
    u32 u32Dst_1_2_Num;    //�ֲ����
    u32 u32Dst_1_2_Per;    //�ٷ���
    u32 u32Dst_2_O_Num;   //�ֲ����
    u32 u32Dst_2_O_Per;   //�ٷ���
    u32 u32Dst_Over_Num;    //�ֲ���� �������
    u32 u32Dst_Over_Per;    //�ٷ���
    
}STATISTIC_DATA;

#define ONESTEP 5

extern STATISTIC_DATA gs_Statistic; //ͳ������
extern DEBUG_RUN_TIME gs_DebugRunTime;

#ifdef DEBUG_MONITORTIME_PRINT
    extern DEBUG_RUN_TIME gs_DebugRunTimeTemp; //��ӡ��ʱ��ʹ����Ϊgs_DebugRunTime�Ļ���
    extern u32 gu32_DebugTarget;
    extern u32 gu32_DebugLarge;
    extern u32 gu32_DebugMiddle;
    extern u32 gu32_DebugSmall;
#endif


extern void Key_RunState(void);
extern void FlapBag(void);
extern void UpdateFastSetRec(void);
extern bool StabOver(bool lb_Reset);
void PredictSp3_Restart(void);
void StatisticClrStruct(void);
void StatisticSaveStartTime(u32 fu32_Time);

#endif

