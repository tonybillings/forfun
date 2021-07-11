#pragma once

#include "TypeDefs.h"

typedef long long ClassId;

class Component;

class IComponentContainer
{
public:
	virtual Component* AddComponent(SharedPtr<Component> component) = 0;
	virtual Component* GetComponent(ClassId classId) const noexcept = 0;

	template <class T>
	T& GetComponent()
	{
		return dynamic_cast<T&>(*GetComponent(T::ClassID()));
	}
};