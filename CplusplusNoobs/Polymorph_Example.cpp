/*
More easy examples apply to game hacking:
a) Subtype Polymorphism
b) Ad-hoc Polymorphism
c) Parametric Polymorphism
d) Coercion Polymorphism
*/

/*polymorph.h*/
#pragma once
#include <iostream>
#include <Windows.h>
#include <ctime>
 
/* Ad-hoc Polymorphism */
bool is_validPlayer(DWORD address)
{
	if (address)
		return true;
	return false;
}
bool is_validPlayer(DWORD address, int classId)
{
	if (address && classId == 35)
		return true;
	return false;
}
bool is_validPlayer(DWORD address, int classId, BYTE lifestate)
{
	if (address && classId == 35 && lifestate == 0)
		return true;
	return false;
}
 
 
/* Subtype Polymorphism */
class Glow
{
protected:
	float r = 0;
	float g = 0;
	float b = 0;
public:
	virtual void setColor(float, float, float) = 0;
};
 
class GlowMyTeam : public Glow
{
public:
	void setColor(float c1, float c2, float c3)
	{
		this->r = c1;
		this->g = c2;
		this->b = c3;
	}
};
 
class GlowEnemyTeam : public Glow
{
public:
	void setColor(float c1, float c2, float c3)
	{
		this->r = c1;
		this->g = c2;
		this->b = c3;
	}
};
 
/* Parametric Polymorphism */
template<typename T>
T ReadMemory(HANDLE handle, DWORD dwAddress)
{
	T buffer = T();
	ReadProcessMemory(handle, (LPCVOID)dwAddres, &buffer, sizeof(T), NULL);
	return buffer;
}
 
/* Coercion Polymorphism */
class PlayerBase
{
protected:
	int health;
	int team;
public:
 
	PlayerBase()
	{
		this->health = 100;
		this->team = 1;
	}	
 
	PlayerBase(int h, int t)
	{
		this->health = h;
		this->team = t;
	}
 
	virtual int getHealth(void) = 0;
	virtual int getTeam(void) = 0;
	virtual void setHealth(int) = 0;
	virtual void setTeam(int) = 0;
	virtual void printHealthTeam(void) = 0;
};
 
class Player : public PlayerBase
{
public:
	Player() : PlayerBase() {}
	Player(int h, int t) : PlayerBase(h, t) {}	
 
	int getHealth(void)
	{
		return this->health;
	}
 
	int getTeam(void)
	{
		return this->team;
	}
 
	void setHealth(int h)
	{
		this->health = h;
	}
 
	void setTeam(int t)
	{
		this->team = t;
	}
 
	void printHealthTeam(void)
	{
		std::cout << "-> Health: " << this->getHealth() << " Team: " << this->getTeam() << std::endl;
	}
};

/*Source.cpp*/

#include "polymorph.h"
/* Subtype Polymorphism */
void set_Color(Glow* glowObject, float r, float g, float b)
{
	glowObject->setColor(r, g, b);
}
 
int main(int argc, char* argv[])
{
 
	/* Ad-hoc Polymorphism */
 
	DWORD add = 0xB00B;
	int classid = 35;
	BYTE lifeState = 6;
	if (is_validPlayer(add, classid, lifeState))
		std::cout << "Is valid!" << std::endl;
 
 
	/* Subtype Polymorphism */
 
	GlowMyTeam glowMyTeam;
	GlowEnemyTeam glowEnemyTeam;
	set_Color(&glowMyTeam, 255.0f, 0.0f, 0.0f);
	set_Color(&glowEnemyTeam, 0.0f, 255.0f, 0.0f);
 
	/* Parametric Polymorphism */
 
	/*HANDLE h = OpenProcess(...);
	DWORD add = 0xFFFFF;
	DWORD address = ReadMemory<DWORD>(h, add);*/
	
	/* Coercion Polymorphism */
 
	srand((unsigned int)time(NULL));
 
	/* example 1*/
	PlayerBase* myPlayer = new Player();
 
	myPlayer->setHealth(rand() % 100 + 1);
	myPlayer->setTeam(rand() & 10 + 1);
	std::cout << "myPlayer object: ";
	myPlayer->printHealthTeam();
 
	delete myPlayer;
 
	/* example 2*/
	PlayerBase* nPlayer = new Player[64]();
	
	for (auto i = 0; i < 64; ++i)
	{		
		nPlayer[i].setHealth(rand() % 100 + 1);
		nPlayer[i].setTeam(rand() & 10 + 1);
		std::cout << "nPlayer object: " << i << " ";
		nPlayer[i].printHealthTeam();
	}
 
	delete[] nPlayer;
 
	std::cin.get();
 
	return 0;
}
