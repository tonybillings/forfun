#ifndef __bones_button__
#define __bones_button__

#include "BonesCommon.h"

class BButton
{
protected: 
    TFT_TYPE_PTR        m_tft;
    bool                m_isPressed;
    
public:
    BPosition          Position;
    BSize              Size;
    BText              Text;
    BColor             BorderColor;
    BColor             FillColor;
    BColor             PressedBorderColor;
    BColor             PressedFillColor;
    uint8_t             BorderThickness;
    int16_t        	    Tag;
    
public:
    Event<BButton*> Pressed;
    
public:
    BButton(TFT_TYPE_PTR tft = 0);
    BButton(const BButton& other);
    ~BButton();
 
public:
    virtual void Initialize(TFT_TYPE_PTR tft);
    virtual void SetTextPosition(int16_t offsetX, int16_t offsetY);
    virtual bool Update(int16_t x, int16_t y);
    virtual void Draw();
};

#endif