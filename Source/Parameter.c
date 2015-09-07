/*
Description:            仪表参数相关函数，如读、写、初始化等操作
Author:                 丛海旭
Date:                   2013-12-9
*/



#include "include.h"
#include "Kalman.h"

/*这是一个随便编的数，用来判断仪表是否有被初始化过或有备份过，
如果初始化过或备份过则在对应地址中存入该数*/
#define INITED  0x12345678  

bool  gb_ExitFlg;              //退出恢复出厂设置
u8 gu8_ResetInTime;              //恢复出厂设置密码输入错误次数
u8 gu8_BorRInTime;              //备份恢复参数的密码输入错误次数
u8 gu8_ModelSetInTime;          //型号自定义密码输入错误次数
u32 gu32_ModelSetSwitch;            //型号自定义开关
u32 gu32_ModelArry[7];              //型号显示保存参数数组 0-logo开关，1~6是6位的型号显示



/*********************************************************************
函数名称: void Read_SetupData(void)
功    能: 读取工作参数相关参数值
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-20
修    改:                      时    间: 
*********************************************************************/
void Read_SetupData(void)
{
    u8 i;
    //获取gs_Setup的变量个数
    i = sizeof(gs_Setup) / sizeof(gs_Setup.TareMode);
    
    gu32_SetupCodeSwitch = FRAM_ReadDW(FMAddr_SetupCodeSwitch); //读工作参数密码保护开关
    gu32_SetupCode = FRAM_ReadDW(FMAddr_SetupCode);     //读工作参数密码
    FRAM_ContReadDW(FMAddr_TareMode, &gs_Setup.TareMode, i);       //读工作参数数据
    gu32_GSDType = FRAM_ReadDW(FMAddr_GSDType);     //读GSD文件类型
    gu32_FollowMasterSwitch = FRAM_ReadDW(FMAddr_FollowMasterSwitch);     //工作参数标定参数由主站控制开关
    gu32_IoTestCodeSwitch = FRAM_ReadDW(FMAddr_IoTestCodeSwitch);     //读开关量测试参数密码
    gu32_IoTestCode = FRAM_ReadDW(FMAddr_IoTestCode);     //读开关量测试参数密码
}

/*********************************************************************
函数名称: void Read_SumData(void)
功    能: 读累计重量和次数
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-15
修    改:                      时    间: 
*********************************************************************/
void Read_SumData(void)
{
    u8 i;
    
    for (i = 1; i <= 20; i++)
    {
        gu32_SumWt_Rec[i] = FRAM_ReadDW(FMAddr_SumWt_1 + i * 4);
        gu32_SumNum_Rec[i] = FRAM_ReadDW(FMAddr_SumNum_1 + i * 4);
    }
    
    //累计重量、次数、收发货累计
    gu64_SumWt = FRAM_Read8Byte(FMAddr_SumWt);
    gu32_SumNum = FRAM_ReadDW(FMAddr_SumNum);
    
//    gu64_SumWt = 10000203; //for test
//    gu32_SumNum = 987654321;    //for test
    
    //获取显示的4字节的重量和对应的单位
    if (GetSumWt_u32(gu64_SumWt,      //需要转换的当前8字节重量数据
                     gs_Cal.Unit,     //当前重量对应的单位
                     gs_Cal.Point,    //当前重量对应的小数点
                     &gu32_SumWt,     //得到的4字节目标重量
                     &gu32_SumWtUnit,   //目标重量的单位
                     &gu32_SumWtPoint   //目标重量的小数点
                    ))
    {
        ClrSum();
    }
}

/*********************************************************************
函数名称: void Read_RecipeData(u32 ful_RecNum)
功    能: 读取配方参数相关参数值
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-20
修    改:                      时    间: 
*********************************************************************/
void Read_RecipeData(u32 ful_RecNum)
{
    u8 i;
    //获取gs_Cal的变量个数
    i = sizeof(gs_Rec) / sizeof(gs_Rec.Target);
    
    gu32_RecCodeSwitch = FRAM_ReadDW(FMAddr_RecCodeSwitch); //读配方参数密码保护开关
    gu32_RecCode = FRAM_ReadDW(FMAddr_RecCode);     //读配方参数密码
    
    FRAM_ContReadDW(FMAddr_Target(ful_RecNum), &gs_Rec.Target, i);       //读配方参数数据
}

/*********************************************************************
函数名称: void Read_CalibData(void)
功    能: 读取标定相关参数值
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-13
修    改:                      时    间: 
*********************************************************************/
void Read_CalibData(void)
{
    u8 i;
    //获取gs_Cal的变量个数
    i = sizeof(gs_Cal) / sizeof(gs_Cal.Unit) - 1;   //注意ADK单独读 所以-1
    
    gu32_CalCode = FRAM_ReadDW(FMAddr_CalCode);     //读标定密码
    FRAM_ContReadDW(FMAddr_CalUnit, &gs_Cal.Unit, i);       //读标定参数数据
    gs_Cal.ADK = FRAM_ReadFloat(FMAddr_CalADK);
}

