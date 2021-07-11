#pragma once

#include "Win32.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "TypeDefs.h"
#include "HrException.h"
#include "Registry.h"

class File
{
private:
	static Map<String, ComPtr<ID3DBlob>>	_files;
	static WString*							_outputDirectory;

private:
	static void SetOutputDirectory();

public:
	static ID3DBlob* Get(const String& name);
	static ID3DBlob* Get(const WString& name);
	static void Unload(const String& name);
	static void Unload(const WString& name);
	static void UnloadAll();
};