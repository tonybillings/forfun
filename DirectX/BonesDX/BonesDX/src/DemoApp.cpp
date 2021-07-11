#include "DemoApp.h"

DemoApp::DemoApp()
	: App(WINDOW_WIDTH, WINDOW_HEIGHT, APP_TITLE)
{
	AddObject(NEW_OBJECT(DemoScene));
}