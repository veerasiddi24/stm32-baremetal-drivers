/*
 * stm32L4xx.h
 *
 *  Created on: May 6, 2026
 *      Author: lenovo
 */

#ifndef INC_STM32L4XX_H_
#define INC_STM32L4XX_H_

#include <stdint.h>

#define ENABLE 1
#define DISABLE 0
#define SET     ENABLE
#define RESET DISABLE


#define IRQ_NO_EXTI0       6
#define IRQ_NO_EXTI1       7
#define IRQ_NO_EXTI2       8
#define IRQ_NO_EXTI3       9
#define IRQ_NO_EXTI4       10
#define IRQ_NO_EXTI9_5     23
#define IRQ_NO_EXTI15_10   40

#define IRQ_NO_SPI1   35
#define IRQ_NO_SPI2   36
#define IRQ_NO_SPI3   51

#define IRQ_NO_UART4    52 //

#define SPI_CR1_CPHA        0
#define SPI_CR1_CPOL        1
#define SPI_CR1_MSTR        2
#define SPI_CR1_BR          3
#define SPI_CR1_SPE         6
#define SPI_CR1_LSBFIRST    7
#define SPI_CR1_SSI         8
#define SPI_CR1_SSM         9
#define SPI_CR1_RXONLY      10
#define SPI_CR1_CRCL        11
#define SPI_CR1_CRCNEXT     12
#define SPI_CR1_CRCEN       13
#define SPI_CR1_BIDIOE      14
#define SPI_CR1_BIDIMODE    15


#define USART_CR1_UE        0
#define USART_CR1_RE        2
#define USART_CR1_TE        3
#define USART_CR1_RXNEIE    5
#define USART_CR1_TCIE      6
#define USART_CR1_TXEIE     7

#define USART_ISR_ORE       3
#define USART_ISR_RXNE      5
#define USART_ISR_TC        6
#define USART_ISR_TXE       7

#define USART_ICR_ORECF     3

#define FLASH_BASE_ADDR    0x08000000U
#define SRAM1_BASE_ADDR    0x20000000U
#define SRAM               SRAM1_BASE_ADDR
#define SRAM2_BASE_ADDR    0x10000000U
#define ROM_BASE_ADDR      0x1FFF8000U


#define PERIPHERIAL_BASE_ADDR 0x40000000U
#define APB1_PERIPH_BASE_ADDR PERIPHERIAL_BASE_ADDR
#define APB2_PERIPH_BASE_ADDR 0x40010000U
#define AHB1_PERIPH_BASE_ADDR 0x40020000U
#define AHB2_PERIPH_BASE_ADDR 0x48000000U


#define RCC_BASE_ADDR (AHB1_PERIPH_BASE_ADDR+0x1000)

#define GPIOA_BASE_ADDR (AHB2_PERIPH_BASE_ADDR+0x0000)
#define GPIOB_BASE_ADDR (AHB2_PERIPH_BASE_ADDR+0x0400)
#define GPIOC_BASE_ADDR (AHB2_PERIPH_BASE_ADDR+0x0800)
#define GPIOD_BASE_ADDR (AHB2_PERIPH_BASE_ADDR+0x0C00)
#define GPIOE_BASE_ADDR (AHB2_PERIPH_BASE_ADDR+0x1000)
#define GPIOF_BASE_ADDR (AHB2_PERIPH_BASE_ADDR+0x1400)
#define GPIOG_BASE_ADDR (AHB2_PERIPH_BASE_ADDR+0x1800)
#define GPIOH_BASE_ADDR (AHB2_PERIPH_BASE_ADDR+0x1C00)

#define ADC_BASE_ADDR    (0x50040000u)
#define SPI1_BASE_ADDR  (APB2_PERIPH_BASE_ADDR+0x3000)
#define SPI2_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x3800)
#define SPI3_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x3C00)
#define USART2_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x4400)
#define USART3_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x4800)
#define UART4_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x4C00)
#define UART5_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x5000)
#define I2C1_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x5400)
#define I2C2_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x5800)
#define I2C3_BASE_ADDR (APB1_PERIPH_BASE_ADDR+0x5C00)


#define SYSCONFG_BASE_ADDR   (APB2_PERIPH_BASE_ADDR+0x0000)
#define EXTI_BASE_ADDR       (APB2_PERIPH_BASE_ADDR+0x0400)
#define USART1_BASE_ADDR     (APB2_PERIPH_BASE_ADDR+0x3800)


#define NO_PR_BITS_IMPLEMENTED 4

#define NVIC_ISER0   ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1   ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2   ((volatile uint32_t*)0xE000E108)

#define NVIC_ICER0   ((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1   ((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2   ((volatile uint32_t*)0xE000E188)
#define NVIC_PR_BASE_ADDR ((volatile uint32_t*)0xE000E400)



