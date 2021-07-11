#pragma once

#include <d3d11.h>

class IBindable
{
public:
	virtual bool Bind(ID3D11DeviceContext* deviceContext) = 0;
};