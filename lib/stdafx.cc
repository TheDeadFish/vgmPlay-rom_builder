#include "stdafx.h"
#include "console.cc"

LRESULT CALLBACK LineNumListBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if(uMsg == OCM_DRAWITEM) {
		DRAWITEMSTRUCT& ds = *(DRAWITEMSTRUCT*)lParam;
		if( ds.itemState & ODS_SELECTED ){
			SetTextColor( ds.hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			SetBkColor( ds.hDC, GetSysColor(COLOR_HIGHLIGHT));
			FillRect( ds.hDC, &ds.rcItem, GetSysColorBrush(COLOR_HIGHLIGHT));
		}else{
			SetTextColor( ds.hDC, GetSysColor(COLOR_WINDOWTEXT));
			SetBkColor( ds.hDC, GetSysColor(COLOR_WINDOW));
			FillRect( ds.hDC, &ds.rcItem, GetSysColorBrush(COLOR_WINDOW));
		}
		if(ds.itemID != -1){ WCHAR buff[MAX_PATH+20];
			int len = _swprintf(buff, L"%d: ", ds.itemID);
			len += ListBox_GetText(hWnd, ds.itemID, buff+len);
			TextOut(ds.hDC, ds.rcItem.left, ds.rcItem.top, buff, len); }
		if((ds.itemAction & ODA_FOCUS) != 0)
			if(!(ds.itemState & ODS_FOCUS))
				DrawFocusRect( ds.hDC, &ds.rcItem);		
	}
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void WINAPI LinNumListBoxInit(HWND hList)
{
	SetWindowSubclass(hList, LineNumListBoxProc, 0, 0);
}


