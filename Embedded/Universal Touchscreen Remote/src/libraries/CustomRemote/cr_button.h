#ifndef __cr_button__
#define __cr_button__

#include "cr_common.h"

class CRButton
{
protected: 
    TFT_TYPE_PTR        m_tft;
    bool                m_isPressed;
    
public:
    CRPosition          Position;
    CRSize              Size;
    CRText              Text;
    CRColor             BorderColor;
    CRColor             FillColor;
    CRColor             PressedBorderColor;
    CRColor             PressedFillColor;
    uint8_t             BorderThickness;
    int16_t        	    Tag;
    
public:
    Event<CRButton*> Pressed;
    
public:
    CRButton(TFT_TYPE_PTR tft = 0);
    CRButton(const CRButton& other);
    ~CRButton();
 
public:
    virtual void Initialize(TFT_TYPE_PTR tft);
    virtual void SetTextPosition(int16_t offsetX, int16_t offsetY);
    virtual bool Update(int16_t x, int16_t y);
    virtual void Draw();
};

#endif