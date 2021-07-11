/*******************************************************
*** INCLUDES *******************************************
*******************************************************/

#include <SPI.h>
#include <Wire.h>
#include <WiFiNINA.h>
#include "pt.h"
#include "RA8875.h"
#include "Adafruit_STMPE610.h"
#include "Adafruit_MCP9808.h"
#include "Event.h"
#include "UI.h"

/*******************************************************
*** DEFINES ********************************************
*******************************************************/

// DEVELOPER
#define DEBUG               false
#define PRINT(arg)          if (DEBUG) { Serial.print(arg); }
#define PRINTLN(arg)        if (DEBUG) { Serial.println(arg); }

// NETWORKING
#define SSID                "FreeThought"
#define PASSWORD            "AABBCCDDEEFF1029384756"
#define IP_ADDRESS          IPAddress(192, 168, 1, 64)
#define PORT                6163
#define SYNC                22
#define ETB                 23

// COMMANDS
#define CMD_HEAT_MODE       145
#define CMD_AC_MODE         146
#define CMD_INC_SET_TEMP    147
#define CMD_DEC_SET_TEMP    148
#define CMD_FAN_MODE        149
#define CMD_CTRL_SCREEN     150
#define CMD_LOCK_SCREEN     151
#define CMD_GET_STATE       152
#define CMD_SET_SCREEN_IMG  153

// VIDEO
#define RA8875_CS           12
#define RA8875_RESET        10
#define RA8875_INT          8
#define BLACK               0x0000
#define WHITE               0xFFFF
#define MAGENTA             0xF81F
#define TEAL                0x6EDF
#define ORANGE              0xFC84
#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       480
#define LS_REDRAW_INTERVAL  25000

// PROTOTHREADS
#define SEM_BLOCK           1      
#define SEM_UNBLOCK         0     
#define PT_STRUCT           struct pt * const pt

// TEMPERATURE
#define INITIAL_TEMP        70
#define MAX_TEMP            80
#define MIN_TEMP            65
#define DELTA_TEMP          15
#define MAX_TEMP_Y          113
#define MIN_TEMP_Y          318
#define DELTA_TEMP_Y        205
#define TEMP_OFFSET         (-6.0f)

// RELAYS
#define RELAY_FAN           5 // Relay #3
#define RELAY_AC            6 // Relay #2
#define RELAY_HEAT          7 // Relay #1

/*******************************************************
*** PROTOTHREADS SETUP *********************************
*******************************************************/

PT_THREAD(TouchThread(PT_STRUCT));
PT_THREAD(TempThread(PT_STRUCT));
PT_THREAD(UIThread(PT_STRUCT));
PT_THREAD(ControlThread(PT_STRUCT));

static struct pt touch_pt;
static struct pt temp_pt;
static struct pt ui_pt;
static struct pt ctrl_pt;

static byte touch_sem       = SEM_BLOCK;
static byte temp_sem        = SEM_BLOCK;
static byte ui_sem          = SEM_BLOCK;
static byte ctrl_sem        = SEM_BLOCK;

/*******************************************************
*** GLOBALS ********************************************
*******************************************************/

static RA8875               tft = RA8875(RA8875_CS, RA8875_RESET);
static Adafruit_STMPE610    touch = Adafruit_STMPE610();
static Adafruit_MCP9808     temp = Adafruit_MCP9808();
static WiFiServer           wifi(PORT);
static int                  wifiStatus = WL_IDLE_STATUS;

enum OperationMode          { Heat, AC };
enum FanMode                { Auto, On };
static OperationMode        opMode;
static FanMode              fanMode;
static bool                 isOpModeDisplayed;
static bool                 isFanModeDisplayed;
static uint32_t             timeOpModeDisplayed;     
static uint32_t             timeFanModeDisplayed;

enum HeatingState           { Off, Rising, Running, Falling };
static HeatingState         heatingState;
static uint32_t             timeHeatRisingStarted;
static uint32_t             timeHeatFallingStarted;

