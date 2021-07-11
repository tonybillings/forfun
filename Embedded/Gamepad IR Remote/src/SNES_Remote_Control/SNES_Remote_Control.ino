//------------------------------------------------------------------------------------------------------
#include <avr/power.h>
#include <avr/sleep.h>
#include <IRremote.h>
//------------------------------------------------------------------------------------------------------
#define SELECT  0
#define START   1
#define LB      2
#define RB      3
#define UP      4
#define DOWN    5
#define LEFT    6
#define RIGHT   7
#define A       8
#define B       9
#define X       10
#define Y       11

#define LOOP_INTERVAL 50
#define CHECK_WAIT_PERIOD 4  

#define PRESSED(BUTTON) (BUTTON->IsPressed)
#define WAS_PRESSED(BUTTON) (BUTTON->LastPressedState && !BUTTON->IsPressed)
//------------------------------------------------------------------------------------------------------
typedef unsigned int uint;

typedef struct Button
{
  byte        Pin;
  bool        IsPressed;
  bool        LastPressedState;
  byte        PressedCount;
  byte        LastPressedCount;
  byte        RecCode[35];
  byte        RecCodeLength;
  uint        ProjCode;

  Button() : Pin(0), IsPressed(false), LastPressedState(false), PressedCount(0), LastPressedCount(0), RecCodeLength(0), ProjCode(0)
  {
    memset(RecCode, 0, 35);
  }
} Button;
//------------------------------------------------------------------------------------------------------
static Button Buttons[12];
static IRsend IR;
static bool InReceiverMode = true;
static bool RecInputToggle = false;
static bool ProjPowerToggle = false;

static byte RecInputGameCode[35] = { 90,90,  90,90,  90,90,  175,90, 90,90,  90,90,  90,44, 90,90,  90,90,  90,90,  90,90,  90,90, 90,175, 90,90,  90,90, 90,90, 90,90, 175 };
static byte RecInputMediaPlayerCode[23] = { 90,90,  175,90,  90,90,  90,175, 175,175,  90,90,  90,90,  90,90,   90,90,   90,90,   90,90,  90 };
static byte RecPowerToggleCode[23] = { 90,90,  90,90,   90,90,  175,90, 90,90,  90,90,  90,90,  90,90,  90,175,  90,90,  175,90, 90 };
static byte RecMenuCode[29] = { 175,90, 90,175,  175,90, 90,90,  90,90,  90,44, 90,175, 175,90,  90,175,  175,175, 90,90,  90,90, 90,90,  175,90, 90 };
static uint ProjOnCode = 0xCEA0;
static uint ProjPowerOffCode = 0xCE60;
static uint ProjMenuCode = 0xCE74;
//------------------------------------------------------------------------------------------------------
inline void  sendRaw(const byte* data, int length)
{
  uint signal[length];

  for (int i = 0; i < length; i++)
  {
    const byte value = data[i];
    if (value != 44)
      signal[i] = value * (uint)10;
    else
      signal[i] = value * (uint)100;
  }

  IR.sendRaw(signal, sizeof(signal) / sizeof(signal[0]), 38);
}

inline void sendJVC(int data)
{
  IR.sendJVC(data, 16, 0);
  delay(20);
  IR.sendJVC(data, 16, 1);
}

