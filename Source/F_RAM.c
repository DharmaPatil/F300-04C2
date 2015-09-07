/*************************************************
Project:                GM8804C-2_ARM
File name:              F_RAM.c
Description:            ����洢��������غ���
Author:                 �Ժ���
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
/* Ped=Protected:������;UnPed=Unprotected:δ������
BP1 BP0     Ped Address Range
 0   0      None
 0   1      6000h to 7FFFh (upper 1/4)
 1   0      4000h to 7FFFh (upper 1/2)
 1   1      0000h to 7FFFh (all)
*/
/*  Ped=Protected:������;UnPed=Unprotected:δ������
WEL|WPEN|/WP :  Ped_Blocks|UnPed_Blocks|Status_Register
0    X    X         Ped         Ped          Ped
1    0    X         Ped         UnPed        UnPed
1    1    0         Ped         UnPed        Ped
1    1    1         Ped         UnPed        UnPed
*/

/*
ע��ó�����Ƶ�ԭ����ƽʱ�洢����ֻ��FRAM��ǰ1/2��ַ����1/2��ַ�Ǵ���д����״̬
ֻ���ڵ���void FM_WriteDW_Backup(u16 u16StarAddr,u32 u32WriteVal)�������ʱ��
�Ž�ȫ����д����״̬ȡ������ǰ1/2��ַ�е����ݴ��뵽��1/2��ַ�У����Ҵ�д����
chx 2013-12-5
*/

//���Ŷ��� ע�⣬���������FRAM��˵�ģ�CUP��DOUT���൱��FRAM��DIN
//��FRAM���
#define FRAM_DOUT_READ      GPIO_BitRead(GPIO1,6)
//дFRAM����˿�
#define FRAM_DIN_CLR        GPIO_BitWrite(GPIO1,8,0)
#define FRAM_DIN_SET        GPIO_BitWrite(GPIO1,8,1)
//дFRAMʱ�Ӷ˿�
#define FRAM_CLK_CLR        GPIO_BitWrite(GPIO1,7,0)
#define FRAM_CLK_SET        GPIO_BitWrite(GPIO1,7,1)
//дFRAMƬѡ�˿�
#define FRAM_CS_CLR         GPIO_BitWrite(GPIO1,5,0)
#define FRAM_CS_SET         GPIO_BitWrite(GPIO1,5,1)
//�Ĵ���ֵ
#define     WREN            0x06        //д����
#define     WRDI            0X04        //д��ֹ
#define     RDSR            0X05        //��״̬�Ĵ���
#define     WRSR            0X01        //д״̬�Ĵ���
#define     READ            0X03        //����������
#define     WRITE           0X02        //д��������

#define FM_WPEN_MARK    B1000_0000
#define FM_BP1_MARK     B0000_1000
#define FM_BP0_MARK     B0000_0100
#define FM_WEL_MARK     B0000_0010
#define FM_BP_ALL   (FM_WPEN_MARK|FM_BP1_MARK|FM_BP0_MARK)
#define FM_BP_NON   B0000_0000
#define FM_BP_HALF  B0000_1000          //4000h to 7FFFh (upper 1/2)������������������

//FRAM�Ĺܽų�ʼ��
void FRAM_Init(void)
{   
    GPIO_Config(GPIO1,(1<<6),GPIO_IN_TRI_TTL);  //��� ��׼TTL���룬GPIO_IN_TRI_CMOS����ģ������
    GPIO_Config(GPIO1,(1<<8),GPIO_OUT_PP);      //���� �������
    GPIO_Config(GPIO1,(1<<7),GPIO_OUT_PP);      //ʱ�� �������
    GPIO_Config(GPIO1,(1<<5),GPIO_OUT_PP);      //Ƭѡ �������
}

//��FRAM����һ���ֽ� ����CS
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
//��FRAM��ȡһ���ֽ�
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

//��״̬�Ĵ���
u8 FRAM_Read_SR(void)
{
    u8 status;
    FRAM_CS_CLR;
    FRAM_SendByte(RDSR);                    //��״̬�Ĵ���
    status = FRAM_ReceiveByte();
    FRAM_CS_SET;
    return(status);
}

//д״̬�Ĵ���
void FRAM_Write_SR(u8 u8newStatesRegVal)
{
    FRAM_CS_CLR;
    FRAM_SendByte(WRSR);
    FRAM_SendByte(u8newStatesRegVal);
    FRAM_CS_SET;
}

//ȫ����ַд����
void FRAM_BPALL(void)
{
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
    FRAM_CS_SET;
    FRAM_Write_SR(FM_BP_ALL);
}

//ȫ����ַ������
void FRAM_BPNON(void)
{
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
    FRAM_CS_SET;
    FRAM_Write_SR(FM_BP_NON);
}

//ֻ����4000h to 7FFFh 1/2��ַ
void FRAM_BPHALF(void)
{
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
    FRAM_CS_SET;
    FRAM_Write_SR(FM_BP_HALF);
}

//�Ӵ洢��ĳ��ַ��ʼ����˫��,С��ģʽ,���ֽ��ڵ͵�ַ.
u32 FRAM_ReadDW(u16 u16StarAddr)
{
    ULDATA sReadVal;
    sReadVal.ul = 0;
    FRAM_CS_CLR;
    FRAM_SendByte(READ);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    sReadVal.uc[0]=FRAM_ReceiveByte();  //RAMС��ģʽ,���ֽڴ�͵�ַλ
    sReadVal.uc[1]=FRAM_ReceiveByte();
    sReadVal.uc[2]=FRAM_ReceiveByte();
    sReadVal.uc[3]=FRAM_ReceiveByte();
    FRAM_CS_SET;;
    return sReadVal.ul;
}

