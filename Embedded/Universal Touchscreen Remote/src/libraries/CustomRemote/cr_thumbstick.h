#ifndef __cr_thumbstick__
#define __cr_thumbstick__

#include "cr_common.h"

enum ThumbstickMode { DIGITAL, ANALOG };
enum ThumbstickState { IDLE, PRESSED, UP, DOWN, LEFT, RIGHT };

class CRThumbstick
{
private: 
    ThumbstickMode      m_mode;
    ThumbstickState     m_state;
    int16_t             m_pinX;
    int16_t             m_pinY;
    int16_t             m_pinEnter;
        
public:
    CRPosition          PositionRaw;
    CRPositionF         PositionNormal;
    bool                IsPressed;
    int8_t              Tag;
    
private:
    inline float NormalizePosition(int16_t val)
    {
        return ((510.0f - val) / 510.0f) * -1.0f;
    }
    
public:
    Event<CRThumbstick*> StateChanged;
    
public:
    CRThumbstick(int16_t pinX = 0, int16_t pinY = 0, int16_t pinEnter = 0, ThumbstickMode mode = DIGITAL);
    ~CRThumbstick();
 
public:
    void Initialize(int16_t pinX = 0, int16_t pinY = 0, int16_t pinEnter = 0, ThumbstickMode mode = DIGITAL);
    bool Update();
    ThumbstickState GetState();
    void SetMode(ThumbstickMode mode);
};

#endif