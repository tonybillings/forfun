#pragma once

#include "IMaterial.h"

class BasicMaterial : public IMaterial
{
protected:
	CBBasicMaterial _properties;

public:
	inline CBBasicMaterial& GetProperties() noexcept
	{
		return _properties;
	}

public:
	virtual const void* GetData() override
	{
		return &_properties;
	}

	virtual UINT GetByteWidth() override
	{
		return sizeof(_properties);
	}

public:
	static inline SharedPtr<BasicMaterial> New(
		Float4 emissive = Float4(),
		Float4 ambient = Float4(),
		Float4 diffuse = Float4(),
		Float4 specular = Float4(),
		float specularPower = 128.f,
		bool useTexture = true) noexcept
	{
		auto result = std::make_shared<BasicMaterial>();
		result->GetProperties().Emissive = emissive;
		result->GetProperties().Ambient = ambient;
		result->GetProperties().Diffuse = diffuse;
		result->GetProperties().Specular = specular;
		result->GetProperties().SpecularPower = specularPower;
		result->GetProperties().UseTexture = useTexture;
		return result;
	}
};