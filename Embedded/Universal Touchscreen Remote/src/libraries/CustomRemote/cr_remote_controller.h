#ifndef __cr_remote_controller__
#define __cr_remote_controller__

#include "cr_common.h"

/*** COMMAND NAMES *************************************************************************************************************/

#define RC_HDMI_CABLE           0
#define RC_HDMI_ROKU            1
#define RC_LIGHT_ON             2
#define RC_LIGHT_OFF            3
#define RC_LIGHT_RED            4
#define RC_LIGHT_GREEN          5
#define RC_LIGHT_BLUE           6
#define RC_LIGHT_BRIGHTER       7
#define RC_LIGHT_DIMMER         8
#define RC_ROKU_HOME            9
#define RC_ROKU_BACK            10
#define RC_ROKU_UP              11 
#define RC_ROKU_DOWN            12 
#define RC_ROKU_LEFT            13
#define RC_ROKU_RIGHT           14
#define RC_ROKU_ENTER           15
#define RC_ROKU_RESTART         16 
#define RC_ROKU_STAR            17 
#define RC_ROKU_REWIND          18 
#define RC_ROKU_FORWARD         19 
#define RC_ROKU_PLAY_PAUSE      20 
#define RC_ROKU_NETFLIX         21 
#define RC_ROKU_AMAZON          22 
#define RC_ROKU_BLOCKBUSTER     23 
#define RC_ROKU_MGO             24 
#define RC_TV_POWER             50
#define RC_TV_SOURCE            51
#define RC_TV_VOL_UP            52
#define RC_TV_VOL_DOWN          53
#define RC_TV_MUTE              54
#define RC_TV_MENU              55
#define RC_TV_TOOLS             56
#define RC_TV_INFO              57
#define RC_TV_UP                58
#define RC_TV_DOWN              59
#define RC_TV_LEFT              60
#define RC_TV_RIGHT             61
#define RC_TV_ENTER             62
#define RC_TV_RETURN            63
#define RC_TV_EXIT              64
#define RC_CABLE_POWER          100
#define RC_CABLE_CHAN_UP        101
#define RC_CABLE_CHAN_DOWN      102
#define RC_CABLE_GUIDE          103
#define RC_CABLE_MENU           104
#define RC_CABLE_UP             105
#define RC_CABLE_DOWN           106
#define RC_CABLE_LEFT           107
#define RC_CABLE_RIGHT          108
#define RC_CABLE_ENTER          109
#define RC_CABLE_LAST           110
#define RC_CABLE_INFO           111
#define RC_CABLE_EXIT           112
#define RC_CABLE_PAUSE          113
#define RC_CABLE_PLAY           114
#define RC_CABLE_REWIND         115
#define RC_CABLE_FORWARD        116
#define RC_CABLE_STOP           117
#define RC_CABLE_RECORD         118
#define RC_CABLE_FAV            119
#define RC_CABLE_0              120
#define RC_CABLE_1              121
#define RC_CABLE_2              122
#define RC_CABLE_3              123
#define RC_CABLE_4              124
#define RC_CABLE_5              125
#define RC_CABLE_6              126
#define RC_CABLE_7              127
#define RC_CABLE_8              128
#define RC_CABLE_9              129

/*** COMMAND DATA **************************************************************************************************************/

