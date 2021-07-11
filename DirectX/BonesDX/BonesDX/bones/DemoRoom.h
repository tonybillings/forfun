#pragma once

#include "bones.h"

class Room : public Object
{
protected:
	List<SharedPtr<DrawableObject>> _walls;

public:
	Room(const String& name = "")
		: Object(name)
	{
		using namespace std;

		auto&& model = Model("demo_plane.obj");
		
		auto&& mat = BasicMaterial::New();
		mat->GetProperties().Diffuse = Float4(.4f, .4f, .4f, 1.f);
		mat->GetProperties().Ambient = Float4(.25f, .25f, .25f, 1.f);
		mat->GetProperties().Specular = Float4(.774597f, .774597f, .774597f, 1.f);
		mat->GetProperties().SpecularPower = 76.8f;

		auto wall = NEW_NAMED_OBJECT(DrawableObject, "TopWall");
		wall->EnableDefaultLighting();
		wall->GetComponent<ModelComponent>()
			.SetModel(model)
			.SetMaterial(mat)
			.SetTexture(Texture2D("demo_wall_top.png"));
		wall->GetComponent<TransformComponent>().Position.y = 5;
		wall->GetComponent<TransformComponent>().SetRotation(QRotRPY(0, 0, DirectX::XM_PI));
		_walls.push_back(move(wall));

		wall = NEW_NAMED_OBJECT(DrawableObject, "RightWall");
		wall->EnableDefaultLighting();
		wall->GetComponent<ModelComponent>()
			.SetModel(model)
			.SetMaterial(mat)
			.SetTexture(Texture2D("demo_wall_right.png"));
		wall->GetComponent<TransformComponent>().Position.x = 5;
		wall->GetComponent<TransformComponent>().SetRotation(QRotRPY(-DirectX::XM_PIDIV2, 0, DirectX::XM_PIDIV2));
		_walls.push_back(move(wall));

		wall = NEW_NAMED_OBJECT(DrawableObject, "LeftWall");
		wall->EnableDefaultLighting();
		wall->GetComponent<ModelComponent>()
			.SetModel(model)
			.SetMaterial(mat)
			.SetTexture(Texture2D("demo_wall_left.png"));
		wall->GetComponent<TransformComponent>().Position.x = -5;
		wall->GetComponent<TransformComponent>().SetRotation(QRotRPY(DirectX::XM_PIDIV2, DirectX::XM_PI, DirectX::XM_PIDIV2));
		_walls.push_back(move(wall));

		wall = NEW_NAMED_OBJECT(DrawableObject, "BackWall");
		wall->EnableDefaultLighting();
		wall->GetComponent<ModelComponent>()
			.SetModel(model)
			.SetMaterial(mat)
			.SetTexture(Texture2D("demo_wall_back.png"));
		wall->GetComponent<TransformComponent>().Position.z = 5.f;
		wall->GetComponent<TransformComponent>().SetRotation(QRotRPY(DirectX::XM_PIDIV2, 0, DirectX::XM_PI));
		_walls.push_back(move(wall));

		wall = NEW_NAMED_OBJECT(DrawableObject, "FrontWall");
		wall->EnableDefaultLighting();
		wall->GetComponent<ModelComponent>()
			.SetModel(model)
			.SetMaterial(mat)
			.SetTexture(Texture2D("demo_wall_front.png"));
		wall->GetComponent<TransformComponent>().Position.z = -5.f;
		wall->GetComponent<TransformComponent>().SetRotation(QRotRPY(DirectX::XM_PIDIV2, 0, 0));
		_walls.push_back(move(wall));

		model.ScaleTextureCoordinates(4.f, 4.f);

		wall = NEW_NAMED_OBJECT(DrawableObject, "BottomWall");
		wall->EnableDefaultLighting();
		wall->GetComponent<ModelComponent>()
			.SetModel(model)
			.SetMaterial(mat)
			.SetTexture(Texture2D("demo_wall_bottom.png"));
		wall->GetComponent<TransformComponent>().Position.y = -5;
		_walls.push_back(move(wall));
	}

public:
	virtual void Init(const AppState& appState) override
	{
		for (auto&& wall : _walls)
			wall->Init(appState);
	}

	virtual void Tick(const AppState& appState) override
	{
		//appState.Graphics->SetRasterizerState(false);
		for (auto&& wall : _walls)
			wall->Tick(appState);
		//appState.Graphics->SetRasterizerState(true);
	}

};