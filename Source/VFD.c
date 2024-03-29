/*************************************************
Project:                GM8804C-2_ARM
File name:              VFD.c
Description:            VFD显示驱动相关
Author:                 丛海旭
Version:                1.0
Date:                   2013-12-5
Others:                 IC:16312
History:
*************************************************/
#include "include.h"
//下面的输入输出是以16312芯片来说
//读16312输出
#define VFD_DOUT_READ   GPIO_BitRead(GPIO1,9)            //数据
//写16312输入端口
#define VFD_DIN_CLR     GPIO_BitWrite(GPIO1,10,0)
#define VFD_DIN_SET     GPIO_BitWrite(GPIO1,10,1)
//写16312时钟端口
#define VFD_CLK_CLR     GPIO_BitWrite(GPIO1,13,0)        //时钟
#define VFD_CLK_SET     GPIO_BitWrite(GPIO1,13,1)
//写16312片选端口
#define VFD_CS_CLR      GPIO_BitWrite(GPIO1,14,0)        //片选
#define VFD_CS_SET      GPIO_BitWrite(GPIO1,14,1)

/* 
显示方式设置指令
MSB                    LSB
0  0  -  -  -  b2  b1  b0
b2/b1/b0 选择显示方式
0  0  0     4位，16段
0  0  1     5位，16段
0  1  0     6位，16段
0  1  1     7位，15段
1  0  0     8位，14段
1  0  1     9位，13段
1  1  0     10位，12段
1  1  1     11位，11段

数据设定指令
MSB                    LSB
0  1  -  -  b3  b2  b1  b0
b1/b0 设定数据读写方式
0  0    写数据至显示存储器
0  1    写数据至LED端
1  0    读键数据
1  1    读SW数据
b2 设定地址增量方式（显示存贮器）
0   数据写入后的增量地址
1   固定地址
b3 设定测试模式
0   正常操作
1   测试模式

地址设定指令
MSB                     LSB
1  1  -  b4  b3  b2  b1  b0
b4/b3/b2/b1/b0      地址（00H~15H）

显示控制指令
MSB                     LSB
1  0  -  -  b3  b2  b1  b0
b2/b1/b0 调光等级设定
0  0  0     设定脉冲宽度为1/16
0  0  1     设定脉冲宽度为2/16
0  1  0     设定脉冲宽度为4/16
0  1  1     设定脉冲宽度为10/16
1  0  0     设定脉冲宽度为11/16
1  0  1     设定脉冲宽度为12/16
1  1  0     设定脉冲宽度为13/16
1  1  1     设定脉冲宽度为14/16
b3 通/断显示
0   显示断（键扫描继续）
1   显示通

LED端口
MSB                     LSB
-  -  -  -  b3  b2  b1  b0
b3 LED4 TARE
b2 LED3 GROSS
b1 LED2 PACK
b0 LED1 NET
为0时LED亮


输入显示数据
命令1：设定显示方式
命令2：设定数据
命令3：设定地址
数据1~n：传输显示数据（最大22字节）
命令4：控制显示

输入指定地址
命令1：设定数据
命令2：设定地址
数据：显示数据

*/


#define TEC16312_DISPMODE       B0000_0111          //显示方式 11位11段
#define TEC16312_DATA_DISP      B0100_0000          //数据设定命令 写显示
#define TEC16312_DATA_LED       B0100_0101          //数据设定命令 写LED
#define TEC16312_DATA_RKEY      B0100_0110          //数据设定命令 读按键
#define TEC16312_ADDRESS        B1100_0000          //地址设定指令，从00H开始
#define TEC16312_BRIGHT         B1000_1111          //显示控制指令 调光等级最高，显示通



 const u8 Value_To_SEG[]
      ={
        //0~9
        SEG_0,SEG_1,SEG_2,SEG_3,SEG_4,SEG_5,SEG_6,SEG_7,SEG_8,SEG_9,
        //A~G
        SEG_A,SEG_B,SEG_C,SEG_D,SEG_E,SEG_F,SEG_G,
        //H~N
        SEG_H,SEG_I,SEG_J,SEG_K,SEG_L,SEG_M,SEG_N,
        //O~T
        SEG_O,SEG_P,SEG_Q,SEG_R,SEG_S,SEG_T,
        //U~Z
        SEG_U,SEG_V,SEG_W,SEG_X,SEG_Y,SEG_Z,
        //空白
        SEG_BLK
       };
