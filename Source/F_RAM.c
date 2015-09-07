/*************************************************
Project:                GM8804C-2_ARM
File name:              F_RAM.c
Description:            铁电存储器驱动相关函数
Author:                 丛海旭
Version:                1.0
Date:                   2013-12-5
Others:                 IC:FM25V02  256K bit
History:
*************************************************/
#include "include.h"

/*FM Status Register
Bit     7   6   5   4   3   2   1   0
Name  WPEN  0   0   0  BP1 BP0 WEL  0
*/
/* Ped=Protected:保护的;UnPed=Unprotected:未保护的
BP1 BP0     Ped Address Range
 0   0      None
 0   1      6000h to 7FFFh (upper 1/4)
 1   0      4000h to 7FFFh (upper 1/2)
 1   1      0000h to 7FFFh (all)
*/
/*  Ped=Protected:保护的;UnPed=Unprotected:未保护的
WEL|WPEN|/WP :  Ped_Blocks|UnPed_Blocks|Status_Register
0    X    X         Ped         Ped          Ped
1    0    X         Ped         UnPed        UnPed
1    1    0         Ped         UnPed        Ped
1    1    1         Ped         UnPed        UnPed
*/

/*
注意该程序设计的原理是平时存储数据只用FRAM的前1/2地址，后1/2地址是处于写保护状态
只有在调用void FM_WriteDW_Backup(u16 u16StarAddr,u32 u32WriteVal)这个函数时，
才将全部的写保护状态取消，将前1/2地址中的数据存入到后1/2地址中，并且打开写保护
chx 2013-12-5
*/

//引脚定义 注意，以下是相对FRAM来说的，CUP的DOUT即相当于FRAM的DIN
//读FRAM输出
#define FRAM_DOUT_READ      GPIO_BitRead(GPIO1,6)
//写FRAM输入端口
#define FRAM_DIN_CLR        GPIO_BitWrite(GPIO1,8,0)
#define FRAM_DIN_SET        GPIO_BitWrite(GPIO1,8,1)
//写FRAM时钟端口
#define FRAM_CLK_CLR        GPIO_BitWrite(GPIO1,7,0)
#define FRAM_CLK_SET        GPIO_BitWrite(GPIO1,7,1)
//写FRAM片选端口
#define FRAM_CS_CLR         GPIO_BitWrite(GPIO1,5,0)
#define FRAM_CS_SET         GPIO_BitWrite(GPIO1,5,1)
//寄存器值
#define     WREN            0x06        //写允许
#define     WRDI            0X04        //写禁止
#define     RDSR            0X05        //读状态寄存器
#define     WRSR            0X01        //写状态寄存器
#define     READ            0X03        //读数据序列
#define     WRITE           0X02        //写数据序列

#define FM_WPEN_MARK    B1000_0000
#define FM_BP1_MARK     B0000_1000
#define FM_BP0_MARK     B0000_0100
#define FM_WEL_MARK     B0000_0010
#define FM_BP_ALL   (FM_WPEN_MARK|FM_BP1_MARK|FM_BP0_MARK)
#define FM_BP_NON   B0000_0000
#define FM_BP_HALF  B0000_1000          //4000h to 7FFFh (upper 1/2)保护，用作参数备份

//FRAM的管脚初始化
void FRAM_Init(void)
{   
    GPIO_Config(GPIO1,(1<<6),GPIO_IN_TRI_TTL);  //输出 标准TTL输入，GPIO_IN_TRI_CMOS高阻模拟输入
    GPIO_Config(GPIO1,(1<<8),GPIO_OUT_PP);      //输入 推挽输出
    GPIO_Config(GPIO1,(1<<7),GPIO_OUT_PP);      //时钟 推挽输出
    GPIO_Config(GPIO1,(1<<5),GPIO_OUT_PP);      //片选 推挽输出
}

//向FRAM发送一个字节 包括CS
void FRAM_SendByte(u8 SendData)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        FRAM_CLK_CLR;
        (SendData & 0x80) ? FRAM_DIN_SET : FRAM_DIN_CLR;
        __nop();
        FRAM_CLK_SET;
        __nop();
        SendData <<= 1;
    }
}
//从FRAM读取一个字节
u8 FRAM_ReceiveByte(void)
{
    u8 i;
    u8 lu8_Data = 0;
    for (i = 0; i < 8; i++)
    {
        FRAM_CLK_SET;
        lu8_Data <<= 1;
        FRAM_CLK_CLR;
        (FRAM_DOUT_READ) ? (lu8_Data|= 0x01) : (lu8_Data|=0x00);
    }
    return lu8_Data;
}

