#ifndef _F_RAM_H
#define _F_RAM_H

#include "UserLib.h"



#define SUPERCODE       880406          //��������,�ͺ���������
#define RESETCODE       880432          //��λ����
#define BACKUPCODE      880404          //������������
#define RELOADCODE      880401          //�����ָ�����
#define SUPERSHUTCODE   8110230         //��ʱ�ػ�ͨ�ó�������


#define CBL     0x0400  //Class Bank Length:�������С  1KB

#define BANK0_BASE      (CBL*0)         //Bank1 ����
#define BANK1_BASE      (CBL*1)         //���Ρ��ۼơ������Ȳ���
#define BANK2_BASE      (CBL*2)
#define CAL_BASE        BANK2_BASE      //�궨������ʼ��ַ
#define BANK3_BASE      (CBL*3)
#define SETUP_BASE      BANK3_BASE      //����������ʼ��ַ  ���������Զ��� ��������
#define BANK4_BASE      (CBL*4)         //�䷽����
#define BANK5_BASE      (CBL*5)         //�䷽ռ��
#define BANK6_BASE      (CBL*6)         //�䷽ռ��
#define BANK7_BASE      (CBL*7)         //�䷽ռ��
#define BANK8_BASE      (CBL*8)         //�䷽ռ��
#define BANK9_BASE      (CBL*9)         //�䷽ռ��

#define BANK10_BASE     (CBL*10)
#define BANK11_BASE     (CBL*11)
#define BANK12_BASE     (CBL*12)
#define BANK13_BASE     (CBL*13)
#define BANK14_BASE     (CBL*14)
#define BANK15_BASE     (CBL*15)
//ǰ16KB����ƽʱ�Ĳ����洢
//��16KB�����������ݣ�ƽʱ�Ǳ�����״̬��ֹ���ݱ��ģ�ֻ���ڱ��ݵ�ʱ���Ǵ򿪱���

#define BACKUPSTART     BANK16_BASE      //����������ʼ��ַ
#define BANK16_BASE     (CBL*16)    //��������3
#define BANK17_BASE     (CBL*17)    //��������4
#define BANK18_BASE     (CBL*18)    //��������5
#define BANK19_BASE     (CBL*19)    //��������6
#define BANK20_BASE     (CBL*20)    //��������7
#define BANK21_BASE     (CBL*21)    //��������8
#define BANK22_BASE     (CBL*22)    //��������9
#define BANK23_BASE     (CBL*23)    //��������10

#define BANK24_BASE     (CBL*24)    //��������11
#define BANK25_BASE     (CBL*25)    //��������12
#define BANK26_BASE     (CBL*26)    //��������13
#define BANK27_BASE     (CBL*27)    //��������14
#define BANK28_BASE     (CBL*28)    //��������15
#define BANK29_BASE     (CBL*29)
#define BANK30_BASE     (CBL*30)
#define BANK31_BASE     (CBL*31)


