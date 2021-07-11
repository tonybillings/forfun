#include "Serial.h"

namespace CSerial
{
	Serial::Serial(UINT portNumber)
	{
		m_connected = false;

		char portName[] = { '\\', '\\', '.', '\\', 'C', 'O', 'M', '\0', '\0', '\0' };

		if (portNumber < 10)
		{
			portName[7] = (char)(portNumber + 48);
		}
		else
		{
			char portNumberStr[3];
			_itoa_s(portNumber, portNumberStr, 3, 10);
			portName[7] = portNumberStr[0];
			portName[8] = portNumberStr[1];
		}

		m_hSerial = CreateFileA(
			portName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (m_hSerial == INVALID_HANDLE_VALUE)
		{
			m_error = GetLastError();
		}
		else
		{
			DCB dcbSerialParams = { 0 };

			if (GetCommState(m_hSerial, &dcbSerialParams))
			{
				dcbSerialParams.BaudRate = CBR_115200;
				dcbSerialParams.ByteSize = 8;
				dcbSerialParams.StopBits = ONESTOPBIT;
				dcbSerialParams.Parity = NOPARITY;
				dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

				if (SetCommState(m_hSerial, &dcbSerialParams))
				{
					m_connected = true;
					PurgeComm(m_hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
					Sleep(WAIT_TIME);
				}
			}
		}
	}

	Serial::~Serial()
	{
		Close();
	}

	BOOL Serial::WriteData(BYTE* buffer, UINT length)
	{
		if (!m_connected || buffer == NULL || length == 0) return false;

		DWORD bytesSend;

		if (WriteFile(m_hSerial, (void*)buffer, length, &bytesSend, 0))
		{
			return true;
		}
		else
		{
			ClearCommError(m_hSerial, &m_error, &m_status);
			return false;
		}
	}

	BOOL Serial::ReadData(UINT bufferSize, LPVOID* buffer, LPDWORD bytesRead)
	{
		if (!m_connected) return false;
		*buffer = new DWORD[bufferSize];
		return ReadFile(m_hSerial, *buffer, bufferSize, bytesRead, NULL);
	}

	VOID Serial::ToggleDtr(UINT count)
	{
		EscapeCommFunction(m_hSerial, CLRDTR);
		Sleep(5);

		for (UINT i = 0; i < count; i++)
		{
			EscapeCommFunction(m_hSerial, SETDTR);
			Sleep(5);
			EscapeCommFunction(m_hSerial, CLRDTR);
			Sleep(5);
		}
	}

	BOOL Serial::IsConnected()
	{
		return m_connected;
	}

	VOID Serial::Close()
	{
		if (m_connected)
		{
			m_connected = false;
			CloseHandle(m_hSerial);
		}
	}
}