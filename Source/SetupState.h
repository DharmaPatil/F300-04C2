#ifndef _SETUP_H
#define _SETUP_H

#include "71x_type.h"

#define DISP_CURR   0       //��ʾ����
#define DISP_TOL1   1       //��ʾ���ۼ�
#define DISP_TOL2   2       //��ʾ�շ����ۼ�
#define DISP_FLU    3       //��ʾ����

#define BINYES      0       //�м�����ģʽ
#define BINNO       1       //�޼�����ģʽ
#define BIN2_A      2       //˫��A
#define BIN2_B      3       //˫��B
#define NO2_A       4       //˫�޶�A
#define NO2_B       5       //˫�޶�B

//#define BULK        2       //ɢ���ۼ�ģʽ

#define GROSS       0       //�޶�ë��
#define NET         1       //�޶�����

#define CO          0       //��ϸ���
#define SIN         1       //��������

#define AUX_REC     0       //����ʾ�䷽
#define AUX_SUM     1       //����ʾ���ۼƴ���
#define AUX_SUM_R   2       //����ʾ�䷽�ۼƴ���


#define OFF         0
#define ON1         1
#define ON2         2


struct SETUP
{   //B1
    u32 TareMode;           //����ģʽ
    u32 GorNMode;           //�޶�ë/���ط�ʽ
    u32 FeedMode;           //���Ϸ�ʽ
    u32 AutoClrZeroInterval;//�Զ�������
    u32 JudgeStabOverTime;  //���ȳ�ʱʱ��
    u32 AuxDispType;        //����ʾ����
    //B2
    u32 ClrZeroRng;         //���㷶Χ
    u32 JudgeStabRng;       //���ȷ�Χ
    u32 TraceZeroRng;       //�����ٷ�Χ
    u32 AdFilterLevel;      //AD�����˲�����
    u32 AdFilterLevelDisc;  //ж���˲�����
    u32 AdFilterLevelComp;  //��ֵ�˲�����
    u32 AdSamplingRate;     //AD�����ٶ�
    u32 ExtType;            //��չ�ӿ�����
    //B3
    u32 PowerOnZeroSwitch;  //�ϵ��Զ����㿪��
    u32 DiscSumSwitch;      //�ֶ�ж���ۼƿ���
    u32 ResultHoldSwitch;   //��ֵ�������ֿ���
    u32 SumCheckCodeSwitch; //�ۼƲ�ѯ���뿪��
    u32 SumClrCodeSwitch;   //�ۼ�������뿪��
    u32 SumExtendSwitch;    //�ۼƷ�Χ��չ����
    //B4
    u32 ComMode_1;          //ͨѶ��ʽ
    u32 TareNumber_1;       //�豸��
    u32 BaudRate_1;         //������
    u32 ParityMode_1;       //���ݸ�ʽ
    u32 HiLo_1;             //modbus�ߵ���˳��
    u32 PrintSwitch_1;      //�Զ���ӡ����
    u32 PrintType_1;        //��ӡ��ʽ
    u32 PrintLanguage_1;    //��ӡ����
    u32 PrintBlankRows_1;   //��ӡ��ֽ����
    //B5
    u32 ComMode_2;          //ͨѶ��ʽ        
    u32 TareNumber_2;       //�豸��          
    u32 BaudRate_2;         //������          
    u32 ParityMode_2;       //���ݸ�ʽ        
    u32 HiLo_2;             //modbus�ߵ���˳��
    u32 PrintSwitch_2;      //�Զ���ӡ����    
    u32 PrintType_2;        //��ӡ��ʽ
    u32 PrintLanguage_2;    //��ӡ����
    u32 PrintBlankRows_2;   //��ӡ��ֽ����
    //B6
    u32 No2PackOffTwoSwitch;    //˫�޶��ӻ���ģʽͬʱ�ɴ�����
    u32 No2ConveyorDelayTime;   //���ͻ�������ʱʱ��          
    u32 No2ConveyorRunTime;     //���ͻ�����ʱ��              
    u32 No2AddAgainDelayTime;   //�ٴμд���ʱ��������ʱ��    
    //B7
    u32 AdaptSW;        //����Ӧ����
    u32 AdaptSP2SW;     //
    u32 AdaptSP2LimitRng;
    u32 AdaptSP2Level;
    u32 AdaptSP2TimeThreshold;
    u32 AdaptSP3SW;
    u32 AdaptSP3LimitRng;
    u32 AdaptSP3Scope;
    u32 AdaptSP2Scope;
    u32 Adapt7_9;
    
};
extern struct SETUP gs_Setup;
extern u32 gu32_SetupCodeSwitch;              //�����������뿪��
extern u32 gu32_SetupCode;                    //������������
extern u8 gu8_SetupInTime;                //����������������������


extern void Key_SetupState(void);
extern void Disp_SetupState(void);
extern void TareModeChange(u32 fu32_NewTareMode);


#endif
