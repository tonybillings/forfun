#include "KSPSerial.h"

extern "C" 
{
	namespace KSPSerial
	{
		static Serial* s_serial = nullptr;

		BOOL Init(INT portNumber)
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