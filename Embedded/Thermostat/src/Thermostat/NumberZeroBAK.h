#ifndef __NumberZero__
#define __NumberZero__

#include <avr/pgmspace.h>
#include "RA8875.h"

PROGMEM static const uint16_t NumberZero_strokes[] = {
106,
24,
1048,
5,
1049,
24,
20,
1053,
4,
1052,
21,
18,
1055,
3,
1056,
18,
16,
1057,
3,
1058,
16,
14,
1059,
3,
1060,
14,
13,
1060,
3,
1061,
13,
11,
1062,
3,
1062,
12,
10,
1063,
3,
1064,
10,
9,
1064,
3,
1065,
9,
8,
1065,
3,
1066,
8,
7,
1066,
3,
1067,
7,
6,
1067,
3,
1067,
7,
6,
1067,
4,
1067,
6,
5,
1067,
5,
1068,
5,
4,
1047,
48,
1047,
4,
4,
1043,
55,
1044,
4,
3,
1042,
59,
1043,
3,
3,
1041,
62,
1041,
3,
2,
1041,
64,
1041,
2,
2,
1040,
66,
1040,
2,
1,
1040,
68,
1039,
2,
1,
1039,
70,
1039,
1,
1,
1038,
71,
1039,
1,
0,
1039,
72,
1038,
1,
0,
1038,
73,
1039,
0,
0,
1038,
74,
1038,
0,
0,
1038,
74,
1038,
0,
0,
1037,
75,
1038,
0,
0,
1037,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
1038,
76,
1037,
0,
0,
1037,
76,
1037,
0,
0,
1037,
76,
1037,
0,
0,
1037,
75,
1038,
0,
0,
1038,
74,
1038,
0,
0,
1038,
74,
1038,
0,
0,
1039,
72,
1038,
1,
1,
1038,
72,
1038,
1,
1,
1039,
70,
1039,
1,
1,
1039,
69,
1040,
1,
2,
1039,
68,
1039,
2,
2,
1040,
66,
1040,
2,
2,
1041,
63,
1041,
3,
3,
1042,
60,
1042,
3,
3,
1044,
56,
1043,
4,
4,
1045,
51,
1046,
4,
5,
1067,
6,
1067,
5,
5,
1068,
4,
1067,
6,
6,
1067,
3,
1068,
6,
7,
1066,
3,
1067,
7,
8,
1065,
3,
1066,
8,
9,
1064,
3,
1065,
9,
10,
1063,
3,
1064,
10,
11,
1062,
3,
1063,
11,
12,
1061,
3,
1062,
12,
14,
1059,
3,
1060,
14,
15,
1058,
3,
1058,
16,
17,
1056,
3,
1057,
17,
20,
1053,
3,
1054,
20,
23,
1050,
4,
1050,
23,
29,
1042,
7,
1043,
29
};

PROGMEM static const uint16_t NumberZero_colors[] = {
2113,
65535
};

#include "Bitmap.h"

class NumberZero : public Bitmap
{
public:
	NumberZero()
	{
		Width = 107;
        Height = 77;
        TransparencyColor = 0;
        StrokesArraySize = sizeof(NumberZero_strokes) / sizeof(NumberZero_strokes[0]);
        ColorsArraySize = sizeof(NumberZero_colors) / sizeof(NumberZero_colors[0]);
		Strokes = NumberZero_strokes;
		Colors = NumberZero_colors;
        LastPosX = LastPosY = -1;
	}
};

#endif