static byte                 setTemp;
static byte                 currentTemp;
static Bitmap*              setTempNum1;
static Bitmap*              setTempNum2;
static Bitmap*              currentTempNum1;
static Bitmap*              currentTempNum2;

static HeatButton           btnHeat;
static CoolButton           btnCool;
static TempUpButton         btnTempUp;
static FanButton            btnFan;
static TempDownButton       btnTempDown;
static LockScreenButton     btnLockScreen;

static ModeText             textMode;
static ACText               textAC;
static HeatText             textHeat;
static FanText              textFan;
static AutoText             textAuto;
static OnText               textOn;

static NumberZero           numberZero;
static NumberOne            numberOne;
static NumberTwo            numberTwo;
static NumberThree          numberThree;
static NumberFour           numberFour;
static NumberFive           numberFive;
static NumberSix            numberSix;
static NumberSeven          numberSeven;
static NumberEight          numberEight;
static NumberNine           numberNine;

static bool                 isScreenLocked;
static Leaf                 leaf;

/*******************************************************
*** PROTOTHREADS ***************************************
*******************************************************/

// TOUCH
void procTouch();
PT_THREAD(TouchThread(PT_STRUCT))
{
    PT_BEGIN(pt);
    
    while(1) 
    {
        PT_WAIT_WHILE(pt, touch_sem);
        touch_sem = SEM_BLOCK;
        procTouch();
    }
    
    PT_END(pt);
}

// TEMPERATURE
void procTemp();
PT_THREAD(TempThread(PT_STRUCT))
{
    PT_BEGIN(pt);
    
    while(1) 
    {
        PT_WAIT_WHILE(pt, temp_sem);
        temp_sem = SEM_BLOCK;
        procTemp();
    }
    
    PT_END(pt);
}

// UI
void updateUI();
PT_THREAD(UIThread(PT_STRUCT))
{
    PT_BEGIN(pt);
    
    while(1) 
    {
        PT_WAIT_WHILE(pt, ui_sem);
        ui_sem = SEM_BLOCK;
        updateUI();
    }
    
    PT_END(pt);
}

// Control
void controlThermostat();
PT_THREAD(ControlThread(PT_STRUCT))
{
    PT_BEGIN(pt);
    
    while(1) 
    {
        PT_WAIT_WHILE(pt, ctrl_sem);
        ctrl_sem = SEM_BLOCK;
        controlThermostat();
    }
    
    PT_END(pt);
}

/*******************************************************
*** FUNCTIONS ******************************************
*******************************************************/

void updateUI()
{
  if (isScreenLocked) return;
  
  uint32_t currentTime = millis();
  
  if (isOpModeDisplayed)
  {
    if (currentTime - timeOpModeDisplayed > 4500)
      hideOperationMode();
  }

  if (isFanModeDisplayed)
  {
    if (currentTime - timeFanModeDisplayed > 4500)
      hideFanMode();
  }
}

void drawTempUpButton()
{
  tft.fillRect(btnTempUp.PosX, btnTempUp.PosY, btnTempUp.Width, btnTempUp.Height, btnTempUp.Color);
  
  uint16_t x = 88;
  uint16_t y = 418 + 50;

  tft.drawFastHLine(x + 3, y - 1, 82, BLACK);
  tft.drawFastHLine(x + 2, y - 2, 84, BLACK);
  tft.drawFastHLine(x + 1, y - 3, 86, BLACK);
  tft.drawFastHLine(x + 0, y - 4, 88, BLACK);
  tft.drawFastHLine(x + 0, y - 5, 88, BLACK);
  tft.drawFastHLine(x + 0, y - 6, 88, BLACK);
  
  for (int i = 0; i < 6; i++)
  {
    int a = i * 8;
    int b = i * 8;
    int c = i * 16;
    tft.drawFastHLine(x + 2 + a, y - 9 - b, 86 - c, BLACK);
    tft.drawFastHLine(x + 3 + a, y - 10 - b, 84 - c, BLACK);
    tft.drawFastHLine(x + 4 + a, y - 11 - b, 82 - c, BLACK);

    if (i != 5)
    {
      tft.drawFastHLine(x + 5 + a, y - 12 - b, 80 - c, BLACK);
      tft.drawFastHLine(x + 6 + a, y - 13 - b, 78 - c, BLACK);
      tft.drawFastHLine(x + 7 + a, y - 14 - b, 76 - c, BLACK);
    }
  }
}