/*********************************************************************
函数名称: void Read_IoDefData(void)
功    能: 读取开关量自定义设置
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-25
修    改:                      时    间: 
*********************************************************************/
void Read_IoDefData(void)
{
    //将8个输入自定义和12个输出自定义读入到对应的数组中
    FRAM_ContReadDW(FMAddr_IoInBuf1, &gu32_IoInBuf_BINYES[1] , 8);
    FRAM_ContReadDW(FMAddr_IoOutBuf1, &gu32_IoOutBuf_BINYES[1], 12);
    //BINNO将8个输入自定义和12个输出自定义读入到对应的数组中
    FRAM_ContReadDW( FMAddr_IoInBuf1_BINNO, &gu32_IoInBuf_BINNO[1] , 8 );
    FRAM_ContReadDW( FMAddr_IoOutBuf1_BINNO, &gu32_IoOutBuf_BINNO[1], 12 );
    //BIN_2将8个输入自定义和12个输出自定义读入到对应的数组中
    FRAM_ContReadDW( FMAddr_IoInBuf1_BIN_2, &gu32_IoInBuf_BIN_2[1] , 8 );
    FRAM_ContReadDW( FMAddr_IoOutBuf1_BIN_2, &gu32_IoOutBuf_BIN_2[1], 12 );
    //NO_2将8个输入自定义和12个输出自定义读入到对应的数组中
    FRAM_ContReadDW( FMAddr_IoInBuf1_NO_2, &gu32_IoInBuf_NO_2[1] , 8 );
    FRAM_ContReadDW( FMAddr_IoOutBuf1_NO_2, &gu32_IoOutBuf_NO_2[1], 12 );
    
    gu32_IoDefCodeSwitch = FRAM_ReadDW(FMAddr_IoDefCodeSwitch);     //读开关量自定义密码开关
    gu32_IoDefCode = FRAM_ReadDW(FMAddr_IoDefCode);     //读开关量自定义密码
    IoDefPointerRefresh(gs_Setup.TareMode);  //更新开关量自定义数组指针
}

/*********************************************************************
函数名称: void Init_Parameter(void)
功    能: 变量值的读取
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-2-18
修    改:                      时    间: 
*********************************************************************/
void Init_Parameter(void)
{
    gb_ConveyorRunFlg = false;
    gb_FlashFlg = false;
    gb_ZeroKeyFlg = false;
    gb_ForceClrZeroFlg = false;
    gb_DispErrorFlg = false;
    gb_FlapWaitFlg = false;         //拍袋延时已启动
    gb_FlapStartFlg = false;        //拍袋已经启动
    gb_FlapBagOutFlg = false;       //拍袋输出信号
    gb_FlapAllHighFlg = false;
    gb_LackFlg = false;
    gb_FillFlg = false;
    gb_ExtFlapBagFlg = false;
    gb_AddFlapBagFlg = false;
    gb_PrintAllRecFlg_UART0 = false;
    gb_PrintAllRecFlg_UART1 = false;
    gb_DiscEnableOutFlg = true;
    gu16_MbIOOutInit = 0;
    gb_GoFlg = false;
    gu16_MbIoTestSwitch = 0;
    gu16_MbIoOutTest = 0;
    gb_BatchOverFlg = false;
    gu8_DispErrorNumber = 0;
//    gu8_DispErrorNumberBackup = 0;
    gb_DiscFlg = false; 
    gb_PackFlg = false;
    gb_JSDDelayFlg = false;                 //清除夹松袋延时标志
    gb_SDDelayFlg = false;
    gb_SDBDelayFlg = false;
    gb_DispNetFlg = false;  
    gb_DiscShakeFlg = false;     //A秤卸料震打输出
    gb_DiscShakeStartFlg = false;     //A秤卸料震打开始
    gb_Error9Flg = false;                    //A秤卸料未干净报警
    gb_AddFlapBagFlg = false;                //A加料中拍袋标志位
    gb_ExtFlapBagFlg = false;                //A额外拍袋输出标志位
    gb_TareGetedFlg = false;
    gb_PrintCodeIngFlg = false;
    gu16_MbIoTestSwitch = 0;
    gu16_MbIoInState = 0;           //开关量输入状态
    gu16_MbIoOutState = 0;          //开关量输出状态
    gb_MbKeyLockFlg = false;
    gb_EndFlg = false;              //结束输入
    gb_DebugUart1 = false;
    gb_DebugUart0 = false;
    gb_LiftBagStartFlg = false;
    gb_PushBagStartFlg = false;
    gb_ConveyorStartFlg = false;
    gb_AddingOutFlg = false;
    
    gu32_BatchTime = FRAM_ReadDW(FMAddr_BatchTime);      //批次数
    gu32_BatchTimeCnt = FRAM_ReadDW(FMAddr_BatchTimeCnt);      //剩余批次数
    Read_CalibData();           //读标定参数值
    Read_SetupData();           //读工作参数值
    gu32_RecNum = FRAM_ReadDW(FMAddr_RecNum);        //读配方号
    Read_RecipeData(gu32_RecNum);                  //读配方各参数值
    Read_IoDefData();           //读开关量自定义
    Read_SumData();             //读累计参数
    kalman1_init(&gs_KalmanState, XLAST, PLAST);
}

/*********************************************************************
函数名称: void Reset_Cal(void)
功    能: 标定参数恢复出厂设置
说    明: 构造一个初始化数据数组，利用连续写入双字的函数写入对应参数的存储地址当中
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-30
修    改:                      时    间: 
*********************************************************************/
void Reset_Cal(void)
{                            //密码 单位  小数点 分度 量程  灵敏度 零点 
    const u32 cu32_CalData[] = {0,  UNIT_kg, 2,   1,  10000, 2,    0x2000};
    FRAM_ContWriteDW(FMAddr_CalCode, (u32 *)cu32_CalData, sizeof(cu32_CalData)/sizeof(cu32_CalData[0]));           //将出厂数据写入对应的存储地址
    //增益初值
    FRAM_WriteFloat(FMAddr_CalADK, 10.0f);
}

