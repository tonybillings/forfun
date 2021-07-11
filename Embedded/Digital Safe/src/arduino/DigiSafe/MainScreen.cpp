#include "MainScreen.h"

using namespace Bones;

namespace DigiSafe
{
#pragma region ScreenController Actions
	void MainScreen::Activate()
	{
		//packetReceivedHandler = SerialController::PacketReceived += new EventHandler<MainScreen, const Packet*>(this, &MainScreen::OnPacketReceived);
		GetIp();
	}

	void MainScreen::Deactivate()
	{
		SerialController::PacketReceived -= packetReceivedHandler;
	}

	void MainScreen::Update(bool screenPressed, TSPoint p)
	{
		if (screenPressed)
		{
			ipVisible = !ipVisible;
			DrawIp();
		}
	}
#pragma endregion

#pragma region SerialController Events
	void MainScreen::OnPacketReceived(const Packet* p)
	{
		switch (p->CommandId)
		{
		case IN_SETIP:
			SetIp(p);
			break;
		}
	}
#pragma endregion

#pragma region Serial Commands
	void MainScreen::GetIp()
	{
		Packet p;
		p.CommandId = OUT_GETIP;
		p.Length = 1;
		p.Send();
	}

	void MainScreen::SetIp(const Packet* p)
	{
		char ip[16];
		memset(ip, 0, 16);

		for (byte i = 0; i < 15; i++)
			ip[i] = p->Data[i];
	}
#pragma endregion

#pragma region Draw Actions
	void MainScreen::DrawIp()
	{
		lcd->setCursor(120, 138);
		lcd->setTextColor(ipVisible ? WHITE : BLACK);
		lcd->setTextSize(1);
		lcd->println(ip);
	}

	void MainScreen::DrawLogo()
	{
		byte* pX = logo.PixelsX;
		byte* pY = logo.PixelsY;

		for (int i = 0; i < logo.PixelsLength; i++)
			lcd->drawPixel(*pX++, *pY++, BLUE);

		for (int i = 0; i < logo.LinesLength; i += 3)
			lcd->drawFastHLine(logo.Lines[i], logo.Lines[i + 1], logo.Lines[i + 2], BLUE);
	}
#pragma endregion
}