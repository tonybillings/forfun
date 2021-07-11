#ifndef __bones_screens__
#define __bones_screens__

/*** INCLUDES ************************************************************************************/

#include "BonesConfig.h"
#include "BScreen.h"
#include "BScreenController.h"
#include "BThumbstick.h"

/*** DEFINES *************************************************************************************/


/*** GLOBAL FUNCTIONS ****************************************************************************/


/*** SBEENS *************************************************************************************/

class MainScreen : public BScreen
{
public:
    void OnLeftThumbstickStateChanged(BThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                
                break; 
            case UP:
			
                break; 
            case DOWN:
				
                break; 
            case LEFT:
			
                break; 
            case RIGHT:
               
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(BThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
               
                break; 
            case UP:
               
                break; 
            case DOWN:
               
                break; 
            case LEFT:
               
                break; 
            case RIGHT:
                
                break; 
        }
    }
    
    virtual void Draw()
    {
        BScreen::Draw();
    }
    
public:
    MainScreen(TFT_TYPE_PTR tft) : BScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<MainScreen, BThumbstick*>(this, &MainScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<MainScreen, BThumbstick*>(this, &MainScreen::OnRightThumbstickStateChanged);
        
        BButton templateButton;
        templateButton.BorderColor = BColor::Gray;
        templateButton.FillColor = BColor::Blue;
        templateButton.PressedBorderColor = BColor::Gray;
        templateButton.PressedFillColor = BColor::FromRGB(31, 37, 118);
        templateButton.BorderThickness = 5;
        
        static BButton btn1(templateButton);
		btn1.Size = BSize(148, 60);
        btn1.Position = BPosition(50, 130); 
        btn1.Text = "Press Me";
        btn1.SetTextPosition(10, 12);
		
        Buttons.Add(&btn1);
    }
};


/*** INITIALIZATION ******************************************************************************/

void InitializeScreens(TFT_TYPE_PTR tft)
{
	// Instantiate screens
	static MainScreen mainScreen(tft);
    
    // Add them to screen controller
    BScreenController::AddScreen(&mainScreen);
    
    // Show main screen upon boot
    BScreenController::ShowScreen(&mainScreen);
}

#endif