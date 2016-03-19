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
	int bolaPosX, bolaPosY, direcaoX, direcaoY;

	DWORD timeStart;

public:

	Pong() {
		this->setBolaPosX(350);
		this->setBolaPosY(250);
	}
	

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

	int getDirecaoX() {
		return this->direcaoX;
	}

	int getDirecaoY() {
		return this->direcaoY;
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

	void setDirecaoX(int x) {
		this->direcaoX = x;
	}

	void setDirecaoY(int y) {
		this->direcaoY = y;
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

		if (GetAsyncKeyState(VK_SPACE) 
			&& this->getBolaPosX() == 350 
			&& this->getBolaPosY() == 250) {
			if (rand() % 10 < 5)
				this->setDirecaoX(-5);
			else
				this->setDirecaoX(5);

			if (rand() & 10 < 5)
				this->setDirecaoY(-5);
			else
				this->setDirecaoY(5);
		}
	}
	
	void moverBola(Jogador* jogador1, Jogador* jogador2) {
		
		/* bola sair da tela */
		if (this->getBolaPosX() + this->getDirecaoX() >= 700) {
			this->setBolaPosX(350);
			this->setBolaPosY(250);
			this->setDirecaoX(0);
			this->setDirecaoY(0);
		}

		if (this->getBolaPosX() + this->getDirecaoX() <= 0) {
			this->setBolaPosX(350);
			this->setBolaPosY(250);
			this->setDirecaoX(0);
			this->setDirecaoY(0);
		}		

		/* colisao com o palito do jogador1 */
		if (this->getBolaPosX() + this->getDirecaoX() <= jogador1->getposX() + 6
			&&
			(this->getBolaPosY() <= jogador1->getposY() + 30
				&& this->getBolaPosY() >= jogador1->getposY() - 30)
			) {
			this->setDirecaoX(-this->getDirecaoX());
		}

		/* colisao com o palito do jogador2 */
		if (this->getBolaPosX() + this->getDirecaoX() >= jogador2->getposX() - 6
			&&
			(this->getBolaPosY() <= jogador2->getposY() + 30
				&& this->getBolaPosY() >= jogador2->getposY() - 30)
			) {
			this->setDirecaoX(-this->getDirecaoX());
		}

		this->setBolaPosX(this->getBolaPosX() + this->getDirecaoX());		

		/* bola dentro da mesa */
		if (this->getBolaPosY() + this->getDirecaoY() < 500
			&& this->getBolaPosY() + this->getDirecaoY() > 0)
			this->setBolaPosY(this->getBolaPosY() + this->getDirecaoY());
		else {
			this->setDirecaoY(-this->getDirecaoY());
			this->setBolaPosY(this->getBolaPosY() + this->getDirecaoY());
		}

		
		
	}
};
