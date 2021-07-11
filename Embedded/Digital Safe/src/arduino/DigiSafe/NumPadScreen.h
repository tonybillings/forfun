#ifndef __digisafe_numpadscreen_H__
#define __digisafe_numpadscreen_H__

#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>
#include <bones.h>

namespace DigiSafe
{
	class NumPadScreen : public Bones::Screen
	{
	private:
		Bones::NumPadButton		numpadButtons[16];
		Bones::ScreenButton		screenButtons[4];
	private:
		void Draw();
		void DrawNumPad();
		void DrawButton(Bones::ScreenButton &button);
		void DrawNumPadButton(int x, int y);
		void DrawPushedNumPadButton(int number, int count);
		void ResetNumPad();
	public:
		NumPadScreen() {}
		NumPadScreen(String name) : Screen(name) {}
	public:
		void Activate() {};
		void Deactivate() {};
		void Update(bool screenPressed, TSPoint point);
	};
}

#endif


