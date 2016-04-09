#include "Header.h"
 
HWND MyControls::CreateButton(LPCTSTR text, int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst, bool png){
	if (png){
		return CreateWindowEx(WS_EX_TRANSPARENT, "BUTTON", text, BS_BITMAP | BS_CENTER | WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			x, y, w, h, hwnd, id, hInst, NULL);
		//you need to initialize gdiplus and uncomment AddPngBtn function to use this option
	}
	else{
		return CreateWindow("BUTTON", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			x, y, w, h, hwnd, id, hInst, NULL);
	}
}
 
HWND MyControls::CreateCheckBox(LPCTSTR text, int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst){
	return CreateWindow("BUTTON", text, WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		x, y, w, h, hwnd, id, hInst, NULL);
}
 
HWND MyControls::CreateGroupBox(LPCTSTR title, int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst){
	return CreateWindowEx(WS_EX_WINDOWEDGE, "BUTTON", title, WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		x, y, w, h, hwnd, id, hInst, NULL);
}
 
HWND MyControls::CreateComboBox(int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst){
	return CreateWindowEx(WS_EX_STATICEDGE, "COMBOBOX", "", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		x, y, w, h, hwnd, id, hInst, NULL);
}
 
HWND MyControls::CreateTrackBar(int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst){
	return CreateWindowEx(0, TRACKBAR_CLASS, "Trackbar", WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
		x, y, w, h, hwnd, id, hInst, NULL);
}
 
HWND MyControls::CreateEdit(int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst){
	return CreateWindowEx(WS_EX_CLIENTEDGE,	"EDIT",	"",	WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		x, y, w, h,	hwnd, id, hInst, NULL);
}
 
HWND MyControls::CreateListView(int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst){
	ZeroMemory(&lv, sizeof(INITCOMMONCONTROLSEX));
	lv.dwSize = sizeof(INITCOMMONCONTROLSEX);
	lv.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&lv);
	return CreateWindow(WC_LISTVIEW, "", WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
		x, y, w, h,	hwnd, id, hInst, NULL);
}
 
void MyControls::AddItemComboBox(HWND handle, LPARAM text){
	SendMessage(handle, CB_ADDSTRING, 0, text);
}
 
//void MyControls::AddPngBtn(HWND hwnd, const WCHAR* fileName){
//  ZeroMemory(&tBmp, sizeof(HBITMAP));
//  Bitmap bmp(fileName);
//  bmp.GetHBITMAP(Color::Transparent, &tBmp);
//  SendMessage(hwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)tBmp);
//  ShowWindow(hwnd, SW_SHOW);
//}
 
void MyControls::ClearComboBox(HWND hwnd){
	SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}
 
void MyControls::SetItemPos(HWND hwnd, WPARAM itemIndex){
	SendMessage(hwnd, CB_SETCURSEL, itemIndex, NULL);
}
 
void MyControls::SetText(HWND handle, LPARAM text){
	SendMessage(handle, WM_SETTEXT, NULL, text);
}
 
void MyControls::AddCollum(HWND hwnd, LPSTR text){
	LVCOLUMN LvCol;
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvCol.cx = 28;
	LvCol.cx = 72;
	LvCol.pszText = text;
	SendMessage(hwnd, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);
}
 
void MyControls::AddItem(HWND hwnd, LPSTR text, int item){
	LVITEM LvItem;
	memset(&LvItem, 0, sizeof(LvItem));
	LvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	LvItem.cchTextMax = 256;
	LvItem.iSubItem = 0;
	LvItem.iItem = item;
	LvItem.pszText = text;
	SendMessage(hwnd, LVM_INSERTITEM, 0, (LPARAM)&LvItem);
}
 
void MyControls::AddSubItem(HWND hwnd, LPSTR text, int item, int collum){
	LVITEM LvItem;
	memset(&LvItem, 0, sizeof(LvItem));
	LvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	LvItem.cchTextMax = 256;
	LvItem.iSubItem = collum;
	LvItem.iItem = item;
	LvItem.pszText = text;
	SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)&LvItem);
}
