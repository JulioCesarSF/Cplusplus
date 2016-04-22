#pragma once

#include<gdiplus.h>

#pragma comment(lib,"user32.lib") 
#pragma comment(lib, "gdi32.lib")
#pragma comment (lib, "gdiplus.lib")

using namespace Gdiplus;

/*Timer callback*/
VOID CALLBACK Timer(HWND, UINT, UINT_PTR, DWORD);

class Game {
private:
	int x, y, score, width, height, seconds = 60;
	HDC hdc;
	RECT cube = { 0, 0, 20, 20 };
	RECT r;
	HRGN cubeRegion;
	bool started = false;
	UINT timer;
public:

	int getX() {
		return x;
	}
	int getY() {
		return y;
	}

	void setScore() {
		score++;
	}

	int getScore() {
		return score;
	}

	void setStarted(bool s) {
		started = s;
	}

	bool getStarted() {
		return started;
	}

	void setSeconds() {
		seconds--;
	}

	HRGN getCubeRegion() {
		return cubeRegion;
	}

	void deleteCubeRegion() {
		::DeleteObject(cubeRegion);
	}

	void setupGame(HWND h) {		
		::GetClientRect(h, &r);
		width = r.right - r.left;
		height = r.bottom - r.top;		
		x = 0;
		y = 0;
	}

	void startGame() {
		score = 0;
		seconds = 10;
		startCube();
		setTimer(1000);
	}

	void setTimer(int mSeconds) {
		timer = ::SetTimer(0, 0, mSeconds, Timer);
	}

	void killTimer() {
		if (seconds == 0) {
			::KillTimer(0, timer);
			started = false;
		}
	}

	void startCube() {	
		do {
			x = rand() % width;
			::Sleep(1);
		} while ((x + (cube.right - cube.left)) > width);
		
		do {
			y = rand() % height;
			::Sleep(1);
		} while ((y + (cube.bottom - cube.top)) > height);


		cubeRegion = ::CreateRectRgn(x, y, x + (cube.right - cube.left), y + (cube.bottom - cube.top));
	}

	void draw(HWND h) {
		hdc = ::GetDC(h);

		/*d-buff*/
		HBITMAP backBuffer = ::CreateCompatibleBitmap(hdc, width, height);
		HDC backBufferDC = ::CreateCompatibleDC(hdc);
		::SelectObject(backBufferDC, backBuffer);

		/* draw with GDI+ */
		Graphics g(backBufferDC);

		Font font(&FontFamily(L"Arial"), 12);
		LinearGradientBrush brush(Rect(0, 0, 100, 100), Color::Red, Color::Yellow, LinearGradientModeHorizontal);

		/*score*/
		std::wstring text = L"Score: ";
		text.append(std::to_wstring(score));
		std::wstring textTime = L"Time: ";
		textTime.append(std::to_wstring(seconds));
		textTime.append(L"s");

		if (started) {		
			g.DrawString(text.c_str(), -1, &font, PointF(0, 0), &brush);
			g.DrawString(textTime.c_str(), -1, &font, PointF(100, 0), &brush);
			Pen pen(Color(255, 0, 255, 0));
			g.DrawRectangle(&pen, x, y, cube.right - cube.left, cube.bottom - cube.top);			
		}
		else {					
			g.DrawString(L"Hit SPACE to Start/Reset", -1, &font, PointF(width/2 -100, height/2), &brush);
			if (score > 0) 
				g.DrawString(text.c_str(), -1, &font, PointF(0, 0), &brush);
		}
		
		/* transfer what we draw to window (hdc) */
		::BitBlt(hdc, 0, 0, width, height, backBufferDC, 0, 0, SRCCOPY);

		/* release hdc, delete backbufferDC e delete the bitmap*/
		::ReleaseDC(h, hdc);
		::DeleteDC(backBufferDC);
		::DeleteObject(backBuffer);
	}	

};
