#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "GfxTypes.h"

class LightComponent : public Component
{
	SET_CLASS_ID(0xcc458418cdb93898);

protected:
	TransformComponent*		_transform;
	float					_activeRadius;
	uint32_t				_activeLayersBitMask;

private:
	static Float4	_globalAmbient;

public:
	CBLightProperties	Properties;

public:
	LightComponent(IComponentContainer* owner, const String& name = "");

public:
	virtual void Tick(const AppState& appState) override;
	virtual void Init(const AppState& appState) override;

public:
	inline float GetActiveRadius() const noexcept
	{
		return _activeRadius;
	}

	inline LightComponent& SetActiveRadius(float value)  noexcept
	{
		_activeRadius = value;
		return *this;
	}

	inline uint32_t GetActiveLayers() const noexcept
	{
		return _activeLayersBitMask;
	}

	inline LightComponent& SetActiveLayersBitMask(uint32_t value) noexcept
	{
		_activeLayersBitMask = value;
		return *this;
	}

public:
	static inline const Float4& GetGlobalAmbient() noexcept
	{
		return _globalAmbient;
	}

	static inline void SetGlobalAmbient(const Float4& value) noexcept
	{
		_globalAmbient = value;
	}

public:
	static SharedPtr<LightComponent> New(IComponentContainer* owner, const String& name = "") noexcept
	{
		auto result = std::make_shared<LightComponent>(owner, name);
		result->Properties.IsEnabled = true;
		return result;
	}

};