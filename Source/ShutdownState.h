#ifndef _SHUTDOWN_H
#define _SHUTDOWN_H


#define SHUTDOWN_CHECKCODE      0
#define SHUTDOWN_SWITCH         1
#define SHUTDOWN_DAYNUM         2
#define SHUTDOWN_INPUTCODE      3
#define SHUTDOWN_BUYID          4
#define SHUTDOWN_UNLOCK         5

extern u8  gu8_ShutDownInTime;         //��������������
extern u32 gu32_BuyerID;               //�ͻ����� 00~99 ��Ӧ��ͬ��ID�㷨.
extern u32 gu32_RandomID;              //�����ID
extern u32 gu32_ShutDownCode;          //��ʱ�����û�����
extern u32 gu32_RandomUnlockCode;      //�����������
extern u32 gu32_Random30DayCode;       //���30������
extern u32 gu32_Random60DayCode;       //���60������
extern u32 gu32_ShutDownSwitch;        //��ʱ��������
extern u32 gu32_LeaveDays;             //ʣ����������
extern u32 gu32_GetedRandom;           //������Ѿ���ȡ��־
extern bool gb_ReCmpFlg;               //��������־�����±Ƚ�����ʱ��
extern bool gb_SysClosedFlg;           //ϵͳ�Ѿ�������־
extern bool gb_JudgeShutDownFlg;       //�ж�������־λ


extern void Disp_ShutdownState(void);
extern void Key_ShutdownState(void);
extern void UpdataGrandShutCode(void);
extern u32 GetRandom(void);
extern bool GradeShutCodeEQ(void);
extern void Sys_Closed_Set(void);
extern void CmpDay(void);
extern void Auto_ShutDown(void);


#endif
