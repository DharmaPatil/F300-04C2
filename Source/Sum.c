/* 
Description:            累计相关功能
Author:                 丛海旭
Date:                   2013-12-9
*/

#include "include.h"

#define SUM_VIEW_CHECKCODE  0   //累计查询密码校验
#define DISP_SUM_WT         1   //显示累计重量
#define DISP_SUM_NUM_OUTWT  2   //显示累计次数或收发货累计
#define DISP_SUM_BAT_FLUX   3   //显示剩余批次或流量
#define DISP_SUM_WT_REC     4   //显示配方累计重量
#define DISP_SUM_NUM_REC    5   //显示配方累计次数
#define SUM_CLR_CHECKCODE   6   //累计清除密码校验

u32 gu32_DebugSumNum;
u32 gu32_DebugSumNumTemp;   //打印中使用




u64 gu64_SumWt;                //累计重量 最大15位
u32 gu32_SumNum;               //累计次数
//u64 gu64_SumWt_Total;          //系统总累计重量
//u32 gu32_SumNum_Total;         //系统总累计次数
u32 gu32_SumWt_Rec[21];             //配方1-20累计重量
u32 gu32_SumNum_Rec[21];            //配方1-20累计次数

//累计重量小数点和单位转换后的数据变量
u32 gu32_SumWt;
u32 gu32_SumWtUnit;             //累计单位
u32 gu32_SumWtPoint;            //累计小数点
//u32 gu32_SumWt_Rec[21];
//u32 gu32_SumWtUnit_Rec[21];             //累计单位
//u32 gu32_SumWtPoint_Rec[21];            //累计小数点


u8 gu8_SumCheckInTime;      //累计查询密码输入错误次数
u8 gu8_RecDispSum;


/*********************************************************************
函数名称: bool GetSumWt_u32(u64 fu64_SumWt,
                           u32 fu32_Unit,
                           u32 fu32_Point,
                           u32* fu32p_Unit,
                           u32* fu32p_Point)
功    能: 返回4字节的累计重量数据
说    明: 仪表累计最大显示十进制的9位，如显示不下则首先进行小数点后最低位的舍弃，
          还是显示不下则进行单位变换，如kg变为t，然后再显示
          改为独立的可移植函数
入口参数: 
返 回 值: true:溢出  false:未溢出
设    计: 丛海旭               时    间: 2014-1-15
修    改:                      时    间: 
*********************************************************************/
bool GetSumWt_u32(u64 fu64_SumWt,    //需要转换的当前8字节重量数据
                  u32 fu32_Unit,     //当前重量对应的单位
                  u32 fu32_Point,    //当前重量对应的小数点
                  u32* fu32p_SumWt,  //得到的4字节目标重量
                  u32* fu32p_Unit,   //目标重量的单位
                  u32* fu32p_Point   //目标重量的小数点
                )
{   //小数点和单位赋值同系统相同
    *fu32p_Unit = fu32_Unit;
    *fu32p_Point = fu32_Point;
    if (fu64_SumWt <= 999999999)    //小于9位则直接赋值4字节的重量数据
        *fu32p_SumWt = fu64_SumWt;
    else
    {   //重量大于十进制9位数进行转换
        while(fu64_SumWt > 999999999)
        {   //先进性小数点后的数据从低位舍去
            if (*fu32p_Point > 0)
            {
                (*fu32p_Point)--;
                fu64_SumWt /= 10;
            }
            else if (*fu32p_Unit < 2)
            {   //小数点为0后进行单位的转换
    //          UNIT_g      0
    //          UNIT_kg     1
    //          UNIT_t      2
                (*fu32p_Unit)++;
                (*fu32p_Point) = 3;
            }
            else
            {   //单位已经转变为t还是显示不下则认为溢出，重新开始累计
                return true;
            }
        }
        //做完数据转换后的重量赋值4字节的累计数据
        *fu32p_SumWt = fu64_SumWt;
    }
    return false;
}

