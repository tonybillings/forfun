#include "Light.h"

Light::Light(const String& name)
	: DrawableObject(name), _properties(nullptr)
{
	EnableDefaultLighting();

	_transform = &GetComponent<TransformComponent>();

	AddComponent(LightComponent::New(this));
	_properties = &GetComponent<LightComponent>().Properties;

	SetLightingLayers(0);
	IsVisible(false);
}