/*********************************************************************
函数名称: void Reset_Setup(bool fb_MbFlg)
功    能: 恢复工作参数
说    明: 构造一个初始化数据数组，利用连续写入双字的函数写入对应参数的存储地址当中
入口参数: fb_MbFlg =false键盘初始化  =true Modbus通讯初始化，不初始化串口相关参数
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-30
修    改:                      时    间: 
*********************************************************************/
void Reset_Setup(bool fb_MbFlg)
{                             
    const u32 cu32_SetupData[] = {
    //密码开关 密码
       0,       0,
    //秤体模式 毛净重 给料方式 清零间隔 判稳超时 副显内容
       BINYES,  NET,     CO,    0,       0,       0,
    //清零范围 判稳范围 追零范围 滤波  卸料滤波 定值滤波 AD采样 扩展接口类型
       50,       1,      1,       5,    3,       4,       0,      0,        
    //上电清零 卸料累计 定值保持 累计查询密码 累计清除密码 累计扩展
       0,       0,       0,       0,            0,          1,
    //通讯方式 秤号 波特率 校验     HiLo 自动打印 打印类型 打印语言    打印走纸行数
      COM_PRINT,  1,   2,  P_18N1,   0,   0,       1,      PRINTER_ENG,      3,
    //通讯方式 秤号 波特率 校验     HiLo 自动打印 打印类型 打印语言 打印走纸行数
      COM_CONT, 1,   2,    P_17E1,   0,   0,       1,      PRINTER_ENG,      3,
    //无斗同时松袋 输送机启动延时 输送机运行时间 再次夹袋延时加料
      0,            5,             40,            20,
    //自适应开关 SP2 最大门限 等级 时间分布 SP3 最大门限 幅度 预留 预留
      0,         0,  100,     1,   500,     0,  100,     1,   0,   0,
    };
    
    u8 j;
    if (fb_MbFlg)   //Modbus通讯不初始化串口相关
    {   //密码 B1 B2 B3
        FRAM_ContWriteDW(FMAddr_SetupCodeSwitch, (u32 *)cu32_SetupData, 22);           //将出厂数据写入对应的存储地址
        //B6
        FRAM_ContWriteDW(FMAddr_No2PackOffTwoSwitch, (u32 *)&cu32_SetupData[40], 4);           //将出厂数据写入对应的存储地址
    }
    else    //恢复所有
    {
        j = sizeof(cu32_SetupData) / sizeof(cu32_SetupData[0]);
        FRAM_ContWriteDW(FMAddr_SetupCodeSwitch, (u32 *)cu32_SetupData, j);           //将出厂数据写入对应的存储地址
    }
    
    FRAM_WriteDW(FMAddr_GSDType, 0);               //GSD文件
    FRAM_WriteDW(FMAddr_FollowMasterSwitch, 0);               //工作参数标定参数由主站控制开关
    FRAM_WriteDW(FMAddr_IoTestCodeSwitch, 0);         //IO测试密码开关
    FRAM_WriteDW(FMAddr_IoTestCode, 0);               //IO测试密码
}

/*********************************************************************
函数名称: void Reset_Rec(void)
功    能: 恢复配方参数
说    明: 构造一个初始化数据数组，利用连续写入双字的函数写入对应参数的存储地址当中
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-4-12
修    改:                      时    间: 
*********************************************************************/
void Reset_Rec(void)
{
    const u32 cu32_RecData[] = {
    //目标 大投 中投 落差 零区 拍袋起始
       0,  0,    0,   0,   0,   0,
    //超欠开关 Over under 超欠暂停
       0,      0,   0,    0,
    //T1 T2A T3 T4 T5 T6 T7 T2B T9 T10 T11 T12
       5,9,  5,  5, 5, 5, 5, 9, 0,  0, 0,  0,
    //修正开关 次数 范围 幅度
       0,      1,   20,  2,
    //拍袋模式 中次数 后次数 拍袋延时 有效时间 无效时间 额外拍袋时间
       0,      0,     4,      5,       5,        5,         0,
    //小投点动 有效时间 无效时间
       0,      5,       5,
    //单秤组合次数
       1,
    //卸料震打 卸料有效 高时间 低时间 次数
       0,      20,      5,     5,     10,
    //打码开关 启动延时 打码时间 屏蔽卸料
       0,      5,       5,       0
       };
       
    u8 i, j;
    
    j = sizeof(cu32_RecData) / sizeof(cu32_RecData[0]);
    for (i = 1; i <= 20; i++)            //恢复各个配方参数
        FRAM_ContWriteDW(FMAddr_Target(i), (u32 *)cu32_RecData, j);           //将出厂数据写入对应的存储地址
    FRAM_WriteDW(FMAddr_RecNum, 1);                   //恢复配方号为1
    FRAM_WriteDW(FMAddr_RecCodeSwitch, 0);            //配方参数密码开关关闭
    FRAM_WriteDW(FMAddr_RecCode, 0);                  //配方参数密码000000
}

