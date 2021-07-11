#ifndef __cr_screens__
#define __cr_screens__

/*** INCLUDES ************************************************************************************/

#include "cr_config.h"
#include "cr_screen.h"
#include "cr_screen_controller.h"
#include "cr_remote_controller.h"
#include "cr_thumbstick.h"

/*** DEFINES *************************************************************************************/

#define SCREEN_ROKU_MAIN		1 
#define SCREEN_ROKU_FAV			2 
#define SCREEN_XFINITY_MAIN		3
#define SCREEN_XFINITY_FAV		4
#define SCREEN_XFINITY_NAV		5
#define SCREEN_TV_MAIN			6
#define SCREEN_TV_ADV			7
#define SCREEN_LIGHTS_MAIN		8

/*** GLOBAL FUNCTIONS ****************************************************************************/

void SendChannelNumber(int number)
{
	switch (number)
	{
		case 0:
			RemoteController::ExecCommand(RC_CABLE_0);
			break;
		case 1:
			RemoteController::ExecCommand(RC_CABLE_1);
			break;
		case 2:
			RemoteController::ExecCommand(RC_CABLE_2);
			break;
		case 3:
			RemoteController::ExecCommand(RC_CABLE_3);
			break;
		case 4:
			RemoteController::ExecCommand(RC_CABLE_4);
			break;
		case 5:
			RemoteController::ExecCommand(RC_CABLE_5);
			break;
		case 6:
			RemoteController::ExecCommand(RC_CABLE_6);
			break;
		case 7:
			RemoteController::ExecCommand(RC_CABLE_7);
			break;
		case 8:
			RemoteController::ExecCommand(RC_CABLE_8);
			break;
		case 9:
			RemoteController::ExecCommand(RC_CABLE_9);
			break;
	}
}

void OnButtonPressed_ExecCommand(CRButton* sender)
{
    RemoteController::ExecCommand(sender->Tag);
}

void OnButtonPressed_ShowScreen(CRButton* sender)
{
    ScreenController::ShowScreen(sender->Tag);
}

void OnButtonPressed_ChangeChannel(CRButton* sender)
{
	char channel[4];
	itoa(sender->Tag, channel, 10);
	int chan1 = ((int)channel[0]) - 48;
	int chan2 = ((int)channel[1]) - 48;
	int chan3 = ((int)channel[2]) - 48;
	SendChannelNumber(chan1);
	delay(64);
	SendChannelNumber(chan2);
	delay(64);
	SendChannelNumber(chan3);
	delay(96);
	RemoteController::ExecCommand(RC_CABLE_ENTER);
	delay(64);
}

void DrawScreenTitle(TFT_TYPE_PTR tft, const char* text)
{
	tft->textMode();
    tft->textSetCursor(5, 0);
    tft->textTransparent(BLUE);
    tft->textEnlarge(1);
    tft->textWrite(text);
    tft->graphicsMode();
}

void UpdateMap(TFT_TYPE_PTR tft, int screen)
{
	tft->drawRect(720, 5, 15, 15, BLUE);
	tft->drawRect(720, 25, 15, 15, BLUE);
	
	tft->drawRect(740, 5, 15, 15, BLUE);
	tft->drawRect(740, 25, 15, 15, BLUE);
	tft->drawRect(740, 45, 15, 15, BLUE);
	
	tft->drawRect(760, 5, 15, 15, BLUE);
	tft->drawRect(760, 25, 15, 15, BLUE);
	
	tft->drawRect(780, 5, 15, 15, BLUE);
	
	switch(screen)
	{
		case SCREEN_ROKU_MAIN:
			tft->fillRect(720, 5, 15, 15, BLUE);
			break;
		case SCREEN_ROKU_FAV:
			tft->fillRect(720, 25, 15, 15, BLUE);
			break;
		case SCREEN_XFINITY_MAIN:
			tft->fillRect(740, 5, 15, 15, BLUE);
			break;
		case SCREEN_XFINITY_FAV:
			tft->fillRect(740, 25, 15, 15, BLUE);
			break;
		case SCREEN_XFINITY_NAV:
			tft->fillRect(740, 45, 15, 15, BLUE);
			break;
		case SCREEN_TV_MAIN:
			tft->fillRect(760, 5, 15, 15, BLUE);
			break;
		case SCREEN_TV_ADV:
			tft->fillRect(760, 25, 15, 15, BLUE);
			break;
		case SCREEN_LIGHTS_MAIN:
			tft->fillRect(780, 5, 15, 15, BLUE);
			break;
	}
}

/*** SCREENS *************************************************************************************/

