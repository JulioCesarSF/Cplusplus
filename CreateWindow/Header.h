#pragma once
#include <Windows.h>

/* callback for messages */
LRESULT CALLBACK Proc(HWND, UINT, WPARAM, LPARAM);


class Window {
private:
	HINSTANCE hInst;
	MSG msg;
	LPSTR windowName = "Teste";
	LPSTR className = "class";
	WNDCLASSEX wce;
	HWND hwnd;
public:

	void setHinst(HINSTANCE hInst) {
		this->hInst = hInst;
	}

	void setWindowName(LPSTR lpWindowName) {
		this->windowName = lpWindowName;
	}

	void setClassName(LPSTR lpClassName) {
		this->className = lpClassName;
	}

	void setHwnd(HWND hwnd) {
		this->hwnd = hwnd;
	}

	HWND getHwnd() {
		return this->hwnd;
	}

	LPSTR getWindowName() {
		return this->windowName;
	}

	LPSTR getClassName() {
		return this->className;
	}
	
	HINSTANCE getHinst() {
		return this->hInst;
	}

	bool setWCE() {
		this->wce.cbSize = sizeof(WNDCLASSEX);
		wce.style = CS_HREDRAW | CS_VREDRAW;
		wce.hInstance = this->getHinst();
		wce.lpfnWndProc = Proc;
		wce.cbClsExtra = 0;
		wce.lpszClassName = this->getClassName();
		wce.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wce.hIcon = LoadIcon(this->getHinst(), MAKEINTRESOURCE(IDI_APPLICATION));
		wce.hCursor = LoadCursor(NULL, IDC_ARROW);
		wce.cbWndExtra = 0;
		wce.hIconSm = LoadIcon(wce.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wce.lpszMenuName = NULL;

		if (!RegisterClassEx(&wce))
			return false;
		else
			return true;
	}

	bool createWindow() {
		this->setHwnd(CreateWindow(this->getClassName(), this->getWindowName(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, this->getHinst(), NULL));

		if (!this->getHinst()) 
			return false;		
		else
			return true;
	}

	void show(int iCmdShow) {
		ShowWindow(this->getHwnd(), iCmdShow);
		UpdateWindow(this->getHwnd());
	}

	int messages() {
		while (GetMessage(&this->msg, NULL, 0, 0)) {
			TranslateMessage(&this->msg);
			DispatchMessage(&this->msg);
		}

		return (int)this->msg.wParam;
	}	

};
