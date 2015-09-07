#ifndef _F_RAM_H
#define _F_RAM_H

#include "UserLib.h"



#define SUPERCODE       880406          //超级密码,型号设置密码
#define RESETCODE       880432          //复位密码
#define BACKUPCODE      880404          //参数备份密码
#define RELOADCODE      880401          //参数恢复密码
#define SUPERSHUTCODE   8110230         //定时关机通用超级密码


#define CBL     0x0400  //Class Bank Length:基本块大小  1KB

#define BANK0_BASE      (CBL*0)         //Bank1 保留
#define BANK1_BASE      (CBL*1)         //批次、累计、锁机等参数
#define BANK2_BASE      (CBL*2)
#define CAL_BASE        BANK2_BASE      //标定参数起始地址
#define BANK3_BASE      (CBL*3)
#define SETUP_BASE      BANK3_BASE      //工作参数起始地址  开关量测试定义 快速设置
#define BANK4_BASE      (CBL*4)         //配方参数
#define BANK5_BASE      (CBL*5)         //配方占用
#define BANK6_BASE      (CBL*6)         //配方占用
#define BANK7_BASE      (CBL*7)         //配方占用
#define BANK8_BASE      (CBL*8)         //配方占用
#define BANK9_BASE      (CBL*9)         //配方占用

#define BANK10_BASE     (CBL*10)
#define BANK11_BASE     (CBL*11)
#define BANK12_BASE     (CBL*12)
#define BANK13_BASE     (CBL*13)
#define BANK14_BASE     (CBL*14)
#define BANK15_BASE     (CBL*15)
//前16KB用作平时的参数存储
//后16KB用作参数备份，平时是保护的状态防止数据被改，只有在备份的时候是打开保护

#define BACKUPSTART     BANK16_BASE      //参数备份起始地址
#define BANK16_BASE     (CBL*16)    //备份区块3
#define BANK17_BASE     (CBL*17)    //备份区块4
#define BANK18_BASE     (CBL*18)    //备份区块5
#define BANK19_BASE     (CBL*19)    //备份区块6
#define BANK20_BASE     (CBL*20)    //备份区块7
#define BANK21_BASE     (CBL*21)    //备份区块8
#define BANK22_BASE     (CBL*22)    //备份区块9
#define BANK23_BASE     (CBL*23)    //备份区块10

#define BANK24_BASE     (CBL*24)    //备份区块11
#define BANK25_BASE     (CBL*25)    //备份区块12
#define BANK26_BASE     (CBL*26)    //备份区块13
#define BANK27_BASE     (CBL*27)    //备份区块14
#define BANK28_BASE     (CBL*28)    //备份区块15
#define BANK29_BASE     (CBL*29)
#define BANK30_BASE     (CBL*30)
#define BANK31_BASE     (CBL*31)


