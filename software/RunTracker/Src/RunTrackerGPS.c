#include "RunTrackerGPS.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// how long are max NMEA lines to parse?
#define MAXLINELENGTH 120

// we double buffer: read one line in and leave one for the main program
volatile char line1[MAXLINELENGTH];
volatile char line2[MAXLINELENGTH];
// our index into filling the current line
volatile uint8_t lineidx=0;
// pointers to the double buffers
volatile char *currentline;
volatile char *lastline;
volatile bool recvdflag;
volatile bool inStandbyMode;


// Private function prototypes
static bool RunTracker_parse(RunTracker_GPS * GPS, char * nmea);
static uint8_t parseHex(char c);


/*
 * typedef struct {
  uint8_t hour, minute, seconds, year, month, day;
  uint16_t milliseconds;
  float latitude, longitude;
  int32_t latitude_fixed, longitude_fixed;
  float latitudeDegrees, longitudeDegrees;
  float geoidheight, altitude;
  float speed, angle, magvariation, HDOP;
  char lat, lon, mag;
  bool fix;
  uint8_t fixquality, satellites;

  UART_HandleTypeDef * huart;

} RunTracker_GPS;

// Public Functions
void RunTracker_GPS_init(RunTracker_GPS * const, UART_HandleTypeDef *);
void RunTracker_GPS_rxCallback(RunTracker_GPS * const);
void RunTracker_GPS_sendCommand(RunTracker_GPS * const, const char *);
void RunTracker_GPS_pause(bool);
bool RunTracker_GPS_wakeup(void);
bool RunTracker_GPS_standby(void);
bool RunTracker_GPS_LOCUS_StartLogger(void);
bool RunTracker_GPS_LOCUS_StopLogger(void);
bool RunTracker_GPS_LOCUS_ReadStatus(void);

 */

void RunTracker_GPS_init(RunTracker_GPS * const GPS, UART_HandleTypeDef * huart)
{
  recvdflag 		= false;
  GPS->paused 		= false;
  lineidx 		= 0;
  currentline 		= line1;
  lastline 		= line2;

  GPS->hour 		= 0;
  GPS->minute		= 0;
  GPS->seconds		= 0;
  GPS->year 		= 0;
  GPS->month		= 0;
  GPS->day		= 0;
  GPS->fixquality	= 0;
  GPS->satellites	= 0;

  GPS->lat		= 0;
  GPS->lon		= 0;
  GPS->mag		= 0;

  GPS->fix		= false;
  GPS->milliseconds	= 0;

  GPS->latitude 	= 0.0f;
  GPS->longitude	= 0.0f;
  GPS->geoidheight	= 0.0f;
  GPS->altitude		= 0.0f;
  GPS->speed		= 0.0f;
  GPS->angle		= 0.0f;
  GPS->magvariation	= 0.0f;
  GPS->HDOP		= 0.0f;

  GPS->huart		= huart;
}

