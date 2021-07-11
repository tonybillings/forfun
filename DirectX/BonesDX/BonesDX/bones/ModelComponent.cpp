#include "ModelComponent.h"

ModelComponent::ModelComponent(IComponentContainer* owner, const String& name)
	: Component(owner, name),
	_pGraphics(nullptr),
	_pVertexBuffer(nullptr),
	_pInputLayout(nullptr),
	_primitiveTopology(EPrimitiveTopology::TriangleList),
	_offset(0u),
	_hasTexture(false)
{ }

void ModelComponent::CreateVertexBuffer()
{
	if (_pGraphics == nullptr)
		throw EX("ModelComponent::CreateVertexBuffer() cannot be called until the ModelComponent has been initialized");
	
	if (_model.IsEmpty()) return;

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = _model.GetStride() * _model.GetVertexCount();
	bd.StructureByteStride = _model.GetStride();

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = _model.GetVertexData();

	THROW_IF_FAILED(_pGraphics->GetDevice()->CreateBuffer(&bd, &sd, &_pVertexBuffer));
}

void ModelComponent::CreateInputLayout()
{
	if (_vertexShaderFileName == WString(L"") || _model.IsEmpty()) return;

	auto file = File::Get(_vertexShaderFileName.c_str());

	switch (_model.GetVertexType())
	{
		case EVertexType::VertexPositionColor:
		{
			const D3D11_INPUT_ELEMENT_DESC ied[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			THROW_IF_FAILED(_pGraphics->GetDevice()->CreateInputLayout(ied, 2u, file->GetBufferPointer(), file->GetBufferSize(), &_pInputLayout));
			break;
		}
		case EVertexType::VertexPositionNormal:
		{
			const D3D11_INPUT_ELEMENT_DESC ied[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			THROW_IF_FAILED(_pGraphics->GetDevice()->CreateInputLayout(ied, 2u, file->GetBufferPointer(), file->GetBufferSize(), &_pInputLayout));
			break;
		}
		case EVertexType::VertexPositionNormalTexture:
		{
			const D3D11_INPUT_ELEMENT_DESC ied[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			THROW_IF_FAILED(_pGraphics->GetDevice()->CreateInputLayout(ied, 3u, file->GetBufferPointer(), file->GetBufferSize(), &_pInputLayout));
			break;
		}
	}
}

ID3D11Buffer* ModelComponent::GetVertexBuffer() const noexcept
{
	if (_pVertexBuffer != nullptr)
		return _pVertexBuffer.Get();
	else
		return nullptr;
}

ID3D11Buffer** ModelComponent::GetVertexBuffers() noexcept
{
	if (_pVertexBuffer != nullptr)
		return _pVertexBuffer.GetAddressOf();
	else
		return nullptr;
}

ID3D11InputLayout* ModelComponent::GetInputLayout() const noexcept
{
	if (_pInputLayout != nullptr)
		return _pInputLayout.Get();
	else
		return nullptr;
}

void ModelComponent::Init(const AppState& state)
{
	_pGraphics = state.Graphics;

	CreateVertexBuffer();
	CreateInputLayout();

	if (_texture.GetFileName() != WString(L""))
		_texture.Load(state.Graphics);
}

bool ModelComponent::Bind(ID3D11DeviceContext* deviceContext)
{
	if (_pVertexBuffer == nullptr || _pInputLayout == nullptr) return false;

	deviceContext->IASetInputLayout(_pInputLayout.Get());
	deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)_primitiveTopology);
	deviceContext->IASetVertexBuffers(0u, 1u, _pVertexBuffer.GetAddressOf(), _model.GetStrides(), &_offset);

	auto texture = _texture.GetResourceViews();
	if (texture != nullptr) deviceContext->PSSetShaderResources(0u, 1u, texture);

	return true;
}