

#include "stm32_l476xx_I2c_Driver.h"
#include "stm32l476xx.h"

void delay_ms(uint32_t time) {
	for (uint32_t i = 0; i < time * 4000; i++) {
		__asm("nop");
	}
}

//void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi) {
//	if (EnOrDi == ENABLE) {
//		if (pI2Cx == I2C1) {
//			I2C1_CLK_ENABLE();
//		} else if (pI2Cx == I2C2) {
//			I2C2_CLK_ENABLE();
//		} else if (pI2Cx == I2C3) {
//			I2C3_CLK_ENABLE();
//		}
//	} else {
//
//	}
//}

void I2C_Init(I2C_Handle_t *pI2CHandle) {
//	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	/* Disable peripheral */
	pI2CHandle->pI2Cx->CR1 &= ~(1 << 0);

	/*
	 * TIMINGR value for:
	 * SYSCLK = 4MHz
	 * I2C = 100kHz
	 */
	pI2CHandle->pI2Cx->TIMINGR = 0x00707CBB;

	/* Enable peripheral */
	pI2CHandle->pI2Cx->CR1 |= (1 << 0);

	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_TXIE_Pos);
	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_RXIE_Pos);
	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_STOPIE_Pos);
	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_TCIE_Pos);
	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_ERRIE_Pos);
	pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_NACKIE_Pos);


}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer,
		uint32_t Len, uint8_t SlaveAddr) {
	uint32_t timeout;

	/* Clear flags */
	pI2CHandle->pI2Cx->ICR |= I2C_ICR_STOPCF | I2C_ICR_NACKCF;

	/* Clear CR2 */
	pI2CHandle->pI2Cx->CR2 = 0;

	/* Slave address */
	pI2CHandle->pI2Cx->CR2 |= (SlaveAddr << 1);

	/* Number of bytes */
	pI2CHandle->pI2Cx->CR2 |= (Len << I2C_CR2_NBYTES_Pos);

	/* AUTOEND */
	pI2CHandle->pI2Cx->CR2 |=  (1 << 25);

	/* START */
	pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_START_Pos);

	while (Len > 0) {
		timeout = 100000;

		while (!(pI2CHandle->pI2Cx->ISR &
		I2C_ISR_TXIS)) {
			if (pI2CHandle->pI2Cx->ISR &
			I2C_ISR_NACKF) {
				return;
			}

			if (--timeout == 0) {
				return;
			}
		}

		pI2CHandle->pI2Cx->TXDR = *pTxBuffer;

		pTxBuffer++;
		Len--;
	}

	while (!(pI2CHandle->pI2Cx->ISR &
	I2C_ISR_STOPF)) {
		;
	}

	/* Clear STOP */
	pI2CHandle->pI2Cx->ICR |= I2C_ICR_STOPCF;

	/* Reset CR2 */
	pI2CHandle->pI2Cx->CR2 = 0;
}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer,
		uint32_t Len, uint8_t SlaveAddr) {
	uint32_t timeout;

	/* Clear flags */
	pI2CHandle->pI2Cx->ICR |= I2C_ICR_STOPCF | I2C_ICR_NACKCF;

	/* Clear CR2 */
	pI2CHandle->pI2Cx->CR2 = 0;

	/* Slave address */
	pI2CHandle->pI2Cx->CR2 |= (SlaveAddr << 1);

	/* READ mode */
	pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_RD_WRN_Pos);

	/* Number of bytes */
	pI2CHandle->pI2Cx->CR2 |= (Len << I2C_CR2_NBYTES_Pos);

	/* AUTOEND */
	pI2CHandle->pI2Cx->CR2 |=  (1 << 25);

	/* START */
	pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_START_Pos);

	while (Len > 0) {
		timeout = 100000;

		/* Wait until RXNE */
		while (!(pI2CHandle->pI2Cx->ISR &
		I2C_ISR_RXNE)) {
			/* NACK */
			if (pI2CHandle->pI2Cx->ISR &
			I2C_ISR_NACKF) {
				return;
			}

			/* Timeout */
			if (--timeout == 0) {
				return;
			}
		}

		/* Read data */
		*pRxBuffer = pI2CHandle->pI2Cx->RXDR;

		pRxBuffer++;
		Len--;
	}

	timeout = 100000;

	/* Wait STOP */
	while (!(pI2CHandle->pI2Cx->ISR &
	I2C_ISR_STOPF)) {
		if (--timeout == 0) {
			return;
		}
	}

	/* Clear STOP */
	pI2CHandle->pI2Cx->ICR |= I2C_ICR_STOPCF;

	/* Reset CR2 */
	pI2CHandle->pI2Cx->CR2 = 0;
}

