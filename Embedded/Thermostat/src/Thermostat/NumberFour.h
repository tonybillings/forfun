#ifndef __NumberFour__
#define __NumberFour__

#include <avr/pgmspace.h>
#include "RA8875.h"

PROGMEM static const uint8_t NumberFour_strokes[] = {
96,
38,
145,
25,
140,
0,
37,
146,
25,
141,
36,
148,
24,
141,
35,
148,
25,
141,
34,
148,
26,
141,
33,
148,
27,
141,
32,
148,
28,
141,
31,
148,
29,
141,
30,
148,
30,
141,
29,
148,
31,
141,
28,
148,
32,
141,
27,
148,
33,
141,
26,
148,
34,
141,
25,
148,
35,
141,
24,
148,
36,
141,
24,
147,
37,
141,
23,
147,
38,
141,
22,
147,
39,
141,
21,
147,
40,
141,
20,
147,
41,
141,
19,
147,
42,
141,
18,
147,
43,
141,
17,
147,
44,
141,
16,
147,
45,
141,
15,
147,
46,
141,
14,
147,
47,
141,
13,
147,
48,
141,
12,
147,
49,
141,
11,
147,
50,
141,
10,
147,
51,
141,
9,
147,
52,
141,
8,
147,
53,
141,
7,
147,
54,
141,
6,
147,
55,
141,
5,
147,
56,
141,
4,
147,
57,
141,
3,
147,
5,
194,
2,
147,
5,
195,
1,
147,
5,
196,
0,
147,
5,
197,
0,
146,
5,
198,
146,
5,
199,
145,
5,
200,
144,
5,
201,
143,
5,
202,
0,
141,
5,
203,
0,
140,
5,
204,
0,
139,
5,
205,
1,
137,
6,
205,
3,
134,
8,
204,
6,
130,
10,
203,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
82,
141,
83,
138,
1
};

PROGMEM static const uint16_t NumberFour_colors[] = {
63519,
65535
};

#include "Bitmap.h"

class NumberFour : public Bitmap
{
public:
	NumberFour()
	{
		Width = 97;
        Height = 77;
        TransparencyColor = 63519;
        StrokesArraySize = sizeof(NumberFour_strokes) / sizeof(NumberFour_strokes[0]);
        ColorsArraySize = sizeof(NumberFour_colors) / sizeof(NumberFour_colors[0]);
		SmallStrokes = NumberFour_strokes;
		Colors = NumberFour_colors;
        LastPosX = LastPosY = -1;
		IsSmallBitmap = true;
	}
};

#endif
