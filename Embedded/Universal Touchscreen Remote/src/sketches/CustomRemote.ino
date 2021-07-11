/*** INCLUDES **********************************************************************************************************************/

#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "RA8875_Calibration.h"
#include "IRLib.h"
#include "CustomRemote.h"

/*** SETUP *************************************************************************************************************************/

void setup() 
{
    // Initialize custom remote library
    InitializeCR(); 
}

/*** LOOP **************************************************************************************************************************/

void loop()
{   
    // Update current screen
    ScreenController::Update();
    
    // Delay to prevent rapid fire commands
    delay(10);
}
