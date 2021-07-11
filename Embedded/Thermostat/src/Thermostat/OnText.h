#ifndef __OnText__
#define __OnText__

#include <avr/pgmspace.h>
#include "RA8875.h"

PROGMEM static const uint8_t OnText_strokes[] = {
72,
3,
153,
7,
133,
25,
130,
1,
157,
4,
136,
23,
130,
0,
131,
23,
131,
3,
131,
1,
132,
21,
130,
131,
26,
130,
2,
131,
3,
131,
20,
130,
130,
27,
130,
2,
131,
5,
131,
18,
130,
130,
27,
130,
2,
131,
6,
132,
16,
130,
130,
27,
130,
2,
131,
8,
132,
14,
130,
130,
27,
130,
2,
131,
10,
132,
12,
130,
130,
27,
130,
2,
131,
12,
131,
11,
130,
130,
27,
130,
2,
131,
14,
131,
9,
130,
130,
27,
130,
2,
131,
16,
131,
7,
130,
130,
27,
130,
2,
131,
17,
132,
5,
130,
131,
26,
130,
2,
131,
19,
132,
3,
130,
0,
131,
23,
131,
3,
131,
21,
132,
1,
130,
1,
157,
4,
131,
23,
135,
3,
153,
7,
129,
26,
132,
0,
72
};

PROGMEM static const uint16_t OnText_colors[] = {
63519,
65535
};

#include "Bitmap.h"

class OnText : public Bitmap
{
public:
	OnText()
	{
		Width = 73;
        Height = 18;
        TransparencyColor = 63519;
        StrokesArraySize = sizeof(OnText_strokes) / sizeof(OnText_strokes[0]);
        ColorsArraySize = sizeof(OnText_colors) / sizeof(OnText_colors[0]);
		SmallStrokes = OnText_strokes;
		Colors = OnText_colors;
        LastPosX = LastPosY = -1;
        IsSmallBitmap = true;
	}
};

#endif
