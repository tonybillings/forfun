#ifndef __SwipeButton__
#define __SwipeButton__

#include "Event.h"

class SwipeButton
{
public:
	int				ID;
	int				PosX;
	int				PosY;
	int				Width;
	int				Height;
	int				Right;
	int				Bottom;

protected:
	unsigned long	timeL;
	unsigned long	timeM;
	unsigned long	timeR;
	
public:
	SwipeButton()
	{
		ID = PosX = PosY = Width = Height = Right = Bottom = -1;
		timeL = timeM = timeR = 0;
	}

	SwipeButton(int posX, int posY, int width, int height, int id = -1)
	{
		ID = id;
		PosX = posX;
		PosY = posY;
		Width = width;
		Height = height;
		Right = posX + width;
		Bottom = posY + height;
		timeL = timeM = timeR = 0;
	}

public:
	void onScreenTouched(uint16_t x, uint16_t y)
	{
		if (x >= PosX && x <= Right && y >= PosY && y <= Bottom)
		{
			unsigned long currentTime = millis();
			
			if (x < 267)
			{
				if (currentTime - timeM < 600 && currentTime - timeR < 1200 && timeM > timeR)
				{
					timeL = timeM = timeR = 0;
					leftSwipe();
				}
				else
				{
					timeL = currentTime;
				}
			} 
			else if (x < 533)
			{
				timeM = currentTime;
			}
			else
			{
				if (currentTime - timeM < 600 && currentTime - timeL < 1200 && timeM > timeL)
				{
					timeL = timeM = timeR = 0;
					rightSwipe();
				}
				else
				{
					timeR = currentTime;
				}
			}
		}
	}

public:
	Event<> rightSwipe;
	Event<> leftSwipe;
};

#endif

