#pragma once
#include <Windows.h>
#include <ctime>

class Jogador {
private:
	int posX, posY;
	RECT rect;
public:

	Jogador() {
		this->posX = 0;
		this->posY = 0;
		this->rect.left = 0;
		this->rect.right = 0;
		this->rect.top = 0;
		this->rect.bottom = 0;
	}

	void setRect(int x, int y, int w, int h) {
		rect.left = x;
		rect.right = y;
		rect.top = w;
		rect.bottom = h;
	}

	void setposX(int posX) {
		this->posX = posX;
	}
	void setposY(int posY) {
		this->posY = posY;
	}

	int getposX() {
		return this->posX;
	}
	int getposY() {
		return this->posY;
	}

};

class Pong {

private:
	HPEN vermelho, verde, azul, preto;
	HBRUSH branco, hAzul, hVerde;
	int jogador1PosX, jogador1PosY, jogador2PosX, jogador2PosY;
	int bolaPosX, bolaPosY, x, y;

	DWORD timeStart;

public:

	HBRUSH getBranco() {
		return this->branco;
	}

	HBRUSH getHAzul() {
		return this->hAzul;
	}

	HBRUSH getHVerde() {
		return this->hVerde;
	}

	HPEN getVermelho() {
		return this->vermelho;
	}	

	HPEN getAzul() {
		return this->azul;
	}

	HPEN getVerde() {
		return this->verde;
	}

	HPEN getPreto() {
		return this->preto;
	}

	int getJogador1PosX() {
		return this->jogador1PosX;
	}

	int getJogador1PosY() {
		return this->jogador1PosY;
	}

	int getJogador2PosX() {
		return this->jogador2PosX;
	}

	int getJogador2PosY() {
		return this->jogador2PosY;
	}

	int getBolaPosX() {
		return this->bolaPosX;
	}

	int getBolaPosY() {
		return this->bolaPosY;
	}

	int getX() {
		return this->x;
	}

	int getY() {
		return this->y;
	}

	DWORD getTime() {
		return this->timeStart;
	}

	void setTime(DWORD time) {
		this->timeStart = time;
	}

	void setBolaPosX(int bolaPosX) {
		this->bolaPosX = bolaPosX;
	}

	void setBolaPosY(int bolaPosY) {
		this->bolaPosY = bolaPosY;
	}

	void setX(int x) {
		this->x = x;
	}

	void setY(int y) {
		this->y = y;
	}

	void setup(Jogador* jogador1, Jogador* jogador2) {
		this->vermelho = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		this->verde = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		this->azul = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		this->preto = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		this->branco = CreateSolidBrush(RGB(255, 255, 255));
		this->hAzul = CreateSolidBrush(RGB(0, 0, 255));
		this->hVerde = CreateSolidBrush(RGB(0, 255, 0));

		jogador1->setposX(50);
		jogador1->setposY(250 - 20);
		jogador2->setposX(700 - 50);
		jogador2->setposY(250 - 20);
		
		bolaPosX = 350;
		bolaPosY = 250;

		jogador1->setRect(175 - 50, 175 + 50, 20, 40);
		jogador2->setRect(175 * 3 - 50, 175 * 3 + 50, 20, 40);
	}

	void input(Jogador* jogador1, Jogador* jogador2) {

		/* movimentar jogador1 */
		if (GetAsyncKeyState(VK_UP) && jogador1->getposY() > 0)
			jogador1->setposY(jogador1->getposY() - 10);
		if (GetAsyncKeyState(VK_DOWN) && jogador1->getposY() < 460)
			jogador1->setposY(jogador1->getposY() + 10);

		/* movimentar jogador2 */
		if (GetAsyncKeyState(0x41) && jogador2->getposY() > 0)
			jogador2->setposY(jogador2->getposY() - 10);

		if (GetAsyncKeyState(0x5A) && jogador2->getposY() < 460)
			jogador2->setposY(jogador2->getposY() + 10);

		/* iniciar jogo */

		if (GetAsyncKeyState(VK_SPACE)) {
			if (rand() % 10 < 5)
				this->setX(-5);
			else
				this->setX(5);

			if (rand() & 10 < 5)
				this->setY(-5);
			else
				this->setY(5);
		}
	}
	
	void moverBola(Jogador* jogador1, Jogador* jogador2) {
		/*if (this->getBolaPosX() + x >= 700) {
			this->setBolaPosX(350);
			this->setBolaPosY(250);
			x = 0;
			y = 0;
		}

		if (this->getBolaPosX() + x <= 0) {
			this->setBolaPosX(350);
			this->setBolaPosY(250);
			x = 0;
			y = 0;
		}

		if(this->getBolaPosX() + x >= jogador2->getposX() - 6 
			&&
			(this->getBolaPosY() <= jogador2->getposY() + 30 && this->getBolaPosY() >= jogador2->getposY() - 30) ) {
			x = -y;
		}

		if (this->getBolaPosY() > 0 && this->getBolaPosY() < 500)
			this->setBolaPosY(this->getX());
		else {
			this->setY(this->getY() - 5);
			this->setBolaPosY(this->getBolaPosY() + this->getY());
		}*/
	}
};
