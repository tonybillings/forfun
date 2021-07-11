#include "CSerial.h"

extern "C" 
{
	namespace CSerial
	{
		static Serial* s_serial = nullptr;

		BOOL Init(UINT portNumber)
		{
			s_serial = new Serial(portNumber);
			return s_serial->IsConnected();
		}

		BOOL Write(BYTE* data, UINT length)
		{
			if (s_serial != nullptr)
				return s_serial->WriteData(data, length);
			else
				return false;
		}

		BOOL ReadData(UINT bufferSize, LPVOID* buffer, LPDWORD bytesRead)
		{
			if (s_serial != nullptr)
				return s_serial->ReadData(bufferSize, buffer, bytesRead);
		}

		VOID ToggleDtr(UINT count)
		{
			if (s_serial != nullptr)
				s_serial->ToggleDtr(count);
		}

		VOID Close()
		{
			if (s_serial != nullptr)
				s_serial->Close();
		}

		BOOL IsConnected()
		{
			if (s_serial != nullptr && s_serial->IsConnected())
				return true;
			else
				return false;
		}
	}
}