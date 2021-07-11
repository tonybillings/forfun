#ifndef __bones_size_H__
#define __bones_size_H__

namespace Bones
{
	struct Size
	{
		int Width;
		int Height;

		Size(int w = 0, int h = 0) : Width(w), Height(h) {}
	};
}

#endif