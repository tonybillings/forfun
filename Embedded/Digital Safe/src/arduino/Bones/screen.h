#ifndef __bones_screen_H__
#define __bones_screen_H__

#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h>

namespace Bones
{
	class Screen
	{
	protected:
		Adafruit_TFTLCD* lcd;
	public:
		String Name;
	public:
		Screen() : Name("") {}
		Screen(String name) : Name(name) {}
	public:
		virtual void Activate() {};
		virtual void Deactivate() {};
		virtual void Update(bool screenPressed, TSPoint point) {};
		void SetLcd(Adafruit_TFTLCD* tftlcd)
		{
			lcd = tftlcd;
		};
	};
}

#endif