void checkInput()
{
  static uint checkCount;
  checkCount++;

  Button* lb = &Buttons[LB];
  Button* rb = &Buttons[RB];
  Button* select = &Buttons[SELECT];
  Button* start = &Buttons[START];
  Button* a = &Buttons[A];
  Button* b = &Buttons[B];
  Button* x = &Buttons[X];
  Button* y = &Buttons[Y];
  Button* up = &Buttons[UP];
  Button* down = &Buttons[DOWN];
  Button* left = &Buttons[LEFT];
  Button* right = &Buttons[RIGHT];

  for (int i = 0; i < 12; i++)
  {
    Button* b = &Buttons[i];
    b->IsPressed = digitalRead(b->Pin) == LOW;

    if (b->IsPressed)
      b->PressedCount = b->PressedCount + 1;
    else
      b->PressedCount = 0;
  }
  
  if (WAS_PRESSED(select))
  {
    if (select->LastPressedCount > 39)
    {
      RecInputToggle = !RecInputToggle;
      checkCount = 0;

      if (RecInputToggle)
        sendRaw(RecInputMediaPlayerCode, 23);
      else
        sendRaw(RecInputGameCode, 35);
    }
    else
    {
      InReceiverMode = !InReceiverMode;
    }
  }
  else if ((checkCount > CHECK_WAIT_PERIOD) && PRESSED(lb) && PRESSED(rb))
  {
    checkCount = 0;

    if (InReceiverMode)
      sendRaw(RecMenuCode, 29);
    else
      sendJVC(ProjMenuCode);
  }
  else if (InReceiverMode && PRESSED(lb))
  {
    lb->PressedCount = checkCount = 0;
    sendRaw(lb->RecCode, lb->RecCodeLength);
  }
  else if (InReceiverMode && PRESSED(rb))
  {
    rb->PressedCount = checkCount = 0;
    sendRaw(rb->RecCode, rb->RecCodeLength);
  }
  else if (!InReceiverMode && checkCount > CHECK_WAIT_PERIOD && PRESSED(lb))
  {
    lb->PressedCount = checkCount = 0;
    sendJVC(lb->ProjCode);
  }
  else if (!InReceiverMode && checkCount > CHECK_WAIT_PERIOD && PRESSED(rb))
  {
    rb->PressedCount = checkCount = 0;
    sendJVC(rb->ProjCode);
  }
  else if (checkCount > CHECK_WAIT_PERIOD)
  {
    if (PRESSED(a))
    {
      a->PressedCount = checkCount = 0;

      if (InReceiverMode)
        sendRaw(a->RecCode, a->RecCodeLength);
      else
        sendJVC(a->ProjCode);
    }
    else if (PRESSED(b))
    {
      b->PressedCount = checkCount = 0;

      if (InReceiverMode)
        sendRaw(b->RecCode, b->RecCodeLength);
      else
        sendJVC(b->ProjCode);
    }
    else if (PRESSED(x))
    {
      x->PressedCount = checkCount = 0;

      if (InReceiverMode) 
         IR.sendRC5x(16, 37, 29); 
      else
        sendJVC(x->ProjCode);
    }
    else if (PRESSED(y))
    {
      y->PressedCount = checkCount = 0;

      if (InReceiverMode)
         IR.sendRC5x(16, 37, 53);
      else
        sendJVC(y->ProjCode);
    }
    else if (PRESSED(up))
    {
      up->PressedCount = checkCount = 0;

      if (InReceiverMode)
        sendRaw(up->RecCode, up->RecCodeLength);
      else
        sendJVC(up->ProjCode);
    }
    else if (PRESSED(down))
    {
      down->PressedCount = checkCount = 0;

      if (InReceiverMode)
        sendRaw(down->RecCode, down->RecCodeLength);
      else
        sendJVC(down->ProjCode);
    }
    else if (PRESSED(left))
    {
      left->PressedCount = checkCount = 0;

      if (InReceiverMode)
        sendRaw(left->RecCode, left->RecCodeLength);
      else
        sendJVC(left->ProjCode);
    }
    else if (PRESSED(right))
    {
      right->PressedCount = checkCount = 0;

      if (InReceiverMode)
        sendRaw(right->RecCode, right->RecCodeLength);
      else
        sendJVC(right->ProjCode);
    }
    else if (WAS_PRESSED(start))
    {
      checkCount = 0;

      if (InReceiverMode)
      {
        sendRaw(RecPowerToggleCode, 23);
      }
      else
      {
        ProjPowerToggle = !ProjPowerToggle;

        if (ProjPowerToggle)
        { 
          sendJVC(0xCEA0);
        }
        else
        {
          sendJVC(0xCE60);
          delay(600);
          sendJVC(0xCE60);
        }
      }
    }
  }

  for (int i = 0; i < 12; i++)
  {
    Button* btn = &Buttons[i];
    btn->LastPressedState = btn->IsPressed;
    btn->LastPressedCount = btn->PressedCount;
  }
}

void idle()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  noInterrupts();
  sleep_enable();
  interrupts();
  sleep_cpu();
  sleep_disable();
}

void loop()
{
  static long lastCheckTime = 0;

  if ((millis() - lastCheckTime) > LOOP_INTERVAL)
  {
    lastCheckTime = millis();
    checkInput();
  }
  else
  {
    idle();
  }
}

