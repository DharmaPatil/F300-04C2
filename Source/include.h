#ifndef _INCLUDE_
#define _INCLUDE_

//����Ϊ�����ù��ܵĺ궨�壬��ʽ����ʱҪע�͵�      for test

#define DEBUG_ADCNVT_TIMES      //����ʾʵ��ADת������/��

#define DEBUG_WT_SEND           //ע�ⲻ�ܺ������DEBUG_WT_STATE_SENDͬʱ����
//CONTͨѶ��ʽ�£�����1��ͣ�����˲�ǰAD����˲���AD�룻����2��ͣ�����˲�ǰ�������˲�������

//#define DEBUG_WT_STATE_SEND     //ע�ⲻ�ܺ������DEBUG_WT_SENDͬʱ����
//CONTͨѶ��ʽ�£���ͣ���͵�ǰ�����ͼ���������ݡ�

#define DEBUG_MONITORTIME_PRINT
//ʱ��ĳ�msΪ��λ 2015-7-23
//PRINT��ʽÿ�μ��Ϲ��̽����󣬷��ͼ��Ͻ��������ݣ���ʽ���£�����Printstate.c�У�
//    {"���ԡ����Ե�����   ����:          ʱ��:             ADת���ٶ�:   ��/�� \r\n"},
//    {"-----------------------------------------------------------------------------------------------------\r\n"},
//    {" ���  �м�  ���� ��ֵ�� ж����   ���   Ŀ��ֵ  ��Ͷֵ  ��Ͷֵ  СͶֵ T2a  T2b T1  T4  ���� ����ʱ \r\n"},
//    {" (��)  (��)  (��)   (��)   (��)  (    )  (    )  (    )  (    )  (    ) (��)(��)(��)(��)       (��)  \r\n"} };


#define DEBUG_JUDESTABLE_TIME
//����ʱ�����������������0.5s����b1.5��������

#define DEBUG_T2A
//����ʱ��Ͷ��ֹ�Ƚ�ʱ�����ò�Ϊ0����ʹ�����Ѿ�������Ͷ��ֹ��Ҳ������Ͷ������Ͷ��ֹ�Ƚ�ʱ��

#define DEBUG_RUN_STOP_BY_KEY
//��������������ͣ TARE������ ������ͣ

//#define REFRESH_AT_SP23_MAX
//����Ͷ����������������ߵ�ˢ�²�������

#define DEBUG_SMARTT2B
//�����ж�T2B��ֹ�Ƚ�ʱ��

#define DEBUG_SMARTT2B_FILTER
//�����ǰ�����Ѿ�ʹ���������DEBUG_SMARTT2B�����жϽ�ֹ�Ƚ�ʱ��
//�����жϽ�ֹ�Ƚ�ʱ��ʱ���õ��˲���������F2.3(�����ٷ�Χ)������

#define ADFILTER_02CS
//ʹ��GM8802CS���˲����� ��120��240��480��ͬAD�ٶȵ��˲�

#define DISC_TIME_CONTROL
//ʱ�����ж�� F1.5��λΪms С������Ч

//#define PREDICT_SP2_STOP
//����Ԥ����Ͷ��ֹ��    //��B3.3����������

#define DONOT_SAVE_SUM
//�������ۼ����ݵ�����  ��ֹӰ�촮�ڷ������ݵ��ٶ�


    #include "string.h"     //Ϊ��ʹ��memcpy()�ڴ濽��,memset()�Ⱥ���
    #include "71x_lib.h"
    #include "stdlib.h"     //���������srand() rand() abs()��
    #include "IO.h"
    #include "RTC.h"
    #include "Buzzer.h"
    #include "UserBin.h"
    #include "UserLib.h"
    #include "F_RAM.h"
    #include "VFD.h"
    #include "Timer.h"
    #include "AD.h"
    #include "Parameter.h"
    #include "DisplayAndKey.h"
    #include "main.h"
    #include "StopAndRunState.h"
    #include "CalibState.h"
    #include "ADtoWeight.h"
    #include "SetupState.h"
    #include "Recipe.h"
    #include "TimeAndDate.h"
    #include "BatchState.h"
    #include "Sum.h"
    #include "ShutdownState.h"
    #include "RunState.h"
    #include "UART.h"
    #include "Modbus.h"
    #include "DebugState.h"
    #include "PrintState.h"
    #include "PROFIBUS_DP.h"

#endif
