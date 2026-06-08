
#ifndef INC_STM32L476XX_I2C_DRIVERS_H_
#define INC_STM32L476XX_I2C_DRIVERS_H_


#include "stm32l476xx.h"

typedef struct {
	uint32_t SCLSpeed;
	uint8_t DeviceAddress;
	uint8_t ACKControl;
	uint16_t FM_DutyCycle;

} I2C_Config_t;

typedef struct
{
    I2C_RegDef_t *pI2Cx;

    I2C_Config_t I2C_Config;

    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;

    uint32_t TxLen;
    uint32_t RxLen;

    uint8_t TxRxState;
    uint8_t DevAddr;

}I2C_Handle_t;

#define I2C_ISR_TXIS      (1 << 1)
#define I2C_ISR_RXNE      (1 << 2)
#define I2C_ISR_ADDR      (1 << 3)
#define I2C_ISR_NACKF     (1 << 4)
#define I2C_ISR_STOPF     (1 << 5)
#define I2C_ISR_TC        (1 << 6)
#define I2C_ISR_BERR      (1 << 8)
#define I2C_ISR_ARLO      (1 << 9)
#define I2C_ISR_OVR       (1 << 10)
#define I2C_ISR_BUSY      (1 << 15)


//SPEED MODES
#define I2C_SCL_SPEED_SM            100000U      /* Standard mode 100KHz */
#define I2C_SCL_SPEED_FM4K          400000U      /* Fast mode 400KHz */

//ACK CONTROL
#define I2C_ACK_DISABLE             0
#define I2C_ACK_ENABLE              1

//FM_DUTY
#define I2C_FM_DUTY_2				0
#define I2C_FM_DUTY_16_9			1


#define I2C_CR1_TXIE_Pos     1
#define I2C_CR1_RXIE_Pos      2
#define I2C_CR1_ADDRIE_Pos    3
#define I2C_CR1_NACKIE_Pos    4
#define I2C_CR1_STOPIE_Pos    5
#define I2C_CR1_TCIE_Pos      6
#define I2C_CR1_ERRIE_Pos     7


#define I2C_ICR_BERRCF    (1 << 8)
#define I2C_ICR_ARLOCF    (1 << 9)
#define I2C_ICR_OVRCF     (1 << 10)

#define I2C_READY      0
#define I2C_BUSY_IN_RX 1
#define I2C_BUSY_IN_TX 2

#define I2C_EV_TX_CMPLT     1
#define I2C_EV_RX_CMPLT     2
#define I2C_ERROR_NACK      3

//FUNCTIONS

//void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

void I2C_Init(I2C_Handle_t *pI2CHandle);

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer,
		uint32_t Len, uint8_t SlaveAddr);

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer,
		uint32_t Len, uint8_t SlaveAddr);

void I2C_ScanBus(I2C_Handle_t *pI2CHandle);

void delay_ms(uint32_t time);

void I2C_MasterWriteRead(I2C_Handle_t *pI2CHandle, uint8_t SlaveAddr,
		uint8_t *pTxBuffer, uint32_t TxLen, uint8_t *pRxBuffer, uint32_t RxLen);


#endif /* INC_STM32L476XX_I2C_DRIVERS_H_ */
//stm32l476xx_i2c_drivers.h
//Displaying main.c