//读状态寄存器
u8 FRAM_Read_SR(void)
{
    u8 status;
    FRAM_CS_CLR;
    FRAM_SendByte(RDSR);                    //读状态寄存器
    status = FRAM_ReceiveByte();
    FRAM_CS_SET;
    return(status);
}

//写状态寄存器
void FRAM_Write_SR(u8 u8newStatesRegVal)
{
    FRAM_CS_CLR;
    FRAM_SendByte(WRSR);
    FRAM_SendByte(u8newStatesRegVal);
    FRAM_CS_SET;
}

//全部地址写保护
void FRAM_BPALL(void)
{
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_Write_SR(FM_BP_ALL);
}

//全部地址不保护
void FRAM_BPNON(void)
{
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_Write_SR(FM_BP_NON);
}

//只保护4000h to 7FFFh 1/2地址
void FRAM_BPHALF(void)
{
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_Write_SR(FM_BP_HALF);
}

//从存储器某地址开始读出双字,小端模式,低字节在低地址.
u32 FRAM_ReadDW(u16 u16StarAddr)
{
    ULDATA sReadVal;
    sReadVal.ul = 0;
    FRAM_CS_CLR;
    FRAM_SendByte(READ);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    sReadVal.uc[0]=FRAM_ReceiveByte();  //RAM小端模式,低字节存低地址位
    sReadVal.uc[1]=FRAM_ReceiveByte();
    sReadVal.uc[2]=FRAM_ReceiveByte();
    sReadVal.uc[3]=FRAM_ReceiveByte();
    FRAM_CS_SET;;
    return sReadVal.ul;
}

//从存储器某地址开始读出双字float类型,小端模式,低字节在低地址.
float FRAM_ReadFloat(u16 u16StarAddr)
{
    FCDATA FCReadVal;
    FCReadVal.f = 0;
    FRAM_CS_CLR;
    FRAM_SendByte(READ);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    FCReadVal.uc[0]=FRAM_ReceiveByte();  //RAM小端模式,低字节存低地址位
    FCReadVal.uc[1]=FRAM_ReceiveByte();
    FCReadVal.uc[2]=FRAM_ReceiveByte();
    FCReadVal.uc[3]=FRAM_ReceiveByte();
    FRAM_CS_SET;;
    return FCReadVal.f;
}

//从存储器某地址开始读出8字节,小端模式,低字节在低地址.
u64 FRAM_Read8Byte(u16 u16StarAddr)  //64位数据读出
{
    ULLDATA sReadVal;
    sReadVal.ull=0;
    FRAM_CS_CLR;
    FRAM_SendByte(READ);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    sReadVal.uc[0]=FRAM_ReceiveByte();  //RAM小端模式,低字节存低地址位
    sReadVal.uc[1]=FRAM_ReceiveByte();
    sReadVal.uc[2]=FRAM_ReceiveByte();
    sReadVal.uc[3]=FRAM_ReceiveByte();
    sReadVal.uc[4]=FRAM_ReceiveByte();  //RAM小端模式,低字节存低地址位
    sReadVal.uc[5]=FRAM_ReceiveByte();
    sReadVal.uc[6]=FRAM_ReceiveByte();
    sReadVal.uc[7]=FRAM_ReceiveByte();
    FRAM_CS_SET;;
    return sReadVal.ull;
}

//从起始地址按字节连续读
void FRAM_ContReadByte(u16 u16StarAddr,u8* u8Arry,u16 u16ByteNum)
{
    u16 i;

    FRAM_CS_CLR;
    FRAM_SendByte(READ);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    for(i=0;i<u16ByteNum;i++){
        u8Arry[i]=FRAM_ReceiveByte();
    }
    FRAM_CS_SET;;
}

//从起始地址按双字连续读
void FRAM_ContReadDW(u16 u16StarAddr,u32* u32Arry,u16 u16DWNum)
{
    u16 i;
    ULDATA sReadVal;
    FRAM_CS_CLR;
    FRAM_SendByte(READ);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    for(i=0;i<u16DWNum;i++){
        sReadVal.uc[0]=FRAM_ReceiveByte();
        sReadVal.uc[1]=FRAM_ReceiveByte();
        sReadVal.uc[2]=FRAM_ReceiveByte();
        sReadVal.uc[3]=FRAM_ReceiveByte();
        u32Arry[i]=sReadVal.ul;
    }
    FRAM_CS_SET;;
}

