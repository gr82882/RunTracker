#include "VS1053.h"
#include "ff.h"

static FIL currentTrack;

// Private functions
static void sciWrite(VS1053 * codec, uint8_t addr, uint16_t data);
static uint16_t sciRead(VS1053 * codec, uint8_t addr);
static bool readyForData(void);
static void playData(VS1053 * codec, uint8_t *buffer, uint8_t buffsiz);
static void feedBuffer(VS1053 * codec);

void VS1053_Init(VS1053 * codec, SharedSpi * spi, uint8_t dataHandle, uint8_t cmdHandle)
{
  codec->spi = spi;
  codec->dataHandle = dataHandle;
  codec->cmdHandle = cmdHandle;
  codec->playingMusic = false;

  VS1053_Reset(codec);
}

void VS1053_StartPlayingFile(VS1053 * codec, const char * filename)
{
  // reset playback
  sciWrite(codec, VS1053_REG_MODE, VS1053_MODE_SM_LINE1 | VS1053_MODE_SM_SDINEW);

  // resync
  sciWrite(codec, VS1053_REG_WRAMADDR, 0x1e29);
  sciWrite(codec, VS1053_REG_WRAM, 0);

  if(f_open(&currentTrack, filename, FA_READ) != FR_OK)
  {
    // TODO FAIL!
    return;
  }

  // As explained in data sheet, set twice 0 in REG_DECODETIME to set time back to 0
  sciWrite(codec, VS1053_REG_DECODETIME, 0x00);
  sciWrite(codec, VS1053_REG_DECODETIME, 0x00);

  // wait till its ready for data
  while (! readyForData() );

  codec->playingMusic = true;
  while (codec->playingMusic && readyForData())
  //while(codec->playingMusic)
    feedBuffer(codec);

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

  sciWrite(codec, VS1053_REG_CLOCKF, 0x6000);
  //Mp3WriteRegister(SCI_CLOCKF, 0x60, 0x00); //Set multiplier to 3.0x

}

void VS1053_SineTest(VS1053 * codec, uint8_t n, uint16_t ms)
{
  uint8_t sine[8] = {0x53, 0xEF, 0x6E, n, 0x00, 0x00, 0x00, 0x00};
  VS1053_Reset(codec);

  uint16_t mode = sciRead(codec, VS1053_REG_MODE);
  mode = 0x0820;
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

  HAL_Delay(10);

  while(HAL_GPIO_ReadPin(VS1053_DREQ_GPIO_Port, VS1053_DREQ_Pin) == GPIO_PIN_RESET) {}
}

static uint16_t sciRead(VS1053 * codec, uint8_t addr)
{
  uint8_t tx[2];
  uint8_t rx[2];
  //uint16_t rx;

  SharedSpi_SendReceiveData(codec->spi, codec->cmdHandle, tx, (uint8_t *)&rx, 2);

  HAL_Delay(10);

  while(HAL_GPIO_ReadPin(VS1053_DREQ_GPIO_Port, VS1053_DREQ_Pin) == GPIO_PIN_RESET) {}

  return (rx[0] << 8 | rx[1]);
}

static bool readyForData(void)
{
  return HAL_GPIO_ReadPin(VS1053_DREQ_GPIO_Port, VS1053_DREQ_Pin) == GPIO_PIN_SET;
}

static void playData(VS1053 * codec, uint8_t *buffer, uint8_t bufferSize)
{
  //SharedSpi_SendDataDMA(codec->spi, codec->dataHandle, buffer, bufferSize);
  SharedSpi_SendData(codec->spi, codec->dataHandle, buffer, bufferSize, false);
}

void VS1053_DREQ_Callback(VS1053 * codec)
{
  feedBuffer(codec);
}

static void feedBuffer(VS1053 * codec)
{
  unsigned int bytesRead = 0;

  if (! codec->playingMusic) {
    //running = 0;
    return; // paused or stopped
  }
  //if (currentTrack == NULL) {
    //running = 0;
    //return;
  //}
  if (! readyForData()) {
    //running = 0;
    return;
  }

  // Feed the hungry buffer! :)
  while (readyForData())
  {
      // Read some audio data from the SD card file
    FRESULT res = f_read(&currentTrack, codec->trackBuffer, VS1053_DATABUFFERLEN, &bytesRead);
    if(res != FR_OK)
    {
      // TODO FAIL!
      codec->playingMusic = false;
      return;
    }

    if (bytesRead == 0) {
      // must be at the end of the file, wrap it up!
      codec->playingMusic = false;
      f_close(&currentTrack);
      return;
    }
    playData(codec, codec->trackBuffer, bytesRead);
  }

  return;
}
