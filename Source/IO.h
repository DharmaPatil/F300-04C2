#ifndef _IO_H
#define _IO_H

//#define IO_INMAX    23              //�����������Զ������ֵ
//#define IO_INMAX    24              //�����������Զ������ֵ v1.01 ��������/��ͣ����
#define IO_INMAX    25              //�����������Զ������ֵ v1.02 ���ӱ궨��
#define IO_OUTMAX   22              //����������Զ������ֵ

#define IO_PORT_INMAX   8
#define IO_PORT_OUTMAX  12


//IO
#define IO_IN        0
#define IO_OUT       1

#define IOTEST_CHECKCODE    0
#define IOTEST              1
#define IOTEST_SWITCH       2
#define IOTEST_SETCODE      3
#define IOTEST_TESTDISP     4

//���������״̬�жϺ�����ʹ��Sp_Disc_State();
#define SP1_STATE   0
#define SP2_STATE   1
#define SP3_STATE   2
#define DISC_STATE  3

#define IODEF_CHECKCODE    0
#define IODEF_IN1          1
#define IODEF_IN2          2
#define IODEF_IN3          3
#define IODEF_IN4          4
#define IODEF_IN5          5
#define IODEF_IN6          6
#define IODEF_IN7          7
#define IODEF_IN8          8
#define IODEF_OUT1         9
#define IODEF_OUT2         10
#define IODEF_OUT3         11
#define IODEF_OUT4         12
#define IODEF_OUT5         13
#define IODEF_OUT6         14
#define IODEF_OUT7         15
#define IODEF_OUT8         16
#define IODEF_OUT9         17
#define IODEF_OUT10        18
#define IODEF_OUT11        19
#define IODEF_OUT12        20
#define IODEF_SWITCH       21
#define IODEF_SETCODE      22


#define IN1_READ    GPIO_BitRead(GPIO2,15)
#define IN2_READ    GPIO_BitRead(GPIO2,14)
#define IN3_READ    GPIO_BitRead(GPIO2,13)
#define IN4_READ    GPIO_BitRead(GPIO2,12)
#define IN5_READ    GPIO_BitRead(GPIO2,11)
#define IN6_READ    GPIO_BitRead(GPIO2,10)
#define IN7_READ    GPIO_BitRead(GPIO2,7)
#define IN8_READ    GPIO_BitRead(GPIO2,6)

//ע��CLR״̬ʱ�Ǳ�ʵ�ʿ��������״̬Ϊ��Ч��SETΪ��Ч
#define OUT1_CLR    GPIO_BitWrite(GPIO2,5,1)
#define OUT1_SET    GPIO_BitWrite(GPIO2,5,2)
#define OUT2_CLR    GPIO_BitWrite(GPIO2,4,1)
#define OUT2_SET    GPIO_BitWrite(GPIO2,4,0)
#define OUT3_CLR    GPIO_BitWrite(GPIO2,3,1)
#define OUT3_SET    GPIO_BitWrite(GPIO2,3,0)
#define OUT4_CLR    GPIO_BitWrite(GPIO2,2,1)
#define OUT4_SET    GPIO_BitWrite(GPIO2,2,0)
#define OUT5_CLR    GPIO_BitWrite(GPIO0,14,1)
#define OUT5_SET    GPIO_BitWrite(GPIO0,14,0)
#define OUT6_CLR    GPIO_BitWrite(GPIO0,13,1)
#define OUT6_SET    GPIO_BitWrite(GPIO0,13,0)
#define OUT7_CLR    GPIO_BitWrite(GPIO2,1,1)
#define OUT7_SET    GPIO_BitWrite(GPIO2,1,0)
#define OUT8_CLR    GPIO_BitWrite(GPIO2,0,1)
#define OUT8_SET    GPIO_BitWrite(GPIO2,0,0)
#define OUT9_CLR    GPIO_BitWrite(GPIO0,12,1)
#define OUT9_SET    GPIO_BitWrite(GPIO0,12,0)
#define OUT10_CLR   GPIO_BitWrite(GPIO0,7,1)
#define OUT10_SET   GPIO_BitWrite(GPIO0,7,0)
#define OUT11_CLR   GPIO_BitWrite(GPIO0,6,1)
#define OUT11_SET   GPIO_BitWrite(GPIO0,6,0)
#define OUT12_CLR   GPIO_BitWrite(GPIO0,5,1)
#define OUT12_SET   GPIO_BitWrite(GPIO0,5,0)

extern u8 const cu8_MultiAd[];

extern u32 gu32_IoInBuf_BINYES[9];              //�����������Զ���  gu32_IoInBuf_BINYES[1~8]
extern u32 gu32_IoOutBuf_BINYES[13];             //����������Զ���  gu32_IoOutBuf_BINYES[1~12]
extern u32 gu32_IoInBuf_BINNO[9];              //�����������Զ���
extern u32 gu32_IoOutBuf_BINNO[13];             //����������Զ���
extern u32 gu32_IoInBuf_BIN_2[9];              //�����������Զ���
extern u32 gu32_IoOutBuf_BIN_2[13];             //����������Զ���
extern u32 gu32_IoInBuf_NO_2[9];              //�����������Զ���
extern u32 gu32_IoOutBuf_NO_2[13];             //����������Զ���

