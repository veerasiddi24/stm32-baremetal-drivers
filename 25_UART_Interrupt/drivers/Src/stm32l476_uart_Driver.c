/*
 * stm32l476_uart_Driver.c
 *
 *  Created on: May 11, 2026
 *      Author: lenovo
 */
#include <stddef.h>
#include <stdint.h>
#include "stm32l476xx.h"
#include "stm32l476xx_gpio_Driver.h"
#include "stm32l476xx_uart_Driver.h"



UART_Handle_t UART4Handle;

void UART_Config()
{


	UART4Handle.pUSARTx = UART4;

	UART4Handle.UART_Config.UART_BaudRate = 115200;
	UART4Handle.UART_Config.UART_Mode = UART_MODE_TXRX;

	UART_Init(&UART4Handle);
}

void UART_Init(UART_Handle_t *pUARTHandle)
{
    uint32_t tempreg = 0;

    /******** Enable TX ********/
    if(pUARTHandle->UART_Config.UART_Mode ==
            UART_MODE_ONLY_TX)
    {
        tempreg |= (1 << USART_CR1_TE);
    }

    /******** Enable RX ********/
    else if(pUARTHandle->UART_Config.UART_Mode ==
            UART_MODE_ONLY_RX)
    {
        tempreg |= (1 << USART_CR1_RE);
    }

    /******** Enable TXRX ********/
    else if(pUARTHandle->UART_Config.UART_Mode ==
            UART_MODE_TXRX)
    {
        tempreg |= (1 << USART_CR1_TE);
        tempreg |= (1 << USART_CR1_RE);
    }

    pUARTHandle->pUSARTx->CR1 = tempreg;

    /******** Baudrate ********/
    pUARTHandle->pUSARTx->BRR = 0x8b;

    /******** UART Enable ********/
    pUARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_UE);
}


void UART_SendData(USART_TypeDef *pUSARTx,
                   uint8_t *pTxBuffer,
                   uint32_t Len)
{
    for(uint32_t i = 0; i < Len; i++)
    {
        while(!(pUSARTx->ISR & (1 << USART_ISR_TXE)));

        pUSARTx->TDR = pTxBuffer[i];
    }

    while(!(pUSARTx->ISR & (1 << USART_ISR_TC)));
}


void UART_ReceiveData(USART_TypeDef *pUSARTx,
                      uint8_t *pRxBuffer,
                      uint32_t Len)
{
    for(uint32_t i = 0; i < Len; i++)
    {
        // Check Overrun Error
        if(pUSARTx->ISR & (1 << USART_ISR_ORE))
        {
            // Clear ORE flag
            pUSARTx->ICR = (1 << USART_ICR_ORECF);
        }

        // Wait until data received
        while(!(pUSARTx->ISR & (1 << USART_ISR_RXNE)));

        // Read data
        pRxBuffer[i] = pUSARTx->RDR;
    }
}

uint8_t UART_SendDataIT(UART_Handle_t *pUARTHandle,
                        uint8_t *pTxBuffer,
                        uint32_t Len)
{
    uint8_t state = pUARTHandle->TxBusyState;

    if(state != UART_BUSY_IN_TX)
    {
        pUARTHandle->pTxBuffer = pTxBuffer;
        pUARTHandle->TxLen = Len;

        pUARTHandle->TxBusyState = UART_BUSY_IN_TX;

        // Enable TXE interrupt
        pUARTHandle->pUSARTx->CR1 |=
                (1 << USART_CR1_TXEIE);
    }

    return state;
}

uint8_t UART_ReceiveDataIT(UART_Handle_t *pUARTHandle,
                           uint8_t *pRxBuffer,
                           uint32_t Len)
{
    uint8_t state = pUARTHandle->RxBusyState;

    if(state != UART_BUSY_IN_RX)
    {
        pUARTHandle->pRxBuffer = pRxBuffer;
        pUARTHandle->RxLen = Len;

        pUARTHandle->RxBusyState = UART_BUSY_IN_RX;

        // Enable RXNE interrupt
        pUARTHandle->pUSARTx->CR1 |=
                (1 << USART_CR1_RXNEIE);
    }

    return state;
}

void UART_IRQHandling(UART_Handle_t *pHandle)
{
    uint32_t temp1,temp2;

    // TXE
    temp1 = pHandle->pUSARTx->ISR &
            (1 << USART_ISR_TXE);

    temp2 = pHandle->pUSARTx->CR1 &
            (1 << USART_CR1_TXEIE);

    if(temp1 && temp2)
    {
        UART_TXE_Interrupt_Handle(pHandle);
    }

    // RXNE
    temp1 = pHandle->pUSARTx->ISR &
            (1 << USART_ISR_RXNE);

    temp2 = pHandle->pUSARTx->CR1 &
            (1 << USART_CR1_RXNEIE);

    if(temp1 && temp2)
    {
        UART_RXNE_Interrupt_Handle(pHandle);
    }
}

void UART_TXE_Interrupt_Handle(UART_Handle_t *pHandle)
{
    pHandle->pUSARTx->TDR =
            *(pHandle->pTxBuffer);

    pHandle->pTxBuffer++;
    pHandle->TxLen--;

    if(pHandle->TxLen == 0)
    {
        pHandle->pUSARTx->CR1 &=
                ~(1 << USART_CR1_TXEIE);

        pHandle->TxBusyState =
                UART_READY;

        pHandle->pTxBuffer = NULL;
    }
}

void UART_RXNE_Interrupt_Handle(UART_Handle_t *pHandle)
{
    *(pHandle->pRxBuffer) =
            pHandle->pUSARTx->RDR;

    pHandle->pRxBuffer++;
    pHandle->RxLen--;

    if(pHandle->RxLen == 0)
    {
        pHandle->pUSARTx->CR1 &=
                ~(1 << USART_CR1_RXNEIE);

        pHandle->RxBusyState =
                UART_READY;

        pHandle->pRxBuffer = NULL;
    }
}

void UART_IRQInterruptConfig(uint8_t IRQNumber,
                             uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 &&
                IRQNumber < 64)
        {
            *NVIC_ISER1 |=
                    (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 &&
                IRQNumber < 96)
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
        else if(IRQNumber > 31 &&
                IRQNumber < 64)
        {
            *NVIC_ICER1 |=
                    (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 &&
                IRQNumber < 96)
        {
            *NVIC_ICER2 |=
                    (1 << (IRQNumber % 64));
        }
    }
}

void UART_IRQPriorityConfig(uint8_t IRQNumber,
                            uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;

    uint8_t iprx_section =
            IRQNumber % 4;

    uint8_t shift_amount =
            (8 * iprx_section) +
            (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_PR_BASE_ADDR + iprx) |=
            (IRQPriority << shift_amount);
}
