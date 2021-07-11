#include "stdafx.h"
#include "WebsocketServer.h"

XPLMDataRef gSpeed = NULL;
XPLMDataRef gHeadingTrue = NULL;
XPLMDataRef gHeadingMag = NULL;
XPLMDataRef gPitch = NULL;
XPLMDataRef gRoll = NULL;
XPLMDataRef gAltitude = NULL;
XPLMDataRef gLongitude = NULL;
XPLMDataRef gLatitude = NULL;
XPLMDataRef gWarpLon = NULL;
XPLMDataRef gWarpLat = NULL;

float gWarpLonValue = 0;
float gWarpLatValue = 0;

float RegisterWarpDataRefsInDRE(float elapsedMe, float elapsedSim, int counter, void * refcon);  //  Declare callback to register dataref

XPLMCommandRef MyCommand = NULL;
int MyCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);

XPLMCommandRef WarpCommand = NULL;
int WarpCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon);

float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);

asio::io_service io_service;
asio::ip::udp::endpoint endpoint;
asio::ip::udp::socket* pSocket;

thread* serverThread;
WebsocketServer server;

void StartWebSocketsServer()
{
	serverThread = new thread([]() {
		try {
			thread t(bind(&WebsocketServer::process_messages, &server));
			server.run(8666);
			t.join();
		}
		catch (websocketpp::exception const & e) { }
	});
}

float RegisterWarpDataRefsInDRE(float elapsedMe, float elapsedSim, int counter, void * refcon)
{
	XPLMPluginID PluginID = XPLMFindPluginBySignature("xplanesdk.examples.DataRefEditor");

	if (PluginID != XPLM_NO_PLUGIN_ID)
	{
		XPLMSendMessageToPlugin(PluginID, 0x01000000, (void*)"warp/latitude");
		XPLMSendMessageToPlugin(PluginID, 0x01000000, (void*)"warp/longitude");
	}

	return 0;
}

float GetWarpLongitude(void* inRefcon)
{
	return gWarpLonValue;
}

void SetWarpLongitude(void* inRefcon, float inValue)
{
	gWarpLonValue = inValue;
}

float GetWarpLatitude(void* inRefcon)
{
	return gWarpLatValue;
}

void SetWarpLatitude(void* inRefcon, float inValue)
{
	gWarpLatValue = inValue;
}

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
	strcpy_s(outName, 18, "Bones Data Export");
	strcpy_s(outSig, 17, "Bones.DataExport");
	strcpy_s(outDesc, 34, "A plugin to output data over TCP.");

	MyCommand = XPLMCreateCommand("BONES/DataExport/ToggleEnable", "Toggle Enable");
	XPLMRegisterCommandHandler(MyCommand, MyCommandHandler,	1, (void*)0);		

	WarpCommand = XPLMCreateCommand("BONES/Warp/Execute", "Execute Warp");
	XPLMRegisterCommandHandler(WarpCommand, WarpCommandHandler, 1, (void*)0);

	gSpeed = XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot");
	gHeadingTrue = XPLMFindDataRef("sim/flightmodel/position/true_psi");
	gHeadingMag = XPLMFindDataRef("sim/flightmodel/position/mag_psi");
	gPitch = XPLMFindDataRef("sim/cockpit/gyros/the_vac_ind_deg");
	gRoll = XPLMFindDataRef("sim/cockpit/gyros/phi_vac_ind_deg");
	gAltitude = XPLMFindDataRef("sim/flightmodel/misc/h_ind");
	gLongitude = XPLMFindDataRef("sim/flightmodel/position/longitude");
	gLatitude = XPLMFindDataRef("sim/flightmodel/position/latitude");

	gWarpLon = XPLMRegisterDataAccessor("warp/longitude",
		xplmType_Float,                                // The types we support
		1,                                             // Writable
		NULL, NULL,									   // Integer accessors
		GetWarpLongitude, SetWarpLongitude,            // Float accessors
		NULL, NULL,                                    // Doubles accessors
		NULL, NULL,                                    // Int array accessors
		NULL, NULL,                                    // Float array accessors
		NULL, NULL,                                    // Raw data accessors
		NULL, NULL);                                   // Refcons not used

	gWarpLat = XPLMRegisterDataAccessor("warp/latitude",
		xplmType_Float,                                // The types we support
		1,                                             // Writable
		NULL, NULL,									   // Integer accessors
		GetWarpLatitude, SetWarpLatitude,              // Float accessors
		NULL, NULL,                                    // Doubles accessors
		NULL, NULL,                                    // Int array accessors
		NULL, NULL,                                    // Float array accessors
		NULL, NULL,                                    // Raw data accessors
		NULL, NULL);                                   // Refcons not used

	gWarpLon = XPLMFindDataRef("warp/longitude");
	gWarpLat = XPLMFindDataRef("warp/latitude");

	XPLMSetDataf(gWarpLon, 0);
	XPLMSetDataf(gWarpLat, 0);

	endpoint = asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.66"), 6666);
	pSocket = new asio::ip::udp::socket(io_service);
	pSocket->connect(endpoint);

	XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, -1, (void*)0);
	XPLMRegisterFlightLoopCallback(RegisterWarpDataRefsInDRE, -1, (void*)0);

	StartWebSocketsServer();

	return 1;
}

