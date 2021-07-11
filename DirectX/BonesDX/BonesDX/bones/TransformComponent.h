#pragma once

#include "Component.h"
#include "GfxTypes.h"

class TransformComponent : public Component
{
	SET_CLASS_ID(0x2b12132d120f21b2);

public:
	TransformComponent(IComponentContainer* owner, const String& name = "");

protected:
	Vector _position;
	Vector _rotation;
	Vector _scale;
	Vector _orbitPosition;
	Vector _orbitRotation;
	Vector _worldPosition;
	Vector _forward;
	Vector _up;
	Matrix _worldMatrix;

public:
	Float4	Position;
	Float4	Rotation;
	Float4	OrbitPosition;
	Float4	OrbitRotation;
	Float4	Scale;

public:
	inline Vector& GetPosition() noexcept
	{
		_position = VLoad(&Position);
		return _position;
	}

	inline TransformComponent& SetPosition(FVector vector) noexcept
	{
		VStore(&Position, vector);
		return *this;
	}

	inline Vector& GetRotation() noexcept
	{
		_rotation = VLoad(&Rotation);
		return _rotation;
	}

	inline TransformComponent& SetRotation(FVector vector) noexcept
	{
		VStore(&Rotation, vector);
		return *this;
	}

	inline Vector& GetOrbitPosition() noexcept
	{
		_orbitPosition = VLoad(&OrbitPosition);
		return _orbitPosition;
	}

	inline TransformComponent& SetOrbitPosition(FVector vector) noexcept
	{
		VStore(&OrbitPosition, vector);
		return *this;
	}

	inline Vector& GetOrbitRotation() noexcept
	{
		_orbitRotation = VLoad(&OrbitRotation);
		return _orbitRotation;
	}

	inline TransformComponent& SetOrbitRotation(FVector vector) noexcept
	{
		VStore(&OrbitRotation, vector);
		return *this;
	}

	inline Vector& GetScale() noexcept
	{
		_scale = VLoad(&Scale);
		return _scale;
	}

	inline TransformComponent& SetScale(FVector vector) noexcept
	{
		VStore(&Scale, vector);
		return *this;
	}

	inline const Vector& GetWorldPosition() const noexcept
	{
		return _worldPosition;
	}

	inline const Matrix& GetWorldMatrix() const noexcept
	{
		return _worldMatrix;
	}

	inline const Vector& GetForwardVector() const noexcept
	{
		return _forward;
	}

	inline const Vector& GetUpVector() const noexcept
	{
		return _up;
	}

public:
	TransformComponent& RotateTo(FVector target) noexcept;

public:
	virtual void Tick(const AppState& appState) override;

public:
	static inline SharedPtr<TransformComponent> New(IComponentContainer* owner, const String& name = "") noexcept
	{
		return std::make_shared<TransformComponent>(owner, name);
	}
};