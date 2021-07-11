#include "ShaderComponent.h"

ShaderComponent::ShaderComponent(IComponentContainer* owner, const String& name)
	: Component(owner, name),
	_vertexShader(nullptr),
	_pixelShader(nullptr),
	_pGraphics(nullptr)
{
	D3D11_VIEWPORT vp;
	vp.Width = 0;
	vp.Height = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_viewPort = vp;
}

ShaderComponent::ShaderComponent(ShaderComponent&& other) noexcept
{
	this->_vertexShader = other._vertexShader;
	this->_pixelShader = other._pixelShader;
	this->_vsResource = other._vsResource;
	this->_psResource = other._psResource;
	this->_viewPort = other._viewPort;
	this->_pGraphics = other._pGraphics;
	this->_vsConstantBuffers = other._vsConstantBuffers;
	this->_psConstantBuffers = other._psConstantBuffers;
	this->_vsConstantBufferDatas = other._vsConstantBufferDatas;
	this->_vsConstantBufferByteWidths = other._vsConstantBufferByteWidths;
	this->_psConstantBufferDatas = other._psConstantBufferDatas;
	this->_psConstantBufferByteWidths = other._psConstantBufferByteWidths;
}

ShaderComponent::ShaderComponent(const ShaderComponent& other) noexcept
{
	this->_vertexShader = other._vertexShader;
	this->_pixelShader = other._pixelShader;
	this->_vsResource = other._vsResource;
	this->_psResource = other._psResource;
	this->_viewPort = other._viewPort;
	this->_pGraphics = other._pGraphics;

	for (UINT i = 0; i < (UINT)other._vsConstantBufferDatas.size(); i++)
	{
		auto pData = other._vsConstantBufferDatas[i];
		auto width = other._vsConstantBufferByteWidths[i];
		this->VSSetConstantBufferData(i, pData, width);
	}

	for (UINT i = 0; i < (UINT)other._psConstantBufferDatas.size(); i++)
	{
		auto pData = other._psConstantBufferDatas[i];
		auto width = other._psConstantBufferByteWidths[i];
		this->PSSetConstantBufferData(i, pData, width);
	}
}

ShaderComponent::~ShaderComponent()
{
	for (auto data : _vsConstantBufferDatas)
		delete[] data;

	for (auto data : _psConstantBufferDatas)
		delete[] data;
}

ShaderComponent& ShaderComponent::operator=(const ShaderComponent& other) noexcept
{
	this->_vertexShader = other._vertexShader;
	this->_pixelShader = other._pixelShader;
	this->_vsResource = other._vsResource;
	this->_psResource = other._psResource;
	this->_viewPort = other._viewPort;
	this->_pGraphics = other._pGraphics;

	for (UINT i = 0; i < (UINT)other._vsConstantBufferDatas.size(); i++)
	{
		auto pData = other._vsConstantBufferDatas[i];
		auto width = other._vsConstantBufferByteWidths[i];
		this->VSSetConstantBufferData(i, pData, width);
	}

	for (UINT i = 0; i < (UINT)other._psConstantBufferDatas.size(); i++)
	{
		auto pData = other._psConstantBufferDatas[i];
		auto width = other._psConstantBufferByteWidths[i];
		this->PSSetConstantBufferData(i, pData, width);
	}

	return *this;
}

ShaderComponent& ShaderComponent::operator=(ShaderComponent&& other) noexcept
{
	this->_vertexShader = other._vertexShader;
	this->_pixelShader = other._pixelShader;
	this->_vsResource = other._vsResource;
	this->_psResource = other._psResource;
	this->_viewPort = other._viewPort;
	this->_pGraphics = other._pGraphics;
	this->_vsConstantBuffers = other._vsConstantBuffers;
	this->_psConstantBuffers = other._psConstantBuffers;
	this->_vsConstantBufferDatas = other._vsConstantBufferDatas;
	this->_vsConstantBufferByteWidths = other._vsConstantBufferByteWidths;
	this->_psConstantBufferDatas = other._psConstantBufferDatas;
	this->_psConstantBufferByteWidths = other._psConstantBufferByteWidths;

	return *this;
}

void ShaderComponent::CreateVertexShader()
{
	if (_vsResource == WString(L"")) return;
	auto file = File::Get(_vsResource.c_str());
	THROW_IF_FAILED(_pGraphics->GetDevice()->CreateVertexShader(file->GetBufferPointer(), file->GetBufferSize(), nullptr, &_vertexShader));
}

void ShaderComponent::CreatePixelShader()
{
	if (_psResource == WString(L"")) return;
	auto file = File::Get(_psResource.c_str());
	THROW_IF_FAILED(_pGraphics->GetDevice()->CreatePixelShader(file->GetBufferPointer(), file->GetBufferSize(), nullptr, &_pixelShader));
}