extern u32 *gu32p_IoInBuf[9];     //�����������Զ�������ָ��
extern u32 *gu32p_IoOutBuf[13];    //����������Զ�������ָ��

extern bool  gb_JSDDelayFlg;               //���ɴ���ʱ������־
extern bool  gb_SDDelayFlg;                //�ɴ���ʱ������־
extern bool  gb_SDBDelayFlg;               //�ɴ�ǰ��ʱ������־
extern u32 gu32_JSDTime;                  //���ɴ���ʱʱ��Ĵ���
extern u32 gu32_FlgBright;
extern u32 gu32_IoTestCode;        //��������������
extern u32 gu32_IoTestCodeSwitch;
extern u32 gu8_IoTestInTime;
extern u32 gu32_IoDefCode;        //��������������
extern u32 gu32_IoDefCodeSwitch;
extern u32 gu8_IoDefInTime;
extern u32 gu32_SmallPluseTimeBuf;           //СͶ�㶯��ʱ��Ĵ���
extern u32 gu32_AutoClrZeroCnt;         //�Զ�������������
extern u32 gu32_CorrectFallTimeCnt;     //�����������������
extern u32 gu32_CorrectFallWt;          //��������ۼ�ֵ
extern bool gb_DiscShakeFlg;           //ж��������
extern bool gb_DiscShakeStartFlg;      //ж�����ʼ
extern bool  gb_DiscSumOnFlg;                //�ֶ�ж���ۼƿ���ж�ϱ�־λ
extern bool  gb_DiscSumOffFlg;               //�ֶ�ж���ۼƹر�ж�ϱ�־λ

extern bool gb_RunFlg;
extern bool gb_Sp1Flg;
extern bool gb_Sp2Flg;
extern bool gb_Sp3Flg;
extern bool gb_CompFlg;
extern bool gb_OverFlg;
extern bool gb_UnderFlg;
extern bool gb_OUOverFlg;            //��Ƿ����ͣ�ر�ʱ��Ƿ��ı�����־ OVER
extern bool gb_OUUnderFlg;           //��Ƿ����ͣ�ر�ʱ��Ƿ��ı�����־ UNDER
extern bool gb_PackFlg;
extern bool gb_DiscFlg;
extern bool gb_FillFlg;
extern bool gb_BatchOverFlg;
extern bool gb_LackFlg;
extern bool gb_KeyLockFlg;
extern bool gb_FirstStartFlg;           //�������һ�ӱ�־λ
extern bool gb_OflDelayFlg;             //����ʱ�����־
extern bool  gb_HighDefineFlg;             //����λ�Ѷ����־
extern bool  gb_LowDefineFlg;              //����λ�Ѷ����־
extern bool  gb_HighFlg;                   //����λ
extern bool  gb_LowFlg;                    //����λ
extern bool  gb_MiddleFlg;                 //����λ
extern bool  gb_LackFlg;                   //ȱ��
extern bool gb_PrintCodeOutFlg;        //�������
extern bool gb_EndFlg;                 //ֹͣ����
extern bool  gb_DiscCloseOKFlg;          //ж���Ź��ŵ�λ����
extern bool  gb_EnDiscCloseOKFlg;        //ж���Ź��ŵ�λ�Ѷ����־
extern bool  gb_PackOKFlg;             //�д���λ����
extern bool  gb_EnPackOKFlg;           //�д���λ�Ѷ����־
extern bool gb_LiftBagFlg;             //����ź�
extern bool gb_PushBagFlg;             //�ƴ��ź�
extern bool gb_DiscEnableOutFlg;       //˫�ӻ������ ������һ��ж��
extern bool gb_DiscEnableInFlg;        //˫�ӻ������� ����ж��
extern bool gb_AddingInFlg;            //�޶�˫�ӻ������� ���Ϲ���
extern bool gb_AddingOutFlg;           //�޶�˫�ӻ������ ���Ϲ������
extern bool gb_GoFlg;
extern bool gb_ConveyorRunFlg;         //���ͻ�����
extern bool gb_No2PackOffAnother;      //��һ���Ѿ��ɴ���־
extern bool gb_No2AddAgainDelay;        //˫���޶���ʱ�������ϱ�־
extern bool gb_CalLockDefFlg;          //�궨���Ѷ����־λ
extern bool gb_CalLockFlg;              //�궨������

extern void IO_Init(void);
extern void IO_CloseOut(void);
extern void Key_IoTestState(void);
extern void Disp_IoTestState(void);
extern void Key_IoDefState(void);
extern void Disp_IoDefState(void);
extern void IO_InScan(void);
extern void IO_OutPut(void);
extern void Deal_Pack(void);
extern void Clr_Alarm(void);
extern void RunStopInit(bool fb_RunStop);
extern bool Sp_Disc_State(u8 fu8_Item);
extern bool Feed_SP(u8 fu8_Item);
extern u32 NextRecNum(u32 fu32_Num);
extern void IoDefPointerRefresh(u32 fu32_TareMode);
extern u16 GetIoAddr(u8 fu8_i, u8 fu8_Num, u32 fu32_TareMode);
extern void PackInput(void);
extern void Pack_JD(void);
extern void Pack_SD(void);
extern void ConveyorDeal(bool *fbp_ConveyorStart);
extern void ManualDisc(u8 fu8_State);
extern void ManualSp123(u8 fu8_State);
extern void ManualSp3(u8 fu8_State);

#endif
