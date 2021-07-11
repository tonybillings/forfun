#ifndef __bones_numpadbutton_H__
#define __bones_numpadbutton_H__
 
#include <Arduino.h>
#include "position.h"
 
namespace Bones
{
	class NumPadButton
	{
	public:
		Bones::Position		Position;
		byte				Value;
	public:
		NumPadButton();
		NumPadButton(Bones::Position pos, byte val);
	};
}

#endif
 
