#include "screenbutton.h"

using namespace Bones;

#pragma region Constructors
ScreenButton::ScreenButton(int x, int y, int width, int height)
{
	Position = Bones::Position(x, y);
	Size = Bones::Size(width, height);
	maxX = x + width;
	maxY = y + height;
}
#pragma endregion

#pragma region Screen Actions
void ScreenButton::ScreenPressed(int x, int y)
{
	if (x > Position.X && x < maxX && y > Position.Y && y < maxY)
		ButtonPushed();
}
#pragma endregion


