#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

using std::cout;
using std::endl;

class Process {

private:
	DWORD pid;

public:

	DWORD getPid(LPSTR lpProcessName) {
		DWORD p = 0;
		PROCESSENTRY32 pe32;
		HANDLE h;

		pe32.dwSize = sizeof(PROCESSENTRY32);
		h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!h)
			return p;

		BOOL have = Process32First(h, &pe32);

		if (have) {

			do {
				if (strcmp(pe32.szExeFile, lpProcessName) == 0) {
					p = pe32.th32ProcessID;
					break;					
				}
				have = Process32Next(h, &pe32);
			} while (have);
		}
		else {
			CloseHandle(h);
			return p;
		}

		this->pid = p;

		return this->pid;

	}

};
