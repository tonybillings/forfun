#ifndef __cr_screen__
#define __cr_screen__

#include "cr_config.h"
#include "cr_common.h"
#include "cr_button.h"
#include "cr_triangle_button.h"
#include "cr_thumbstick.h"

class CRScreen
{  
protected:
    bool                m_isInitialized;
    TFT_TYPE_PTR        m_tft;
    
public:
    int8_t              ID;
    List<CRButton*>     Buttons;
    CRThumbstick        LeftThumbstick;
    CRThumbstick        RightThumbstick;
    
public:
    CRScreen(TFT_TYPE_PTR tft);
    ~CRScreen();

private:
    void InitializeButtons();

public:
    void Update(int16_t x, int16_t y);
    virtual void Draw();

};

#endif