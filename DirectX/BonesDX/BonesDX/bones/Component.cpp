#include "Component.h"

Component::Component(IComponentContainer* owner, const String& name)
	: _owner(owner), _name(name)
{
	Component::Register(this);
}

Component::~Component()
{
	Component::Unregister(this);
}