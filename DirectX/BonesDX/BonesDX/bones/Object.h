#pragma once

#include <iterator> 
#include <typeinfo>
#include "IAppObject.h"
#include "IComponentContainer.h"
#include "AppState.h"
#include "Component.h"
#include "TypeDefs.h"
#include "Registry.h"

#define NEW_OBJECT(className) std::make_shared<className>()
#define NEW_NAMED_OBJECT(className, instanceName) std::make_shared<className>(instanceName)

class Object : public IAppObject, public IComponentContainer
{
	REGISTERED_CLASS(Object, 0x53888c500d5c80e4);

protected:
	List<SharedPtr<Component>>	_components;
	bool						_isEnabled;
	
public:
	Object(const String& name = "", bool isEnabled = true);
	virtual ~Object();

public:
	virtual inline bool IsEnabled() const noexcept override
	{
		return _isEnabled;
	}

	virtual inline void IsEnabled(bool value) noexcept
	{
		_isEnabled = value;
	}

	virtual inline const String& GetName() const noexcept override
	{ 
		return _name; 
	} 
	
public:
	virtual Component* AddComponent(SharedPtr<Component> component) override;
	virtual Component* GetComponent(ClassId classId) const noexcept override;

	template <class T>
	T& GetComponent()
	{
		return dynamic_cast<T&>(*GetComponent(T::ClassID()));
	}

public:
	virtual void Init(const AppState& state) override;
	virtual void Tick(const AppState& state) override;
};