void I2C_ScanBus(I2C_Handle_t *pI2CHandle) {
	//printf("Scanning I2C Bus...\r\n");

	for (uint8_t addr = 1; addr < 128; addr++) {
		/* Clear flags */
		pI2CHandle->pI2Cx->ICR |=
		I2C_ICR_STOPCF |
		I2C_ICR_NACKCF;

		/* Clear CR2 */
		pI2CHandle->pI2Cx->CR2 = 0;

		/* Slave address */
		pI2CHandle->pI2Cx->CR2 |= (addr << 1);

		/* 1 byte */
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_NBYTES_Pos);

		/* AUTOEND */
		pI2CHandle->pI2Cx->CR2 |=  (1 << 25);

		/* START */
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_START_Pos);

		uint32_t timeout = 10000;

		while (1) {
			uint32_t isr = pI2CHandle->pI2Cx->ISR;

			/* ADDRESS ACKED */
			if (isr & I2C_ISR_TXIS) {
				//printf("Device Found at 0x%X\r\n", addr);

				/* send dummy byte */
				pI2CHandle->pI2Cx->TXDR = 0x00;

				break;
			}

			/* NACK */
			if (isr & I2C_ISR_NACKF) {
				break;
			}

			if (--timeout == 0) {
				break;
			}
		}

		/* Wait STOP */
		timeout = 10000;

		while (!(pI2CHandle->pI2Cx->ISR &
		I2C_ISR_STOPF)) {
			if (--timeout == 0)
				break;
		}

		/* Clear flags */
		pI2CHandle->pI2Cx->ICR |=
		I2C_ICR_STOPCF |
		I2C_ICR_NACKCF;

		delay_ms(2);
	}

}

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,
                             uint8_t *pTxBuffer,
                             uint32_t Len,
                             uint8_t SlaveAddr)
{
    if(pI2CHandle->TxRxState != I2C_READY)
    {
        return 0;
    }

    pI2CHandle->pTxBuffer = pTxBuffer;

    pI2CHandle->TxLen = Len;

    pI2CHandle->TxRxState = I2C_BUSY_IN_TX;

    pI2CHandle->DevAddr = SlaveAddr;

    /* Clear flags */
    pI2CHandle->pI2Cx->ICR |=
            I2C_ICR_STOPCF |
            I2C_ICR_NACKCF;

    /* Clear CR2 */
    pI2CHandle->pI2Cx->CR2 = 0;

    /* Slave address */
    pI2CHandle->pI2Cx->CR2 |=
            (SlaveAddr << 1);

    /* Number of bytes */
    pI2CHandle->pI2Cx->CR2 |=
            (Len << I2C_CR2_NBYTES_Pos);

    /* AUTOEND */
    pI2CHandle->pI2Cx->CR2 |=
            (1 << 25);

    /* ================= ENABLE INTERRUPTS ================= */

    pI2CHandle->pI2Cx->CR1 |=
            (1 << I2C_CR1_TXIE_Pos)   |
            (1 << I2C_CR1_STOPIE_Pos) |
            (1 << I2C_CR1_NACKIE_Pos);

    /* ================= GENERATE START ================= */

    pI2CHandle->pI2Cx->CR2 |=
            (1 << I2C_CR2_START_Pos);

    return 1;
}
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,
                                uint8_t *pRxBuffer,
                                uint32_t Len,
                                uint8_t SlaveAddr)
{
    uint8_t busystate = pI2CHandle->TxRxState;

    if(busystate != I2C_BUSY_IN_TX &&
       busystate != I2C_BUSY_IN_RX)
    {
        pI2CHandle->pRxBuffer = pRxBuffer;

        pI2CHandle->RxLen = Len;

        pI2CHandle->TxRxState = I2C_BUSY_IN_RX;

        pI2CHandle->DevAddr = SlaveAddr;

        /* Clear flags */
        pI2CHandle->pI2Cx->ICR |=
                I2C_ICR_STOPCF |
                I2C_ICR_NACKCF;

        /* Clear CR2 */
        pI2CHandle->pI2Cx->CR2 = 0;

        /* Slave address */
        pI2CHandle->pI2Cx->CR2 |=
                (SlaveAddr << 1);

        /* READ mode */
        pI2CHandle->pI2Cx->CR2 |=
                (1 << I2C_CR2_RD_WRN_Pos);

        /* Number of bytes */
        pI2CHandle->pI2Cx->CR2 |=
                (Len << I2C_CR2_NBYTES_Pos);

        /* AUTOEND */
        pI2CHandle->pI2Cx->CR2 |=
                (1 << 25);

        /* START */
        pI2CHandle->pI2Cx->CR2 |=
                (1 << I2C_CR2_START_Pos);

        /* Enable interrupts */
        pI2CHandle->pI2Cx->CR1 |=
                (1 << I2C_CR1_RXIE_Pos)   |
                (1 << I2C_CR1_STOPIE_Pos) |
                (1 << I2C_CR1_NACKIE_Pos);
    }

    return busystate;
}

