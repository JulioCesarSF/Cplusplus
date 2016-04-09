#include <Windows.h>
#include <commctrl.h>
 
using namespace std;
 
class Draw{
public:
	//singleton
	static Draw& Instance(){
		static Draw draw;
		return draw;
	}
 
	void DrawMyText(HDC hdc, LPCTSTR text, UINT formated, long left, long top, long right, long bottom, int size, LPCTSTR font);
};
 
/*********************************************************************************************************/
 
class MyControls{
public:
	//singleton
	static MyControls& Instance(){
	static MyControls mC;
	return mC;
	}
	//controls
	HWND CreateButton(LPCTSTR text, int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst, bool png);
	HWND CreateComboBox(int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst);
	HWND CreateCheckBox(LPCTSTR text, int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst);
	HWND CreateGroupBox(LPCTSTR title, int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst);
	HWND CreateTrackBar(int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst);
	HWND CreateEdit(int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst);
	INITCOMMONCONTROLSEX lv;
	HWND CreateListView(int x, int y, int w, int h, HMENU id, HWND hwnd, HINSTANCE hInst);
 
	//ComboBox commands
	void AddItemComboBox(HWND handle, LPARAM text);
	void ClearComboBox(HWND hwnd);
	void SetItemPos(HWND hwnd, WPARAM itemIndex);
 
	//add png to btn
	/*HBITMAP tBmp;
	void AddPngBtn(HWND hwnd, const WCHAR* fileName);*/
 
	//edit commands
	void SetText(HWND handle, LPARAM text);
 
	//ListView command
	void AddCollum(HWND hwnd, LPSTR text);
	void AddItem(HWND hwnd, LPSTR text, int item);
	void AddSubItem(HWND hwnd, LPSTR text, int item, int collum);
};
