/*
 * RunTrackerGPS.h
 *
 *  Created on: Aug 30, 2016
 *      Author: G. Andrews
 */

#ifndef RUNTRACKERGPS_H_
#define RUNTRACKERGPS_H_

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "CircularBuffer.h"

#include <stdbool.h>

#define UART_DMA_BUFFER_SIZE  240

typedef struct {
  uint8_t hour, minute, seconds, year, month, day;
  uint16_t milliseconds;
  float latitude, longitude;
  int32_t latitude_fixed, longitude_fixed;
  float latitudeDegrees, longitudeDegrees;
  float geoidheight, altitude;
  float speed, angle, magvariation, HDOP;
  char lat, lon, mag;
  bool fix;
  bool paused;
  bool inStandby;
  uint8_t fixquality, satellites;

  UART_HandleTypeDef * huart;
  osMailQId mailId;

  CircularBuffer_TypeDef ringBuffer;
} RunTracker_GPS;

typedef enum {
  NMEA_INVALID_CMD = 0,
  NMEA_UNSUPPORTED_CMD,
  NMEA_ACTION_FAILED,
  NMEA_SUCCESS
} NMEA_Return_Type;


// Public Functions
void RunTracker_GPS_Init(RunTracker_GPS *, UART_HandleTypeDef *);
void RunTracker_GPS_RXCallback(RunTracker_GPS *, bool);
void RunTracker_GPS_Pause(RunTracker_GPS * GPS, bool);
bool RunTracker_GPS_Wakeup(RunTracker_GPS * GPS);
bool RunTracker_GPS_Standby(RunTracker_GPS * GPS);


// different commands to set the update rate from once a second (1 Hz) to 10 times a second (10Hz)
// Note that these only control the rate at which the position is echoed, to actually speed up the
// position fix you must also send one of the position fix rate commands below too.
#define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ  "$PMTK220,10000*2F\r\n" // Once every 10 seconds, 100 millihertz.
#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ  "$PMTK220,5000*1B\r\n"  // Once every 5 seconds, 200 millihertz.
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F\r\n"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C\r\n"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F\r\n"

// Position fix update rate commands.
#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ  "$PMTK300,10000,0,0,0,0*2C\r\n" // Once every 10 seconds, 100 millihertz.
#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ  "$PMTK300,5000,0,0,0,0*18\r\n"  // Once every 5 seconds, 200 millihertz.
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C\r\n"
#define PMTK_API_SET_FIX_CTL_5HZ  "$PMTK300,200,0,0,0,0*2F\r\n"

// Can't fix position faster than 5 times a second!


#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C\r\n"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17\r\n"

// turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
// turn off output
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"

// to generate your own sentences, check out the MTK command datasheet and use a checksum calculator
// such as the awesome http://www.hhhh.org/wiml/proj/nmeaxor.html

#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22\r\n"
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23\r\n"
#define PMTK_LOCUS_STARTSTOPACK "$PMTK001,185,3*3C\r\n"
#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38\r\n"
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22\r\n"
#define LOCUS_OVERLAP 0
#define LOCUS_FULLSTOP 1

#define PMTK_ENABLE_SBAS "$PMTK313,1*2E\r\n"
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E\r\n"

// standby command & boot successful message
#define PMTK_STANDBY "$PMTK161,0*28\r\n"
#define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36\r\n"  // Not needed currently
#define PMTK_AWAKE "$PMTK010,002*2D\r\n"

// ask for the release and version
#define PMTK_Q_RELEASE "$PMTK605*31\r\n"

// request for updates on antenna status
#define PGCMD_ANTENNA "$PGCMD,33,1*6C\r\n"
#define PGCMD_NOANTENNA "$PGCMD,33,0*6D\r\n"

// how long to wait when we're looking for a response
#define MAXWAITSENTENCE 5000


#endif /* RUNTRACKERGPS_H_ */
