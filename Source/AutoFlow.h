#ifndef _AUTOFLOW_H
#define _AUTOFLOW_H


typedef struct
{   //����Ӧ��ʼ������ز���
    u32 u32Sp2SampleTime;       //��Ͷ����ʱ��
    u32 u32Sp3SampleTime;       //СͶ����ʱ��
    
}AutoFlowStart;

typedef struct
{   //����Ӧ���Ϲ�����ز���
    u32 u32Sp2Time,             //��Ͷʱ��
    u32 u32Sp2Weight,           //��Ͷ��Ͷ����
    u32 u32Sp2Flux,             //��Ͷ����
    u32 u32Sp2Lead,             //��Ͷ��ǰ��
    u32 u32Sp3Time,             //СͶʱ��
    u32 u32Sp3Weight,           //СͶ��Ͷ����
    u32 u32Sp3Flux,             //СͶ����
    u32 u32Sp3Lead,             //СͶ��ǰ��
}AotoFlowProcess;





#endif
