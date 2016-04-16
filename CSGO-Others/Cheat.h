#pragma once
#include <Windows.h>
#include "Memory.h"

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
		flags;

public:
	
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
	int workTrigger(bool shoot, bool writeMemoryToShoot) {
		while (true) {
			if (shoot) {

				int ammo = getAmmo();
				int teamNum = getEntityTeam(getMyCrossId());
				bool dormant = getEntityDormant(getMyCrossId());

				if ( ammo > 0 && !dormant
					&&	(teamNum > 0) && teamNum != getMyTeam() ) {

					if (writeMemoryToShoot) {
						WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceAttack, 5);
						::Sleep(15);
						WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceAttack, 4);
					}else
						click(1, 14);
				}
			}

			::Sleep(1);

			if (::GetAsyncKeyState(VK_END))
				break;
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
	int workNoFlash() {
		while (true) {
			float flashDuration = RPM<float>(this->getGameHandle(), getLocalPlayer() + floatFlashDuration, sizeof(float));

			if (flashDuration > 0.0f)
				WPM<float>(this->getGameHandle(), getLocalPlayer() + floatFlashDuration, 0.0);

			if (::GetAsyncKeyState(VK_HOME))
				break;

			Sleep(2);
		}
		return 0;
	}

	/*bunnyhop: write to force jump (space key pressed) */
	int workBH() {
		while (true) {			

			if (::GetAsyncKeyState(VK_SPACE) & 0x8000) { //space pressed
				int flag = RPM<int>(this->getGameHandle(), getLocalPlayer() + flags, sizeof(int));
				if (flag == 257) { //ground
					WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceJump, 5);
					::Sleep(10);
					WPM<int>(this->getGameHandle(), this->getClientDll() + dwordForceJump, 4);
				}
			}

			if (::GetAsyncKeyState(VK_DELETE))
				break;

			Sleep(2);
		}
		return 0;
	}


};
