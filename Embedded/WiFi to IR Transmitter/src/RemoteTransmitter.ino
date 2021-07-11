#include <WiFi.h>
#include <IRremote.h>

#define SSID        "FreeThought"
#define PASSWORD    {CHANGE ME!!!}
#define IP_ADDRESS  IPAddress(192, 168, 1, 62)
#define PORT        6163
#define SYNC        22
#define ETB         23

static uint32_t RecPowerToggle[23] = {900,900,  900,900,   900,900,  1750,900, 900,900,    900,900,  900,900,  900,900,   900,1750,  900,900,   1750,900, 900};
static uint32_t RecOk[21] = {1750,900, 900,1750,  1750,900, 900,900,  900,900,    900,900,  900,1750, 1750,1750, 900,900,   900,900,   900};  
static uint32_t RecBack[29] = {1750,900, 900,1750,  1750,900, 900,900,  900,900,    900,4400, 900,1750, 1750,1750, 900,900,   900,900,   1750,900, 900,900, 900,1750, 1750,900, 900}; 
static uint32_t RecMenu[29] = {1750,900, 900,1750,  1750,900, 900,900,  900,900,    900,4400, 900,1750, 1750,900,  900,1750,  1750,1750, 900,900,  900,900, 900,900,  1750,900, 900}; 
static uint32_t RecUp[21] = {1750,900, 900,1750,  1750,900, 900,900,  900,900,    900,900,  900,1750, 1750,900,  900,900,   900,900,   900};  
static uint32_t RecDown[21] = {1750,900, 900,1750,  1750,900, 900,900,  900,900,    900,900,  900,1750, 1750,900,  900,900,   900,1750,  900};  
static uint32_t RecLeft[19] = {1750,900, 900,1750,  1750,900, 900,900,  900,900,    900,900,  900,1750, 1750,1750, 1750,1750, 900};  
static uint32_t RecRight[19] = {1750,900, 900,1750,  1750,900, 900,900,  900,900,    900,900,  900,1750, 1750,1750, 900,900,   1750}; 
static uint32_t RecVolumeUp[23] = {900,900,  900,900,   900,900,  1750,900, 900,900,    900,900,  900,900,  900,1750,  1750,900,  900,900,   900,900,  900}; 
static uint32_t RecVolumeDown[23] = {900,900,  900,900,   900,900,  1750,900, 900,900,    900,900,  900,900,  900,1750,  1750,900,  900,900,   900,1750, 900};
static uint32_t RecInputSelectGame[35] = {900,900,  900,900,   900,900,  1750,900, 900,900,    900,900,  900,4400, 900,900,   900,900,   900,900,   900,900,  900,900, 900,1750, 900,900,  900,900, 900,900, 900,900, 1750};
static uint32_t RecInputSelectMediaPlayer[23] = {900,900,  1750,900,  900,900,  900,1750, 1750,1750,  900,900,  900,900,  900,900,   900,900,   900,900,   900,900,  900};  
static uint32_t RecMovieSoundMode[11] = {900,900,  1750,1750, 1750,900, 900,900,  900,900,    900};  
static uint32_t RecMusicSoundMode[11] = {900,900,  1750,1750, 1750,900, 900,900,  900,900,    900};  

static uint32_t ProPowerOn = 0xCEA0;
static uint32_t ProPowerOff = 0xCE60;
static uint32_t ProOk = 0xCEF4;
static uint32_t ProBack = 0xCEC0;
static uint32_t ProMenu = 0xCE74;
static uint32_t ProUp = 0xCE80;
static uint32_t ProDown = 0xCE40;
static uint32_t ProLeft = 0xCE6C;
static uint32_t ProRight = 0xCE2C;
static uint32_t ProCycleAdvMenu = 0xCECE;
static uint32_t ProCycleColorProfile = 0xCE11;
static uint32_t ProCyclePictureMode = 0xCE2F;

static uint32_t RokuOk = 0x574354AB;
static uint32_t RokuBack = 0x57436699;
static uint32_t RokuMenu = 0x5743C03F;
static uint32_t RokuUp = 0x57439867;
static uint32_t RokuDown = 0x5743CC33;
static uint32_t RokuLeft = 0x57437887;
static uint32_t RokuRight = 0x5743B44B;
static uint32_t RokuStar = 0x57438679;
static uint32_t RokuPlayPause = 0x574332CD;
static uint32_t RokuRewind = 0x57432CD3;
static uint32_t RokuForward = 0x5743AA55;

