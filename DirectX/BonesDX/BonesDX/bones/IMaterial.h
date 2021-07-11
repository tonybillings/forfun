#pragma once

#include "GfxTypes.h"

class IMaterial
{
public:
	virtual const void* GetData() = 0;
	virtual UINT GetByteWidth() = 0;
};