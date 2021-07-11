#ifndef __custom_remote__
#define __custom_remote__

/*** INCLUDES ************************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "cr_config.h"
#include "cr_screens.h"
#include "cr_screen_controller.h"

/*** MAIN INIT METHOD ****************************************************************************/

void InitializeCR()
{
    // TFT display 
    static Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
    
    // Initialize display
    if (!tft.begin(RA8875_800x480)) 
    {
        Serial.println("ERROR: RA8875 not found!");
        while(true);
    } 
    
    // Set display parameters
    tft.displayOn(true);
    tft.GPIOX(true);   
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); 
    tft.PWM1out(255);
    tft.touchEnable(true);
 
    // Initialize interrupt pin
    pinMode(RA8875_INT, INPUT);
    digitalWrite(RA8875_INT, HIGH);
    
    // Initialize screen controller
    ScreenController::Initialize(&tft, RA8875_INT);
    
    // Initialize screens by calling global function in cr_screens.h
	InitializeScreens(&tft);
}

#endif