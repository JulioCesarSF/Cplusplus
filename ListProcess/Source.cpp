#include "Header.h"

int main() {

	Process* p = new Process();

	p->listProcess();
	
	delete p;

	return 0;
}
