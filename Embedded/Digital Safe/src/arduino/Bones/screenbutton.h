#ifndef __bones_screenbutton_H__
#define __bones_screenbutton_H__

#include "position.h"
#include "size.h"
#include "event.h"

namespace Bones
{
	class ScreenButton
	{
	private:
		int					maxX;
		int					maxY;
	public:
		Bones::Position		Position;
		Bones::Size			Size;
	public:
		Bones::Event<> ButtonPushed;
	public:
		ScreenButton() : Position(0), Size(0) {};
		ScreenButton(int x, int y, int width, int height);
	public:
		void ScreenPressed(int pX, int pY);
	};
}

#endif