//���� BANK1_BASE-----------------------------------------------------------------------------------------
//�䷽�ۼ����� ռ4�ֽ�
#define FMAddr_SumWt_1              (BANK1_BASE+0x000)       //�䷽1�ۼ�����
#define FMAddr_SumWt_2              (BANK1_BASE+0x004)       //�䷽2�ۼ�����
#define FMAddr_SumWt_3              (BANK1_BASE+0x008)       //�䷽3�ۼ�����
#define FMAddr_SumWt_4              (BANK1_BASE+0x00C)       //�䷽4�ۼ�����
#define FMAddr_SumWt_5              (BANK1_BASE+0x010)       //�䷽5�ۼ�����
#define FMAddr_SumWt_6              (BANK1_BASE+0x014)       //�䷽6�ۼ�����
#define FMAddr_SumWt_7              (BANK1_BASE+0x018)       //�䷽7�ۼ�����
#define FMAddr_SumWt_8              (BANK1_BASE+0x01C)       //�䷽8�ۼ�����
#define FMAddr_SumWt_9              (BANK1_BASE+0x020)       //�䷽9�ۼ�����
#define FMAddr_SumWt_10             (BANK1_BASE+0x024)       //�䷽10�ۼ�����
#define FMAddr_SumWt_11             (BANK1_BASE+0x028)       //�䷽11�ۼ�����
#define FMAddr_SumWt_12             (BANK1_BASE+0x02C)       //�䷽12�ۼ�����
#define FMAddr_SumWt_13             (BANK1_BASE+0x030)       //�䷽13�ۼ�����
#define FMAddr_SumWt_14             (BANK1_BASE+0x034)       //�䷽14�ۼ�����
#define FMAddr_SumWt_15             (BANK1_BASE+0x038)       //�䷽15�ۼ�����
#define FMAddr_SumWt_16             (BANK1_BASE+0x03C)       //�䷽16�ۼ�����
#define FMAddr_SumWt_17             (BANK1_BASE+0x040)       //�䷽17�ۼ�����
#define FMAddr_SumWt_18             (BANK1_BASE+0x044)       //�䷽18�ۼ�����
#define FMAddr_SumWt_19             (BANK1_BASE+0x048)       //�䷽19�ۼ�����
#define FMAddr_SumWt_20             (BANK1_BASE+0x04C)       //�䷽20�ۼ�����
//�䷽�ۼƴ��� ռ4�ֽ�
#define FMAddr_SumNum_1             (BANK1_BASE+0x09C)       //�䷽1�ۼƴ���
#define FMAddr_SumNum_2             (BANK1_BASE+0x010)       //�䷽2�ۼƴ���
#define FMAddr_SumNum_3             (BANK1_BASE+0x114)       //�䷽3�ۼƴ���
#define FMAddr_SumNum_4             (BANK1_BASE+0x118)       //�䷽4�ۼƴ���
#define FMAddr_SumNum_5             (BANK1_BASE+0x11C)       //�䷽5�ۼƴ���
#define FMAddr_SumNum_6             (BANK1_BASE+0x120)       //�䷽6�ۼƴ���
#define FMAddr_SumNum_7             (BANK1_BASE+0x124)       //�䷽7�ۼƴ���
#define FMAddr_SumNum_8             (BANK1_BASE+0x128)       //�䷽8�ۼƴ���
#define FMAddr_SumNum_9             (BANK1_BASE+0x12C)       //�䷽9�ۼƴ���
#define FMAddr_SumNum_10            (BANK1_BASE+0x130)       //�䷽10�ۼƴ���
#define FMAddr_SumNum_11            (BANK1_BASE+0x134)       //�䷽11�ۼƴ���
#define FMAddr_SumNum_12            (BANK1_BASE+0x138)       //�䷽12�ۼƴ���
#define FMAddr_SumNum_13            (BANK1_BASE+0x13C)       //�䷽13�ۼƴ���
#define FMAddr_SumNum_14            (BANK1_BASE+0x140)       //�䷽14�ۼƴ���
#define FMAddr_SumNum_15            (BANK1_BASE+0x144)       //�䷽15�ۼƴ���
#define FMAddr_SumNum_16            (BANK1_BASE+0x148)       //�䷽16�ۼƴ���
#define FMAddr_SumNum_17            (BANK1_BASE+0x14C)       //�䷽17�ۼƴ���
#define FMAddr_SumNum_18            (BANK1_BASE+0x150)       //�䷽18�ۼƴ���
#define FMAddr_SumNum_19            (BANK1_BASE+0x154)       //�䷽19�ۼƴ���
#define FMAddr_SumNum_20            (BANK1_BASE+0x158)       //�䷽20�ۼƴ���

#define FMAddr_SumWt                (BANK1_BASE+0x15C)       //�ۼ����� 8�ֽ�
#define FMAddr_SumNum               (BANK1_BASE+0x164)       //�ۼƴ���
#define FMAddr_BatchTime            (BANK1_BASE+0X168)       //������
#define FMAddr_BatchTimeCnt         (BANK1_BASE+0x16C)       //ʣ��������


//#define FMAddr_OutSumWt             (BANK1_BASE+0x18C)       //�շ����ۼ�
//#define FMAddr_OutTarget            (BANK1_BASE+0x190)       //��������
//#define FMAddr_OutEndNoRun          (BANK1_BASE+0x194)       //������δ������־
//#define FMAddr_SFJHeight            (BANK1_BASE+0x19C)       //���ͻ��߶�

