#include "Memory.h"
#include "Findwindow.h"
#include "Cheat.h"


int main() {
	Cheat* t = new Cheat();

	Window* w = new Window();
	bool found = false;
	cout << "Waiting game window." << endl;
	do {		
		if (w->isWindowOpen("Valve001"))
			found = true;	
		Sleep(1);
	} while (!found);

	
	if (t->openIt("csgo.exe", PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION)) {
		cout << "Attached!" << endl;
		t->setAttached(true);
		do {
			t->setClientDll(t->getDllModule("client.dll", "csgo.exe"));
			t->setEngineDll(t->getDllModule("engine.dll", "csgo.exe"));
			::Sleep(50);
		} while (t->getEngineDll() == 0 || t->getClientDll() == 0);
		cout << "client.dll:\t" << t->getClientDll() << endl;
		cout << "engine.dll:\t" << t->getEngineDll() << endl;

		std::thread trigger = t->callTrigger(true, false, true);
		std::thread noflash = t->callNoFlash();
		std::thread bunny = t->callBH();
		std::thread aim = t->callAimbot(true);
		trigger.detach();
		noflash.detach();
		bunny.detach();
		aim.detach();

		found = false;

		while (true) {

			cout << '\r' << "Trigger: " << t->getUseTrigger() 
				<< "\tNoFlash: " << t->getUseNoFlash() 
				<< "\tBunnyhop: " << t->getUseBH() 
				<< "\tAimbot: " << t->getUseAimbot() << std::flush;

			if (GetAsyncKeyState(VK_DELETE) || found)
				break;

			if (!w->isWindowOpen("Valve001"))
				found = true;

			Sleep(50);
		}		
	}
	else
		cout << "Not attached!" << endl;

	if (t->getGameHandle() != NULL &&  t->deAttach(t->getGameHandle()))
		cout << "De attached!" << endl;

	delete t;

	return 0;
}