//杂项 BANK1_BASE-----------------------------------------------------------------------------------------
//配方累计重量 占4字节
#define FMAddr_SumWt_1              (BANK1_BASE+0x000)       //配方1累计重量
#define FMAddr_SumWt_2              (BANK1_BASE+0x004)       //配方2累计重量
#define FMAddr_SumWt_3              (BANK1_BASE+0x008)       //配方3累计重量
#define FMAddr_SumWt_4              (BANK1_BASE+0x00C)       //配方4累计重量
#define FMAddr_SumWt_5              (BANK1_BASE+0x010)       //配方5累计重量
#define FMAddr_SumWt_6              (BANK1_BASE+0x014)       //配方6累计重量
#define FMAddr_SumWt_7              (BANK1_BASE+0x018)       //配方7累计重量
#define FMAddr_SumWt_8              (BANK1_BASE+0x01C)       //配方8累计重量
#define FMAddr_SumWt_9              (BANK1_BASE+0x020)       //配方9累计重量
#define FMAddr_SumWt_10             (BANK1_BASE+0x024)       //配方10累计重量
#define FMAddr_SumWt_11             (BANK1_BASE+0x028)       //配方11累计重量
#define FMAddr_SumWt_12             (BANK1_BASE+0x02C)       //配方12累计重量
#define FMAddr_SumWt_13             (BANK1_BASE+0x030)       //配方13累计重量
#define FMAddr_SumWt_14             (BANK1_BASE+0x034)       //配方14累计重量
#define FMAddr_SumWt_15             (BANK1_BASE+0x038)       //配方15累计重量
#define FMAddr_SumWt_16             (BANK1_BASE+0x03C)       //配方16累计重量
#define FMAddr_SumWt_17             (BANK1_BASE+0x040)       //配方17累计重量
#define FMAddr_SumWt_18             (BANK1_BASE+0x044)       //配方18累计重量
#define FMAddr_SumWt_19             (BANK1_BASE+0x048)       //配方19累计重量
#define FMAddr_SumWt_20             (BANK1_BASE+0x04C)       //配方20累计重量
//配方累计次数 占4字节
#define FMAddr_SumNum_1             (BANK1_BASE+0x09C)       //配方1累计次数
#define FMAddr_SumNum_2             (BANK1_BASE+0x010)       //配方2累计次数
#define FMAddr_SumNum_3             (BANK1_BASE+0x114)       //配方3累计次数
#define FMAddr_SumNum_4             (BANK1_BASE+0x118)       //配方4累计次数
#define FMAddr_SumNum_5             (BANK1_BASE+0x11C)       //配方5累计次数
#define FMAddr_SumNum_6             (BANK1_BASE+0x120)       //配方6累计次数
#define FMAddr_SumNum_7             (BANK1_BASE+0x124)       //配方7累计次数
#define FMAddr_SumNum_8             (BANK1_BASE+0x128)       //配方8累计次数
#define FMAddr_SumNum_9             (BANK1_BASE+0x12C)       //配方9累计次数
#define FMAddr_SumNum_10            (BANK1_BASE+0x130)       //配方10累计次数
#define FMAddr_SumNum_11            (BANK1_BASE+0x134)       //配方11累计次数
#define FMAddr_SumNum_12            (BANK1_BASE+0x138)       //配方12累计次数
#define FMAddr_SumNum_13            (BANK1_BASE+0x13C)       //配方13累计次数
#define FMAddr_SumNum_14            (BANK1_BASE+0x140)       //配方14累计次数
#define FMAddr_SumNum_15            (BANK1_BASE+0x144)       //配方15累计次数
#define FMAddr_SumNum_16            (BANK1_BASE+0x148)       //配方16累计次数
#define FMAddr_SumNum_17            (BANK1_BASE+0x14C)       //配方17累计次数
#define FMAddr_SumNum_18            (BANK1_BASE+0x150)       //配方18累计次数
#define FMAddr_SumNum_19            (BANK1_BASE+0x154)       //配方19累计次数
#define FMAddr_SumNum_20            (BANK1_BASE+0x158)       //配方20累计次数

#define FMAddr_SumWt                (BANK1_BASE+0x15C)       //累计重量 8字节
#define FMAddr_SumNum               (BANK1_BASE+0x164)       //累计次数
#define FMAddr_BatchTime            (BANK1_BASE+0X168)       //批次数
#define FMAddr_BatchTimeCnt         (BANK1_BASE+0x16C)       //剩余批次数


//#define FMAddr_OutSumWt             (BANK1_BASE+0x18C)       //收发货累计
//#define FMAddr_OutTarget            (BANK1_BASE+0x190)       //发货总量
//#define FMAddr_OutEndNoRun          (BANK1_BASE+0x194)       //发货后未启动标志
//#define FMAddr_SFJHeight            (BANK1_BASE+0x19C)       //输送机高度

