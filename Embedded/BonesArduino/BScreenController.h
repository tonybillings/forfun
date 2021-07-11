#ifndef __bones_screen_controller__
#define __bones_screen_controller__

#include "BonesCommon.h"
#include "BScreen.h"

class BScreenController
{
private:
    static BScreen*        	m_activeScreen;
    static TFT_TYPE_PTR     m_tft;
    static int              m_interruptPin;
    
public:
    static List<BScreen*> Screens;
 
public:
    static void Initialize(TFT_TYPE_PTR tft, int interruptPin);
    static void AddScreen(BScreen* screen);
    static void ShowScreen(BScreen* screen);
    static void ShowScreen(int8_t id);
    static void Update();
};

#endif