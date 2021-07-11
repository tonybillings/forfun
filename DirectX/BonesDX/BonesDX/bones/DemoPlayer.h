#pragma once

#include "bones.h"

class DemoPlayer : public DrawableObject
{
public:
	DemoPlayer(const String& name = "")
		: DrawableObject(name)
	{
		IsVisible(false);
		AddComponent(FirstPersonController::New(this));
		AddComponent(CameraComponent::New(this));
	}
};