#pragma once

#include <typeinfo>
#include "IComponentContainer.h"
#include "AppState.h"
#include "TypeDefs.h"
#include "Registry.h"
#include "Exception.h"

class Component
{
	REGISTERED_CLASS(Component, 0xd1543cb69fd06912);

protected:
	IComponentContainer*	_owner;
	bool					_isEnabled;

public:
	Component(IComponentContainer* owner = nullptr, const String& name = "");
	virtual ~Component();

public:
	virtual void Init(const AppState& state) {};
	virtual void Tick(const AppState& state) {};

public:
	inline IComponentContainer* GetOwner() const noexcept
	{
		return _owner;
	}

	inline void SetOwner(IComponentContainer* value) noexcept
	{
		_owner = value;
	}

	inline bool IsEnabled() const noexcept
	{
		return _isEnabled;
	}

	inline void IsEnabled(bool value) noexcept
	{
		_isEnabled = value;
	}

public:
	template <class T>
	static inline SharedPtr<T> New(IComponentContainer* owner, const String& name = "") noexcept
	{
		return std::make_shared<T>(owner, name);
	}
};