PLUGIN_API void XPluginStop(void)
{
	XPLMUnregisterCommandHandler(MyCommand, MyCommandHandler, 0, 0);
	XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, (void*)0);
	XPLMUnregisterFlightLoopCallback(RegisterWarpDataRefsInDRE, (void*)0);
	serverThread->~thread();
}

PLUGIN_API void XPluginDisable(void)
{

}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, long inMessage, void* inParam)
{

}

bool bonesEnabled = false;

int MyCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon)
{
	if (inPhase == xplm_CommandEnd)
	{
		bonesEnabled = !bonesEnabled;
	}

	return 0;
}

int WarpCommandHandler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon)
{
	if (inPhase == xplm_CommandEnd)
	{
		auto altitude = XPLMGetDataf(gAltitude);
		

		double x, y, z;
		XPLMWorldToLocal(gWarpLatValue, gWarpLonValue, altitude, &x, &y, &z);

		auto localXDataRef = XPLMFindDataRef("sim/flightmodel/position/local_x");
		auto localYDataRef = XPLMFindDataRef("sim/flightmodel/position/local_y");
		auto localZDataRef = XPLMFindDataRef("sim/flightmodel/position/local_z");

		XPLMSetDatad(localXDataRef, x);
		XPLMSetDatad(localYDataRef, y);
		XPLMSetDatad(localZDataRef, z);

		auto latDataRef = XPLMFindDataRef("sim/flightmodel/position/latitude");
		auto lonDataRef = XPLMFindDataRef("sim/flightmodel/position/longitude");

		XPLMSetDatad(latDataRef, gWarpLatValue);
		XPLMSetDatad(lonDataRef, gWarpLonValue);

	}

	return 0;
}

int bonesTick = 0;
float MyFlightLoopCallback(
	float                inElapsedSinceLastCall,
	float                inElapsedTimeSinceLastFlightLoop,
	int                  inCounter,
	void *               inRefcon)
{
	if (!bonesEnabled) return -1;
	bonesTick++;

	if (bonesTick % 8 == 0)
	{
		float speed = XPLMGetDataf(gSpeed);
		float headingT = XPLMGetDataf(gHeadingTrue);
		float headingM = XPLMGetDataf(gHeadingMag);
		float pitch = XPLMGetDataf(gPitch);
		float roll = XPLMGetDataf(gRoll);
		float altitude = XPLMGetDataf(gAltitude);
		float longitude = XPLMGetDataf(gLongitude);
		float latitude = XPLMGetDataf(gLatitude);

		std::string packet = "";
		packet += "S";
		packet += std::to_string(speed);
		packet += " ";
		packet += "H";
		packet += std::to_string(headingT);
		packet += " ";
		packet += "h";
		packet += std::to_string(headingM);
		packet += " ";
		packet += "P";
		packet += std::to_string(pitch);
		packet += " ";
		packet += "R";
		packet += std::to_string(roll);
		packet += " ";
		packet += "A";
		packet += std::to_string(altitude);
		packet += " ";
		packet += "X";
		packet += std::to_string(longitude);
		packet += " ";
		packet += "Y";
		packet += std::to_string(latitude);
		packet += " ";

		pSocket->send(asio::buffer(packet));

		if (bonesTick % 48 == 0)
		{
			server.broadcast(packet);
		}
	}

	return -1;
}