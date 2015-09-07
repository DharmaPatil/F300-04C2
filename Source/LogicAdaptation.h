#ifndef __LogicAdaptation_H_
#define __LogicAdaptation_H_


#define PRODUCT_PARAM_DELAY_TIME_UNIT	 100
/*******************************************************************************
��ע��: ����Ӧ����
*******************************************************************************/
typedef	struct
{
    int32_t  *ps32Weight;			// ��ǰ����,  ����ë���ذ�װ���ã�ָ��ͬ�ĵ�ַ�� ���ڼ���ǰ��λָ��
    int32_t  *const ps32SP1;		// ָ���Ͷ��ǰ��������ʱ�ã�
    int32_t  *const ps32SP2;		// ָ����Ͷ��ǰ��������ʱ�ã�
    int32_t  *const ps32SP3;		// ָ�������ǰ��������ʱ�ã�
    int32_t  *ps32SP12;				// ָ���Ͷ����Ͷ��ǰ��������ʱ�ã�
    uint32_t *const pu32SP3RealTime;// ָ��ʵ�ʲ�õ�����ʱ��

	uint32_t u32AdatpEn;			// ����Ӧ����
	
	// ��/��Ͷ������
	bool 	 bFirstIn;				// ֹͣ����1�����buf�����㣬�������buf
	int32_t  s32RushWt;				// ������
	int32_t  s32SP1Bkp;				// ��Ͷ����ֵ�����ݣ�
	int32_t  s32SP2Bkp;				// ��Ͷ����ֵ�����ݣ�
	
	uint32_t u32SP12En;				// ��/��Ͷ��������
    uint32_t u32SP12Range;			// ��/��Ͷ������Χ
    uint32_t u32SP12Scope;			// ��/��Ͷ��������
    uint32_t u32SP12Count;			// ��/��Ͷ��ǰ��¼����		��ֵֻ���ϵ��������������ʱ����
    uint32_t u32SP12SubCount;		// ��/��Ͷ��ǰ��¼����		��ֵֻ���ϵ��������������ʱ����

	
	// ���������
	int32_t  s32ConfirmWt;			// ��ֵ�����������Ƿ��ϣ����¼����ǰ�����������޲��ϣ���ֵΪ0
    int32_t  s32TargetVal;			// Ŀ��ֵ
	int32_t  s32SP3Bkp;				// �������ֵ�����ݣ�
	int32_t  s32SP3RealWt;			// ʵ��СͶֹͣ����
	
	uint32_t u32SP3ForbidTime;		// СͶ����ʱ��
	uint32_t u32SP3En;				// �����������
    uint32_t u32SP3Range;			// ���������Χ
    uint32_t u32SP3Scope;			// �����������
    uint32_t u32SP3Count;			// ��ǰ��¼����		��ֵֻ���ϵ��������������ʱ����
}_strAdapt;
extern _strAdapt 	gstrAdapt;


/*******************************************************************************
			����ӿ�
*******************************************************************************/
void Adapt_InitSP12Temp(void);					// ��ʼ����/��Ͷ�����ݴ�ֵ
void Adapt_InitSP3Temp(void);					// ��ʼ����������ݴ�ֵ
void Adapt_InitData(void);						// ��ʼ�������������
uint32_t Adapt_Process(_strAdapt *pAdapt);		// ����Ӧ����  
void Bubble_Sort(int32_t *num, uint32_t n);

#endif

/***************************************The End Of This File***************************************/
