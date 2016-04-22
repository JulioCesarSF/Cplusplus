#pragma once

#include <Windows.h>
#include <string>
#include "Game.h"

/*callback for messages*/
LRESULT CALLBACK Proc(HWND, UINT, WPARAM, LPARAM);

class Window : public Game {
private:
	HWND hwnd;
	HINSTANCE hInstance;
	int width, height;
	LPSTR title, className;
	MSG msg;
public:

	Window() {
		width = 500;
		height = 500;
		title = "Reflex Game Teste";
		className = "cls_reflexGame";
	}

	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}

	bool setup(HINSTANCE hInst, int nCmdShow) {
		WNDCLASSEX w;
		w.cbSize = sizeof(WNDCLASSEX);
		w.cbClsExtra = 0;
		w.cbWndExtra = 0;
		w.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		w.hCursor = LoadCursor(NULL, IDC_CROSS);
		w.hIcon = LoadIcon(hInst, IDI_APPLICATION);
		w.hIconSm = NULL;
		w.hInstance = hInst;
		w.lpfnWndProc = Proc;
		w.lpszClassName = className;
		w.lpszMenuName = NULL;
		w.style = CS_HREDRAW | CS_VREDRAW;

		if (!::RegisterClassEx(&w))
			return false;

		/* client area size */
		RECT r = { 0, 0, width, height };
		::AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
		int ww = r.right - r.left;
		int hh = r.bottom - r.top;

		hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, className, title, WS_OVERLAPPED | WS_SYSMENU,
			0, 0, ww, hh, NULL, NULL, hInst, NULL);

		if (hwnd == NULL)
			return false;		

		::ShowWindow(hwnd, nCmdShow);
		::UpdateWindow(hwnd);

		return true;
	}

	int messages() {
		bool running = true;

		const int FRAMES_PER_SECOND = 30;
		const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

		DWORD next_game_tick = GetTickCount();

		int sleep_time = 0;

		while (running) {

			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT || ::GetAsyncKeyState(VK_ESCAPE))
				running = false;
			else {

				next_game_tick += SKIP_TICKS;
				sleep_time = next_game_tick - GetTickCount();
				if (sleep_time >= 0) {
					::Sleep(sleep_time);
				}

				draw(hwnd);
				killTimer();
			}
		}

		return (int)msg.message;
	}

};
