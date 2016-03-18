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
		//wce.style = CS_HREDRAW | CS_VREDRAW;
		wce.hInstance = this->getHinst();
		wce.lpfnWndProc = Proc;
		wce.cbClsExtra = 0;
		wce.lpszClassName = this->getClassName();
		wce.hbrBackground = (HBRUSH)(BLACK_BRUSH);
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
		this->setHwnd(CreateWindowEx(WS_EX_COMPOSITED, this->getClassName(), this->getWindowName(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 700, 500, NULL, NULL, this->getHinst(), NULL));

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