typedef struct
{
  volatile uint32_t MODER;    // Mode register
  volatile uint32_t OTYPER;   // Output type
  volatile uint32_t OSPEEDR;  // Output speed
  volatile uint32_t PUPDR;    // Pull-up / pull-down
  volatile uint32_t IDR;      // Input data
  volatile uint32_t ODR;      // Output data
  volatile uint32_t BSRR;     // Bit set/reset
  volatile uint32_t LCKR;     // Lock register
  volatile uint32_t AFR[2];   // Alternate function (AFR[0]=low, AFR[1]=high)
  volatile uint32_t BRR;      // Bit reset register
} GPIO_TypeDef;


#define GPIOA (GPIO_TypeDef*)(GPIOA_BASE_ADDR)
#define GPIOB (GPIO_TypeDef*)(GPIOB_BASE_ADDR)
#define GPIOC (GPIO_TypeDef*)(GPIOC_BASE_ADDR)
#define GPIOD (GPIO_TypeDef*)(GPIOD_BASE_ADDR)
#define GPIOE (GPIO_TypeDef*)(GPIOE_BASE_ADDR)
#define GPIOF (GPIO_TypeDef*)(GPIOF_BASE_ADDR)
#define GPIOG (GPIO_TypeDef*)(GPIOG_BASE_ADDR)
#define GPIOH (GPIO_TypeDef*)(GPIOH_BASE_ADDR)



typedef struct
{
  volatile uint32_t CR;          // Clock control register
  volatile uint32_t ICSCR;       // Internal clock sources calibration
  volatile uint32_t CFGR;        // Clock configuration
  volatile uint32_t PLLCFGR;     // PLL configuration
  volatile uint32_t PLLSAI1CFGR; // PLLSAI1 configuration
  volatile uint32_t PLLSAI2CFGR; // PLLSAI2 configuration
  volatile uint32_t CIER;        // Clock interrupt enable
  volatile uint32_t CIFR;        // Clock interrupt flag
  volatile uint32_t CICR;        // Clock interrupt clear
  volatile uint32_t RESERVED0;
  volatile uint32_t AHB1RSTR;    // AHB1 reset
  volatile uint32_t AHB2RSTR;    // AHB2 reset
  volatile uint32_t AHB3RSTR;    // AHB3 reset
  volatile uint32_t RESERVED1;
  volatile uint32_t APB1RSTR1;   // APB1 reset
  volatile uint32_t APB1RSTR2;   // APB1 reset (2)
  volatile uint32_t APB2RSTR;    // APB2 reset
  volatile uint32_t RESERVED2;
  volatile uint32_t AHB1ENR;     // AHB1 enable
  volatile uint32_t AHB2ENR;     // AHB2 enable  🔥 (GPIO here)
  volatile uint32_t AHB3ENR;     // AHB3 enable
  volatile uint32_t RESERVED3;
  volatile uint32_t APB1ENR1;    // APB1 enable
  volatile uint32_t APB1ENR2;    // APB1 enable (2)
  volatile uint32_t APB2ENR;     // APB2 enable
  volatile uint32_t RESERVED4;
  volatile uint32_t AHB1SMENR;   // Sleep mode enable
  volatile uint32_t AHB2SMENR;
  volatile uint32_t AHB3SMENR;
  volatile uint32_t RESERVED5;
  volatile uint32_t APB1SMENR1;
  volatile uint32_t APB1SMENR2;
  volatile uint32_t APB2SMENR;
  volatile uint32_t RESERVED6;
  volatile uint32_t CCIPR;       // Peripheral clock selection
  volatile uint32_t RESERVED7;
  volatile uint32_t BDCR;        // Backup domain control
  volatile uint32_t CSR;         // Control/status
  volatile uint32_t CRRCR;
  volatile uint32_t CCIPR2;
} RCC_TypeDef;


#define RCC ((RCC_TypeDef*)RCC_BASE_ADDR)

typedef struct
{
  volatile uint32_t EXTI_IMR1;
  volatile uint32_t EXTI_EMR1;
  volatile uint32_t EXTI_RTSR1;
  volatile uint32_t EXTI_FTSR1;
  volatile uint32_t EXTI_SWIER1;
  volatile uint32_t EXTI_PR1;

}EXTI_TypeDef;

#define EXTI ((EXTI_TypeDef*)EXTI_BASE_ADDR)

typedef struct
{
  volatile uint32_t SYSCFG_MEMRMP;
  volatile uint32_t SYSCFG_CFGR1;
  volatile uint32_t SYSCFG_EXTICR[4];
  volatile uint32_t SYSCFG_SCSR;
  volatile uint32_t SYSCFG_CFGR2;
}SYSCNFG_TypeDef;

#define SYSCNFG ((SYSCNFG_TypeDef*)SYSCONFG_BASE_ADDR)

typedef struct
{
	volatile uint32_t SPIx_CR1;
	volatile uint32_t SPIx_CR2;
	volatile uint32_t SPIx_SR;
	volatile uint32_t SPIx_DR;
	volatile uint32_t SPIx_CRCPR;
	volatile uint32_t SPIx_RXCRCR;
	volatile uint32_t SPIx_TXCRCR;
}SPI_TypeDef;


