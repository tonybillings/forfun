#pragma once

#include "Object.h"
#include "IBindable.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "ShaderComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "BasicMaterial.h"

class DrawableObject : public Object
{
protected:
	List<IBindable*>			_bindables;
	TransformComponent*			_transform;
	ModelComponent*				_model;
	ShaderComponent*			_shader;
	bool						_isVisible;
	bool						_isDefaultLightingEnabled;
	uint32_t					_lightingLayersBitMask;

public:
	DrawableObject(const String& name = "");

public:
	inline bool IsVisible() const noexcept
	{
		return _isVisible;
	}

	inline void IsVisible(bool value) noexcept
	{
		_isVisible = value;
	}

	inline bool IsDefaultLightingEnabled() const noexcept
	{
		return _isDefaultLightingEnabled;
	}

	inline uint32_t GetLightingLayers() const noexcept
	{
		return _lightingLayersBitMask;
	}

	inline void SetLightingLayers(uint32_t bitmask) noexcept
	{
		_lightingLayersBitMask = bitmask;
	}

public:
	virtual void EnableDefaultLighting();
	virtual void DisableDefaultLighting();
	virtual void UpdateDefaultLighting(const AppState& state);
	virtual void Draw(const AppState& state);

public:
	virtual void Tick(const AppState& state) override;
	virtual Component* AddComponent(SharedPtr<Component> component) override;
};