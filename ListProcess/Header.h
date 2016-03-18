#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

using std::cout;
using std::endl;

class Process {

public:

	bool listProcess() {
		
		PROCESSENTRY32 pe32;
		HANDLE h;

		pe32.dwSize = sizeof(PROCESSENTRY32);
		h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (h == INVALID_HANDLE_VALUE)
			return false;

		BOOL have = Process32First(h, &pe32);

		if (have) {

			do{
				cout << "Process name: " << pe32.szExeFile << endl;
				have = Process32Next(h, &pe32);
			} while (have);
		}
		else {
			CloseHandle(h);
			return false;
		}
		CloseHandle(h);

		return true;

	}

};