class RokuMainScreen : public CRScreen
{
public:
    void OnLeftThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_ROKU_BACK);
                break; 
            case UP:
                break; 
            case DOWN:
				ScreenController::ShowScreen(SCREEN_ROKU_FAV);
                break; 
            case LEFT:
                break; 
            case RIGHT:
                ScreenController::ShowScreen(SCREEN_XFINITY_MAIN);
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_ROKU_ENTER);
                break; 
            case UP:
                RemoteController::ExecCommand(RC_ROKU_UP);
                break; 
            case DOWN:
                RemoteController::ExecCommand(RC_ROKU_DOWN);
                break; 
            case LEFT:
                RemoteController::ExecCommand(RC_ROKU_LEFT);
                break; 
            case RIGHT:
                RemoteController::ExecCommand(RC_ROKU_RIGHT);
                break; 
        }
    }
    
    virtual void Draw()
    {
        DrawScreenTitle(m_tft, "Roku::main");
		UpdateMap(m_tft, SCREEN_ROKU_MAIN);
        CRScreen::Draw();
    }
    
public:
    RokuMainScreen(TFT_TYPE_PTR tft) : CRScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<RokuMainScreen, CRThumbstick*>(this, &RokuMainScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<RokuMainScreen, CRThumbstick*>(this, &RokuMainScreen::OnRightThumbstickStateChanged);
        
        CRButton templateButton;
        templateButton.BorderColor = CRColor::Gray;
        templateButton.FillColor = CRColor::Blue;
        templateButton.PressedBorderColor = CRColor::Gray;
        templateButton.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateButton.BorderThickness = 5;
        
        static CRButton tvPower(templateButton);
		tvPower.Size = CRSize(148, 60);
        tvPower.Position = CRPosition(50, 130); 
        tvPower.Text = "TV Power";
        tvPower.SetTextPosition(10, 12);
        tvPower.Tag = RC_TV_POWER;
        tvPower.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRButton hdmiSwitch(templateButton);
		hdmiSwitch.Size = CRSize(148, 60);
        hdmiSwitch.Position = CRPosition(323, 130); 
        hdmiSwitch.Text = "Switch";
        hdmiSwitch.SetTextPosition(25, 12);
        hdmiSwitch.Tag = RC_HDMI_ROKU;
        hdmiSwitch.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton home(templateButton);
		home.Size = CRSize(148, 60);
        home.Position = CRPosition(596, 130); 
        home.Text = "Home";
        home.SetTextPosition(43, 12);
        home.Tag = RC_ROKU_HOME;
        home.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton rewind(templateButton);
		rewind.Size = CRSize(148, 60);
        rewind.Position = CRPosition(50, 283); 
        rewind.Text = "<<";
        rewind.SetTextPosition(58, 12);
        rewind.Tag = RC_ROKU_REWIND;
        rewind.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton playPause(templateButton);
		playPause.Size = CRSize(148, 60);
        playPause.Position = CRPosition(323, 283); 
        playPause.Text = ">||";
        playPause.SetTextPosition(51, 12);
        playPause.Tag = RC_ROKU_PLAY_PAUSE;
        playPause.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton forward(templateButton);
		forward.Size = CRSize(148, 60);
        forward.Position = CRPosition(596, 283); 
        forward.Text = ">>";
        forward.SetTextPosition(62, 12);
        forward.Tag = RC_ROKU_FORWARD;
        forward.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
        Buttons.Add(&tvPower);
        Buttons.Add(&hdmiSwitch);
		Buttons.Add(&home);
		Buttons.Add(&rewind);
		Buttons.Add(&playPause);
		Buttons.Add(&forward);
    }
};

/*-----------------------------------------------------------------------------------------------*/

class RokuFavScreen : public CRScreen
{
public:
    void OnLeftThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_ROKU_BACK);
                break; 
            case UP:
				ScreenController::ShowScreen(SCREEN_ROKU_MAIN);
                break; 
            case DOWN:
                break; 
            case LEFT:
                break; 
            case RIGHT:
                ScreenController::ShowScreen(SCREEN_XFINITY_MAIN);
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_ROKU_ENTER);
                break; 
            case UP:
                RemoteController::ExecCommand(RC_ROKU_UP);
                break; 
            case DOWN:
                RemoteController::ExecCommand(RC_ROKU_DOWN);
                break; 
            case LEFT:
                RemoteController::ExecCommand(RC_ROKU_LEFT);
                break; 
            case RIGHT:
                RemoteController::ExecCommand(RC_ROKU_RIGHT);
                break; 
        }
    }
    
    virtual void Draw()
    {
        DrawScreenTitle(m_tft, "Roku::favorites");
		UpdateMap(m_tft, SCREEN_ROKU_FAV);
        CRScreen::Draw();
    }
    
