/*
 * SharedSpi.h
 *
 *  Created on: Sep 6, 2016
 *      Author: G. Andrews
 */

#ifndef USER_SHAREDSPI_H_
#define USER_SHAREDSPI_H_

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include <stdbool.h>

#define MAX_NUM_SPI_DEVICES	5

typedef struct {
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
} SharedSpi_CS_Type;

typedef struct {
	osMutexId spiMutex;
	SPI_HandleTypeDef *spiHandle;
	SharedSpi_CS_Type devices[MAX_NUM_SPI_DEVICES];
	int8_t numDevices;
	int8_t currentDevice;

} SharedSpi;

typedef enum {
	SHARED_SPI_SUCCESS,
	SHARED_SPI_FAIL
} SharedSpi_Return_Type;



// Public Interface Functions
int8_t SharedSpi_Register_CS(SharedSpi *, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
SharedSpi_Return_Type SharedSpi_Init(SharedSpi *, SPI_HandleTypeDef *);
SharedSpi_Return_Type SharedSpi_SendDataDMA(SharedSpi *, int8_t device, uint8_t * txData, uint16_t numBytes);
SharedSpi_Return_Type SharedSpi_ReceiveDataDMA(SharedSpi *, int8_t device, uint8_t * rxData, uint16_t numBytes);
SharedSpi_Return_Type SharedSpi_SendData(SharedSpi *, int8_t device, uint8_t * txData, uint16_t numBytes, bool leaveSelected);
SharedSpi_Return_Type SharedSpi_ReceiveData(SharedSpi *, int8_t device, uint8_t * rxData, uint16_t numBytes, bool selectDevice);
SharedSpi_Return_Type SharedSpi_SendReceiveData(SharedSpi *, int8_t device, uint8_t * txData, uint8_t * rxData, uint16_t numBytes);
SharedSpi_Return_Type SharedSpi_SelectDevice(SharedSpi * spi, uint8_t device);
SharedSpi_Return_Type SharedSpi_DeselectDevice(SharedSpi * spi);

void SharedSpi_RxCallback(SharedSpi *, bool firstHalf);
void SharedSpi_TxCallback(SharedSpi *, bool firstHalf);

#endif /* USER_SHAREDSPI_H_ */
