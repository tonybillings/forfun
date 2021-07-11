#include "cr_remote_controller.h"

#define RC_RAW_DATA_LENGTH      35
#define RC_SIGNAL_FREQUENCY     38

IRsend RemoteController::m_irsend;

void RemoteController::ExecCommand(int16_t id)
{
    if (id < 50)
        SendNEC(id);
    else if (id < 100)
        SendSAMSUNG(id);
    else
        SendRaw(id);
}

void RemoteController::SendNEC(int16_t id)
{
    uint32_t code;
	
    switch (id)
    {
        case RC_HDMI_CABLE:
            code = hdmi_cable;
            break;
        case RC_HDMI_ROKU:
            code = hdmi_roku;
            break;
        case RC_LIGHT_ON:
            code = light_on;
            break;
        case RC_LIGHT_OFF:
            code = light_off;
            break;
        case RC_LIGHT_RED:
            code = light_red;
            break;
        case RC_LIGHT_GREEN:
            code = light_green;
            break;
        case RC_LIGHT_BLUE:
            code = light_blue;
            break;
        case RC_LIGHT_BRIGHTER:
            code = light_brighter;
            break;
        case RC_LIGHT_DIMMER:
            code = light_dimmer;
            break;
        case RC_ROKU_HOME:
            code = ((uint32_t)roku_code << 16) | roku_home;
            break;
        case RC_ROKU_BACK:
            code = ((uint32_t)roku_code << 16) | roku_back;
            break;
        case RC_ROKU_UP:
            code = ((uint32_t)roku_code << 16) | roku_up;
            break;
        case RC_ROKU_DOWN:
            code = ((uint32_t)roku_code << 16) | roku_down;
            break;
        case RC_ROKU_LEFT:
            code = ((uint32_t)roku_code << 16) | roku_left;
            break;
        case RC_ROKU_RIGHT:
            code = ((uint32_t)roku_code << 16) | roku_right;
            break;
        case RC_ROKU_ENTER:
            code = ((uint32_t)roku_code << 16) | roku_enter;
            break;
        case RC_ROKU_RESTART:
            code = ((uint32_t)roku_code << 16) | roku_restart;
            break;
        case RC_ROKU_STAR:
            code = ((uint32_t)roku_code << 16) | roku_star;
            break;        
        case RC_ROKU_REWIND:
            code = ((uint32_t)roku_code << 16) | roku_rewind;
            break;
        case RC_ROKU_FORWARD:
            code = ((uint32_t)roku_code << 16) | roku_forward;
            break;
        case RC_ROKU_PLAY_PAUSE:
            code = ((uint32_t)roku_code << 16) | roku_play_pause;
            break;
        case RC_ROKU_NETFLIX:
            code = ((uint32_t)roku_code << 16) | roku_netflix;
            break;
        case RC_ROKU_AMAZON:
            code = ((uint32_t)roku_code << 16) | roku_amazon;
            break;
        case RC_ROKU_BLOCKBUSTER:
            code = ((uint32_t)roku_code << 16) | roku_blockbuster;
            break;
        case RC_ROKU_MGO:
            code = ((uint32_t)roku_code << 16) | roku_mgo;
            break;     
        default:
            code = 0;
    }
    
    if (code != 0) { m_irsend.IRsendNEC::send(code); }
}

void RemoteController::SendSAMSUNG(int16_t id)
{
    uint16_t* code;
    
    switch (id)
    {
        case RC_TV_POWER:
            code = &tv_power;
            break;
        case RC_TV_SOURCE:
            code = &tv_source;
            break;
        case RC_TV_VOL_UP:
            code = &tv_vol_up;
            break;
        case RC_TV_VOL_DOWN:
            code = &tv_vol_down;
            break;
        case RC_TV_MUTE:
            code = &tv_mute;
            break;
		case RC_TV_MENU:
            code = &tv_menu;
            break;
        case RC_TV_TOOLS:
            code = &tv_tools;
            break;
        case RC_TV_INFO:
            code = &tv_info;
            break;
        case RC_TV_UP:
            code = &tv_up;
            break;
        case RC_TV_DOWN:
            code = &tv_down;
            break;
        case RC_TV_LEFT:
            code = &tv_left;
            break;
        case RC_TV_RIGHT:
            code = &tv_right;
            break;
        case RC_TV_ENTER:
            code = &tv_enter;
            break;
        case RC_TV_RETURN:
            code = &tv_return;
            break;
        case RC_TV_EXIT:
            code = &tv_exit;
            break;
        default:
            code = 0;
    }
   
    if (code != 0) 
	{ 
		uint32_t fullCode = ((uint32_t)tv_code << 16) | *code;
		m_irsend.IRsendNECx::send(fullCode); 
	}	
}

void RemoteController::SendRaw(int16_t id)
{
    uint16_t* code;

    switch (id)
    {
        case RC_CABLE_POWER:
            code = cable_power;
            break;
        case RC_CABLE_CHAN_UP:
            code = cable_chan_up;
            break;
        case RC_CABLE_CHAN_DOWN:
            code = cable_chan_down;
            break;
        case RC_CABLE_GUIDE:
            code = cable_guide;
            break;
        case RC_CABLE_MENU:
            code = cable_menu;
            break;
        case RC_CABLE_UP:
            code = cable_up;
            break;
        case RC_CABLE_DOWN:
            code = cable_down;
            break; 
        case RC_CABLE_LEFT:
            code = cable_left;
            break;
        case RC_CABLE_RIGHT:
            code = cable_right;
            break;
        case RC_CABLE_ENTER:
            code = cable_enter;
            break;
        case RC_CABLE_LAST:
            code = cable_last;
            break;
        case RC_CABLE_INFO:
            code = cable_info;
            break;
        case RC_CABLE_EXIT:
            code = cable_exit;
            break;
        case RC_CABLE_PAUSE:
            code = cable_pause;
            break;
        case RC_CABLE_PLAY:
            code = cable_play;
            break;
        case RC_CABLE_REWIND:
            code = cable_rewind;
            break;
        case RC_CABLE_FORWARD:
            code = cable_forward;
            break;
        case RC_CABLE_STOP:
            code = cable_stop;
            break;
        case RC_CABLE_RECORD:
            code = cable_record;
            break;
        case RC_CABLE_FAV:
            code = cable_fav;
            break;
        case RC_CABLE_0:
            code = cable_0;
            break;
        case RC_CABLE_1:
            code = cable_1;
            break;
        case RC_CABLE_2:
            code = cable_2;
            break;
        case RC_CABLE_3:
            code = cable_3;
            break;
        case RC_CABLE_4:
            code = cable_4;
            break;
        case RC_CABLE_5:
            code = cable_5;
            break;
        case RC_CABLE_6:
            code = cable_6;
            break;
        case RC_CABLE_7:
            code = cable_7;
            break;
        case RC_CABLE_8:
            code = cable_8;
            break;
        case RC_CABLE_9:
            code = cable_9;
            break;            
        default:
            code = 0;     
    }  
	
    if (code != 0) 
	{ 
		memcpy(cable_code + 21, code, 28); 
		m_irsend.IRsendRaw::send(cable_code, RC_RAW_DATA_LENGTH, RC_SIGNAL_FREQUENCY); 
	}
}