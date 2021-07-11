#include "Graphics.h"

void Graphics::Init(HWND hWnd, int width, int height)
{
	CreateDeviceAndSwapChain(hWnd);
	SetViewPorts(width, height);
	SetRasterizerState();
	SetDepthStencilState();
	CreateDepthStencilView(width, height);
	SetRenderTargets();

	THROW_IF_FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));
}

void Graphics::CreateDeviceAndSwapChain(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT debugFlags = 0;
#ifdef _DEBUG
	debugFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		debugFlags,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&_swapChain,
		&_device,
		nullptr,
		&_context
	));
}

void Graphics::SetViewPorts(int width, int height)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	_context->RSSetViewports(1u, &vp);
}

void Graphics::SetRasterizerState(bool enableCulling)
{
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = enableCulling ? D3D11_CULL_BACK : D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	THROW_IF_FAILED(_device->CreateRasterizerState(&rasterDesc, &_rasterizerState));
	_context->RSSetState(_rasterizerState.Get());
}

void Graphics::SetDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.StencilEnable = FALSE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	ComPtr<ID3D11DepthStencilState> pDSState;

	THROW_IF_FAILED(_device->CreateDepthStencilState(&dsDesc, &pDSState));
	_context->OMSetDepthStencilState(pDSState.Get(), 1u);
}

void Graphics::CreateDepthStencilView(int width, int height)
{
	UINT maxQuality;
	THROW_IF_FAILED(_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4u, &maxQuality));
	
	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.Width = (UINT)width;
	txtDesc.Height = (UINT)height;
	txtDesc.MipLevels = 1u;
	txtDesc.ArraySize = 1u;
	txtDesc.Format = DXGI_FORMAT_D32_FLOAT;
	txtDesc.SampleDesc.Count = 4;
	txtDesc.SampleDesc.Quality = 0;
	txtDesc.Usage = D3D11_USAGE_DEFAULT;
	txtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	
	THROW_IF_FAILED(_device->CreateTexture2D(&txtDesc, nullptr, &pDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc = {};
	dsDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0u;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	
	THROW_IF_FAILED(_device->CreateDepthStencilView(pDepthStencil.Get(), &dsDesc, &_depthStencil));
}

void Graphics::SetRenderTargets()
{
	ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	THROW_IF_FAILED(_swapChain->GetBuffer(IDX_BACK_BUFFER, UID_RESOURCE, &pBackBuffer));
	THROW_IF_FAILED(_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &_renderTarget));
	_context->OMSetRenderTargets(1u, _renderTarget.GetAddressOf(), _depthStencil.Get());
}

void Graphics::Present()
{
	HRESULT hr;
	if (FAILED(hr = _swapChain->Present(_vSyncEnabled, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw HR_EX(_device->GetDeviceRemovedReason());
		else
			throw HR_EX(hr);
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	_context->ClearRenderTargetView(_renderTarget.Get(), color);
	_context->ClearDepthStencilView(_depthStencil.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
}