#ifdef DECLSPEC_EXPORTING
	#define DECLSPEC __declspec(dllexport)
#else
	#define DECLSPEC __declspec(dllimport)
#endif

#include "Serial.h"

extern "C" 
{
	namespace CSerial
	{
		DECLSPEC BOOL Init(UINT portNumber);
		DECLSPEC BOOL Write(BYTE* data, UINT length);
		DECLSPEC BOOL ReadData(UINT bufferSize, LPVOID* buffer, LPDWORD bytesRead);
		DECLSPEC VOID ToggleDtr(UINT count);
		DECLSPEC VOID Close();
		DECLSPEC BOOL IsConnected();
	}
}