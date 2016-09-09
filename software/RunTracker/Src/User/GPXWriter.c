//TODO Implement GPX Writer
#include "GPXWriter.h"
#include "rtc.h"
#include <string.h>

static const char* GPXHeader = "<gpx creator=\"RunTracker\" version=\"1.0\" xmlns=\"http://www.topografix.com/GPX/1/1\" "
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 "
		"http://www.topografix.com/GPX/1/1/gpx.xsd "
		"http://www.garmin.com/xmlschemas/GpxExtensions/v3 "
		"http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd "
		"http://www.garmin.com/xmlschemas/TrackPointExtension/v1 "
		"http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd "
		"http://www.garmin.com/xmlschemas/GpxExtensions/v3 "
		"http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd "
		"http://www.garmin.com/xmlschemas/TrackPointExtension/v1 "
		"http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd "
		"http://www.garmin.com/xmlschemas/GpxExtensions/v3 "
		"http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd "
		"http://www.garmin.com/xmlschemas/TrackPointExtension/v1 "
		"http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd "
		"http://www.garmin.com/xmlschemas/GpxExtensions/v3 "
		"http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd "
		"http://www.garmin.com/xmlschemas/TrackPointExtension/v1 "
		"http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd "
		"http://www.garmin.com/xmlschemas/GpxExtensions/v3 "
		"http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd "
		"http://www.garmin.com/xmlschemas/TrackPointExtension/v1 "
		"http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd"
		"\" xmlns:gpxtpx=\"http://www.garmin.com/xmlschemas/TrackPointExtension/v1"
		"\" xmlns:gpxx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\">\n";


// Public Interfaces

// Structure Initialization Routine
GPX_Return_Type GPXWriter_Init(GPXWriter * writer)
{
  writer->gpxFile = NULL;
  writer->logTemperature = false;
  writer->logHeartrate = false;
  writer->logCadence = false;
  writer->logElevation = false;

  writer->temperature = 0;
  writer->heartrate = 0;
  writer->cadence = 0;
  writer->lat = 0.0f;
  writer->lon = 0.0f;
  writer->elevation = 0.0f;

  osMutexDef(dataMutex);
  writer->dataMutex = osMutexCreate(osMutex(dataMutex));

  osMutexDef(fileMutex);
  writer->fileMutex = osMutexCreate(osMutex(fileMutex));

  return GPX_SUCCESS;
}

// Setter Functions
GPX_Return_Type GPXWriter_SetTemperature(GPXWriter * writer, uint8_t temperature)
{
  if (osMutexWait(writer->dataMutex, osWaitForever) != osOK)
  {
    return GPX_MUTEX_FAIL;
  }

  writer->temperature = temperature;

  osMutexRelease(writer->dataMutex);

  return GPX_SUCCESS;
}

GPX_Return_Type GPXWriter_SetHeartrate(GPXWriter * writer, uint8_t heartrate)
{
  if (osMutexWait(writer->dataMutex, osWaitForever) != osOK)
  {
    return GPX_MUTEX_FAIL;
  }

  writer->heartrate = heartrate;

  osMutexRelease(writer->dataMutex);

  return GPX_SUCCESS;
}

GPX_Return_Type GPXWriter_SetCadence(GPXWriter * writer, uint8_t cadence)
{
  if (osMutexWait(writer->dataMutex, osWaitForever) != osOK)
  {
    return GPX_MUTEX_FAIL;
  }

  writer->cadence = cadence;

  osMutexRelease(writer->dataMutex);

  return GPX_SUCCESS;
}

GPX_Return_Type GPXWriter_SetPosition(GPXWriter * writer, float lat, float lon, float elevation)
{
  if (osMutexWait(writer->dataMutex, osWaitForever) != osOK)
  {
    return GPX_MUTEX_FAIL;
  }

  writer->lat = lat;
  writer->lon = lon;
  writer->elevation = elevation;

  osMutexRelease(writer->dataMutex);

  return GPX_SUCCESS;
}

// Create a trackpoint and write to the log
GPX_Return_Type GPXWriter_Update(GPXWriter * writer)
{
  GPXTrackPoint track;
  RTC_TimeTypeDef currTime;
  RTC_DateTypeDef currDate;

  HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);

  track.year    = currDate.Year;
  track.month   = currDate.Month;
  track.day     = currDate.Date;
  track.hour    = currTime.Hours;
  track.minute  = currTime.Minutes;
  track.seconds = currTime.Seconds;

  if (osMutexWait(writer->dataMutex, osWaitForever) != osOK)
  {
    return GPX_MUTEX_FAIL;
  }

  track.cadence = writer->cadence;
  track.latitudeDegrees = writer->lat;
  track.longitudeDegrees = writer->lon;
  track.elevation = writer->elevation;
  track.temperature = writer->temperature;
  track.heartrate = writer->heartrate;
  track.cadence = writer->cadence;

  osMutexRelease(writer->dataMutex);

  return GPXWriter_WritePoint(writer, &track);
}

