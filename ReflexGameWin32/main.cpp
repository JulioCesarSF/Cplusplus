#include "window.h"

ULONG_PTR gdiToken;

Window* w = new Window();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	::GdiplusStartupInput gdiInput;

	Status r = ::GdiplusStartup(&gdiToken, &gdiInput, NULL);

	if (r != Ok)
		return 1;

	if (!w->setup(hInstance, nCmdShow))
		return 1;

	return w->messages();
}

LRESULT CALLBACK Proc(HWND h, UINT m, WPARAM wParam, LPARAM lParam) {
	switch (m)
	{

	case WM_CREATE: {
		RECT r;
		::GetWindowRect(h, &r);
		int x = (::GetSystemMetrics(SM_CXSCREEN) - r.right) / 2;
		int y = (::GetSystemMetrics(SM_CYSCREEN) - r.bottom) / 2;
		::SetWindowPos(h, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		w->setupGame(h);		
	}break;

	case WM_LBUTTONUP: {
		int xPos = (int)LOWORD(lParam);
		int yPos = (int)HIWORD(lParam);
		if (::PtInRegion(w->getCubeRegion(), xPos, yPos) != 0) {			
			w->setScore();
			w->deleteCubeRegion();
			w->startCube();
		}		
	}break;	

	case WM_KEYUP: {
		switch (wParam)
		{
		case VK_SPACE: {
			w->setStarted(true);
			w->startGame();
		}break;
		}
	}break;

	case WM_DESTROY:		
		::GdiplusShutdown(gdiToken);
		::PostQuitMessage(0);
		break;

	default:
		break;
	}

	return ::DefWindowProc(h, m, wParam, lParam);
}

VOID CALLBACK Timer(HWND h, UINT m, UINT_PTR idEvent, DWORD dwTime) {
	w->setSeconds();
}
