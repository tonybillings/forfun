#include "cr_triangle_button.h"

CRTriangleButton::CRTriangleButton(TFT_TYPE_PTR tft)
{
    Initialize(tft);
	m_isPressed = false;
}

CRTriangleButton::CRTriangleButton(const CRTriangleButton& other)
{
    m_tft = other.m_tft;
	m_isPressed = false;
    this->Position = other.Position;
    this->Size = other.Size;
    this->BorderColor = other.BorderColor;
    this->FillColor = other.FillColor;
    this->PressedBorderColor = other.PressedBorderColor;
    this->PressedFillColor = other.PressedFillColor;
    this->BorderThickness = other.BorderThickness;
    this->Tag = other.Tag;
	this->Triangle = other.Triangle;
}

CRTriangleButton::~CRTriangleButton()
{
    
}

void CRTriangleButton::Initialize(TFT_TYPE_PTR tft)
{
    m_tft = tft;
}

void CRTriangleButton::Draw()
{
    for (uint16_t i = 0; i < this->BorderThickness; i++)
        m_tft->drawRect(this->Position.X + i, this->Position.Y + i, this->Size.Width - (i * 2), this->Size.Height - (i * 2), (m_isPressed) ? this->PressedBorderColor.Value : this->BorderColor.Value);
    
    uint16_t fillSize = ((this->BorderThickness - 1) * 2) + 2;
    m_tft->fillRect(this->Position.X + this->BorderThickness, this->Position.Y + this->BorderThickness, this->Size.Width - fillSize, this->Size.Height - fillSize, (m_isPressed) ? this->PressedFillColor.Value : this->FillColor.Value);
	
	int16_t offsetX = this->Triangle.Position.X + this->Position.X;
	int16_t offsetY = this->Triangle.Position.Y + this->Position.Y;
	
	m_tft->fillTriangle(Triangle.Vertices[0] + offsetX, Triangle.Vertices[1] + offsetY, Triangle.Vertices[2] + offsetX, Triangle.Vertices[3] + offsetY, Triangle.Vertices[4] + offsetX, Triangle.Vertices[5] + offsetY, this->Triangle.Color.Value);
}

