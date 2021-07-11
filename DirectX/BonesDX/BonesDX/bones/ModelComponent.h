#pragma once

#include "Component.h"
#include "IBindable.h"
#include "TypeDefs.h"
#include "GfxTypes.h"
#include "Exception.h"
#include "File.h"
#include "Model.h"
#include "Texture2D.h"
#include "IMaterial.h"

class ModelComponent : public Component, public IBindable
{
	SET_CLASS_ID(0xae10a0d36fa48e38);

protected:
	const Graphics*					_pGraphics;
	ComPtr<ID3D11Buffer>			_pVertexBuffer;
	ComPtr<ID3D11InputLayout>		_pInputLayout;
	EPrimitiveTopology				_primitiveTopology;
	WString							_vertexShaderFileName;
	Model							_model;
	Texture2D						_texture;
	bool							_hasTexture;
	SharedPtr<IMaterial>			_material;
	UINT							_offset;

public:
	ModelComponent(IComponentContainer* owner, const String& name = "");

public:
	void CreateVertexBuffer();
	void CreateInputLayout();

	ID3D11Buffer* GetVertexBuffer() const noexcept;
	ID3D11Buffer** GetVertexBuffers() noexcept;
	ID3D11InputLayout* GetInputLayout() const noexcept;

public:
	virtual void Init(const AppState& state) override;
	virtual bool Bind(ID3D11DeviceContext* deviceContext) override;

public:
	inline const UINT GetOffset()
	{
		return _offset;
	}

	inline UINT* GetOffsets()
	{
		return &_offset;
	}

	inline ModelComponent& SetOffset(UINT value) noexcept
	{
		_offset = value;
		return *this;
	}

	inline D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const noexcept
	{
		return (D3D11_PRIMITIVE_TOPOLOGY)_primitiveTopology;
	}

	inline ModelComponent& SetPrimitiveTopology(EPrimitiveTopology value) noexcept
	{
		_primitiveTopology = value;
		return *this;
	}

	inline WString GetVertexShaderResource() const noexcept
	{
		return _vertexShaderFileName;
	}

	inline ModelComponent& SetVertexShaderResource(const String& filename) noexcept
	{
		_vertexShaderFileName = WStringConverter().from_bytes(filename);
		return *this;
	}

	inline ModelComponent& SetVertexShaderResource(const WString& filename) noexcept
	{
		_vertexShaderFileName = filename;
		return *this;
	}

	inline Model& GetModel() noexcept
	{
		return _model;
	}

	inline ModelComponent& SetModel(const Model& model) noexcept
	{
		_model = model;
		return *this;
	}

	inline Texture2D& GetTexture() noexcept
	{
		return _texture;
	}

	inline ModelComponent& SetTexture(const Texture2D& texture) noexcept
	{
		_texture = texture;

		if (_pGraphics != nullptr)
			_texture.Load(_pGraphics);

		_hasTexture = true;
		return *this;
	}

	inline bool HasTexture() const noexcept
	{
		return _hasTexture;
	}

	inline SharedPtr<IMaterial> GetMaterial() const noexcept
	{
		return _material;
	}

	inline ModelComponent& SetMaterial(SharedPtr<IMaterial> material) noexcept
	{
		_material = material;
		return *this;
	}

	inline bool HasMaterial() const noexcept
	{
		return _material != nullptr;
	}

public:
	static inline SharedPtr<ModelComponent> New(IComponentContainer* owner, const String& name = "") noexcept
	{
		return std::make_shared<ModelComponent>(owner, name);
	}

	static inline SharedPtr<ModelComponent> New(
		EVertexType vertexType,
		EPrimitiveTopology primitiveTopology,
		const String& vertexShaderFileName,
		const Model& model,
		IComponentContainer* owner,
		const String& name = "") noexcept
	{
		auto result = std::make_shared<ModelComponent>(owner, name);
		result->SetPrimitiveTopology(primitiveTopology);
		result->SetVertexShaderResource(WStringConverter().from_bytes(vertexShaderFileName));
		result->SetModel(model);
		return result;
	}
};