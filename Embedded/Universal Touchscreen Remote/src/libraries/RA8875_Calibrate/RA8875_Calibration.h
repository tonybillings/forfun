/******************************************************************
 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source hardware
 by purchasing products from Adafruit!
 
 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
 ******************************************************************/

#ifndef _RA8875_CALIBRATION_
#define _RA8875_CALIBRATION_

#include <EEPROM.h>
#include "adafruit_ra8875.h"


// Touchscreen Calibration and EEPROM Storage Defines
#define CFG_EEPROM_TOUCHSCREEN_CAL_AN 0
#define CFG_EEPROM_TOUCHSCREEN_CAL_BN 4
#define CFG_EEPROM_TOUCHSCREEN_CAL_CN 8
#define CFG_EEPROM_TOUCHSCREEN_CAL_DN 12
#define CFG_EEPROM_TOUCHSCREEN_CAL_EN 16
#define CFG_EEPROM_TOUCHSCREEN_CAL_FN 20
#define CFG_EEPROM_TOUCHSCREEN_CAL_DIVIDER 24
#define CFG_EEPROM_TOUCHSCREEN_CALIBRATED 28

#if defined(__AVR_ATmega328P__)
#define EEPROMSIZE 1024
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define EEPROMSIZE 4096
#else
#define EEPROMSIZE 512
#endif


/**************************************************************************/
/*!
 Touchscreen Calibration Persistence Functions
 */
/**************************************************************************/
uint32_t eepromReadS32(int location);
void eepromWriteS32(int location, int32_t value);
bool readCalibration(int location, tsMatrix_t * matrixPtr);
void writeCalibration(int location, tsMatrix_t * matrixPtr);

void writeCalibrationMatrix (tsMatrix_t* matrixPtr);
bool readCalibrationMatrix (tsMatrix_t* matrixPtr);

/**************************************************************************/
/*
 Touchscreen Auxiliar Calibration Functions
*/
/**************************************************************************/
int setCalibrationMatrix( tsPoint_t * displayPtr, tsPoint_t * screenPtr, tsMatrix_t * matrixPtr);
int calibrateTSPoint( tsPoint_t * displayPtr, tsPoint_t * screenPtr, tsMatrix_t * matrixPtr );

#endif