public:
    RokuFavScreen(TFT_TYPE_PTR tft) : CRScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<RokuFavScreen, CRThumbstick*>(this, &RokuFavScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<RokuFavScreen, CRThumbstick*>(this, &RokuFavScreen::OnRightThumbstickStateChanged);
        
        CRButton templateButton;
        templateButton.BorderColor = CRColor::Gray;
        templateButton.FillColor = CRColor::Blue;
        templateButton.PressedBorderColor = CRColor::Gray;
        templateButton.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateButton.BorderThickness = 5;
        
        static CRButton netflix(templateButton);
		netflix.Size = CRSize(148, 60);
        netflix.Position = CRPosition(190, 130); 
        netflix.Text = "Netflix";
        netflix.SetTextPosition(17, 12);
        netflix.Tag = RC_ROKU_NETFLIX;
        netflix.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRButton amazon(templateButton);
		amazon.Size = CRSize(148, 60);
        amazon.Position = CRPosition(463, 130); 
        amazon.Text = "Amazon";
        amazon.SetTextPosition(25, 12);
        amazon.Tag = RC_ROKU_AMAZON;
        amazon.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton mgo(templateButton);
		mgo.Size = CRSize(148, 60);
        mgo.Position = CRPosition(190, 283); 
        mgo.Text = "M-GO";
        mgo.SetTextPosition(44, 12);
        mgo.Tag = RC_ROKU_MGO;
        mgo.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton blockbuster(templateButton);
		blockbuster.Size = CRSize(148, 60);
        blockbuster.Position = CRPosition(463, 283); 
        blockbuster.Text = "Blk Bstr";
        blockbuster.SetTextPosition(12, 12);
        blockbuster.Tag = RC_ROKU_BLOCKBUSTER;
        blockbuster.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
        Buttons.Add(&netflix);
        Buttons.Add(&amazon);
		Buttons.Add(&mgo);
		Buttons.Add(&blockbuster);
    }
};

/*-----------------------------------------------------------------------------------------------*/

class XfinityMainScreen : public CRScreen
{
public:
    void OnLeftThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_CABLE_EXIT);
                break; 
            case UP:
                break; 
            case DOWN:
				ScreenController::ShowScreen(SCREEN_XFINITY_FAV);
                break; 
            case LEFT:
				ScreenController::ShowScreen(SCREEN_ROKU_MAIN);
                break; 
            case RIGHT:
                ScreenController::ShowScreen(SCREEN_TV_MAIN);
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_CABLE_ENTER);
                break; 
            case UP:
                RemoteController::ExecCommand(RC_CABLE_UP);
                break; 
            case DOWN:
                RemoteController::ExecCommand(RC_CABLE_DOWN);
                break; 
            case LEFT:
                RemoteController::ExecCommand(RC_CABLE_LEFT);
                break; 
            case RIGHT:
                RemoteController::ExecCommand(RC_CABLE_RIGHT);
                break; 
        }
    }
    
    virtual void Draw()
    {
        DrawScreenTitle(m_tft, "Xfinity::main");
		UpdateMap(m_tft, SCREEN_XFINITY_MAIN);
        CRScreen::Draw();
    }
    
public:
    XfinityMainScreen(TFT_TYPE_PTR tft) : CRScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<XfinityMainScreen, CRThumbstick*>(this, &XfinityMainScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<XfinityMainScreen, CRThumbstick*>(this, &XfinityMainScreen::OnRightThumbstickStateChanged);
        
        CRButton templateButton;
        templateButton.BorderColor = CRColor::Gray;
        templateButton.FillColor = CRColor::Blue;
        templateButton.PressedBorderColor = CRColor::Gray;
        templateButton.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateButton.BorderThickness = 5;
        
        static CRButton tvPower(templateButton);
		tvPower.Size = CRSize(148, 60);
        tvPower.Position = CRPosition(50, 130); 
        tvPower.Text = "TV Power";
        tvPower.SetTextPosition(10, 12);
        tvPower.Tag = RC_TV_POWER;
        tvPower.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRButton hdmiSwitch(templateButton);
		hdmiSwitch.Size = CRSize(148, 60);
        hdmiSwitch.Position = CRPosition(323, 130); 
        hdmiSwitch.Text = "Switch";
        hdmiSwitch.SetTextPosition(25, 12);
        hdmiSwitch.Tag = RC_HDMI_CABLE;
        hdmiSwitch.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton cablePower(templateButton);
		cablePower.Size = CRSize(148, 60);
        cablePower.Position = CRPosition(596, 130); 
        cablePower.Text = "Power";
        cablePower.SetTextPosition(35, 12);
        cablePower.Tag = RC_CABLE_POWER;
        cablePower.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRButton guide(templateButton);
		guide.Size = CRSize(148, 60);
        guide.Position = CRPosition(50, 283); 
        guide.Text = "Guide";
        guide.SetTextPosition(35, 12);
        guide.Tag = RC_CABLE_GUIDE;
        guide.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton info(templateButton);
		info.Size = CRSize(148, 60);
        info.Position = CRPosition(323, 283); 
        info.Text = "Info";
        info.SetTextPosition(41, 12);
        info.Tag = RC_CABLE_INFO;
        info.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton menu(templateButton);
		menu.Size = CRSize(148, 60);
        menu.Position = CRPosition(596, 283); 
        menu.Text = "Menu";
        menu.SetTextPosition(44, 12);
        menu.Tag = RC_CABLE_MENU;
        menu.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
        Buttons.Add(&tvPower);
        Buttons.Add(&cablePower);
		Buttons.Add(&hdmiSwitch);
		Buttons.Add(&guide);
		Buttons.Add(&info);
		Buttons.Add(&menu);
    }
};

