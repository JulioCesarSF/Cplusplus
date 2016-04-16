#pragma once
#include <Windows.h>
#include <thread>
#include <vector>
#include "Memory.h"
#include "Vector.h"

using std::vector;

class Cheat : public Memory {
private:
	DWORD dwordLocalPlayer,
		intTeam,
		intCrossId,
		dwordEntityList,
		activeWeapon,
		intAmmo1,
		intLifeState,
		boolDormant,
		dwordForceAttack,
		dwordForceJump,
		floatFlashDuration,
		flags,
		vectorOrigin,
		vectorViewAngle,
		enginePointer,
		vectorPunch,
		boneMatrix,
		vectorViewOffset;

	bool useTrigger = false, useNoFlash = false, useBH = false, useAimbot = false;
	vector<Entity> ents;

public:
	
	static Cheat& Instance() {
		static Cheat cheat;
		return cheat;
	}	
	
	Cheat() {
		dwordLocalPlayer = 0xA804CC;
		intTeam = 0xF0;
		intCrossId = 0xA954;
		dwordEntityList = 0x4A9B4E4;
		activeWeapon = 0x2EE8;
		intAmmo1 = 0x31F4;
		intLifeState = 0x25B;
		boolDormant = 0xE9;
		dwordForceAttack = 0x02EDB4C8;
		dwordForceJump = 0x4F30168;
		floatFlashDuration = 0xA2F8;
		flags = 0x100;
		vectorOrigin = 0x134;
		vectorViewAngle = 0x4D0C;
		enginePointer = 0x610344;
		vectorPunch = 0x3018;
		boneMatrix = 0x2698;
		vectorViewOffset = 0x104;
	}

	DWORD getLocalPlayer() {
		return RPM<DWORD>(this->getGameHandle(), this->getClientDll() + dwordLocalPlayer, sizeof(DWORD));
	}

	int getMyTeam() {
		return RPM<int>(this->getGameHandle(), getLocalPlayer() + intTeam, sizeof(int));
	}

	int getMyCrossId() {
		return RPM<int>(this->getGameHandle(), getLocalPlayer() + intCrossId, sizeof(int));
	}

	Vector getMyPos() {
		return RPM<Vector>(this->getGameHandle(), getLocalPlayer() + vectorOrigin, sizeof(Vector));
	}

	Vector getMyEyeVector() {
		Vector viewOffset = RPM<Vector>(this->getGameHandle(), getLocalPlayer() + vectorViewOffset, sizeof(Vector));
		Vector pos = getMyPos();
		Vector eye;
		eye.x = pos.x + viewOffset.x;
		eye.y = pos.y + viewOffset.y;
		eye.z = pos.z + viewOffset.z;
		return eye;
	}

	Vector getPunch() {
		return RPM<Vector>(this->getGameHandle(), getLocalPlayer() + vectorPunch, sizeof(Vector));
	}

	DWORD getAnglePointer() {
		return RPM<DWORD>(this->getGameHandle(), this->getEngineDll() + enginePointer, sizeof(DWORD));
	}

	Vector getEntityPosById(int id) {
		return RPM<Vector>(this->getGameHandle(), getEntity(id) + vectorOrigin, sizeof(Vector));
	}

	Vector bonePos(DWORD target, DWORD boneMatrixOffset, int boneId){ //boneId 6 is head
		DWORD boneBase = RPM<DWORD>(this->getGameHandle(), target + boneMatrixOffset, sizeof(DWORD));
		Vector vBone;
		vBone.x = RPM<float>(this->getGameHandle(), (boneBase + (0x30 * boneId) + 0x0C), sizeof(float));
		vBone.y = RPM<float>(this->getGameHandle(), (boneBase + (0x30 * boneId) + 0x1C), sizeof(float));
		vBone.z = RPM<float>(this->getGameHandle(), (boneBase + (0x30 * boneId) + 0x2C), sizeof(float));
		return vBone;		
	}

	Vector bonePos(int idEntity, DWORD boneMatrixOffset, int boneId) { //boneId 6 is head
		DWORD ent = RPM<DWORD>(this->getGameHandle(), getClientDll() + dwordEntityList + idEntity * 0x10, sizeof(DWORD));
		DWORD boneBase = RPM<DWORD>(this->getGameHandle(), ent + boneMatrixOffset, sizeof(DWORD));
		Vector vBone;
		vBone.x = RPM<float>(this->getGameHandle(), (boneBase + (0x30 * boneId) + 0x0C), sizeof(float));
		vBone.y = RPM<float>(this->getGameHandle(), (boneBase + (0x30 * boneId) + 0x1C), sizeof(float));
		vBone.z = RPM<float>(this->getGameHandle(), (boneBase + (0x30 * boneId) + 0x2C), sizeof(float));
		return vBone;
	}

