/*************************************************
Copyright [C], 1993-2009, Shen zhen General Measure Tech. Co., Ltd.
File name:              TIMER.C
Description:            ��ʱ�� �ײ㺯��
Author:                 JXH
Version:                0.0.0.1
Date:                   2009-4-4 17:03:17
Others: 
History:
*************************************************/
#include "include.h"

GPIO_TypeDef* const getOutGPIOx[16]
    ={rdefGPIO0,rdefGPIO2,rdefGPIO2,rdefGPIO0,
      rdefGPIO0,rdefGPIO2,rdefGPIO2,rdefGPIO2,
      rdefGPIO2,rdefGPIO2,rdefGPIO2,rdefGPIO2,
      rdefGPIO2,rdefGPIO2,rdefGPIO2,rdefGPIO2 };

const u8    getOutGPIOPin[16]
    ={12, 0, 1,13,
      14, 2, 3, 4,
       5, 6, 7, 8,
       9,10,11,12 };

GPIO_TypeDef* const getInGPIOx[12]
    ={rdefGPIO2,rdefGPIO2,rdefGPIO2,rdefGPIO0,
      rdefGPIO1,rdefGPIO1,rdefGPIO1,rdefGPIO1,
      rdefGPIO1,rdefGPIO1,rdefGPIO0,rdefGPIO0 };

const u8 getInGPIOPin[12]
    ={13,14,15,15,
      11,12, 9,10,
      13,15, 3, 7 };

/*********************************************************************
��������: void IO_PinConfig(void)
��    ��: �������������IO���������Ͷ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: chx                  ʱ    ��: 2013-12-4
��    ��:                      ʱ    ��: 
*********************************************************************/
void IO_PinConfig(void)
{
    //8���뿪����IO�������Ͷ���
	GPIO_Config(GPIO2,(1<<15),GPIO_IN_TRI_TTL); //IN0_READ
    GPIO_Config(GPIO2,(1<<14),GPIO_IN_TRI_TTL); //IN1_READ
    GPIO_Config(GPIO2,(1<<13),GPIO_IN_TRI_TTL); //IN2_READ
    GPIO_Config(GPIO2,(1<<12),GPIO_IN_TRI_TTL); //IN3_READ
    GPIO_Config(GPIO2,(1<<11),GPIO_IN_TRI_TTL); //IN4_READ
    GPIO_Config(GPIO2,(1<<10),GPIO_IN_TRI_TTL); //IN5_READ
    GPIO_Config(GPIO2,(1<<7) ,GPIO_IN_TRI_TTL); //IN6_READ
    GPIO_Config(GPIO2,(1<<6) ,GPIO_IN_TRI_TTL); //IN7_READ
    //12���������IO�������Ͷ���
    GPIO_Config(GPIO2,(1<<5) ,GPIO_OUT_PP);      //OUT0
    GPIO_Config(GPIO2,(1<<4) ,GPIO_OUT_PP);      //OUT1
    GPIO_Config(GPIO2,(1<<3) ,GPIO_OUT_PP);      //OUT2
    GPIO_Config(GPIO2,(1<<2) ,GPIO_OUT_PP);      //OUT3
    GPIO_Config(GPIO0,(1<<14),GPIO_OUT_PP);      //OUT4
    GPIO_Config(GPIO0,(1<<13),GPIO_OUT_PP);      //OUT5
    GPIO_Config(GPIO2,(1<<1) ,GPIO_OUT_PP);      //OUT6
    GPIO_Config(GPIO2,(1<<0) ,GPIO_OUT_PP);      //OUT7
    GPIO_Config(GPIO0,(1<<12),GPIO_OUT_PP);      //OUT8
    GPIO_Config(GPIO0,(1<<7) ,GPIO_OUT_PP);      //OUT9
    GPIO_Config(GPIO0,(1<<6) ,GPIO_OUT_PP);      //OUT10
    GPIO_Config(GPIO0,(1<<5) ,GPIO_OUT_PP);      //OUT11
    //���������ȫ����Ϊ��Ч
    OUT0_SET;
    OUT1_SET;
    OUT2_SET;
    OUT3_SET;
    OUT4_SET;
    OUT5_SET;
    OUT6_SET;
    OUT7_SET;
    OUT8_SET;
    OUT9_SET;
    OUT10_SET;
    OUT11_SET;
}

