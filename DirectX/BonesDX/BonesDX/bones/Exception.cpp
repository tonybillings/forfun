#include "Exception.h"

Exception::Exception(const char* file, int line, const char* message) noexcept
	: _file(file), _line(line), _message(message)
{}

const char* Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Message] " << _message << std::endl
		<< GetOriginString();

	return (new String(oss.str()))->c_str();
}

const char* Exception::GetType() const noexcept
{
	return "Exception";
}

int Exception::GetLine() const noexcept
{
	return _line;
}

const String& Exception::GetFile() const noexcept
{
	return _file;
}

String Exception::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << _file << std::endl
		<< "[Line] " << _line;

	return oss.str();
}