/*-----------------------------------------------------------------------------------------------*/

class XfinityFavScreen : public CRScreen
{
public:
    void OnLeftThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_CABLE_EXIT);
                break; 
            case UP:
				ScreenController::ShowScreen(SCREEN_XFINITY_MAIN);
                break; 
            case DOWN:
				ScreenController::ShowScreen(SCREEN_XFINITY_NAV);
                break; 
            case LEFT:
				ScreenController::ShowScreen(SCREEN_ROKU_MAIN);
                break; 
            case RIGHT:
                ScreenController::ShowScreen(SCREEN_TV_MAIN);
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_CABLE_ENTER);
                break; 
            case UP:
                RemoteController::ExecCommand(RC_CABLE_UP);
                break; 
            case DOWN:
                RemoteController::ExecCommand(RC_CABLE_DOWN);
                break; 
            case LEFT:
                RemoteController::ExecCommand(RC_CABLE_LEFT);
                break; 
            case RIGHT:
                RemoteController::ExecCommand(RC_CABLE_RIGHT);
                break; 
        }
    }
    
    virtual void Draw()
    {
        DrawScreenTitle(m_tft, "Xfinity::favorites");
		UpdateMap(m_tft, SCREEN_XFINITY_FAV);
        CRScreen::Draw();
    }
    
public:
    XfinityFavScreen(TFT_TYPE_PTR tft) : CRScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<XfinityFavScreen, CRThumbstick*>(this, &XfinityFavScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<XfinityFavScreen, CRThumbstick*>(this, &XfinityFavScreen::OnRightThumbstickStateChanged);
        
        CRButton templateButton;
        templateButton.BorderColor = CRColor::Gray;
        templateButton.FillColor = CRColor::Blue;
        templateButton.PressedBorderColor = CRColor::Gray;
        templateButton.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateButton.BorderThickness = 5;
        
		static CRButton ap(templateButton);
		ap.Size = CRSize(148, 60);
        ap.Position = CRPosition(50, 130); 
        ap.Text = "AP";
        ap.SetTextPosition(60, 12);
        ap.Tag = 241;
        ap.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		
		static CRButton disc(templateButton);
		disc.Size = CRSize(148, 60);
        disc.Position = CRPosition(50, 230); 
        disc.Text = "DISC";
        disc.SetTextPosition(43, 12);
        disc.Tag = 239;
        disc.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		
		static CRButton as(templateButton);
		as.Size = CRSize(148, 60);
        as.Position = CRPosition(50, 330); 
        as.Text = "AS";
        as.SetTextPosition(59, 12);
        as.Tag = 878;
        as.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		
		static CRButton cspan(templateButton);
		cspan.Size = CRSize(148, 60);
        cspan.Position = CRPosition(323, 130); 
        cspan.Text = "CSPAN";
        cspan.SetTextPosition(37, 12);
        cspan.Tag = 941;
        cspan.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		
		static CRButton cnn(templateButton);
		cnn.Size = CRSize(148, 60);
        cnn.Position = CRPosition(323, 230); 
        cnn.Text = "CNN";
        cnn.SetTextPosition(49, 12);
        cnn.Tag = 234;
        cnn.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		      
		static CRButton com(templateButton);
		com.Size = CRSize(148, 60);
        com.Position = CRPosition(323, 330); 
        com.Text = "COM";
        com.SetTextPosition(50, 12);
        com.Tag = 828;
        com.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		
		static CRButton fox(templateButton);
		fox.Size = CRSize(148, 60);
        fox.Position = CRPosition(596, 130); 
        fox.Text = "FOX";
        fox.SetTextPosition(51, 12);
        fox.Tag = 213;
        fox.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		
		static CRButton h2(templateButton);
		h2.Size = CRSize(148, 60);
        h2.Position = CRPosition(596, 230); 
        h2.Text = "H2";
        h2.SetTextPosition(58, 12);
        h2.Tag = 876;
        h2.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		
		static CRButton hbo(templateButton);
		hbo.Size = CRSize(148, 60);
        hbo.Position = CRPosition(596, 330); 
        hbo.Text = "HBO";
        hbo.SetTextPosition(51, 12);
        hbo.Tag = 228;
        hbo.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ChangeChannel);
		
		Buttons.Add(&ap);
		Buttons.Add(&disc);
		Buttons.Add(&as);
        Buttons.Add(&cspan);
		Buttons.Add(&cnn);
		Buttons.Add(&com);
		Buttons.Add(&fox);
		Buttons.Add(&h2);
		Buttons.Add(&hbo);
    }
};

