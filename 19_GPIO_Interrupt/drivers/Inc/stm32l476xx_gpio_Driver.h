/*
 * stm32l476xx_gpio_Driver.h
 *
 *  Created on: May 6, 2026
 *      Author: lenovo
 */

#ifndef INC_STM32L476XX_GPIO_DRIVER_H_
#define INC_STM32L476XX_GPIO_DRIVER_H_

#define GPIO_MODE_INPUT     0
#define GPIO_MODE_OUTPUT    1
#define GPIO_MODE_ALTFN     2
#define GPIO_MODE_ANALOG    3
#define GPIO_MODE_IT_FT     4
#define GPIO_MODE_IT_RT     5
#define GPIO_MODE_IT_RFT    6

#define GPIO_OP_TYPE_PP     0
#define GPIO_OP_TYPE_OD     1

#define GPIO_NO_PUPD        0
#define GPIO_PIN_PU         1
#define GPIO_PIN_PD         2

#define AF0 0
#define AF1 1
#define AF2 2
#define AF3 3
#define AF4 4
#define AF5 5
#define AF6 6
#define AF7 7
#define AF8 8
#define AF9 9
#define AF10 10
#define AF11 11
#define AF12 12
#define AF13 13
#define AF14 14
#define AF15 15

typedef struct
{
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_PinMode;
    uint8_t GPIO_PinSpeed;
    uint8_t GPIO_PinPuPdControl;
    uint8_t GPIO_PinOPType;
    uint8_t GPIO_PinAltFunMode;
} GPIO_PinConfig_t;

typedef struct
{
    GPIO_TypeDef *pGPIOx;
    GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;


#define GPIOA_Reg_Reset() do{ RCC->AHB2RSTR |= (0x01<<0); RCC->AHB2RSTR &= ~(0x01<<0);}while(0)
#define GPIOB_Reg_Reset() do{ RCC->AHB2RSTR |= (0x01<<1); RCC->AHB2RSTR &= ~(0x01<<1);}while(0)
#define GPIOC_Reg_Reset() do{ RCC->AHB2RSTR |= (0x01<<2); RCC->AHB2RSTR &= ~(0x01<<2);}while(0)
#define GPIOD_Reg_Reset() do{ RCC->AHB2RSTR |= (0x01<<3); RCC->AHB2RSTR &= ~(0x01<<3);}while(0)
#define GPIOE_Reg_Reset() do{ RCC->AHB2RSTR |= (0x01<<4); RCC->AHB2RSTR &= ~(0x01<<4);}while(0)
#define GPIOF_Reg_Reset() do{ RCC->AHB2RSTR |= (0x01<<5); RCC->AHB2RSTR &= ~(0x01<<5);}while(0)
#define GPIOG_Reg_Reset() do{ RCC->AHB2RSTR |= (0x01<<6); RCC->AHB2RSTR &= ~(0x01<<6);}while(0)
#define GPIOH_Reg_Reset() do{ RCC->AHB2RSTR |= (0x01<<7); RCC->AHB2RSTR &= ~(0x01<<7);}while(0)

#define GPIO_BASEADDR_TO_CODE(x)  (((x) == GPIOA) ? 0 : \
                                   ((x) == GPIOB) ? 1 : \
                                   ((x) == GPIOC) ? 2 : \
                                   ((x) == GPIOD) ? 3 : \
                                   ((x) == GPIOE) ? 4 : \
                                   ((x) == GPIOF) ? 5 : \
                                   ((x) == GPIOG) ? 6 : \
                                   ((x) == GPIOH) ? 7 : 0)

void GPIO_peripherial_Clk_Cntrl(GPIO_TypeDef *pGPIOx, uint8_t ENorDI);
void GPIO_config();
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_TypeDef *pGPIOx);
uint8_t GPIO_ReadFrom_Input_Pin(GPIO_TypeDef *pGPIOx, uint8_t Pin_Number);
uint8_t GPIO_ReadFrom_Input_Port(GPIO_TypeDef *pGPIOx);
void GPIO_Write_Output_Pin(GPIO_TypeDef *pGPIOx, uint8_t Pin_Number,uint8_t val);
void GPIO_Write_Output_Port(GPIO_TypeDef *pGPIOx, uint16_t val);
void GPIO_Toggle_Pin(GPIO_TypeDef *pGPIOx, uint8_t Pin_Number);





#endif /* INC_STM32L476XX_GPIO_DRIVER_H_ */
