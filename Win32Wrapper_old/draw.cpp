void Draw::DrawMyText(HDC hdc, LPCTSTR text, UINT formated, long left, long top, long right, long bottom, int size, LPCTSTR font){
	RECT txtRect = { left, top, left + right, top + bottom };
	/*
	left = upper x (start)
	top = upper y (start)
	right = lower x (end)
	bottom = lower y (end)
	*/
 
	HFONT MyFont = CreateFont(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, font);
 
	SelectObject(hdc, MyFont);
	SetBkMode(hdc, TRANSPARENT);
 
	DrawText(hdc, text, -1, &txtRect, formated);
	//recommeded: DT_LEFT | DT_VCENTER | DT_NOCLIP
}
