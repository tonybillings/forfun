#ifndef __ModeText__
#define __ModeText__

#include <avr/pgmspace.h>
#include "RA8875.h"

PROGMEM static const uint16_t ModeText_strokes[] = {
2,
1025,
26,
1025,
115,
0,
1029,
22,
1029,
7,
1049,
7,
1052,
9,
1055,
0,
0,
1030,
20,
1030,
5,
1053,
4,
1055,
6,
1055,
1,
0,
1026,
1,
1027,
16,
1027,
1,
1026,
4,
1027,
23,
1027,
3,
1027,
24,
1027,
5,
1026,
30,
0,
1026,
2,
1027,
14,
1027,
2,
1026,
3,
1027,
26,
1026,
2,
1027,
26,
1026,
4,
1026,
30,
0,
1026,
3,
1027,
12,
1027,
3,
1026,
3,
1026,
27,
1026,
2,
1027,
26,
1026,
4,
1026,
30,
0,
1026,
5,
1026,
10,
1026,
5,
1026,
3,
1026,
27,
1026,
2,
1027,
26,
1026,
4,
1026,
30,
0,
1026,
6,
1027,
6,
1027,
6,
1026,
3,
1026,
27,
1026,
2,
1027,
26,
1026,
4,
1050,
6,
0,
1026,
7,
1027,
4,
1027,
7,
1026,
3,
1026,
27,
1026,
2,
1027,
26,
1026,
4,
1051,
5,
0,
1026,
8,
1027,
2,
1027,
8,
1026,
3,
1026,
27,
1026,
2,
1027,
26,
1026,
4,
1026,
30,
0,
1026,
10,
1030,
10,
1026,
3,
1026,
27,
1026,
2,
1027,
26,
1026,
4,
1026,
30,
0,
1026,
11,
1028,
11,
1026,
3,
1026,
27,
1026,
2,
1027,
26,
1026,
4,
1026,
30,
0,
1026,
28,
1026,
3,
1026,
27,
1026,
2,
1027,
26,
1026,
4,
1026,
30,
0,
1026,
28,
1026,
3,
1027,
26,
1026,
2,
1027,
26,
1026,
4,
1026,
30,
0,
1026,
28,
1026,
4,
1027,
23,
1027,
3,
1027,
24,
1027,
5,
1026,
30,
0,
1026,
28,
1026,
5,
1053,
4,
1027,
4,
1046,
6,
1055,
1,
0,
1026,
28,
1026,
7,
1049,
7,
1025,
4,
1045,
9,
1055,
0,
149
};

PROGMEM static const uint16_t ModeText_colors[] = {
63519,
65535
};

#include "Bitmap.h"

class ModeText : public Bitmap
{
public:
	ModeText()
	{
		Width = 150;
        Height = 18;
        TransparencyColor = 63519;
        StrokesArraySize = sizeof(ModeText_strokes) / sizeof(ModeText_strokes[0]);
        ColorsArraySize = sizeof(ModeText_colors) / sizeof(ModeText_colors[0]);
		Strokes = ModeText_strokes;
		Colors = ModeText_colors;
        LastPosX = LastPosY = -1;
        IsSmallBitmap = false;
	}
};

#endif