///*void I2C_EV_IRQHandling(I2C_Handle_t *pHandle)
//{
//    uint32_t temp;
//
//    /* ================= TX ================= */
//
//    temp = pHandle->pI2Cx->ISR & I2C_ISR_TXIS;
//
//    if(temp)
//    {
//        if(pHandle->TxRxState == I2C_BUSY_IN_TX)
//        {
//            if(pHandle->TxLen > 0)
//            {
//                pHandle->pI2Cx->TXDR =
//                        *(pHandle->pTxBuffer);
//
//                pHandle->pTxBuffer++;
//
//                pHandle->TxLen--;
//            }
//        }
//    }
//
//    /* ================= RX ================= */
//
//    temp = pHandle->pI2Cx->ISR & I2C_ISR_RXNE;
//
//    if(temp)
//    {
//        if(pHandle->TxRxState == I2C_BUSY_IN_RX)
//        {
//            if(pHandle->RxLen > 0)
//            {
//                *(pHandle->pRxBuffer) =
//                        pHandle->pI2Cx->RXDR;
//
//                pHandle->pRxBuffer++;
//
//                pHandle->RxLen--;
//            }
//        }
//    }
//
//    /* ================= STOP ================= */
//
//    temp = pHandle->pI2Cx->ISR & I2C_ISR_STOPF;
//
//    if(temp)
//    {
//
//        /* Clear STOP */
//        pHandle->pI2Cx->ICR |=
//                I2C_ICR_STOPCF;
//
//        /* Disable interrupts */
//        pHandle->pI2Cx->CR1 &=
//                ~((1 << I2C_CR1_TXIE_Pos)   |
//                  (1 << I2C_CR1_RXIE_Pos)   |
//                  (1 << I2C_CR1_STOPIE_Pos) |
//                  (1 << I2C_CR1_NACKIE_Pos));
//
//        /* Reset state */
//        pHandle->TxRxState = I2C_READY;
//    }
//
//    /* ================= NACK ================= */
//
//    temp = pHandle->pI2Cx->ISR & I2C_ISR_NACKF;
//
//    if(temp)
//    {
//        /* Clear NACK */
//        pHandle->pI2Cx->ICR |=
//                I2C_ICR_NACKCF;
//
//        pHandle->TxRxState = I2C_READY;
//    }
//}

