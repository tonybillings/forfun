#ifndef __Button__
#define __Button__

#include "RA8875.h"
#include "Event.h"
#include "Bitmap.h"

class Button
{
public:
    static RA8875* TFT;
	
public:
	int			ID;
	int			PosX;
	int			PosY;
	int			Width;
	int			Height;
	int			Right;
	int			Bottom;
	uint16_t	Color;
	Bitmap*		Icon;

public:
	Button()
	{
		ID = PosX = PosY = Width = Height = Right = Bottom = -1;
		Color = 0;
		Icon = nullptr;
	}

	Button(int posX, int posY, int width, int height, uint16_t color, int id = -1)
	{
		ID = id;
		PosX = posX;
		PosY = posY;
		Width = width;
		Height = height;
		Right = posX + width;
		Bottom = posY + height;
		Color = color;
		Icon = nullptr;
	}

public:
	static void draw(Button& btn)
	{
		if (TFT == nullptr) return;
		TFT->fillRect(btn.PosX, btn.PosY, btn.Width, btn.Height, btn.Color);

		if (btn.Icon != nullptr)
		{
			int posX = (btn.PosX + (btn.Width * 0.5f)) - (btn.Icon->Width * 0.5f);
			int posY = (btn.PosY + (btn.Height * 0.5f)) - (btn.Icon->Height * 0.5f);
			Bitmap::draw(btn.Icon, posX, posY);
		}
	}

	static void onScreenTouched(Button& btn, uint16_t x, uint16_t y)
	{
		if (x >= btn.PosX && x <= btn.Right && y >= btn.PosY && y <= btn.Bottom)
			btn.clicked(&btn);
	}

public:
	Event<Button*> clicked;
};

RA8875* Button::TFT = nullptr;

#endif