	/* aimbot */
	int getAllEntitys() {
		ents.clear();

		for (int i = 0; i < 64; i++) {

			DWORD addEntity = RPM<DWORD>(this->getGameHandle(), this->getClientDll() + dwordEntityList + i * 0x10, sizeof(DWORD));
						
			if (!addEntity)
				continue;

			int lifeState = RPM<int>(this->getGameHandle(), addEntity + intLifeState, sizeof(int));
			int team = RPM<int>(this->getGameHandle(), addEntity + intTeam, sizeof(int));
			int myTeam = getMyTeam();

			if ( (lifeState == 0) &&  (team > 0) && (team != myTeam)) {
				
				Vector entityPos = RPM<Vector>(this->getGameHandle(), addEntity + vectorOrigin, sizeof(Vector));
				Vector headBone = bonePos(addEntity, boneMatrix, 6);
				int flag_p = RPM<int>(this->getGameHandle(), addEntity + flags, sizeof(int));
				Entity e;
				e.pos = entityPos;
				e.team = team;
				e.lifeState = lifeState;
				e.index = i; //index in entity list
				e.flag = flag_p;
				e.address = addEntity;
				e.headBone = headBone;

				ents.push_back(e);
			}
		}
		return ents.size();
	}

	int getTarget() {
		float maxDistance = 9999.0f, tempDistance;
		int intEntity = 0;

		for (vector<Vector>::size_type i = 0; i != ents.size(); i++) {
			tempDistance = distance(getMyPos(), ents[i].pos);

			if (tempDistance < maxDistance) {
				maxDistance = tempDistance;
				intEntity = i;
			}
		}

		return intEntity;
	}

	float distance(Vector myPos, Vector entityPos) {
		return (float)sqrt(pow(myPos.x - entityPos.x, 2.0) + pow(myPos.y - entityPos.y, 2.0) + pow(myPos.z - entityPos.z, 2.0));
	}

	Vector calcAngle(Vector src, Vector dst) {
		Vector angle;
		Vector delta;
		delta.x = src.x - dst.x;
		delta.y = src.y - dst.y;
		delta.z = src.z - dst.z;

		double hyp = (double)sqrt(delta.x * delta.x + delta.y * delta.y);
		angle.x = (float)(asinf(delta.z / (float)hyp) * 57.295779513082f);
		angle.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);
		angle.z = 0.0f;
		if (delta.x >= 0.0) {
			angle.y += 180.0f;
		}

		ClampAngles(angle);		