static WiFiUDP _server;
static IRsend _ir;

inline void sendJVC(uint32_t data)
{
  _ir.sendJVC(data, 16, 0);
  delay(20);
  _ir.sendJVC(data, 16, 1);
}

inline void sendNEC(uint32_t data)
{
  _ir.sendNEC(data, 32);
}

void sendCommand(byte code)
{
  if (code < 114)
  {
    switch (code)
    {
      case 100:
        _ir.sendRaw(RecPowerToggle, 23, 38);
        break;
      case 101:
        _ir.sendRaw(RecOk, 21, 38);
        break;
      case 102:
        _ir.sendRaw(RecBack, 29, 38);
        break;
      case 103:
        _ir.sendRaw(RecMenu, 29, 38);
        break;
      case 104:
        _ir.sendRaw(RecUp, 21, 38);
        break;
      case 105:
        _ir.sendRaw(RecDown, 21, 38);
        break;
      case 106:
        _ir.sendRaw(RecLeft, 19, 38);
        break;
      case 107:
        _ir.sendRaw(RecRight, 19, 38);
        break;
      case 108:
        _ir.sendRaw(RecVolumeUp, 23, 38);
        break;
      case 109:
        _ir.sendRaw(RecVolumeDown, 23, 38);
        break;
      case 110:
        _ir.sendRaw(RecInputSelectGame, 35, 38);
        break;
      case 111:
        _ir.sendRaw(RecInputSelectMediaPlayer, 23, 38);
        break;
      case 112:
        _ir.sendRC5x(16, 37, 29); 
        break;
      case 113:
        _ir.sendRC5x(16, 37, 53); 
        break;
    }
  }
  else if (code < 126)
  {
    switch (code)
    {
      case 114:
        sendJVC(ProPowerOn);
        break;
      case 115:
        sendJVC(ProPowerOff);
        delay(600);
        sendJVC(ProPowerOff);
        break;
      case 116:
        sendJVC(ProOk);
        break;
      case 117:
        sendJVC(ProBack);
        break;
      case 118:
        sendJVC(ProMenu);
        break;
      case 119:
        sendJVC(ProUp);
        break;
      case 120:
        sendJVC(ProDown);
        break;
      case 121:
        sendJVC(ProLeft);
        break;
      case 122:
        sendJVC(ProRight);
        break;
      case 123:
        sendJVC(ProCycleAdvMenu);
        break;
      case 124:
        sendJVC(ProCycleColorProfile);
        break;
      case 125:
        sendJVC(ProCyclePictureMode);
        break;
    }
  }
  else
  {
    switch (code)
    {
      case 126:
        sendNEC(RokuOk);
        break;
      case 127:
        sendNEC(RokuBack);
        break;
      case 128:
        sendNEC(RokuMenu);
        break;
      case 129:
        sendNEC(RokuUp);
        break;
      case 130:
        sendNEC(RokuDown);
        break;
      case 131:
        sendNEC(RokuLeft);
        break;
      case 132:
        sendNEC(RokuRight);
        break;
      case 133:
        sendNEC(RokuStar);
        break;
      case 134:
        sendNEC(RokuPlayPause);
        break;
      case 135:
        sendNEC(RokuRewind);
        break;
      case 136:
        sendNEC(RokuForward);
        break;
    }
  }
}

void processData(byte data)
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
      sendCommand(commandCode);
    
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
}

void setup()
{
  WiFi.config(IP_ADDRESS, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(SSID, PASSWORD);
  WiFi.setSleep(false);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  _server.begin(PORT);
  pinMode(13, OUTPUT);
}

void loop()
{
  if (_server.parsePacket() > 0 && _server.available() > 0)
  {
    auto remoteIP = _server.remoteIP();

    if (remoteIP == IPAddress(192, 168, 1, 37) || remoteIP == IPAddress(192, 168, 1, 18) || remoteIP == IPAddress(192, 168, 1, 38))
    {
      while (_server.available() > 0)
        processData((byte)_server.read());
    }
    else
    {
      _server.flush();
    }
  }
}