#define FMAddr_BuyerID              (BANK1_BASE+0x200)       //�û�ID
#define FMAddr_ShutDownCode         (BANK1_BASE+0x204)       //�Զ���������
#define FMAddr_LeaveDays            (BANK1_BASE+0x208)       //����ʣ������
#define FMAddr_ShutDownSwitch       (BANK1_BASE+0x20C)       //��ʱ��������
#define FMAddr_CmpYear              (BANK1_BASE+0x210)       //�����Ƚ���
#define FMAddr_CmpMonth             (BANK1_BASE+0x214)       //�����Ƚ��·�
#define FMAddr_CmpDate              (BANK1_BASE+0x218)       //�����Ƚ�����
#define FMAddr_GetedRandom          (BANK1_BASE+0x21C)       //������Ѿ���ȡ
#define FMAddr_RandomID             (BANK1_BASE+0x220)       //�����

#define FMAddr_BackupYear           (BANK1_BASE+0x230)       //�������� ��
#define FMAddr_BackupMonth          (BANK1_BASE+0x234)       //�������� ��
#define FMAddr_BackupDate           (BANK1_BASE+0x238)       //�������� ��
#define FMAddr_Backuped             (BANK1_BASE+0x23C)       //�б��ݹ�����


//#define FMAddr_SumWt_Total          (BANK1_BASE+0x240)       //ϵͳ���ۼ�����8�ֽ�
//#define FMAddr_SumNum_Total         (BANK1_BASE+0x248)       //ϵͳ���ۼƴ���
//#define FMAddr_NoSumWt              (BANK1_BASE+0x24C)       //δ�ۼ�����8�ֽ�
//#define FMAddr_NoSumNum             (BANK1_BASE+0x254)       //δ�ۼƴ���

#define FMAddr_ModelSetSwitch       (BANK1_BASE+0x300)      //�ͺ��Զ��忪��
#define FMAddr_ModelSet0            (BANK1_BASE+0x304)      //logo����
#define FMAddr_ModelSet1            (BANK1_BASE+0x308)      //�����1λ
#define FMAddr_ModelSet2            (BANK1_BASE+0x30C)      //�����2λ
#define FMAddr_ModelSet3            (BANK1_BASE+0x310)      //�����3λ
#define FMAddr_ModelSet4            (BANK1_BASE+0x314)      //�����4λ
#define FMAddr_ModelSet5            (BANK1_BASE+0x318)      //�����5λ
#define FMAddr_ModelSet6            (BANK1_BASE+0x31C)      //�����6λ
#define FMAddr_HadBeenInited        (BANK1_BASE+0X320)      //�Ǳ��Ѿ���ʼ����

//CAL�� BANK2_BASE--------------------------------------------------------------------------
#define FMAddr_CalCode              (CAL_BASE+0x00)        //�궨����
#define FMAddr_CalUnit              (CAL_BASE+0x04)
#define FMAddr_CalPoint             (CAL_BASE+0x08)
#define FMAddr_CalDiv               (CAL_BASE+0x0C)
#define FMAddr_CalCap               (CAL_BASE+0x10)
#define FMAddr_CalSensor            (CAL_BASE+0x14)
#define FMAddr_CalZero              (CAL_BASE+0x18)
#define FMAddr_CalADK               (CAL_BASE+0x1C)


//SETUP�� BANK3_BASE-------------------------------------------------------------------------
#define FMAddr_SetupCodeSwitch      (SETUP_BASE+0x00)        //�����������뿪��
#define FMAddr_SetupCode            (SETUP_BASE+0x04)        //������������

