#include "Serial.h"

namespace KSPSerial
{
	Serial::Serial(INT portNumber)
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
					Sleep(ARDUINO_WAIT_TIME);
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
		if (!m_connected) return false;

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