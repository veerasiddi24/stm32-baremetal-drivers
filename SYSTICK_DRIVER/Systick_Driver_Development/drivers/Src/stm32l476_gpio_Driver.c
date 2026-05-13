/*
 * stm32l476_gpio_Driver.c
 *
 *  Created on: May 6, 2026
 *      Author: lenovo
 */
#include <stdint.h>
#include "stm32l476xx.h"
#include "stm32l476xx_gpio_Driver.h"


void GPIO_peripherial_Clk_Cntrl(GPIO_TypeDef *pGPIOx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx ==GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
	}
	else
	{

	}
}




void GPIO_config()
{
	GPIO_Handle_t Button;
//
////	GPIO_Handle_t SPIclk;
////
////	GPIO_Handle_t SPImiso;
////
////	GPIO_Handle_t SPImosi;
//
	Button.pGPIOx = GPIOA;
	Button.GPIO_PinConfig.GPIO_PinNumber = 5;
	Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	Button.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	Button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	Button.GPIO_PinConfig.GPIO_PinSpeed = 3;

//
///*	SPIclk.pGPIOx = GPIOA;
//	SPIclk.GPIO_PinConfig.GPIO_PinNumber = 5;
//	SPIclk.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
//	SPIclk.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//	SPIclk.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//	SPIclk.GPIO_PinConfig.GPIO_PinSpeed = 3;
//	SPIclk.GPIO_PinConfig.GPIO_PinAltFunMode = AF5;
//
//
//	SPImiso.pGPIOx = GPIOA;
//	SPImiso.GPIO_PinConfig.GPIO_PinNumber = 6;
//	SPImiso.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
//	SPImiso.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//	SPImiso.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//	SPImiso.GPIO_PinConfig.GPIO_PinSpeed = 3;
//	SPImiso.GPIO_PinConfig.GPIO_PinAltFunMode = AF5;
//
//	SPImosi.pGPIOx = GPIOA;
//	SPImosi.GPIO_PinConfig.GPIO_PinNumber = 7;
//	SPImosi.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
//	SPImosi.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//	SPImosi.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//	SPImosi.GPIO_PinConfig.GPIO_PinSpeed = 3;
//	SPImosi.GPIO_PinConfig.GPIO_PinAltFunMode = AF5;
//
//
	GPIO_Init(&Button);
//	GPIO_Init(&SPIclk);
//	GPIO_Init(&SPImiso);
//	GPIO_Init(&SPImosi);*/
//
//	GPIO_Handle_t UART4_TX;
//	GPIO_Handle_t UART4_RX;
//
//
//	UART4_TX.pGPIOx = GPIOA;
//	UART4_TX.GPIO_PinConfig.GPIO_PinNumber = 0;
//	UART4_TX.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
//	UART4_TX.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//	UART4_TX.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//	UART4_TX.GPIO_PinConfig.GPIO_PinSpeed = 3;
//	UART4_TX.GPIO_PinConfig.GPIO_PinAltFunMode = AF8;
//
//	UART4_RX.pGPIOx = GPIOA;
//	UART4_RX.GPIO_PinConfig.GPIO_PinNumber = 1;
//	UART4_RX.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
//	UART4_RX.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//	UART4_RX.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//	UART4_RX.GPIO_PinConfig.GPIO_PinSpeed = 3;
//	UART4_RX.GPIO_PinConfig.GPIO_PinAltFunMode = AF8;
//
//
//	GPIO_Init(&UART4_TX);
//	GPIO_Init(&UART4_RX);
//
//
}

void UART_GPIO_Init(void)
{
    GPIO_Handle_t UARTPins;

    UARTPins.pGPIOx = GPIOA;

    // TX -> PA0
    UARTPins.GPIO_PinConfig.GPIO_PinNumber = 0;
    UARTPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    UARTPins.GPIO_PinConfig.GPIO_PinAltFunMode = AF8;
    UARTPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    UARTPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    UARTPins.GPIO_PinConfig.GPIO_PinSpeed = 3;

    GPIO_Init(&UARTPins);

    // RX -> PA1
    UARTPins.GPIO_PinConfig.GPIO_PinNumber = 1;

    GPIO_Init(&UARTPins);
}



