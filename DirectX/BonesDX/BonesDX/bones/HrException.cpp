#include "HrException.h"

HrException::HrException(const char* file, int line, HRESULT hr, bool throwIfFailed)
	: Exception(file, line), _hr(hr)
{
	if (throwIfFailed && FAILED(hr))
		throw HrException(file, line, hr, false);
}

const char* HrException::GetType() const noexcept
{
	return "HRESULT Exception";
}

String HrException::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);

	if (nMsgLen == 0)
		return "Unidentified error code";

	String errorString = pMsgBuf;
	LocalFree(pMsgBuf);

	return errorString;
}

HRESULT HrException::GetErrorCode() const noexcept
{
	return _hr;
}

String HrException::GetErrorCodeString() const noexcept
{
	std::ostringstream oss;
	oss << "0x" << std::hex << _hr;
	return oss.str();
}

String HrException::GetErrorString() const noexcept
{
	return TranslateErrorCode(_hr);
}

const char* HrException::what() const noexcept
{
	std::ostringstream oss;

	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCodeString() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();

	return (new String(oss.str()))->c_str();
}