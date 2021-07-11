#pragma once

#include "bones.h"

class DemoTorus : public DrawableObject
{
public:
	DemoTorus(const String& name = "")
		: DrawableObject(name)
	{
		EnableDefaultLighting();

		auto mat = BasicMaterial::New();
		mat->GetProperties().Ambient = Float4(0, 0, 0, 1.f);
		mat->GetProperties().Diffuse = Float4(.55f, .55f, .55f, 1.f);
		mat->GetProperties().Specular = Float4(.7f, .7f, .7f, 1.f);
		mat->GetProperties().SpecularPower = 32.f;

		GetComponent<ModelComponent>()
			.SetModel(Model("demo_torus.obj"))
			.SetTexture(Texture2D("demo_torus.png"))
			.SetMaterial(mat);
	}
};