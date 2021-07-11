#pragma once

#include <exception>
#include <string>
#include <sstream>
#include "TypeDefs.h"

class Exception : public std::exception
{
protected:
	String				_file;
	int					_line;
	mutable String		_message;

public:
	Exception(const char* file, int line, const char* message = "") noexcept;

public:
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const String& GetFile() const noexcept;
	String GetOriginString() const noexcept;
};

#define EX(MSG) Exception(__FILE__, __LINE__, MSG)