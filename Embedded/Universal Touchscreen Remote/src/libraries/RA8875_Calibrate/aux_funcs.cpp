#include "aux_funcs.h"


/**************************************************************************/
/*!
    @brief  Waits for a touch event
*/
/**************************************************************************/
void waitForTouchEvent(Adafruit_RA8875 tft, tsPoint_t * point)
{
  /* Clear the touch data object and placeholder variables */
  memset(point, 0, sizeof(tsPoint_t));
  
  /* Clear any previous interrupts to avoid false buffered reads */
  uint16_t x, y;
  tft.touchRead(&x, &y);
  delay(1);

  /* Wait around for a new touch event (INT pin goes low) */
  while (digitalRead(RA8875_INT))
  {
  }
  
  /* Make sure this is really a touch event */
  if (tft.touched())
  {
    tft.touchRead(&x, &y);
    point->x = x;
    point->y = y;
    Serial.print("Touch: ");
    Serial.print(point->x); Serial.print(", "); Serial.println(point->y);
  }
  else
  {
    point->x = 0;
    point->y = 0;
  }
}



/**************************************************************************/
/*!
    @brief  Renders the calibration screen with an appropriately
            placed test point and waits for a touch event
*/
/**************************************************************************/
tsPoint_t renderCalibrationScreen(Adafruit_RA8875 tft, uint16_t x, uint16_t y, uint16_t radius)
{
  tft.fillScreen(RA8875_WHITE);
  tft.drawCircle(x, y, radius, RA8875_RED);
  tft.drawCircle(x, y, radius + 2, 0x8410);  /* 50% Gray */

  // Wait for a valid touch events
  tsPoint_t point = { 0, 0 };
  
  /* Keep polling until the TS event flag is valid */
  bool valid = false;
  while (!valid)
  {
    waitForTouchEvent(tft, &point);
    if (point.x || point.y) 
    {
      valid = true;
    }
  }
  
  return point;
}

/**************************************************************************/
/*!
    @brief  Starts the screen calibration process.  Each corner will be
            tested, meaning that each boundary (top, left, right and 
            bottom) will be tested twice and the readings averaged.
*/
/**************************************************************************/
void tsCalibrate(Adafruit_RA8875 tft, tsPoint_t _tsLCDPoints[], tsPoint_t _tsTSPoints[], tsMatrix_t *_tsMatrix)
{
  tsPoint_t data;

  /* --------------- Welcome Screen --------------- */
  Serial.println("Starting the calibration process");
  data = renderCalibrationScreen(tft, tft.width() / 2, tft.height() / 2, 5);
  delay(250);

  /* ----------------- First Dot ------------------ */
  // 10% over and 10% down
  data = renderCalibrationScreen(tft, tft.width() / 10, tft.height() / 10, 5);
  _tsLCDPoints[0].x = tft.width() / 10;
  _tsLCDPoints[0].y = tft.height() / 10;
  _tsTSPoints[0].x = data.x;
  _tsTSPoints[0].y = data.y;
  Serial.print("Point 1 - LCD");
  Serial.print(" X: ");
  Serial.print(_tsLCDPoints[0].x);
  Serial.print(" Y: ");
  Serial.print(_tsLCDPoints[0].y); 
  Serial.print(" TS X: ");
  Serial.print(_tsTSPoints[0].x); 
  Serial.print(" Y: ");
  Serial.println(_tsTSPoints[0].y); 
  delay(250);

  /* ---------------- Second Dot ------------------ */
  // 50% over and 90% down
  data = renderCalibrationScreen(tft, tft.width() / 2, tft.height() - tft.height() / 10, 5);
  _tsLCDPoints[1].x = tft.width() / 2;
  _tsLCDPoints[1].y = tft.height() - tft.height() / 10;
  _tsTSPoints[1].x = data.x;
  _tsTSPoints[1].y = data.y;
  Serial.print("Point 2 - LCD");
  Serial.print(" X: ");
  Serial.print(_tsLCDPoints[1].x);
  Serial.print(" Y: ");
  Serial.print(_tsLCDPoints[1].y);
  Serial.print(" TS X: ");
  Serial.print(_tsTSPoints[1].x);
  Serial.print(" Y: ");
  Serial.println(_tsTSPoints[1].y);
  delay(250);

  /* ---------------- Third Dot ------------------- */
  // 90% over and 50% down
  data = renderCalibrationScreen(tft, tft.width() - tft.width() / 10, tft.height() / 2, 5);
  _tsLCDPoints[2].x = tft.width() - tft.width() / 10;
  _tsLCDPoints[2].y = tft.height() / 2;
  _tsTSPoints[2].x = data.x;
  _tsTSPoints[2].y = data.y;
  Serial.print("Point 3 - LCD");
  Serial.print(" X: ");
  Serial.print(_tsLCDPoints[2].x);
  Serial.print(" Y: ");
  Serial.print(_tsLCDPoints[2].y);
  Serial.print(" TS X: ");
  Serial.print(_tsTSPoints[2].x);
  Serial.print(" Y: ");
  Serial.println(_tsTSPoints[2].y);
  delay(250);
  
  /* Clear the screen */
  tft.fillScreen(RA8875_WHITE);

  // Do matrix calculations for calibration and store to EEPROM
  setCalibrationMatrix(&_tsLCDPoints[0], &_tsTSPoints[0], _tsMatrix);
}
