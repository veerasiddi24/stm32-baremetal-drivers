/*
 * stm32l476xx_uart_Driver.h
 *
 *  Created on: May 11, 2026
 *      Author: lenovo
 */

#ifndef INC_STM32L476XX_UART_DRIVER_H_
#define INC_STM32L476XX_UART_DRIVER_H_
#define UART_MODE_ONLY_TX    0
#define UART_MODE_ONLY_RX    1
#define UART_MODE_TXRX       2

#define UART_STD_BAUD_9600     9600
#define UART_STD_BAUD_115200   115200


#define UART_READY       0
#define UART_BUSY_IN_RX  1
#define UART_BUSY_IN_TX  2


typedef struct
{
    uint32_t UART_BaudRate;
    uint8_t UART_Mode;

}UART_Config_t;

typedef struct
{
    USART_TypeDef *pUSARTx;
    UART_Config_t UART_Config;

    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;

    uint32_t TxLen;
    uint32_t RxLen;

    uint8_t TxBusyState;
    uint8_t RxBusyState;

}UART_Handle_t;


#endif /* INC_STM32L476XX_UART_DRIVER_H_ */
