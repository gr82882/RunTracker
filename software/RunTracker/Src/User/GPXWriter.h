/*
 * GPXWriter.h
 *
 *  Created on: Sep 2, 2016
 *      Author: G. Andrews
 */

#ifndef GPXWRITER_H_
#define GPXWRITER_H_

#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include <stdbool.h>

// Define a GPXWriter's attributes
typedef struct {
  FIL *gpxFile;

  bool logTemperature;
  bool logHeartrate;
  bool logCadence;
  bool logElevation;

  osMutexId fileMutex;

} GPXWriter;

typedef struct
{
  float latitudeDegrees, longitudeDegrees;
  float elevation;
  uint8_t temperature;
  uint8_t heartrate;
  uint8_t cadence;
  uint8_t hour, minute, seconds, year, month, day;
} GPXTrackPoint;

typedef enum {
  GPX_SUCCESS,
  GPX_FILE_ALLOC_FAIL,
  GPX_FILE_OPEN_FAIL,
  GPX_MUTEX_FAIL
} GPX_Return_Type;

// Public interface functions
GPX_Return_Type GPXWriter_Init(GPXWriter *);
GPX_Return_Type GPXWriter_StartLog(GPXWriter *);
GPX_Return_Type GPXWriter_StopLog(GPXWriter *);
GPX_Return_Type GPXWriter_WritePoint(GPXWriter *, GPXTrackPoint *);


#endif


/* Example Strava GPX File ...

<?xml version="1.0" encoding="UTF-8"?>
<gpx creator="StravaGPX" version="1.1" xmlns="http://www.topografix.com/GPX/1/1" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd http://www.garmin.com/xmlschemas/TrackPointExtension/v1 http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd http://www.garmin.com/xmlschemas/TrackPointExtension/v1 http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd http://www.garmin.com/xmlschemas/TrackPointExtension/v1 http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd http://www.garmin.com/xmlschemas/TrackPointExtension/v1 http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd http://www.garmin.com/xmlschemas/TrackPointExtension/v1 http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd" xmlns:gpxtpx="http://www.garmin.com/xmlschemas/TrackPointExtension/v1" xmlns:gpxx="http://www.garmin.com/xmlschemas/GpxExtensions/v3">
 <metadata>
  <time>2014-06-15T15:45:39Z</time>
 </metadata>
 <trk>
  <name>Example GPX file from Strava</name>
  <trkseg>
   <trkpt lat="50.8758450" lon="4.6710150">
    <ele>60.0</ele>
    <time>2014-06-15T15:45:39Z</time>
    <extensions>
     <gpxtpx:TrackPointExtension>
      <gpxtpx:atemp>25</gpxtpx:atemp>
      <gpxtpx:hr>107</gpxtpx:hr>
      <gpxtpx:cad>65</gpxtpx:cad>
     </gpxtpx:TrackPointExtension>
    </extensions>
   </trkpt>
... Lots of track points
  </trkseg>
 </trk>
</gpx>

 */
