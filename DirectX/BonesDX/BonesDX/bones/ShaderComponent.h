#pragma once

#include "Win32.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Component.h"
#include "IBindable.h"
#include "TypeDefs.h"
#include "GfxTypes.h"
#include "Exception.h"
#include "File.h"

class ShaderComponent : public Component, public IBindable
{
	SET_CLASS_ID(0x76c89a1c33d0337b);

protected:
	const Graphics* _pGraphics;
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3D11PixelShader>		_pixelShader;
	ComPtr<ID3D11SamplerState>		_sampler;
	WString							_vsResource;
	WString							_psResource;
	D3D11_VIEWPORT					_viewPort;
	List<ComPtr<ID3D11Buffer>>		_vsConstantBuffers;
	List<ComPtr<ID3D11Buffer>>		_psConstantBuffers;
	List<byte*>						_vsConstantBufferDatas;
	List<UINT>						_vsConstantBufferByteWidths;
	List<byte*>						_psConstantBufferDatas;
	List<UINT>						_psConstantBufferByteWidths;
	
public:
	ShaderComponent(IComponentContainer* owner, const String& name = "");
	ShaderComponent(const ShaderComponent& other) noexcept;
	ShaderComponent(ShaderComponent&& other) noexcept;
	virtual ~ShaderComponent();
	
public:
	ShaderComponent& operator=(const ShaderComponent& other) noexcept;
	ShaderComponent& operator=(ShaderComponent&& other) noexcept;

public:
	inline WString GetVertexShaderResource() const noexcept
	{
		return _vsResource;
	}

	inline ShaderComponent& SetVertexShaderResource(const String& filename) noexcept
	{
		_vsResource = WStringConverter().from_bytes(filename);
		return *this;
	}

	inline ShaderComponent& SetVertexShaderResource(const WString& filename) noexcept
	{
		_vsResource = filename;
		return *this;
	}

	inline WString GetPixelShaderResource() const noexcept
	{
		return _psResource;
	}

	inline ShaderComponent& SetPixelShaderResource(const String& filename) noexcept
	{
		_psResource = WStringConverter().from_bytes(filename);
		return *this;
	}

	inline ShaderComponent& SetPixelShaderResource(const WString& filename) noexcept
	{
		_psResource = filename;
		return *this;
	}

	inline D3D11_VIEWPORT GetViewPort() const noexcept
	{
		return _viewPort;
	}

	inline D3D11_VIEWPORT* GetViewPorts()
	{
		return &_viewPort;
	}
	
	inline ShaderComponent& SetViewPort(D3D11_VIEWPORT value) noexcept
	{
		_viewPort = value;
		return *this;
	}

	inline const Graphics* GetGraphics() const noexcept
	{
		return _pGraphics;
	}

	inline ShaderComponent& SetGraphics(const Graphics* graphics) noexcept
	{
		_pGraphics = graphics;
		return *this;
	}

	inline UINT VSGetConstantBufferCount() const noexcept
	{
		return (UINT)_vsConstantBuffers.size();
	}

	inline UINT PSGetConstantBufferCount() const noexcept
	{
		return (UINT)_psConstantBuffers.size();
	}

public:
	void CreateVertexShader();
	void CreatePixelShader();

	ID3D11VertexShader* GetVertexShader() const noexcept;
	ID3D11PixelShader* GetPixelShader() const noexcept;

	void VSCreateConstantBuffer(int slotNumber = -1);
	void PSCreateConstantBuffer(int slotNumber = -1);

	void VSUpdateConstantBuffer(int slotNumber = -1);
	void PSUpdateConstantBuffer(int slotNumber = -1);

	ID3D11Buffer* VSGetConstantBuffer(int slotNumber) const noexcept;
	ID3D11Buffer** VSGetConstantBuffers();

	ID3D11Buffer* PSGetConstantBuffer(int slotNumber) const noexcept;
	ID3D11Buffer** PSGetConstantBuffers();

	ShaderComponent& VSSetConstantBufferData(UINT slotNumber, const void* const data, UINT byteWidth);
	ShaderComponent& PSSetConstantBufferData(UINT slotNumber, const void* const data, UINT byteWidth);

	void CreateSampler();
	ID3D11SamplerState* GetSamplerState() const noexcept;
	ID3D11SamplerState** GetSamplerStates() noexcept;

public:
	virtual void Init(const AppState& state) override;
	virtual bool Bind(ID3D11DeviceContext* deviceContext) override;

public:
	static inline SharedPtr<ShaderComponent> New(IComponentContainer* owner, const String& name = "") noexcept
	{
		return std::make_shared<ShaderComponent>(owner, name);
	}

	static inline SharedPtr<ShaderComponent> New(
		const String& vertexShaderResource,
		const String& pixelShaderResource,
		IComponentContainer* owner,
		const String& name = "") noexcept
	{
		auto result = std::make_shared<ShaderComponent>(owner, name);
		result->SetVertexShaderResource(WStringConverter().from_bytes(vertexShaderResource));
		result->SetPixelShaderResource(WStringConverter().from_bytes(pixelShaderResource));
		return result;
	}
};