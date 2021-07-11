#ifdef DECLSPEC_EXPORTING
	#define DECLSPEC __declspec(dllexport)
#else
	#define DECLSPEC __declspec(dllimport)
#endif

#include "Serial.h"

extern "C" 
{
	namespace KSPSerial
	{
		DECLSPEC BOOL Init(INT portNumber);
		DECLSPEC BOOL Write(BYTE* data, UINT length);
		DECLSPEC VOID Close();
		DECLSPEC BOOL IsConnected();
	}
}