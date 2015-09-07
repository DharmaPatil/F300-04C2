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
    u32 Target;     //目标值
    s32 Large;      //大投提前量
    s32 Middle;     //中投提前量
    s32 Small;      //落差值
    u32 NearZero;   //零区值
    u32 FlapBagWt;  //拍袋起始重量
    //F2
    u32 OUSwitch;       //超欠差报警开关
    u32 OverValue;      //超量值
    u32 UnderValue;     //欠量值
    u32 OUStopSwitch;   //超欠差暂停开关
    //F3
    u32 TimeT1;     //加料延时T1
    u32 TimeT2b;    //小投禁止比较时间T2b
    u32 TimeT3;     //超差检测时间T3
    u32 TimeT4;     //定值保持时间T4
    u32 TimeT5;     //卸料延时T5
    u32 TimeT6;     //夹袋延时T6
    u32 TimeT7;     //松袋前延时T7
    u32 TimeT2a;    //中投禁止比较时间T2a
    u32 TimeT9;     //提袋延时
    u32 TimeT10;    //提袋输出有效时间
    u32 TimeT11;    //推袋延时
    u32 TimeT12;    //推袋输出有效时间
    //F4
    u32 CorrectFallSwitch;        //落差修正开关
    u32 CorrectFallTime;          //落差修正次数
    u32 CorrectFallRng1;          //落差修正范围
    u32 CorrectFallRng2;          //落差修正幅度
    //F5
    u32 FlapBagSwitch;            //拍袋开关
    u32 FlapBagAddNum;            //加料中拍袋次数
    u32 FlapBagNum;               //定值后拍袋次数
    u32 FlapBagDelay;             //拍袋延时
    u32 FlapBagTimeHi;            //拍袋有效电平时间
    u32 FlapBagTimeLow;           //拍袋无效电平时间
    u32 FlapBagExtTime;           //额外拍袋输出有效时间
    //F6
    u32 SmallPluseSwitch;         //小投点动输出开关
    u32 SmallPluseHi;             //点动有效电平时间
    u32 SmallPluseLow;            //点动无效电平时间
    //F7
    u32 CombinedNum;              //单秤组合次数
    //F8
    u32 DiscShakeSwitch;          //卸料震打开关    
    u32 DiscEffTime;              //卸料有效时间    
    u32 DiscShakeTimeHi;          //卸料震打有效时间
    u32 DiscShakeTimeLow;         //卸料震打无效时间
    u32 DiscShakeNum;             //卸料震打次数    
    //F9
    u32 PrintCodeSwitch;          //打码开关    
    u32 PrintCodeRunDelay;        //打码启动延时
    u32 PrintCodeTime;            //打码时间    
    u32 SheildDiscSwitch;         //打码屏蔽卸料或加料开关
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