/*********************************************************************
函数名称: void Reset_IoDef(void)
功    能: 
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-4-12
修    改:                      时    间: 
*********************************************************************/
void Reset_IoDef(void)
{
    //输入和输出开关量出厂设置数据
    const u32 cu32_IoDefDatap[] = {1, 2, 3, 4, 5, 6, 7, 8,
                                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                   1, 2, 3, 4, 5, 6, 7, 8,
                                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                   1, 2, 3, 4, 5, 6, 7, 23,
                                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 21,
                                   1, 2, 3, 4, 5, 6, 7, 23,
                                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 22, 21
                                   };
    FRAM_ContWriteDW(FMAddr_IoInBuf1, (u32 *)cu32_IoDefDatap, sizeof(cu32_IoDefDatap)/sizeof(cu32_IoDefDatap[0]));           //将出厂数据写入对应的存储地址
    FRAM_WriteDW(FMAddr_IoDefCodeSwitch, 1);          //IO自定义密码开关
    FRAM_WriteDW(FMAddr_IoDefCode, 0);                //IO自定义密码
}

/*********************************************************************
函数名称: void Reset_All(void)
功    能: 恢复全部参数
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-4-12
修    改:                      时    间: 
*********************************************************************/
void Reset_All(void)
{
    if (INITED != FRAM_ReadDW(FMAddr_HadBeenInited))
    {   //判断仪表是否有被初始化过，如果没有则进行如下处理
        
        //定时锁机功能密码设置为000000,开关关闭
        gu32_ShutDownSwitch = 0;
        FRAM_WriteDW(FMAddr_ShutDownCode, 0);
        gu32_ShutDownCode = 0;
        FRAM_WriteDW(FMAddr_ShutDownSwitch, 0);
        //清除累计数据
        ClrSum();
        //关闭型号自定义功能
        gu32_ModelSetSwitch = 0;
        gu32_ModelArry[0] = 1;
        gu32_ModelArry[1] = 8;
        gu32_ModelArry[2] = 8;
        gu32_ModelArry[3] = 0;
        gu32_ModelArry[4] = 4;
        gu32_ModelArry[5] = 12;
        gu32_ModelArry[6] = 2;
        FRAM_WriteDW(FMAddr_ModelSetSwitch, gu32_ModelSetSwitch);   //型号自定义开关
        FRAM_ContWriteDW(FMAddr_ModelSet0, gu32_ModelArry, 7);        //读型号设置和logo开关
        
        //写入初始化判断地址数据INITED，表示已经进行过初始化
        FRAM_WriteDW(FMAddr_HadBeenInited, INITED);
    }
    
    Reset_Cal();
    Reset_Setup(false);
    Reset_Rec();
    Reset_IoDef();
    
    FRAM_WriteDW(FMAddr_BatchTime, 0);
    FRAM_WriteDW(FMAddr_BatchTimeCnt, 0);
}

/*********************************************************************
函数名称: void Key_ResetState(void)
功    能: 恢复出厂设置操作
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-30
修    改:                      时    间: 
*********************************************************************/
void Key_ResetState(void)
{
    u8 luc_DataLen = 0;      //输入数据位数
    
    if (gu32_KeyCode == K_RIGHT || gu32_KeyCode == K_INC)
    {
        if (gu8_Item != RESET_CHECKCODE)        //密码输入时按→不进入下一项
            gu8_Item++;
        if (gu8_Item > RESET_IODEF)
            gu8_Item = RESET_ALL;
    }
    switch (gu8_Item)
    {
        case RESET_CHECKCODE:       //密码输入
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            break;
        default:
            luc_DataLen = 0;
            break;
    }
    
    //数据输入
    ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
    
    //每一项下的按键处理程序
    if(gu32_KeyCode ==K_ESC)           //退出按键处理
    {
        gu32_SystemState = STOPSTATE;
        gb_FirstNumKeyFlg = true;
        gu8_Item = 0;
        gb_FlashFlg = false;
        gb_ExitFlg = true;            //退出恢复出厂设置状态
    }
    if (gu32_KeyCode == K_ENTER)         //确认键处理
    {
        gb_FirstNumKeyFlg = true;
        gu8_DataInLength = 0;
        switch(gu8_Item)
        {
            case RESET_CHECKCODE:       //密码输入
                gu8_ResetInTime++;
                if (ulParameter.ul == RESETCODE)        //输入密码等于恢复出厂设置密码
                {
                    gu8_ResetInTime = 0;            //清除密码输入错误次数计数器
                    ulParameter.ul = 0;
                }
                else if (gu8_ResetInTime >= 3)      //密码输入错误超过3次
                {
                    ErrorDeal(4, 0);        //显示error4
                    Disp_Error();
                    VFD_CopyDispArryToDispRam();         //将显示数组存入显示缓存
                    VFD_WriteDispRamArry();                    //写显示缓存 
                    while(1);
                }
                else            //密码输入错误，提示error
                {
                    ErrorDeal(0, 100);
                    ulParameter.ul = 0xFFFFFFFF;
                    return;
                }
                break;
            case RESET_ALL:
                Reset_All();            //恢复全部参数
                Disp_Delay();
                gu32_SystemState = STOPSTATE;
                gb_FirstNumKeyFlg = true;
                gu8_Item = 0;
                gb_FlashFlg = false;
                gb_ExitFlg = true;            //退出恢复出厂设置状态
                break;
            case RESET_CAL:
                Reset_Cal();            //恢复标定参数
                Disp_Delay();
                break;
            case RESET_SET:
                Reset_Setup(false);          //恢复工作参数
                Disp_Delay();
                break;
            case RESET_REC:
                Reset_Rec();            //恢复配方参数
                Disp_Delay();
                break;
            case RESET_IODEF:
                Reset_IoDef();          //恢复开关量自定义
                Disp_Delay();
                break;
            default:
                break;
        }
        if (++gu8_Item > RESET_IODEF)
            gu8_Item = RESET_ALL;
    }
}

