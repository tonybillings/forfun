#pragma once

#include "Light.h"
#include "CONFIG.h"

class DirectionalLight : public Light
{
	SET_CLASS_ID(0x1972c6a78bfbda7b);

public:
	DirectionalLight(const String& name = "");

};