#define SPI1 ((SPI_TypeDef*)SPI1_BASE_ADDR)
#define SPI2 ((SPI_TypeDef*)SPI2_BASE_ADDR)
#define SPI3 ((SPI_TypeDef*)SPI3_BASE_ADDR)

typedef struct
{
    volatile uint32_t CR1;      // 0x00
    volatile uint32_t CR2;      // 0x04
    volatile uint32_t CR3;      // 0x08
    volatile uint32_t BRR;      // 0x0C
    volatile uint32_t GTPR;     // 0x10
    volatile uint32_t RTOR;     // 0x14
    volatile uint32_t RQR;      // 0x18
    volatile uint32_t ISR;      // 0x1C
    volatile uint32_t ICR;      // 0x20
    volatile uint32_t RDR;      // 0x24
    volatile uint32_t TDR;      // 0x28

}USART_TypeDef;

#define UART4 ((USART_TypeDef*)UART4_BASE_ADDR)
#define UART5 ((USART_TypeDef*)UART5_BASE_ADDR)
/*=========================================================
 * ADC REGISTER DEFINITIONS
 * STM32L476
 *=========================================================*/

typedef struct
{
    volatile uint32_t ISR;          // 0x00
    volatile uint32_t IER;          // 0x04
    volatile uint32_t CR;           // 0x08
    volatile uint32_t CFGR;         // 0x0C
    volatile uint32_t CFGR2;        // 0x10

    volatile uint32_t SMPR1;        // 0x14
    volatile uint32_t SMPR2;        // 0x18

    volatile uint32_t RESERVED1;    // 0x1C

    volatile uint32_t TR1;          // 0x20
    volatile uint32_t TR2;          // 0x24
    volatile uint32_t TR3;          // 0x28

    volatile uint32_t RESERVED2;    // 0x2C

    volatile uint32_t SQR1;         // 0x30
    volatile uint32_t SQR2;         // 0x34
    volatile uint32_t SQR3;         // 0x38
    volatile uint32_t SQR4;         // 0x3C

    volatile uint32_t DR;           // 0x40

}ADC_TypeDef;

typedef struct
{
    volatile uint32_t CSR;
    volatile uint32_t CCR;
    volatile uint32_t CDR;

}ADC_Common_TypeDef;


#define ADC1 ((ADC_TypeDef*)ADC_BASE_ADDR)
#define ADC_COMMON ((ADC_Common_TypeDef*)ADC1)




#define SYSCNFG_PCLK_EN() (RCC->APB2ENR |= (0X01<<0))

#define GPIOA_PCLK_EN() (RCC->AHB2ENR |= (0x01<<0))
#define GPIOB_PCLK_EN() (RCC->AHB2ENR |= (0x01<<1))
#define GPIOC_PCLK_EN() (RCC->AHB2ENR |= (0x01<<2))
#define GPIOD_PCLK_EN() (RCC->AHB2ENR |= (0x01<<3))
#define GPIOE_PCLK_EN() (RCC->AHB2ENR |= (0x01<<4))
#define GPIOF_PCLK_EN() (RCC->AHB2ENR |= (0x01<<5))
#define GPIOG_PCLK_EN() (RCC->AHB2ENR |= (0x01<<6))
#define GPIOH_PCLK_EN() (RCC->AHB2ENR |= (0x01<<7))

#define I2C1_PCLK_EN() (RCC->APB1ENR1 |= (0x01<<21))

#define UART4_PCLK_EN() (RCC->APB1ENR1 |= (0x01<<19))
#define UART5_PCLK_EN() (RCC->APB1ENR1 |= (0x01<<20))

#define SPI1_PCLK_EN() (RCC->APB2ENR |= (0X01<<12))
#define SPI2_PCLK_EN() (RCC->APB1ENR1 |= (0X01<<14))
#define SPI3_PCLK_EN() (RCC->APB1ENR1 |= (0X01<<15))


#define SYSCNFG_PCLK_DI()    (RCC->APB2ENR &= ~(0X01<<0))

#define GPIOA_PCLK_DI() (RCC->AHB2ENR &= ~(0x01<<0))
#define GPIOB_PCLK_DI() (RCC->AHB2ENR &= ~(0x01<<1))
#define GPIOC_PCLK_DI() (RCC->AHB2ENR &= ~(0x01<<2))
#define GPIOD_PCLK_DI() (RCC->AHB2ENR &= ~(0x01<<3))
#define GPIOE_PCLK_DI() (RCC->AHB2ENR &= ~(0x01<<4))
#define GPIOF_PCLK_DI() (RCC->AHB2ENR &= ~(0x01<<5))
#define GPIOG_PCLK_DI() (RCC->AHB2ENR &= ~(0x01<<6))
#define GPIOH_PCLK_DI() (RCC->AHB2ENR &= ~(0x01<<7))

#define I2C1_PCLK_DI() (RCC->APB1ENR1 &= ~(0x01<<21))

#define UART4_PCLK_DI() (RCC->APB1ENR1 &= ~(0x01<<19))

#define SPI1_PCLK_DI() (RCC->APB2ENR &= ~(0X01<<12))




#endif /* INC_STM32L4XX_H_ */