u8 gu8_VFDDispRamArray[22];          //16312的显示RAM数组，11段，每段2字节
u8 gu8_VFDLedRam;       //16312的LED灯RAM缓存
u8 gu8_MainDispArray[6];     //主显示数组
u8 gu8_AuxDispArray[3];      //副显示数组
u8 gu8_UnitDispArray[2];     //单位显示数组
u8 gu8_FlgDispArray[6];      //标志灯显示数组



//ASCII码对应的段码表，一些奇怪的符号不能显示，主要显示字母数字和-等简单符号
const u8 ASCII_To_SEG[]=
 {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    //0x00~0x0F
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    //0x10~0x1F
//                   #                                                     -
    0x00,0x00,0x00,SEG_3LINE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,SEG_1LINE,0x00,0x00,    //0x20~0x2F
//   0     1     2     3     4     5     6     7     8     9                     =     
    SEG_0,SEG_1,SEG_2,SEG_3,SEG_4,SEG_5,SEG_6,SEG_7,SEG_8,SEG_9,0x00,0x00,0x00,SEG_2LINE,0x00,0x00,    //0x30~0x3F
//        A     B     C     D     E     F     G     H     I     J     K     L     M     N     O
    0x00,SEG_A,SEG_B,SEG_C,SEG_D,SEG_E,SEG_F,SEG_G,SEG_H,SEG_I,SEG_J,SEG_K,SEG_L,SEG_M,SEG_N,SEG_O,    //0x40~0x4F
//   P     Q     R     S     T     U     V     W     X     Y     Z                         _
    SEG_P,SEG_Q,SEG_R,SEG_S,SEG_T,SEG_U,SEG_V,SEG_W,SEG_X,SEG_Y,SEG_Z,0x00,0x00,0x00,0x00,0x02,    //0x50~0x5F
//        a     b     c     d     e     f     g     h     i     j     k     l     m     n     o
    0x00,SEG_A,SEG_B,SEG_C,SEG_D,SEG_E,SEG_F,SEG_G,SEG_H,SEG_I,SEG_J,SEG_K,SEG_L,SEG_M,SEG_N,SEG_O,    //0x60~0x6F
//    p    q     r     s     t     u     v     w     x     y     z
    SEG_P,SEG_Q,SEG_R,SEG_S,SEG_T,SEG_U,SEG_V,SEG_W,SEG_X,SEG_Y,SEG_Z,0x00,0x00,0x00,0x00,0x00,    //0x70~0x7F
 };

/*********************************************************************
函数名称: void VFD_SendByte(u8 outdata)
功    能: 写入16312一字节数据
说    明: 底层驱动
入口参数: outdata = 发送的数据
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-5
修    改:                      时    间: 
*********************************************************************/
void VFD_SendByte(u8 outdata)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        VFD_CLK_CLR;
        __nop();__nop();__nop();
        (outdata & 0x01) ? VFD_DIN_SET : VFD_DIN_CLR;
        __nop();__nop();__nop();
        VFD_CLK_SET;
        __nop();__nop();__nop();
        outdata >>= 1;
    }
}

/*********************************************************************
函数名称: u8 VFD_ReceiveByte(void)
功    能: 从16312读出一字节数据
说    明: 底层驱动
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-5
修    改:                      时    间: 
*********************************************************************/
u8 VFD_ReceiveByte(void)
{
    u8 i;
    u8 indata = 0;
    for (i = 0; i < 8; i++)
    {
        VFD_CLK_SET;
        __nop();__nop();__nop();
        indata >>= 1;
        __nop();__nop();__nop();

        VFD_CLK_CLR;
        __nop();__nop();__nop();
        __nop();__nop();__nop();

        if(VFD_DOUT_READ == true)
            indata |= 0x80;
        else
            __nop();__nop();__nop();
    }
    return indata;
}

