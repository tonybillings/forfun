#include "CodeScreen.h"

using namespace Bones;

namespace DigiSafe
{
#pragma region ScreenController Actions
	void CodeScreen::Activate()
	{
		packetReceivedHandler = SerialController::PacketReceived += new EventHandler<CodeScreen, const Packet*>(this, &CodeScreen::OnPacketReceived);
		GetCode();
	}

	void CodeScreen::Deactivate()
	{
		SerialController::PacketReceived -= packetReceivedHandler;
	}

	void CodeScreen::Update(bool screenPressed, TSPoint p)
	{
		if (screenPressed)
		{
			ScreenController::ActivateScreen("Main");
		}
	}
#pragma endregion

#pragma region SerialController Events
	void CodeScreen::OnPacketReceived(const Packet* p)
	{
		switch (p->CommandId)
		{
		case IN_SETCODE:
			SetCode(p);
			break;
		}
	}
#pragma endregion

#pragma region Serial Commands
	void CodeScreen::GetCode()
	{
		Packet p;
		p.CommandId = OUT_GETCODE;
		p.Length = 0;
		p.Send();
	}

	void CodeScreen::SetCode(const Packet* p)
	{
		char code[14];
		byte index = 0;

		for (byte i = 0; i < 14; i += 2)
		{
			code[i] = p->Data[index++];
			code[i + 1] = ' ';
		}

		code[13] = 0;
		DrawCode(code);
	}
#pragma endregion

#pragma region Draw Actions	
	void CodeScreen::DrawCode(char code[])
	{
		lcd->drawFastHLine(0, 104, lcd->width(), BLUE);
		lcd->fillRect(0, 105, lcd->width(), 25, RED);
		lcd->drawFastHLine(0, 131, lcd->width(), BLUE);
		lcd->setCursor(45, 107);
		lcd->setTextColor(WHITE);
		lcd->setTextSize(3);
		lcd->println(code);
	}
#pragma endregion
}