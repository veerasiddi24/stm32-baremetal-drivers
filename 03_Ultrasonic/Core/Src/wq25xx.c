/*
 * wq25xx.c
 *
 *  Created on: May 18, 2026
 *      Author: lenovo
 */


#include "main.h"


extern SPI_HandleTypeDef hspi1;

void cs_low(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void cs_high(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}


void spi_write(uint8_t *data, uint8_t len)
{
	HAL_SPI_Transmit(&hspi1, data, len, 2000);
}


void spi_read(uint8_t *data, uint8_t len)
{
	HAL_SPI_Receive(&hspi1, data, len, 5000);
}

void w25q_reset(void)
{
	uint8_t rst_cmd[2];
	rst_cmd[0] = 0x66;//enabling reset
	rst_cmd[1] = 0x99;//reset

	cs_low();
	spi_write(rst_cmd, 2);
	cs_high();
	HAL_Delay(100);

}

void write_enable(void)
{
	uint8_t cmd = 0x06;
	cs_low();
	spi_write(&cmd, 1);
	cs_high();
	HAL_Delay(5);
}

void write_disable(void)
{
	uint8_t cmd = 0x04;
	cs_low();
	spi_write(&cmd, 1);
	cs_high();
	HAL_Delay(5);
}

uint32_t w25q_readID(void)
{
	uint8_t r_cmd = 0x9F;
	uint8_t ID[3]={0};

	cs_low();
	spi_write(&r_cmd, 1);
	spi_read(ID, 3);
	cs_high();

	return ((ID[0]<<16)|(ID[1]<<8)|ID[2]);
}



uint8_t w25q_read_status(void)
{
    uint8_t cmd = 0x05;
    uint8_t status;

    cs_low();
    spi_write(&cmd, 1);
    spi_read(&status, 1);
    cs_high();

    return status;
}

void w25q_read(uint32_t Addr, uint8_t *rData, uint8_t size)
{
	uint8_t r_cmd[4]={0};

	r_cmd[0] = 0x03;
	r_cmd[1] =(Addr>>16)&0xFF;
	r_cmd[2] = (Addr>>8)&0xFF;
	r_cmd[3] = (Addr)&0xFF;

	cs_low();
	spi_write(r_cmd, 4);
	spi_read(rData, size);
	cs_high();

}


void w25q_write(uint32_t Addr, uint8_t *data, uint8_t size)
{
	uint8_t wr_cmd[4];
	uint8_t r_sts[4];

	wr_cmd[0] = 0x02;//page program
	wr_cmd[1] = (Addr >> 16) & 0xFF;
	wr_cmd[2] = (Addr >> 8) & 0xFF;
	wr_cmd[3] = (Addr) & 0xFF;

	w25q_read(Addr, r_sts, 4);
	if(r_sts[0] == 0xFF)
	{
		write_enable();
	    cs_low();
	    spi_write(wr_cmd, 4);      // send command + address
	    spi_write(data, size);     // write ONLY 4 bytes
	    cs_high();

	    HAL_Delay(5);
	    write_disable();
	}
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

    cs_low();
    spi_write(cmd, 4);
    cs_high();

    HAL_Delay(450);

    write_disable();
}





