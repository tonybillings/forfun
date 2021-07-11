#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <Adafruit_Trellis.h>
#include <bones.h>
#include "Screens.h"

using namespace Bones;
using namespace DigiSafe;

void setup(void)
{
	SerialController::Begin();
	ScreenController::Begin();
	NumPadController::Begin();

	ScreenController::AddScreen(new MainScreen("Main"), true);
	//ScreenController::AddScreen(new CodeScreen("Code"));
	//ScreenController::AddScreen(new NumPadScreen("NumPad"));
}

void loop()
{
	ScreenController::Update();
	SerialController::Update();
	NumPadController::Update();
	delay(30);
}