/*********************************************************************
函数名称: void Disp_ResetState(void)
功    能: 恢复出厂设置显示处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-30
修    改:                      时    间: 
*********************************************************************/
void Disp_ResetState(void)
{
    gb_FlashFlg = false;
    
    if (gu8_Item != RESET_CHECKCODE)
        VFD_AuxDisp_Str("ini");

    switch (gu8_Item)
    {
        case RESET_CHECKCODE:       //密码输入
            Disp_InputCode();       //密码输入显示处理
            gb_FlashFlg = true;
            VFD_AuxDisp_Str("INI");
            Flash_MainDisp(0x01);                //闪烁处理
            break;
        case RESET_CAL:
            VFD_MainDisp_Str("   CAL");
            break;
        case RESET_SET:
            VFD_MainDisp_Str("   SET");
            break;
        case RESET_REC:
            VFD_MainDisp_Str("   REC");
            break;
        case RESET_IODEF:
            VFD_MainDisp_Str("    IO");
            break;
        case RESET_ALL:
            VFD_MainDisp_Str("   ALL");
            break;
    }
}

/*********************************************************************
函数名称: void Reset_Parameter(void)
功    能: 恢复出厂设置
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-4-11
修    改:                      时    间: 
*********************************************************************/
void Reset_Parameter(void)
{
    gb_ExitFlg = false;
    ulParameter.ul = 0xFFFFFFFF;
    gu32_KeyCode = 0;
    gu8_Item = 0;
    gu8_DataInLength = 0;

    while (gb_ExitFlg == false)
    {
        gu32_KeyCode = Key_Deal();      //读按键
        OPBuzzer();
        Key_ResetState();               //恢复出厂设置操作
        VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);       //清显示缓存
        Disp_ResetState();              //恢复出厂设置显示处理
        if (gb_DispErrorFlg)        //显示error时不填充设置相关显示缓存
            Disp_Error();           //error报警显示处理
        VFD_CopyDispArryToDispRam();    //将显示数组存入显示缓存
        VFD_WriteDispRamArry();         //写显示缓存 
    }
}

/*********************************************************************
函数名称: Disp_Delay
功    能: 参数备份和恢复时的进度显示
说    明: 
调    用: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2010-8-16
修    改:                      时    间: 
*********************************************************************/
void Disp_Delay(void)
{
    u8 i;
    
    memset(gu8_MainDispArray, SEG_BLK, sizeof(gu8_MainDispArray));
    for (i = 0; i < 6; i++)
    {
        gu8_MainDispArray[i] = SEG_SIG;
        VFD_CopyDispArryToDispRam();         //将显示数组存入显示缓存
        VFD_WriteDispRamArry();                    //写显示缓存 
        delay(60);
    }
}

/*********************************************************************
函数名称: void Backup_Parameter(void)
功    能: 备份仪表参数
说    明: 包括工作参数，配方，开关量自定义
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-5-27
修    改:                      时    间: 
*********************************************************************/
void Backup_Parameter(void)
{
    u32 lu32_ParameterAddr = SETUP_BASE;        //配置参数起始地址
    u32 lu32_BackupAddr = BACKUPSTART;          //备份起始地址
    u32 i = (BACKUPSTART - SETUP_BASE) / 4;         //备份数据的总数
    
    for (; i != 0; i--, lu32_ParameterAddr += 4, lu32_BackupAddr += 4)
    {
        FRAM_WriteDW_Backup(lu32_BackupAddr, FRAM_ReadDW(lu32_ParameterAddr));
    }
    //将备份的时间进行保存，并保存INITED代表已经进行过备份
    RTC_ReadDateTime(&SysDataTime);      //读系统日期和时间
    FRAM_WriteDW(FMAddr_BackupYear, SysDataTime.year);
    FRAM_WriteDW(FMAddr_BackupMonth, SysDataTime.month);
    FRAM_WriteDW(FMAddr_BackupDate, SysDataTime.date);
    FRAM_WriteDW(FMAddr_Backuped, INITED);
}

/*********************************************************************
函数名称: void Reload_Parameter(void)
功    能: 恢复仪表参数
说    明: 包括工作参数，配方，开关量自定义
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-5-28
修    改:                      时    间: 
*********************************************************************/
void Reload_Parameter(void)
{
    u32 lu32_ParameterAddr = SETUP_BASE;    //配置参数起始地址
    u32 lu32_BackupAddr = BACKUPSTART;        //备份起始地址
    u32 i = (BACKUPSTART - SETUP_BASE) / 4;         //备份数据的总数
    
    for (; i != 0; i--, lu32_ParameterAddr += 4, lu32_BackupAddr += 4)
    {
        FRAM_WriteDW(lu32_ParameterAddr, FRAM_ReadDW(lu32_BackupAddr));
    }
}

