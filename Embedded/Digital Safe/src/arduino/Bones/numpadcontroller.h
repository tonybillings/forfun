#ifndef __bones_numpadcontroller_H__
#define __bones_numpadcontroller_H__

#include <Adafruit_Trellis.h>
#include "numpadbutton.h"
#include "event.h"

#define INTPIN 19
#define MOMENTARY 0
#define LATCHING 1
#define MODE LATCHING 
#define NUMTRELLIS 1
#define NUMKEYS (NUMTRELLIS * 16)

namespace Bones
{
	class NumPadController
	{
	private:
		static byte					count;
		static Adafruit_Trellis		matrix0;
		static Adafruit_TrellisSet	trellis;
		static bool					enabled;
	public:
		static void Begin();
		static void Update();
		static void Clear();
		static void SetEnabled(bool value);
		static Bones::Event<int, int> ButtonPushed;
	};
}

#endif
