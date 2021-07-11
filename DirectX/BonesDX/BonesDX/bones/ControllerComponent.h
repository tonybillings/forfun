#pragma once

#include "Component.h"
#include "ControllerState.h"

enum class EControllerType { KeyboardMouse, GamePad, Joystick };

class ControllerComponent : public Component
{
	SET_CLASS_ID(0xa40812873c9d20aa);

protected:
	ControllerState		_state;
	EControllerType		_type;

public:
	ControllerComponent(IComponentContainer* owner, const String& name = "")
		: Component(owner, name), _type(EControllerType::KeyboardMouse)	{ }

public:
	inline const ControllerState& GetState() const noexcept
	{
		return _state;
	}

	inline void SetState(const ControllerState& state) noexcept
	{
		_state = state;
	}

	inline const EControllerType& GetType() const noexcept
	{
		return _type;
	}

	inline void SetType(EControllerType value) noexcept
	{
		_type = value;
	}
};