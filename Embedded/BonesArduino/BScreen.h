#ifndef __bones_screen__
#define __bones_screen__

#include "BonesConfig.h"
#include "BonesCommon.h"
#include "BButton.h"
#include "BTriangleButton.h"
#include "BThumbstick.h"

class BScreen
{  
protected:
    bool                m_isInitialized;
    TFT_TYPE_PTR        m_tft;
    
public:
    int8_t              ID;
    List<BButton*>     Buttons;
    BThumbstick        LeftThumbstick;
    BThumbstick        RightThumbstick;
    
public:
    BScreen(TFT_TYPE_PTR tft);
    ~BScreen();

private:
    void InitializeButtons();

public:
    void Update(int16_t x, int16_t y);
    virtual void Draw();

};

#endif