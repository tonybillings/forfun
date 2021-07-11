#ifndef __bones_serialcontroller_H__
#define __bones_serialcontroller_H__

#include <Arduino.h>
#include "event.h"

#define PACKET_DATA_SIZE 32
#define PACKET_START_CHAR 187
#define PACKET_END_CHAR 171
#define PACKET_START_CHAR_COUNT 3
#define PACKET_END_CHAR_COUNT 3

namespace Bones
{
	class Packet
	{
	public:
		byte CommandId;
		byte Data[PACKET_DATA_SIZE];
		byte Length;
		bool IsValid;
	public:
		Packet() : CommandId(-1), Length(-1), IsValid(false) 
		{
			memset(Data, 0, PACKET_DATA_SIZE);
		}
	public:
		void Send();
		static Packet GetFromSerial();
	};

	class SerialController
	{
	public:
		static void Begin();
		static void Update();
		static void Clear();
		static byte Read();
		static bool Read(byte* value);
	public:
		static Event<const Packet*> PacketReceived;
	};
}

#endif
