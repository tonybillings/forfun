#pragma once

#include "Win32.h"
#include <d3d11.h>
#include "WICTextureLoader.h"
#include "HrException.h"
#include "Graphics.h"
#include "TypeDefs.h"

class Texture2D
{
protected:
	WString								_fileName;
	ComPtr<ID3D11Resource>				_texture;
	ComPtr<ID3D11ShaderResourceView>	_textureView;
	static WString*						_outputDirectory;

private:
	static void SetOutputDirectory();

public:
	Texture2D(const String& fileName = "")
		: _texture(nullptr), _textureView(nullptr)
	{
		_fileName = WStringConverter().from_bytes(fileName);
	}

	Texture2D(const String& fileName, const Graphics* const pGraphics)
		: _texture(nullptr), _textureView(nullptr)
	{
		_fileName = WStringConverter().from_bytes(fileName);
		Load(pGraphics);
	}

public:
	void Load(const Graphics* const pGraphics);

public:
	inline const WString& GetFileName() const noexcept
	{
		return _fileName;
	}

	inline ID3D11Resource* GetResource() const noexcept
	{
		if (_texture == nullptr) return nullptr;
		return _texture.Get();
	}

	inline ID3D11ShaderResourceView* GetResourceView() const noexcept
	{
		if (_textureView == nullptr) return nullptr;
		return _textureView.Get();
	}

	inline ID3D11ShaderResourceView** GetResourceViews() noexcept
	{
		if (_textureView == nullptr) return nullptr;
		return _textureView.GetAddressOf();
	}
};