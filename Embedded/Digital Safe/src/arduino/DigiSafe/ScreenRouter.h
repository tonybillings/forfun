#ifndef __digisafe_screen_router_H__
#define __digisafe_screen_router_H__

#include <bones.h>
#include "SerialCommands.h"

namespace DigiSafe
{
	class ScreenRouter
	{
	public:
		static void Begin()
		{
			SerialController::PacketReceived += new EventHandler<STATICFUNC, const Packet*>(OnPacketReceived);
		}

		static void OnPacketReceived(const Packet* p)
		{
			if (p->CommandId != IN_ACTIVATESCREEN) { return; }
			char* screenName = new char[p->Length + 1];

			for (int i = 0; i < p->Length; i++)
				screenName[i] = p->Data[i];

			screenName[p->Length] = 0;
			ScreenController::ActivateScreen(screenName);
			delete[] screenName;
		}
	};
}
#endif