/*********************************************************************
函数名称: void Key_BorRState(void)
功    能: 备份和恢复参数操作
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-31
修    改:                      时    间: 
*********************************************************************/
void Key_BorRState(void)
{
    u8 luc_DataLen = 0;      //输入数据位数
    
    switch (gu8_Item)
    {
        case BORR_RELOAD_CHECKCODE:       //密码输入
        case BORR_BACKUP_CHECKCODE:
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            //数据输入
            ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
            break;
        default:
            break;
    }
    
    //每一项下的按键处理程序
    switch (gu32_KeyCode)
    {
        case K_ESC:
            if (gu8_Item == BORR_RELOAD || gu8_Item == BORR_BACKUP)
            {
                gu32_SystemState = STOPSTATE;
                gb_FirstNumKeyFlg = true;
                gu8_Item = 0;
                gb_FlashFlg = false;
                gb_ExitFlg = true;            //退出恢复出厂设置状态
            }
            else
                gu8_Item--;
            if (gu8_Item== BORR_RELOAD_DATE ||
                gu8_Item == BORR_BACKUP_DATE)      //备份日期显示
            {
                ulParameter.ul = FRAM_ReadDW(FMAddr_Backuped);
                if (ulParameter.ul == INITED)   //有备份过
                {   //读出备份的年月日信息
                    SysDataTime.year = FRAM_ReadDW(FMAddr_BackupYear);
                    SysDataTime.month = FRAM_ReadDW(FMAddr_BackupMonth);
                    SysDataTime.date = FRAM_ReadDW(FMAddr_BackupDate);
                }
            }
            break;
        case K_ENTER:
            if (gu8_Item == BORR_BACKUP_CHECKCODE || gu8_Item == BORR_RELOAD_CHECKCODE)
            {
                gu8_BorRInTime++;
                if ((ulParameter.ul == BACKUPCODE && gu8_Item == BORR_BACKUP_CHECKCODE) ||
                    (ulParameter.ul == RELOADCODE && gu8_Item == BORR_RELOAD_CHECKCODE))       //输入密码等于恢复出厂设置密码
                {
                    if (gu8_Item == BORR_BACKUP_CHECKCODE)
                        Backup_Parameter();
                    else
                        Reload_Parameter();
                    Disp_Delay();
                    gu8_BorRInTime = 0;            //清除密码输入错误次数计数器
                    ulParameter.ul = 0;
                    gu8_Item -= 2;
                }
                else if (gu8_BorRInTime >= 3)      //密码输入错误超过3次
                {
                    ErrorDeal(4, 0);        //显示error4
                    Disp_Error();
                    VFD_CopyDispArryToDispRam();         //将显示数组存入显示缓存
                    VFD_WriteDispRamArry();                    //写显示缓存 
                    while(1);
                }
                else            //密码输入错误，提示error
                {
                    ErrorDeal(0, 100);
                    ulParameter.ul = 0xFFFFFFFF;
                    return;
                }
                break;
            }
            else
                gu8_Item++;
            gu8_DataInLength = 0;
            if (gu8_Item== BORR_RELOAD_DATE ||
                gu8_Item == BORR_BACKUP_DATE)      //备份日期显示
            {
                ulParameter.ul = FRAM_ReadDW(FMAddr_Backuped);
                if (ulParameter.ul == INITED)   //有备份过
                {   //读出备份的年月日信息
                    SysDataTime.year = FRAM_ReadDW(FMAddr_BackupYear);
                    SysDataTime.month = FRAM_ReadDW(FMAddr_BackupMonth);
                    SysDataTime.date = FRAM_ReadDW(FMAddr_BackupDate);
                }
            }
            break;
        case K_RIGHT:
            if (gu8_Item == BORR_RELOAD)
                gu8_Item = BORR_BACKUP;
            else if (gu8_Item == BORR_BACKUP)
                gu8_Item = BORR_RELOAD;
            break;
    }
//    if(gu32_KeyCode ==K_ESC)           //退出按键处理
//    {
//        gu32_SystemState = STOPSTATE;
//        gb_FirstNumKeyFlg = true;
//        gu8_Item = 0;
//        gb_FlashFlg = false;
//        gb_ExitFlg = true;            //退出恢复出厂设置状态
//    }
//    if (gu32_KeyCode == K_ENTER)         //确认键处理
//    {
//        gb_FirstNumKeyFlg = true;
//        gu8_DataInLength = 0;
//        switch(gu8_Item)
//        {
//            case BORR_CHECKCODE:       //密码输入
//                gu8_BorRInTime++;
//                if ((ulParameter.ul == BACKUPCODE && gu32_SystemState == BORRSTATE) ||
//                    (ulParameter.ul == RELOADCODE && gu32_SystemState == RELOADSTATE))       //输入密码等于恢复出厂设置密码
//                {
//                    gu8_BorRInTime = 0;            //清除密码输入错误次数计数器
//                    ulParameter.ul = 0;
//                    gu8_Item = BORR_DEAL;
//                }
//                else if (gu8_BorRInTime >= 3)      //密码输入错误超过3次
//                {
//                    ErrorDeal(MDP_A, 4, 0);        //显示error4
//                    Disp_Error();
//                    VFD_WriteUpMainDispArry(g_UCurrDisp.ALL);         //将显示数组存入显示缓存
//                    VFD_Write();                    //写显示缓存
//                    while(1);
//                }
//                else            //密码输入错误，提示error
//                {
//                    ErrorDeal(MDP_A, 0, 100);
//                    ulParameter.ul = 0xFFFFFFFF;
//                    return;
//                }
//                break;
//            case BORR_DEAL:         //执行参数备份或恢复
//                if (gu32_SystemState == BORRSTATE)
//                {
//                    Backup_Parameter();         //备份参数
//                    BuzzerOff();
//                    Disp_Delay();
//                }
//                else
//                {
//                    Reload_Parameter();         //恢复参数'
//                    BuzzerOff();
//                    Disp_Delay();
//                }
//                
//                gu32_SystemState = STOPSTATE;
//                gb_FirstNumKeyFlg = true;
//                gu8_Item = 0;
//                gb_FlashFlg = false;
//                gb_ExitFlg = true;            //退出恢复出厂设置状态
//                break;
//            default:
//                break;
//        }
//    }
}