/*-----------------------------------------------------------------------------------------------*/

class XfinityNavScreen : public CRScreen
{
public:
    void OnLeftThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_CABLE_EXIT);
                break; 
            case UP:
				ScreenController::ShowScreen(SCREEN_XFINITY_FAV);
                break; 
            case DOWN:
                break; 
            case LEFT:
				ScreenController::ShowScreen(SCREEN_ROKU_MAIN);
                break; 
            case RIGHT:
                ScreenController::ShowScreen(SCREEN_TV_MAIN);
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_CABLE_ENTER);
                break; 
            case UP:
                RemoteController::ExecCommand(RC_CABLE_UP);
                break; 
            case DOWN:
                RemoteController::ExecCommand(RC_CABLE_DOWN);
                break; 
            case LEFT:
                RemoteController::ExecCommand(RC_CABLE_LEFT);
                break; 
            case RIGHT:
                RemoteController::ExecCommand(RC_CABLE_RIGHT);
                break; 
        }
    }
    
    virtual void Draw()
    {
        DrawScreenTitle(m_tft, "Xfinity::navigation");
		UpdateMap(m_tft, SCREEN_XFINITY_NAV);	

		m_tft->drawRect(35, 100, 130, 326, CRColor::Gray);
		m_tft->textMode();
        m_tft->textSetCursor(69, 105);
        m_tft->textEnlarge(1);
		m_tft->textColor(CRColor::Gray, CRColor::Black);
        m_tft->textWrite("Chan");
        m_tft->graphicsMode();
		
        CRScreen::Draw();
    }
    
public:
    XfinityNavScreen(TFT_TYPE_PTR tft) : CRScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<XfinityNavScreen, CRThumbstick*>(this, &XfinityNavScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<XfinityNavScreen, CRThumbstick*>(this, &XfinityNavScreen::OnRightThumbstickStateChanged);
        
		CRButton templateButton;
        templateButton.BorderColor = CRColor::Gray;
        templateButton.FillColor = CRColor::Blue;
        templateButton.PressedBorderColor = CRColor::Gray;
        templateButton.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateButton.BorderThickness = 5;
		
		CRTriangleButton templateTriangleButtonUp;
		templateTriangleButtonUp.Size = CRSize(100, 100);
        templateTriangleButtonUp.BorderColor = CRColor::Gray;
        templateTriangleButtonUp.FillColor = CRColor::Blue;
        templateTriangleButtonUp.PressedBorderColor = CRColor::Gray;
        templateTriangleButtonUp.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateTriangleButtonUp.BorderThickness = 5;
		templateTriangleButtonUp.Triangle.SetVertices(0, 50, 50, 50, 25, 0);
		templateTriangleButtonUp.Triangle.Position = CRPosition(25, 24);
		
		CRTriangleButton templateTriangleButtonDown(templateTriangleButtonUp);
		templateTriangleButtonDown.Triangle.SetVertices(0, 0, 25, 50, 50, 0);
		
        static CRButton seven(templateButton);
		seven.Size = CRSize(60, 60);
        seven.Position = CRPosition(504, 100); 
        seven.Text = "7";
        seven.SetTextPosition(22, 13);
        seven.Tag = RC_CABLE_7;
        seven.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
        static CRButton eight(templateButton);
		eight.Size = CRSize(60, 60);
        eight.Position = CRPosition(594, 100); 
        eight.Text = "8";
        eight.SetTextPosition(22, 13);
        eight.Tag = RC_CABLE_8;
        eight.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton nine(templateButton);
		nine.Size = CRSize(60, 60);
        nine.Position = CRPosition(684, 100); 
        nine.Text = "9";
        nine.SetTextPosition(22, 13);
        nine.Tag = RC_CABLE_9;
        nine.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton four(templateButton);
		four.Size = CRSize(60, 60);
        four.Position = CRPosition(504, 190); 
        four.Text = "4";
        four.SetTextPosition(22, 13);
        four.Tag = RC_CABLE_4;
        four.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
        static CRButton five(templateButton);
		five.Size = CRSize(60, 60);
        five.Position = CRPosition(594, 190); 
        five.Text = "5";
        five.SetTextPosition(22, 13);
        five.Tag = RC_CABLE_5;
        five.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton six(templateButton);
		six.Size = CRSize(60, 60);
        six.Position = CRPosition(684, 190); 
        six.Text = "6";
        six.SetTextPosition(22, 13);
        six.Tag = RC_CABLE_6;
        six.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton one(templateButton);
		one.Size = CRSize(60, 60);
        one.Position = CRPosition(504, 280); 
        one.Text = "1";
        one.SetTextPosition(22, 13);
        one.Tag = RC_CABLE_1;
        one.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
        static CRButton two(templateButton);
		two.Size = CRSize(60, 60);
        two.Position = CRPosition(594, 280); 
        two.Text = "2";
        two.SetTextPosition(22, 13);
        two.Tag = RC_CABLE_2;
        two.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton three(templateButton);
		three.Size = CRSize(60, 60);
        three.Position = CRPosition(684, 280); 
        three.Text = "3";
        three.SetTextPosition(22, 13);
        three.Tag = RC_CABLE_3;
        three.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton zero(templateButton);
		zero.Size = CRSize(60, 60);
        zero.Position = CRPosition(594, 370); 
        zero.Text = "0";
        zero.SetTextPosition(22, 13);
        zero.Tag = RC_CABLE_0;
        zero.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton guide(templateButton);
		guide.Size = CRSize(148, 60);
        guide.Position = CRPosition(256, 190); 
        guide.Text = "Guide";
        guide.SetTextPosition(35, 12);
        guide.Tag = RC_CABLE_GUIDE;
        guide.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRButton last(templateButton);
		last.Size = CRSize(148, 60);
        last.Position = CRPosition(256, 280); 
        last.Text = "Last";
        last.SetTextPosition(43, 12);
        last.Tag = RC_CABLE_LAST;
        last.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRTriangleButton chanUp(templateTriangleButtonUp);
        chanUp.Position = CRPosition(50, 150); 
        chanUp.Tag = RC_CABLE_CHAN_UP;
        chanUp.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRTriangleButton chanDown(templateTriangleButtonDown);
        chanDown.Position = CRPosition(50, 280); 
        chanDown.Tag = RC_CABLE_CHAN_DOWN;
        chanDown.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

        Buttons.Add(&seven);
		Buttons.Add(&eight);
		Buttons.Add(&nine);
		Buttons.Add(&four);
		Buttons.Add(&five);
		Buttons.Add(&six);
		Buttons.Add(&one);
		Buttons.Add(&two);
		Buttons.Add(&three);
		Buttons.Add(&zero);
		Buttons.Add(&guide);
		Buttons.Add(&last);
		Buttons.Add(&chanUp);
		Buttons.Add(&chanDown);
    }
};