#define FMAddr_BuyerID              (BANK1_BASE+0x200)       //用户ID
#define FMAddr_ShutDownCode         (BANK1_BASE+0x204)       //自动锁机密码
#define FMAddr_LeaveDays            (BANK1_BASE+0x208)       //锁机剩余天数
#define FMAddr_ShutDownSwitch       (BANK1_BASE+0x20C)       //定时锁机开关
#define FMAddr_CmpYear              (BANK1_BASE+0x210)       //锁机比较年
#define FMAddr_CmpMonth             (BANK1_BASE+0x214)       //锁机比较月份
#define FMAddr_CmpDate              (BANK1_BASE+0x218)       //锁机比较日期
#define FMAddr_GetedRandom          (BANK1_BASE+0x21C)       //随机数已经获取
#define FMAddr_RandomID             (BANK1_BASE+0x220)       //随机数

#define FMAddr_BackupYear           (BANK1_BASE+0x230)       //备份数据 年
#define FMAddr_BackupMonth          (BANK1_BASE+0x234)       //备份数据 月
#define FMAddr_BackupDate           (BANK1_BASE+0x238)       //备份数据 日
#define FMAddr_Backuped             (BANK1_BASE+0x23C)       //有备份过数据


//#define FMAddr_SumWt_Total          (BANK1_BASE+0x240)       //系统总累计重量8字节
//#define FMAddr_SumNum_Total         (BANK1_BASE+0x248)       //系统总累计次数
//#define FMAddr_NoSumWt              (BANK1_BASE+0x24C)       //未累计重量8字节
//#define FMAddr_NoSumNum             (BANK1_BASE+0x254)       //未累计次数

#define FMAddr_ModelSetSwitch       (BANK1_BASE+0x300)      //型号自定义开关
#define FMAddr_ModelSet0            (BANK1_BASE+0x304)      //logo开关
#define FMAddr_ModelSet1            (BANK1_BASE+0x308)      //段码第1位
#define FMAddr_ModelSet2            (BANK1_BASE+0x30C)      //段码第2位
#define FMAddr_ModelSet3            (BANK1_BASE+0x310)      //段码第3位
#define FMAddr_ModelSet4            (BANK1_BASE+0x314)      //段码第4位
#define FMAddr_ModelSet5            (BANK1_BASE+0x318)      //段码第5位
#define FMAddr_ModelSet6            (BANK1_BASE+0x31C)      //段码第6位
#define FMAddr_HadBeenInited        (BANK1_BASE+0X320)      //仪表已经初始化过

//CAL块 BANK2_BASE--------------------------------------------------------------------------
#define FMAddr_CalCode              (CAL_BASE+0x00)        //标定密码
#define FMAddr_CalUnit              (CAL_BASE+0x04)
#define FMAddr_CalPoint             (CAL_BASE+0x08)
#define FMAddr_CalDiv               (CAL_BASE+0x0C)
#define FMAddr_CalCap               (CAL_BASE+0x10)
#define FMAddr_CalSensor            (CAL_BASE+0x14)
#define FMAddr_CalZero              (CAL_BASE+0x18)
#define FMAddr_CalADK               (CAL_BASE+0x1C)


//SETUP块 BANK3_BASE-------------------------------------------------------------------------
#define FMAddr_SetupCodeSwitch      (SETUP_BASE+0x00)        //工作参数密码开关
#define FMAddr_SetupCode            (SETUP_BASE+0x04)        //工作参数密码

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