void I2C_EV_IRQHandling(I2C_Handle_t *pHandle)
{
    uint32_t temp;

    /* =========================================================
     * TXIS : Transmit interrupt
     * ========================================================= */
    temp = pHandle->pI2Cx->ISR & I2C_ISR_TXIS;

    if(temp)
    {
        if(pHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            if(pHandle->TxLen > 0)
            {
                /* Send byte */
                pHandle->pI2Cx->TXDR =
                        *(pHandle->pTxBuffer);

                /* Move buffer pointer */
                pHandle->pTxBuffer++;

                /* Decrement length */
                pHandle->TxLen--;
            }
        }
    }

    /* =========================================================
     * RXNE : Receive interrupt
     * ========================================================= */
    temp = pHandle->pI2Cx->ISR & I2C_ISR_RXNE;

    if(temp)
    {
        if(pHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            if(pHandle->RxLen > 0)
            {
                /* Read received byte */
                *(pHandle->pRxBuffer) =
                        pHandle->pI2Cx->RXDR;

                /* Move buffer pointer */
                pHandle->pRxBuffer++;

                /* Decrement remaining bytes */
                pHandle->RxLen--;
            }
        }
    }

    /* =========================================================
     * TC : Transfer complete
     * ========================================================= */
    temp = pHandle->pI2Cx->ISR & I2C_ISR_TC;

    if(temp)
    {
        /*
         * TC becomes SET when:
         * - all bytes transferred
         * - AUTOEND = 0
         * - STOP not generated yet
         *
         * Useful for repeated START.
         */

    }

    /* =========================================================
     * STOPF : STOP detected
     * ========================================================= */
    temp = pHandle->pI2Cx->ISR & I2C_ISR_STOPF;

    if(temp)
    {
        /* Clear STOP flag */
        pHandle->pI2Cx->ICR |= I2C_ICR_STOPCF;

        /* Disable interrupt sources */
        pHandle->pI2Cx->CR1 &=
                ~((1 << I2C_CR1_TXIE_Pos)   |
                  (1 << I2C_CR1_RXIE_Pos)   |
                  (1 << I2C_CR1_STOPIE_Pos) |
                  (1 << I2C_CR1_NACKIE_Pos));

        /* Clear CR2 */
        pHandle->pI2Cx->CR2 = 0;

        /* ================= TX COMPLETE ================= */

        if(pHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            pHandle->TxRxState = I2C_READY;

            I2C_ApplicationEventCallback(
                    pHandle,
                    I2C_EV_TX_CMPLT);
        }

        /* ================= RX COMPLETE ================= */

        else if(pHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            pHandle->TxRxState = I2C_READY;

            I2C_ApplicationEventCallback(
                    pHandle,
                    I2C_EV_RX_CMPLT);
        }
    }

    /* =========================================================
     * NACKF : NACK received
     * ========================================================= */
    temp = pHandle->pI2Cx->ISR & I2C_ISR_NACKF;

    if(temp)
    {
        /* Clear NACK flag */
        pHandle->pI2Cx->ICR |= I2C_ICR_NACKCF;

        /* Generate STOP */
        pHandle->pI2Cx->CR2 |=
                (1 << I2C_CR2_STOP_Pos);

        /* Reset state */
        pHandle->TxRxState = I2C_READY;
    }
}

void I2C_ER_IRQHandling(I2C_Handle_t *pHandle)
{
    uint32_t temp1;

    temp1 = pHandle->pI2Cx->ISR;

    if(temp1 & I2C_ISR_BERR)
    {
        pHandle->pI2Cx->ICR |= I2C_ICR_BERRCF;
    }

    if(temp1 & I2C_ISR_ARLO)
    {
        pHandle->pI2Cx->ICR |= I2C_ICR_ARLOCF;
    }

    if(temp1 & I2C_ISR_OVR)
    {
        pHandle->pI2Cx->ICR |= I2C_ICR_OVRCF;
    }
}


void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
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

void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}