void drawTempDownButton()
{
  tft.fillRect(btnTempDown.PosX, btnTempDown.PosY, btnTempDown.Width, btnTempDown.Height, btnTempDown.Color);
  
  uint16_t x = 623;
  uint16_t y = 418;

  tft.drawFastHLine(x + 3, y + 1, 82, BLACK);
  tft.drawFastHLine(x + 2, y + 2, 84, BLACK);
  tft.drawFastHLine(x + 1, y + 3, 86, BLACK);
  tft.drawFastHLine(x + 0, y + 4, 88, BLACK);
  tft.drawFastHLine(x + 0, y + 5, 88, BLACK);
  tft.drawFastHLine(x + 0, y + 6, 88, BLACK);
  
  for (int i = 0; i < 6; i++)
  {
    int a = i * 8;
    int b = i * 8;
    int c = i * 16;
    tft.drawFastHLine(x + 2 + a, y + 9 + b, 86 - c, BLACK);
    tft.drawFastHLine(x + 3 + a, y + 10 + b, 84 - c, BLACK);
    tft.drawFastHLine(x + 4 + a, y + 11 + b, 82 - c, BLACK);

    if (i != 5)
    {
      tft.drawFastHLine(x + 5 + a, y + 12 + b, 80 - c, BLACK);
      tft.drawFastHLine(x + 6 + a, y + 13 + b, 78 - c, BLACK);
      tft.drawFastHLine(x + 7 + a, y + 14 + b, 76 - c, BLACK);
    }
  }
}

uint16_t rgb888to565(byte red, byte green, byte blue)
{
    uint16_t b = (blue >> 3) & 0x1f;
    uint16_t g = ((green >> 2) & 0x3f) << 5;
    uint16_t r = ((red >> 3) & 0x1f) << 11;
    return (r | g | b);
}

Bitmap* getNumber(byte value)
{
  switch (value)
  {
    case 0:
      return &numberZero;
    case 1:
      return &numberOne;
    case 2:
      return &numberTwo;
    case 3:
      return &numberThree;
    case 4:
      return &numberFour;
    case 5:
      return &numberFive;
    case 6:
      return &numberSix;
    case 7:
      return &numberSeven;
    case 8:
      return &numberEight;
    case 9:
      return &numberNine;
    default:
      return nullptr;
  }
}

void drawTempGauge()
{
  if (isScreenLocked) return;
  
  uint16_t r1 = 254;
  uint16_t g1 = 115;
  uint16_t b1 = 116;
  uint16_t r2 = 118;
  uint16_t g2 = 115;
  uint16_t b2 = 252;
  
  for (int i = 0; i < 256; i+=2)
  {
    tft.drawFastHLine(385, 112 + i + 0, 30, rgb888to565(r1, g1, b1));
    tft.drawFastHLine(385, 112 + i + 1, 30, rgb888to565(r1, g1, b1));

    tft.fillRect(385, 112 + i + 0, 3, 2, rgb888to565(r2, g2, b2));
    tft.fillRect(412, 112 + i + 0, 3, 2, rgb888to565(r2, g2, b2));

    r1--;
    b1 += (i > 160) ? 2 : 1;
    b1 = (b1 > 255) ? 255 : b1;
    
    r2++;
    b2 -= (i < 96) ? 2 : 1;
    b2 = (b2 > 5000) ? 0 : b2;
  }

  tft.drawFastHLine(385, 112, 30, rgb888to565(118, 115, 252));
  tft.drawFastHLine(385, 113, 30, rgb888to565(118, 115, 252));
  tft.drawFastHLine(385, 114, 30, rgb888to565(118, 115, 252));
  tft.drawFastHLine(385, 366, 30, rgb888to565(254, 115, 116));
  tft.drawFastHLine(385, 367, 30, rgb888to565(254, 115, 116));
  tft.drawFastHLine(385, 368, 30, rgb888to565(254, 115, 116));
}

