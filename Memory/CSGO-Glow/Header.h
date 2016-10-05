#pragma once
 
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
 
using std::cout;
 
class Glow
{
public:
	DWORD dll, pid, clientDll, localPlayer, glowArray;
	HANDLE handle;
	bool stop = false;
	bool Run();
	int classId(DWORD& add);
	int gl();
	static DWORD rungl();
};
 
extern Glow glow;
 
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
 
struct Offset
{
	DWORD dwLP, iTn,
		bDorm, dwGl;
 
	explicit Offset() :
		dwLP(0xA9D44C), iTn(0xF0),
		bDorm(0xE9), dwGl(0x4B5A7E4){}
};
 
struct glObject
{
	//Thanks to buFFy ( http://www.unknowncheats.me/forum/member461911.html )
	DWORD pEntity;
	float r;
	float g;
	float b;
	float a;
	uint8_t unk1[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	uint8_t unk2[14];
};
