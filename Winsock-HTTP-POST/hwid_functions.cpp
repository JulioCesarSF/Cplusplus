#include "hwid.h"
 
void HwidF::SystemLetter(TCHAR *vol){
	TCHAR buffer[30];
	GetSystemWindowsDirectory(buffer, 30);
	for (int i = 0; i < 2; i++)
		vol[i] = buffer[i];
	vol[2] = 0;
}
 
string HwidF::ID(){
	TCHAR vol[3];
	SystemLetter(vol);
	string letter = string(vol) + "\\";
	string ss;
	ss = "Err_StringIsNull";
	UCHAR szFileSys[255],
		szVolNameBuff[255];
	DWORD dwSerial;
	DWORD dwMFL;
	DWORD dwSysFlags;
	int error = 0;
	BOOL success = GetVolumeInformation(LPCTSTR(letter.c_str()), (LPTSTR)szVolNameBuff,
		255, &dwSerial,
		&dwMFL, &dwSysFlags,
		(LPTSTR)szFileSys,
		255);
	if (!success) {
		ss = "Err_Not_Elevated";
	}
	std::stringstream errorStream;
	errorStream << dwSerial;
	return string(errorStream.str().c_str());
}