//向储存器某起始地址开始写入双字,小端模式低字节在低地址.
void FRAM_WriteDW(u16 u16StarAddr,u32 u32WriteVal)
{
    u16 i;
    ULDATA sWriteVal;
    sWriteVal.ul=u32WriteVal;
    FRAM_BPHALF();            //注意只能写入前1/2地址，后1/2保护状态用作参数备份，只有在备份操作时才打开保护
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_CS_CLR;
    FRAM_SendByte(WRITE);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    for(i=0; i<4; i++){
        FRAM_SendByte(sWriteVal.uc[i]);
    }
    FRAM_CS_SET;
    FRAM_BPALL();
}

//向储存器某起始地址开始写入双字float类型,小端模式低字节在低地址.
void FRAM_WriteFloat(u16 u16StarAddr,float fWriteVal)
{
    u16 i;
    FCDATA FCWriteVal;
    FCWriteVal.f=fWriteVal;
    FRAM_BPHALF();            //注意只能写入前1/2地址，后1/2保护状态用作参数备份，只有在备份操作时才打开保护
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_CS_CLR;
    FRAM_SendByte(WRITE);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    for(i=0; i<4; i++){
        FRAM_SendByte(FCWriteVal.uc[i]);
    }
    FRAM_CS_SET;
    FRAM_BPALL();
}

//向储存器某起始地址开始写入8字节,小端模式低字节在低地址.
void FRAM_Write8Byte(u16 u16StarAddr,u64 u64WriteVal)
{
    u16 i;

    ULLDATA sWriteVal;
    sWriteVal.ull=u64WriteVal;
    FRAM_BPHALF();            //注意只能写入前1/2地址，后1/2保护状态用作参数备份，只有在备份操作时才打开保护
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_CS_CLR;
    FRAM_SendByte(WRITE);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    for(i=0; i<8; i++){
        FRAM_SendByte(sWriteVal.uc[i]);
    }
    FRAM_CS_SET;
    FRAM_BPALL();
}

//备份数据时使用，打开后1/2地址的写入权限
void FRAM_WriteDW_Backup(u16 u16StarAddr,u32 u32WriteVal)
{
    u16 i;

    ULDATA sWriteVal;
    sWriteVal.ul=u32WriteVal;

    FRAM_BPNON();            //取消全部保护，用来向后1/2地址中写入参数
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_CS_CLR;
    FRAM_SendByte(WRITE);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    for(i=0; i<4; i++){
        FRAM_SendByte(sWriteVal.uc[i]);
    }
    FRAM_CS_SET;
    FRAM_BPALL();
}

//从起始地址按字节连续写
void FRAM_ContWriteByte(u16 u16StarAddr,u8* u8Arry,u16 u16ByteNum)
{
    u16 i;
    
    FRAM_BPHALF();            //注意只能写入前1/2地址，后1/2保护状态用作参数备份，只有在备份操作时才打开保护
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_CS_CLR;
    FRAM_SendByte(WRITE);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    for(i=0;i<u16ByteNum;i++){
        FRAM_SendByte(u8Arry[i]);
    }
    FRAM_CS_SET;
    FRAM_BPALL();
}

//从起始地址按双字连续写,写入数据存在于长整型数组
void FRAM_ContWriteDW(u16 u16StarAddr,u32* u32Arry,u16 u16DWNum)
{
    u16 i;
    ULDATA sWriteVal;
    FRAM_BPHALF();            //注意只能写入前1/2地址，后1/2保护状态用作参数备份，只有在备份操作时才打开保护
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //写使能,在任何写操作前都必须执行写使能
    FRAM_CS_SET;
    FRAM_CS_CLR;
    FRAM_SendByte(WRITE);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    for(i=0;i<u16DWNum;i++){
        sWriteVal.ul=u32Arry[i];
        FRAM_SendByte(sWriteVal.uc[0]);
        FRAM_SendByte(sWriteVal.uc[1]);
        FRAM_SendByte(sWriteVal.uc[2]);
        FRAM_SendByte(sWriteVal.uc[3]);
    }

    FRAM_CS_SET;;
    FRAM_BPALL();
}








