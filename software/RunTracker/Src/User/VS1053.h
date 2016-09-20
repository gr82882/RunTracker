/*
 * VS1053.h
 *
 *  Created on: Sep 19, 2016
 *      Author: G. Andrews
 *      Heaviliy leveraged the Adafruit VS1053 library
 */

#ifndef VS1053_H_
#define VS1053_H_

#include "stm32f4xx.h"
#include "SharedSpi.h"
#include <stdbool.h>


#define VS1053_DATABUFFERLEN 32

typedef struct {
  SharedSpi * spi;
  uint8_t dataHandle;
  uint8_t cmdHandle;
  bool playingMusic;

  uint8_t trackBuffer[VS1053_DATABUFFERLEN];

} VS1053;

// Public Interfaces
void VS1053_Init(VS1053 * codec, SharedSpi * spi, uint8_t dataHandle, uint8_t cmdHandle);
void VS1053_Play_File(VS1053 * codec, const char* filename);
void VS1053_DREQ_Callback(VS1053 * codec);
void VS1053_Reset(VS1053 * codec);
void VS1053_SoftReset(VS1053 * codec);
void VS1053_SineTest(VS1053 * codec, uint8_t n, uint16_t ms);
void VS1053_StartPlayingFile(VS1053 * codec, const char * filename);

// Definitions
#define VS1053_SCI_READ 0x03
#define VS1053_SCI_WRITE 0x02

#define VS1053_REG_MODE  0x00
#define VS1053_REG_STATUS 0x01
#define VS1053_REG_BASS 0x02
#define VS1053_REG_CLOCKF 0x03
#define VS1053_REG_DECODETIME 0x04
#define VS1053_REG_AUDATA 0x05
#define VS1053_REG_WRAM 0x06
#define VS1053_REG_WRAMADDR 0x07
#define VS1053_REG_HDAT0 0x08
#define VS1053_REG_HDAT1 0x09
#define VS1053_REG_VOLUME 0x0B

#define VS1053_GPIO_DDR 0xC017
#define VS1053_GPIO_IDATA 0xC018
#define VS1053_GPIO_ODATA 0xC019

#define VS1053_INT_ENABLE  0xC01A

#define VS1053_MODE_SM_DIFF 0x0001
#define VS1053_MODE_SM_LAYER12 0x0002
#define VS1053_MODE_SM_RESET 0x0004
#define VS1053_MODE_SM_CANCEL 0x0008
#define VS1053_MODE_SM_EARSPKLO 0x0010
#define VS1053_MODE_SM_TESTS 0x0020
#define VS1053_MODE_SM_STREAM 0x0040
#define VS1053_MODE_SM_SDINEW 0x0800
#define VS1053_MODE_SM_ADPCM 0x1000
#define VS1053_MODE_SM_LINE1 0x4000
#define VS1053_MODE_SM_CLKRANGE 0x8000


#define VS1053_SCI_AIADDR 0x0A
#define VS1053_SCI_AICTRL0 0x0C
#define VS1053_SCI_AICTRL1 0x0D
#define VS1053_SCI_AICTRL2 0x0E
#define VS1053_SCI_AICTRL3 0x0F


#endif