void drawSetTemp()
{
  if (isScreenLocked) return;
  
  uint16_t color = BLACK;
  if (opMode == Heat)
    color = ORANGE;
  else if (opMode == AC)
    color = TEAL;
    
  tft.fillRect(359, MAX_TEMP_Y - 1, 25, 258, BLACK);

  if (setTemp < 65)
  {
    tft.fillTriangle(359, MIN_TEMP_Y, 383, MIN_TEMP_Y + 24, 359, MIN_TEMP_Y + 48, color);
    tft.fillTriangle(359, MIN_TEMP_Y, 383, MIN_TEMP_Y + 24, 359, MIN_TEMP_Y + 48, color); 
  }
  else if (setTemp > 80)
  {
    tft.fillTriangle(359, MAX_TEMP_Y, 383, MAX_TEMP_Y + 24, 359, MAX_TEMP_Y + 48, color);
    tft.fillTriangle(359, MAX_TEMP_Y, 383, MAX_TEMP_Y + 24, 359, MAX_TEMP_Y + 48, color);
  }
  else
  {
    uint16_t y = MIN_TEMP_Y - ((((setTemp - MIN_TEMP) / (float)DELTA_TEMP) * (float)DELTA_TEMP_Y) + 0.5f);
    y = y > MIN_TEMP_Y ? MIN_TEMP_Y : y;
    y = y < MAX_TEMP_Y ? MAX_TEMP_Y : y;
    tft.fillTriangle(359, y, 383, y + 24, 359, y + 48, color);
    tft.fillTriangle(359, y, 383, y + 24, 359, y + 48, color);
  }

  tft.fillRect(20,195,290, 81, BLACK);

  byte num1 = setTemp / 10;
  byte num2 = setTemp % 10;
  setTempNum1 = getNumber(num1);
  setTempNum2 = getNumber(num2);
  setTempNum1->TransparencyColor = 63519;
  setTempNum2->TransparencyColor = 63519;
  byte x1 = ((275 - setTempNum2->Width) - 20) - setTempNum1->Width;
  byte x2 = 275 - setTempNum2->Width;

  Bitmap::UseOverrideColor = true;
  Bitmap::OverrideColor = color;
  Bitmap::draw(setTempNum1, x1, 198); 
  Bitmap::draw(setTempNum2, x2, 198);
  Bitmap::UseOverrideColor = false;
}

void drawCurrentTemp()
{   
  if (isScreenLocked) return;
  tft.fillRect(415, MAX_TEMP_Y - 1, 26, 258, BLACK);

  if (currentTemp < 65)
  {
    tft.fillTriangle(440, MIN_TEMP_Y, 416, MIN_TEMP_Y + 24, 440, MIN_TEMP_Y + 48, MAGENTA);
    tft.fillTriangle(440, MIN_TEMP_Y, 416, MIN_TEMP_Y + 24, 440, MIN_TEMP_Y + 48, MAGENTA); 
  }
  else if (currentTemp > 80)
  {
    tft.fillTriangle(440, MAX_TEMP_Y, 416, MAX_TEMP_Y + 24, 440, MAX_TEMP_Y + 48, MAGENTA);
    tft.fillTriangle(440, MAX_TEMP_Y, 416, MAX_TEMP_Y + 24, 440, MAX_TEMP_Y + 48, MAGENTA);
  }
  else
  {
    uint16_t y = MIN_TEMP_Y - ((((currentTemp - MIN_TEMP) / (float)DELTA_TEMP) * (float)DELTA_TEMP_Y) + 0.5f);
    y = y > MIN_TEMP_Y ? MIN_TEMP_Y : y;
    y = y < MAX_TEMP_Y ? MAX_TEMP_Y : y;
    tft.fillTriangle(440, y, 416, y + 24, 440, y + 48, MAGENTA);
    tft.fillTriangle(440, y, 416, y + 24, 440, y + 48, MAGENTA);
  }
  
  tft.fillRect(490,195,290, 81, BLACK);

  byte num1 = currentTemp / 10;
  byte num2 = currentTemp % 10;
  currentTempNum1 = getNumber(num1);
  currentTempNum2 = getNumber(num2);
  currentTempNum1->TransparencyColor = 65535;
  currentTempNum2->TransparencyColor = 65535;

  Bitmap::draw(currentTempNum1, 520, 198, true); 
  Bitmap::draw(currentTempNum2, 520 + currentTempNum1->Width + 20, 198, true);
}

