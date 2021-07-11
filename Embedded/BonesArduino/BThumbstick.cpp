#include "BThumbstick.h"

BThumbstick::BThumbstick(int16_t pinX, int16_t pinY, int16_t pinEnter, ThumbstickMode mode) : IsPressed(false), Tag(0)
{
    Initialize(pinX, pinY, pinEnter, mode);
}

BThumbstick::~BThumbstick () {}

void BThumbstick::Initialize(int16_t pinX, int16_t pinY, int16_t pinEnter, ThumbstickMode mode)
{
    m_pinX = pinX;
    m_pinY = pinY;
    m_pinEnter = pinEnter;
    m_mode = mode;
    m_state = IDLE;
    
    pinMode(pinEnter, INPUT_PULLUP);
}

ThumbstickState BThumbstick::GetState()
{
    return m_state;
}

bool BThumbstick::Update()
{
    this->PositionRaw.X = analogRead(m_pinX);
    this->PositionRaw.Y = analogRead(m_pinY);
    this->PositionNormal.X = NormalizePosition(PositionRaw.X);
    this->PositionNormal.Y = NormalizePosition(PositionRaw.Y);
    this->IsPressed = (digitalRead(m_pinEnter) == LOW);
    
    if (m_mode == DIGITAL)
    {
        ThumbstickState currentState;
        
        if (this->IsPressed)
            currentState = PRESSED;
        else if (this->PositionNormal.Y > 0.5f && this->PositionNormal.X > -0.5f && this->PositionNormal.X < 0.5f)
            currentState = UP;
        else if (this->PositionNormal.Y < -0.5f && this->PositionNormal.X > -0.5f && this->PositionNormal.X < 0.5f)
            currentState = DOWN;
        else if (this->PositionNormal.X < -0.5f && this->PositionNormal.Y < 0.5f && this->PositionNormal.Y > -0.5f)
            currentState = LEFT;
        else if (this->PositionNormal.X > 0.5f && this->PositionNormal.Y < 0.5f && this->PositionNormal.Y > -0.5f)
            currentState = RIGHT;
		else
			currentState = IDLE;
        
        if (currentState != m_state)
        {
            m_state = currentState;
            StateChanged(this);
            return true;
        }
        
        return false;
    }
    
    return true;
}

void BThumbstick::SetMode(ThumbstickMode mode)
{
    m_mode = mode;
}