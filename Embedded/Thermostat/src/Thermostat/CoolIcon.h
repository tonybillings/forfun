#ifndef __CoolIcon__
#define __CoolIcon__

#include <avr/pgmspace.h>
#include "RA8875.h"

PROGMEM static const uint8_t CoolIcon_strokes[] = {
19,
130,
19,
19,
130,
19,
19,
130,
19,
19,
130,
19,
19,
130,
19,
12,
129,
4,
130,
4,
129,
12,
11,
132,
2,
130,
2,
131,
12,
12,
133,
0,
130,
0,
133,
12,
8,
129,
3,
140,
3,
130,
7,
7,
130,
4,
138,
4,
130,
7,
7,
130,
6,
134,
6,
130,
7,
7,
130,
8,
130,
8,
130,
7,
0,
129,
4,
130,
8,
130,
8,
130,
4,
129,
0,
132,
2,
130,
8,
130,
8,
130,
2,
132,
0,
133,
0,
130,
8,
130,
8,
130,
0,
133,
0,
2,
135,
8,
130,
8,
135,
2,
3,
134,
8,
130,
8,
134,
3,
5,
133,
7,
130,
7,
133,
5,
5,
135,
5,
130,
5,
136,
4,
3,
138,
4,
130,
4,
139,
2,
2,
132,
2,
133,
2,
130,
2,
133,
3,
132,
1,
0,
133,
5,
133,
0,
130,
0,
133,
6,
133,
132,
8,
141,
10,
131,
0,
129,
12,
138,
15,
17,
134,
17,
17,
134,
17,
130,
12,
138,
12,
129,
0,
132,
8,
142,
9,
131,
0,
133,
4,
133,
1,
130,
1,
133,
5,
132,
0,
2,
132,
2,
133,
2,
130,
2,
133,
2,
133,
1,
3,
138,
4,
130,
4,
138,
3,
5,
134,
6,
130,
6,
134,
5,
5,
133,
7,
130,
8,
132,
5,
3,
134,
8,
130,
8,
134,
3,
1,
136,
8,
130,
8,
136,
1,
0,
132,
1,
130,
8,
130,
8,
130,
1,
132,
0,
132,
2,
130,
8,
130,
8,
130,
2,
132,
0,
129,
4,
130,
8,
130,
8,
130,
4,
129,
0,
7,
130,
7,
132,
7,
130,
7,
7,
130,
5,
135,
6,
130,
7,
7,
130,
4,
138,
4,
130,
7,
8,
129,
2,
142,
2,
130,
7,
12,
132,
1,
130,
1,
132,
12,
11,
132,
2,
130,
2,
132,
11,
12,
129,
4,
130,
4,
129,
12,
19,
130,
19,
19,
130,
19,
19,
130,
19,
19,
130,
19,
20,
128,
20
};

PROGMEM static const uint16_t CoolIcon_colors[] = {
63519,
0
};

#include "Bitmap.h"

class CoolIcon : public Bitmap
{
public:
	CoolIcon()
	{
		Width = 43;
        Height = 50;
        TransparencyColor = 63519;
        StrokesArraySize = sizeof(CoolIcon_strokes) / sizeof(CoolIcon_strokes[0]);
        ColorsArraySize = sizeof(CoolIcon_colors) / sizeof(CoolIcon_colors[0]);
		SmallStrokes = CoolIcon_strokes;
		Colors = CoolIcon_colors;
        LastPosX = LastPosY = -1;
        IsSmallBitmap = true;
	}
};

#endif
