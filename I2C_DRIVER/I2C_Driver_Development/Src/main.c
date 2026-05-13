
#include "stm32l476xx.h"
#include "stm32_l476xx_I2c_Driver.h"
#include "stm32l476xx_gpio_Driver.h"



#define OPT3001_ADDR               0x47

/* ================= REGISTER MAP ================= */

#define OPT3001_REG_RESULT         0x00
#define OPT3001_REG_CONFIG         0x01
#define OPT3001_REG_LOWLIMIT       0x02
#define OPT3001_REG_HIGHLIMIT      0x03
#define OPT3001_REG_MANFID         0x7E
#define OPT3001_REG_DEVICEID       0x7F

I2C_Handle_t I2C1Handle;

volatile uint8_t TxComplete = 0;
volatile uint8_t RxComplete = 0;

uint8_t OPT3001_TxBuf[3];
uint8_t OPT3001_RxBuf[2];
uint8_t OPT3001_RegAddr;

int lux =0;

void I2C1_GPIOInits(void)
{

		GPIO_Handle_t I2CPins;

		I2CPins.pGPIOx = GPIOB;

		I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
		I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
		I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
		I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
		I2CPins.GPIO_PinConfig.GPIO_PinSpeed = 2;

		/* PB10 -> SCL */
		I2CPins.GPIO_PinConfig.GPIO_PinNumber = 8;
		GPIO_Init(&I2CPins);

		/* PB11 -> SDA */
		I2CPins.GPIO_PinConfig.GPIO_PinNumber = 9;
		GPIO_Init(&I2CPins);


}

//WRITE REGISTER
void OPT3001_WriteReg(I2C_Handle_t *pI2CHandle, uint8_t reg, uint16_t value) {
	uint8_t data[3];

	data[0] = reg;

	data[1] = (value >> 8) & 0xFF;
	data[2] = value & 0xFF;

//	I2C_MasterSendData(pI2CHandle, data, 3,
//	OPT3001_ADDR);
	I2C_MasterSendDataIT(pI2CHandle, data, 3, OPT3001_ADDR);
}

//READ REGISTER
uint16_t OPT3001_ReadReg(I2C_Handle_t *pI2CHandle, uint8_t reg) {
	uint8_t rx[2];

	/* Clear flags */
	pI2CHandle->pI2Cx->ICR |=
	I2C_ICR_STOPCF |
	I2C_ICR_NACKCF;

	/* ================= WRITE REGISTER ================= */

	pI2CHandle->pI2Cx->CR2 = 0;

	pI2CHandle->pI2Cx->CR2 |= (OPT3001_ADDR << 1);

	pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_NBYTES_Pos);

	pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_START_Pos);

	while (!(pI2CHandle->pI2Cx->ISR &
	I2C_ISR_TXIS))
		;

	pI2CHandle->pI2Cx->TXDR = reg;

	while (!(pI2CHandle->pI2Cx->ISR &
	I2C_ISR_TC))
	{
		if (pI2CHandle->pI2Cx->ISR & I2C_ISR_NACKF)
		{
			uint8_t FLAG = 1;
		}
	}

	/* ================= REPEATED START READ ================= */

	pI2CHandle->pI2Cx->CR2 = 0;

	pI2CHandle->pI2Cx->CR2 |= (OPT3001_ADDR << 1);

	pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_RD_WRN_Pos);

	pI2CHandle->pI2Cx->CR2 |= (2 << I2C_CR2_NBYTES_Pos);

	pI2CHandle->pI2Cx->CR2 |=  (1 << 25);

	pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_START_Pos);

	/* Read MSB */
	while (!(pI2CHandle->pI2Cx->ISR &
	I2C_ISR_RXNE))
		;

	rx[0] = pI2CHandle->pI2Cx->RXDR;

	/* Read LSB */
	while (!(pI2CHandle->pI2Cx->ISR &
	I2C_ISR_RXNE))
		;

	rx[1] = pI2CHandle->pI2Cx->RXDR;

	/* Wait STOP */
	while (!(pI2CHandle->pI2Cx->ISR &
	I2C_ISR_STOPF))
		;

	/* Clear STOP */
	pI2CHandle->pI2Cx->ICR |=
	I2C_ICR_STOPCF;

	return ((rx[0] << 8) | rx[1]);
}

//SENSOR INIT
void OPT3001_Init(I2C_Handle_t *pI2CHandle) {

//	OPT3001_WriteReg(pI2CHandle,
//	OPT3001_REG_CONFIG, 0xC410);

    TxComplete = 0;

    OPT3001_TxBuf[0] = OPT3001_REG_CONFIG;
    OPT3001_TxBuf[1] = (0xC410 >> 8) & 0xFF;
    OPT3001_TxBuf[2] = 0xC410 & 0xFF;

    I2C_MasterSendDataIT(pI2CHandle,
                         OPT3001_TxBuf,
                         3,
                         OPT3001_ADDR);

    while(!TxComplete);

}

//READ LUX
int OPT3001_ReadLux(I2C_Handle_t *pI2CHandle) {
	uint16_t raw = 0x0000;
	uint16_t exponent;
	uint16_t mantissa;

//	raw = OPT3001_ReadReg(pI2CHandle,
//	OPT3001_REG_RESULT);

    /* ================= SEND REGISTER ADDRESS ================= */

    TxComplete = 0;

    OPT3001_RegAddr = OPT3001_REG_RESULT;

    I2C_MasterSendDataIT(pI2CHandle,
                         &OPT3001_RegAddr,
                         1,
                         OPT3001_ADDR);

    while(!TxComplete);

    /* ================= RECEIVE 2 BYTES ================= */

    RxComplete = 0;

    I2C_MasterReceiveDataIT(pI2CHandle,
                            OPT3001_RxBuf,
                            2,
                            OPT3001_ADDR);

    while(!RxComplete);

    /* ================= COMBINE DATA ================= */

    raw = (OPT3001_RxBuf[0] << 8) |
           OPT3001_RxBuf[1];

    exponent = (raw >> 12) & 0x0F;

    mantissa = raw & 0x0FFF;

    int lux = mantissa *
             (0.01 * (1UL << exponent));

    return lux;
}



int main(void)
{

	GPIO_peripherial_Clk_Cntrl(GPIOB,SET);

	I2C1_PCLK_EN();

	I2C1_GPIOInits();

	I2C1Handle.pI2Cx = I2C1;

	I2C1Handle.I2C_Config.SCLSpeed = 100000;
	I2C1Handle.I2C_Config.DeviceAddress = 0x61;
	I2C1Handle.I2C_Config.ACKControl = ENABLE;

	I2C_Init(&I2C1Handle);

	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	I2C_IRQPriorityConfig(IRQ_NO_I2C1_EV, 2);
	I2C_IRQPriorityConfig(IRQ_NO_I2C1_ER, 1);

	OPT3001_Init(&I2C1Handle);

	while (1)
	{

		lux = OPT3001_ReadLux(&I2C1Handle);

		delay_ms(50);
	}
}


void I2C1_EV_IRQHandler(void)
{
    I2C_EV_IRQHandling(&I2C1Handle);
}

void I2C1_ER_IRQHandler(void)
{
    I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,
                                  uint8_t AppEv)
{
    if(AppEv == I2C_EV_TX_CMPLT)
    {
        TxComplete = 1;
    }

    else if(AppEv == I2C_EV_RX_CMPLT)
    {
        RxComplete = 1;
    }

    else if(AppEv == I2C_ERROR_NACK)
    {
        while(1);
    }
}
//main.c
//Displaying main.c.