void increaseSetTemp()
{
  if (setTemp != 80)
  {
    setTemp++;
    drawSetTemp();
  }
}

void decreaseSetTemp()
{
  if (setTemp != 65)
  {
    setTemp--;
    drawSetTemp();
  }
}

void enableHeatMode()
{
  hideOperationMode();
  opMode = Heat;
  showOperationMode();
  drawSetTemp();
}

void enableACMode()
{
  hideOperationMode();
  opMode = AC;
  showOperationMode();
  drawSetTemp();
}

void showOperationMode()
{
  if (isScreenLocked) return;
  
  switch (opMode)
  {
    case Heat:
      Bitmap::draw(&textHeat, 239, 85);
      Bitmap::draw(&textMode, 413, 85);
      break;
    case AC:
      Bitmap::draw(&textAC, 261, 85);
      Bitmap::draw(&textMode, 381, 85);
      break;
  }

  timeOpModeDisplayed = millis();
  isOpModeDisplayed = true;
}

void hideOperationMode()
{
  if (isScreenLocked) return;
  
  switch (opMode)
  {
    case Heat:
      Bitmap::clear(&textHeat);
      Bitmap::clear(&textMode);
      break;
    case AC:
      Bitmap::clear(&textAC);
      Bitmap::clear(&textMode);
      break;
  }

  isOpModeDisplayed = false;
}

void toggleFanMode()
{
  hideFanMode();
  
  switch (fanMode)
  {
    case Auto:
      fanMode = On;
      break;
    case On:
      fanMode = Auto;
      break;
  }

  showFanMode();
}

void showFanMode()
{
  if (isScreenLocked) return;
  
  switch (fanMode)
  {
    case Auto:
      Bitmap::draw(&textFan, 260, 378);
      Bitmap::draw(&textAuto, 398, 378);
      break;
    case On:
      Bitmap::draw(&textFan, 293, 378);
      Bitmap::draw(&textOn, 431, 378);
      break;
  }

  timeFanModeDisplayed = millis();
  isFanModeDisplayed = true;
}

void hideFanMode()
{
  if (isScreenLocked) return;
  
  switch (fanMode)
  {
    case Auto:
      Bitmap::clear(&textFan);
      Bitmap::clear(&textAuto);
      break;
    case On:
      Bitmap::clear(&textFan);
      Bitmap::clear(&textOn);
      break;
  }

  isFanModeDisplayed = false;
}

uint32_t getState()
{
  return ((uint32_t)opMode << 24) | ((uint32_t)fanMode << 16) | ((uint32_t)setTemp << 8) | (uint32_t)currentTemp;
}

bool procCommand(byte code)
{
  switch(code)
  {
    case CMD_HEAT_MODE:
      enableHeatMode();
       return true;
    case CMD_AC_MODE:
      enableACMode();
       return true;
    case CMD_INC_SET_TEMP:
      increaseSetTemp();
       return true;
    case CMD_DEC_SET_TEMP:
      decreaseSetTemp();
       return true;
    case CMD_FAN_MODE:
      toggleFanMode();
       return true;
    case CMD_CTRL_SCREEN:
      showControlScreen();
      return true;
    case CMD_LOCK_SCREEN:
      showLockScreen();
      return true;
    case CMD_GET_STATE:
      return true;
    default:
      return false;
  }
}

