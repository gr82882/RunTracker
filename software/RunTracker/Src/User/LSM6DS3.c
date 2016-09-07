#include "LSM6DS3.h"


void LSM6DS3_Init(LSM6DS3 * imu, SharedSpi * spi, uint8_t deviceHandle)
{
  imu->spi = spi;
  imu->deviceHandle = deviceHandle;
}


