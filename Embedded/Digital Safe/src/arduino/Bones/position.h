#ifndef __bones_position_H__
#define __bones_position_H__

namespace Bones
{
	struct Position
	{
		int X;
		int Y;

		Position(int x = 0, int y = 0) : X(x), Y(y) {}
	};
}

#endif
