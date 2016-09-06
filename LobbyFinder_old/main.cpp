#include "dialog.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "Winmm.lib")

Dialog d;

#pragma warning(suppress: 28251)//warnning about inconsistent annotation 'WinMain' in code analysis. probally LPSTR -> LPWSTR

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	return ::DialogBox(hInstance, MAKEINTRESOURCE(janela1), NULL, Proc);
}

INT_PTR CALLBACK Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_INITDIALOG, d.OnInitDialog);
		HANDLE_MSG(hwnd, WM_CLOSE, d.OnClose);

		HANDLE_MSG(hwnd, WM_COMMAND, d.OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, d.OnSize);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, d.OnLButtonDown);

	case WM_TRAYICON: {
		switch (lParam)
		{
		case WM_LBUTTONUP: {
			::ShowWindow(hwnd, SW_SHOWNORMAL);
			::SetForegroundWindow(hwnd);
		}break;

		case WM_RBUTTONUP: {
			d.OnClose(hwnd);
		}break;

		}

		return TRUE;
	}break;	
	
	}

	return FALSE;
}

VOID CALLBACK Timer(HWND hwnd, UINT message, UINT_PTR idEvent, DWORD dwTime) {
	if (!d.getThreadUpdate() &&  ListView_GetItemCount(d.getListView()) > 0) {
		std::thread update = d.callCheckList();
		update.detach();
		::EnableMenuItem(::GetMenu(hwnd), ID_LIST_UPDATELIST, MF_DISABLED);
		d.setThreadUpdate(true);
	}	
}