#define FMAddr_TareMode             (SETUP_BASE+0x08)   //B1
#define FMAddr_GorNMode             (SETUP_BASE+0x0C)
#define FMAddr_FeedMode             (SETUP_BASE+0x10)
#define FMAddr_AutoClrZeroInterval  (SETUP_BASE+0x14)
#define FMAddr_JudgeStabOverTime    (SETUP_BASE+0x18)
#define FMAddr_AuxDispType          (SETUP_BASE+0x1C)
#define FMAddr_ClrZeroRng           (SETUP_BASE+0x20)   //B2
#define FMAddr_JudgeStabRng         (SETUP_BASE+0x24)
#define FMAddr_TraceZeroRng         (SETUP_BASE+0x28)
#define FMAddr_AdFilterLevel        (SETUP_BASE+0x2C)
#define FMAddr_AdFilterLevelDisc    (SETUP_BASE+0x30)
#define FMAddr_AdFilterLevelComp    (SETUP_BASE+0x34)
#define FMAddr_AdSamplingRate       (SETUP_BASE+0x38)
#define FMAddr_ExtType              (SETUP_BASE+0x3C)
#define FMAddr_PowerOnZeroSwitch    (SETUP_BASE+0x40)   //B3
#define FMAddr_DiscSumSwitch        (SETUP_BASE+0x44)
#define FMAddr_ResultHoldSwitch     (SETUP_BASE+0x48)
#define FMAddr_SumCheckCodeSwitch   (SETUP_BASE+0x4C)
#define FMAddr_SumClrCodeSwitch     (SETUP_BASE+0x50)
#define FMAddr_SumExtendSwitch      (SETUP_BASE+0x54)
#define FMAddr_ComMode_1            (SETUP_BASE+0x58)   //B4
#define FMAddr_TareNumber_1         (SETUP_BASE+0x5C)
#define FMAddr_BaudRate_1           (SETUP_BASE+0x60)
#define FMAddr_ParityMode_1         (SETUP_BASE+0x64)
#define FMAddr_HiLo_1               (SETUP_BASE+0x68)
#define FMAddr_PrintSwitch_1        (SETUP_BASE+0x6C)
#define FMAddr_PrintType_1          (SETUP_BASE+0x70)
#define FMAddr_PrintLanguage_1      (SETUP_BASE+0x74)
#define FMAddr_PrintBlankRows_1     (SETUP_BASE+0x78)
#define FMAddr_ComMode_2            (SETUP_BASE+0x7C)   //B5
#define FMAddr_TareNumber_2         (SETUP_BASE+0x80)
#define FMAddr_BaudRate_2           (SETUP_BASE+0x84)
#define FMAddr_ParityMode_2         (SETUP_BASE+0x88)
#define FMAddr_HiLo_2               (SETUP_BASE+0x8C)
#define FMAddr_PrintSwitch_2        (SETUP_BASE+0x90)
#define FMAddr_PrintType_2          (SETUP_BASE+0x94)
#define FMAddr_PrintLanguage_2      (SETUP_BASE+0x98)
#define FMAddr_PrintBlankRows_2     (SETUP_BASE+0x9C)
#define FMAddr_No2PackOffTwoSwitch  (SETUP_BASE+0xA0)   //B6
#define FMAddr_No2ConveyorDelayTime (SETUP_BASE+0xA4)
#define FMAddr_No2ConveyorRunTime   (SETUP_BASE+0xA8)
#define FMAddr_No2AddAgainDelayTime (SETUP_BASE+0xAC)
#define FMAddr_AdaptSW               (SETUP_BASE+0xB0)  //B7
#define FMAddr_AdaptSP2SW            (SETUP_BASE+0xB4)
#define FMAddr_AdaptSP2LimitRng      (SETUP_BASE+0xB8)
#define FMAddr_AdaptSP2Level         (SETUP_BASE+0xBC)
#define FMAddr_AdaptSP2TimeThreshold (SETUP_BASE+0xC0)
#define FMAddr_AdaptSP3SW            (SETUP_BASE+0xC4)
#define FMAddr_AdaptSP3LimitRng      (SETUP_BASE+0xC8)
#define FMAddr_AdaptSP3Scope         (SETUP_BASE+0xCC)
#define FMAddr_AdaptSP2Scope         (SETUP_BASE+0xD0)
#define FMAddr_Adapt7_9              (SETUP_BASE+0xD4)






#define FMAddr_GSDType              (SETUP_BASE+0xF0)
#define FMAddr_FollowMasterSwitch   (SETUP_BASE+0xF4)

