#include "LSM6DS3.h"

// Private function prototypes
static IMU_Return_Type writeRegister(LSM6DS3 * imu, uint8_t offset, uint8_t dataToWrite);
static IMU_Return_Type readRegister(LSM6DS3 * imu, uint8_t * output, uint8_t offset);
static IMU_Return_Type readRegisterInt16(LSM6DS3 * imu, int16_t * output, uint8_t offset);
static IMU_Return_Type readRegisterRegion(LSM6DS3 * imu, uint8_t * output, uint8_t offset, uint8_t length);

// Public interfaces
void LSM6DS3_Init(LSM6DS3 * imu, SharedSpi * spi, uint8_t deviceHandle)
{
  uint8_t dataToWrite = 0;  //Temporary variable
  SensorSettings settings;

  imu->spi = spi;
  imu->deviceHandle = deviceHandle;

  // Default configuration values
  settings.gyroEnabled = 0;
  settings.gyroFifoEnabled = 0;

  settings.accelEnabled = 1;
  settings.accelRange = 16;      		//Max G force readable.  Can be: 2, 4, 8, 16
  settings.accelSampleRate = 833;  	//Hz.  Can be: 13, 26, 52, 104, 208, 416, 833, 1666, 3332, 6664, 13330
  settings.accelBandWidth = 200;  		//Hz.  Can be: 50, 100, 200, 400;
  settings.accelFifoEnabled = 0;  		//Set to include accelerometer in the FIFO
  settings.accelFifoDecimation = 1;	//set 1 for on /1
  settings.tempEnabled = 0;

  //FIFO control settings
  settings.fifoThreshold = 100;  		//Can be 0 to 4096 (16 bit bytes)
  settings.fifoSampleRate = 50;  		//Hz.  Can be: 10, 25, 50, 100, 200, 400, 800, 1600, 3300, 6600
  settings.fifoModeWord = 6;  			  //FIFO mode.

  //FIFO mode.  Can be:
  //  0 (Bypass mode, FIFO off)
  //  1 (Stop when full)
  //  3 (Continuous during trigger)
  //  4 (Bypass until trigger)
  //  6 (Continous mode)

  imu->settings = settings;

  //Setup the accelerometer******************************
  dataToWrite = 0; //Start Fresh!
  if ( settings.accelEnabled == 1) {
    //Build config reg
    //First patch in filter bandwidth
    switch (settings.accelBandWidth) {
    case 50:
      dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_50Hz;
      break;
    case 100:
      dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_100Hz;
      break;
    case 200:
      dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_200Hz;
      break;
    default:  //set default case to max passthrough
    case 400:
      dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_400Hz;
      break;
    }
    //Next, patch in full scale
    switch (settings.accelRange) {
    case 2:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_2g;
      break;
    case 4:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_4g;
      break;
    case 8:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_8g;
      break;
    default:  //set default case to 16(max)
    case 16:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_16g;
      break;
    }
    //Lastly, patch in accelerometer ODR
    switch (settings.accelSampleRate) {
    case 13:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_13Hz;
      break;
    case 26:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_26Hz;
      break;
    case 52:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_52Hz;
      break;
    default:  //Set default to 104
    case 104:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_104Hz;
      break;
    case 208:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_208Hz;
      break;
    case 416:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_416Hz;
      break;
    case 833:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_833Hz;
      break;
    case 1660:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_1660Hz;
      break;
    case 3330:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_3330Hz;
      break;
    case 6660:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_6660Hz;
      break;
    case 13330:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_13330Hz;
      break;
    }
  }
  else
  {
    //dataToWrite already = 0 (powerdown);
  }

  //Now, write the patched together data
  writeRegister(imu, LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);

  //Set the ODR bit
  readRegister(imu, &dataToWrite, LSM6DS3_ACC_GYRO_CTRL4_C);
  dataToWrite &= ~((uint8_t)LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED);
  if ( settings.accelODROff == 1) {
    dataToWrite |= LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED;
  }
  writeRegister(imu, LSM6DS3_ACC_GYRO_CTRL4_C, dataToWrite);

  //Setup the gyroscope**********************************************
  dataToWrite = 0; //Start Fresh!
  if ( settings.gyroEnabled == 1) {
    //Build config reg
    //First, patch in full scale
    switch (settings.gyroRange) {
    case 125:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_125_ENABLED;
      break;
    case 245:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_G_245dps;
      break;
    case 500:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_G_500dps;
      break;
    case 1000:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_G_1000dps;
      break;
    default:  //Default to full 2000DPS range
    case 2000:
      dataToWrite |= LSM6DS3_ACC_GYRO_FS_G_2000dps;
      break;
    }
    //Lastly, patch in gyro ODR
    switch (settings.gyroSampleRate) {
    case 13:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_G_13Hz;
      break;
    case 26:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_G_26Hz;
      break;
    case 52:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_G_52Hz;
      break;
    default:  //Set default to 104
    case 104:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_G_104Hz;
      break;
    case 208:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_G_208Hz;
      break;
    case 416:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_G_416Hz;
      break;
    case 833:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_G_833Hz;
      break;
    case 1660:
      dataToWrite |= LSM6DS3_ACC_GYRO_ODR_G_1660Hz;
      break;
    }
  }
  else
  {
    //dataToWrite already = 0 (powerdown);
  }
  //Write the byte
  writeRegister(imu, LSM6DS3_ACC_GYRO_CTRL2_G, dataToWrite);

}

// Private Function Implementations --------------
static IMU_Return_Type writeRegister(LSM6DS3 * imu, uint8_t offset, uint8_t dataToWrite)
{
  uint8_t txData[2] = {offset, dataToWrite};
  if(SharedSpi_SendData(imu->spi, imu->deviceHandle, txData, 2, false) != SHARED_SPI_SUCCESS)
    return IMU_HW_ERROR;

  return IMU_SUCCESS;
}

static IMU_Return_Type readRegister(LSM6DS3 * imu, uint8_t * output, uint8_t offset)
{
  uint8_t txData[2] = {offset | 0x80, 0x00};
  uint8_t rxData[2];

  if(SharedSpi_SendReceiveData(imu->spi, imu->deviceHandle, txData, rxData, 2) != SHARED_SPI_SUCCESS)
      return IMU_HW_ERROR;

  *output = rxData[1];

  return IMU_SUCCESS;
}

static IMU_Return_Type readRegisterRegion(LSM6DS3 * imu, uint8_t * output, uint8_t offset, uint8_t length)
{
  uint8_t txData = (offset | 0x80);

  if(SharedSpi_SendData(imu->spi, imu->deviceHandle, &txData, 1, true) != SHARED_SPI_SUCCESS)
    return IMU_HW_ERROR;

  if(SharedSpi_ReceiveData(imu->spi, imu->deviceHandle, output, length, false) != SHARED_SPI_SUCCESS)
    return IMU_HW_ERROR;

  SharedSpi_DeselectDevice(imu->spi);

  return IMU_SUCCESS;
}

static IMU_Return_Type readRegisterInt16(LSM6DS3 * imu, int16_t * output, uint8_t offset)
{
  uint8_t myBuffer[2];
  if(readRegisterRegion(imu, myBuffer, offset, 2) != IMU_SUCCESS)
    return IMU_HW_ERROR;

  *output = (int16_t)myBuffer[0] | ((int16_t)myBuffer[1] << 8);

  return IMU_SUCCESS;
}