ID3D11VertexShader* ShaderComponent::GetVertexShader() const noexcept
{
	if (_vertexShader != nullptr)
		return _vertexShader.Get();
	else
		return nullptr;
}

ID3D11PixelShader* ShaderComponent::GetPixelShader() const noexcept
{
	if (_pixelShader != nullptr)
		return _pixelShader.Get();
	else
		return nullptr;
}

void ShaderComponent::VSCreateConstantBuffer(int slotNumber)
{
	using namespace DirectX;

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};

	if (slotNumber == -1)
	{
		for (size_t i = 0; i < _vsConstantBufferDatas.size(); i++)
		{
			auto pData = _vsConstantBufferDatas[i];
			auto width = _vsConstantBufferByteWidths[i];
			cbd.ByteWidth = width;
			csd.pSysMem = pData;

			ComPtr<ID3D11Buffer> pBuffer;
			THROW_IF_FAILED(_pGraphics->GetDevice()->CreateBuffer(&cbd, &csd, &pBuffer));

			if (i > _vsConstantBuffers.size() - 1)
				_vsConstantBuffers.push_back(pBuffer);
			else
				_vsConstantBuffers[slotNumber] = pBuffer;
		}
	}
	else
	{
		auto pData = _vsConstantBufferDatas[slotNumber];
		auto width = _vsConstantBufferByteWidths[slotNumber];
		cbd.ByteWidth = width;
		csd.pSysMem = pData;

		ComPtr<ID3D11Buffer> pBuffer;
		THROW_IF_FAILED(_pGraphics->GetDevice()->CreateBuffer(&cbd, &csd, &pBuffer));

		if (slotNumber > (int)_vsConstantBuffers.size() - 1)
			_vsConstantBuffers.push_back(pBuffer);
		else
			_vsConstantBuffers[slotNumber] = pBuffer;
	}
}

void ShaderComponent::PSCreateConstantBuffer(int slotNumber)
{
	using namespace DirectX;

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};

	if (slotNumber == -1)
	{
		for (size_t i = 0; i < _psConstantBufferDatas.size(); i++)
		{
			auto pData = _psConstantBufferDatas[i];
			auto width = _psConstantBufferByteWidths[i];
			cbd.ByteWidth = width;
			csd.pSysMem = pData;

			ComPtr<ID3D11Buffer> pBuffer;
			THROW_IF_FAILED(_pGraphics->GetDevice()->CreateBuffer(&cbd, &csd, &pBuffer));

			if (i > _psConstantBuffers.size() - 1)
				_psConstantBuffers.push_back(pBuffer);
			else
				_psConstantBuffers[slotNumber] = pBuffer;
		}
	}
	else
	{
		auto pData = _psConstantBufferDatas[slotNumber];
		auto width = _psConstantBufferByteWidths[slotNumber];
		cbd.ByteWidth = width;
		csd.pSysMem = pData;

		ComPtr<ID3D11Buffer> pBuffer;
		THROW_IF_FAILED(_pGraphics->GetDevice()->CreateBuffer(&cbd, &csd, &pBuffer));

		if (slotNumber > (int)_psConstantBuffers.size() - 1)
			_psConstantBuffers.push_back(pBuffer);
		else
			_psConstantBuffers[slotNumber] = pBuffer;
	}
}

void ShaderComponent::VSUpdateConstantBuffer(int slotNumber)
{
	auto pBuffer = _vsConstantBuffers[slotNumber];
	auto pData = _vsConstantBufferDatas[slotNumber];
	auto width = _vsConstantBufferByteWidths[slotNumber];

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	_pGraphics->GetDeviceContext()->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, pData, width);

	_pGraphics->GetDeviceContext()->Unmap(pBuffer.Get(), 0);
}

void ShaderComponent::PSUpdateConstantBuffer(int slotNumber)
{
	auto pBuffer = _psConstantBuffers[slotNumber];
	auto pData = _psConstantBufferDatas[slotNumber];
	auto width = _psConstantBufferByteWidths[slotNumber];

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	_pGraphics->GetDeviceContext()->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, pData, width);

	_pGraphics->GetDeviceContext()->Unmap(pBuffer.Get(), 0);
}

ID3D11Buffer* ShaderComponent::VSGetConstantBuffer(int slotNumber) const noexcept
{
	if (_vsConstantBuffers.size() == 0) return nullptr;
	return _vsConstantBuffers[slotNumber].Get();
}

ID3D11Buffer** ShaderComponent::VSGetConstantBuffers()
{
	if (_vsConstantBuffers.size() == 0) return nullptr;
	return _vsConstantBuffers[0].GetAddressOf();
}

