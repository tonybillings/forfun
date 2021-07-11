#include "File.h"
#include "HrException.h"

Map<String, ComPtr<ID3DBlob>> File::_files;
WString* File::_outputDirectory = nullptr;

void File::SetOutputDirectory()
{
	TCHAR exeFullNameBuffer[MAX_PATH];
	if (!GetModuleFileName(NULL, exeFullNameBuffer, MAX_PATH))
		throw LAST_HR_EX();

	String exeFullName(exeFullNameBuffer);
	String exePath = exeFullName.substr(0, exeFullName.find_last_of('\\', exeFullName.size()));
	_outputDirectory = new WString(WStringConverter().from_bytes(exePath));
}

ID3DBlob* File::Get(const String& name)
{
	if (_files.count(name))
	{
		return _files.at(name).Get();
	}
	else
	{
		if (_outputDirectory == nullptr)
			SetOutputDirectory();

		WString fullName = *_outputDirectory + WString(L"\\") + WStringConverter().from_bytes(name);

		ComPtr<ID3DBlob> file;
		THROW_IF_FAILED(D3DReadFileToBlob(fullName.c_str(), &file));
		_files.insert(std::pair(name, file));
		return file.Get();
	}
}

ID3DBlob* File::Get(const WString& name)
{
	String nName = WStringConverter().to_bytes(name);

	if (_files.count(nName))
	{
		return _files.at(nName).Get();
	}
	else
	{
		if (_outputDirectory == nullptr)
			SetOutputDirectory();

		WString fullName = *_outputDirectory + WString(L"\\") + name;

		ComPtr<ID3DBlob> file;
		THROW_IF_FAILED(D3DReadFileToBlob(fullName.c_str(), &file));
		_files.insert(std::pair(nName, file));
		return file.Get();
	}
}

void File::Unload(const String& name)
{
	if (_files.count(name))
		_files.erase(name);
}

void File::Unload(const WString& name)
{
	String nName = WStringConverter().to_bytes(name);

	if (_files.count(nName))
		_files.erase(nName);
}

void File::UnloadAll()
{
	_files.clear();
}