bool procData(byte data)
{
  static int packetState = 0;
  static byte commandCode = 0;

  if (data == SYNC)
  {
    if (packetState == 0)
      packetState = 1;
    else if (packetState == 1)
      packetState = 2;
    else
      packetState = 0;
  }
  else if (data == ETB)
  {
    if (packetState == 3)
    {
      packetState = 0;
      return procCommand(commandCode);
    }

    packetState = 0;
  }
  else
  {
    if (packetState == 2)
    {
      packetState = 3;
      commandCode = data;
    }
    else
    {
      packetState = 0;
    }
  }

  return false;
}

void checkWifi()
{
  WiFiClient client = wifi.available();
  
  if (client) 
  {
    IPAddress remoteIP = client.remoteIP();

    if (remoteIP != IPAddress(192, 168, 1, 37) && remoteIP != IPAddress(192, 168, 1, 18) && remoteIP != IPAddress(192, 168, 1, 38))
    {
      client.flush();
      client.stop();
      return;
    }
   
    while (client.connected()) 
    {
      while (client.available()) 
      {
        char data = client.read();
        
        if (procData(data))
        {
          uint32_t state = getState();
          byte response[] = {state & 0x000000FF, (state & 0x0000FF00) >> 8, (state & 0x00FF0000) >> 16, (state & 0xFF000000) >> 24 };
          client.write(response, 4);
          client.stop();
          return;
        }
      }

      client.stop();
      return;
    }
  }
}

void onScreenTouched(uint16_t x, uint16_t y)
{
  btnLockScreen.onScreenTouched(x, y);
  if (isScreenLocked) return;
  
  if (y > 240)
  {
    Button::onScreenTouched(btnTempUp, x, y);
    Button::onScreenTouched(btnFan, x, y);
    Button::onScreenTouched(btnTempDown, x, y);
  }
  else
  {
    Button::onScreenTouched(btnHeat, x, y);
    Button::onScreenTouched(btnCool, x, y);
  }
}

void procTouch()
{
  static uint32_t lastTouchTime = 0;
  static byte touchCount = 0;
  
  uint16_t x, y;
  uint8_t z;

  if (touch.touched()) 
  {
    while (!touch.bufferEmpty()) 
      touch.readData(&x, &y, &z);

    if (millis() - lastTouchTime > 40)
      touchCount = 1;
    else
      touchCount++;

    if (touchCount > 9)
    {
      x = constrain(x, 350, 3900);
      y = constrain(y, 400, 3800);
      x = SCREEN_WIDTH - map(x, 350, 3900, 0, SCREEN_WIDTH);
      y = SCREEN_HEIGHT - map(y, 400, 3800, 0, SCREEN_HEIGHT);

      touchCount = 0;
      onScreenTouched(x, y);
    }
    
    lastTouchTime = millis();
    touch.writeRegister8(STMPE_INT_STA, 0xFF);
  }
}

void procTemp()
{
  static float tempReadings[4] = { INITIAL_TEMP, INITIAL_TEMP, INITIAL_TEMP, INITIAL_TEMP };
  static byte readingIndex = 0;
  
  float reading = temp.readTempF() + TEMP_OFFSET;
  tempReadings[readingIndex] = reading;

  if (readingIndex == 3)
  {
    byte oldTemp = currentTemp;
    currentTemp = ((tempReadings[0] + tempReadings[1] + tempReadings[2] + tempReadings[3]) / 4.0f) + 0.5f;

    if (!isScreenLocked && (currentTemp != oldTemp))
      drawCurrentTemp();
  }

  readingIndex = (readingIndex + 1) % 4;
}

void initSerial()
{
#if DEBUG == true
  Serial.begin(9600);
  while (!Serial);
#endif  
}

