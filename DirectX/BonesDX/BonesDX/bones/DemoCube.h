#pragma once

#include "bones.h"

class DemoCube : public DrawableObject
{
protected:
	float _angle;

public:
	DemoCube(const String& name = "")
		: DrawableObject(name), _angle(0)
	{
		EnableDefaultLighting();

		auto mat = BasicMaterial::New();
		mat->GetProperties().Ambient = Float4(0, 0, 0, 1.f);
		mat->GetProperties().Diffuse = Float4(.55f, .55f, .55f, 1.f);
		mat->GetProperties().Specular = Float4(.7f, .7f, .7f, 1.f);
		mat->GetProperties().SpecularPower = 32.f;

		GetComponent<ModelComponent>()
			.SetModel(Model("demo_cube.obj"))
			.SetTexture(Texture2D("demo_cube.png"))
			.SetMaterial(mat);
	}

	virtual void Tick(const AppState& state) override
	{
		_angle += 0.0008f;

		GetComponent<TransformComponent>().SetRotation(QRotRPY(_angle, _angle, 0));

		DrawableObject::Tick(state);
	}
};