/*
 * w25qxx.c
 *
 *  Created on: May 15, 2026
 *      Author: lenovo
 */


#include "w25qxx.h"


extern uint8_t Spi_Rx_Buff;
/* ================= CS ================= */

void W25Q_CS_LOW(void) {
	GPIO_Write_Output_Pin(W25Q_CS_PORT, W25Q_CS_PIN, 0);
}

void W25Q_CS_HIGH(void) {
	GPIO_Write_Output_Pin(W25Q_CS_PORT, W25Q_CS_PIN, 1);
}

/* ================= INIT ================= */

void W25Q_Init(void) {
	W25Q_CS_HIGH();
}

/* ================= WRITE ENABLE ================= */

void W25Q_WriteEnable(void) {
	uint8_t cmd = W25Q_CMD_WRITE_ENABLE;

	W25Q_CS_LOW();

	SPI_SendData(SPI1, &cmd, 1);

	W25Q_CS_HIGH();
}

/* ================= READ STATUS ================= */

uint8_t W25Q_ReadStatus1(void) {
	uint8_t cmd = W25Q_CMD_READ_STATUS1;
	uint8_t status;

	W25Q_CS_LOW();

	SPI_SendData(SPI1, &cmd, 1);

	SPI_ReceiveData(SPI1, &status, 1);

	W25Q_CS_HIGH();

	return status;
}

/* ================= WAIT BUSY ================= */

void W25Q_WaitBusy(void) {
	while (W25Q_ReadStatus1() & 0x01)
		;
}

/* ================= READ ID ================= */

void W25Q_ReadID(uint8_t *id) {
	uint8_t cmd = W25Q_CMD_JEDEC_ID;

	W25Q_CS_LOW();

	SPI_SendData(SPI1, &cmd, 1);

	SPI_ReceiveData(SPI1, id, 3);

	W25Q_CS_HIGH();
}

/* ================= READ DATA ================= */

void W25Q_Read(uint32_t addr, uint8_t *buf, uint32_t len) {
	uint8_t cmd[4];

	cmd[0] = W25Q_CMD_READ_DATA;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	W25Q_CS_LOW();

	SPI_SendData(SPI1, cmd, 4);

	SPI_ReceiveData(SPI1, buf, len);

	W25Q_CS_HIGH();
}

/* ================= PAGE WRITE ================= */

void W25Q_WritePage(uint32_t addr, uint8_t *buf, uint16_t len) {
	W25Q_WriteEnable();

	uint8_t cmd[4];

	cmd[0] = W25Q_CMD_PAGE_PROGRAM;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	W25Q_CS_LOW();

	SPI_SendData(SPI1, cmd, 4);

	SPI_SendData(SPI1, buf, len);

	W25Q_CS_HIGH();

	W25Q_WaitBusy();
}

/* ================= MULTI PAGE WRITE ================= */

void W25Q_Write(uint32_t addr, uint8_t *buf, uint32_t len) {
	while (len > 0) {
		uint16_t chunk;

		chunk = 256 - (addr % 256);

		if (chunk > len)
			chunk = len;

		W25Q_WritePage(addr, buf, chunk);

		addr += chunk;
		buf += chunk;
		len -= chunk;
	}
}

/* ================= ERASE SECTOR ================= */

void W25Q_EraseSector(uint32_t addr) {
	W25Q_WriteEnable();

	uint8_t cmd[4];

	cmd[0] = W25Q_CMD_SECTOR_ERASE;
	cmd[1] = (addr >> 16) & 0xFF;
	cmd[2] = (addr >> 8) & 0xFF;
	cmd[3] = addr & 0xFF;

	W25Q_CS_LOW();

	SPI_SendData(SPI1, cmd, 4);

//	SPI_SendReceiveData(SPI1, cmd, Spi_Rx_Buff, 4);

	W25Q_CS_HIGH();

	W25Q_WaitBusy();
}






//void cs_low(void)
//{
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//}
//
//void cs_high(void)
//{
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//}


//void spi_write(uint8_t *data, uint8_t len)
//{
//	HAL_SPI_Transmit(&hspi1, data, len, 2000);
//}
//
//
//void spi_read(uint8_t *data, uint8_t len)
//{
//	HAL_SPI_Receive(&hspi1, data, len, 5000);
//}

void w25q_reset(void)
{
	uint8_t rst_cmd[2];
	rst_cmd[0] = 0x66;//enabling reset
	rst_cmd[1] = 0x99;//reset

	W25Q_CS_LOW();
//	spi_write(rst_cmd, 2);
	SPI_SendData(SPI1, rst_cmd, 2);
	W25Q_CS_HIGH();
//	HAL_Delay(100);
//	SysTick_DelayMs(100);

//	delay_us(100000);

delay_us(10);

}

