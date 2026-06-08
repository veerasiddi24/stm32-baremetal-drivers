/*
 * stm32f103xx_tim_driver.c
 *
 *  Created on: May 14, 2026
 *      Author: lenovo
 */
#include "stm32l476xx.h"

#include "stm32l476xx_tim_driver.h"

void TIM_Init(TIM_Handle_t *pTIMHandle)
{
    /* Configure prescaler */
    pTIMHandle->pTIMx->PSC =
            pTIMHandle->TIM_Config.Prescaler;

    /* Configure auto reload */
    pTIMHandle->pTIMx->ARR =
            pTIMHandle->TIM_Config.Period;
}

void TIM2345_Init(TIM2345_Handle_t *pTIMHandle)
{
    /* Configure prescaler */
    pTIMHandle->pTIMx->PSC =
            pTIMHandle->TIM2345_Config.Prescaler;

    /* Configure auto reload */
    pTIMHandle->pTIMx->ARR =
            pTIMHandle->TIM2345_Config.Period;
}

void TIM_IC_Init(TIM_Handle_t *pTIMHandle)
{
    uint32_t temp = 0;

    /* CC1 channel configured as input */
    temp |= (1 << 0);

    pTIMHandle->pTIMx->CCMR1 = temp;

    /* Select polarity */
    if(pTIMHandle->TIM_Config.ICPolarity ==
            TIM_IC_FALLING_EDGE)
    {
        pTIMHandle->pTIMx->CCER |= (1 << 1);
    }
    else
    {
        pTIMHandle->pTIMx->CCER &= ~(1 << 1);
    }

    /* Enable Capture */
    pTIMHandle->pTIMx->CCER |= (1 << 0);
}

void TIM_Start(TIM_RegDef_t *pTIMx)
{
    pTIMx->CR1 |= (1 << 0);
}

void TIM2345_Start(TIM_RegDef_t *pTIMx)
{
    pTIMx->CR1 |= (1 << 0);
}

void TIM_Stop(TIM_RegDef_t *pTIMx)
{
    pTIMx->CR1 &= ~(1 << 0);
}
void TIM2345_Stop(TIM_RegDef_t *pTIMx)
{
    pTIMx->CR1 &= ~(1 << 0);
}

void Timer_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ISER1 |=
                (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ISER2 |=
                (1 << (IRQNumber % 64));
        }
    }
    else
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ICER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ICER1 |=
                (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ICER2 |=
                (1 << (IRQNumber % 64));
        }
    }
}

void Timer_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}
