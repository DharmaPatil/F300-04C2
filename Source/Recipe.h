#ifndef _RECIPE_H
#define _RECIPE_H

#include "71x_type.h"


#define DISP_RECNUM         0
#define REC_CHECKCODE       1
#define REC_F1              2
#define REC_F1_1            3
#define REC_F1_2            4
#define REC_F1_3            5
#define REC_F1_4            6
#define REC_F1_5            7
#define REC_F1_6            8
#define REC_F2              9
#define REC_F2_1            10
#define REC_F2_2            11
#define REC_F2_3            12
#define REC_F3              13
#define REC_F3_1            14
#define REC_F3_2            15
#define REC_F3_3            16
#define REC_F3_4            17
#define REC_F3_5            18
#define REC_F3_6            19
#define REC_F3_7            20
#define REC_F3_8            21
#define REC_F3_9            22
#define REC_F3_10           23
#define REC_F3_11           24
#define REC_F3_12           25
#define REC_F4              26
#define REC_F4_1            27
#define REC_F4_2            28
#define REC_F4_3            29
#define REC_F5              30
#define REC_F5_1            31
#define REC_F5_2            32
#define REC_F5_3            33
#define REC_F5_4            34
#define REC_F5_5            35
#define REC_F5_6            36
#define REC_F6              37
#define REC_F6_1            38
#define REC_F6_2            39
#define REC_F7              40
#define REC_F8              41
#define REC_F8_1            42
#define REC_F8_2            43
#define REC_F8_3            44
#define REC_F8_4            45
#define REC_F9              46
#define REC_F9_1            47
#define REC_F9_2            48
#define REC_F9_3            49
#define REC_FA              50
#define REC_FA_1            51
#define REC_END             52

#define POFF    0
#define PF_     1
#define P_D     2
#define PFD     3


struct RECIPE
{   //F1
    u32 Target;     //Ŀ��ֵ
    s32 Large;      //��Ͷ��ǰ��
    s32 Middle;     //��Ͷ��ǰ��
    s32 Small;      //���ֵ
    u32 NearZero;   //����ֵ
    u32 FlapBagWt;  //�Ĵ���ʼ����
    //F2
    u32 OUSwitch;       //��Ƿ�������
    u32 OverValue;      //����ֵ
    u32 UnderValue;     //Ƿ��ֵ
    u32 OUStopSwitch;   //��Ƿ����ͣ����
    //F3
    u32 TimeT1;     //������ʱT1
    u32 TimeT2b;    //СͶ��ֹ�Ƚ�ʱ��T2b
    u32 TimeT3;     //������ʱ��T3
    u32 TimeT4;     //��ֵ����ʱ��T4
    u32 TimeT5;     //ж����ʱT5
    u32 TimeT6;     //�д���ʱT6
    u32 TimeT7;     //�ɴ�ǰ��ʱT7
    u32 TimeT2a;    //��Ͷ��ֹ�Ƚ�ʱ��T2a
    u32 TimeT9;     //�����ʱ
    u32 TimeT10;    //��������Чʱ��
    u32 TimeT11;    //�ƴ���ʱ
    u32 TimeT12;    //�ƴ������Чʱ��
    //F4
    u32 CorrectFallSwitch;        //�����������
    u32 CorrectFallTime;          //�����������
    u32 CorrectFallRng1;          //���������Χ
    u32 CorrectFallRng2;          //�����������
    //F5
    u32 FlapBagSwitch;            //�Ĵ�����
    u32 FlapBagAddNum;            //�������Ĵ�����
    u32 FlapBagNum;               //��ֵ���Ĵ�����
    u32 FlapBagDelay;             //�Ĵ���ʱ
    u32 FlapBagTimeHi;            //�Ĵ���Ч��ƽʱ��
    u32 FlapBagTimeLow;           //�Ĵ���Ч��ƽʱ��
    u32 FlapBagExtTime;           //�����Ĵ������Чʱ��
    //F6
    u32 SmallPluseSwitch;         //СͶ�㶯�������
    u32 SmallPluseHi;             //�㶯��Ч��ƽʱ��
    u32 SmallPluseLow;            //�㶯��Ч��ƽʱ��
    //F7
    u32 CombinedNum;              //������ϴ���
    //F8
    u32 DiscShakeSwitch;          //ж����򿪹�    
    u32 DiscEffTime;              //ж����Чʱ��    
    u32 DiscShakeTimeHi;          //ж�������Чʱ��
    u32 DiscShakeTimeLow;         //ж�������Чʱ��
    u32 DiscShakeNum;             //ж��������    
    //F9
    u32 PrintCodeSwitch;          //���뿪��    
    u32 PrintCodeRunDelay;        //����������ʱ
    u32 PrintCodeTime;            //����ʱ��    
    u32 SheildDiscSwitch;         //��������ж�ϻ���Ͽ���
};

extern struct RECIPE gs_Rec;
extern u32 gu32_RecCodeSwitch;
extern u32 gu32_RecCode;
extern u32 gu32_RecNum;
extern u8 gu8_RecInTime;
extern const u16 gu16_RecRealItem[];


extern void Key_RecipeState(void);
extern void Disp_RecipeState(void);
extern u32 GetRecipeItemValue(u8 fuc_Item);
extern u16 GetRecipePort(u8 fuc_Item);
extern bool RecDataInputCheck(void);



#endif