void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint32_t temp = 0;

    // 2. MODE
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <<(2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

        pGPIOHandle->pGPIOx->MODER &= ~(3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER |= temp;

        // 3. OUTPUT TYPE
        temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

        pGPIOHandle->pGPIOx->OTYPER &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        pGPIOHandle->pGPIOx->OTYPER |= temp;

        // 4. SPEED (MISSING in your code)
        temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed <<
               (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

        pGPIOHandle->pGPIOx->OSPEEDR &= ~(3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->OSPEEDR |= temp;

        // 5. PULL-UP/DOWN
        temp = pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl <<(2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

        pGPIOHandle->pGPIOx->PUPDR &= ~(3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->PUPDR |= temp;

        // 6. ALTERNATE FUNCTION
        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
        {
            uint32_t Temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
            uint32_t Temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;

            pGPIOHandle->pGPIOx->AFR[Temp1] &= ~(0xF << (4 * Temp2));
            pGPIOHandle->pGPIOx->AFR[Temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * Temp2));
        }
    }
    else
    {
        // 1. Configure as INPUT mode
        pGPIOHandle->pGPIOx->MODER &= ~(3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

        // 2. Enable SYSCFG clock
        SYSCNFG_PCLK_EN();

        // 3. Configure edge trigger

        if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
            EXTI->EXTI_FTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

            EXTI->EXTI_RTSR1 &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }

        else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
            EXTI->EXTI_RTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

            EXTI->EXTI_FTSR1 &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }

        else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
            EXTI->EXTI_FTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

            EXTI->EXTI_RTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }

        // 4. Configure SYSCFG EXTICR

        uint8_t Temp3 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;

        uint8_t Temp4 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;

        uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

        SYSCNFG->SYSCFG_EXTICR[Temp3] &= ~(0xF << (4 * Temp4));

        SYSCNFG->SYSCFG_EXTICR[Temp3] |= (portcode << (4 * Temp4));

        // 5. Unmask interrupt
        EXTI->EXTI_IMR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

    }

}


void GPIO_DeInit(GPIO_TypeDef *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_Reg_Reset();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_Reg_Reset();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_Reg_Reset();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOD_Reg_Reset();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOE_Reg_Reset();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOF_Reg_Reset();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOG_Reg_Reset();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOH_Reg_Reset();
	}
}

uint8_t GPIO_ReadFrom_Input_Pin(GPIO_TypeDef *pGPIOx, uint8_t Pin_Number)
{
	uint8_t val =0;

	val = pGPIOx->IDR>>Pin_Number & 0x01;

	return val;
}


uint8_t GPIO_ReadFrom_Input_Port(GPIO_TypeDef *pGPIOx)
{
	uint16_t val =0;

	val = (uint16_t)pGPIOx->IDR;

	return val;
}


void GPIO_Write_Output_Pin(GPIO_TypeDef *pGPIOx, uint8_t Pin_Number,uint8_t val)
{
	if(val == SET)
	{
		pGPIOx->ODR |= (0x01<<Pin_Number);
	}
	else
	{
		pGPIOx->ODR &= ~(0x01<<Pin_Number);
	}

}

void GPIO_Write_Output_Port(GPIO_TypeDef *pGPIOx, uint16_t val)
{
	pGPIOx->ODR = val;
}


void GPIO_Toggle_Pin(GPIO_TypeDef *pGPIOx, uint8_t Pin_Number)
{
	pGPIOx->ODR ^= (0x01<<Pin_Number);
}


void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
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

void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
    if(EXTI->EXTI_PR1 & (1 << PinNumber))
    {
        EXTI->EXTI_PR1 |= (1 << PinNumber);
    }
}

