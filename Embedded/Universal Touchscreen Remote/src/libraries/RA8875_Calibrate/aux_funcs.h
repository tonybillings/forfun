/******************************************************************
 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source hardware
 by purchasing products from Adafruit!
 
 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
 ******************************************************************/

#ifndef _AUX_FUNCS_
#define _AUX_FUNCS_

#include "ra8875_calibration.h"

#define RA8875_INT 3
#define RA8875_CS 53
#define RA8875_RESET 48

void tsCalibrate(Adafruit_RA8875 tft, tsPoint_t _tsLCDPoints[], tsPoint_t _tsTSPoints[], tsMatrix_t *_tsMatrix);
void waitForTouchEvent(Adafruit_RA8875 tft, tsPoint_t * point);
tsPoint_t renderCalibrationScreen(Adafruit_RA8875 tft, uint16_t x, uint16_t y, uint16_t radius);

#endif
