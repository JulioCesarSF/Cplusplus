#pragma once

#include <Windows.h>
#include "resource.h"

#define WM_TRAYICON ( WM_USER + 1 )
#define ID_TRAY_APP_ICON   5000


class TrayIcon {
private:
	UINT WM_TASKBAR = 0;
	NOTIFYICONDATA n;
public:

	void init(HWND h) {
		::ZeroMemory(&n, sizeof(NOTIFYICONDATA));
		n.cbSize = sizeof(NOTIFYICONDATA);
		n.hWnd = h;
		n.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

		// {98D19854-D98A-4A6F-B872-F42E07BC867D}
		static const GUID myGUID =
		{ 0x98d19854, 0xd98a, 0x4a6f, { 0xb8, 0x72, 0xf4, 0x2e, 0x7, 0xbc, 0x86, 0x7d } };
		n.guidItem = myGUID;
		n.uID = ID_TRAY_APP_ICON; //just a id
		n.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		n.uCallbackMessage = WM_TRAYICON; //callback
		n.hIcon = (HICON)::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		strcpy_s(n.szTip, "Lobby Finder 0.3 | dmthread");

		::Shell_NotifyIcon(NIM_ADD, &n);
	}

	void del() {
		::Shell_NotifyIcon(NIM_DELETE, &n);
	}

};
