#include "PointLight.h"

PointLight::PointLight(const String& name)
	: Light(name)
{
	GetComponent<ModelComponent>()
		.SetModel(Model("point_light.obj"))
		.SetTexture(Texture2D("point_light.png"))
		.SetMaterial(BasicMaterial::New(Float4(1.f, 1.f, 1.f, 1.f)));
	
	_properties->LightType = POINT_LIGHT;
}