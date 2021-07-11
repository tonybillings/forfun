#ifndef __bones_screencontroller_H__
#define __bones_screencontroller_H__

#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>
#include "screenbutton.h"
#include "screen.h"
#include "list.h"

#define YP A4 
#define XM A5 
#define YM 31 
#define XP 30 

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define SCREEN_ROT_LANDSCAPE 3

namespace Bones
{
	class ScreenController
	{
	private:
		static Adafruit_TFTLCD 	lcd;
		static TouchScreen 		ts;
		static List<Screen*>	screens;
		static Screen*			activeScreen;
	public:
		static void Clear();
		static void Begin(int rotation = SCREEN_ROT_LANDSCAPE);
		static void Update();
		static Screen* AddScreen(Screen* screen, bool activate = false);
		static void ActivateScreen(String name);
		static void ActivateScreen(Screen* screen);
		static Screen* GetScreen(String name);
	};
}

#endif