static uint32_t hdmi_cable             = 0x40BF609F;
static uint32_t hdmi_roku              = 0x40BF50AF;
static uint32_t light_on               = 0xFFE01F;
static uint32_t light_off              = 0xFF609F;
static uint32_t light_red              = 0xFF906F;
static uint32_t light_green            = 0xFF10EF;
static uint32_t light_blue             = 0xFF50AF;
static uint32_t light_brighter         = 0xFFA05F;
static uint32_t light_dimmer           = 0xFF20DF;
static uint16_t roku_home              = 0xC03F;
static uint16_t roku_back              = 0x6699;
static uint16_t roku_up                = 0x9867;
static uint16_t roku_down              = 0xCC33;
static uint16_t roku_left              = 0x7887;
static uint16_t roku_right             = 0xB44B;
static uint16_t roku_enter             = 0x54AB;
static uint16_t roku_restart           = 0x1EE1;
static uint16_t roku_star              = 0x8679;
static uint16_t roku_rewind            = 0x2CD3;
static uint16_t roku_forward           = 0xAA55;
static uint16_t roku_play_pause        = 0x32CD;
static uint16_t roku_netflix           = 0xD22D;
static uint16_t roku_amazon            = 0x08F7;
static uint16_t roku_blockbuster       = 0x04FB;
static uint16_t roku_mgo               = 0xF00F;
static uint16_t roku_code			   = 0x5743;
static uint16_t tv_power               = 0x40BF;
static uint16_t tv_source              = 0x807F;
static uint16_t tv_vol_up              = 0xE01F;
static uint16_t tv_vol_down            = 0xD02F;
static uint16_t tv_mute                = 0xF00F;
static uint16_t tv_menu                = 0x58A7;
static uint16_t tv_tools               = 0xD22D;
static uint16_t tv_info                = 0xF807;
static uint16_t tv_up                  = 0x06F9;
static uint16_t tv_down                = 0x8679;
static uint16_t tv_left                = 0xA659;
static uint16_t tv_right               = 0x46B9;
static uint16_t tv_enter               = 0x16E9;
static uint16_t tv_return              = 0x1AE5;
static uint16_t tv_exit                = 0xB44B;
static uint16_t tv_code				   = 0xE0E0;
static uint16_t cable_power[]          = { 824, 280, 690, 280, 690, 280, 690, 280, 2760, 280, 690, 280, 690, 280 };
static uint16_t cable_chan_up[]        = { 1100, 280, 690, 280, 690, 280, 690, 280, 2480, 280, 690, 280, 690, 280 };
static uint16_t cable_chan_down[]      = { 964, 280, 690, 280, 690, 280, 690, 280, 2620, 280, 690, 280, 690, 280 };
static uint16_t cable_guide[]          = { 1650, 280, 690, 280, 690, 280, 960, 280, 1656, 280, 690, 280, 690, 280 };
static uint16_t cable_menu[]           = { 2620, 280, 690, 280, 690, 280, 960, 280, 690, 280, 690, 280, 690, 280 };
static uint16_t cable_up[]             = { 2480, 280, 690, 280, 690, 280, 960, 280, 824, 280, 690, 280, 690, 280 }; 
static uint16_t cable_down[]           = { 2344, 280, 690, 280, 690, 280, 960, 280, 960, 280, 690, 280, 690, 280 }; 
static uint16_t cable_left[]           = { 2208, 280, 690, 280, 690, 280, 960, 280, 1100, 280, 690, 280, 690, 280 }; 
static uint16_t cable_right[]          = { 2064, 280, 690, 280, 690, 280, 960, 280, 1240, 280, 690, 280, 690, 280 };
static uint16_t cable_enter[]          = { 1924, 280, 690, 280, 690, 280, 960, 280, 1380, 280, 690, 280, 690, 280 }; 
static uint16_t cable_last[]           = { 2068, 280, 690, 280, 690, 280, 1376, 280, 828, 280, 690, 280, 690, 280 }; 
static uint16_t cable_info[]           = { 1796, 280, 690, 280, 690, 280, 960, 280, 1520, 280, 690, 280, 690, 280 }; 
static uint16_t cable_exit[]           = { 1240, 280, 690, 280, 690, 280, 960, 280, 2064, 280, 690, 280, 690, 280 };
static uint16_t cable_pause[]          = { 2208, 280, 690, 280, 690, 280, 1100, 280, 960, 280, 690, 280, 690, 280 };
static uint16_t cable_play[]           = { 2480, 280, 690, 280, 690, 280, 1100, 280, 690, 280, 690, 280, 690, 280 }; 
static uint16_t cable_rewind[]         = { 2068, 280, 690, 280, 690, 280, 1100, 280, 1100, 280, 690, 280, 690, 280 }; 
static uint16_t cable_forward[]        = { 1928, 280, 690, 280, 690, 280, 1100, 280, 1244, 280, 690, 280, 690, 280 }; 
static uint16_t cable_stop[]           = { 2344, 280, 690, 280, 690, 280, 1100, 280, 828, 280, 690, 280, 690, 280 }; 
static uint16_t cable_record[]         = { 1796, 280, 690, 280, 690, 280, 1100, 280, 1376, 280, 690, 280, 690, 280 }; 
static uint16_t cable_fav[]            = { 1932, 280, 690, 280, 690, 280, 1376, 280, 960, 280, 690, 280, 690, 280 }; 
static uint16_t cable_0[]              = { 690, 280, 690, 280, 690, 280, 690, 280, 690, 280, 690, 280, 690, 280 }; 
static uint16_t cable_1[]              = { 2756, 280, 690, 280, 690, 280, 690, 280, 820, 280, 690, 280, 690, 280 }; 
static uint16_t cable_2[]              = { 2620, 280, 690, 280, 690, 280, 690, 280, 964, 280, 690, 280, 690, 280 }; 
static uint16_t cable_3[]              = { 2488, 280, 690, 280, 690, 280, 690, 280, 1100, 280, 690, 280, 690, 280 }; 
static uint16_t cable_4[]              = { 2344, 280, 690, 280, 690, 280, 690, 280, 1244, 280, 690, 280, 690, 280 }; 
static uint16_t cable_5[]              = { 2208, 280, 690, 280, 690, 280, 690, 280, 1380, 280, 690, 280, 690, 280 }; 
static uint16_t cable_6[]              = { 2068, 280, 690, 280, 690, 280, 690, 280, 1520, 280, 690, 280, 690, 280 }; 
static uint16_t cable_7[]              = { 1928, 280, 690, 280, 690, 280, 690, 280, 1656, 280, 690, 280, 690, 280 }; 
static uint16_t cable_8[]              = { 1792, 280, 690, 280, 690, 280, 690, 280, 1792, 280, 690, 280, 690, 280 }; 
static uint16_t cable_9[]              = { 1652, 280, 690, 280, 690, 280, 690, 280, 1928, 280, 690, 280, 690, 280 }; 
static uint16_t cable_code[]           = { 280, 690, 280, 1796, 280, 690, 280, 2760, 280, 1240, 280, 1240, 280, 1100, 280, 2620, 280, 12928, 280, 690, 280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/*** CONTROLLER ****************************************************************************************************************/

class RemoteController
{
private:
    static IRsend m_irsend;
    
private:
    static void SendNEC(int16_t id);
    static void SendSAMSUNG(int16_t id);
    static void SendRaw(int16_t id);
    
public:
    static void ExecCommand(int16_t id);
};

#endif