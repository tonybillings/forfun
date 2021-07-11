#ifndef __digisafe_mainscreen_H__
#define __digisafe_mainscreen_H__

#include <bones.h>
#include "Logo.h"
#include "SerialCommands.h"

namespace DigiSafe
{
	class MainScreen : public Bones::Screen
	{
	private:
		bool						ipVisible;
		char*						ip;
		DigiSafeLogo				logo;
		const Bones::IEventHandler*	packetReceivedHandler;
	public:
		MainScreen(String name) : ipVisible(false), Screen(name) {}
	private:
		void GetIp();
		void SetIp(const Bones::Packet* p);
		void DrawIp();
		void DrawLogo();
	public:
		void Activate();
		void Deactivate();
		void Update(bool screenPressed, TSPoint point);
	public:
		void OnPacketReceived(const Bones::Packet*);
	};
}

#endif