/*********************************************************************
函数名称: void Save_SumData(u32 fu32_Weight, u32 fu32_Num)
功    能: 累计数据存入铁电
说    明: 
入口参数: fu32_Weight=存入的累计重量  fu32_Num=存入的累计次数
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-15
修    改:                      时    间: 
*********************************************************************/
void Save_SumData(u32 fu32_Weight, u32 fu32_Num)
{
    gu64_SumWt += fu32_Weight;  //累计
    gu32_SumNum += fu32_Num;
    
    gu32_SumWt_Rec[gu32_RecNum] += fu32_Weight;     //对应配方累计
    gu32_SumNum_Rec[gu32_RecNum] += fu32_Num;
    
    //累计过限 15位或9位
    if (gu64_SumWt > (gs_Setup.SumExtendSwitch ? 999999999999999 : 999999999))
    {
        gu64_SumWt -= (gs_Setup.SumExtendSwitch ? 1000000000000000 : 1000000000);
//        gu32_SumNum = 0;
    }
    if (gu32_SumNum > 999999999)
    {
        gu32_SumNum = 0;
//        gu32_SumWt = 0;
    }
    
    if (gu32_SumWt_Rec[gu32_RecNum] > 999999999)          //累计过限
    {
        gu32_SumWt_Rec[gu32_RecNum] -= 1000000000;
//        gu32_SumNum_Rec[gu32_RecNum] = 0;
    }
    if (gu32_SumNum_Rec[gu32_RecNum] > 999999999)
    {
//        gu32_SumWt_Rec[gu32_RecNum] = 0;
        gu32_SumNum_Rec[gu32_RecNum] = 0;
    }
    //保存总累计重量和次数
    FRAM_Write8Byte(FMAddr_SumWt, gu64_SumWt);
    FRAM_WriteDW(FMAddr_SumNum, gu32_SumNum);
    //保存配方累计重量和次数
    FRAM_WriteDW(FMAddr_SumWt_1 + gu32_RecNum * 4, gu32_SumWt_Rec[gu32_RecNum]);
    FRAM_WriteDW(FMAddr_SumNum_1 + gu32_RecNum * 4, gu32_SumNum_Rec[gu32_RecNum]);
    //获取显示的4字节的重量和对应的单位
    if (GetSumWt_u32(gu64_SumWt,      //需要转换的当前8字节重量数据
                     gs_Cal.Unit,     //当前重量对应的单位
                     gs_Cal.Point,    //当前重量对应的小数点
                     &gu32_SumWt,     //得到的4字节目标重量
                     &gu32_SumWtUnit,   //目标重量的单位
                     &gu32_SumWtPoint   //目标重量的小数点
                    ))
    {   //重量已经超过能累计的最大值，溢出，清除累计重新计算
        ClrSum();
    }
}

/*********************************************************************
函数名称: void ClrSum_Rec(u8 fu8_Rec)
功    能: 清配方累计
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-15
修    改:                      时    间: 
*********************************************************************/
void ClrSum_Rec(u8 fu8_Rec)
{
    gu32_SumWt_Rec[fu8_Rec] = 0;
    gu32_SumNum_Rec[fu8_Rec] = 0;
    FRAM_WriteDW(FMAddr_SumWt_1 + fu8_Rec*4, 0);
    FRAM_WriteDW(FMAddr_SumNum_1 + fu8_Rec*4, 0);
}

/*********************************************************************
函数名称: void ClrSum(void)
功    能: 清累计
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-15
修    改:                      时    间: 
*********************************************************************/
void ClrSum(void)
{
    u8 i;
    for (i = 1; i <= 20; i++)    //清除配方累计
        ClrSum_Rec(i);
    gu64_SumWt = 0;
    gu32_SumNum = 0;
    Save_SumData(0, 0);
    GetSumWt_u32(gu64_SumWt,      //需要转换的当前8字节重量数据
                 gs_Cal.Unit,     //当前重量对应的单位
                 gs_Cal.Point,    //当前重量对应的小数点
                 &gu32_SumWt,     //得到的4字节目标重量
                 &gu32_SumWtUnit,   //目标重量的单位
                 &gu32_SumWtPoint   //目标重量的小数点
                 );
}

