#pragma once

#include "DrawableObject.h"

class Light : public DrawableObject
{
protected:
	CBLightProperties*	_properties;

public:
	Light(const String& name = "");

public:
	inline CBLightProperties& GetProperties() const noexcept
	{
		return *_properties;
	}

};