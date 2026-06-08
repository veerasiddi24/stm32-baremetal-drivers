/*
 * stm32l476x_spi_Driver.c
 *
 *  Created on: May 7, 2026
 *      Author: lenovo
 */
#include <stddef.h>
#include "stm32l476xx.h"
#include "stm32l476x_spi_Driver.h"
SPI_Handle_t SPI1Handle;

void SPI_PCLK_EN(SPI_TypeDef* pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}

	}
	else
	{

	}
}
void SPI_Config()
{
	SPI1Handle.pSPIx = SPI1;
	SPI1Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI1Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI1Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV16;
	SPI1Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI1Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI1Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI1Handle.SPIConfig.SPI_SSM = SPI_SSM_EN;
	SPI1Handle.TxState = SPI_READY;
	SPI1Handle.RxState = SPI_READY;

	SPI_Init(&SPI1Handle);
}

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    uint32_t tempreg = 0;

    /*******************
     * 1. Configure device mode
     *******************/
    tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode
               << SPI_CR1_MSTR;

    /*******************
     * 2. Configure bus config
     *******************/

    if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
        // Full duplex

        tempreg &= ~(1 << SPI_CR1_BIDIMODE);
        tempreg &= ~(1 << SPI_CR1_RXONLY);

    }
    else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
        // Half duplex

        tempreg |= (1 << SPI_CR1_BIDIMODE);
    }
    else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RX)
    {
        // Simplex RX

        tempreg &= ~(1 << SPI_CR1_BIDIMODE);
        tempreg |=  (1 << SPI_CR1_RXONLY);
    }

    /*******************
     * 3. Configure clock speed
     *******************/

    tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed
               << SPI_CR1_BR;

    /*******************
     * 4. Configure DFF
     *******************/

//    tempreg |= pSPIHandle->SPIConfig.SPI_DFF
//               << SPI_CR1_DFF;

    pSPIHandle->pSPIx->SPIx_CR2 &= ~(0xF << 8);
    pSPIHandle->pSPIx->SPIx_CR2 |= (7 << 8);

    pSPIHandle->pSPIx->SPIx_CR2 |= (1 << 12);

    /*******************
     * 5. Configure CPOLs
     *******************/

    tempreg |= pSPIHandle->SPIConfig.SPI_CPOL
               << SPI_CR1_CPOL;

    /*******************
     * 6. Configure CPHA
     *******************/

    tempreg |= pSPIHandle->SPIConfig.SPI_CPHA
               << SPI_CR1_CPHA;

    /*******************
     * 7. Configure SSM
     *******************/

    tempreg |= pSPIHandle->SPIConfig.SPI_SSM
               << SPI_CR1_SSM;

    if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN)
    {
        tempreg |= (1 << SPI_CR1_SSI);
    }

    /*******************
     * Write to CR1
     *******************/


    pSPIHandle->pSPIx->SPIx_CR1 = tempreg;
}

void SPI_PeripheralControl(SPI_TypeDef *pSPIx, uint8_t EnOrDi)
{
    if(EnOrDi == ENABLE)
    {
        pSPIx->SPIx_CR1 |= (1 << SPI_CR1_SPE);
    }
    else
    {
        pSPIx->SPIx_CR1 &= ~(1 << SPI_CR1_SPE);
    }
}

//void SPI_SendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
//{
//    while(Len > 0)
//    {
//        // Wait until TXE is set
//        while(!(pSPIx->SPIx_SR & (1 << 1)));
//
//            // 8-bit
//        	*((volatile uint8_t*)&pSPIx->SPIx_DR) = *pTxBuffer;
//
//            Len--;
//            pTxBuffer++;
//
//
//
//    }
//
//    while(pSPIx->SPIx_SR & (1 << 7));
//}

void SPI_SendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
    volatile uint8_t dummy;

    while(Len > 0)
    {
        // Wait until TXE=1
        while(!(pSPIx->SPIx_SR & (1 << 1)));

        // Write data
        *((volatile uint8_t*)&pSPIx->SPIx_DR) = *pTxBuffer;

        // Wait until RXNE=1
        while(!(pSPIx->SPIx_SR & (1 << 0)));

        // Read and discard received byte
        dummy = *((volatile uint8_t*)&pSPIx->SPIx_DR);

        pTxBuffer++;
        Len--;
    }

    // Wait until not busy
    while(pSPIx->SPIx_SR & (1 << 7));
}

//void SPI_SendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
//{
//    while(Len > 0)
//    {
//        // Wait until TXE=1
//        while(!(pSPIx->SR & SPI_SR_TXE));
//
//        // Write data
//        *((volatile uint8_t*)&pSPIx->DR) = *pTxBuffer;
//
//        // Wait until RXNE=1 (data shifted in)
//        while(!(pSPIx->SR & SPI_SR_RXNE));
//
//        // Read and discard received byte
//        volatile uint8_t dummy = pSPIx->DR;
//
//        pTxBuffer++;
//        Len--;
//    }
//
//    // Wait until BSY=0 (last byte fully shifted out)
//    while(pSPIx->SR & SPI_SR_BSY);
//}


