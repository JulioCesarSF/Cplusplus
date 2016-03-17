#include "Header.h"

int main() {

	Process* p = new Process();

	cout << "PID for explorer: " << p->getPid("explorer.exe") << endl;
	
	delete p;

	return 0;
}