/*********************************************************************
函数名称: void Key_SumState(void)
功    能: 累计查询状态相关处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-27
修    改:                      时    间: 
*********************************************************************/
void Key_SumState(void)
{
    static u8 su8_ItemBak;
     
    if (gu8_Item != SUM_VIEW_CHECKCODE && gu8_Item != SUM_CLR_CHECKCODE)
    {
        if (gb_FlashFlg == true)             //累计闪烁
        {
            switch (gu32_KeyCode)        //按键处理
            {
                case K_ESC:
                    gb_FlashFlg = false;
                    break;
                case K_ENTER:           //清除累计重量和累计次数
                    if (gs_Setup.SumClrCodeSwitch != 0)     //清除累计需要密码开关
                    {
            			su8_ItemBak = gu8_Item;
            			gu8_Item = SUM_CLR_CHECKCODE;
                        gu32_KeyCode = 0;
            			gb_SetFlg = false;
            			ulParameter.ul = 0xFFFFFFFF;
                        gb_FlashFlg = false;
            			gu8_DataInLength = 0;
                    }
                    else
                    {
                        if (gu8_Item <= DISP_SUM_BAT_FLUX)  //清除所有累计
                        {
                            ClrSum();
                            gu8_Item = DISP_SUM_WT;
                        }
                        else    //清除配方累计
                        {
                            ClrSum_Rec(gu8_RecDispSum);
                            gu8_Item = DISP_SUM_NUM_REC;
                        }
                        gb_FlashFlg = false;
                    }
                    break;
                default:
                    break;
            }
        }
        else                        //累计显示状态
        {
            switch (gu32_KeyCode)       //按键处理
            {
                case K_7:   //查看累计时按打印键的处理
                    if (gs_Setup.ComMode_1 == COM_PRINT || gs_Setup.ComMode_2 == COM_PRINT)
                    {
                        if (gu8_Item == DISP_SUM_WT || gu8_Item == DISP_SUM_NUM_OUTWT)
                        {   //查看总累计的时候按打印键打印总累计数据
                            PrintStart(PRINT_TOTAL_REPORT, 0, 0);
                        }
                        else if (gu8_Item == DISP_SUM_WT_REC || gu8_Item == DISP_SUM_NUM_REC)
                        {   //查看配方累计的时候打印配方累计报表
                            gb_FirstPrintFlg_UART0 = true;
                            gb_PrintAllRecFlg_UART0 = false;
                            gb_FirstPrintFlg_UART1 = true;
                            gb_PrintAllRecFlg_UART1 = false;
                            PrintStart(PRINT_REC_REPORT, gu8_RecDispSum, 0);
                        }
                    }
                    else    //串口没有定义成打印，提示ERROR10
                        ErrorDeal(10, 100);
                    break;
                case K_6:           //
                case K_RIGHT:
                    if (gu8_Item >= DISP_SUM_WT_REC)
                    {
                        if (++gu8_Item > DISP_SUM_NUM_REC)
                            gu8_Item = DISP_SUM_WT_REC;
                    }
                    else
                    {   //批次数设置为0时不显示剩余批次
                        if (++gu8_Item > ((gu32_BatchTime == 0) ? DISP_SUM_NUM_OUTWT : DISP_SUM_BAT_FLUX))
                        {
                            if (gu32_SystemState != RUNSTATE)  //未处于运行状态
                                gu32_SystemState = STOPSTATE;
                            gb_FlashFlg = false;
                            gb_SumFlg = false;
                        }
                    }
                    break;
                case K_INC:
                    if (gu8_Item <= DISP_SUM_BAT_FLUX) //查看总累计时按↑键进入分配方累计查看
                    {
                        gu8_Item = DISP_SUM_WT_REC;
                        gu8_RecDispSum = gu32_RecNum;
                    }
                    else if (gu8_Item >= DISP_SUM_WT_REC) //处于查看配方累计状态按↑则增加配方号循环显示每个配方的累计
                    {
                        gu8_Item = DISP_SUM_WT_REC;
                        if (++gu8_RecDispSum > 20)
                            gu8_RecDispSum = 1;
                        ulParameter.ul = gu32_SumWt_Rec[gu8_RecDispSum];
                    }
                    break;
                case K_ESC:
                    if (gu8_Item >= DISP_SUM_WT_REC)
                    {
                        gu8_Item = DISP_SUM_WT;
                    }
                    else
                    {
                        gb_FlashFlg = false;
                        gb_SumFlg = false;
                        if (gu32_SystemState != RUNSTATE)  //未处于运行状态
                            gu32_SystemState = STOPSTATE;
                    }
                    break;
                case K_ZERO:
                    if (gu8_Item != DISP_SUM_BAT_FLUX && gb_RunFlg == false)   //显示累计重量和次数可以按ZERO
                        gb_FlashFlg = true;
                    break;
                default:
                    break;
            }
        }
    }
    
    switch (gu8_Item)
    {
        case SUM_VIEW_CHECKCODE:    //查询密码
        case SUM_CLR_CHECKCODE:     //清除密码
            gb_FirstNumKeyFlg = false;
            //数据输入
            ulParameter.ul = Key_NumInput(ulParameter.ul, 6);
            if (gu32_KeyCode == K_ESC)
            {
                if (gu8_Item == SUM_VIEW_CHECKCODE)
                {
                    gb_FlashFlg = false;
                    if (gu32_SystemState != RUNSTATE)  //未处于运行状态
                        gu32_SystemState = STOPSTATE;
                    gb_SumFlg = false;
                }
                else if (gu8_Item == SUM_CLR_CHECKCODE)
                {
                    gu8_Item = su8_ItemBak;
                    gb_FlashFlg = false;
                }
            }
            else if (gu32_KeyCode == K_ENTER)
            {
                gu8_SumCheckInTime++;
                if ((ulParameter.ul == gu32_SetupCode && gu8_DataInLength ==6) || 
                    (ulParameter.ul == SUPERCODE && gu8_DataInLength ==6))        //输入密码等于设定密码或超级密码
                {
                    if (gu8_Item == SUM_VIEW_CHECKCODE)      //累计查询密码输入正确
                    {
                        gu8_SumCheckInTime = 0;
                        gb_FlashFlg = false;
                		ulParameter.ul = gu32_SumWt;
                		gu8_Item = DISP_SUM_WT;
                		gb_SumFlg = true;      //sum指示灯亮
                	}
                	else if (gu8_Item == SUM_CLR_CHECKCODE)     //累计清除密码输入正确
                	{
                	    if (su8_ItemBak <= DISP_SUM_NUM_OUTWT)  //清除所有累计
                        {
                            ClrSum();
                    		ulParameter.ul = gu32_SumWt;
                    		gu8_Item = DISP_SUM_WT;
                        }
                        else    //清除配方累计
                        {
                            ClrSum_Rec(gu8_RecDispSum);
                    		ulParameter.ul = gu32_SumWt_Rec[gu8_RecDispSum];
                    		gu8_Item = DISP_SUM_NUM_REC;
                        }
                        gb_FlashFlg = false;
                        gu8_SumCheckInTime = 0;
                		gb_SumFlg = true;      //sum指示灯亮
                    }
                }
                else if (gu8_SumCheckInTime >= 3)      //密码输入错误超过3次
                {
                    ErrorDeal(4, 0);        //显示error4
                    Disp_Error();
                    VFD_CopyDispArryToDispRam();
                    VFD_WriteDispRamArry();                  //写显示缓存
                    while(1);
                }
                else            //密码输入错误，提示error
                {
                    ErrorDeal(0, 100);
                    ulParameter.ul = 0;
                    gu8_DataInLength = 0;
                    return;
                }
            }
            break;
        case DISP_SUM_WT:             //显示累计重量
            ulParameter.ul = gu32_SumWt;
            break;
        case DISP_SUM_NUM_OUTWT:             //显示累计次数，散料秤模式显示收发货累计
            ulParameter.ul = gu32_SumNum;
            break;
        case DISP_SUM_BAT_FLUX:             //流量值或批次
            ulParameter.ul = gu32_BatchTimeCnt;
            break;
        case DISP_SUM_WT_REC:             //配方累计重量
            ulParameter.ul = gu32_SumWt_Rec[gu8_RecDispSum];
            break;
        case DISP_SUM_NUM_REC:             //配方累计次数
            ulParameter.ul = gu32_SumNum_Rec[gu8_RecDispSum];
        default:
            break;
    }
}

