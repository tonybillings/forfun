#include "numpadcontroller.h"

namespace Bones
{
#pragma region Initialization
	Adafruit_Trellis NumPadController::matrix0 = Adafruit_Trellis();
	Adafruit_TrellisSet NumPadController::trellis = Adafruit_TrellisSet(&matrix0);
	byte NumPadController::count(0);
	bool NumPadController::enabled = true;
	Event<int, int> NumPadController::ButtonPushed;
#pragma endregion

#pragma region Setters
	void NumPadController::SetEnabled(bool value)
	{
		enabled = value;
	}
#pragma endregion

#pragma region Core Actions
	void NumPadController::Begin()
	{
		pinMode(INTPIN, INPUT);
		digitalWrite(INTPIN, HIGH);
		trellis.begin(0x70);
		Clear();
	}

	void NumPadController::Update()
	{
		if (!enabled) { return; }

		if (MODE == LATCHING)
		{
			if (trellis.readSwitches())
			{
				for (byte i = 0; i < NUMKEYS; i++)
				{
					if (trellis.justPressed(i))
					{
						if (!trellis.isLED(i))
						{
							trellis.setLED(i);
							ButtonPushed(i, ++count);
						}
					}
				}
				trellis.writeDisplay();
			}
		}
	}
#pragma endregion

#pragma region Button Actions
	void NumPadController::Clear()
	{
		count = 0;

		for (byte i = 0; i < NUMKEYS; i++)
			trellis.clrLED(i);

		trellis.writeDisplay();
	}
#pragma endregion
}
 
 
