#ifndef __UI__
#define __UI__

#include "Bitmap.h"
#include "Button.h"
#include "SwipeButton.h"

#include "Leaf.h"

#include "HeatIcon.h"
#include "CoolIcon.h"
#include "FanIcon.h"

#include "ModeText.h"
#include "ACText.h"
#include "HeatText.h"
#include "FanText.h"
#include "AutoText.h"
#include "OnText.h"

#include "NumberZero.h"
#include "NumberOne.h"
#include "NumberTwo.h"
#include "NumberThree.h"
#include "NumberFour.h"
#include "NumberFive.h"
#include "NumberSix.h"
#include "NumberSeven.h"
#include "NumberEight.h"
#include "NumberNine.h"

class HeatButton : public Button
{
private:
	HeatIcon iconHeat;
	
public:
	HeatButton()
		: Button(0, 0, 400, 75, 0xFC84, 0)
	{
		this->Icon = &iconHeat;
	}
};

class CoolButton : public Button
{
private:
	CoolIcon iconCool;
	
public:
	CoolButton()
		: Button(400, 0, 400, 75, 0x6EDF, 1)
	{
		this->Icon = &iconCool;
	}
};

class TempUpButton : public Button
{
public:
	TempUpButton()
		: Button(0, 405, 266, 75, 0xFB8E, 2)
	{
				
	}
};

class FanButton : public Button
{
private:
	FanIcon iconFan;
	
public:
	FanButton()
		: Button(266, 405, 266, 75, 0xA7E9, 3)
	{
		this->Icon = &iconFan;
	}
};

class TempDownButton : public Button
{
public:
	TempDownButton()
		: Button(532, 405, 266, 75, 0x739F, 4)
	{
		
	}
};

class LockScreenButton : public SwipeButton
{	
public:
	LockScreenButton()
		: SwipeButton(0, 90, 800, 300)
	{
		
	}
};

#endif
