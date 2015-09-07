#ifndef _ADTOWEIGHT_H
#define _ADTOWEIGHT_H

#include "71x_type.h"


extern u32  gu32_AdFedValue;       //AD���ݽ����˲���õ�������
extern u32  gu32_NetAdFedValue;    //��ȥ�궨����AD��
extern bool gb_NetAdFlg;           //��ȥ�궨����AD�����λ
extern u32  gu32_Nquad;            //4���ֶ���
extern bool gb_SigNquadFlg;        //4���ֶ����ķ���λ
extern u32  gu32_FloatZero;        //�������
extern bool gb_SigFZeroFlg;        //����������
extern u32  gu32_N;                //�ֶ���
extern u32  gu32_Wt;               //����
extern bool gb_StabFlg;        //�ȶ�
extern bool gb_MvStabFlg;      //�������ȶ�
extern bool gb_PowerOnZeroFlg;     //�ϵ��Զ�����
extern bool gb_ZeroKeyFlg;         //�������
extern bool gb_ForceClrZeroFlg;    //ǿ�������־λ �����������������ȳ�ʱ
extern bool gb_AlarmFlg;       //������־
extern bool gb_ZeroPosition;   //��λ��־
extern bool gb_NzFlg;          //������־
extern bool gb_DispNetFlg;         //��ʾ���ر�־λ
extern bool gb_NetSigFlg;          //���ط���
extern u32 gu32_TareNquad;        //Ƥ��ֵ��4���ֶ���
extern bool gb_TareSigFlg;         //Ƥ�ط���
extern s32 gs32_Wt;                //�з�����������ֵ
extern s32 gs32_WtKalmanFed;       //�������˲�����������ֵ�з�������


#ifdef DEBUG_WT_SEND
extern u32 gu32_WtNotFed;          //û���˲���AD��ת���ɵ�����
extern bool gb_SigWtNotFedFlg;     //û���˲���AD��ת���ɵ������ķ���
#endif

#ifdef REFRESH_AT_SP23_MAX
    extern bool gb_RefreshAdArrayFlg;
#endif

//#ifdef DEBUG_SMARTT2B
    extern bool gb_T2bRunFlg;  //СͶ��ֹ�Ƚ�ʱ���Ѿ���ʼ
    extern u32 gu32_T2bTime;
//#endif

extern bool gb_Sp2StopTimeRunFlg;  //Ԥ����Ͷֹͣʱ���Ѿ���ʼ
extern u32 gu32_Sp2StopTime01ms;       //Ԥ����Ͷֹͣ��ʱ�� ��λ0.1ms

extern float gf_WtKalmanFed;       //�������˲�����������ֵ
//�������˲�����ʾ����ֵ ���ֶ��й�
#define DISP_WT_KALMAN  (gf_WtKalmanFed > 0 ? ((u32)(gf_WtKalmanFed + (float)gs_Cal.Div / 2)) / gs_Cal.Div * gs_Cal.Div : ((u32)((-gf_WtKalmanFed) + (float)gs_Cal.Div / 2)) / gs_Cal.Div * gs_Cal.Div)
//�������˲�������ֵ ���ֶ��޹� ��λ1
//#define WT_KALMAN   (gf_WtKalmanFed > 0 ? (u32)(gf_WtKalmanFed + 0.5) : (u32)((-gf_WtKalmanFed) + 0.5))

extern void GetWt(void);
void GetMaxLinearK(bool fb_StartFlg);

#endif


