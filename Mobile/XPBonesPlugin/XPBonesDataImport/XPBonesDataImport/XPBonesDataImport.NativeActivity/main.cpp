#include "view_main.h"
#include "view_test.h"

using namespace bones;

void android_main(struct android_app* app) 
{
	APP::Start(app, new MainView);
}
