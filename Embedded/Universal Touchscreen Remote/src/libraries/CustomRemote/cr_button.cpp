#include "cr_button.h"

CRButton::CRButton(TFT_TYPE_PTR tft) : m_isPressed(false), BorderThickness(1), Tag(0)
{
    Initialize(tft);
}

CRButton::CRButton(const CRButton& other) : m_isPressed(false)
{
    m_tft = other.m_tft;
    this->Position = other.Position;
    this->Size = other.Size;
    this->Text = other.Text;
    this->BorderColor = other.BorderColor;
    this->FillColor = other.FillColor;
    this->PressedBorderColor = other.PressedBorderColor;
    this->PressedFillColor = other.PressedFillColor;
    this->BorderThickness = other.BorderThickness;
    this->Tag = other.Tag;
}

CRButton::~CRButton()
{
    
}

void CRButton::Initialize(TFT_TYPE_PTR tft)
{
    m_tft = tft;
}

void CRButton::SetTextPosition(int16_t offsetX, int16_t offsetY)
{
    this->Text.Position = CRPosition(this->Position.X + offsetX, this->Position.Y + offsetY);  
}

bool CRButton::Update(int16_t x, int16_t y)
{
    if (x >= this->Position.X && x <= (this->Position.X + this->Size.Width) && y >= this->Position.Y && y <= (this->Position.Y + this->Size.Height))
    {
        if (!m_isPressed)
        {
            m_isPressed = true;
            Draw();
            Pressed(this);
        }
        
        return true;
    }
    else
    {
        if (m_isPressed)
        {
            m_isPressed = false;
            Draw();
        }
        
        return false;
    }
}

void CRButton::Draw()
{
    for (uint16_t i = 0; i < this->BorderThickness; i++)
        m_tft->drawRect(this->Position.X + i, this->Position.Y + i, this->Size.Width - (i * 2), this->Size.Height - (i * 2), (m_isPressed) ? this->PressedBorderColor.Value : this->BorderColor.Value);
    
    uint16_t fillSize = ((this->BorderThickness - 1) * 2) + 2;
    m_tft->fillRect(this->Position.X + this->BorderThickness, this->Position.Y + this->BorderThickness, this->Size.Width - fillSize, this->Size.Height - fillSize, (m_isPressed) ? this->PressedFillColor.Value : this->FillColor.Value);
    
    if (this->Text.Value != "")
    {
        m_tft->textMode();
        m_tft->textSetCursor(this->Text.Position.X, this->Text.Position.Y);
        m_tft->textTransparent((m_isPressed) ? this->Text.PressedColor.Value : this->Text.Color.Value);
        m_tft->textEnlarge(this->Text.Size);
        m_tft->textWrite(this->Text.Value.c_str(), this->Text.Value.length());
        m_tft->graphicsMode();
    }
}

