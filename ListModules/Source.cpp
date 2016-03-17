#include "Header.h"

int main() {

	Process* p = new Process();

	/* first get pid */

	cout << "PID for explorer: " << p->getProcessPid("AsusAudioCenter.exe") << endl;

	/* second list modules by process id */

	if (!p->listModules(p->getPid()))
		cout << "Could not get modules!" << endl;

	delete p;

	return 0;
}
