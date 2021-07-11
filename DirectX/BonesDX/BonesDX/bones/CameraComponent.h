#pragma once

#include "Component.h"
#include "IComponentContainer.h"
#include "TransformComponent.h"
#include "GfxTypes.h"
#include "TypeDefs.h"

class CameraComponent :	public Component
{
	SET_CLASS_ID(0xb24bdc3445f8d010);
	
protected:
	TransformComponent* _ownerTransform;
	float				_fovRadians;
	float				_nearPlane;
	float				_farPlane;
	Matrix				_viewMatrix;
	Matrix				_projMatrix;
	Matrix				_viewProjMatrix;

public:
	CameraComponent(IComponentContainer* owner, const String& name = "");

public:
	inline const Vector& GetPosition() noexcept
	{
		return _ownerTransform->GetWorldPosition();
	}

	inline const Vector& GetRotation() const noexcept
	{
		return _ownerTransform->GetRotation();
	}

	inline float GetFieldOfViewRadians() const noexcept
	{
		return _fovRadians;
	}

	inline void SetFieldOfView(float degrees) noexcept
	{
		_fovRadians = degrees * (3.14f / 180.f);
	}

	inline float GetNearPlane() const noexcept
	{
		return _nearPlane;
	}

	inline void SetNearPlane(float value) noexcept
	{
		_nearPlane = value;
	}

	inline float GetFarPlane() const noexcept
	{
		return _farPlane;
	}

	inline void SetFarPlane(float value) noexcept
	{
		_farPlane = value;
	}

	inline const Matrix& GetViewMatrix() const noexcept
	{
		return _viewMatrix;
	}

	inline const Matrix& GetProjectionMatrix() const noexcept
	{
		return _projMatrix;
	}

	inline const Matrix& GetViewProjectionMatrix() const noexcept
	{
		return _viewProjMatrix;
	}

public:
	void Init(const AppState& state) override;
	void Tick(const AppState& state) override;

public:
	static inline SharedPtr<CameraComponent> New(IComponentContainer* owner, const String& name = "") noexcept
	{
		return std::make_shared<CameraComponent>(owner, name);
	}

	static inline CameraComponent* FindActiveCamera()
	{
		auto&& cameras = Component::Find<CameraComponent>();

		for (auto cam : cameras)
		{
			if (cam->IsEnabled()) { return cam; }
		}

		return nullptr;
	}
};