//���뿪����BINYES
#define FMAddr_IoInBuf1             (SETUP_BASE+0x100)
#define FMAddr_IoInBuf2             (SETUP_BASE+0x104)
#define FMAddr_IoInBuf3             (SETUP_BASE+0x108)
#define FMAddr_IoInBuf4             (SETUP_BASE+0x10C)
#define FMAddr_IoInBuf5             (SETUP_BASE+0x110)
#define FMAddr_IoInBuf6             (SETUP_BASE+0x114)
#define FMAddr_IoInBuf7             (SETUP_BASE+0x118)
#define FMAddr_IoInBuf8             (SETUP_BASE+0x11C)
//���������BINYES
#define  FMAddr_IoOutBuf1           (SETUP_BASE+0x120)
#define  FMAddr_IoOutBuf2           (SETUP_BASE+0x124)
#define  FMAddr_IoOutBuf3           (SETUP_BASE+0x128)
#define  FMAddr_IoOutBuf4           (SETUP_BASE+0x12C)
#define  FMAddr_IoOutBuf5           (SETUP_BASE+0x130)
#define  FMAddr_IoOutBuf6           (SETUP_BASE+0x134)
#define  FMAddr_IoOutBuf7           (SETUP_BASE+0x138)
#define  FMAddr_IoOutBuf8           (SETUP_BASE+0x13C)
#define  FMAddr_IoOutBuf9           (SETUP_BASE+0x140)
#define  FMAddr_IoOutBuf10          (SETUP_BASE+0x144)
#define  FMAddr_IoOutBuf11          (SETUP_BASE+0x148)
#define  FMAddr_IoOutBuf12          (SETUP_BASE+0x14C)
//���뿪����BINNO
#define FMAddr_IoInBuf1_BINNO             (SETUP_BASE+0x150)
#define FMAddr_IoInBuf2_BINNO             (SETUP_BASE+0x154)
#define FMAddr_IoInBuf3_BINNO             (SETUP_BASE+0x158)
#define FMAddr_IoInBuf4_BINNO             (SETUP_BASE+0x15C)
#define FMAddr_IoInBuf5_BINNO             (SETUP_BASE+0x160)
#define FMAddr_IoInBuf6_BINNO             (SETUP_BASE+0x164)
#define FMAddr_IoInBuf7_BINNO             (SETUP_BASE+0x168)
#define FMAddr_IoInBuf8_BINNO             (SETUP_BASE+0x16C)
//���������BINNO
#define  FMAddr_IoOutBuf1_BINNO           (SETUP_BASE+0x170)
#define  FMAddr_IoOutBuf2_BINNO           (SETUP_BASE+0x174)
#define  FMAddr_IoOutBuf3_BINNO           (SETUP_BASE+0x178)
#define  FMAddr_IoOutBuf4_BINNO           (SETUP_BASE+0x17C)
#define  FMAddr_IoOutBuf5_BINNO           (SETUP_BASE+0x180)
#define  FMAddr_IoOutBuf6_BINNO           (SETUP_BASE+0x184)
#define  FMAddr_IoOutBuf7_BINNO           (SETUP_BASE+0x188)
#define  FMAddr_IoOutBuf8_BINNO           (SETUP_BASE+0x18C)
#define  FMAddr_IoOutBuf9_BINNO           (SETUP_BASE+0x190)
#define  FMAddr_IoOutBuf10_BINNO          (SETUP_BASE+0x194)
#define  FMAddr_IoOutBuf11_BINNO          (SETUP_BASE+0x198)
#define  FMAddr_IoOutBuf12_BINNO          (SETUP_BASE+0x19C)
//���뿪����BIN_2
#define FMAddr_IoInBuf1_BIN_2             (SETUP_BASE+0x1A0)
#define FMAddr_IoInBuf2_BIN_2             (SETUP_BASE+0x1A4)
#define FMAddr_IoInBuf3_BIN_2             (SETUP_BASE+0x1A8)
#define FMAddr_IoInBuf4_BIN_2             (SETUP_BASE+0x1AC)
#define FMAddr_IoInBuf5_BIN_2             (SETUP_BASE+0x1B0)
#define FMAddr_IoInBuf6_BIN_2             (SETUP_BASE+0x1B4)
#define FMAddr_IoInBuf7_BIN_2             (SETUP_BASE+0x1B8)
#define FMAddr_IoInBuf8_BIN_2             (SETUP_BASE+0x1BC)
//���������BIN_2
#define  FMAddr_IoOutBuf1_BIN_2           (SETUP_BASE+0x1C0)
#define  FMAddr_IoOutBuf2_BIN_2           (SETUP_BASE+0x1C4)
#define  FMAddr_IoOutBuf3_BIN_2           (SETUP_BASE+0x1C8)
#define  FMAddr_IoOutBuf4_BIN_2           (SETUP_BASE+0x1CC)
#define  FMAddr_IoOutBuf5_BIN_2           (SETUP_BASE+0x1D0)
#define  FMAddr_IoOutBuf6_BIN_2           (SETUP_BASE+0x1D4)
#define  FMAddr_IoOutBuf7_BIN_2           (SETUP_BASE+0x1D8)
#define  FMAddr_IoOutBuf8_BIN_2           (SETUP_BASE+0x1DC)
#define  FMAddr_IoOutBuf9_BIN_2           (SETUP_BASE+0x1E0)
#define  FMAddr_IoOutBuf10_BIN_2          (SETUP_BASE+0x1E4)
#define  FMAddr_IoOutBuf11_BIN_2          (SETUP_BASE+0x1E8)
#define  FMAddr_IoOutBuf12_BIN_2          (SETUP_BASE+0x1EC)
//���뿪����NO_2
#define FMAddr_IoInBuf1_NO_2             (SETUP_BASE+0x1F0)
#define FMAddr_IoInBuf2_NO_2             (SETUP_BASE+0x1F4)
#define FMAddr_IoInBuf3_NO_2             (SETUP_BASE+0x1F8)
#define FMAddr_IoInBuf4_NO_2             (SETUP_BASE+0x1FC)
#define FMAddr_IoInBuf5_NO_2             (SETUP_BASE+0x200)
#define FMAddr_IoInBuf6_NO_2             (SETUP_BASE+0x204)
#define FMAddr_IoInBuf7_NO_2             (SETUP_BASE+0x208)
#define FMAddr_IoInBuf8_NO_2             (SETUP_BASE+0x20C)
//���������NO_2
#define  FMAddr_IoOutBuf1_NO_2           (SETUP_BASE+0x210)
#define  FMAddr_IoOutBuf2_NO_2           (SETUP_BASE+0x214)
#define  FMAddr_IoOutBuf3_NO_2           (SETUP_BASE+0x218)
#define  FMAddr_IoOutBuf4_NO_2           (SETUP_BASE+0x21C)
#define  FMAddr_IoOutBuf5_NO_2           (SETUP_BASE+0x220)
#define  FMAddr_IoOutBuf6_NO_2           (SETUP_BASE+0x224)
#define  FMAddr_IoOutBuf7_NO_2           (SETUP_BASE+0x228)
#define  FMAddr_IoOutBuf8_NO_2           (SETUP_BASE+0x22C)
#define  FMAddr_IoOutBuf9_NO_2           (SETUP_BASE+0x230)
#define  FMAddr_IoOutBuf10_NO_2          (SETUP_BASE+0x234)
#define  FMAddr_IoOutBuf11_NO_2          (SETUP_BASE+0x238)
#define  FMAddr_IoOutBuf12_NO_2          (SETUP_BASE+0x23C)


