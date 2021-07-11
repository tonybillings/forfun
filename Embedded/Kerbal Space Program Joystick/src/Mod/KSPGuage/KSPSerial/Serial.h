#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define ARDUINO_WAIT_TIME 2000

namespace KSPSerial
{
	class Serial
	{
	private:
		HANDLE		m_hSerial;
		BOOL		m_connected;
		COMSTAT		m_status;
		DWORD		m_error;

	public:
		Serial(INT portNumber);
		~Serial();

	public:
		BOOL WriteData(BYTE* buffer, UINT length);
		BOOL IsConnected();
		VOID Close();
	};
}