/*********************************************************************
函数名称: void VFD_WriteByte(u8 fu8_Data)
功    能: VFD写入一字节数据，带片选CS逻辑
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
void VFD_WriteByte(u8 fu8_Data)
{
    VFD_CS_CLR;
    VFD_SendByte(fu8_Data);
    VFD_CS_SET;
}

/*********************************************************************
函数名称: void VFD_Init(void)
功    能: VFD显示的初始化
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-6
修    改:                      时    间: 
*********************************************************************/
void VFD_Init(void)
{
    //输入 标准TTL输入，GPIO_IN_TRI_CMOS高阻模拟输入
    GPIO_Config(GPIO1,(1<<9),GPIO_IN_TRI_TTL);
    //输出 推挽输出
    GPIO_Config(GPIO1,(1<<10),GPIO_OUT_PP);
    //时钟 推挽输出GPIO_OUT_PP
    GPIO_Config(GPIO1,(1<<13),GPIO_OUT_PP);
    //片选 推挽输出
    GPIO_Config(GPIO1,(1<<14),GPIO_OUT_PP);

    VFD_WriteByte(TEC16312_DISPMODE);  //11位11段
    VFD_WriteByte(TEC16312_BRIGHT);    //最高亮度
}

/*********************************************************************
函数名称: void VFD_MainDisp_u32(u32 fu32_Data)
功    能: 将要显示的整数数据转为SEG段码存入主显示数组
说    明: 最多6位
入口参数: fu32_Data=显示数据
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
void VFD_MainDisp_u32(u32 fu32_Data)
{
    u8 i;
    fu32_Data %= 1000000;    //不允许大于6位数据的转换
    BIN4toNBCD(fu32_Data, gu8_MainDispArray, 6);
    for (i = 0; i < 6; i++)         //将数值转换成显示译码
        gu8_MainDispArray[i] = Value_To_SEG[gu8_MainDispArray[i]];
}

/*********************************************************************
函数名称: void VFD_MainDisp_Str(char* fu8p_Arry)
功    能: 将要显示的字符串数据转为SEG段码存入主显示数组
说    明: 最多6位
入口参数: fu32_Data=显示数据
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
void VFD_MainDisp_Str(char* fu8p_Arry)
{
    u8 i;
    
    for (i = 0; (i < 6) && ((*fu8p_Arry) != '\0'); i++, fu8p_Arry++)
        gu8_MainDispArray[i] = ASCII_To_SEG[*fu8p_Arry];
}

/*********************************************************************
函数名称: void VFD_MainDisp_ASCII(u8* fu8_Arry)
功    能: 将要显示的ASCII码数组数据转为SEG段码存入主显示数组
说    明: 最多6位
入口参数: fu8_Arry=显示数据数组
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-23
修    改:                      时    间: 
*********************************************************************/
void VFD_MainDisp_ASCII(u8* fu8_Arry)
{
    u8 i;
    for (i = 0; i <= 5; i++)
        fu8_Arry[i] = ASCII_To_SEG[fu8_Arry[i]];
}
/*********************************************************************
函数名称: void VFD_AuxDisp_u32(u32 fu32_Data)
功    能: 将要显示的整数数据转为SEG段码存入副显示数组
说    明: 最多3位
入口参数: fu32_Data=显示数据
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-11
修    改:                      时    间: 
*********************************************************************/
void VFD_AuxDisp_u32(u32 fu32_Data)
{
    u8 i;
    fu32_Data %= 1000;    //不允许大于3位数据的转换
    BIN4toNBCD(fu32_Data, gu8_AuxDispArray, 3);
    for (i = 0; i < 3; i++)         //将数值转换成显示译码
        gu8_AuxDispArray[i] = Value_To_SEG[gu8_AuxDispArray[i]];
}

/*********************************************************************
函数名称: void VFD_AuxDisp_Str(char* fu8p_Arry)
功    能: 将要显示的字符串数据转为SEG段码存入副显示数组
说    明: 最多3位
入口参数: fu32_Data=显示数据
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-11
修    改:                      时    间: 
*********************************************************************/
void VFD_AuxDisp_Str(char* fu8p_Arry)
{
    u8 i;
    
    for (i = 0; (i < 3) && ((*fu8p_Arry) != '\0'); i++, fu8p_Arry++)
        gu8_AuxDispArray[i] = ASCII_To_SEG[*fu8p_Arry];
}