void write_enable(void)
{
	uint8_t cmd = 0x06;
//	cs_low();
//	spi_write(&cmd, 1);
//	cs_high();
//	HAL_Delay(5);

	W25Q_CS_LOW();
	SPI_SendData(SPI1, &cmd, 1);
	W25Q_CS_HIGH();
//	SysTick_DelayMs(5);
	delay_ms(5);
}

void write_disable(void)
{
	uint8_t cmd = 0x04;
//	cs_low();
//	spi_write(&cmd, 1);
//	cs_high();
//	HAL_Delay(5);

	W25Q_CS_LOW();
	SPI_SendData(SPI1, &cmd, 1);
	W25Q_CS_HIGH();
//	SysTick_DelayMs(5);
	delay_ms(5);
}

uint32_t w25q_readID(void)
{
	uint8_t r_cmd = 0x9F;
	uint8_t ID[3]={0};

	W25Q_CS_LOW();
//	spi_write(&r_cmd, 1);
	SPI_SendData(SPI1, &r_cmd, 1);
//	spi_read(ID, 3);
	SPI_ReceiveData(SPI1,ID, 3);
	W25Q_CS_HIGH();

	return ((ID[0]<<16)|(ID[1]<<8)|ID[2]);
}



uint8_t w25q_read_status(void)
{
    uint8_t cmd = 0x05;
    uint8_t status;

//    cs_low();
//    spi_write(&cmd, 1);
//    spi_read(&status, 1);
//    cs_high();

	W25Q_CS_LOW();
	SPI_SendData(SPI1, &cmd, 1);
	SPI_ReceiveData(SPI1, &status, 1);
	W25Q_CS_HIGH();

    return status;
}

void w25q_read(uint32_t Addr, uint8_t *rData, uint8_t size)
{
	uint8_t r_cmd[4]={0};

	r_cmd[0] = 0x03;
	r_cmd[1] =(Addr>>16)&0xFF;
	r_cmd[2] = (Addr>>8)&0xFF;
	r_cmd[3] = (Addr)&0xFF;

//	cs_low();
//	spi_write(r_cmd, 4);
//	spi_read(rData, size);
//	cs_high();

	W25Q_CS_LOW();
	SPI_SendData(SPI1, r_cmd, 4);
	SPI_ReceiveData(SPI1, rData, size);
	W25Q_CS_HIGH();
}


//void w25q_write(uint32_t Addr, uint8_t *data, uint8_t size)
//{
//	uint8_t wr_cmd[4];
//	uint8_t r_sts[4];
//
//	wr_cmd[0] = 0x02;//page program
//	wr_cmd[1] = (Addr >> 16) & 0xFF;
//	wr_cmd[2] = (Addr >> 8) & 0xFF;
//	wr_cmd[3] = (Addr) & 0xFF;
//
//	w25q_read(Addr, r_sts, 4);
////	if(r_sts[0] == 0xFF)
//	{
//		write_enable();
////	    cs_low();
////	    spi_write(wr_cmd, 4);      // send command + address
////	    spi_write(data, size);     // write ONLY 4 bytes
////	    cs_high();
//
//
//		W25Q_CS_LOW();
//		SPI_SendData(SPI1, wr_cmd, 4);
//		SPI_SendData(SPI1,data, size);
//		W25Q_CS_HIGH();
//
//
////		SysTick_DelayMs(450);
//
////		delay_us(450000);
//
//		delay_us(450);
//		write_disable();
//	}
//}

void w25q_write(uint32_t Addr, uint8_t *data, uint8_t size)
{
    uint8_t wr_cmd[4];

    wr_cmd[0] = 0x02; // Page Program
    wr_cmd[1] = (Addr >> 16) & 0xFF;
    wr_cmd[2] = (Addr >> 8) & 0xFF;
    wr_cmd[3] = Addr & 0xFF;

    write_enable();

    W25Q_CS_LOW();

    SPI_SendData(SPI1, wr_cmd, 4);

    SPI_SendData(SPI1, data, size);

    W25Q_CS_HIGH();

    while(w25q_read_status() & 0x01);
}

void w25q_erase_sector(uint32_t sector)
{
    uint8_t cmd[4];
    uint32_t addr = sector*16*256;

    write_enable();

    cmd[0] = 0x20;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

//    cs_low();
//    spi_write(cmd, 4);
//    cs_high();

	W25Q_CS_LOW();
	SPI_SendData(SPI1, cmd, 4);
	W25Q_CS_HIGH();
//	SysTick_DelayMs(450);
//	delay_us(450000);

	delay_ms(450);

//    HAL_Delay(450);

    write_disable();
}