void initVideo()
{
  tft.begin(RA8875_800x480);
  
  if (tft.errorCode() != 0) 
  {
    while (1);
  }
  else
  {
    tft.begin(RA8875_800x480);
    tft.displayOn(true);
    tft.GPIOX(true);
    tft.setRotation(2);
    tft.fillWindow(BLACK);
  }
}

void initTouch()
{
  if (!touch.begin()) 
  {
    while(1);
  }
}

void initTemp()
{ 
  if (!temp.begin()) 
  {
    while(1);
  }
  else
  {
    temp.setResolution(3);
    temp.wake();
  }

  setTemp = 65;
  currentTemp = INITIAL_TEMP;
}

void initPt()
{
  touch_sem = SEM_UNBLOCK;
  temp_sem = SEM_UNBLOCK;
}

void initButtons()
{
  btnHeat.clicked += new EventHandler<StaticFunction>(enableHeatMode);
  btnCool.clicked += new EventHandler<StaticFunction>(enableACMode);
  btnTempUp.clicked += new EventHandler<StaticFunction>(increaseSetTemp);
  btnFan.clicked += new EventHandler<StaticFunction>(toggleFanMode);
  btnTempDown.clicked += new EventHandler<StaticFunction>(decreaseSetTemp);
  btnLockScreen.rightSwipe += new EventHandler<StaticFunction>(showControlScreen);
  btnLockScreen.leftSwipe += new EventHandler<StaticFunction>(showLockScreen);
}

void initWifi()
{
  if (WiFi.status() == WL_NO_MODULE) 
  {
    while(1);
  }
  else
  {
    WiFi.config(IP_ADDRESS, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
    int tryCount = 0;
    
    while (wifiStatus != WL_CONNECTED) 
    {
      wifiStatus = WiFi.begin(SSID, PASSWORD);
      delay(10000);
      
      if (++tryCount > 2)
      {
        break;
      }
    }
    
    wifi.begin();
  }
}

void initModes()
{
  opMode = Heat;
  fanMode = Auto;
  heatingState = Off;

  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_HEAT, OUTPUT);
  pinMode(RELAY_AC, OUTPUT);
}

void drawUI()
{
  tft.fillWindow(BLACK);
  
  Bitmap::TFT = &tft;
  Button::TFT = &tft;  

  Button::draw(btnHeat);
  Button::draw(btnCool);;
  drawTempUpButton();
  Button::draw(btnFan);
  drawTempDownButton();
  drawTempGauge();
  drawSetTemp();
  drawCurrentTemp();

  isScreenLocked = false;
}

void drawLockScreen(bool forceNow = false)
{
  static uint32_t lastDrawCheck = 0;
  static uint32_t lastColorChangeCheck = 0;
  static bool inverseColorsFlag = true;
  static uint16_t colors[] = { 1792, 33823, 32833 };
  static byte colorsIndex = 0;
  static byte drawCount = 0;
  
  if (forceNow || (millis() - lastDrawCheck) > LS_REDRAW_INTERVAL) 
  {
    inverseColorsFlag = !inverseColorsFlag;

    if (inverseColorsFlag)
      leaf.draw(tft, 0, 0, inverseColorsFlag);
    else
      leaf.reverseDraw(tft, 0, 0, inverseColorsFlag);

    drawCount++;
    lastDrawCheck = millis();
  }

  if (drawCount > 1) 
  {
    drawCount = 0;
    leaf.setColor(colors[++colorsIndex % 3]);
    lastColorChangeCheck = millis();
  }
}

void showLockScreen()
{
  isScreenLocked = true;
  drawLockScreen(true);
}

void showControlScreen()
{
  isScreenLocked = false;
  drawUI();
}