/*********************************************************************
函数名称: void VFD_UnitDisp(u32 fu32_Data)
功    能: 单位显示数据存入单位数组
说    明: 
入口参数: fu32_Data=显示数据
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-11
修    改:                      时    间: 
*********************************************************************/
void VFD_UnitDisp(u32 fu32_Data)
{
    switch (fu32_Data)
    {
        case UNIT_g:
            gu8_UnitDispArray[0] = 0x80;
            gu8_UnitDispArray[1] = 0x02;
            break;
        case UNIT_kg:
            gu8_UnitDispArray[0] = 0xC4;
            gu8_UnitDispArray[1] = 0x02;
            break;
        case UNIT_t:
            gu8_UnitDispArray[0] = 0x58;
            gu8_UnitDispArray[1] = 0x00;
            break;
        case UNIT_lb:
            gu8_UnitDispArray[0] = 0xC2;
            gu8_UnitDispArray[1] = 0x00;
            break;
        case UNIT_N:
            gu8_UnitDispArray[0] = 0x20;
            gu8_UnitDispArray[1] = 0x00;
            break;
        case UNIT_PCS:
            gu8_UnitDispArray[0] = 0x01;
            gu8_UnitDispArray[1] = 0x00;
            break;
        default:
            gu8_UnitDispArray[0] = 0x00;
            gu8_UnitDispArray[1] = 0x00;
            break;
    }
}

/*********************************************************************
函数名称: void VFD_LEDDisp(bool fb_Gross, bool fb_Net, bool fb_Tare, bool fb_Pack)
功    能: LED灯显示
说    明: 
入口参数: fb_Gross fb_Net fb_Tare fb_Pack 对应4个LED状态
返 回 值: 
设    计: 丛海旭               时    间: 2014-1-7
修    改:                      时    间: 
*********************************************************************/
void VFD_LEDDisp(bool fb_Gross, bool fb_Net, bool fb_Tare, bool fb_Pack)
{
    gu8_VFDLedRam = 0;
    
    if (fb_Tare)
        gu8_VFDLedRam |= 0x01;
    if (fb_Gross)
        gu8_VFDLedRam |= 0x02;
    if (fb_Pack)
        gu8_VFDLedRam |= 0x04;
    if (fb_Net)
        gu8_VFDLedRam |= 0x08;
    gu8_VFDLedRam = (~gu8_VFDLedRam);
}

/*********************************************************************
函数名称: void VFD_FlgDisp(u32 fu32_Data)
功    能: 状态位显示
说    明: 
入口参数: fu32_Data
fu32_Data的每一位状态代表一个指示灯
^17:ZERO    ^16:STAB    ^15:RUN     ^14:SUM     ^13:OVER    ^12:UNDER
^11:SP1     ^10:SP2     ^9:SP3      ^8:DISC     ^7:NZ       ^6:FILL
^5:HILIM    ^4:HI       ^3:GO       ^2:LO       ^1:LOLIM    ^0:HOLD
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-16
修    改:                      时    间: 
*********************************************************************/
void VFD_FlgDisp(u32 fu32_Data)
{
//----------------------------------------------//
    if (fu32_Data & 0x00020000)     //ZERO
        gu8_FlgDispArray[0] |= 0x02;
    if (fu32_Data & 0x00000800)     //SP1
        gu8_FlgDispArray[0] |= 0x01;
    if (fu32_Data & 0x00000020)     //HILIM
        gu8_FlgDispArray[0] |= 0x04;
//----------------------------------------------//
    if(fu32_Data & 0x00010000)      //STAB
        gu8_FlgDispArray[1] |= 0x02;
    if(fu32_Data & 0x00000400)      //SP2
        gu8_FlgDispArray[1] |= 0x01;
    if(fu32_Data & 0x00000010)      //HI
        gu8_FlgDispArray[1] |= 0x04;
//---------------------------------------------//
    if(fu32_Data & 0x00008000)      //RUN
        gu8_FlgDispArray[2] |= 0x02;
    if(fu32_Data & 0x00000200)      //SP3
        gu8_FlgDispArray[2] |= 0x01;
    if(fu32_Data & 0x00000008)      //GO
        gu8_FlgDispArray[2] |= 0x04;
//---------------------------------------------//
    if(fu32_Data & 0x00004000)      //SUM
        gu8_FlgDispArray[3] |= 0x02;
    if(fu32_Data & 0x00000100)      //DISC
        gu8_FlgDispArray[3] |= 0x01;
    if(fu32_Data & 0x00000004)      //LO
        gu8_FlgDispArray[3] |= 0x04;
//---------------------------------------------//
    if(fu32_Data & 0x00002000)      //OVER
        gu8_FlgDispArray[4] |= 0x02;
    if(fu32_Data & 0x00000080)      //NZ
        gu8_FlgDispArray[4] |= 0x01;
    if(fu32_Data & 0x00000002)      //LOLIM
        gu8_FlgDispArray[4] |= 0x04;
//---------------------------------------------//
    if(fu32_Data & 0x00001000)      //UNDER
        gu8_FlgDispArray[5] |= 0x02;
    if(fu32_Data & 0x00000040)      //FILL
        gu8_FlgDispArray[5] |= 0x01;
    if(fu32_Data & 0x00000001)      //HOLD
        gu8_FlgDispArray[5] |= 0x04;
//---------------------------------------------//
}