/*********************************************************************
函数名称: void Disp_BorRState(void)
功    能: 备份和恢复参数的显示处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-31
修    改:                      时    间: 
*********************************************************************/
void Disp_BorRState(void)
{
    gb_FlashFlg = false;

    switch  (gu8_Item)
    {
        case BORR_RELOAD_CHECKCODE:       //密码输入
        case BORR_BACKUP_CHECKCODE:
            Disp_InputCode();       //密码输入显示处理
            gb_FlashFlg = true;
            Flash_MainDisp(0x01);                //闪烁处理
            break;
        case BORR_RELOAD:
            VFD_MainDisp_Str("   RLD");
            VFD_AuxDisp_Str("-1-");
            break;
        case BORR_RELOAD_DATE:
            if (ulParameter.ul == INITED)
            {
                RTC_sDatetoASC(&SysDataTime, gu8_MainDispArray);
                VFD_MainDisp_ASCII(gu8_MainDispArray);
                gu8_MainDispArray[1] |= 0x80;
                gu8_MainDispArray[3] |= 0x80;
            }
            else
            VFD_MainDisp_Str("  none");
            VFD_AuxDisp_Str("-1-");
            break;
        case BORR_BACKUP:
            VFD_MainDisp_Str("   BAC");
            VFD_AuxDisp_Str("-2-");
            break;
        case BORR_BACKUP_DATE:
            if (ulParameter.ul == INITED)
            {
                RTC_sDatetoASC(&SysDataTime, gu8_MainDispArray);
                VFD_MainDisp_ASCII(gu8_MainDispArray);
                gu8_MainDispArray[1] |= 0x80;
                gu8_MainDispArray[3] |= 0x80;
            }
            else
                VFD_MainDisp_Str("  none");
            VFD_AuxDisp_Str("-2-");
            break;
        default:
            break;
    }
}

/*********************************************************************
函数名称: void BorR_Parameter(void)
功    能: 
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-2
修    改:                      时    间: 
*********************************************************************/
void BorR_Parameter(void)
{
    gb_ExitFlg = false;
    ulParameter.ul = 0xFFFFFFFF;
    gu32_KeyCode = 0;
    gu8_Item = 0;
    gu8_DataInLength = 0;

    while (gb_ExitFlg == false)
    {
        gu32_KeyCode = Key_Deal();                     //读按键
        OPBuzzer();
        Key_BorRState();                //备份和恢复参数操作
        VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);       //清显示缓存
        Disp_BorRState();               //备份和恢复参数显示处理
        if (gb_DispErrorFlg)        //显示error时不填充设置相关显示缓存
            Disp_Error();           //error报警显示处理
        VFD_CopyDispArryToDispRam();    //将显示数组存入显示缓存
        VFD_WriteDispRamArry();         //写显示缓存 
    }
}

/*********************************************************************
函数名称: void ModelSetStateDeal(void)
功    能: 型号设置
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-2
修    改:                      时    间: 
*********************************************************************/
void Key_ModelSetState(void)
{
    u8 luc_DataLen = 0;      //输入数据位数
    
    switch (gu8_Item)
    {
        case MODELSET_CHECKCODE:       //密码输入
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            //数据输入
            ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
            break;
        case MODELSET_SWITCH:
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //两项切换，用↑键修改
            break;
        case MODELSET_SETUP_0:      //电源灯
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //两项切换，用↑键修改
            break;
        case MODELSET_SETUP_1:
        case MODELSET_SETUP_2:
        case MODELSET_SETUP_3:
        case MODELSET_SETUP_4:
        case MODELSET_SETUP_5:
        case MODELSET_SETUP_6:
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 36);
            ulParameter.ul = Key_T9Input(ulParameter.ul);        //T9输入法
            break;
        default:
            luc_DataLen = 0;
            break;
    }
    
    //每一项下的按键处理程序
    switch (gu32_KeyCode)
    {
        case K_RIGHT:
            if (gu8_Item != MODELSET_CHECKCODE && gu8_Item != MODELSET_SWITCH)        //密码输入时按→不进入下一项
            {
                gu8_Item++;
                if (gu8_Item == MODELSET_SETUP_1)
                    gu32_ModelArry[0] = FRAM_ReadDW(FMAddr_ModelSet0);
                if (gu8_Item > MODELSET_SETUP_6)
                    gu8_Item = MODELSET_SETUP_0;
                ulParameter.ul = gu32_ModelArry[gu8_Item-2];
            }
            break;
        case K_ESC:
            gu32_SystemState = STOPSTATE;
            gb_FirstNumKeyFlg = true;
            gu8_Item = 0;
            gb_FlashFlg = false;
            gb_ExitFlg = true;            //退出恢复出厂设置状态
            break;
        case K_ENTER:
            gb_FirstNumKeyFlg = true;
            gu8_DataInLength = 0;
            switch(gu8_Item)
            {
                case MODELSET_CHECKCODE:       //密码输入
                    gu8_ModelSetInTime++;
                    if (ulParameter.ul == SUPERCODE)        //输入密码等于恢复出厂设置密码
                    {
                        gu8_ModelSetInTime = 0;            //清除密码输入错误次数计数器
                        ulParameter.ul = gu32_ModelSetSwitch;
                    }
                    else if (gu8_ModelSetInTime >= 3)      //密码输入错误超过3次
                    {
                        ErrorDeal(4, 0);        //显示error4
                        Disp_Error();
                        VFD_CopyDispArryToDispRam();         //将显示数组存入显示缓存
                        VFD_WriteDispRamArry();                    //写显示缓存 
                        while(1);
                    }
                    else            //密码输入错误，提示error
                    {
                        ErrorDeal(0, 100);
                        ulParameter.ul = 0xFFFFFFFF;
                        return;
                    }
                    break;
                case MODELSET_SWITCH:
                    gu32_ModelSetSwitch = ulParameter.ul;
                    FRAM_WriteDW(FMAddr_ModelSetSwitch, gu32_ModelSetSwitch);
                    gu32_FlashDelay = gu32_Sys10ms;
                    if (gu32_ModelSetSwitch == 0)
                        return;
                    break;
                case MODELSET_SETUP_0:      //电源灯
                case MODELSET_SETUP_1:
                case MODELSET_SETUP_2:
                case MODELSET_SETUP_3:
                case MODELSET_SETUP_4:
                case MODELSET_SETUP_5:
                case MODELSET_SETUP_6:
                    gu32_ModelArry[gu8_Item-2] = ulParameter.ul;
                    FRAM_WriteDW(FMAddr_ModelSet0+(gu8_Item-2)*4, gu32_ModelArry[gu8_Item-2]);
                    break;
                default:
                    break;
            }
            if (++gu8_Item > MODELSET_SETUP_6)
            {
                gu8_Item = MODELSET_SETUP_0;
                gu32_FlashDelay = gu32_Sys10ms;
            }
            if (gu8_Item >= MODELSET_SETUP_0)
            {
                ulParameter.ul = FRAM_ReadDW(FMAddr_ModelSet0+(gu8_Item-2)*4);
            }
            break;
    }
}

