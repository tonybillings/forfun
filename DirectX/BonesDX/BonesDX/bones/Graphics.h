#pragma once

#include "Win32.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "TypeDefs.h"
#include "HrException.h"

#define IDX_BACK_BUFFER		0
#define IDX_FRONT_BUFFER	1
#define UID_RESOURCE		__uuidof(ID3D11Resource)

class Graphics
{
public:
	Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

private:
	ComPtr<ID3D11Device>			_device = nullptr;
	ComPtr<IDXGISwapChain>			_swapChain = nullptr;
	ComPtr<ID3D11DeviceContext>		_context = nullptr;
	ComPtr<ID3D11RenderTargetView>	_renderTarget = nullptr;
	ComPtr<ID3D11RasterizerState>	_rasterizerState = nullptr;
	ComPtr<ID3D11DepthStencilView>	_depthStencil = nullptr;
	ComPtr<ID3D11SamplerState>		_samplerState = nullptr;
	UINT							_vSyncEnabled = 1;

private:
	void CreateDeviceAndSwapChain(HWND hWnd);
	void SetViewPorts(int width, int height);
	void SetDepthStencilState();
	void CreateDepthStencilView(int width, int height);
	void SetRenderTargets();

public:
	void Init(HWND hWnd, int width, int height);
	void Present();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void SetRasterizerState(bool enableCulling = true);

public:
	inline ID3D11Device* GetDevice() const noexcept
	{
		return _device.Get();
	}

	inline IDXGISwapChain* GetSwapChain() const noexcept
	{
		return _swapChain.Get();
	}

	inline ID3D11DeviceContext* GetDeviceContext() const noexcept
	{
		return _context.Get();
	}

	inline ID3D11RenderTargetView* GetRenderTargetView() const noexcept
	{
		return _renderTarget.Get();
	}

	inline ID3D11RenderTargetView** GetRenderTargetViews()
	{
		return _renderTarget.GetAddressOf();
	}

	inline ID3D11RasterizerState* GetRasterizerState() const noexcept
	{
		return _rasterizerState.Get();
	}

	inline ID3D11RasterizerState** GetRasterizerStates()
	{
		return _rasterizerState.GetAddressOf();
	}

	inline bool IsVsyncEnabled() const noexcept
	{
		return _vSyncEnabled == 1;
	}

	inline void IsVsyncEnabled(bool value) noexcept
	{
		_vSyncEnabled = (value == true) ? 1 : 0;
	}
};