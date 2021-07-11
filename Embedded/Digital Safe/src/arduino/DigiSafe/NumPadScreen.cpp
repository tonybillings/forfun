#include "NumPadScreen.h"

using namespace Bones;

namespace DigiSafe
{
	void NumPadScreen::Update(bool screenPressed, TSPoint point)
	{
		if (screenPressed)
		{
			int y = point.x;
			int x = map(point.y, 0, 240, 320, 0);
			int buttonCount = ARRAYSIZE(screenButtons);

			for (int i = 0; i < buttonCount; i++)
				screenButtons[i].ScreenPressed(x, y);
		}
	}

	void NumPadScreen::ResetNumPad()
	{
		Draw();
		NumPadController::Clear();
	}

	void NumPadScreen::Draw()
	{
		ScreenButton btnResetNumPad = ScreenButton(240, 50, 75, 25);
		//btnResetNumPad.ButtonPushed += ResetNumPad;
		DrawButton(btnResetNumPad);
		screenButtons[0] = btnResetNumPad;
	}

	void NumPadScreen::DrawButton(ScreenButton &button)
	{
		lcd->drawRect(button.Position.X, button.Position.Y, button.Size.Width, button.Size.Height, BLUE);
	}

	void NumPadScreen::DrawNumPad()
	{
		lcd->fillRect(0, 0, 220, 220, BLACK);
		int spacing = 50;
		int offsetX = 25;
		int offsetY = 25;
		int index = 0;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				Position pos(x * spacing + offsetX, y * spacing + offsetY);
				numpadButtons[index++] = NumPadButton(pos, 123);
				DrawNumPadButton(pos.X, pos.Y);
			}
		}
	}

	void NumPadScreen::DrawNumPadButton(int x, int y)
	{
		lcd->drawRect(x, y, 40, 40, BLUE);
	}

	void NumPadScreen::DrawPushedNumPadButton(int number, int count)
	{
		NumPadButton b = numpadButtons[number];
		lcd->fillRect(b.Position.X, b.Position.Y, 40, 40, BLUE);
		lcd->setCursor(b.Position.X + ((count < 10) ? 14 : 8), b.Position.Y + ((count < 10) ? 13 : 13));
		lcd->setTextColor(WHITE);
		lcd->setTextSize(2);
		lcd->print(count);
	}
}