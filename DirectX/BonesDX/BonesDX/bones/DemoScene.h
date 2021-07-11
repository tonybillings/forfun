#pragma once

#include "bones.h"
#include "DemoRoom.h"
#include "DemoCube.h"
#include "DemoTorus.h"
#include "DemoPlayer.h"

class DemoScene : public Scene
{
protected:
	TransformComponent*	_light1;
	TransformComponent*	_light2;
	TransformComponent*	_light3;
	TransformComponent* _light4;
	TransformComponent* _light5;
	TransformComponent* _light6;
	TransformComponent* _light7;
	TransformComponent* _light8;

public:
	DemoScene(const String& name = "")
		: Scene(name)
	{
		AddObject(NEW_OBJECT(Room));

		auto cube = NEW_OBJECT(DemoCube);
		AddObject(cube);

		auto torus = NEW_OBJECT(DemoTorus);
		torus->GetComponent<TransformComponent>().SetScale(VRep(.4f));
		AddObject(torus);

		auto dirLight1 = NEW_OBJECT(DirectionalLight);
		dirLight1->IsVisible(true);
		dirLight1->GetComponent<TransformComponent>().Position.x = -4.f;
		dirLight1->GetComponent<TransformComponent>().SetScale(VRep(.5f));
		dirLight1->GetProperties().Color = Float4(.7f, .25f, .25f, 1.f);
		_light1 = &dirLight1->GetComponent<TransformComponent>();
		AddObject(dirLight1);

		auto dirLight2 = NEW_OBJECT(DirectionalLight);
		dirLight2->IsVisible(true);
		dirLight2->GetComponent<TransformComponent>().Position.x = 4.f;
		dirLight2->GetComponent<TransformComponent>().SetRotation(QRotRPY(DEGREES_180, 0, 0));
		dirLight2->GetComponent<TransformComponent>().SetScale(VRep(.5f));
		dirLight2->GetProperties().Color = Float4(.25f, .5f, .25f, 1.f);
		_light2 = &dirLight2->GetComponent<TransformComponent>();
		AddObject(dirLight2);

		auto spotLight1 = NEW_OBJECT(SpotLight);
		spotLight1->IsVisible(true);
		spotLight1->GetComponent<TransformComponent>().OrbitPosition.z = 3.f;
		spotLight1->GetComponent<TransformComponent>().SetScale(VRep(.25f));
		spotLight1->GetProperties().Color = Float4(10.f, 0, 0, 1.f);
		spotLight1->GetProperties().SpotAngle = DEGREES_45 / 4.f;
		_light3 = &spotLight1->GetComponent<TransformComponent>();
		AddObject(spotLight1);

		auto spotLight2 = NEW_OBJECT(SpotLight);
		spotLight2->IsVisible(true);
		spotLight2->GetComponent<TransformComponent>().OrbitPosition.z = 3.f;
		spotLight2->GetComponent<TransformComponent>().SetScale(VRep(.25f));
		spotLight2->GetProperties().Color = Float4(0, 10.f, 0, 1.f);
		spotLight2->GetProperties().SpotAngle = DEGREES_45 / 4.f;
		_light4 = &spotLight2->GetComponent<TransformComponent>();
		AddObject(spotLight2);

		auto spotLight3 = NEW_OBJECT(SpotLight);
		spotLight3->IsVisible(true);
		spotLight3->GetComponent<TransformComponent>().OrbitPosition.z = 3.f;
		spotLight3->GetComponent<TransformComponent>().SetScale(VRep(.25f));
		spotLight3->GetProperties().Color = Float4(0, 0, 10.f, 1.f);
		spotLight3->GetProperties().SpotAngle = DEGREES_45 / 4.f;
		_light5 = &spotLight3->GetComponent<TransformComponent>();
		AddObject(spotLight3);

		auto pointLight = NEW_OBJECT(PointLight);
		pointLight->IsVisible(true);
		pointLight->GetComponent<TransformComponent>().Position.x = -2.5f;
		pointLight->GetComponent<TransformComponent>().Position.z = 0;

		pointLight->GetComponent<TransformComponent>().SetScale(VRep(.5f));
		pointLight->GetProperties().Color = Float4(1.f, 0, 0, 1.f);
		//AddObject(pointLight);

		auto player = NEW_OBJECT(DemoPlayer);
		player->GetComponent<TransformComponent>().Position.z = -5.f;
		player->GetComponent<FirstPersonController>().SetMoveSpeed(.5f);
		player->GetComponent<FirstPersonController>().SetFastMoveSpeed(1.f);
		player->GetComponent<FirstPersonController>().SetLookSpeed(.3f);
		player->GetComponent<FirstPersonController>().SetRollSpeed(.5f);
		AddObject(player);
	}

public:
	virtual void Init(const AppState& appState) override
	{
		appState.Mouse->EnableRaw();
		Scene::Init(appState);
	}

	virtual void Tick(const AppState& appState) override
	{
		static float light1Rot = 0;
		static float light2Rot = DEGREES_180;

		light1Rot += 0.1f * appState.DeltaTime;
		light2Rot += 0.1f * appState.DeltaTime;

		_light1->SetRotation(QRotRPY(light1Rot, 0, 0));
		_light2->SetRotation(QRotRPY(light2Rot, 0, 0));

		static float light3Rot = 0;
		static float light3Y = 0;
		static float light3X = 0;
		light3X += 0.3f * appState.DeltaTime;
		light3Rot += 0.3f * appState.DeltaTime;
		light3Y = sinf(light3X * 3.f) * 2.f;
		_light3->SetOrbitRotation(QRotNorm(VECTOR_UP, light3Rot));
		_light3->SetPosition(VSet(0, light3Y, 0, 1.f));
		_light3->RotateTo(VECTOR_ORIGIN);

		static float light4Rot = 2.094f;
		static float light4Y = 0;
		static float light4X = 10;
		light4X += 0.3f * appState.DeltaTime;
		light4Rot += 0.3f * appState.DeltaTime;
		light4Y = sinf(light4X * 3.f) * 2.f;
		_light4->SetOrbitRotation(QRotNorm(VECTOR_UP, light4Rot));
		_light4->SetPosition(VSet(0, light4Y, 0, 1.f));
		_light4->RotateTo(VECTOR_ORIGIN);

		static float light5Rot = 4.18879f;
		static float light5Y = 0;
		static float light5X = 20;
		light5X += 0.3f * appState.DeltaTime;
		light5Rot += 0.3f * appState.DeltaTime;
		light5Y = sinf(light5X * 3.f) * 2.f;
		_light5->SetOrbitRotation(QRotNorm(VECTOR_UP, light5Rot));
		_light5->SetPosition(VSet(0, light5Y, 0, 1.f));
		_light5->RotateTo(VECTOR_ORIGIN);




		Scene::Tick(appState);
	}
};