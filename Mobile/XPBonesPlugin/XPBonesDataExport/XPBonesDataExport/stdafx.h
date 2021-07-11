#pragma once

#define XPLM200 
#define XPLM210 
#define IBM 1
#define APL 0
#define _WEBSOCKETPP_CPP11_STRICT_ 1

#include "targetver.h"

#include "asio/asio.hpp"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <set>

#include "XPLMDefs.h"
#include "XPLMUtilities.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMMenus.h"
#include "XPWidgets.h"
#include "XPStandardWidgets.h"
#include "XPLMPlugin.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMScenery.h"

