 #include "screencontroller.h"

using namespace Bones;

#pragma region Initialization
	Adafruit_TFTLCD ScreenController::lcd(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
	TouchScreen ScreenController::ts = TouchScreen(XP, YP, XM, YM, 300);
	List<Screen*> ScreenController::screens;
	Screen* ScreenController::activeScreen = 0;
#pragma endregion

#pragma region Core Actions
void ScreenController::Begin(int rotation)
{
	lcd.reset();
	uint16_t identifier = lcd.readID();
	lcd.begin(identifier);
	lcd.setRotation(rotation);
}
 
void ScreenController::Update()
{
	TSPoint p = ts.getPoint();
	p.x = map(p.x, TS_MINX, TS_MAXX, lcd.height(), 0);
	p.y = map(p.y, TS_MINY, TS_MAXY, lcd.height(), 0);
	bool screenPressed = p.z > MINPRESSURE && p.z < MAXPRESSURE;
	if (activeScreen != 0) { activeScreen->Update(screenPressed, p); }
	if (screenPressed) { delay(200); }
}
#pragma endregion

#pragma region Screen Actions
Screen* ScreenController::AddScreen(Screen* screen, bool activate)
{
	screen->SetLcd(&lcd);
	screens.Add(screen);
	if (activate) { ActivateScreen(screen); }
	return screen;
}

void ScreenController::ActivateScreen(String name)
{
	for (int i = 0; i < screens.Count(); i++)
	{
		Screen* screen = screens[i];

		if (screen->Name.equals(name))
		{
			activeScreen->Deactivate();
			Clear();
			activeScreen = screen;
			screen->Activate();
			break;
		}
	}
}

void ScreenController::ActivateScreen(Screen* screen)
{
	for (int i = 0; i < screens.Count(); i++)
	{
		if (screens[i] == screen)
		{
			if (activeScreen != 0) { activeScreen->Deactivate(); }
			Clear();
			activeScreen = screen;
			screen->Activate();
			break;
		}
	}
}

Screen* ScreenController::GetScreen(String name)
{
	for (int i = 0; i < screens.Count(); i++)
		if (screens[i]->Name.equals(name)) { return screens[i]; }
}
#pragma endregion

#pragma region Draw Actions
void ScreenController::Clear()
{
	lcd.fillScreen(BLACK);
}
#pragma endregion