/*-----------------------------------------------------------------------------------------------*/

class TvMainScreen : public CRScreen
{
public:
    void OnLeftThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_TV_RETURN);
                break; 
            case UP:
                break; 
            case DOWN:
				ScreenController::ShowScreen(SCREEN_TV_ADV);
                break; 
            case LEFT:
				ScreenController::ShowScreen(SCREEN_XFINITY_MAIN);
                break; 
            case RIGHT:
                ScreenController::ShowScreen(SCREEN_LIGHTS_MAIN);
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_TV_ENTER);
                break; 
            case UP:
                RemoteController::ExecCommand(RC_TV_UP);
                break; 
            case DOWN:
                RemoteController::ExecCommand(RC_TV_DOWN);
                break; 
            case LEFT:
                RemoteController::ExecCommand(RC_TV_LEFT);
                break; 
            case RIGHT:
                RemoteController::ExecCommand(RC_TV_RIGHT);
                break; 
        }
    }
    
    virtual void Draw()
    {
        DrawScreenTitle(m_tft, "TV::main");
		UpdateMap(m_tft, SCREEN_TV_MAIN);
		
		m_tft->drawRect(630, 100, 130, 326, CRColor::Gray);
		m_tft->textMode();
        m_tft->textSetCursor(670, 105);
        m_tft->textEnlarge(1);
		m_tft->textColor(CRColor::Gray, CRColor::Black);
        m_tft->textWrite("Vol");
        m_tft->graphicsMode();
		
        CRScreen::Draw();
    }
    
