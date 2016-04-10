#include "window.h"
#include "pong.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

Window* myWin = new Window();
Pong* pong = new Pong();
Jogador* jogador1 = new Jogador();
Jogador* jogador2 = new Jogador();

LRESULT CALLBACK Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {

	case WM_CREATE:
	{
		pong->setup(jogador1, jogador2);
		SetFocus(hwnd);
	}break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		/* linha no meio */
		SelectObject(hdc, pong->getVermelho());
		MoveToEx(hdc, 350, 0, NULL);
		LineTo(hdc, 350, 500);

		/* desenhar bola */
		SelectObject(hdc, pong->getBranco());
		SelectObject(hdc, pong->getPreto());
		Ellipse(hdc, pong->getBolaPosX() - 9, pong->getBolaPosY() - 9,
			pong->getBolaPosX() + 9, pong->getBolaPosY() + 9);

		/* desenhar jogador1 */
		SelectObject(hdc, pong->getHAzul());
		SelectObject(hdc, pong->getAzul());
		Rectangle(hdc, jogador1->getposX() - 5, jogador1->getposY() - 30,
			jogador1->getposX() + 5, jogador1->getposY() + 30);

		/* desenhar jogador2 */
		SelectObject(hdc, pong->getHVerde());
		SelectObject(hdc, pong->getVerde());
		Rectangle(hdc, jogador2->getposX() - 5, jogador2->getposY() - 30,
			jogador2->getposX() + 5, jogador2->getposY() + 30);	

		SetBkMode(hdc, TRANSPARENT);	
		SelectObject(hdc, pong->getScoreFont());
		/* desenhar score jogador 1 */		
		TextOut(hdc, 680, 10, jogador1->getScoreString().c_str(), jogador1->getScoreString().length());

		/* desenhar score jogador 2 */		
		TextOut(hdc, 10, 10, jogador2->getScoreString().c_str(), jogador2->getScoreString().length());

		EndPaint(hwnd, &ps);

	}break;	

	case WM_CLOSE:
	{
		delete jogador1;
		delete jogador2;
		delete pong;		
		DestroyWindow(hwnd);
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

	myWin->setWidth(700);
	myWin->setHeight(500);

	if (!myWin->createWindow())
		return 1;

	myWin->show(nCmdShow);

	myWin->messages(myWin->getHwnd(), 
		jogador1, jogador2, pong);

	return 0;
}
