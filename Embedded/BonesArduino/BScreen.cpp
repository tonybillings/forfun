#include "BScreen.h"

BScreen::BScreen(TFT_TYPE_PTR tft) : m_isInitialized(false), ID(-1) 
{
    m_tft = tft; 
    this->LeftThumbstick.Initialize(THUMBSTICK_1_PIN_X, THUMBSTICK_1_PIN_Y, THUMBSTICK_1_PIN_ENTER);
    this->RightThumbstick.Initialize(THUMBSTICK_2_PIN_X, THUMBSTICK_2_PIN_Y, THUMBSTICK_2_PIN_ENTER);
}

BScreen::~BScreen() { }

inline void BScreen::InitializeButtons()
{
    for (int i = 0; i < this->Buttons.Count(); i++)
        this->Buttons[i]->Initialize(m_tft);
    
    m_isInitialized = true;
}

void BScreen::Update(int16_t x, int16_t y)
{
    if (!m_isInitialized) { InitializeButtons(); }
    
    this->LeftThumbstick.Update();
    this->RightThumbstick.Update();
    
    for (int i = 0; i < this->Buttons.Count(); i++)
    {
        if (this->Buttons[i]->Update(x, y))
            break;
    }
}

void BScreen::Draw()
{
    if (!m_isInitialized) { InitializeButtons(); }
    
    for (int i = 0; i < this->Buttons.Count(); i++)
        this->Buttons[i]->Draw();
}