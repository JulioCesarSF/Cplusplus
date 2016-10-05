#pragma once

typedef struct{
	float x = 0;
	float y = 0;
	float z = 0;
}Vector, *PVector;

typedef struct {
	Vector pos;
	Vector headBone;
	int lifeState;
	int team;
	int index;
	int flag;
	DWORD address;
}Entity, *PEntity;

/*
class Entity {
private:
	Vector pos;
	int lifeState;
	int team;

public:
	void setLifeState(int l) {
		this->lifeState = l;
	}
	void setPos(Vector pos) {
		this->pos = pos;
	}
	void setTeam(int team) {
		this->team = team;
	}

	int getLifeState() {
		return this->lifeState;
	}
	Vector getPos() {
		return this->pos;
	}
	int getTeam() {
		return this->team;
	}
};
*/