//void SPI_ReceiveData(SPI_TypeDef *pSPIx,
//                     uint8_t *pRxBuffer,
//                     uint32_t Len)
//{
//    while(Len > 0)
//    {
//        while(!(pSPIx->SPIx_SR & (1 << 1)));
//
//        *((volatile uint8_t*)&pSPIx->SPIx_DR) = 0xff;
//
//        // Wait until RXNE becomes set
//        while(!(pSPIx->SPIx_SR & (1 << 0)));
//
//        // 16-bit DFF
////        if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
////        {
////            *((uint16_t*)pRxBuffer) = pSPIx->DR;
////
////            Len--;
////            Len--;
////
////            (uint16_t*)pRxBuffer++;
////        }
//
//        {
//            // 8-bit
//            *pRxBuffer = *((volatile uint8_t*)&pSPIx->SPIx_DR);
//
//            Len--;
//            pRxBuffer++;
//        }
//    }
//}

//void SPI_ReceiveData(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
//{
//    while (Len > 0)
//    {
//        // Wait until TXE is set
//        while (!(pSPIx->SPIx_SR & (1 << 1)));
//
//        // Send dummy byte to generate clock
//        pSPIx->SPIx_DR = 0xFF;
//
//        // Wait until RXNE is set
//        while (!(pSPIx->SPIx_SR & (1 << 0)));
//
//        // Read received byte (mask to 8 bits)
//        *pRxBuffer = (uint8_t)(pSPIx->SPIx_DR & 0xFF);
//
//        pRxBuffer++;
//        Len--;
//    }
//}

void SPI_ReceiveData(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
    while(Len > 0)
    {
        // Wait TXE
        while(!(pSPIx->SPIx_SR & (1 << 1)));

        // Send dummy byte
        *((volatile uint8_t*)&pSPIx->SPIx_DR) = 0xFF;

        // Wait RXNE
        while(!(pSPIx->SPIx_SR & (1 << 0)));

        // Read byte
        *pRxBuffer = *((volatile uint8_t*)&pSPIx->SPIx_DR);

        pRxBuffer++;
        Len--;
    }

    while(pSPIx->SPIx_SR & (1 << 7));
}

void SPI_SendReceiveData(SPI_TypeDef *pSPIx,
                         uint8_t *TxBuffer,
                         uint8_t *RxBuffer,
                         uint32_t Len)
{
    while(Len > 0)
    {
        // 1. Wait until TX buffer empty
        while(!(pSPIx->SPIx_SR & (1 << 1)));

        // 2. Write data into DR
        *((volatile uint8_t*)&pSPIx->SPIx_DR) = *TxBuffer;

        // 3. Wait until RX buffer not empty
        while(!(pSPIx->SPIx_SR & (1 << 0)));

        // 4. Read received data
        *RxBuffer = *((volatile uint8_t*)&pSPIx->SPIx_DR);

        // 5. Move buffers
        TxBuffer++;
        RxBuffer++;

        // 6. Decrease length
        Len--;
    }

    while(pSPIx->SPIx_SR & (1 << 7));
}


void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
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

void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,
                       uint8_t *pTxBuffer,
                       uint32_t Len)
{
    uint8_t state = pSPIHandle->TxState;

    if(state != SPI_BUSY_IN_TX)
    {
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->TxLen = Len;

        pSPIHandle->TxState = SPI_BUSY_IN_TX;

        // Enable TXE interrupt
        pSPIHandle->pSPIx->SPIx_CR2 |= (1 << SPI_CR2_TXEIE);
    }

    return state;
}


uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle,
                          uint8_t *pRxBuffer,
                          uint32_t Len)
{
    uint8_t state = pSPIHandle->RxState;

    if(state != SPI_BUSY_IN_RX)
    {
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->RxLen = Len;

        pSPIHandle->RxState = SPI_BUSY_IN_RX;

        // Enable RXNE interrupt
        pSPIHandle->pSPIx->SPIx_CR2 |= (1 << SPI_CR2_RXNEIE);
    }

    return state;
}


void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
    uint8_t temp1,temp2;

    // TXE
    temp1 = pHandle->pSPIx->SPIx_SR & (1 << 1);
    temp2 = pHandle->pSPIx->SPIx_CR2 & (1 << SPI_CR2_TXEIE);

    if(temp1 && temp2)
    {
        SPI_TXE_Interrupt_Handle(pHandle);
    }

    // RXNE
    temp1 = pHandle->pSPIx->SPIx_SR & (1 << 0);
    temp2 = pHandle->pSPIx->SPIx_CR2 & (1 << SPI_CR2_RXNEIE);

    if(temp1 && temp2)
    {
        SPI_RXNE_Interrupt_Handle(pHandle);
    }
}

void SPI_TXE_Interrupt_Handle(SPI_Handle_t *pHandle)
{
	*((volatile uint8_t*)&pHandle->pSPIx->SPIx_DR) = *(pHandle->pTxBuffer);
    pHandle->pTxBuffer++;
    pHandle->TxLen--;

    if(pHandle->TxLen == 0)
    {
        // Close TX
        pHandle->pSPIx->SPIx_CR2 &= ~(1 << SPI_CR2_TXEIE);

        pHandle->TxState = SPI_READY;
        pHandle->pTxBuffer = NULL;
    }
}

void SPI_RXNE_Interrupt_Handle(SPI_Handle_t *pHandle)
{
	*(pHandle->pRxBuffer) = *((volatile uint8_t*)&pHandle->pSPIx->SPIx_DR);

    pHandle->pRxBuffer++;
    pHandle->RxLen--;

    if(pHandle->RxLen == 0)
    {
        pHandle->pSPIx->SPIx_CR2 &= ~(1 << SPI_CR2_RXNEIE);

        pHandle->RxState = SPI_READY;
        pHandle->pRxBuffer = NULL;
    }
}