//��������������
#define FMAddr_IoTestCodeSwitch           (SETUP_BASE+0x280)
#define FMAddr_IoTestCode                 (SETUP_BASE+0x284)
//�������Զ�������
#define FMAddr_IoDefCodeSwitch           (SETUP_BASE+0x288)
#define FMAddr_IoDefCode                 (SETUP_BASE+0x28C)

//����������Ŀ�Զ��������ַ
//#define FMAddr_FastSetDef_0         (SETUP_BASE+0x300)
//#define FMAddr_FastSetDef_1         (SETUP_BASE+0x304)
//#define FMAddr_FastSetDef_2         (SETUP_BASE+0x308)
//#define FMAddr_FastSetDef_3         (SETUP_BASE+0x30C)
//#define FMAddr_FastSetDef_4         (SETUP_BASE+0x310)
//#define FMAddr_FastSetDef_5         (SETUP_BASE+0x314)
//#define FMAddr_FastSetDef_6         (SETUP_BASE+0x318)
//#define FMAddr_FastSetDef_7         (SETUP_BASE+0x31C)
//#define FMAddr_FastSetDef_8         (SETUP_BASE+0x320)
//#define FMAddr_FastSetDef_9         (SETUP_BASE+0x324)
//#define FMAddr_FastSetDef_10        (SETUP_BASE+0x328)