/*********************************************************************
函数名称: void Disp_SumState(void)
功    能: 批次数设置相关显示处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-27
修    改:                      时    间: 
*********************************************************************/
void Disp_SumState(void)
{
    bool lb_Echo0 = true;
    u32 lul_Point = 0;
    static u32 su32_TimeTemp;
    
    if (gu8_Item == SUM_VIEW_CHECKCODE || gu8_Item == SUM_CLR_CHECKCODE)      //密码输入
    {
        Disp_InputCode();       //密码输入显示处理
        gb_FlashFlg = true;
        Flash_MainDisp(0x01);                //闪烁处理
    }
    else if (gu8_Item == DISP_SUM_WT)
    {
        VFD_UnitDisp(gu32_SumWtUnit);               //累计重量显示单位
        lul_Point = gu32_SumWtPoint;               //累计重量显示系统小数点
    }
    else if (gu8_Item == DISP_SUM_WT_REC)
    {
        VFD_UnitDisp(gs_Cal.Unit);               //累计重量显示单位
        lul_Point = gs_Cal.Point;               //累计重量显示系统小数点
    }
    else if (gu8_Item == DISP_SUM_BAT_FLUX)
    {
        VFD_UnitDisp(UNIT_PCS);               //剩余批次显示PCS
        lul_Point = 0;                          //累计次数不显示小数点
    }
    else
    {
        VFD_UnitDisp(UNIT_N);               //累计次数显示N
        lul_Point = 0;                          //累计次数不显示小数点
    }
    
    
    //累计重量和累计次数的显示，最多9位，上面低6位下面高3位
    if (gu8_Item == DISP_SUM_WT || gu8_Item == DISP_SUM_NUM_OUTWT || gu8_Item == DISP_SUM_BAT_FLUX || 
        gu8_Item == DISP_SUM_WT_REC || gu8_Item == DISP_SUM_NUM_REC)
    {
        //累计大于6位，则上行显示不消隐
        if (ulParameter.ul / 1000000 > 0)
        {
            lb_Echo0 = false;
            
            if (gu8_Item == DISP_SUM_WT_REC || gu8_Item == DISP_SUM_NUM_REC)
            {
                //显示配方累计时，超过6位要占用副显示，副显示在累计高3位和配方号之间闪烁切换
                if(gu32_Sys10ms - su32_TimeTemp > 40)
                {
                    VFD_AuxDisp_u32(ulParameter.ul/1000000);       //副显示值 消隐
                    Disp_Echo0(0, gu8_AuxDispArray, 3);
                    if(gu32_Sys10ms - su32_TimeTemp > 80)
                        su32_TimeTemp = gu32_Sys10ms;
                }
                else
                    Disp_Aux_Rec(gu8_RecDispSum);
            }
            else
            {
                VFD_AuxDisp_u32(ulParameter.ul/1000000);       //副显示值 消隐
                Disp_Echo0(0, gu8_AuxDispArray, 3);
            }
        }
        else if (gu8_Item == DISP_SUM_WT_REC || gu8_Item == DISP_SUM_NUM_REC)
        {
            Disp_Aux_Rec(gu8_RecDispSum);
        }
        
        Disp_Main_Data(ulParameter.ul % 1000000, lul_Point, 6, lb_Echo0, false);       //主显示值
        if (lb_Echo0 == true)  //此处防止清累计闪烁时0消隐失效的问题
            Disp_Echo0(lul_Point, gu8_MainDispArray, 6);
        if (gb_RunFlg)
        {   //运行时查看累计时所有指示灯都显示 v1.01
            Disp_FlgLight();
        }
        else
        {//停止时查看累计时为了显示清晰简洁，只有SUM指示灯亮 v1.01
            VFD_FlgDisp_SUM(gb_SumFlg);     //SUM指示灯亮
            //LED指示灯的显示
            //夹袋输出时，如果定义了夹袋到位但是夹袋到位无效则PACK闪烁
            if (gb_EnPackOKFlg && (!gb_PackOKFlg))
                VFD_LEDDisp(!gb_DispNetFlg, gb_DispNetFlg, false, Flash_Flg(gb_PackFlg));  //LED灯显示
            else
                VFD_LEDDisp(!gb_DispNetFlg, gb_DispNetFlg, false, gb_PackFlg);  //LED灯显示
        }
        Flash_MainDisp(0x3F);   //主显示闪烁
        Flash_AuxDisp(0x07);    //副显示闪烁
    }
}

