/*** INCLUDES **********************************************************************************************************************/

#include <LiquidCrystalFast.h>
#include <Time.h>
#include <TimerThree.h>
#include "Messaging.h"
#include "pt.h"

/*** DEFINES ***********************************************************************************************************************/

#define SEM_BLOCK 1			
#define SEM_UNBLOCK 0			
#define PT_STRUCT struct pt * const pt

#define PIN_STAGE 0
#define PIN_VIEW 1
#define PIN_RCS 2
#define PIN_RCS_LED 22
#define PIN_SAS 3
#define PIN_SAS_LED 23
#define PIN_GRAB 13
#define PIN_RJOY_MODE 19
#define PIN_LJOY_X 0
#define PIN_LJOY_Y 1
#define PIN_LJOY_Z 2
#define PIN_RJOY_Y 3
#define PIN_RJOY_X 4
#define PIN_LTHROTTLE 6
#define PIN_RTHROTTLE 21

/*** GLOBAL VARIABLES **************************************************************************************************************/

LiquidCrystalFast lcd(12, 11, 10, 9, 8, 7, 6);

byte emptyGlyph[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte halfGlyph[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};

byte fullGlyph[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

static byte lastBarValues[20][3];
static char lastSpeed[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
static char lastAltitude[7] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
static char warpSpeed[11] = { 'W', 'a', 'r', 'p', ' ', 'S', 'p', 'e', 'e', 'd', 0 };
static char outerSpace[8] = { 'O', 'T', 'R', ' ', 'S', 'P', 'C', 0 };
static bool isWarpSpeed = false;
static bool isOuterSpace = false;
static bool isReadyScreenVisible = false;

/*** PROTO-THREAD SETUP ************************************************************************************************************/

PT_THREAD(ControlsThread(PT_STRUCT));
PT_THREAD(MessageThread(PT_STRUCT));

byte ctrls_sem = SEM_BLOCK;

static struct pt ctrls_pt;
static struct pt msg_pt;

/*** DRAW FUNCTIONS ***************************************************************************************************************/

void reset_state()
{
  memset(lastBarValues, 255, 60);
  memset(lastSpeed, 32, 10);
  memset(lastAltitude, 32, 7);
  isWarpSpeed = isOuterSpace = false;
}

float get_resource_pct(char* pPayload)
{
  char value[4] = { *pPayload, *(pPayload + 1), *(pPayload + 2), 0 };
  return atoi(value) / 100.0f;
}

void draw_resource_bars(float fuel, float mp, float ec)
{
  float resources[3] = { fuel, mp, ec };
  
  for (int i = 0; i < 3; i++)
  {
    float glyphs = resources[i] * 20.0f;
    int fullGlyphs = (int)(glyphs + 0.5f);
    bool halfGlyph = glyphs - (float)fullGlyphs > 0.0f;
    
    for (int j = 0; j < 20; j++)
    {
      byte* val = &lastBarValues[j][i];
      
      if (j < fullGlyphs)
      { 
        if (*val != 2)
        {
          *val = 2;
          lcd.setCursor(j, i);
          lcd.write(2);
        } 
      }
      else if (j == fullGlyphs && halfGlyph)
      {  
        if (*val != 1)
        {
          *val = 1;
          lcd.setCursor(j, i);
          lcd.write(1);
        } 
      }
      else
      {
        if (*val != 0)
        {
          *val = 0;
          lcd.setCursor(j, i);
          lcd.write(0);
        } 
      }
    }
  }
}

void draw_speed(char* pPayload)
{
  if (*pPayload == '-')
  {
    if (!isWarpSpeed)
    {
      isWarpSpeed = true;
      lcd.setCursor(0, 3);
      lcd.print(warpSpeed);   
    }
    
    return;
  }
  else if (isWarpSpeed)
  {
    isWarpSpeed = false;
    lcd.setCursor(0, 3);
    lcd.print("          ");   
    memset(lastSpeed, ' ', 10);
  }
  
  int shift = 0;
  
  for (int i = 0; i < 8; i++)
  {
    if (*(pPayload + i) == '0')
      shift++;
    else
      break;
  }
  
  if (shift > 4) shift += 2;
  else if (shift > 1) shift += 1;

  char tmp1[10] = { 
    *pPayload, 
    *(pPayload + 1), 
    ',', 
    *(pPayload + 2), 
    *(pPayload + 3), 
    *(pPayload + 4), 
    ',', 
    *(pPayload + 5),
    *(pPayload + 6), 
    *(pPayload + 7) };
  
  char tmp2[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
  
  int index = 0;
  for (int i = shift; i < 10; i++)
    tmp2[index++] = tmp1[i];

  for (int i = 0; i < 10; i++)
  {
    char c = tmp2[i];
    
    if (c != lastSpeed[i])
    {
      lcd.setCursor(i, 3);
      lcd.print(c);
      lastSpeed[i] = c;
    }
  }
}

void draw_altitude(char* pPayload)
{
  if (*pPayload == '-')
  {
    if (!isOuterSpace)
    {
      isOuterSpace = true;
      lcd.setCursor(13, 3);
      lcd.print(outerSpace);   
    }
    
    return;
  }
  else if (isOuterSpace)
  {
    isOuterSpace = false;
    lcd.setCursor(13, 3);
    lcd.print("       ");   
    memset(lastAltitude, ' ', 7);
  }
  
  int shift = 0;
  
  for (int i = 0; i < 6; i++)
  {
    if (*(pPayload + i) == '0')
      shift++;
    else
      break;
  }
  
  char tmp[7] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
  int index = 5;
  
  if (shift != 6)
  {
    for (int i = 6; i >= 0; i--)
    {
      if (i == 3)
      {
        tmp[3] = ',';
        continue;
      }
     
      tmp[i] = *(pPayload + index);
      if (index - shift == 0) break;
      index--;
    }
  }
  
  for (int i = 0; i < 7; i++)
  {
    char c = tmp[i];
    
    if (c != lastAltitude[i])
    {
      lcd.setCursor(13 + i, 3);
      lcd.print(c);
      lastAltitude[i] = c;
    }
  }
}

void update_display(Message& msg)
{   
  float fuel = get_resource_pct(msg.payload);
  float mp = get_resource_pct(msg.payload + 3);
  float ec = get_resource_pct(msg.payload + 6);

  draw_resource_bars(fuel, mp, ec);
  draw_speed(msg.payload + 9);
  draw_altitude(msg.payload + 17);
  
  char sas[2] = { msg.payload[23], 0 };
  char rcs[2] = { msg.payload[24], 0 };
  
  int sasEnabled = atoi(sas);
  int rcsEnabled = atoi(rcs);
 
  if (sasEnabled)
    digitalWrite(PIN_SAS_LED, HIGH);
  else
    digitalWrite(PIN_SAS_LED, LOW);
    
  if (rcsEnabled)
    digitalWrite(PIN_RCS_LED, HIGH);
  else
    digitalWrite(PIN_RCS_LED, LOW);
}

/*** MESSAGING THREAD **********************************************************************************************************/

void process_message(Message& msg)
{ 
  switch (msg.messageType)
  {
    case 1:
    {
      if (!isReadyScreenVisible)
        draw_ready_screen(); 
    }
    break;
    case 2:
    {
      if (isReadyScreenVisible)
      {
        isReadyScreenVisible = false;
        lcd.clear(); 
        reset_state();
      }
      
      update_display(msg);
    }
    break;
  }
}

PT_THREAD(MessageThread(PT_STRUCT))
{
  static Message msg(process_message);
 
  PT_BEGIN(pt);
  while (1) {
    PT_WAIT_UNTIL(pt, Serial.available() > 0);
    msg.processByte(Serial.read());
  }
  PT_END(pt);
}

/*** CONTROLS THREAD ***************************************************************************************************************/

PT_THREAD(ControlsThread(PT_STRUCT))
{
  PT_BEGIN(pt);
  while (1) {
    PT_WAIT_WHILE(pt, ctrls_sem);
    ctrls_sem = SEM_BLOCK;
    
    Joystick.X(analogRead(PIN_LJOY_X));
    Joystick.Y(analogRead(PIN_LJOY_Y));
    Joystick.Zrotate(analogRead(PIN_LJOY_Z));
    
    int lt = analogRead(PIN_LTHROTTLE);
    if (lt < 4) 
      lt = 0;
    else if (lt > 1019)
      lt = 1023;
    Joystick.sliderLeft(lt);
    
    int rt = analogRead(PIN_RTHROTTLE);
    if (rt < 4) 
      rt = 0;
    else if (rt > 1019)
      rt = 1023;
    Joystick.sliderRight(rt);

    Joystick.button(1, digitalRead(PIN_STAGE) == LOW);
    Joystick.button(2, digitalRead(PIN_VIEW) == LOW);
    Joystick.button(3, digitalRead(PIN_RCS) == LOW);
    Joystick.button(4, digitalRead(PIN_SAS) == LOW);
    Joystick.button(5, 0);
    Joystick.button(6, 0);
    Joystick.button(7, 0);
    Joystick.button(8, 0);
    Joystick.button(9, 0);
    Joystick.button(10, 0);
    Joystick.button(11, 0);
    Joystick.button(12, 0);
    Joystick.button(13, digitalRead(PIN_GRAB) == LOW);
   
    int x = analogRead(PIN_RJOY_X);
    int y = analogRead(PIN_RJOY_Y);
    int mode = digitalRead(PIN_RJOY_MODE);
    
    if (x < 818 && x > 205)
    {
      if (y > 818)
      {
        if (mode) 
          Joystick.button(5, 1);
        else
          Joystick.button(9, 1);
      }
      else if (y < 205)
      {
        if (mode) 
          Joystick.button(6, 1);
        else
          Joystick.button(10, 1);
      }
    }
    else if (y < 818 && y > 205)
    {
      if (x > 818)
      {
        if (mode)
          Joystick.button(7, 1);
        else
          Joystick.button(11, 1);
      }
      else if (x < 205)
      {
        if (mode)
          Joystick.button(8, 1);
        else
          Joystick.button(12, 1);
      }
    }
   
    Joystick.send_now();
  }
  PT_END(pt);
}

/*** SCREENS ***********************************************************************************************************************/

void draw_ready_screen()
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("ALL SYSTEMS  ARE");
  lcd.setCursor(3, 2);
  lcd.print("GOOD  TO  GO!!");
  digitalWrite(PIN_SAS_LED, LOW);
  digitalWrite(PIN_RCS_LED, LOW);
  isReadyScreenVisible = true;
}

/*** TIMER INTERRUPT ***************************************************************************************************************/

void timerInterrupt()
{
  ctrls_sem = SEM_UNBLOCK;
}

/*** LOOP FUNCTION *****************************************************************************************************************/

void loop() 
{
  PT_SCHEDULE(ControlsThread(&ctrls_pt));
  PT_SCHEDULE(MessageThread(&msg_pt));
}

/*** SETUP FUNCTION ****************************************************************************************************************/

void setup() 
{
  Serial.begin(115200);
  Joystick.useManualSend(true);
  
  pinMode(PIN_STAGE, INPUT);
  pinMode(PIN_VIEW, INPUT);
  pinMode(PIN_RJOY_MODE, INPUT);
  pinMode(PIN_RCS, INPUT);
  pinMode(PIN_RCS_LED, OUTPUT);
  pinMode(PIN_SAS, INPUT);
  pinMode(PIN_SAS_LED, OUTPUT);
  pinMode(PIN_GRAB, INPUT);
  
  reset_state();

  lcd.begin(20, 4);
  
  lcd.createChar(0, emptyGlyph);
  lcd.createChar(1, halfGlyph);
  lcd.createChar(2, fullGlyph);
  
  draw_ready_screen();
  
  PT_INIT(&ctrls_pt);
  PT_INIT(&msg_pt);
  
  ctrls_sem = SEM_UNBLOCK;

  Timer3.initialize(10000);
  Timer3.attachInterrupt(timerInterrupt);
}
