#pragma once

#include "Win32.h"
#include "Exception.h"

class HrException : public Exception
{
protected:
	HRESULT _hr;

public:
	HrException(const char* file, int line, HRESULT hr, bool throwIfFailed = false);
	virtual const char* GetType() const noexcept;
	HRESULT GetErrorCode() const noexcept;
	String GetErrorCodeString() const noexcept;
	String GetErrorString() const noexcept;
	const char* what() const noexcept override;

protected:
	static String TranslateErrorCode(HRESULT hr);
};

#define HR_EX(HR) HrException(__FILE__, __LINE__, HR)
#define LAST_HR_EX() HrException(__FILE__, __LINE__, GetLastError())
#define THROW_IF_FAILED(HRCALL) HrException(__FILE__, __LINE__, (HRCALL), true)