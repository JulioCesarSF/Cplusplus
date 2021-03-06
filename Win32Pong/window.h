#pragma once
#include <Windows.h>
#include "pong.h"

/* callback for messages */
LRESULT CALLBACK Proc(HWND, UINT, WPARAM, LPARAM);


class Window {
private:
	HINSTANCE hInst;
	MSG msg;
	LPSTR windowName = "C++ Win32 Pong by Julio Cesar Schincariol Filho";
	LPSTR className = "pingpong";
	WNDCLASSEX wce;
	HWND hwnd;
	int width, height;
public:

	void setWidth(int w) {
		width = w;
	}

	void setHeight(int h) {
		height = h;
	}

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

	int getWidth() {
		return this->width;
	}

	int getHeight() {
		return this->height;
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
		//wce.style = CS_HREDRAW | CS_VREDRAW;
		this->wce.hInstance = this->getHinst();
		this->wce.lpfnWndProc = Proc;
		this->wce.cbClsExtra = 0;
		this->wce.lpszClassName = this->getClassName();
		this->wce.hbrBackground = (HBRUSH)(BLACK_BRUSH);
		this->wce.hIcon = LoadIcon(this->getHinst(), MAKEINTRESOURCE(IDI_APPLICATION));
		this->wce.hCursor = LoadCursor(NULL, IDC_ARROW);
		this->wce.cbWndExtra = 0;
		this->wce.hIconSm = LoadIcon(wce.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		this->wce.lpszMenuName = NULL;

		if (!RegisterClassEx(&this->wce))
			return false;
		else
			return true;
	}

	bool createWindow() { //WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU
		this->setHwnd(CreateWindowEx(WS_EX_COMPOSITED, this->getClassName(), this->getWindowName(),
			WS_POPUP | WS_VISIBLE,
			(this->getWidth() /2), (this->getHeight() /2), this->getWidth(), this->getHeight(), NULL, NULL, this->getHinst(), NULL));

		if (!this->getHinst()) 
			return false;		
		else
			return true;
	}

	void show(int iCmdShow) {
		ShowWindow(this->getHwnd(), iCmdShow);
		UpdateWindow(this->getHwnd());
	}

	int messages(HWND hwnd, Jogador* jogador1, Jogador* jogador2, Pong* pong) {
		while (true) {

			srand(GetTickCount());

			while (PeekMessage(&this->msg, 0, 0, 0, PM_REMOVE)) {
				TranslateMessage(&this->msg);
				DispatchMessage(&this->msg);
			}

			if (this->msg.message == WM_QUIT)
				break;

			pong->setTime(GetTickCount());

			while (GetTickCount() - pong->getTime() < 5);

			pong->input(jogador1, jogador2);
			pong->moverBola(jogador1, jogador2);

			InvalidateRect(hwnd, NULL, true);
		}

		return (int)this->msg.wParam;
	}	

};