ID3D11Buffer* ShaderComponent::PSGetConstantBuffer(int slotNumber) const noexcept
{
	if (_psConstantBuffers.size() == 0) return nullptr;
	return _psConstantBuffers[slotNumber].Get();
}

ID3D11Buffer** ShaderComponent::PSGetConstantBuffers()
{
	if (_psConstantBuffers.size() == 0) return nullptr;
	return _psConstantBuffers[0].GetAddressOf();
}

ShaderComponent& ShaderComponent::VSSetConstantBufferData(UINT slotNumber, const void* const data, UINT byteWidth)
{
	if (_vsConstantBufferDatas.size() == 0 || slotNumber > _vsConstantBufferDatas.size() - 1)
	{
		auto pData = new BYTE[byteWidth];
		memcpy(pData, data, byteWidth);

		_vsConstantBufferDatas.push_back(pData);
		_vsConstantBufferByteWidths.push_back(byteWidth);

		if (_pGraphics != nullptr)
			VSCreateConstantBuffer(slotNumber);
	}
	else
	{
		auto pData = _vsConstantBufferDatas[slotNumber];
		auto width = _vsConstantBufferByteWidths[slotNumber];

		if (width == byteWidth)
		{
			memcpy(pData, data, byteWidth);
		}
		else
		{
			delete[] pData;
			pData = new BYTE[byteWidth];
			_vsConstantBufferDatas[slotNumber] = pData;
			_vsConstantBufferByteWidths[slotNumber] = byteWidth;
			memcpy(pData, data, byteWidth);
		}

		if (_pGraphics != nullptr)
			VSUpdateConstantBuffer(slotNumber);
	}

	return *this;
}

ShaderComponent& ShaderComponent::PSSetConstantBufferData(UINT slotNumber, const void* const data, UINT byteWidth)
{
	if (_psConstantBufferDatas.size() == 0 || slotNumber > _psConstantBufferDatas.size() - 1)
	{
		auto pData = new BYTE[byteWidth];
		memcpy(pData, data, byteWidth);

		_psConstantBufferDatas.push_back(pData);
		_psConstantBufferByteWidths.push_back(byteWidth);

		if (_pGraphics != nullptr)
			PSCreateConstantBuffer(slotNumber);
	}
	else
	{
		auto pData = _psConstantBufferDatas[slotNumber];
		auto width = _psConstantBufferByteWidths[slotNumber];

		if (width == byteWidth)
		{
			memcpy(pData, data, byteWidth);
		}
		else
		{
			delete[] pData;
			pData = new BYTE[byteWidth];
			_psConstantBufferDatas[slotNumber] = pData;
			_psConstantBufferByteWidths[slotNumber] = byteWidth;
			memcpy(pData, data, byteWidth);
		}

		if (_pGraphics != nullptr)
			PSUpdateConstantBuffer(slotNumber);
	}

	return *this;
}

void ShaderComponent::CreateSampler()
{
	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.MipLODBias = 0.0f;
	sd.MaxAnisotropy = 16;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.BorderColor[0] = 1.0f;
	sd.BorderColor[1] = 1.0f;
	sd.BorderColor[2] = 1.0f;
	sd.BorderColor[3] = 1.0f;
	sd.MinLOD = -FLT_MAX;
	sd.MaxLOD = FLT_MAX;

	THROW_IF_FAILED(_pGraphics->GetDevice()->CreateSamplerState(&sd, &_sampler));
}

ID3D11SamplerState* ShaderComponent::GetSamplerState() const noexcept
{
	return _sampler.Get();
}

ID3D11SamplerState** ShaderComponent::GetSamplerStates()  noexcept
{
	if (_sampler == nullptr) return nullptr;
	return _sampler.GetAddressOf();
}

void ShaderComponent::Init(const AppState& state)
{
	_pGraphics = state.Graphics;
	_viewPort.Width = (float)state.WindowWidth;
	_viewPort.Height = (float)state.WindowHeight;
	CreateVertexShader();
	CreatePixelShader();
	VSCreateConstantBuffer();
	PSCreateConstantBuffer();
	CreateSampler();
}

bool ShaderComponent::Bind(ID3D11DeviceContext* deviceContext)
{
	if (_vertexShader == nullptr || _pixelShader == nullptr) return false;

	auto vsCB = VSGetConstantBuffers();
	if (vsCB != nullptr) deviceContext->VSSetConstantBuffers(0u, (UINT)_vsConstantBuffers.size(), vsCB);

	auto psCB = PSGetConstantBuffers();
	if (psCB != nullptr) deviceContext->PSSetConstantBuffers(0u, (UINT)_psConstantBuffers.size(), psCB);

	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0u);
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0u);

	if (_sampler != nullptr)
		deviceContext->PSSetSamplers(0u, 1u, _sampler.GetAddressOf());

	return true;
}