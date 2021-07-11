#ifndef __bones_config__
#define __bones_config__

/*** INCLUDE TFT LIBRARY *************************************************************************/

#include "Adafruit_RA8875.h"

/*** DEFINE TYPE FOR TFT POINTER *****************************************************************/

#define TFT_TYPE_PTR Adafruit_RA8875*

/***  DEFINE THUMBSTICK PINS *********************************************************************/

#define THUMBSTICK_1_PIN_X          0
#define THUMBSTICK_1_PIN_Y          1
#define THUMBSTICK_1_PIN_ENTER      30
#define THUMBSTICK_2_PIN_X          2
#define THUMBSTICK_2_PIN_Y          3
#define THUMBSTICK_2_PIN_ENTER      32

/*** DEFINE TFT PINS *****************************************************************************/

#define RA8875_INT      3
#define RA8875_CS       53
#define RA8875_RESET    48

#endif