void BSPI_PinConfig(void)
{
    ////BSPI0   ϵͳSPI����0 ADC0
    //GPIO_Config(GPIO0,(1<<0),GPIO_IN_TRI_TTL);    //SPIO.MISO
    //GPIO_Config(GPIO0,(1<<1),GPIO_AF_PP);     //SPIO.MOSI
    //GPIO_Config(GPIO0,(1<<2),GPIO_AF_PP);     //SPIO.SCLK
    //
    ////BSPI1 ϵͳSPI����1 ADC1
    //GPIO_Config(GPIO0,(1<<4),GPIO_IN_TRI_TTL);    //SPI0.MISO
    //GPIO_Config(GPIO0,(1<<5),GPIO_AF_PP);     //SPI0.MOSI
    //GPIO_Config(GPIO0,(1<<6),GPIO_AF_PP);     //SPI0.SCLK

    //BSPI0 ϵͳSPI����0  ADC0
    //2009-3-31 13:05:48 ��ʹ��ģ��
    GPIO_Config(GPIO0,(1<<0),GPIO_IN_TRI_TTL);  //SPIO.MISO
    GPIO_Config(GPIO0,(1<<1),GPIO_OUT_PP);      //SPIO.MOSI
    GPIO_Config(GPIO0,(1<<2),GPIO_OUT_PP);      //SPIO.SCLK

    //BSPI1 ϵͳSPI����1 ADC1
    //2009-3-31 13:06:02 ��ʹ��ģ��
    GPIO_Config(GPIO0,(1<<4),GPIO_IN_TRI_TTL);  //SPI0.MISO
    GPIO_Config(GPIO0,(1<<5),GPIO_OUT_PP);      //SPI0.MOSI
    GPIO_Config(GPIO0,(1<<6),GPIO_OUT_PP);      //SPI0.SCLK
}


void USPI2_PinConfig(void)
{
    //USPI2 ���ģ��SPI���� RTC
    GPIO_Config(GPIO1,(1<<2),GPIO_OUT_PP);      //SPI2.SS1
    GPIO_Config(GPIO1,(1<<3),GPIO_OUT_PP);      //SPI2.SCLK
    //GPIO_Config(GPIO1,(1<<4),GPIO_INOUT_WP);  //SPI2.SDIO
    GPIO_Config(GPIO1,(1<<4),GPIO_IN_TRI_TTL);
}

void USPI3_PinConfig(void)
{
    //USPI3 ���ģ��SPI���� FM VFD
    GPIO_Config(GPIO1,(1<<6),GPIO_IN_TRI_TTL);  //SPI3.MISO
    GPIO_Config(GPIO1,(1<<8),GPIO_OUT_PP);      //SPI3.MOSI
    GPIO_Config(GPIO1,(1<<7),GPIO_OUT_PP);      //SPI3.SCLK

    GPIO_Config(GPIO1,(1<<5),GPIO_OUT_PP);      //SPI3.SS1  :����洢��Ƭѡ
    GPIO_BitWrite(GPIO1, 5,1);                      //Ƭѡ��Ч

    GPIO_Config(GPIO1,(1<<14),GPIO_OUT_PP);     //SPI3.SS2  :VFD����16311Ƭѡ
    GPIO_BitWrite(GPIO1, 5,1);                                              //Ƭѡ��Ч
}

void UART0_PinConfig(void)
{
    //UART0 UART0 ��RS232��RS485��ѡ��
    GPIO_Config(GPIO0,(1<<9),GPIO_AF_PP);       //U0.TXD
    GPIO_BitWrite(GPIO0,9,1);
    GPIO_Config(GPIO0,(1<<8),GPIO_IN_TRI_TTL);  //U0.RXD
    GPIO_BitWrite(GPIO0,8,1);
    GPIO_Config(GPIO1,(1<<1),GPIO_OUT_PP);      //U0.S485
}

void UART1_PinConfig(void)
{
    //UART1 ֻ����RS232
    GPIO_Config(GPIO0,(1<<11),GPIO_AF_PP);      //U1.TXD
    GPIO_BitWrite(GPIO0,11,1);
    GPIO_Config(GPIO0,(1<<10),GPIO_IN_TRI_TTL); //U1.RXD
    GPIO_BitWrite(GPIO0,10,1);
}

/*********************************************************************
��������: void BUZER_PinConfig(void)
��    ��: ����������������Ͷ���
˵    ��: 
��ڲ���: 
�� �� ֵ: 
��    ��: chx                  ʱ    ��: 2013-12-4
��    ��:                      ʱ    ��: 
*********************************************************************/
void BUZER_PinConfig(void)
{
    GPIO_Config(GPIO1,(1<<0),GPIO_OUT_PP);
    GPIO_BitWrite(GPIO1,0,0);               //��������
}

void PinConfig(void)
{
    IO_PinConfig();
//    UART0_PinConfig();
//    UART1_PinConfig();
    BUZER_PinConfig();
    BSPI_PinConfig();
    USPI2_PinConfig();
    USPI3_PinConfig();
}

