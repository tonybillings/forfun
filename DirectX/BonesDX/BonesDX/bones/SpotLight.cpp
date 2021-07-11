#include "SpotLight.h"

SpotLight::SpotLight(const String& name)
	: Light(name)
{
	GetComponent<ModelComponent>()
		.SetModel(Model("spot_light.obj"))
		.SetTexture(Texture2D("spot_light.png"))
		.SetMaterial(BasicMaterial::New(Float4(1.f, 1.f, 1.f, 1.f)));

	_properties->LightType = SPOT_LIGHT;
}