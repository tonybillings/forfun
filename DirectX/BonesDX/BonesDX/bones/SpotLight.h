#pragma once

#include "Light.h"
#include "CONFIG.h"

class SpotLight : public Light
{
	SET_CLASS_ID(0x52b1257573859790);

public:
	SpotLight(const String& name = "");
};