//�Ӵ洢��ĳ��ַ��ʼ����˫��float����,С��ģʽ,���ֽ��ڵ͵�ַ.
float FRAM_ReadFloat(u16 u16StarAddr)
{
    FCDATA FCReadVal;
    FCReadVal.f = 0;
    FRAM_CS_CLR;
    FRAM_SendByte(READ);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    FCReadVal.uc[0]=FRAM_ReceiveByte();  //RAMС��ģʽ,���ֽڴ�͵�ַλ
    FCReadVal.uc[1]=FRAM_ReceiveByte();
    FCReadVal.uc[2]=FRAM_ReceiveByte();
    FCReadVal.uc[3]=FRAM_ReceiveByte();
    FRAM_CS_SET;;
    return FCReadVal.f;
}

//�Ӵ洢��ĳ��ַ��ʼ����8�ֽ�,С��ģʽ,���ֽ��ڵ͵�ַ.
u64 FRAM_Read8Byte(u16 u16StarAddr)  //64λ���ݶ���
{
    ULLDATA sReadVal;
    sReadVal.ull=0;
    FRAM_CS_CLR;
    FRAM_SendByte(READ);
    FRAM_SendByte((u8)(u16StarAddr>>8));
    FRAM_SendByte((u8)(u16StarAddr&0x00ff));
    sReadVal.uc[0]=FRAM_ReceiveByte();  //RAMС��ģʽ,���ֽڴ�͵�ַλ
    sReadVal.uc[1]=FRAM_ReceiveByte();
    sReadVal.uc[2]=FRAM_ReceiveByte();
    sReadVal.uc[3]=FRAM_ReceiveByte();
    sReadVal.uc[4]=FRAM_ReceiveByte();  //RAMС��ģʽ,���ֽڴ�͵�ַλ
    sReadVal.uc[5]=FRAM_ReceiveByte();
    sReadVal.uc[6]=FRAM_ReceiveByte();
    sReadVal.uc[7]=FRAM_ReceiveByte();
    FRAM_CS_SET;;
    return sReadVal.ull;
}

//����ʼ��ַ���ֽ�������
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

//����ʼ��ַ��˫��������
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

//�򴢴���ĳ��ʼ��ַ��ʼд��˫��,С��ģʽ���ֽ��ڵ͵�ַ.
void FRAM_WriteDW(u16 u16StarAddr,u32 u32WriteVal)
{
    u16 i;
    ULDATA sWriteVal;
    sWriteVal.ul=u32WriteVal;
    FRAM_BPHALF();            //ע��ֻ��д��ǰ1/2��ַ����1/2����״̬�����������ݣ�ֻ���ڱ��ݲ���ʱ�Ŵ򿪱���
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
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

//�򴢴���ĳ��ʼ��ַ��ʼд��˫��float����,С��ģʽ���ֽ��ڵ͵�ַ.
void FRAM_WriteFloat(u16 u16StarAddr,float fWriteVal)
{
    u16 i;
    FCDATA FCWriteVal;
    FCWriteVal.f=fWriteVal;
    FRAM_BPHALF();            //ע��ֻ��д��ǰ1/2��ַ����1/2����״̬�����������ݣ�ֻ���ڱ��ݲ���ʱ�Ŵ򿪱���
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
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

//�򴢴���ĳ��ʼ��ַ��ʼд��8�ֽ�,С��ģʽ���ֽ��ڵ͵�ַ.
void FRAM_Write8Byte(u16 u16StarAddr,u64 u64WriteVal)
{
    u16 i;

    ULLDATA sWriteVal;
    sWriteVal.ull=u64WriteVal;
    FRAM_BPHALF();            //ע��ֻ��д��ǰ1/2��ַ����1/2����״̬�����������ݣ�ֻ���ڱ��ݲ���ʱ�Ŵ򿪱���
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
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

//��������ʱʹ�ã��򿪺�1/2��ַ��д��Ȩ��
void FRAM_WriteDW_Backup(u16 u16StarAddr,u32 u32WriteVal)
{
    u16 i;

    ULDATA sWriteVal;
    sWriteVal.ul=u32WriteVal;

    FRAM_BPNON();            //ȡ��ȫ���������������1/2��ַ��д�����
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
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

//����ʼ��ַ���ֽ�����д
void FRAM_ContWriteByte(u16 u16StarAddr,u8* u8Arry,u16 u16ByteNum)
{
    u16 i;
    
    FRAM_BPHALF();            //ע��ֻ��д��ǰ1/2��ַ����1/2����״̬�����������ݣ�ֻ���ڱ��ݲ���ʱ�Ŵ򿪱���
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
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

//����ʼ��ַ��˫������д,д�����ݴ����ڳ���������
void FRAM_ContWriteDW(u16 u16StarAddr,u32* u32Arry,u16 u16DWNum)
{
    u16 i;
    ULDATA sWriteVal;
    FRAM_BPHALF();            //ע��ֻ��д��ǰ1/2��ַ����1/2����״̬�����������ݣ�ֻ���ڱ��ݲ���ʱ�Ŵ򿪱���
    FRAM_CS_CLR;
    FRAM_SendByte(WREN);  //дʹ��,���κ�д����ǰ������ִ��дʹ��
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








