#ifndef __bones__
#define __bones__

/*** INCLUDES ************************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "BonesConfig.h"
#include "BonesScreens.h"
#include "BScreenController.h"

/*** MAIN INIT METHOD ****************************************************************************/

void InitializeBones()
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
    BScreenController::Initialize(&tft, RA8875_INT);
    
    // Initialize screens by calling global function in cr_screens.h
	InitializeScreens(&tft);
}

#endif