		return angle;
	}

	void ClampAngles(Vector &angles)
	{
		if (angles.x > 89.f)
			angles.x -= 360.f;
		else if (angles.x < -89.f)
			angles.x += 360.f;
		if (angles.y > 180.f)
			angles.y -= 360.f;
		else if (angles.y < -180.f)
			angles.y += 360.f;

		angles.z = 0;
	}

	/*end aimbot*/

	DWORD getEntity(int& id) {
		return RPM<DWORD>(this->getGameHandle(), this->getClientDll() + dwordEntityList + (id-1)  * 0x10  , sizeof(DWORD));
	}

	int getEntityTeam(int id) {
		return RPM<int>(this->getGameHandle(), getEntity(id) + intTeam, sizeof(int));
	}

	int getEntityLifeState(int id) {
		return RPM<int>(this->getGameHandle(), getEntity(id) + intLifeState, sizeof(int));
	}

	bool getEntityDormant(int id) {
		return RPM<bool>(this->getGameHandle(), getEntity(id) + boolDormant, sizeof(bool));
	}	

	Vector getEntityPos(int id) {
		return RPM<Vector>(this->getGameHandle(), getEntity(id) + vectorOrigin, sizeof(Vector));
	}

	int getAmmo() {
		DWORD aWeapon = RPM<DWORD>(this->getGameHandle(), getLocalPlayer() + activeWeapon, sizeof(DWORD));
		int index = aWeapon & 0xFFF;
		DWORD weapon = getEntity(index);
		return RPM<int>(this->getGameHandle(), weapon + intAmmo1, sizeof(int));
	}	

	int getClassId(DWORD add)
	{
		int vt = RPM<int>(this->getGameHandle(), (add + 0x8), sizeof(int));
		int fn = RPM<int>(this->getGameHandle(), (vt + 2 * 0x4), sizeof(int));
		int cls = RPM<int>(this->getGameHandle(), (fn + 0x1), sizeof(int));
		int classID = RPM<int>(this->getGameHandle(), (cls + 20), sizeof(int));
		return classID;
	}

	/* triggerbot: crosshairId 64m+- max distance to work */

	std::thread callTrigger(bool shoot, bool writeMemoryToShoot, bool checkRecoilToShoot) {
		return std::thread([=] {workTrigger(shoot, writeMemoryToShoot, checkRecoilToShoot); });
	}

	int workTrigger(bool shoot, bool writeMemoryToShoot, bool checkRecoilToShoot) {
		while (true) {		

			::Sleep(1);

			if ((::GetKeyState(VK_LEFT) & 0x0001) != 0)
				useTrigger = !useTrigger;

			if (!useTrigger)
				continue;

			if (shoot) {

				int ammo = getAmmo();
				int teamNum = getEntityTeam(getMyCrossId());
				bool dormant = getEntityDormant(getMyCrossId());

				if (ammo > 0 && !dormant
					&& (teamNum > 0) && teamNum != getMyTeam()) {

					if (checkRecoilToShoot) {

						Vector p = getPunch();

						if (p.x == 0.0f && p.y == 0.0f) {
							if (writeMemoryToShoot) {
								WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceAttack, 5);
								::Sleep(15);
								WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceAttack, 4);
							}
							else
								click(1, 14);
						}

					}
					else {

						if (writeMemoryToShoot) {
							WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceAttack, 5);
							::Sleep(15);
							WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceAttack, 4);
						}
						else
							click(1, 14);

					}

					
				}
			}
		}
		return 0;
	}

	void click(int msToTrigger, int msToRelease) {
		INPUT Input = { 0 };
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		::Sleep(msToTrigger);
		::SendInput(1, &Input, sizeof(INPUT));
		
		::Sleep(msToRelease);
		
		::ZeroMemory(&Input, sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		::SendInput(1, &Input, sizeof(INPUT));
	}

	/* no flash: write to duration */

	std::thread callNoFlash() {
		return std::thread([=] {workNoFlash(); });
	}

	int workNoFlash() {
		while (true) {

			::Sleep(2);

			if ((::GetAsyncKeyState(VK_RIGHT) & 0x0001) != 0)
				useNoFlash = !useNoFlash;

			if (!useNoFlash)
				continue;

			float flashDuration = RPM<float>(this->getGameHandle(), getLocalPlayer() + floatFlashDuration, sizeof(float));

			if (flashDuration > 0.0f)
				WPM<float>(this->getGameHandle(), getLocalPlayer() + floatFlashDuration, 0.0);					
		}
		return 0;
	}

	/*bunnyhop: write to force jump (space key pressed) */

	std::thread callBH() {
		return std::thread([=] {workBH(); });
	}

	int workBH() {
		while (true) {

			Sleep(2);

			if ((::GetAsyncKeyState(VK_UP) & 0x0001) != 0)
				useBH = !useBH;

			if (!useBH)
				continue;

			if (::GetAsyncKeyState(VK_SPACE) & 0x8000) { //space pressed
				int flag = RPM<int>(this->getGameHandle(), getLocalPlayer() + flags, sizeof(int));
				if (flag == 257) { //ground
					WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceJump, 5);
					::Sleep(10);
					WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceJump, 4);
				}
			}					
		}
		return 0;
	}

	/*aimbot (no vision check) */
	std::thread callAimbot(bool useBonePosition) {
		return std::thread([=] {workAimbot(useBonePosition); });
	}

	int workAimbot(bool useBonePosition) {
		while (true) {
			::Sleep(1);

			if ((::GetAsyncKeyState(VK_DOWN) & 0x0001) != 0)
				useAimbot = !useAimbot;

			if (!useAimbot)
				continue;			

			while (GetAsyncKeyState(0x46) & 0x8000) { //"f"
				if (getAllEntitys() > 0) {
					int intEntity = getTarget();

					Vector angle;

					if (useBonePosition)
						angle = calcAngle(getMyEyeVector(), ents[intEntity].headBone);
					else
						angle = calcAngle(getMyPos(), ents[intEntity].pos);
					
					if (std::isfinite(angle.x) && std::isfinite(angle.y) && std::isfinite(angle.z))
						/* write to my player view angle */
						WPM<Vector>(this->getGameHandle(), getAnglePointer() + vectorViewAngle, angle);
				}
				Sleep(1);
			}

		}

		return 0;
	}

	bool getUseTrigger() {
		return this->useTrigger;
	}

	bool getUseNoFlash() {
		return this->useNoFlash;
	}

	bool getUseBH() {
		return this->useBH;
	}
	bool getUseAimbot() {
		return this->useAimbot;
	}
};
