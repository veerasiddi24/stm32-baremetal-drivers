/*
 * wq25xx.h
 *
 *  Created on: May 18, 2026
 *      Author: lenovo
 */


#ifndef INC_W25Q32F_H_
#define INC_W25Q32F_H_

void spi_write(uint8_t *data, uint8_t len);
void spi_read(uint8_t *data, uint8_t len);
void cs_low(void);
void cs_high(void);
uint32_t w25q_readID(void);
void w25q_reset(void);
void w25q_write(uint32_t Addr, uint8_t *data, uint8_t size);
void w25q_read(uint32_t Addr, uint8_t *rData, uint8_t size);
void w25q_erase_sector(uint32_t sector);
uint8_t w25q_read_status(void);
void write_enable(void);
void write_disable(void);


#endif /* INC_W25Q32F_H_ */
