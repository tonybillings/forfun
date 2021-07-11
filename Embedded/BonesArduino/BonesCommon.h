#ifndef __bones_common__
#define __bones_common__

/*** INCLUDES ******************************************************************************************************************/
#include <Arduino.h>
#include "BonesConfig.h"
#include "BList.h"
#include "BEvent.h"

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

struct BPosition
{
    int16_t X;
    int16_t Y;
    
    BPosition() : X(0), Y(0) {}
    BPosition(int16_t x, int16_t y) : X(x), Y(y) {}
};

struct BPositionF
{
    float X;
    float Y;
    
    BPositionF() : X(0), Y(0) {}
    BPositionF(float x, float y) : X(x), Y(y) {}
};

struct BSize
{
    int16_t Width;
    int16_t Height;
    
    BSize() : Width(0), Height(0) {}
    BSize(int16_t width, int16_t height) : Width(width), Height(height) {}
};

struct BColor
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
    
    BColor() : Value(0) {}
    BColor(uint16_t value) : Value(value) {}
};

struct BText
{
    String Value;
    BPosition Position;
    BColor Color;
    BColor BackgroundColor;
    BColor PressedColor;
    BColor PressedBackgroundColor;
    uint8_t Size;
    
    BText() : Value(""), Color(BColor::White), BackgroundColor(BColor::Black), PressedColor(BColor::White), PressedBackgroundColor(BColor::Black), Size(1) {}
    BText(String value) : Value(value), Color(BColor::White), BackgroundColor(BColor::Black), PressedColor(BColor::White), PressedBackgroundColor(BColor::Black), Size(1) {} 
    BText(const char* value) : Value(value), Color(BColor::White), BackgroundColor(BColor::Black), PressedColor(BColor::White), PressedBackgroundColor(BColor::Black), Size(1) {} 
};

struct BTriangle
{
	BPosition 	Position;
	BColor		Color;
	int16_t		Vertices[6];
	
	BTriangle()
	{
		Color = BColor::White;
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