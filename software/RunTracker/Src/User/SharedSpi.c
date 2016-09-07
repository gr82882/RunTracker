/*
 * SharedSpi.c
 *
 *  Created on: Sep 6, 2016
 *      Author: G. Andrews
 */

#include "SharedSpi.h";

// Public interfaces

SharedSpi_Return_Type SharedSpi_Init(SharedSpi *spi, SPI_HandleTypeDef *hspi)
{
  osMutexDef(spiMutex);
  spi->spiMutex = osMutexCreate(osMutex(spiMutex));

  spi->numDevices = 0;
  spi->currentDevice = 0;
  spi->spiHandle = hspi;

  return SHARED_SPI_SUCCESS;
}

int8_t SharedSpi_Register_CS(SharedSpi * spi, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin)
{
  // Make sure we haven't already registered too many devices
  if(spi->numDevices >= MAX_NUM_SPI_DEVICES)
    return SHARED_SPI_FAIL;

  SharedSpi_CS_Type device;
  device.GPIO_Pin = GPIO_Pin;
  device.GPIOx = GPIOx;

  osMutexWait(spi->spiMutex, osWaitForever);

  spi->devices[spi->numDevices++] = device;

  osMutexRelease(spi->spiMutex);

  return spi->numDevices;
}

SharedSpi_Return_Type SharedSpi_SendDataDMA(SharedSpi * spi, int8_t device, uint8_t * txData, uint16_t numBytes)
{
  // Make sure device is registered
  if(device < spi->numDevices)
    return SHARED_SPI_FAIL;

  // Lock the mutex guard
  osMutexWait(spi->spiMutex, osWaitForever);

  spi->currentDevice = device;

  // Assert chip select for the selected device
  HAL_GPIO_WritePin(spi->devices[device-1].GPIOx, spi->devices[device-1].GPIO_Pin, GPIO_PIN_RESET);

  // Transmit Data via DMA
  HAL_SPI_Transmit_DMA(spi->spiHandle, txData, numBytes);

  return SHARED_SPI_SUCCESS;
}

SharedSpi_Return_Type SharedSpi_ReceiveDataDMA(SharedSpi * spi, int8_t device, uint8_t * rxData, uint16_t numBytes)
{
  // Make sure device is registered
  if(device < spi->numDevices)
    return SHARED_SPI_FAIL;

  // Lock the mutex guard
  osMutexWait(spi->spiMutex, osWaitForever);

  spi->currentDevice = device;

  // Assert chip select for the selected device
  HAL_GPIO_WritePin(spi->devices[device-1].GPIOx, spi->devices[device-1].GPIO_Pin, GPIO_PIN_RESET);

  // Receive Data via DMA
  HAL_SPI_Receive_DMA(spi->spiHandle, rxData, numBytes);

  return SHARED_SPI_SUCCESS;
}

void SharedSpi_RxCallback(SharedSpi * spi, bool firstHalf)
{
  // De-assert chip select
  HAL_GPIO_WritePin(spi->devices[spi->currentDevice -1].GPIOx, spi->devices[spi->currentDevice -1].GPIO_Pin, GPIO_PIN_SET);

  // Clear the selected device
  spi->currentDevice = 0;

  // Unlock the mutex guard
  osMutexRelease(spi->spiMutex);
}

void SharedSpi_TxCallback(SharedSpi * spi, bool firstHalf)
{
  // De-assert chip select
  HAL_GPIO_WritePin(spi->devices[spi->currentDevice -1].GPIOx, spi->devices[spi->currentDevice -1].GPIO_Pin, GPIO_PIN_SET);

  // Clear the selected device
  spi->currentDevice = 0;

  // Unlock the mutex guard
  osMutexRelease(spi->spiMutex);
}