/*********************************************************************
函数名称: void DiscSumDeal(void)
功    能: 手动卸料累计相关数据处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-16
修    改:                      时    间: 
*********************************************************************/
void DiscSumDeal(void)
{
    static u32  su32_SavedWt;
    static bool sb_SavedWtSigFlg;
    u32 lu32_SumTemp = 0;
    
    if (gb_RunFlg == false)     //停止状态下才可以进行手动卸料
    {
        if (gb_DiscFlg == false)     //未启动手动卸料
        {
            if (gb_DiscSumOnFlg == true)
            {
                /*手动卸料累计信号有效后，等待仪表稳定，保存卸料前的稳定重量，
                供计算卸料后的差值从而进行累计计算，并输出卸料信号*/
                if (gb_StabFlg == true)
                {
                    su32_SavedWt = gu32_Wt;       //保存卸料前重量
                    sb_SavedWtSigFlg = gb_SigWtFlg;       //保存卸料前重量符号+、-
                    gb_DiscFlg = true;        //启动卸料
                    gb_DiscSumOnFlg = false;
                }
            }
        }
        else    //手动卸料已经启动
        {
            if (gb_DiscSumOffFlg == true)
            {
                /*手动卸料累计信号关闭，等待仪表稳定后比较卸料前和卸料后的重量差值，
                从而计算出累计的重量，并关闭卸料信号。*/
                if (gb_StabFlg == true)
                {
                    if (gb_SigWtFlg == sb_SavedWtSigFlg)  //卸料前后符号相同
                    {
                        if (gb_SigWtFlg == false)    //卸料前后都为+号
                        {
                            if (su32_SavedWt >= gu32_Wt)    //卸料前重量大于卸料后重量则计算差值作为卸料值，否则属于不正常卸料不予计算
                            {
                                lu32_SumTemp = (su32_SavedWt - gu32_Wt);
                            }
                        }
                        else        //卸料前后都为-号
                        {
                            if (gu32_Wt >= su32_SavedWt)    //卸料前重量大于卸料后重量则计算差值作为卸料值，否则属于不正常卸料不予计算
                            {
                                lu32_SumTemp = (gu32_Wt - su32_SavedWt);
                            }
                        }
                    }
                    else    //卸料前后符号不同
                    {
                        if (gb_SigWtFlg == true)    //卸料前+卸料后-则有卸料
                        {
                            lu32_SumTemp = (gu32_Wt + su32_SavedWt);
                        }
                    }
                    if (lu32_SumTemp != 0)
                    {
                        Save_SumData(lu32_SumTemp, 1);    //保存累计数据
                    #ifndef DEBUG_MONITORTIME_PRINT
                        //自动打印
                        PrintStart(PRINT_AUTO, lu32_SumTemp, 0);
                    #endif
                    }
                    gb_DiscFlg = false;   //关闭卸料
                    gb_DiscSumOffFlg = false;
                }
            }
        }
    }
}