//RECIPE�� BANK4_BASE----------------------------------------------------------------------------
//�䷽�ţ����뿪�غ�����ռ��12���ֽڵ�ַ
//ÿһ���䷽�·��� PER_REC_LENGTH ��ַ����������չ����ôÿ���䷽�����ĵ�ַ����Ϊ gu32_RecNum * PER_REC_LENGTH + ������ַ
//PER_REC_LENGTH=280ʱ����ռ��280��20+12=5612����ַ��ռ��6��CBL��
#define FMAddr_RecCodeSwitch        (BANK4_BASE+0x00)       //�䷢�������뿪��
#define FMAddr_RecCode              (BANK4_BASE+0x04)       //�䷽��������
#define FMAddr_RecNum               (BANK4_BASE+0x08)       //�䷽��
//�䷽����������ʼ��ַ,��20�飬���䷽�źţ�1~20��������(r)���䷽��
#define RECIPE_BASE     (BANK4_BASE+0x0C)                   //�䷽������ʼ��ַ
//ÿһ���䷽��ռ�õĳ���
#define PER_REC_LENGTH              280     //0x118

#define FMAddr_Target(r)                (RECIPE_BASE+0x00+(r-1)*PER_REC_LENGTH)//F1
#define FMAddr_Large(r)                 (RECIPE_BASE+0x04+(r-1)*PER_REC_LENGTH)
#define FMAddr_Middle(r)                (RECIPE_BASE+0x08+(r-1)*PER_REC_LENGTH)
#define FMAddr_Small(r)                 (RECIPE_BASE+0x0C+(r-1)*PER_REC_LENGTH)
#define FMAddr_NearZero(r)              (RECIPE_BASE+0x10+(r-1)*PER_REC_LENGTH)
#define FMAddr_FlapBagWt(r)             (RECIPE_BASE+0x14+(r-1)*PER_REC_LENGTH)
#define FMAddr_OUSwitch(r)              (RECIPE_BASE+0x18+(r-1)*PER_REC_LENGTH)//F2
#define FMAddr_OverValue(r)             (RECIPE_BASE+0x1C+(r-1)*PER_REC_LENGTH)
#define FMAddr_UnderValue(r)            (RECIPE_BASE+0x20+(r-1)*PER_REC_LENGTH)
#define FMAddr_OUStopSwitch(r)          (RECIPE_BASE+0x24+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT1(r)                (RECIPE_BASE+0x28+(r-1)*PER_REC_LENGTH)//F3
#define FMAddr_TimeT2b(r)               (RECIPE_BASE+0x2C+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT3(r)                (RECIPE_BASE+0x30+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT4(r)                (RECIPE_BASE+0x34+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT5(r)                (RECIPE_BASE+0x38+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT6(r)                (RECIPE_BASE+0x3C+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT7(r)                (RECIPE_BASE+0x40+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT2a(r)               (RECIPE_BASE+0x44+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT9(r)                (RECIPE_BASE+0x48+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT10(r)               (RECIPE_BASE+0x4C+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT11(r)               (RECIPE_BASE+0x50+(r-1)*PER_REC_LENGTH)
#define FMAddr_TimeT12(r)               (RECIPE_BASE+0x54+(r-1)*PER_REC_LENGTH)//F4 
#define FMAddr_CorrectFallSwitch(r)     (RECIPE_BASE+0x58+(r-1)*PER_REC_LENGTH)     
#define FMAddr_CorrectFallTime(r)       (RECIPE_BASE+0x5C+(r-1)*PER_REC_LENGTH)     
#define FMAddr_CorrectFallRng1(r)       (RECIPE_BASE+0x60+(r-1)*PER_REC_LENGTH)     
#define FMAddr_CorrectFallRng2(r)       (RECIPE_BASE+0x64+(r-1)*PER_REC_LENGTH)//F5 
#define FMAddr_FlapBagSwitch(r)         (RECIPE_BASE+0x68+(r-1)*PER_REC_LENGTH)     
#define FMAddr_FlapBagAddNum(r)         (RECIPE_BASE+0x6C+(r-1)*PER_REC_LENGTH)     
#define FMAddr_FlapBagNum(r)            (RECIPE_BASE+0x70+(r-1)*PER_REC_LENGTH)     
#define FMAddr_FlapBagDelay(r)          (RECIPE_BASE+0x74+(r-1)*PER_REC_LENGTH)     
#define FMAddr_FlapBagTimeHi(r)         (RECIPE_BASE+0x78+(r-1)*PER_REC_LENGTH)     
#define FMAddr_FlapBagTimeLow(r)        (RECIPE_BASE+0x7C+(r-1)*PER_REC_LENGTH)     
#define FMAddr_FlapBagExtTime(r)        (RECIPE_BASE+0x80+(r-1)*PER_REC_LENGTH)//F6 
#define FMAddr_SmallPluseSwitch(r)      (RECIPE_BASE+0x84+(r-1)*PER_REC_LENGTH)     
#define FMAddr_SmallPluseHi(r)          (RECIPE_BASE+0x88+(r-1)*PER_REC_LENGTH)     
#define FMAddr_SmallPluseLow(r)         (RECIPE_BASE+0x8C+(r-1)*PER_REC_LENGTH)//F7 
#define FMAddr_CombinedNum(r)           (RECIPE_BASE+0x90+(r-1)*PER_REC_LENGTH)//F8 
#define FMAddr_DiscShakeSwitch(r)       (RECIPE_BASE+0x94+(r-1)*PER_REC_LENGTH)     
#define FMAddr_DiscEffTime(r)           (RECIPE_BASE+0x98+(r-1)*PER_REC_LENGTH)     
#define FMAddr_DiscShakeTimeHi(r)       (RECIPE_BASE+0x9C+(r-1)*PER_REC_LENGTH)     
#define FMAddr_DiscShakeTimeLow(r)      (RECIPE_BASE+0xA0+(r-1)*PER_REC_LENGTH)     
#define FMAddr_DiscShakeNum(r)          (RECIPE_BASE+0xA4+(r-1)*PER_REC_LENGTH)//F9 
#define FMAddr_PrintCodeSwitch(r)       (RECIPE_BASE+0xA8+(r-1)*PER_REC_LENGTH)
#define FMAddr_PrintCodeRunDelay(r)     (RECIPE_BASE+0xAC+(r-1)*PER_REC_LENGTH)
#define FMAddr_PrintCodeTime(r)         (RECIPE_BASE+0xB0+(r-1)*PER_REC_LENGTH)
#define FMAddr_SheildDiscSwitch(r)      (RECIPE_BASE+0xB4+(r-1)*PER_REC_LENGTH)