void controlThermostat()
{
  switch (opMode)
  {
    case Heat:
      {
        digitalWrite(RELAY_AC, LOW);
        
        if (currentTemp < setTemp)
        {
          switch (heatingState)
          {
            case Running:
              {
                digitalWrite(RELAY_HEAT, HIGH);
                digitalWrite(RELAY_FAN, HIGH);
              }
              break;
            case Off:
              {
                heatingState = Rising;
                digitalWrite(RELAY_FAN, LOW);
                delayMicroseconds(5000);
                digitalWrite(RELAY_HEAT, HIGH);
                timeHeatRisingStarted = millis();
              }
              break;
            case Rising:
              {
                if (millis() - timeHeatRisingStarted > 120000)
                {
                  heatingState = Running;
                  digitalWrite(RELAY_FAN, HIGH);
                }
              }
              break;
            case Falling:
              {
                if (millis() - timeHeatFallingStarted > 240000)
                  heatingState = Off;
              }
              break;
          }
        }
        else
        {
          switch (heatingState)
          {
            case Off:
              {
                digitalWrite(RELAY_HEAT, LOW);
                
                switch (fanMode)
                {
                  case Auto:
                  {
                    digitalWrite(RELAY_FAN, LOW);
                  }
                  break;
                  case On:
                  {
                    digitalWrite(RELAY_FAN, HIGH);
                  }
                  break;
                }
              }
              break;
            case Running:
              {
                heatingState = Falling;
                digitalWrite(RELAY_HEAT, LOW);
                timeHeatFallingStarted = millis();
              }
              break;
            case Falling:
              {
                if (millis() - timeHeatFallingStarted > 240000)
                  heatingState = Off;
              }
              break;
            case Rising:
              {
                if (millis() - timeHeatRisingStarted > 120000)
                {
                  heatingState = Running;
                  digitalWrite(RELAY_FAN, HIGH);
                }
              }
              break;
          }
        }
      }
      break;
    case AC:
      {
        digitalWrite(RELAY_HEAT, LOW);
        heatingState = Off;
        
        if (currentTemp > setTemp)
        {
          digitalWrite(RELAY_AC, HIGH);
          digitalWrite(RELAY_FAN, HIGH);
        }
        else
        {
          digitalWrite(RELAY_AC, LOW);
          
          switch (fanMode)
          {
            case Auto:
            {
              digitalWrite(RELAY_FAN, LOW);
            }
            break;
            case On:
            {
              digitalWrite(RELAY_FAN, HIGH);
            }
            break;
          }
        }
      }
      break;
  }
}

/*******************************************************
*** LOOP & SETUP ***************************************
*******************************************************/

void loop()
{ 
  static uint32_t lastTouchCheck = 0;
  static uint32_t lastTempCheck = 0;
  static uint32_t lastUICheck = 0;
  static uint32_t lastControlCheck = 0;

  uint32_t checkTime = millis();
  
  if (checkTime - lastTouchCheck > 10) // 10 milliseconds
  {
    lastTouchCheck = checkTime;
    touch_sem = SEM_UNBLOCK; 
  }
  
  if (checkTime - lastUICheck > 2517) // ~2.5 seconds 
  {
    lastUICheck = checkTime;
    ui_sem = SEM_UNBLOCK;
  }
  
  if (checkTime - lastTempCheck > 15013) // ~15 seconds
  {
    lastTempCheck = checkTime;
    temp_sem = SEM_UNBLOCK;
  }
  
  if (checkTime - lastControlCheck > 20403) // ~20 seconds 
  {
    lastControlCheck = checkTime;
    ctrl_sem = SEM_UNBLOCK;
  }
  
  PT_SCHEDULE(TouchThread(&touch_pt));
  PT_SCHEDULE(UIThread(&ui_pt));
  PT_SCHEDULE(TempThread(&temp_pt));
  PT_SCHEDULE(ControlThread(&ctrl_pt));

  checkWifi();
  
  if (isScreenLocked)
  {
    drawLockScreen();
  }
}

void setup()
{
  initSerial();
  initVideo();
  initTouch();
  initTemp();
  initPt();
  initButtons();
  initWifi();
  initModes();
  drawUI();
}
