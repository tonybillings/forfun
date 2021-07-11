#include "ra8875_calibration.h"
#include <EEPROM.h>

/**************************************************************************/
/*!
 Touchscreen Calibration Persistence Functions
 */
/**************************************************************************/

uint32_t eepromReadS32(int location){
    uint32_t value = ((uint32_t)EEPROM.read(location))<<24;
    value = value | ((uint32_t)EEPROM.read(location+1))<<16;
    value = value | ((uint32_t)EEPROM.read(location+2))<<8;
    value = value | ((uint32_t)EEPROM.read(location+3));
    return value;
}

/**************************************************************************/
/*!
 
 */
/**************************************************************************/
void eepromWriteS32(int location, int32_t value){
    EEPROM.write(location,   (value >> 24)&0xff);
    EEPROM.write(location+1, (value >> 16)&0xff);
    EEPROM.write(location+2, (value >> 8)&0xff);
    EEPROM.write(location+3, (value )&0xff);
}

/**************************************************************************/
/*!
 
 */
/**************************************************************************/
bool readCalibration(int location, tsMatrix_t * matrixPtr){
    if (location+sizeof(tsMatrix_t) > EEPROMSIZE){
        return false; //readCalibration::Calibration location outside of EEPROM memory bound
    }
    if (EEPROM.read(location+CFG_EEPROM_TOUCHSCREEN_CALIBRATED) == 1){
        matrixPtr->An = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_AN);
        matrixPtr->Bn = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_BN);
        matrixPtr->Cn = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_CN);
        matrixPtr->Dn = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_DN);
        matrixPtr->En = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_EN);
        matrixPtr->Fn = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_FN);
        matrixPtr->Divider = eepromReadS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_DIVIDER);
        return true;
    }
    return false;
}

/**************************************************************************/
/*!
 
 */
/**************************************************************************/
void writeCalibration(int location, tsMatrix_t * matrixPtr){
    if (location+sizeof(tsMatrix_t) < EEPROMSIZE){    // Check to see it calibration location outside of EEPROM memory bound
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_AN, matrixPtr->An);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_BN, matrixPtr->Bn);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_CN, matrixPtr->Cn);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_DN, matrixPtr->Dn);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_EN, matrixPtr->En);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_FN, matrixPtr->Fn);
        eepromWriteS32(location+CFG_EEPROM_TOUCHSCREEN_CAL_DIVIDER, matrixPtr->Divider);
        EEPROM.write(location+CFG_EEPROM_TOUCHSCREEN_CALIBRATED, 1);
    }
}


void writeCalibrationMatrix (tsMatrix_t* matrixPtr)
{
     eepromWriteS32(CFG_EEPROM_TOUCHSCREEN_CAL_AN, matrixPtr->An);
     eepromWriteS32(CFG_EEPROM_TOUCHSCREEN_CAL_BN, matrixPtr->Bn);
     eepromWriteS32(CFG_EEPROM_TOUCHSCREEN_CAL_CN, matrixPtr->Cn);
     eepromWriteS32(CFG_EEPROM_TOUCHSCREEN_CAL_DN, matrixPtr->Dn);
     eepromWriteS32(CFG_EEPROM_TOUCHSCREEN_CAL_EN, matrixPtr->En);
     eepromWriteS32(CFG_EEPROM_TOUCHSCREEN_CAL_FN, matrixPtr->Fn);
     eepromWriteS32(CFG_EEPROM_TOUCHSCREEN_CAL_DIVIDER, matrixPtr->Divider);
     EEPROM.write(CFG_EEPROM_TOUCHSCREEN_CALIBRATED, 1);
}

bool readCalibrationMatrix (tsMatrix_t* matrixPtr)
{
	// Read data to EEPROM
	matrixPtr->An = eepromReadS32(CFG_EEPROM_TOUCHSCREEN_CAL_AN);
	matrixPtr->Bn = eepromReadS32(CFG_EEPROM_TOUCHSCREEN_CAL_BN);
	matrixPtr->Cn = eepromReadS32(CFG_EEPROM_TOUCHSCREEN_CAL_CN);
	matrixPtr->Dn = eepromReadS32(CFG_EEPROM_TOUCHSCREEN_CAL_DN);
	matrixPtr->En = eepromReadS32(CFG_EEPROM_TOUCHSCREEN_CAL_EN);
	matrixPtr->Fn = eepromReadS32(CFG_EEPROM_TOUCHSCREEN_CAL_FN);
	matrixPtr->Divider = eepromReadS32(CFG_EEPROM_TOUCHSCREEN_CAL_DIVIDER);

	//other way to charge the matrix
	//_tft.writeCalibration(0, matrixPtr)

	if( matrixPtr->Divider == 0 )
		return false;
	return true;
}


/**************************************************************************/
/*!
    @brief Calculates the difference between the touch screen and the
           actual screen co-ordinates, taking into account misalignment
           and any physical offset of the touch screen.

    @note  This is based on the public domain touch screen calibration code
           written by Carlos E. Vidales (copyright (c) 2001).

           For more information, see the following app notes:

           - AN2173 - Touch Screen Control and Calibration
             Svyatoslav Paliy, Cypress Microsystems
           - Calibration in touch-screen systems
             Wendy Fang and Tony Chang,
             Analog Applications Journal, 3Q 2007 (Texas Instruments)
*/
/**************************************************************************/
int setCalibrationMatrix( tsPoint_t * displayPtr, tsPoint_t * screenPtr, tsMatrix_t * matrixPtr)
{
  int  retValue = 0;
  
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  
  if( matrixPtr->Divider == 0 )
  {
    retValue = -1 ;
  }
  else
  {
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
  
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
  
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
  
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
  
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;

    // Persist data to EEPROM
		writeCalibrationMatrix (matrixPtr);
  }


  return( retValue ) ;
} 

/**************************************************************************/
/*!
    @brief  Converts raw touch screen locations (screenPtr) into actual
            pixel locations on the display (displayPtr) using the
            supplied matrix.
            
    @param[out] displayPtr  Pointer to the tsPoint_t object that will hold
                            the compensated pixel location on the display
    @param[in]  screenPtr   Pointer to the tsPoint_t object that contains the
                            raw touch screen co-ordinates (before the
                            calibration calculations are made)
    @param[in]  matrixPtr   Pointer to the calibration matrix coefficients
                            used during the calibration process (calculated
                            via the tsCalibrate() helper function)

    @note  This is based on the public domain touch screen calibration code
           written by Carlos E. Vidales (copyright (c) 2001).
*/
/**************************************************************************/
int calibrateTSPoint( tsPoint_t * displayPtr, tsPoint_t * screenPtr, tsMatrix_t * matrixPtr )
{
  int  retValue = 0 ;
  
  if( matrixPtr->Divider != 0 )
  {
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;

    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    return -1;
  }

  return( retValue );
}
