#include "LightComponent.h"

Float4 LightComponent::_globalAmbient = Float4(1.f, 1.f, 1.f, 1.f);

LightComponent::LightComponent(IComponentContainer* owner, const String& name)
	: Component(owner, name),
	_transform(nullptr),
	_activeRadius(1000000.f),
	_activeLayersBitMask(1) 
{

}

void LightComponent::Init(const AppState& appState)
{
	Properties.IsEnabled = true;
	_transform = &_owner->GetComponent<TransformComponent>();
}

void LightComponent::Tick(const AppState& appState)
{
	Properties.IsEnabled = IsEnabled();
	VStore(&Properties.Position, _transform->GetWorldPosition());
	VStore(&Properties.Direction, _transform->GetForwardVector());
}