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
		//t->workTrigger(true, false);
		//t->workNoFlash();
		//t->workBH();
	}
	else
		cout << "Not attached!" << endl;

	if (t->getGameHandle() != NULL &&  t->deAttach(t->getGameHandle()))
		cout << "De attached!" << endl;

	delete t;

	return 0;
}
