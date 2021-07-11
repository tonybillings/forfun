#include "BScreenController.h"

BScreen* 			BScreenController::m_activeScreen;
TFT_TYPE_PTR 		BScreenController::m_tft;
int 				BScreenController::m_interruptPin;
List<BScreen*> 		BScreenController::Screens;

void BScreenController::Initialize(TFT_TYPE_PTR tft, int interruptPin)
{
    m_activeScreen = 0;
    m_tft = tft;
    m_interruptPin = interruptPin;
}

void BScreenController::AddScreen(BScreen* screen)
{
    if (screen->ID == -1) { screen->ID = Screens.Count() + 1; }
    Screens.Add(screen);
}

void BScreenController::ShowScreen(BScreen* screen)
{
    for (int i = 0; i < Screens.Count(); i++)
    {
        if (Screens[i] == screen)
        {
            m_tft->fillScreen(BLACK);
            screen->Draw();
            m_activeScreen = screen;
			delay(150);
            break;
        }
    }
}

void BScreenController::ShowScreen(int8_t id)
{
    for (int i = 0; i < Screens.Count(); i++)
    {
        BScreen* screen = Screens[i];
        
        if (screen->ID == id)
        {
            m_tft->fillScreen(BLACK);
            screen->Draw();
            m_activeScreen = screen;
			delay(150);
            break;
        }
    }
}

void BScreenController::Update()
{
    // TODO: get x/y coordinate where screen is touched
	int x, y;
    
    if (m_activeScreen != 0)
        m_activeScreen->Update(x, y);
}
