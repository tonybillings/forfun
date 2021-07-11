#include "Texture2D.h"

WString* Texture2D::_outputDirectory = nullptr;

void Texture2D::SetOutputDirectory()
{
	TCHAR exeFullNameBuffer[MAX_PATH];
	if (!GetModuleFileName(NULL, exeFullNameBuffer, MAX_PATH))
		throw LAST_HR_EX();

	String exeFullName(exeFullNameBuffer);
	String exePath = exeFullName.substr(0, exeFullName.find_last_of('\\', exeFullName.size()));
	_outputDirectory = new WString(WStringConverter().from_bytes(exePath));
}

void Texture2D::Load(const Graphics* const pGraphics)
{
	if (_outputDirectory == nullptr)
		SetOutputDirectory();

	THROW_IF_FAILED(DirectX::CreateWICTextureFromFile(
		pGraphics->GetDevice(),
		pGraphics->GetDeviceContext(),
		(*_outputDirectory + WString(L"\\") + _fileName).c_str(),
		&_texture,
		&_textureView));
}