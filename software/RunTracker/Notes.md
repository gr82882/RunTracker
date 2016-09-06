## GPS Module
- Run DMA in circular mode with a large-ish buffer (~1.5X a NMEA message?)
- Interrupt on half and full DMA complete
  - Copy the half into a ring buffer in the GPS module
    - Maybe we can complete processing the data before next DMA complete to prevent having to copy?
  - DMA continues to fill the other half while you are copying
- GPS module processes ring buffer 

## Mission Manager
- Do we need this?  What does it do?
- Probaly should maintain a state machine
  - Are we playing music, streaming bluetooth, etc.
  - Something needs to grab data from sensors (e.g. altimeter) and pass them off (e.g. GPX logger)
- Handle button press interrupts and shuttle them off to the appropriate thread/module

## OLED Display
- Haven't really thought about this
- Need some kind of window manager to scroll through songs, settings, etc.
- A clock / homescreen would be nice

## VS1053 (Audio Codec)
- This is on SPI2 (dedicated)
- Need an interrupt handler for DREQ that then does DMA transfer of file data via SPI2
- Need to enable I2S output when bluetooth is enabled

## LSM6DS3 (IMU)
- On SPI1 (shared with altimeter & BC127)
- Do we want this for anything other than a Pedometer?  (E.g. tilt sensor)
 - Tap/Double Tap for song skip?
- Has an 8KB FIFO to buffer up step counter & timestamp data

## LPS22HB (Altimeter)
- On SPI1 (shared with IMU and BC127)
- Has a 32-slit FIFO

## BC127 (Bluetooth)
- On SPI1 (shared with IMU and altimeter)
- SPI only used for commands.  Data should be coming across on I2S out of the VS1053
- Hopefully can interface with heartrate monitor and bluetooth headphones

## Power Management
- Assume a 100mAh battery [like this one](https://www.adafruit.com/products/1570)
- 11.5mm x 31 x 3.8mm might tuck in nicely on the back side of the board
 
- Bluetooth is <1mA (connectable) and 15mA (streaming).  Unknown when sleeping
- GPS is 25mA (acquisition) and 20mA (tracking)
- Processor is 100uA/MHz.  12uA for RTC only
- Audio is 11mA @ 3.3V (analog) and 11mA @ 1.8V (digital).  ~12uA in reset
- Accelerometer is ~24uA in low-power mode (FIFO enabled, I assume)
- Altimeter is 3-12uA depending on LC_EN bit (Low Current Enable)
- microSD is maybe 20mA when reading?
- OLED I_CI = ~240uA.  I_CC = 23-33mA (13V) (50-100% illuminated).  Sleep is 2uA

## GPS / Audio Mode:
- 20mA for GPS
- 20mA for CPU/SDIO?
- 22mA for Audio CODEC?
- 1 mA for Pedometer / Altimeter?
- OLED??
- Yikes... that's ~1hr of operation (no good)




