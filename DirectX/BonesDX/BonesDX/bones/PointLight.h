#pragma once

#include "Light.h"
#include "CONFIG.h"

class PointLight : public Light
{
	SET_CLASS_ID(0x59dbd568c0bae27a);

public:
	PointLight(const String& name = "");
};