#include <Windows.h>
#include <windowsx.h>
#include <iostream>
 
class Window{
public:	
	WNDCLASSEX wcx;
	HWND hwnd;
	MSG msg;
	bool gameFound = false;
 
	bool RegClassEx(HINSTANCE hInst); //wndclassex
	bool CreateMyWindow(HINSTANCE hInst); //registerclassex
	int MyMsg(); //process window msgs - this run the program
 
	//buttons
	static const int IDC_MAIN_BUTTON = 101;
	
}; Window MyWnd;
 
class ScanWindow{
public:	
	WNDCLASSEX wcx;
	HWND hwnd;
	MSG msg;
	HDC hdc = NULL, rectHdc = NULL;
	RECT rect;
 
	bool RegClassExScan(HINSTANCE hInst); //wndclassex
	bool CreateMyWindowScan(HINSTANCE hInst); //registerclassex
	int MyMsgScan(); //process window msgs - this run the program
 
	void PixelScanner(); // scan for color using GetPixel
 
	bool p, enableRectDraw = false;
	POINT point, ptPrevious;
	int xM, yM;
	int xC, yC;
 
}; ScanWindow ScanWnd;
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //process all window msgs	
LRESULT CALLBACK WndProcScan(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //process all window msgs	
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(szCmdLine);
	UNREFERENCED_PARAMETER(iCmdShow);
	//Main window
	if (!MyWnd.RegClassEx(hInstance)) return 1;
	if (!MyWnd.CreateMyWindow(hInstance)) return 1;
	ShowWindow(MyWnd.hwnd, SW_SHOW);
	UpdateWindow(MyWnd.hwnd);
 
	//ScanWindow
	if (!ScanWnd.RegClassExScan(hInstance)) return 1;
	if (!ScanWnd.CreateMyWindowScan(hInstance)) return 1;
 
	//register a hot key to exit program
	if (!RegisterHotKey(MyWnd.hwnd, 100, NULL, VK_END)){ MessageBox(NULL, "RegisterHotKey fail. No Exit key", "RegisterHotKey", MB_OK | MB_ICONERROR); }
	
	return MyWnd.MyMsg();
}
 
/*
MAIN WINDOW
 
 
*/
 
bool Window::RegClassEx(HINSTANCE hInst){
	ZeroMemory(&wcx, sizeof(WNDCLASSEX));
	MyWnd.wcx.cbSize = sizeof(WNDCLASSEX);
	MyWnd.wcx.style = CS_HREDRAW | CS_VREDRAW;
	MyWnd.wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	MyWnd.wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	MyWnd.wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	MyWnd.wcx.lpszClassName = "main";
	MyWnd.wcx.lpszMenuName = NULL;
	MyWnd.wcx.hInstance = hInst;
	MyWnd.wcx.lpfnWndProc = WndProc;
 
	if (!RegisterClassEx(&wcx)){ MessageBox(NULL, "RegisterClassEx", "RegisterClassEx", MB_OK | MB_ICONERROR); return false; }
	return true;
}
 
bool Window::CreateMyWindow(HINSTANCE hInst){
	const int x = GetSystemMetrics(SM_CXSCREEN) / 2 - 175;
	const int y = GetSystemMetrics(SM_CYSCREEN) / 2 - 175;
 
	MyWnd.hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		"main",
		"Hello",
		WS_OVERLAPPEDWINDOW,
		x, y, 350, 100,
		NULL, NULL, hInst, NULL);
	if (MyWnd.hwnd == NULL) { MessageBox(NULL, "CreateWindowEx", "CreateWindowEx", MB_OK | MB_ICONERROR); return false; }
	return true;
}
 
