#include "serialcontroller.h"

using namespace Bones;

#pragma region Initialization
	Event<const Packet*> SerialController::PacketReceived;
#pragma endregion

#pragma region Core Actions
void SerialController::Begin()
{
	pinMode(13, OUTPUT);
	Serial.begin(9600);
}

void SerialController::Update()
{
	if (Serial.available())
	{
		digitalWrite(13, HIGH);

		Packet packet = Packet::GetFromSerial();
		if (packet.IsValid)
			PacketReceived(&packet);
		else
			Clear();

		digitalWrite(13, LOW);
	}
}
#pragma endregion

#pragma region Serial Actions
byte SerialController::Read()
{
	byte b = Serial.read();
	delay(2);
	return b;
}

bool SerialController::Read(byte* value)
{
	if (Serial.available())
	{
		*value = Serial.read();
		delay(2);
		return true;
	}
	else
	{
		return false;
	}
}

void SerialController::Clear()
{
	while (Serial.available())
	{
		delay(2);
		Serial.read();
	}
}
#pragma endregion

#pragma region Packet
	void Packet::Send()
	{
		size_t size = Length + PACKET_START_CHAR_COUNT + PACKET_END_CHAR_COUNT + 2;
		uint8_t* packet = new uint8_t[size];

		for (byte i = 0; i < PACKET_START_CHAR_COUNT; i++)
			packet[i] = PACKET_START_CHAR;

		packet[PACKET_START_CHAR_COUNT] = CommandId;
		packet[PACKET_START_CHAR_COUNT + 1] = Length;

		for (byte i = 0; i < Length; i++)
			packet[i + PACKET_START_CHAR_COUNT + 2] = Data[i];

		for (byte i = size - 1; i >= size - PACKET_END_CHAR_COUNT; i--)
			packet[i] = PACKET_END_CHAR;

		Serial.write(packet, size);
		delete[] packet;
	}

	Packet Packet::GetFromSerial()
	{
		Packet p;
		byte* b;

		for (byte i = 0; i < PACKET_START_CHAR_COUNT; i++)
			if (!SerialController::Read(b) || *b != PACKET_START_CHAR) { return p; }

		if (!SerialController::Read(&p.CommandId)) { return p; }
		if (!SerialController::Read(&p.Length)) { return p; }

		if (p.Length > PACKET_DATA_SIZE) { return p; }

		for (byte i = 0; i < p.Length; i++)
			if (!SerialController::Read(&p.Data[i])) { return p; }

		for (byte i = 0; i < PACKET_END_CHAR_COUNT; i++)
			if (!SerialController::Read(b) || *b != PACKET_END_CHAR) { return p; }

		p.IsValid = true;
		return p;
	}
#pragma endregion
