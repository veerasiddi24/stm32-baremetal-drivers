/*
 * stm32f103xx_tim_driver.h
 *
 *  Created on: May 14, 2026
 *      Author: lenovo
 */

#ifndef INC_STM32L476XX_TIM_DRIVER_H_
#define INC_STM32L476XX_TIM_DRIVER_H_

#define TIM_IC_RISING_EDGE     0
#define TIM_IC_FALLING_EDGE    1

#define TIM_ICSELECTION_DIRECTTI   1


typedef struct
{
    uint32_t Prescaler;
    uint32_t Period;
    uint8_t ICPolarity;
    uint8_t ICSelection;

}TIM_Config_t;

typedef struct
{
    TIM_RegDef_t *pTIMx;
    TIM_Config_t TIM_Config;

}TIM_Handle_t;



typedef struct
{
    uint32_t Prescaler;
    uint32_t Period;
    uint8_t ICPolarity;
    uint8_t ICSelection;

}TIM2345_Config_t;

typedef struct
{
    TIM2345_RegDef_t *pTIMx;
    TIM2345_Config_t TIM2345_Config;

}TIM2345_Handle_t;

void TIM_Init(TIM_Handle_t *pTIMHandle);
void TIM_IC_Init(TIM_Handle_t *pTIMHandle);
void TIM_Start(TIM_RegDef_t *pTIMx);
void TIM_Stop(TIM_RegDef_t *pTIMx);

#endif /* INC_STM32L476XX_TIM_DRIVER_H_ */