/*********************************************************************
函数名称: void Disp_ModelSetState(void)
功    能: 型号设置
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-2
修    改:                      时    间: 
*********************************************************************/
void Disp_ModelSetState(void)
{
//    u8 lu8_FlashBit = 0;
    
    gb_FlashFlg = false;
    
    if (gu8_Item >= MODELSET_SETUP_0 && gu8_Item <= MODELSET_SETUP_6)
    {
        gu8_MainDispArray[0] = Value_To_SEG[gu32_ModelArry[1]];
        gu8_MainDispArray[1] = Value_To_SEG[gu32_ModelArry[2]];
        gu8_MainDispArray[2] = Value_To_SEG[gu32_ModelArry[3]];
        gu8_MainDispArray[3] = Value_To_SEG[gu32_ModelArry[4]];
        gu8_MainDispArray[4] = Value_To_SEG[gu32_ModelArry[5]];
        gu8_MainDispArray[5] = Value_To_SEG[gu32_ModelArry[6]];
        if (gu8_Item >= MODELSET_SETUP_1)
            gu8_MainDispArray[gu8_Item-3] = Value_To_SEG[ulParameter.ul];
    }
    
    switch  (gu8_Item)
    {
        case MODELSET_CHECKCODE:       //密码输入
            Disp_InputCode();       //密码输入显示处理
            gb_FlashFlg = true;
            break;
        case MODELSET_SWITCH:
            Disp_OnOff(ulParameter.ul);           //显示开关
            break;
        case MODELSET_SETUP_0:      //电源灯
            gu32_ModelArry[0] = ulParameter.ul;
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_ZERO(true);
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_ZERO(false);
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_1:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_STAB(true);            //灭掉
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_STAB(false);            //亮
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_2:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_RUN(true);            //灭掉
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_RUN(false);            //亮
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_3:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_SUM(true);            //灭掉
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_SUM(false);            //亮
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_4:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_OVER(true);            //灭掉
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_OVER(false);            //亮
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_5:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                VFD_FlgDisp_UNDER(true);            //灭掉
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                VFD_FlgDisp_UNDER(false);            //亮
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
        case MODELSET_SETUP_6:
            if(gu32_Sys10ms - gu32_FlashDelay < 40)
            {
                gu8_AuxDispArray[0] = SEG_TOPLINE;
            }
            else if(gu32_Sys10ms - gu32_FlashDelay > 80)
            {
                gu8_AuxDispArray[0] = SEG_BLK;
                gu32_FlashDelay = gu32_Sys10ms;
            }
            break;
    }
}

/*********************************************************************
函数名称: void ModelSet(void)
功    能: 型号设置
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-1-7
修    改:                      时    间: 
*********************************************************************/
void ModelSet(void)
{
    gb_ExitFlg = false;
    ulParameter.ul = 0xFFFFFFFF;
    gu32_KeyCode = 0;
    gu8_Item = 0;
    gu8_DataInLength = 0;

    while (gb_ExitFlg == false)
    {
        gu32_KeyCode = Key_Deal();                     //读按键
        OPBuzzer();
        Key_ModelSetState();               //恢复出厂设置操作
        VFD_ClrDispRam(gu32_ModelSetSwitch, gu32_ModelArry[0]);       //清显示缓存
        Disp_ModelSetState();          //恢复出厂设置显示处理
        if (gb_DispErrorFlg)        //显示error时不填充设置相关显示缓存
            Disp_Error();           //error报警显示处理
        VFD_CopyDispArryToDispRam();    //将显示数组存入显示缓存
        VFD_WriteDispRamArry();         //写显示缓存 
    }
}