void VFD_FlgDisp_ZERO(bool fb_On)
{
    gu8_FlgDispArray[0] |= (fb_On ? 0x02 : 0);
}
void VFD_FlgDisp_SP1(bool fb_On)
{
    gu8_FlgDispArray[0] |= (fb_On ? 0x01 : 0);
}
void VFD_FlgDisp_HILIM(bool fb_On)
{
    gu8_FlgDispArray[0] |= (fb_On ? 0x04 : 0);
}
void VFD_FlgDisp_STAB(bool fb_On)
{
    gu8_FlgDispArray[1] |= (fb_On ? 0x02 : 0);
}
void VFD_FlgDisp_SP2(bool fb_On)
{
    gu8_FlgDispArray[1] |= (fb_On ? 0x01 : 0);
}
void VFD_FlgDisp_HI(bool fb_On)
{
    gu8_FlgDispArray[1] |= (fb_On ? 0x04 : 0);
}
void VFD_FlgDisp_RUN(bool fb_On)
{
    gu8_FlgDispArray[2] |= (fb_On ? 0x02 : 0);
}
void VFD_FlgDisp_SP3(bool fb_On)
{
    gu8_FlgDispArray[2] |= (fb_On ? 0x01 : 0);
}
void VFD_FlgDisp_GO(bool fb_On)
{
    gu8_FlgDispArray[2] |= (fb_On ? 0x04 : 0);
}
void VFD_FlgDisp_SUM(bool fb_On)
{
    gu8_FlgDispArray[3] |= (fb_On ? 0x02 : 0);
}
void VFD_FlgDisp_DISC(bool fb_On)
{
    gu8_FlgDispArray[3] |= (fb_On ? 0x01 : 0);
}
void VFD_FlgDisp_LO(bool fb_On)
{
    gu8_FlgDispArray[3] |= (fb_On ? 0x04 : 0);
}
void VFD_FlgDisp_OVER(bool fb_On)
{
    gu8_FlgDispArray[4] |= (fb_On ? 0x02 : 0);
}
void VFD_FlgDisp_NZ(bool fb_On)
{
    gu8_FlgDispArray[4] |= (fb_On ? 0x01 : 0);
}
void VFD_FlgDisp_LOLIM(bool fb_On)
{
    gu8_FlgDispArray[4] |= (fb_On ? 0x04 : 0);
}
void VFD_FlgDisp_UNDER(bool fb_On)
{
    gu8_FlgDispArray[5] |= (fb_On ? 0x02 : 0);
}
void VFD_FlgDisp_FILL(bool fb_On)
{
    gu8_FlgDispArray[5] |= (fb_On ? 0x01 : 0);
}
void VFD_FlgDisp_HOLD(bool fb_On)
{
    gu8_FlgDispArray[5] |= (fb_On ? 0x04 : 0);
}


