#include "..\WinOO.h"

// simple owner draw lisbBox that shows line numbers
CWindowSC( ListBox_LineNum, CListBoxSC )
	BOOL Create(DWORD dwStyle, const RECT& rect, HWND pParentWnd, UINT nID)
	{ bool res = CListBox::Create(dwStyle |= LBS_OWNERDRAWFIXED | LBS_HASSTRINGS, rect, pParentWnd, nID);
	  if( res ) SubClass(_hwnd); return res; }
	// the window must be created with LBS_OWNERDRAWFIXED and LBS_HASSTRINGS, these can not be set
	void SubClass( HWND hwnd ){
		_hwnd = hwnd;
		SubClass( FL_DRAWITEM );
	}
	void OwnerDraw( LPARAM _ds ){
		DRAWITEMSTRUCT& ds = *(DRAWITEMSTRUCT*)_ds;
		if( ds.itemState & ODS_SELECTED ){
			SetTextColor( ds.hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			SetBkColor( ds.hDC, GetSysColor(COLOR_HIGHLIGHT));
			FillRect( ds.hDC, &ds.rcItem, GetSysColorBrush(COLOR_HIGHLIGHT));
		}else{
			SetTextColor( ds.hDC, GetSysColor(COLOR_WINDOWTEXT));
			SetBkColor( ds.hDC, GetSysColor(COLOR_WINDOW));
			FillRect( ds.hDC, &ds.rcItem, GetSysColorBrush(COLOR_WINDOW));
		}
		if(ds.itemID != -1){
			CString tmp;
			GetText(ds.itemID, tmp);
			tmp = CString((int)ds.itemID) += ": " + tmp;
			TextOut(ds.hDC, ds.rcItem.left, ds.rcItem.top, tmp, tmp.GetLength());
		}
		if((ds.itemAction & ODA_FOCUS) != 0)
			if(!(ds.itemState & ODS_FOCUS))
				DrawFocusRect( ds.hDC, &ds.rcItem);
	}
	long _wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lresult){
		if( message == WM_DRAWITEM ){
			OwnerDraw( lParam ); return true;}
		lresult = DefWindowProc(message, wParam, lParam);
		return true;
	}
_CWindowSC(ListBox_LineNum)
