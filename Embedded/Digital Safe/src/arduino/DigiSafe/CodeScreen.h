#ifndef __digisafe_codescreen_H__
#define __digisafe_codescreen_H__

#include <bones.h>
#include "SerialCommands.h"

namespace DigiSafe
{
	class CodeScreen : public Bones::Screen
	{
	private:
		const Bones::IEventHandler* packetReceivedHandler;
	private:
		void SetCode(const Bones::Packet* p);
	public:
		CodeScreen(String name) : Screen(name) {}
	public:
		void Activate();
		void Deactivate();
		void Update(bool screenPressed, TSPoint p);
		void GetCode();
		void DrawCode(char code[]);
	public:
		void OnPacketReceived(const Bones::Packet*);
	};
}

#endif