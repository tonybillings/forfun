#include "Object.h"

Object::Object(const String& name, bool isEnabled)
	: _name(name), _isEnabled(isEnabled)
{
	Object::Register(this);
}

Object::~Object()
{
	Object::Unregister(this);
}

Component* Object::AddComponent(SharedPtr<Component> component)
{
	if (!component) return nullptr;

	if ((*component).GetOwner() == nullptr)
		(*component).SetOwner(this);

	_components.push_back(std::move(component));

	return component.get();
}

Component* Object::GetComponent(ClassId classId) const noexcept
{
	for (auto&& component : _components)
	{
		if ((*component).GetClassID() == classId)
			return component.get();
	}

	return nullptr;
}

void Object::Init(const AppState& state)
{
	for (auto&& component : _components)
		(*component).Init(state);
}

void Object::Tick(const AppState& state)
{
	for (auto&& component : _components)
		(*component).Tick(state);
}