public:
    TvMainScreen(TFT_TYPE_PTR tft) : CRScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<TvMainScreen, CRThumbstick*>(this, &TvMainScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<TvMainScreen, CRThumbstick*>(this, &TvMainScreen::OnRightThumbstickStateChanged);
        
        CRButton templateButton;
        templateButton.BorderColor = CRColor::Gray;
        templateButton.FillColor = CRColor::Blue;
        templateButton.PressedBorderColor = CRColor::Gray;
        templateButton.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateButton.BorderThickness = 5;
        
		CRTriangleButton templateTriangleButtonUp;
		templateTriangleButtonUp.Size = CRSize(100, 100);
        templateTriangleButtonUp.BorderColor = CRColor::Gray;
        templateTriangleButtonUp.FillColor = CRColor::Blue;
        templateTriangleButtonUp.PressedBorderColor = CRColor::Gray;
        templateTriangleButtonUp.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateTriangleButtonUp.BorderThickness = 5;
		templateTriangleButtonUp.Triangle.SetVertices(0, 50, 50, 50, 25, 0);
		templateTriangleButtonUp.Triangle.Position = CRPosition(25, 24);
		
		CRTriangleButton templateTriangleButtonDown(templateTriangleButtonUp);
		templateTriangleButtonDown.Triangle.SetVertices(0, 0, 25, 50, 50, 0);
		
		static CRButton tvPower(templateButton);
		tvPower.Size = CRSize(148, 60);
        tvPower.Position = CRPosition(50, 130); 
        tvPower.Text = "TV Power";
        tvPower.SetTextPosition(10, 12);
        tvPower.Tag = RC_TV_POWER;
        tvPower.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRButton source(templateButton);
		source.Size = CRSize(148, 60);
        source.Position = CRPosition(50, 220); 
        source.Text = "Source";
        source.SetTextPosition(27, 12);
        source.Tag = RC_TV_SOURCE;
        source.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRButton mute(templateButton);
		mute.Size = CRSize(148, 60);
        mute.Position = CRPosition(50, 310); 
        mute.Text = "Mute";
        mute.SetTextPosition(43, 12);
        mute.Tag = RC_TV_MUTE;
        mute.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRTriangleButton volUp(templateTriangleButtonUp);
        volUp.Position = CRPosition(645, 150); 
        volUp.Tag = RC_TV_VOL_UP;
        volUp.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
		static CRTriangleButton volDown(templateTriangleButtonDown);
        volDown.Position = CRPosition(645, 280); 
        volDown.Tag = RC_TV_VOL_DOWN;
        volDown.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		
		Buttons.Add(&tvPower);
		Buttons.Add(&source);
		Buttons.Add(&mute);
		Buttons.Add(&volUp);
		Buttons.Add(&volDown);
    }
};

/*-----------------------------------------------------------------------------------------------*/

class TvAdvScreen : public CRScreen
{
public:
    void OnLeftThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_TV_RETURN);
                break; 
            case UP:
				ScreenController::ShowScreen(SCREEN_TV_MAIN);
                break; 
            case DOWN:
                break; 
            case LEFT:
				ScreenController::ShowScreen(SCREEN_XFINITY_MAIN);
                break; 
            case RIGHT:
                ScreenController::ShowScreen(SCREEN_LIGHTS_MAIN);
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
                RemoteController::ExecCommand(RC_TV_ENTER);
                break; 
            case UP:
                RemoteController::ExecCommand(RC_TV_UP);
                break; 
            case DOWN:
                RemoteController::ExecCommand(RC_TV_DOWN);
                break; 
            case LEFT:
                RemoteController::ExecCommand(RC_TV_LEFT);
                break; 
            case RIGHT:
                RemoteController::ExecCommand(RC_TV_RIGHT);
                break; 
        }
    }
    
    virtual void Draw()
    {
        DrawScreenTitle(m_tft, "TV::advanced");
		UpdateMap(m_tft, SCREEN_TV_ADV);
        CRScreen::Draw();
    }
    
public:
    TvAdvScreen(TFT_TYPE_PTR tft) : CRScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<TvAdvScreen, CRThumbstick*>(this, &TvAdvScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<TvAdvScreen, CRThumbstick*>(this, &TvAdvScreen::OnRightThumbstickStateChanged);
        
        CRButton templateButton;
        templateButton.BorderColor = CRColor::Gray;
        templateButton.FillColor = CRColor::Blue;
        templateButton.PressedBorderColor = CRColor::Gray;
        templateButton.PressedFillColor = CRColor::FromRGB(31, 37, 118);
        templateButton.BorderThickness = 5;
        
        static CRButton menu(templateButton);
		menu.Size = CRSize(148, 60);
        menu.Position = CRPosition(190, 130); 
        menu.Text = "Menu";
        menu.SetTextPosition(42, 12);
        menu.Tag = RC_TV_MENU;
        menu.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRButton tools(templateButton);
		tools.Size = CRSize(148, 60);
        tools.Position = CRPosition(463, 130); 
        tools.Text = "Tools";
        tools.SetTextPosition(34, 12);
        tools.Tag = RC_TV_TOOLS;
        tools.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
						
		static CRButton exit(templateButton);
		exit.Size = CRSize(148, 60);
        exit.Position = CRPosition(190, 283); 
        exit.Text = "Exit";
        exit.SetTextPosition(44, 12);
        exit.Tag = RC_TV_EXIT;
        exit.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);

		static CRButton info(templateButton);
		info.Size = CRSize(148, 60);
        info.Position = CRPosition(463, 283); 
        info.Text = "Info";
        info.SetTextPosition(43, 12);
        info.Tag = RC_TV_INFO;
        info.Pressed += new EventHandler<StaticFunction, CRButton*>(OnButtonPressed_ExecCommand);
		
        Buttons.Add(&menu);
        Buttons.Add(&tools);
		Buttons.Add(&exit);
		Buttons.Add(&info);
    }
};

