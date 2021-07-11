#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const String& name)
	: Light(name)
{
	GetComponent<ModelComponent>()
		.SetModel(Model("directional_light.obj"))
		.SetTexture(Texture2D("directional_light.png"))
		.SetMaterial(BasicMaterial::New(Float4(1.f, 1.f, 1.f, 1.f)));

	_properties->LightType = DIRECTIONAL_LIGHT;
}