int Window::MyMsg(){
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg){
 
	case WM_CREATE:{
		//setup scan window button
		HWND hWndButton = CreateWindowEx(NULL,
			"BUTTON",
			"Setup Scan &Window",
			WS_TABSTOP | WS_VISIBLE |
			WS_CHILD | BS_DEFPUSHBUTTON,
			10,
			10,
			155,
			24,
			hwnd,
			(HMENU)MyWnd.IDC_MAIN_BUTTON,
			GetModuleHandle(NULL),
			NULL);
		
	}break;
 
	case WM_COMMAND:{
		switch (LOWORD(wParam)){
		case MyWnd.IDC_MAIN_BUTTON:{			
			ShowWindow(ScanWnd.hwnd, SW_SHOW);
			//UpdateWindow(ScanWnd.hwnd);
		}break;
		
		}
	}break;
 
	case WM_CLOSE:{		
		DestroyWindow(MyWnd.hwnd);
	}break;
 
	case WM_DESTROY:{
		PostQuitMessage(0);
	}break;
 
	case WM_HOTKEY:{
		if (MyWnd.msg.wParam == 100){ ExitProcess(1); }
	}break;
 
 
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
 
/*
SCAN WINDOW
 
 
*/
 
bool ScanWindow::RegClassExScan(HINSTANCE hInst){
 
	ZeroMemory(&wcx, sizeof(WNDCLASSEX));
	ScanWnd.wcx.cbSize = sizeof(WNDCLASSEX);
	//ScanWnd.wcx.style = CS_HREDRAW | CS_VREDRAW; //stop flicker
	ScanWnd.wcx.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	ScanWnd.wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	ScanWnd.wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	ScanWnd.wcx.lpszClassName = "scan";
	ScanWnd.wcx.lpszMenuName = NULL;
	ScanWnd.wcx.hInstance = hInst;
	ScanWnd.wcx.lpfnWndProc = WndProcScan;
 
	if (!RegisterClassEx(&wcx)){ MessageBox(NULL, "RegisterClassEx", "RegisterClassEx ScanWindow", MB_OK | MB_ICONERROR); return false; }
	return true;
}
 
bool ScanWindow::CreateMyWindowScan(HINSTANCE hInst){
	const int x = GetSystemMetrics(SM_CXSCREEN) / 2 - 300;
	const int y = GetSystemMetrics(SM_CYSCREEN) / 2 - 300;
 
	ScanWnd.hwnd = CreateWindowEx(
		WS_EX_TOPMOST, // remove WS_EX_LAYERED to see the application effect ;)
		"scan",
		"ScanWindow",
		WS_OVERLAPPEDWINDOW,
		x, y, 600, 400,
		NULL, NULL, hInst, NULL);
	if (ScanWnd.hwnd == NULL) { MessageBox(NULL, "CreateWindowEx", "CreateWindowEx", MB_OK | MB_ICONERROR); return false; }
 
	return true;
}
 
int ScanWindow::MyMsgScan(){
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
 
LRESULT CALLBACK WndProcScan(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg){
 
	case WM_CREATE:{
		//SetWindowLong(ScanWnd.hwnd, GWL_EXSTYLE, GetWindowLong(ScanWnd.hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
	}break;
 
	case WM_CLOSE:{
		DestroyWindow(hwnd);
	}break;
 
	case WM_DESTROY:{
		PostQuitMessage(0);
	}break;
 
	case WM_PAINT:{
 
		PAINTSTRUCT ps;
		ScanWnd.hdc = BeginPaint(hwnd, &ps);
		HPEN	   hPen;
		hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(255, 0, 0));
		SelectObject(ScanWnd.hdc, hPen);		
		
		GetClientRect(hwnd, &ScanWnd.rect);
		ScanWnd.xC = ScanWnd.rect.right - ScanWnd.rect.left; // total Horizontal line size
		ScanWnd.yC = ScanWnd.rect.bottom - ScanWnd.rect.top; // total verstical line size
 
		/*
		LINES
		*/
		//Line X		
		MoveToEx(ScanWnd.hdc, ScanWnd.xM, 0, NULL); //start Horizontal line (Total horizontal - mousepos = line horizontal start			
		LineTo(ScanWnd.hdc, ScanWnd.xM, ScanWnd.yC); //end point to horizontal line		
		DeleteObject(hPen);
 
		//Line Y 
		MoveToEx(ScanWnd.hdc, 0, ScanWnd.yM, NULL); //start Vertical line
		LineTo(ScanWnd.hdc, ScanWnd.xC, ScanWnd.yM); //end point to vertical line
 
		//Coord text
		RECT textRect = {0, 0, 100, 25 };
		char buffer[512];
		sprintf_s(buffer, "x: %i, y: %i", ScanWnd.xM, ScanWnd.yM);
		SetBkMode(ScanWnd.hdc, TRANSPARENT);
		DrawText(ScanWnd.hdc, buffer, -1, &textRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		
		EndPaint(hwnd, &ps);	
	}break;
 
	case WM_MOUSEMOVE:{
		ScanWnd.xM = GET_X_LPARAM(lParam);
		ScanWnd.yM = GET_Y_LPARAM(lParam);		
		InvalidateRect(hwnd, &ScanWnd.rect, TRUE);		
	}break;	
 
	case WM_LBUTTONDOWN:{
		ScanWnd.enableRectDraw = true;
		ScanWnd.ptPrevious.x = GET_X_LPARAM(lParam);
		ScanWnd.ptPrevious.y = GET_Y_LPARAM(lParam);		
	}break;
 
	case WM_LBUTTONUP:{
		if (ScanWnd.enableRectDraw){	
 
			ScanWnd.rectHdc = GetDC(hwnd);			
			
			HPEN	   hPen;
			hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
			SelectObject(ScanWnd.rectHdc, hPen);
			SetBkMode(ScanWnd.rectHdc, TRANSPARENT);
			Rectangle(ScanWnd.rectHdc, ScanWnd.ptPrevious.x, ScanWnd.ptPrevious.x, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
 
			DeleteObject(hPen);			
			ReleaseDC(hwnd, ScanWnd.rectHdc);
			
		}
		ScanWnd.enableRectDraw = false;
	}break;
 
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