/*-----------------------------------------------------------------------------------------------*/

class LightsMainScreen : public CRScreen
{
public:
    void OnLeftThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
				RemoteController::ExecCommand(RC_LIGHT_ON);
                break; 
            case UP:
				RemoteController::ExecCommand(RC_LIGHT_BRIGHTER);
                break; 
            case DOWN:
				RemoteController::ExecCommand(RC_LIGHT_DIMMER);
                break; 
            case LEFT:
				ScreenController::ShowScreen(SCREEN_TV_MAIN);
                break; 
            case RIGHT:
                break; 
        }
    }
    
    void OnRightThumbstickStateChanged(CRThumbstick* stick)
    {
        switch (stick->GetState())
        {
            case PRESSED:
				RemoteController::ExecCommand(RC_LIGHT_OFF);
                break; 
            case UP:
				RemoteController::ExecCommand(RC_LIGHT_GREEN);
                break; 
            case DOWN:
                break; 
            case LEFT:
				RemoteController::ExecCommand(RC_LIGHT_RED);
                break; 
            case RIGHT:
				RemoteController::ExecCommand(RC_LIGHT_BLUE);
                break; 
        }
    }
    
    virtual void Draw()
    {
        DrawScreenTitle(m_tft, "Lights::main");
		UpdateMap(m_tft, SCREEN_LIGHTS_MAIN);
		
		m_tft->drawRect(30, 120, 320, 258, CRColor::Gray);
		m_tft->drawRect(450, 120, 320, 258, CRColor::Gray);
		m_tft->drawCircle(190, 255, 50, CRColor::Gray);
		m_tft->drawCircle(610, 255, 50, CRColor::Gray);
		
		m_tft->textMode();
		m_tft->textEnlarge(1);
		m_tft->textColor(CRColor::Gray, CRColor::Black);
		m_tft->textSetCursor(40, 103);
        m_tft->textWrite("Left Thumbstick");
		m_tft->textSetCursor(460, 103);
		m_tft->textWrite("Right Thumbstick");
		m_tft->textColor(CRColor::White, CRColor::Black);
		m_tft->textSetCursor(176, 237);
		m_tft->textWrite("On");
		m_tft->textSetCursor(589, 237);
		m_tft->textWrite("Off");
		m_tft->textSetCursor(128, 157);
		m_tft->textWrite("Brighter");
		m_tft->textSetCursor(144, 317);
		m_tft->textWrite("Dimmer");
		m_tft->textSetCursor(488, 237);
		m_tft->textWrite("Red");
		m_tft->textSetCursor(571, 157);
		m_tft->textWrite("Green");
		m_tft->textSetCursor(685, 237);
		m_tft->textWrite("Blue");
        m_tft->graphicsMode();
		
        CRScreen::Draw();
    }
    
public:
    LightsMainScreen(TFT_TYPE_PTR tft) : CRScreen(tft)
    {       
        LeftThumbstick.StateChanged += new EventHandler<LightsMainScreen, CRThumbstick*>(this, &LightsMainScreen::OnLeftThumbstickStateChanged);
        RightThumbstick.StateChanged += new EventHandler<LightsMainScreen, CRThumbstick*>(this, &LightsMainScreen::OnRightThumbstickStateChanged);
    }
};

/*** INITIALIZATION ******************************************************************************/

void InitializeScreens(TFT_TYPE_PTR tft)
{
	// Instantiate screens
	static RokuMainScreen rokuMainScreen(tft);
	static RokuFavScreen rokuFavScreen(tft);
	static XfinityMainScreen xfinityMainScreen(tft);
	static XfinityFavScreen xfinityFavScreen(tft);
	static XfinityNavScreen xfinityNavScreen(tft);
	static TvMainScreen tvMainScreen(tft);
	static TvAdvScreen tvAdvScreen(tft);
	static LightsMainScreen lightsMainScreen(tft);
    
    // Add them to screen controller
    ScreenController::AddScreen(&rokuMainScreen);
    ScreenController::AddScreen(&rokuFavScreen);
	ScreenController::AddScreen(&xfinityMainScreen);
	ScreenController::AddScreen(&xfinityFavScreen);
	ScreenController::AddScreen(&xfinityNavScreen);
	ScreenController::AddScreen(&tvMainScreen);
	ScreenController::AddScreen(&tvAdvScreen);
	ScreenController::AddScreen(&lightsMainScreen);
    
    // Show Roku main screen upon boot
    ScreenController::ShowScreen(&rokuMainScreen);
}

#endif