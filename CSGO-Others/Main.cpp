#include "Memory.h"
#include "Findwindow.h"
#include "Cheat.h"


Cheat* t = new Cheat();

int main() {

	Window* w = new Window();
	bool found = false;
	cout << "Waiting game window." << endl;
	do {		
		if (w->isWindowOpen("Valve001"))
			found = true;	
		Sleep(1);
	} while (!found);
	delete w;
	
	if (t->openIt("csgo.exe", PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION)) {
		cout << "Attached!" << endl;
		t->setAttached(true);
		t->setClientDll(t->getDllModule("client.dll", "csgo.exe"));
		cout << "client.dll:\t" << t->getClientDll() << endl;

		std::thread trigger = t->callTrigger(true, false);
		std::thread noflash = t->callNoFlash();
		std::thread bunny = t->callBH();	
		trigger.detach();
		noflash.detach();
		bunny.detach();

		while (true) {

			cout << '\r' << "Trigger:\t" << t->getUseTrigger() << "\tNoFlash:\t" << t->getUseNoFlash() << "\tBunnyhop:\t" << t->getUseBH() << std::flush;

			if (GetAsyncKeyState(VK_DELETE))
				break;
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
