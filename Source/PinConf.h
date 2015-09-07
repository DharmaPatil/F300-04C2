#ifndef _PINCONF_H
#define _PINCONF_H


#define IN0_READ    GPIO_BitRead(GPIO2,15)
#define IN1_READ    GPIO_BitRead(GPIO2,14)
#define IN2_READ    GPIO_BitRead(GPIO2,13)
#define IN3_READ    GPIO_BitRead(GPIO2,12)
#define IN4_READ    GPIO_BitRead(GPIO2,11)
#define IN5_READ    GPIO_BitRead(GPIO2,10)
#define IN6_READ    GPIO_BitRead(GPIO2,7)
#define IN7_READ    GPIO_BitRead(GPIO2,6)

//注意CLR状态时仪表实际开关量输出状态为有效，SET为无效
#define OUT0_CLR    GPIO_BitWrite(GPIO2,5,0)
#define OUT0_SET    GPIO_BitWrite(GPIO2,5,1)
#define OUT1_CLR    GPIO_BitWrite(GPIO2,4,0)
#define OUT1_SET    GPIO_BitWrite(GPIO2,4,1)
#define OUT2_CLR    GPIO_BitWrite(GPIO2,3,0)
#define OUT2_SET    GPIO_BitWrite(GPIO2,3,1)
#define OUT3_CLR    GPIO_BitWrite(GPIO2,2,0)
#define OUT3_SET    GPIO_BitWrite(GPIO2,2,1)
#define OUT4_CLR    GPIO_BitWrite(GPIO0,14,0)
#define OUT4_SET    GPIO_BitWrite(GPIO0,14,1)
#define OUT5_CLR    GPIO_BitWrite(GPIO0,13,0)
#define OUT5_SET    GPIO_BitWrite(GPIO0,13,1)
#define OUT6_CLR    GPIO_BitWrite(GPIO2,1,0)
#define OUT6_SET    GPIO_BitWrite(GPIO2,1,1)
#define OUT7_CLR    GPIO_BitWrite(GPIO2,0,0)
#define OUT7_SET    GPIO_BitWrite(GPIO2,0,1)
#define OUT8_CLR    GPIO_BitWrite(GPIO0,12,0)
#define OUT8_SET    GPIO_BitWrite(GPIO0,12,1)
#define OUT9_CLR    GPIO_BitWrite(GPIO0,7,0)
#define OUT9_SET    GPIO_BitWrite(GPIO0,7,1)
#define OUT10_CLR   GPIO_BitWrite(GPIO0,6,0)
#define OUT10_SET   GPIO_BitWrite(GPIO0,6,1)
#define OUT11_CLR   GPIO_BitWrite(GPIO0,5,0)
#define OUT11_SET   GPIO_BitWrite(GPIO0,5,1)

#define rdefGPIO0  ((GPIO_TypeDef *)(0xE0003000))
#define rdefGPIO1  ((GPIO_TypeDef *)(0xE0004000))
#define rdefGPIO2  ((GPIO_TypeDef *)(0xE0005000))

extern       GPIO_TypeDef* const getOutGPIOx[16];
extern const u8	           getOutGPIOPin[16];
extern       GPIO_TypeDef* const getInGPIOx[12];
extern const u8            getInGPIOPin[12];

extern void IO_PinConfig(void);
extern void BSPI_PinConfig(void);
extern void USPI2_PinConfig(void);
extern void USPI3_PinConfig(void);
extern void UART0_PinConfig(void);
extern void UART1_PinConfig(void);
extern void PinConfig(void);

#endif