static bool RunTracker_parse(RunTracker_GPS * GPS, char * nmea) {
  // do checksum check

  // first look if we even have one
  if (nmea[strlen(nmea)-4] == '*') {
    uint16_t sum = parseHex(nmea[strlen(nmea)-3]) * 16;
    sum += parseHex(nmea[strlen(nmea)-2]);

    // check checksum
    for (uint8_t i=2; i < (strlen(nmea)-4); i++) {
      sum ^= nmea[i];
    }
    if (sum != 0) {
      // bad checksum :(
      return false;
    }
  }
  int32_t degree;
  long minutes;
  char degreebuff[10];
  // look for a few common sentences
  if (strstr(nmea, "$GPGGA")) {
    // found GGA
    char *p = nmea;
    // get time
    p = strchr(p, ',')+1;
    float timef = atof(p);
    uint32_t time = timef;
    GPS->hour = time / 10000;
    GPS->minute = (time % 10000) / 100;
    GPS->seconds = (time % 100);

    GPS->milliseconds = fmod(timef, 1.0) * 1000;

    // parse out latitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 2);
      p += 2;
      degreebuff[2] = '\0';
      degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      minutes = 50 * atol(degreebuff) / 3;
      GPS->latitude_fixed = degree + minutes;
      GPS->latitude = degree / 100000 + minutes * 0.000006F;
      GPS->latitudeDegrees = (GPS->latitude-100*(int)(GPS->latitude/100))/60.0;
      GPS->latitudeDegrees += (int)(GPS->latitude/100);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'S') GPS->latitudeDegrees *= -1.0;
      if (p[0] == 'N') GPS->lat = 'N';
      else if (p[0] == 'S') GPS->lat = 'S';
      else if (p[0] == ',') GPS->lat = 0;
      else return false;
    }

    // parse out longitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 3);
      p += 3;
      degreebuff[3] = '\0';
      degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      minutes = 50 * atol(degreebuff) / 3;
      GPS->longitude_fixed = degree + minutes;
      GPS->longitude = degree / 100000 + minutes * 0.000006F;
      GPS->longitudeDegrees = (GPS->longitude-100*(int)(GPS->longitude/100))/60.0;
      GPS->longitudeDegrees += (int)(GPS->longitude/100);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'W') GPS->longitudeDegrees *= -1.0;
      if (p[0] == 'W') GPS->lon = 'W';
      else if (p[0] == 'E') GPS->lon = 'E';
      else if (p[0] == ',') GPS->lon = 0;
      else return false;
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
	GPS->fixquality = atoi(p);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
	GPS->satellites = atoi(p);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
	GPS->HDOP = atof(p);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
	GPS->altitude = atof(p);
    }

    p = strchr(p, ',')+1;
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
	GPS->geoidheight = atof(p);
    }
    return true;
  }
  if (strstr(nmea, "$GPRMC")) {
   // found RMC
    char *p = nmea;

    // get time
    p = strchr(p, ',')+1;
    float timef = atof(p);
    uint32_t time = timef;
    GPS->hour = time / 10000;
    GPS->minute = (time % 10000) / 100;
    GPS->seconds = (time % 100);

    GPS->milliseconds = fmod(timef, 1.0) * 1000;

    p = strchr(p, ',')+1;
    // Serial.println(p);
    if (p[0] == 'A')
      GPS->fix = true;
    else if (p[0] == 'V')
      GPS->fix = false;
    else
      return false;

    // parse out latitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 2);
      p += 2;
      degreebuff[2] = '\0';
      long degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      long minutes = 50 * atol(degreebuff) / 3;
      GPS->latitude_fixed = degree + minutes;
      GPS->latitude = degree / 100000 + minutes * 0.000006F;
      GPS->latitudeDegrees = (GPS->latitude-100*(int)(GPS->latitude/100))/60.0;
      GPS->latitudeDegrees += (int)(GPS->latitude/100);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'S') GPS->latitudeDegrees *= -1.0;
      if (p[0] == 'N') GPS->lat = 'N';
      else if (p[0] == 'S') GPS->lat = 'S';
      else if (p[0] == ',') GPS->lat = 0;
      else return false;
    }

    // parse out longitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 3);
      p += 3;
      degreebuff[3] = '\0';
      degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      minutes = 50 * atol(degreebuff) / 3;
      GPS->longitude_fixed = degree + minutes;
      GPS->longitude = degree / 100000 + minutes * 0.000006F;
      GPS->longitudeDegrees = (GPS->longitude-100*(int)(GPS->longitude/100))/60.0;
      GPS->longitudeDegrees += (int)(GPS->longitude/100);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'W') GPS->longitudeDegrees *= -1.0;
      if (p[0] == 'W') GPS->lon = 'W';
      else if (p[0] == 'E') GPS->lon = 'E';
      else if (p[0] == ',') GPS->lon = 0;
      else return false;
    }
    // speed
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
	GPS->speed = atof(p);
    }

    // angle
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
	GPS->angle = atof(p);
    }

    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      uint32_t fulldate = atof(p);
      GPS->day = fulldate / 10000;
      GPS->month = (fulldate % 10000) / 100;
      GPS->year = (fulldate % 100);
    }
    // we dont parse the remaining, yet!
    return true;
  }

  return false;
}

void RunTracker_GPS_rxCallback(RunTracker_GPS * GPS)
{
  if(GPS->paused) return;

  //TODO This function should copy the rxbuffer to a local variable
  // and pass a message off to the RunTracker thread to tell it to parse
}

void RunTracker_GPS_sendCommand(RunTracker_GPS * GPS, const char * txBuffer)
{
  HAL_UART_Transmit_DMA(GPS->huart, txBuffer, strlen(txBuffer));
}
/*
static char RunTracker_GPS_read(RunTracker_GPS * const GPS, char c) {

  if (GPS->paused) return c;

  {
    if(!gpsHwSerial->available()) return c;
    c = gpsHwSerial->read();
  }

  if (c == '\n') {
    currentline[lineidx] = 0;

    if (currentline == line1) {
      currentline = line2;
      lastline = line1;
    } else {
      currentline = line1;
      lastline = line2;
    }

    lineidx = 0;
    recvdflag = true;
  }

  currentline[lineidx++] = c;
  if (lineidx >= MAXLINELENGTH)
    lineidx = MAXLINELENGTH-1;

  return c;
}*/

static uint8_t parseHex(char c) {
    if (c < '0')
      return 0;
    if (c <= '9')
      return c - '0';
    if (c < 'A')
       return 0;
    if (c <= 'F')
       return (c - 'A')+10;
    // if (c > 'F')
    return 0;
}