//#define     (RECIPE_BASE+0xAC+(r-1)*PER_REC_LENGTH)
//#define     (RECIPE_BASE+0xB0+(r-1)*PER_REC_LENGTH)
//#define     (RECIPE_BASE+0xB4+(r-1)*PER_REC_LENGTH)
//#define     (RECIPE_BASE+0xB8+(r-1)*PER_REC_LENGTH)
//#define     (RECIPE_BASE+0xBC+(r-1)*PER_REC_LENGTH)
//#define     (RECIPE_BASE+0xC0+(r-1)*PER_REC_LENGTH)
//#define     (RECIPE_BASE+0xC4+(r-1)*PER_REC_LENGTH)





typedef union
{
	u8 uc[2];
	u16  ui;
}UIDATA;
typedef union
{
	u16	ui[2];
	u32 ul;
}ULIDATA;
typedef union
{
	u8 	uc[4];
	u16	ui[2];
	u32 ul;
}ULDATA;

typedef union
{
	u8 	uc[4];
	float f;
}FCDATA;


typedef union
{
    u8 	uc[8];
	u16	ui[4];
	u32 ul[2];
	u64 ull;
}ULLDATA;

extern void FRAM_Init(void);
extern u32  FRAM_ReadDW(u16 u16StarAddr);
extern void FRAM_WriteDW(u16 u16StarAddr,u32 u32WriteVal);

extern void FRAM_ContReadByte(u16 u16StarAddr,u8* u8Arry,u16 u16RdLength);
extern void FRAM_ContWriteByte(u16 u16StarAddr,u8* u8Arry,u16 u16RdLength);

extern void FRAM_ContReadDW(u16 u16StarAddr,u32* u32Arry,u16 u16DWNum);
extern void FRAM_ContWriteDW(u16 u16StarAddr,u32* u32Arry,u16 u16DWNum);
extern void FRAM_WriteDW_Backup(u16 u16StarAddr,u32 u32WriteVal);
extern u64 FRAM_Read8Byte(u16 u16StarAddr);
extern void FRAM_Write8Byte(u16 u16StarAddr,u64 u64WriteVal);
extern float FRAM_ReadFloat(u16 u16StarAddr);
extern void FRAM_WriteFloat(u16 u16StarAddr,float fWriteVal);
 

#endif
