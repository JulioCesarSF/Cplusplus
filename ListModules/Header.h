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

	bool listModules(DWORD pid) {

		MODULEENTRY32 me32;
		HANDLE h;
		
		me32.dwSize = sizeof(MODULEENTRY32);

		h = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

		if (h == INVALID_HANDLE_VALUE)
			return false;

		BOOL have = Module32First(h, &me32);

		if (have) {
			do {
				cout << "Module: " << me32.szModule << endl;
				have = Module32Next(h, &me32);
			} while (have);

		}
		else {
			CloseHandle(h);
			return false;
		}

		CloseHandle(h);

		return true;
	}

	DWORD getProcessPid(LPSTR lpProcessName) {
		DWORD p = 0;
		PROCESSENTRY32 pe32;
		HANDLE h;

		pe32.dwSize = sizeof(PROCESSENTRY32);
		h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (h == INVALID_HANDLE_VALUE)
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

		this->pid = p;  /* you don't need to use var pid*/
		
		CloseHandle(h);

		return this->pid;

	}

	DWORD getPid() {
		return this->pid;
	}

};
