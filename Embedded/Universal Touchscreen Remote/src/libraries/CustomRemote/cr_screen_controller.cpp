#include "cr_screen_controller.h"

tsMatrix_t  ScreenController::m_tsCalibratedMatrix;
CRScreen* ScreenController::m_activeScreen;
TFT_TYPE_PTR ScreenController::m_tft;
int ScreenController::m_interruptPin;
List<CRScreen*> ScreenController::Screens;

void ScreenController::Initialize(TFT_TYPE_PTR tft, int interruptPin)
{
    m_activeScreen = 0;
    m_tft = tft;
    m_interruptPin = interruptPin;
    readCalibrationMatrix(&m_tsCalibratedMatrix);
}

void ScreenController::AddScreen(CRScreen* screen)
{
    if (screen->ID == -1) { screen->ID = Screens.Count() + 1; }
    Screens.Add(screen);
}

void ScreenController::ShowScreen(CRScreen* screen)
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

void ScreenController::ShowScreen(int8_t id)
{
    for (int i = 0; i < Screens.Count(); i++)
    {
        CRScreen* screen = Screens[i];
        
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

void ScreenController::Update()
{
    tsPoint_t raw;
    tsPoint_t cal;
    uint16_t x, y;
    memset(&cal, 0, sizeof(tsPoint_t));
    
    if (!digitalRead(m_interruptPin))
    {
        if (m_tft->touched()) 
        {
            m_tft->touchRead(&x, &y);
            raw.x = x;
            raw.y = y;
            calibrateTSPoint(&cal, &raw, &m_tsCalibratedMatrix);        
        }
    }
    
    if (m_activeScreen != 0)
        m_activeScreen->Update(cal.x, cal.y);
}