/*********************************************************************
函数名称: void VFD_CopyMainDispArryToDispRam(void)
功    能: 将显示数组存入VFD显示RAM数组
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
void VFD_CopyMainDispArryToDispRam(void)
{
    u8 i,j;
    //将主显示缓存复制到显示缓存数组 主显示缓存在显示RAM中的起始地址为0x02
    for (i = 0,j = 2; i < 6; i++,j += 2)
        gu8_VFDDispRamArray[j] = gu8_MainDispArray[i];
}

/*********************************************************************
函数名称: void VFD_CopyAuxDispArryToDispRam(void)
功    能: 将显示数组存入VFD显示RAM数组
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
void VFD_CopyAuxDispArryToDispRam(void)
{
    u8 i,j;
    //将副显示缓存复制到显示缓存数组 副显示缓存在显示RAM中的起始地址为0x10
    for (i = 0,j = 0x10; i < 3; i++,j += 2)
        gu8_VFDDispRamArray[j] = gu8_AuxDispArray[i];
}

/*********************************************************************
函数名称: void VFD_CopyUnitDispArryToDispRam(void)
功    能: 将显示数组存入VFD显示RAM数组
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-13
修    改:                      时    间: 
*********************************************************************/
void VFD_CopyUnitDispArryToDispRam(void)
{
    u8 i,j;
    //将单位显示缓存复制到显示缓存数组 单位显示缓存在显示RAM中的起始地址为0x0E
    for (i = 0,j = 0x0E; i < 2; i++,j ++)
        gu8_VFDDispRamArray[j] = gu8_UnitDispArray[i];
}

/*********************************************************************
函数名称: void VFD_CopyFlgDispArryToDispRam(void)
功    能: 将显示数组存入VFD显示RAM数组
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-13
修    改:                      时    间: 
*********************************************************************/
void VFD_CopyFlgDispArryToDispRam(void)
{
    u8 i,j;
    //将标志灯显示缓存复制到显示缓存数组 单位显示缓存在显示RAM中的起始地址为0x05
    for (i = 0,j = 0x01; i < 6; i++,j += 2)
        gu8_VFDDispRamArray[j] = gu8_FlgDispArray[i];
}

/*********************************************************************
函数名称: void VFD_CopyDispArryToDispRam(void)
功    能: 将显示数组存入VFD显示RAM数组
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-13
修    改:                      时    间: 
*********************************************************************/
void VFD_CopyDispArryToDispRam(void)
{
    VFD_CopyMainDispArryToDispRam();    //主显示缓存
    VFD_CopyAuxDispArryToDispRam();     //副显示缓存
    VFD_CopyUnitDispArryToDispRam();    //单位缓存
    VFD_CopyFlgDispArryToDispRam();     //标志灯缓存
}

