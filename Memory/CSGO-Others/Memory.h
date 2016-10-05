#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using std::cout;
using std::endl;

template<typename TYPE>
TYPE RPM(HANDLE proc, DWORD address, SIZE_T size) {
	TYPE buffer = TYPE();
	::ReadProcessMemory(proc, (LPCVOID)address, &buffer, size, 0);
	return buffer;
}

template<typename TYPE>
BOOL WPM(HANDLE proc, DWORD address, TYPE dataToWrite) {
	TYPE buffer = dataToWrite;
	return ::WriteProcessMemory(proc, (LPVOID)address, &buffer, sizeof(buffer), 0);
}

class Memory {

private:
	DWORD pid, clientDll;
	HANDLE gameHandle;
	bool attached = false;

public:

	Memory() {
		pid = 0;
		clientDll = 0;
		gameHandle = NULL;
	}

	DWORD getPidProcess(LPSTR lpProcessName) {

		HANDLE h;
		DWORD pid = 0;

		PROCESSENTRY32 pe32;

		pe32.dwSize = sizeof(PROCESSENTRY32);

		h = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (h == INVALID_HANDLE_VALUE)
			return pid;

		if (!::Process32First(h, &pe32)) {
			::CloseHandle(h);
			return pid;
		}

		do {
			if (strcmp(pe32.szExeFile, lpProcessName) == 0) {
				this->pid = pe32.th32ProcessID;
				pid = this->pid;
				break;
			}
		} while (::Process32Next(h, &pe32));

		::CloseHandle(h);

		return pid;
	}

	DWORD getDllModule(LPSTR lpDllName, LPSTR lpProcessName) {

		HANDLE h;
		DWORD dll = 0;

		MODULEENTRY32 me32;

		me32.dwSize = sizeof(MODULEENTRY32);

		h = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, getPidProcess(lpProcessName));

		if (h == INVALID_HANDLE_VALUE) {
			::CloseHandle(h);
			return dll;
		}

		if (!::Module32First(h, &me32)) {
			::CloseHandle(h);
			return dll;
		}

		do {
			if (strcmp(me32.szModule, lpDllName) == 0) {
				dll = (DWORD)me32.modBaseAddr;
				break;
			}
		} while (::Module32Next(h, &me32));

		::CloseHandle(h);

		return dll;
	}

	BOOL openIt(LPSTR lpProcessName, DWORD flags) {		
		if (attach(flags, getPidProcess(lpProcessName)))
			return TRUE;
		return FALSE;
	}

	BOOL attach(DWORD flags, DWORD processPid) {
		gameHandle = ::OpenProcess(flags, FALSE, processPid);

		if (gameHandle == (HANDLE)ERROR_ACCESS_DENIED || gameHandle == 0) {
			attached = true;
			return FALSE;
		}
		attached = false;
		return TRUE;
	}

	BOOL deAttach(HANDLE handle) {
		if (::CloseHandle(handle)) {
			gameHandle = NULL;
			return TRUE;
		}

		return FALSE;
	}

	void setPid(DWORD pid) {
		this->pid = pid;
	}

	void setClientDll(DWORD address) {
		this->clientDll = address;
	}

	void setGameHandle(HANDLE handle) {
		this->gameHandle = handle;
	}

	void setAttached(bool attached) {
		this->attached = attached;
	}

	DWORD getPid() {
		return this->pid;
	}

	DWORD getClientDll() {
		return this->clientDll;
	}

	HANDLE getGameHandle() {
		return this->gameHandle;
	}

	bool getAttached() {
		return this->attached;
	}

};
