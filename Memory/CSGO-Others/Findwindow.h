#pragma once
#include <Windows.h>

class Window {

private:
	LPSTR windowClassName;

public:

	Window() {}
	Window(LPSTR lpWindowClassName) {
		this->windowClassName = lpWindowClassName;
	}

	BOOL isWindowOpen(LPSTR lpClass) {
		if (::FindWindow(lpClass, NULL))
			return TRUE;
		return FALSE;
	}

	void setWindowClassName(LPSTR lpClass) {
		this->windowClassName = lpClass;
	}

	LPSTR getWindowClassName() {
		return this->windowClassName;
	}

};