// Start a new GPX Log
GPX_Return_Type GPXWriter_StartLog(GPXWriter * writer)
{
  FRESULT res;

  RTC_TimeTypeDef currTime;
  RTC_DateTypeDef currDate;

  char printBuffer[80];

  if (osMutexWait(writer->fileMutex, osWaitForever) != osOK)
  {
    return GPX_MUTEX_FAIL;
  }

  // Allocate memory for the file handle
  writer->gpxFile = pvPortMalloc(sizeof(FIL));
  if(!writer->gpxFile)
  {
    // TODO Log error
    return GPX_FILE_ALLOC_FAIL;
  }

  // Open a file based on current system time
  HAL_RTC_GetTime(&hrtc, &currTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &currDate, RTC_FORMAT_BIN);

  sprintf(printBuffer, "%s/%04d-%02d-%02dT%02d:%02d:%02d.gpx", "GPX",
                                                             currDate.Year,
                                                             currDate.Month,
                                                             currDate.Date,
                                                             currTime.Hours,
                                                             currTime.Minutes,
                                                             currTime.Seconds);
  res = f_open(writer->gpxFile, printBuffer, FA_WRITE);
  if(res != FR_OK)
  {
    // TODO Log error
    return GPX_FILE_OPEN_FAIL;
  }

  // Write the XML header info
  res = f_printf(writer->gpxFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, GPXHeader);
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, " <metadata>\n");
  configASSERT(res == FR_OK);

  // TODO Time should probably be in UTC (Zulu)
  res = f_printf(writer->gpxFile, "  <time>%04d-%02d-%02dT%02d:%02d:%02dZ</time>\n", currDate.Year,
                                                                                     currDate.Month,
                                                                                     currDate.Date,
                                                                                     currTime.Hours,
                                                                                     currTime.Minutes,
                                                                                     currTime.Seconds);
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, " </metadata>\n");
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, " <trk>\n");
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, "  <name>RunTracker GPS Log</name>\n");
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, "  <trkseg>\n");
  configASSERT(res == FR_OK);

  res = f_sync(writer->gpxFile);
  configASSERT(res == FR_OK);

  osMutexRelease(writer->fileMutex);

  return GPX_SUCCESS;
}

// Write a track point to the GPX log
GPX_Return_Type GPXWriter_WritePoint(GPXWriter * writer, GPXTrackPoint * point)
{
  FRESULT res;

  if (osMutexWait(writer->fileMutex, osWaitForever) != osOK)
  {
    return GPX_MUTEX_FAIL;
  }

  // Make sure we don't have a NULL file pointer
  configASSERT(writer->gpxFile);

  // Write out the GPX track point data
  res = f_printf(writer->gpxFile, "   <trkpt lat=\"%f\" lon=\"%f\">\n", point->latitudeDegrees,
                                                                        point->longitudeDegrees);
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, "    <time>%04d-%02d-%02dT%02d:%02d:%02dZ</time>\n", point->year,
                                                                                       point->month,
                                                                                       point->day,
                                                                                       point->hour,
                                                                                       point->minute,
                                                                                       point->seconds);
  configASSERT(res == FR_OK);

  if(writer->logElevation)
  {
    res = f_printf(writer->gpxFile, "    <ele>%f</ele>\n", point->elevation);
    configASSERT(res == FR_OK);
  }

  if(writer->logCadence || writer->logHeartrate || writer->logTemperature)
  {
    res = f_printf(writer->gpxFile, "    <extensions>\n");
    configASSERT(res == FR_OK);

    res = f_printf(writer->gpxFile, "     <gpxtpx:TrackPointExtension>\n");
    configASSERT(res == FR_OK);

    if(writer->logTemperature)
    {
      res = f_printf(writer->gpxFile, "      <gpxtpx:atemp>%d</gpxtpx:atemp>\n", point->temperature);
      configASSERT(res == FR_OK);
    }

    if(writer->logHeartrate)
    {
      res = f_printf(writer->gpxFile, "      <gpxtpx:hr>%d</gpxtpx:hr>\n", point->heartrate);
      configASSERT(res == FR_OK);
    }

    if(writer->logCadence)
    {
      res = f_printf(writer->gpxFile, "      <gpxtpx:cad>%d</gpxtpx:cad>\n", point->cadence);
      configASSERT(res == FR_OK);
    }

    res = f_printf(writer->gpxFile, "     </gpxtpx:TrackPointExtension>\n");
    configASSERT(res == FR_OK);

    res = f_printf(writer->gpxFile, "    </extensions>\n");
    configASSERT(res == FR_OK);
  }

  res = f_printf(writer->gpxFile, "   </trkpt>\n");
  configASSERT(res == FR_OK);

  res = f_sync(writer->gpxFile);
  configASSERT(res == FR_OK);

  osMutexRelease(writer->fileMutex);

  return GPX_SUCCESS;
}

// Close a GPX log file
GPX_Return_Type GPXWriter_StopLog(GPXWriter * writer)
{
  FRESULT res;

  if (osMutexWait(writer->fileMutex, osWaitForever) != osOK)
  {
    return GPX_MUTEX_FAIL;
  }

  // Make sure we don't have a NULL file pointer
  configASSERT(writer->gpxFile);

  res = f_printf(writer->gpxFile, "  </trkseg>\n");
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, " </trk>\n");
  configASSERT(res == FR_OK);

  res = f_printf(writer->gpxFile, "</gpx>\n");
  configASSERT(res == FR_OK);

  res = f_close(writer->gpxFile);
  configASSERT(res == FR_OK);

  osMutexRelease(writer->fileMutex);

  return GPX_SUCCESS;
}