/*********************************************************************
函数名称: void VFD_WriteDispRamArry(void)
功    能: 将显示缓存数组写入VFD，包括LED等数据等
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
void VFD_WriteDispRamArry(void)
{
    u8 i;
    
    VFD_WriteByte(TEC16312_DISPMODE);   //11位11段
    VFD_WriteByte(TEC16312_DATA_DISP);  //数据设置命令字:正常工作,地址加1方式,写显示数据到显示RAM
    VFD_CS_CLR;
    VFD_SendByte(TEC16312_ADDRESS);             //地址设置命令字:从地址0开始
    for (i = 0; i < sizeof(gu8_VFDDispRamArray) / sizeof(gu8_VFDDispRamArray[0]); i++)
    {
        VFD_SendByte(gu8_VFDDispRamArray[i]);        //显示数据
    }
    VFD_CS_SET;
    VFD_WriteByte(TEC16312_BRIGHT);    //最高亮度
    
    VFD_CS_CLR;
    VFD_SendByte(TEC16312_DATA_LED);     //led灯
    VFD_SendByte(gu8_VFDLedRam);        //led
    VFD_CS_SET;
}



/*********************************************************************
函数名称: void VFD_FillDispRam(u32 fu32_ModelSet, u32 fu32_Logo)
功    能: 显示全亮
说    明: 
入口参数: fu32_ModelSet=型号自定义开关；fu32_Logo=LOGO开关
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
void VFD_FillDispRam(u32 fu32_ModelSet, u32 fu32_Logo)
{
    u8 i;
    //显示缓存全亮
    memset(gu8_VFDDispRamArray, 0xFF, sizeof(gu8_VFDDispRamArray));
    gu8_VFDLedRam = 0;          //LED全亮
    
    if (fu32_ModelSet != 0 && fu32_Logo == 0) //型号自定义打开并且电源灯是灭的
        gu8_VFDDispRamArray[0] = 0;            //灭掉电源灯
        
    for (i = 0; i < 6; i++)
    {
        gu8_MainDispArray[i] = 0xFF;        //填充主显示数组
        gu8_FlgDispArray[i] = 0xFF;         //填充标志位数组
        if (i < 3)
            gu8_AuxDispArray[i] = 0xFF;     //填充副显示数组
        if (i < 2)
            gu8_UnitDispArray[i] = 0xFF;    //填充单位显示数组
    }
}

/*********************************************************************
函数名称: void VFD_ClrDispRam(u32 fu32_ModelSet, u32 fu32_Logo)
功    能: 清VFD显示缓存，包括主显示、副显示的缓存数组
说    明: 
入口参数: fu32_ModelSet=型号自定义开关；fu32_Logo=LOGO开关
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
void VFD_ClrDispRam(u32 fu32_ModelSet, u32 fu32_Logo)
{
    u8 i;
    //显示缓存全灭
    memset(gu8_VFDDispRamArray, 0, sizeof(gu8_VFDDispRamArray));
    gu8_VFDLedRam = 0xFF;          //LED全灭
    //型号自定义关闭或者型号自定义打开并且电源灯显示打开
    if (fu32_ModelSet == 0 || (fu32_ModelSet != 0 && fu32_Logo != 0))
        gu8_VFDDispRamArray[0] = 0xFF;            //灭掉电源灯
    
    for (i = 0; i < 6; i++)
    {
        gu8_MainDispArray[i] = 0;        //清主显示数组
        gu8_FlgDispArray[i] = 0;         //清标志位数组
        if (i < 3)
            gu8_AuxDispArray[i] = 0;     //清副显示数组
        if (i < 2)
            gu8_UnitDispArray[i] = 0;    //清单位显示数组
    }
}

/*********************************************************************
函数名称: u32 VFD_ReadKey(void)
功    能: 读键值
说    明: 
入口参数: 
返 回 值: 按键值
设    计: 丛海旭               时    间: 2013-12-9
修    改:                      时    间: 
*********************************************************************/
u32 VFD_ReadKey(void)
{
    UIDATA uiKey;
    
    VFD_CS_CLR;
    VFD_SendByte(TEC16312_DATA_RKEY);
    __nop();__nop();__nop();__nop();__nop();__nop();;
    uiKey.uc[1]=VFD_ReceiveByte();
    uiKey.uc[0]=VFD_ReceiveByte();
    VFD_CS_SET;
    return (u32)uiKey.ui;
}

/*********************************************************************
函数名称: void VFD_MainSigDisp(bool fb_SigFlg)
功    能: 小数点显示
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-13
修    改:                      时    间: 
*********************************************************************/
void VFD_MainSigDisp(bool fb_SigFlg)
{
    u8 i;
    
    if (fb_SigFlg)
    {
        for (i = 0; i < 6; i++)
        {
            if (gu8_MainDispArray[i] != SEG_BLK && i != 0)
            {
                gu8_MainDispArray[i - 1] = SEG_SIG;
                break;
            }
        }
    }
}

/*********************************************************************
函数名称: u8 VFD_KeyCode_To_Value(u32 ful_KeyCode)
功    能: 按键译码转换成数值
说    明: 
入口参数: ful_KeyCode = 按键译码
返 回 值: 按键对应的数字
设    计: 丛海旭               时    间: 2011-3-31
修    改:                      时    间: 
*********************************************************************/
u8 VFD_KeyCode_To_Value(u32 ful_KeyCode)
{
    u8 luc_Value = 0;
    
    switch (ful_KeyCode)
    {
        case K_1:
            luc_Value = 1;break;
        case K_2:
            luc_Value = 2;break;
        case K_3:
            luc_Value = 3;break;
        case K_4:
            luc_Value = 4;break;
        case K_5:
            luc_Value = 5;break;
        case K_6:
            luc_Value = 6;break;
        case K_7:
            luc_Value = 7;break;
        case K_8:
            luc_Value = 8;break;
        case K_9:
            luc_Value = 9;break;
        case K_0:
        default:
            luc_Value = 0;break;
    }
    return luc_Value;
}



