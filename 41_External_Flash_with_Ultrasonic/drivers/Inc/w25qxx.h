/*
 * w25qxx.h
 *
 *  Created on: May 15, 2026
 *      Author: lenovo
 */

#ifndef INC_W25QXX_H_
#define INC_W25QXX_H_


#include "stm32l476xx.h"   // Adjust to your MCU family
#include <stdint.h>

#define MAX_LOGS 1024
#define LOG_SIZE 4

/* ================= COMMAND OPCODES ================= */

#define W25Q_CMD_WRITE_ENABLE      0x06
#define W25Q_CMD_WRITE_DISABLE     0x04
#define W25Q_CMD_READ_STATUS1      0x05
#define W25Q_CMD_PAGE_PROGRAM      0x02
#define W25Q_CMD_READ_DATA         0x03
#define W25Q_CMD_FAST_READ         0x0B
#define W25Q_CMD_SECTOR_ERASE      0x20
#define W25Q_CMD_CHIP_ERASE        0xC7
#define W25Q_CMD_JEDEC_ID          0x9F

/* ================= FLASH GEOMETRY ================= */
#define W25Q_PAGE_SIZE            256
#define W25Q_SECTOR_SIZE          4096
#define W25Q_BLOCK_SIZE           (64 * 1024)

/* ================= CHIP SELECT PIN ================= */
// Define these according to your board wiring
#define W25Q_CS_PORT              GPIOA
#define W25Q_CS_PIN               4

/* ================= FUNCTION PROTOTYPES ================= */
void W25Q_CS_LOW(void);
void W25Q_CS_HIGH(void);

void W25Q_Init(void);

void W25Q_WriteEnable(void);

uint8_t W25Q_ReadStatus1(void);

void W25Q_WaitBusy(void);

void W25Q_ReadID(uint8_t *id);

void W25Q_Read(uint32_t addr, uint8_t *buf, uint32_t len);

void W25Q_WritePage(uint32_t addr, uint8_t *buf, uint16_t len);
void W25Q_Write(uint32_t addr, uint8_t *buf, uint32_t len);

void W25Q_EraseSector(uint32_t addr);





#endif /* INC_W25QXX_H_ */
