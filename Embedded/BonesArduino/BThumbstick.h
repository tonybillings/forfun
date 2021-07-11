#ifndef __bones_thumbstick__
#define __bones_thumbstick__

#include "BonesCommon.h"

enum ThumbstickMode { DIGITAL, ANALOG };
enum ThumbstickState { IDLE, PRESSED, UP, DOWN, LEFT, RIGHT };

class BThumbstick
{
private: 
    ThumbstickMode      m_mode;
    ThumbstickState     m_state;
    int16_t             m_pinX;
    int16_t             m_pinY;
    int16_t             m_pinEnter;
        
public:
    BPosition          PositionRaw;
    BPositionF         PositionNormal;
    bool                IsPressed;
    int8_t              Tag;
    
private:
    inline float NormalizePosition(int16_t val)
    {
        return ((510.0f - val) / 510.0f) * -1.0f;
    }
    
public:
    Event<BThumbstick*> StateChanged;
    
public:
    BThumbstick(int16_t pinX = 0, int16_t pinY = 0, int16_t pinEnter = 0, ThumbstickMode mode = DIGITAL);
    ~BThumbstick();
 
public:
    void Initialize(int16_t pinX = 0, int16_t pinY = 0, int16_t pinEnter = 0, ThumbstickMode mode = DIGITAL);
    bool Update();
    ThumbstickState GetState();
    void SetMode(ThumbstickMode mode);
};

#endif