void initButtons()
{
  Buttons[SELECT].Pin = 6;
  Buttons[START].Pin = 8;

  Buttons[LB].Pin = 2;
  byte lbCode[23] = { 90,90,  90,90,  90,90,  175,90, 90,90,  90,90,  90,90,  90,175,  175,90,  90,90,  90,175, 90 };
  memcpy(Buttons[LB].RecCode, lbCode, 23);
  Buttons[LB].RecCodeLength = 23;
  Buttons[LB].ProjCode = 0xCEAF;

  Buttons[RB].Pin = 9;
  byte rbCode[23] = { 90,90,  90,90,  90,90,  175,90, 90,90,  90,90,  90,90,  90,175,  175,90,  90,90,   90,90, 90 };
  memcpy(Buttons[RB].RecCode, rbCode, 23);
  Buttons[RB].RecCodeLength = 23;
  Buttons[RB].ProjCode = 0xCECE;

  Buttons[A].Pin = 14;
  byte aCode[21] = { 175,90, 90,175,  175,90, 90,90,  90,90,  90,90,  90,175, 175,175, 90,90,  90,90,  90 };
  memcpy(Buttons[A].RecCode, aCode, 21);
  Buttons[A].RecCodeLength = 21;
  Buttons[A].ProjCode = 0xCEF4;

  Buttons[B].Pin = 13;
  byte bCode[29] = { 175,90, 90,175,  175,90, 90,90,  90,90,  90,44, 90,175, 175,175, 90,90,  90,90,  175,90, 90,90, 90,175, 175,90, 90 };
  memcpy(Buttons[B].RecCode, bCode, 29);
  Buttons[B].RecCodeLength = 29;
  Buttons[B].ProjCode = 0xCEC0;

  Buttons[X].Pin = 15;
  byte xCode[11] = {90,90, 175,170, 180,90, 85,90, 85,90, 85};
  memcpy(Buttons[X].RecCode, xCode, 11);
  Buttons[X].RecCodeLength = 11;
  Buttons[X].ProjCode = 0xCE2F;

  Buttons[Y].Pin = 16;
  byte yCode[11] = { 95,80, 180,170, 180,85, 85,90, 85,90, 85 };
  memcpy(Buttons[Y].RecCode, yCode, 11);
  Buttons[Y].RecCodeLength = 11;
  Buttons[Y].ProjCode = 0xCE11;

  Buttons[UP].Pin = 7;
  byte upCode[21] = { 175,90, 90,175,  175,90, 90,90,  90,90,  90,90,  90,175, 175,90,  90,90,  90,90,  90 };
  memcpy(Buttons[UP].RecCode, upCode, 21);
  Buttons[UP].RecCodeLength = 21;
  Buttons[UP].ProjCode = 0xCE80;

  Buttons[DOWN].Pin = 19;
  byte downCode[21] = { 175,90, 90,175,  175,90, 90,90,  90,90,  90,90,  90,175, 175,90,  90,90,  90,175,  90 };
  memcpy(Buttons[DOWN].RecCode, downCode, 21);
  Buttons[DOWN].RecCodeLength = 21;
  Buttons[DOWN].ProjCode = 0xCE40;

  Buttons[LEFT].Pin = 4;
  byte leftCode[19] = { 175,90, 90,175,  175,90, 90,90,  90,90,  90,90,  90,175, 175,175, 175,175, 90 };
  memcpy(Buttons[LEFT].RecCode, leftCode, 19);
  Buttons[LEFT].RecCodeLength = 19;
  Buttons[LEFT].ProjCode = 0xCE6C;

  Buttons[RIGHT].Pin = 17;
  byte rightCode[19] = { 175,90, 90,175,  175,90, 90,90,  90,90,  90,90,  90,175, 175,175, 90,90,  175 };
  memcpy(Buttons[RIGHT].RecCode, rightCode, 19);
  Buttons[RIGHT].RecCodeLength = 19;
  Buttons[RIGHT].ProjCode = 0xCE2C;

  for (int i = 0; i < 12; i++)
    pinMode(Buttons[i].Pin, INPUT_PULLUP);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void initLowPowerMode()
{
  clock_prescale_set(clock_div_2);
  Serial.end();
  ADCSRA = 0;
}

void setup()
{
  initLowPowerMode();
  initButtons();
}
