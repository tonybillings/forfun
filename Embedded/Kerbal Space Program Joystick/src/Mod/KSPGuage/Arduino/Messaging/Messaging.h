#include <Arduino.h>

#define MSG_SYN 22
#define MSG_STX 2
#define MSG_ETX 3
#define MSG_EOT 4

#define MSG_MAX_PAYLOAD_SIZE 100

/*-----------------------------------------------------------

USAGE:

void process_message(Message& msg)
{
	switch (msg.messageType)
	{
		// TODO: process message by type, using msg.payload 
		// and msg.payloadLength to get data
	}
}

void loop()
{
	static Message msg(process_message);
	
	if (Serial.available() > 0) 
		msg.processByte(Serial.read());
}


PACKET STRUCTURE (Top: byte length, Bottom: packet section):

| 1 | 1 | 1 | 003 | 1 | ?????????????? | 1 | 1 | 1 |
 SYN SYN MT   PL   STX      PAYLOAD     ETX CHK EOT
 
------------------------------------------------------------*/

enum MessageState
{
	SYN,	// Synchronization Idle (two of these must be sent consecutively)
	MT,		// Message Type (should not use flow control byte values)
	PL,		// Payload Length (encoded in ASCII) (set to "000" for no payload)
	STX,	// Start of Text
	PAY,	// Payload (encoded in ASCII) (is optional)
	ETX,	// End of Text
	CHK,	// Checksum (exclusive-or of payload bytes and ETX)
	EOT		// End of Transmission
};

struct Message
{
	typedef void(*ProcessMessage)(Message& msg);

	MessageState state;
	byte currentByte;
	byte syncCount;
	byte messageType;
	char payloadLengthBytes[4];
	byte payloadLength;
	char payload[MSG_MAX_PAYLOAD_SIZE];
	byte payloadIndex;

	ProcessMessage processMessage;

	Message(ProcessMessage function)
	{
		processMessage = function;
		reset();
	}

	void reset()
	{		
		state = SYN;
		currentByte = 0;
		syncCount = 0;
		messageType = 0;
		memset(payloadLengthBytes, 0, sizeof(payloadLengthBytes));
		payloadLength = 0;
		memset(payload, 0, sizeof(payload));
		payloadIndex = 0;
	}

	bool isPayloadValid(byte checksum)
	{
		byte chk = MSG_ETX;

		for (int i = 0; i < payloadLength; i++)
			chk ^= payload[i];

		return checksum == chk;
	}

	void processByte(byte value)
	{
		currentByte = value;

		switch (state)
		{
		case SYN:
			if (currentByte == MSG_SYN)
			{
				if (++syncCount == 2) { state = MT; }
			}
			else
			{
				syncCount = 0;
			}
			break;
		case MT:
			messageType = currentByte;
			state = PL;
			break;
		case PL:
			if (payloadLengthBytes[0] == 0)
			{
				payloadLengthBytes[0] = currentByte;
			}
			else if (payloadLengthBytes[1] == 0)
			{
				payloadLengthBytes[1] = currentByte;
			}
			else if (payloadLengthBytes[2] == 0)
			{
				payloadLengthBytes[2] = currentByte;
				payloadLength = atoi(payloadLengthBytes);
				state = (payloadLength  > 0) ? STX : EOT;
			}
			break;
		case STX:
			if (currentByte == MSG_STX)
				state = PAY;
			else
				reset();
			break;
		case PAY:
			payload[payloadIndex++] = currentByte;
			if (payloadIndex == payloadLength) { state = ETX; }
			break;
		case ETX:
			if (currentByte == MSG_ETX)
				state = CHK;
			else
				reset();
			break;
		case CHK:
			if (isPayloadValid(currentByte))
				state = EOT;
			else
				reset();
			break;
		case EOT:
			if (currentByte == MSG_EOT)
			{
				processMessage(*this);
				reset();
			}
			else
			{
				reset();
			}
			break;
		}
	}
};
