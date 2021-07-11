#include "numpadbutton.h"

namespace Bones
{
#pragma region Constructors
	NumPadButton::NumPadButton()
	{
		this->Position = Bones::Position();
		this->Value = 0;
	}

	NumPadButton::NumPadButton(Bones::Position pos, byte val)
	{
		this->Position = pos;
		this->Value = val;
	}
#pragma endregion
}