//输入开关量BINYES
#define FMAddr_IoInBuf1             (SETUP_BASE+0x100)
#define FMAddr_IoInBuf2             (SETUP_BASE+0x104)
#define FMAddr_IoInBuf3             (SETUP_BASE+0x108)
#define FMAddr_IoInBuf4             (SETUP_BASE+0x10C)
#define FMAddr_IoInBuf5             (SETUP_BASE+0x110)
#define FMAddr_IoInBuf6             (SETUP_BASE+0x114)
#define FMAddr_IoInBuf7             (SETUP_BASE+0x118)
#define FMAddr_IoInBuf8             (SETUP_BASE+0x11C)
//输出开关量BINYES
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
//输入开关量BINNO
#define FMAddr_IoInBuf1_BINNO             (SETUP_BASE+0x150)
#define FMAddr_IoInBuf2_BINNO             (SETUP_BASE+0x154)
#define FMAddr_IoInBuf3_BINNO             (SETUP_BASE+0x158)
#define FMAddr_IoInBuf4_BINNO             (SETUP_BASE+0x15C)
#define FMAddr_IoInBuf5_BINNO             (SETUP_BASE+0x160)
#define FMAddr_IoInBuf6_BINNO             (SETUP_BASE+0x164)
#define FMAddr_IoInBuf7_BINNO             (SETUP_BASE+0x168)
#define FMAddr_IoInBuf8_BINNO             (SETUP_BASE+0x16C)
//输出开关量BINNO
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
//输入开关量BIN_2
#define FMAddr_IoInBuf1_BIN_2             (SETUP_BASE+0x1A0)
#define FMAddr_IoInBuf2_BIN_2             (SETUP_BASE+0x1A4)
#define FMAddr_IoInBuf3_BIN_2             (SETUP_BASE+0x1A8)
#define FMAddr_IoInBuf4_BIN_2             (SETUP_BASE+0x1AC)
#define FMAddr_IoInBuf5_BIN_2             (SETUP_BASE+0x1B0)
#define FMAddr_IoInBuf6_BIN_2             (SETUP_BASE+0x1B4)
#define FMAddr_IoInBuf7_BIN_2             (SETUP_BASE+0x1B8)
#define FMAddr_IoInBuf8_BIN_2             (SETUP_BASE+0x1BC)
//输出开关量BIN_2
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
//输入开关量NO_2
#define FMAddr_IoInBuf1_NO_2             (SETUP_BASE+0x1F0)
#define FMAddr_IoInBuf2_NO_2             (SETUP_BASE+0x1F4)
#define FMAddr_IoInBuf3_NO_2             (SETUP_BASE+0x1F8)
#define FMAddr_IoInBuf4_NO_2             (SETUP_BASE+0x1FC)
#define FMAddr_IoInBuf5_NO_2             (SETUP_BASE+0x200)
#define FMAddr_IoInBuf6_NO_2             (SETUP_BASE+0x204)
#define FMAddr_IoInBuf7_NO_2             (SETUP_BASE+0x208)
#define FMAddr_IoInBuf8_NO_2             (SETUP_BASE+0x20C)
//输出开关量NO_2
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


//开关量测试密码
#define FMAddr_IoTestCodeSwitch           (SETUP_BASE+0x280)
#define FMAddr_IoTestCode                 (SETUP_BASE+0x284)
//开关量自定义密码
#define FMAddr_IoDefCodeSwitch           (SETUP_BASE+0x288)
#define FMAddr_IoDefCode                 (SETUP_BASE+0x28C)

//快速设置项目自定义参数地址
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




//RECIPE块 BANK4_BASE----------------------------------------------------------------------------
//配方号，密码开关和密码占用12个字节地址
//每一个配方下分配 PER_REC_LENGTH 地址，供后续扩展，那么每个配方参数的地址计算为 gu32_RecNum * PER_REC_LENGTH + 参数地址
//PER_REC_LENGTH=280时，共占用280×20+12=5612个地址，占用6个CBL块
#define FMAddr_RecCodeSwitch        (BANK4_BASE+0x00)       //配发参数密码开关
#define FMAddr_RecCode              (BANK4_BASE+0x04)       //配方参数密码
#define FMAddr_RecNum               (BANK4_BASE+0x08)       //配方号
//配方参数保存起始地址,共20组，由配方号号（1~20）决定，(r)是配方号
#define RECIPE_BASE     (BANK4_BASE+0x0C)                   //配方参数起始地址
//每一个配方所占用的长度
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
