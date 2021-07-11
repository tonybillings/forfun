#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#define WAIT_TIME 500

namespace CSerial
{
	class Serial
	{
	private:
		HANDLE		m_hSerial;
		BOOL		m_connected;
		COMSTAT		m_status;
		DWORD		m_error;

	public:
		Serial(UINT portNumber);
		~Serial();

	public:
		BOOL WriteData(BYTE* buffer, UINT length);
		BOOL ReadData(UINT bufferSize, LPVOID* buffer, LPDWORD bytesRead);
		VOID ToggleDtr(UINT count);
		BOOL IsConnected();
		VOID Close();
	};
}

