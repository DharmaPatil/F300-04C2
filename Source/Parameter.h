#ifndef _Parameter_H
#define _Parameter_H

#define RESET_CHECKCODE     0
#define RESET_ALL           1
#define RESET_CAL           2
#define RESET_SET           3
#define RESET_REC           4
#define RESET_IODEF         5


#define BORR_RELOAD                 0
#define BORR_RELOAD_DATE            1
#define BORR_RELOAD_CHECKCODE       2
#define BORR_BACKUP                 3
#define BORR_BACKUP_DATE            4
#define BORR_BACKUP_CHECKCODE       5

#define MODELSET_CHECKCODE      0
#define MODELSET_SWITCH         1
#define MODELSET_SETUP_0        2
#define MODELSET_SETUP_1        3
#define MODELSET_SETUP_2        4
#define MODELSET_SETUP_3        5
#define MODELSET_SETUP_4        6
#define MODELSET_SETUP_5        7
#define MODELSET_SETUP_6        8


extern u8 gu8_ResetInTime;              //�ָ�����������������������
extern u8 gu8_BorRInTime;              //���ݻָ���������������������
extern u8 gu8_ModelSetInTime;          //�ͺ��Զ�����������������
extern bool  gb_ExitFlg;              //�˳��ָ���������
extern u32 gu32_ModelSetSwitch;            //�ͺ��Զ��忪��
extern u32 gu32_ModelArry[7];              //�ͺ���ʾ����������� 0-logo���أ�1~6��6λ���ͺ���ʾ

extern void Init_Parameter(void);
extern void Read_CalibData(void);
extern void Read_RecipeData(u32 ful_RecNum);
extern void Reset_Parameter(void);
extern void ModelSet(void);
extern void Save_SumData(u32 fu32_Weight, u32 fu32_Num);
extern void Reset_Cal(void);
extern void Reset_IoDef(void);
extern void Reset_Rec(void);
extern void BorR_Parameter(void);
extern void Backup_Parameter(void);
extern void Reload_Parameter(void);
extern void Disp_Delay(void);
extern void Reset_Setup(bool fb_MbFlg);
extern void Read_SetupData(void);


#endif
