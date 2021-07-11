#ifndef __cr_common__
#define __cr_common__

/*** INCLUDES ******************************************************************************************************************/
#include <Arduino.h>
#include <EEPROM.h>
#include "IRLib.h"
#include "RA8875_Calibration.h"
#include "cr_config.h"
#include "cr_list.h"
#include "cr_event.h"

/*** DEFINES *******************************************************************************************************************/
#define COLOR(r,g,b) (((r>>3)<<11) | ((g>>2)<<5) | (b>>3))
#define	BLACK       0x0000
#define WHITE       0xFFFF
#define	RED         0xF800
#define	GREEN       0x07E0
#define	BLUE        0x001F
#define CYAN        0x07FF
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0  
#define ORANGE      0xFBE4
#define GRAY		0xC618

/*** TYPES *********************************************************************************************************************/

struct CRPosition
{
    int16_t X;
    int16_t Y;
    
    CRPosition() : X(0), Y(0) {}
    CRPosition(int16_t x, int16_t y) : X(x), Y(y) {}
};

struct CRPositionF
{
    float X;
    float Y;
    
    CRPositionF() : X(0), Y(0) {}
    CRPositionF(float x, float y) : X(x), Y(y) {}
};

struct CRSize
{
    int16_t Width;
    int16_t Height;
    
    CRSize() : Width(0), Height(0) {}
    CRSize(int16_t width, int16_t height) : Width(width), Height(height) {}
};

struct CRColor
{
    uint16_t Value;
    
    static const uint16_t Black       = BLACK;
    static const uint16_t White       = WHITE;
    static const uint16_t Red         = RED;
    static const uint16_t Green       = GREEN;
    static const uint16_t Blue        = BLUE;
    static const uint16_t Cyan        = CYAN;
    static const uint16_t Magenta     = MAGENTA;
    static const uint16_t Yellow      = YELLOW;
    static const uint16_t Orange      = ORANGE;
    static const uint16_t Gray	      = GRAY;
	 
    static uint16_t FromRGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        return (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3));
    }
    
    CRColor() : Value(0) {}
    CRColor(uint16_t value) : Value(value) {}
};

struct CRText
{
    String Value;
    CRPosition Position;
    CRColor Color;
    CRColor BackgroundColor;
    CRColor PressedColor;
    CRColor PressedBackgroundColor;
    uint8_t Size;
    
    CRText() : Value(""), Color(CRColor::White), BackgroundColor(CRColor::Black), PressedColor(CRColor::White), PressedBackgroundColor(CRColor::Black), Size(1) {}
    CRText(String value) : Value(value), Color(CRColor::White), BackgroundColor(CRColor::Black), PressedColor(CRColor::White), PressedBackgroundColor(CRColor::Black), Size(1) {} 
    CRText(const char* value) : Value(value), Color(CRColor::White), BackgroundColor(CRColor::Black), PressedColor(CRColor::White), PressedBackgroundColor(CRColor::Black), Size(1) {} 
};

struct CRTriangle
{
	CRPosition 	Position;
	CRColor		Color;
	int16_t		Vertices[6];
	
	CRTriangle()
	{
		Color = CRColor::White;
		memset(Vertices, 0, sizeof(Vertices));
	}
	
	void SetVertices(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
	{
		Vertices[0] = x0;
		Vertices[1] = y0;
		Vertices[2] = x1;
		Vertices[3] = y1;
		Vertices[4] = x2;
		Vertices[5] = y2;
	}
};

#endif