#include "Header.h"

/*enabled visual styles
https://msdn.microsoft.com/en-us/library/windows/desktop/bb773175(v=vs.85).aspx
*/
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

struct Controls {
	HWND btn1;
};

Controls controls;

Window* myWin = new Window();

LRESULT CALLBACK Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {

	case WM_CREATE:
	{
		MyButton btn1("Teste1", 50, 24, 50, 50, myWin->getHinst(), hwnd);
		controls.btn1 = btn1.getHwnd();
	}break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY:
	{
		//delete myWin;
		PostQuitMessage(0);		
	}break;

	case WM_COMMAND:
	{		
		switch (HIWORD(wParam)) {
		case BN_CLICKED:
			if ((HWND)lParam == controls.btn1)
				MessageBox(hwnd, "Hello from btn1", "Button1", MB_OK);
		}break;
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
