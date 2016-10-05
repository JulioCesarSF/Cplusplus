#include "Header.h"
 
Glow glow;
Offset of;
 
bool Glow::Run()
{	
	system("cls");
	HANDLE h;
	//get pid by process name
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
 
	h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h == INVALID_HANDLE_VALUE) return false;
 
	if (!Process32First(h, &pe)) { CloseHandle(h); return false; }
 
	do
	{
		if (strcmp(pe.szExeFile, "csgo.exe") == 0)
		{
			this->pid = pe.th32ProcessID;
			break;
		}
	} while (Process32Next(h, &pe));
	CloseHandle(h);
 
	std::cout << "PID OK\n";
 
	//get client dll module
	MODULEENTRY32 me;
	h = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->pid);
	if (h == INVALID_HANDLE_VALUE) return false;
 
	if (!Module32First(h, &me)) { CloseHandle(h); return false; }
 
	do
	{
		if (strcmp(me.szModule, "client.dll") == 0)
		{
			this->clientDll = (DWORD)me.modBaseAddr;
			break;
		}		
	} while (Module32Next(h, &me));
	CloseHandle(h);
 
	std::cout << "client.dll OK\n";
 
	this->handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, this->pid);
	if (this->handle == (HANDLE)ERROR_ACCESS_DENIED || this->handle == 0) return false;
 
	std::cout << "OP OK\n";
 
	this->localPlayer = RPM<DWORD>(this->handle, this->clientDll + of.dwLP, sizeof(DWORD));
	this->glowArray = RPM<DWORD>(this->handle, this->clientDll + of.dwGl, sizeof(DWORD));
	return true;
}
 
int Glow::classId(DWORD& add)
{
	int vt = RPM<int>(this->handle, (add + 0x8), sizeof(int));
	int fn = RPM<int>(this->handle, (vt + 2 * 0x4), sizeof(int));
	int cls = RPM<int>(this->handle, (fn + 0x1), sizeof(int));
	int classID = RPM<int>(this->handle, (cls + 20), sizeof(int));
	return classID;
}
 
int Glow::gl()
{	
	while (true)
	{
		Sleep(1);
 
		int myTeam=0, entTeam=0, glowCount=0;
 
		if (this->stop)
			break;
 
		glowCount = RPM<int>(this->handle, this->clientDll + of.dwGl + 0x4, sizeof(int));
		if (glowCount == 0)
			continue;		
 
		glObject* g = new glObject[glowCount];
 
		ReadProcessMemory(glow.handle, (LPCVOID)this->glowArray, g, glowCount * sizeof(glObject), 0);
 
		for (int i = 0; i < glowCount; ++i)
		{
			if (g[i].pEntity == 0)
				continue;
 
			if (this->classId(g[i].pEntity) != 35)
				continue;
 
			bool dormant = RPM<bool>(this->handle, g[i].pEntity + of.bDorm, sizeof(bool));
			if (dormant)
				continue;
 
			myTeam = RPM<int>(this->handle, this->localPlayer + of.iTn, sizeof(int));
			entTeam = RPM<int>(this->handle, g[i].pEntity + of.iTn, sizeof(int));
 
			if (entTeam == myTeam)
				continue;
 
			g[i].r = 1.0f;
			g[i].g = 0.0f;
			g[i].b = 0.0f;
			g[i].a = 1.0f;
			g[i].m_bRenderWhenOccluded = true;
		}
 
		WriteProcessMemory(this->handle, (LPVOID)this->glowArray, g, glowCount * sizeof(glObject), 0);
		delete[] g;
	}
	return 0;
}
 
DWORD Glow::rungl()
{
	glow.gl();
 
	return 0;
}
 
int main(int argc, const char* argv[])
{
 
	cout << "CS:GO Glow dmThread\n" << "[INSERT] to ON/OFF\n";
 
	HANDLE thread = 0;
	DWORD wait = WAIT_ABANDONED;
 
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 0x8000)
		{
			if (!thread)
			{
				if (glow.Run() && wait != WAIT_FAILED)
				{
					glow.stop = false;
					thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&glow.rungl, NULL, 0, NULL);
					cout << "GLOW ON\n";
				}
				else
					cout << "Could not start glow thread\n";
			}
			else
			{
				glow.stop = true;
				wait = WaitForSingleObject(thread, INFINITE);
				if (FindWindow("Valve001", NULL) && wait != WAIT_FAILED)
					CloseHandle(glow.handle);
				thread = 0;
				cout << "GLOW OFF\n";
			}
 
			Sleep(150);
		}
 
		if (GetAsyncKeyState(VK_DELETE))
			break;
 
		Sleep(1);
	}
 
	if (thread)
	{
		glow.stop = true;
		wait  = WaitForSingleObject(thread, INFINITE);
		if(FindWindow("Valve001", NULL) && wait != WAIT_FAILED)
			CloseHandle(glow.handle);
	}
	return 0;
}
