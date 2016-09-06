#pragma once

/*WINAPI*/
#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <WinInet.h>
#include <urlmon.h>
/*c++*/
#include <string>
#include <sstream>
#include <regex>
#include <fstream>
#include <thread>
/*mine*/
#include "resource.h"
#include "trayIcon.h"

using std::string;

/* dialog callback */
INT_PTR CALLBACK Proc(HWND, UINT, WPARAM, LPARAM);

/* timer callback */
VOID CALLBACK Timer(HWND, UINT, UINT_PTR, DWORD);

TrayIcon trayIcon;

class Dialog {

private:
	HWND janela, lista, buttonAdd, editAdd;
	LPSTR nomeColunas[4] = { "Profile", "Profile Name", "Lobby/Status", "AVBL" };
	int tamanhoColunas[4] = { 180, 100, 100, 40 };
	HIMAGELIST imagens;
	string lobbyKeyBuff, profileNameBuffer;
	int miliRefresh = 60000;
	UINT timer = NULL;
	bool threadUpdate = false, play = true;

public:

	HWND getJanela() {
		return janela;
	}

	HWND getListView() {
		return lista;
	}

	bool getThreadUpdate() {
		return threadUpdate;
	}

	void setThreadUpdate(bool update) {
		threadUpdate = update;
	}

	int getChecks() {
		int count = ListView_GetItemCount(lista);

		if (count <= 0)
			return 0;

		int l = 0;

		for (int i = 0; i < count; i++) {
			TCHAR t[MAX_PATH];
			ListView_GetItemText(lista, i, 1, t, MAX_PATH);
			string tt = t;
			if (ListView_GetCheckState(lista, i) && (tt.find("steam://joinlobby/730/") != std::string::npos))
				l++;
		}

		return l;

	}

	void loadList() {
		string line;
		std::ifstream listaa("list.txt");
		if (!listaa.is_open()) {
			::MessageBox(NULL, "List not found!", "Load List", MB_ICONERROR);
			return;
		}

		while (std::getline(listaa, line)) {
			addRow("", "", "", const_cast<char *>(line.c_str()));
		}
		listaa.close();

		if (ListView_GetItemCount(lista) > 0) {
			std::thread update = callCheckList();
			update.detach();
			::EnableMenuItem(::GetMenu(janela), ID_LIST_UPDATELIST, MF_DISABLED);
			threadUpdate = true;
		}
	}

	void saveList() {
		std::ofstream list("list.txt");

		if (!list.is_open()) {
			::MessageBox(NULL, "Could not create list!", "Save List", MB_ICONERROR);
			return;
		}

		int items = ListView_GetItemCount(lista);
		if (items <= 0)
			return;

		for (int i = 0; i < items; i++) {
			TCHAR profile[MAX_PATH];
			ListView_GetItemText(lista, i, 3, profile, MAX_PATH);
			string linha = profile;	
			linha += "\n";
			list << linha;
		}

		list.close();

	}

