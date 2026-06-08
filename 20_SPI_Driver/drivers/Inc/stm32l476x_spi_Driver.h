/*
 * stm32l476x_spi_Driver.h
 *
 *  Created on: May 7, 2026
 *      Author: lenovo
 */

#ifndef INC_STM32L476X_SPI_DRIVER_H_
#define INC_STM32L476X_SPI_DRIVER_H_

#define SPI_DEVICE_MODE_MASTER     1
#define SPI_DEVICE_MODE_SLAVE      0

#define SPI_BUS_CONFIG_FD          1
#define SPI_BUS_CONFIG_HD          2
#define SPI_BUS_CONFIG_SIMPLEX_RX  3

#define SPI_SCLK_SPEED_DIV2      0
#define SPI_SCLK_SPEED_DIV4      1
#define SPI_SCLK_SPEED_DIV8      2
#define SPI_SCLK_SPEED_DIV16     3
#define SPI_SCLK_SPEED_DIV32     4
#define SPI_SCLK_SPEED_DIV64     5
#define SPI_SCLK_SPEED_DIV128    6
#define SPI_SCLK_SPEED_DIV256    7

#define SPI_DFF_8BITS      0
#define SPI_DFF_16BITS     1

#define SPI_CPOL_LOW       0
#define SPI_CPOL_HIGH      1

#define SPI_CPHA_LOW       0
#define SPI_CPHA_HIGH      1

#define SPI_SSM_DI         0
#define SPI_SSM_EN         1

#define SPI_READY      0
#define SPI_BUSY_IN_RX 1
#define SPI_BUSY_IN_TX 2

#define SPI_CR2_RXNEIE   6
#define SPI_CR2_TXEIE    7
#define SPI_CR2_ERRIE    5




typedef struct
{
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DFF;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;

}SPI_Config_t;

typedef struct
{
    SPI_TypeDef *pSPIx;
    SPI_Config_t SPIConfig;
    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;

    uint32_t TxLen;
    uint32_t RxLen;

    uint8_t TxState;
    uint8_t RxState;

}SPI_Handle_t;


#endif /* INC_STM32L476X_SPI_DRIVER_H_ */
