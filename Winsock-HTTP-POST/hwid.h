#pragma once
 
#include <winsock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <Psapi.h>
#include <sstream>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
 
using namespace std;
 
class HwidF{
public:
	static HwidF& Instance(){
		static HwidF hwidf;
		return hwidf;
	}
	void SystemLetter(TCHAR *vol);
	string ID();
};