	void displayError(LPCTSTR lpCaption) {
		DWORD e = ::GetLastError();
		LPTSTR error = 0;
		if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, e, 0, (LPTSTR)&error, 0, NULL) != 0)
			::MessageBox(NULL, error, lpCaption, MB_OK | MB_ICONERROR);

		if (error) {
			::LocalFree(error);
			error = 0;
		}
	}

	bool isUrl(string urlTexto) {
		std::regex url(".*\\..*");
		if (std::regex_match(urlTexto, url))
			return true;

		return false;
	}

	void toClipboard(string s) { //subitem3		
		::OpenClipboard(0);
		::EmptyClipboard();
		HGLOBAL hg = ::GlobalAlloc(GMEM_MOVEABLE, s.size()+1);
		if (!hg) {
			::CloseClipboard();
			return;
		}
#pragma warning(suppress: 6387)
		memcpy(GlobalLock(hg), s.c_str(), s.size()+1);
		::GlobalUnlock(hg);
		::SetClipboardData(CF_TEXT, hg);
		::CloseClipboard();
		::GlobalFree(hg);
	}

	bool downloadProfile(int itemId) {
		lobbyKeyBuff.clear();
		profileNameBuffer.clear();

		std::size_t s, e;

		TCHAR p[MAX_PATH];
		ListView_GetItemText(lista, itemId, 3, p, MAX_PATH);
		string profile = p;

		if (profile.length() <= 0)
			return false;

		string file = "profile.dat";
		std::ifstream fFile;

		::DeleteUrlCacheEntry(profile.c_str());

		HRESULT status = ::URLDownloadToFile(0, profile.c_str(), file.c_str(), 0, 0);
		if (!SUCCEEDED(status)) {
			::MessageBox(NULL, "Could not download profile!", "Download Profile", MB_ICONERROR);
			return false;
		}

		fFile.open(file);

		if (!fFile.is_open()) {
			::MessageBox(NULL, "Could not open profile file!", "Download Profile", MB_ICONERROR);
			return false;
		}

		std::ostringstream buffer;
		buffer << fFile.rdbuf();
		string profileBuffer = buffer.str();

		/* get profile name */
		s = profileBuffer.find("actual_persona_name");
		if (s != std::string::npos) {
			s += 21;
			e = profileBuffer.find("<", s);
			e +=8;
			profileNameBuffer = profileBuffer.substr(s, e - s);
			string nome = profileNameBuffer.substr(0, profileNameBuffer.find("<"));
			profileNameBuffer = nome;
		}

		s = profileBuffer.find("profile_in_game persona online");
		if (s != std::string::npos) {
			lobbyKeyBuff = "Online";
		}

		s = profileBuffer.find("profile_in_game persona offline");
		if (s != std::string::npos) {
			lobbyKeyBuff = "Offline";
		}
		
		if (lobbyKeyBuff.length() == 0)
			lobbyKeyBuff = "Private";


		ListView_GetItemText(lista, itemId, 1, p, MAX_PATH);
		string key = p;

		s = profileBuffer.find("profile_in_game persona in-game");
		if (s != std::string::npos && key.find("steam://joinlobby/730/") == std::string::npos ) {
			lobbyKeyBuff = "In-Game";
		}

		if (profileBuffer.find("steam://joinlobby/730/") == std::string::npos)
			return false;
		else {
			/* get keylobby */			
			s = profileBuffer.find("steam://joinlobby/730/");
			if (s != std::string::npos) {
				e = profileBuffer.find("\"", s);

				lobbyKeyBuff = profileBuffer.substr(s, e - s);
			}
			return true;
		}

		return false;

	}

	void updateStatus(int itemId, int imageId) {
		
		LVITEM  item;
		
		if (lobbyKeyBuff.length() > 0) {
			item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			item.stateMask = 0;
			item.iSubItem = 1;
			item.pszText = const_cast<char *>(lobbyKeyBuff.c_str());
			item.state = 0;
			item.iItem = itemId;
			item.iImage = imageId;
			ListView_SetItem(lista, &item);
			/*imagem*/
			item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			item.stateMask = 0;
			item.iSubItem = 0;
			item.pszText = "";
			item.state = 0;
			item.iItem = itemId;
			item.iImage = imageId;
			ListView_SetItem(lista, &item);
		}

		if (profileNameBuffer.length() > 0) {
			item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			item.stateMask = 0;
			item.iSubItem = 2;
			item.pszText = const_cast<char *>(profileNameBuffer.c_str());
			item.state = 0;
			item.iItem = itemId;
			item.iImage = imageId;
			ListView_SetItem(lista, &item);
		}		

		//if (ListView_GetCheckState(lista, itemId) && imageId == 0) {
			//playsound
		//}
				
	}

	void checkList() {
		int count = ListView_GetItemCount(lista);
		if (count <= 0) {
			threadUpdate = false;
			return;
		}

		::SetWindowText(janela, "Lobby Finder 0.3 | dmthread UC 2016 (updating)");
		for (int i = 0; i < count; i++) {
			ListView_SetItemState(lista, i, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
			if (downloadProfile(i))
				updateStatus(i, 0);
			else
				updateStatus(i, 1);
		}

		ListView_SetItemState(lista, 0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
		::DeleteFile("profile.dat");
		::EnableMenuItem(::GetMenu(janela), ID_LIST_UPDATELIST, MF_ENABLED);
		::SetWindowText(janela, "Lobby Finder 0.3 | dmthread UC 2016");		

		if (getChecks() > 0 && play) {
			::PlaySound(MAKEINTRESOURCE(IDR_WAVE1), ::GetModuleHandle(NULL), SND_ASYNC | SND_RESOURCE);
		}

		threadUpdate = false;
	}

	std::thread callCheckList() {
		return std::thread([=] {checkList(); });
	}

	void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

		switch (id)
		{

		case IDC_BUTTON1: {
			TCHAR texto[MAX_PATH];
			int size = ::GetWindowTextLength(editAdd);			
			if (::GetWindowText(editAdd, texto, size + 1) > 0) {
				string url = texto;
				if (isUrl(url)) {
					addRow("", "-", "-", texto);
					::SetWindowText(editAdd, "");					
				}
			}
			
		}break;

		case ID_LIST_UPDATELIST: {
			if (ListView_GetItemCount(lista) > 0 && !threadUpdate) {
				std::thread update = callCheckList();
				update.detach();
				::EnableMenuItem(::GetMenu(hwnd), ID_LIST_UPDATELIST, MF_DISABLED);
				threadUpdate = true;
			}
		}break;

		case ID_LIST_LOADLIST:
			loadList();
			break;

		case ID_LIST_SAVELIST:
			saveList();
			break;

		case ID_PLAYSOUND_SELECTEDLOBBYAVAILABLE: {
			UINT status = ::GetMenuState(::GetMenu(hwnd), ID_PLAYSOUND_SELECTEDLOBBYAVAILABLE, MF_BYCOMMAND);
			if (status == MF_CHECKED) {
				::CheckMenuItem(::GetMenu(hwnd), ID_PLAYSOUND_SELECTEDLOBBYAVAILABLE, MF_UNCHECKED);
				play = false;
			}
			else {
				::CheckMenuItem(::GetMenu(hwnd), ID_PLAYSOUND_SELECTEDLOBBYAVAILABLE, MF_CHECKED);
				play = true;
			}
		}break;

		case ID_CONFIGURATION_AUTOREFRES: {
			UINT status = ::GetMenuState(::GetMenu(hwnd), ID_CONFIGURATION_AUTOREFRES, MF_BYCOMMAND);
			if (status == MF_CHECKED) {
				::CheckMenuItem(::GetMenu(hwnd), ID_CONFIGURATION_AUTOREFRES, MF_UNCHECKED);
				if (::KillTimer(0, timer))					
					timer = NULL;
				else
					displayError("KillTimer");
			}
			else {
				::CheckMenuItem(::GetMenu(hwnd), ID_CONFIGURATION_AUTOREFRES, MF_CHECKED);
				if(timer == NULL)
					timer = ::SetTimer(0, 0, miliRefresh, Timer);
			}
			
		}break;

		case ID_AUTOREFRESHTIMER_5SECONDS: {
			UINT status = ::GetMenuState(::GetMenu(hwnd), ID_CONFIGURATION_AUTOREFRES, MF_BYCOMMAND);
			if (status == MF_CHECKED) {
				status = ::GetMenuState(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_5SECONDS, MF_BYCOMMAND);
				if (status == MF_CHECKED) {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_5SECONDS, MF_UNCHECKED);
					miliRefresh = 60000;
				}
				else {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_5SECONDS, MF_CHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_10SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_30SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_60SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_2MINUTES, MF_UNCHECKED);
					miliRefresh = 5000;
					if (::KillTimer(0, timer)) {
						timer = NULL;
						if (timer == NULL)
							timer = ::SetTimer(0, 0, miliRefresh, Timer);
					}
					else
						displayError("KillTimer");
				}
			}
		}break;

		case ID_AUTOREFRESHTIMER_10SECONDS: {
			UINT status = ::GetMenuState(::GetMenu(hwnd), ID_CONFIGURATION_AUTOREFRES, MF_BYCOMMAND);
			if (status == MF_CHECKED) {
				status = ::GetMenuState(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_10SECONDS, MF_BYCOMMAND);
				if (status == MF_CHECKED) {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_10SECONDS, MF_UNCHECKED);
					miliRefresh = 60000;
				}
				else {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_10SECONDS, MF_CHECKED);

					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_5SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_30SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_60SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_2MINUTES, MF_UNCHECKED);

					miliRefresh = 10000;
					if (::KillTimer(0, timer)) {
						timer = NULL;
						if (timer == NULL)
							timer = ::SetTimer(0, 0, miliRefresh, Timer);
					}
					else
						displayError("KillTimer");
				}
			}
		}break;

		case ID_AUTOREFRESHTIMER_30SECONDS: {
			UINT status = ::GetMenuState(::GetMenu(hwnd), ID_CONFIGURATION_AUTOREFRES, MF_BYCOMMAND);
			if (status == MF_CHECKED) {
				status = ::GetMenuState(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_30SECONDS, MF_BYCOMMAND);
				if (status == MF_CHECKED) {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_30SECONDS, MF_UNCHECKED);
					miliRefresh = 60000;
				}
				else {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_30SECONDS, MF_CHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_10SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_5SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_60SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_2MINUTES, MF_UNCHECKED);
					miliRefresh = 30000;
					if (::KillTimer(0, timer)) {
						timer = NULL;
						if (timer == NULL)
							timer = ::SetTimer(0, 0, miliRefresh, Timer);
					}
					else
						displayError("KillTimer");
				}
			}
		}break;

			//ID_AUTOREFRESHTIMER_60SECONDS
		case ID_AUTOREFRESHTIMER_60SECONDS: {
			UINT status = ::GetMenuState(::GetMenu(hwnd), ID_CONFIGURATION_AUTOREFRES, MF_BYCOMMAND);
			if (status == MF_CHECKED) {
				status = ::GetMenuState(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_60SECONDS, MF_BYCOMMAND);
				if (status == MF_CHECKED) {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_60SECONDS, MF_UNCHECKED);
					miliRefresh = 60000;
				}
				else {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_60SECONDS, MF_CHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_10SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_5SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_30SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_2MINUTES, MF_UNCHECKED);
					miliRefresh = 60000;
					if (::KillTimer(0, timer)) {
						timer = NULL;
						if (timer == NULL)
							timer = ::SetTimer(0, 0, miliRefresh, Timer);
					}
					else
						displayError("KillTimer");
				}
			}
		}break;

			//ID_AUTOREFRESHTIMER_2MINUTES
		case ID_AUTOREFRESHTIMER_2MINUTES: {
			UINT status = ::GetMenuState(::GetMenu(hwnd), ID_CONFIGURATION_AUTOREFRES, MF_BYCOMMAND);
			if (status == MF_CHECKED) {
				status = ::GetMenuState(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_2MINUTES, MF_BYCOMMAND);
				if (status == MF_CHECKED) {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_2MINUTES, MF_UNCHECKED);
					miliRefresh = 60000;
				}
				else {
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_2MINUTES, MF_CHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_10SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_5SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_30SECONDS, MF_UNCHECKED);
					::CheckMenuItem(::GetMenu(hwnd), ID_AUTOREFRESHTIMER_60SECONDS, MF_UNCHECKED);

					miliRefresh = 120000;
					if (::KillTimer(0, timer)) {
						timer = NULL;
						if (timer == NULL)
							timer = ::SetTimer(0, 0, miliRefresh, Timer);
					}
					else
						displayError("KillTimer");
				}
			}
		}break;

		case ID_COPY_COPYPROFILELINK: {
			int item = ListView_GetNextItem(lista, -1, LVNI_SELECTED);
			if (item != -1) {
				TCHAR texto[MAX_PATH];
				ListView_GetItemText(lista, item, 3, texto, MAX_PATH);				
				string textoS = texto;
				if(textoS.length() > 0)
					toClipboard(textoS);
			}

		}break;

		case ID_COPY_COPYLOBBYKEY: {
			int item = ListView_GetNextItem(lista, -1, LVNI_SELECTED);
			if (item != -1) {
				TCHAR texto[MAX_PATH];
				ListView_GetItemText(lista, item, 1, texto, MAX_PATH);
				string textoS = texto;
				if (textoS.length() > 0)
					toClipboard(textoS);
			}
		}break;

		case ID_LOBB_ADDPROFILE:
			::SendMessage(buttonAdd, BM_CLICK, 0, 0);
			break;

		case ID_LOBB_DELETEPROFILE: {
			int item = ListView_GetNextItem(lista, -1, LVNI_SELECTED);
			if (item != -1)
				ListView_DeleteItem(lista, item);
		}break;

		case ID_LOBB_JOIN: {
			if (ListView_GetItemCount(lista) > 0) {
				int item = ListView_GetNextItem(lista, -1, LVNI_SELECTED);
				if (item != -1) {
					TCHAR texto[MAX_PATH];
					ListView_GetItemText(lista, item, 1, texto, MAX_PATH);
					string textoS = texto;
					if (textoS.length() > 0 && textoS.find("steam://joinlobby/730/") != std::string::npos)
						::ShellExecute(0, "open", texto, 0, 0, SW_SHOWNORMAL);
				}
			}
		}break;

		case ID_LOBBY_CLOSE40024:
			OnClose(hwnd);
			break;
		}

	}

	void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
		if (fDoubleClick) {
			if (ListView_GetItemCount(lista) > 0) {
				int item = ListView_GetNextItem(lista, -1, LVNI_SELECTED);
				if (item != -1) {
					TCHAR texto[MAX_PATH];
					ListView_GetItemText(lista, item, 1, texto, MAX_PATH);
					string textoS = texto;
					if (textoS.length() > 0 && textoS.find("steam://joinlobby/730/") != std::string::npos)
						::ShellExecute(0, "open", texto, 0, 0, SW_SHOWNORMAL);
				}
			}
		}
	}

	BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
		/* center screen */
		RECT r;
		::GetWindowRect(hwnd, &r);
		int xPos = (::GetSystemMetrics(SM_CXSCREEN) - r.right) / 2;
		int yPos = (::GetSystemMetrics(SM_CYSCREEN) - r.bottom) / 2;
		if (!::SetWindowPos(hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE))
			displayError("Center screen Error");

		/* icon */		
		HICON hicon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		::SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
		::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);

		/* imagens (imageList) */
		imagens = ImageList_Create(16, 16, ILC_COLOR32, 2, 0);
		if (imagens != NULL) {
			ImageList_AddIcon(imagens, ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2)));
			ImageList_AddIcon(imagens, ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON3)));
		}

		/* tray icon */
		trayIcon.init(hwnd);

		/* HWNDs */
		janela = hwnd;
		lista = ::GetDlgItem(hwnd, IDC_LIST1);
		buttonAdd = ::GetDlgItem(hwnd, IDC_BUTTON1);
		editAdd = ::GetDlgItem(hwnd, IDC_EDIT1);

		/* list view */
		ListView_SetImageList(lista, imagens, LVSIL_SMALL);
		ListView_SetExtendedListViewStyle(lista, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

		for (int i = 0; i < 4; i++)
			addColum(nomeColunas[i], tamanhoColunas[i]);	

		/*auto-update*/
		timer = ::SetTimer(0, 0, miliRefresh, Timer);

		return TRUE;
	}

	//avbl, lobby, profile name, profile
	void addRow(LPSTR avbl, LPSTR lobbyKey, LPSTR profileName, LPSTR profileLink) {
		LVITEM  item;
		item.pszText = avbl;
		item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		item.stateMask = 0;
		item.iSubItem = 0;
		item.state = 0;
		item.iItem = ListView_GetItemCount(lista);
		item.iImage = 1; // 0 = OK, 1 =  X
		

		if (ListView_InsertItem(lista, &item) == -1)
			displayError("Error to add item to list");
		else {
			LPSTR  subitems[3] = { lobbyKey, profileName, profileLink };
			for (int i = 0; i < 3; i++) {
				item.iSubItem = i + 1;
				item.pszText = subitems[i];
				ListView_SetItem(lista, &item);				
			}
		}
	}

	void addColum(LPSTR nome, int tamanho) {
		LVCOLUMN coluna;
		coluna.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
		coluna.fmt = LVCFMT_LEFT;
		coluna.cx = tamanho;
		coluna.iSubItem = 0;
		coluna.iImage = 0;
		coluna.iOrder = 0;
		coluna.pszText = nome;
		coluna.cchTextMax = lstrlen(nome);
		if (ListView_InsertColumn(lista, 0, &coluna) == -1)
			displayError("Error to add collum");
	}

	void OnSize(HWND hwnd, UINT state, int cx, int cy) {
		switch (state)
		{
		case SIZE_MINIMIZED:
			::ShowWindow(hwnd, SW_HIDE);
		}
	}

	void OnClose(HWND hwnd) {
		trayIcon.del();
		ImageList_Destroy(imagens);
		::EndDialog(hwnd, 0);
	}

};
