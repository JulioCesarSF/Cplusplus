#include "Header.h"

Window* myWin = new Window();

LRESULT CALLBACK Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {

	case WM_CREATE:
	{
		MessageBox(NULL, "teste", "teste", MB_ICONINFORMATION);
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);		
	}break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	myWin->setHinst(hInstance);

	if (!myWin->setWCE())
		return 1;

	if (!myWin->createWindow())
		return 1;

	myWin->show(nCmdShow);

	return myWin->messages();
}
