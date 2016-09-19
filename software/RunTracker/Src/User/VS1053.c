#include "VS1053.h"

// Private functions
static void sciWrite(VS1053 * codec, uint8_t addr, uint16_t data);
static uint16_t sciRead(VS1053 * codec, uint8_t addr);

void VS1053_Init(VS1053 * codec, SharedSpi * spi, uint8_t dataHandle, uint8_t cmdHandle)
{
  codec->spi = spi;
  codec->dataHandle = dataHandle;
  codec->cmdHandle = cmdHandle;
}

void VS1053_Reset(VS1053 * codec)
{
  HAL_GPIO_WritePin(VS1053_RST_GPIO_Port, VS1053_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(VS1053_RST_GPIO_Port, VS1053_RST_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(VS1053_CS_GPIO_Port, VS1053_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(VS1053_DCS_GPIO_Port, VS1053_DCS_Pin, GPIO_PIN_SET);
  HAL_Delay(100);

  VS1053_SoftReset(codec);
}

void VS1053_SoftReset(VS1053 * codec)
{
  sciWrite(codec, VS1053_REG_MODE, VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_RESET);
  HAL_Delay(100);
}

void VS1053_SineTest(VS1053 * codec, uint8_t n, uint16_t ms)
{
  uint8_t sine[8] = {0x53, 0xEF, 0x6E, n, 0x00, 0x00, 0x00, 0x00};
  VS1053_Reset(codec);

  uint16_t mode = sciRead(codec, VS1053_REG_MODE);
  mode |= 0x0020;
  sciWrite(codec, VS1053_REG_MODE, mode);

  while(HAL_GPIO_ReadPin(VS1053_DREQ_GPIO_Port, VS1053_DREQ_Pin) == GPIO_PIN_RESET) {}

  SharedSpi_SendData(codec->spi, codec->dataHandle, sine, 8, false);

  HAL_Delay(ms);

  sine[0] = 0x45;
  sine[1] = 0x78;
  sine[2] = 0x69;
  sine[3] = 0x74;

  SharedSpi_SendData(codec->spi, codec->dataHandle, sine, 8, false);
}

static void sciWrite(VS1053 * codec, uint8_t addr, uint16_t data)
{
  uint8_t tx[4];
  tx[0] = VS1053_SCI_WRITE;
  tx[1] = addr;
  tx[2] = data >> 8;
  tx[3] = data & 0xFF;

  SharedSpi_SendData(codec->spi, codec->cmdHandle, tx, 4, false);
}

static uint16_t sciRead(VS1053 * codec, uint8_t addr)
{
  uint8_t tx[2];
  uint16_t rx;

  SharedSpi_SendReceiveData(codec->spi, codec->cmdHandle, tx, (uint8_t *)&rx, 2);
  return rx;
}

