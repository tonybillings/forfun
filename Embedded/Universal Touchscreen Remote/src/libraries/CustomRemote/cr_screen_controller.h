#ifndef __cr_screen_controller__
#define __cr_screen_controller__

#include "cr_common.h"
#include "cr_screen.h"

class ScreenController
{
private:
    static tsMatrix_t       m_tsCalibratedMatrix;
    static CRScreen*        m_activeScreen;
    static TFT_TYPE_PTR     m_tft;
    static int              m_interruptPin;
    
public:
    static List<CRScreen*> Screens;
 
public:
    static void Initialize(TFT_TYPE_PTR tft, int interruptPin);
    static void AddScreen(CRScreen* screen);
    static void ShowScreen(CRScreen* screen);
    static void ShowScreen(int8_t id);
    static void Update();
};

#endif