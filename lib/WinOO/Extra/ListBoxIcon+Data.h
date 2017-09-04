// implements a ownerdrawn listbox with Icon and extra data storage
// Dynamically Adding a Horizontal Scroll Bar to a List Box
#include "CWindowSC.h"

typedef struct
{
	HICON icon;
	int strSize;
	TCHAR str[];
}LbIconData_STR;

typedef struct
{
	int index;
	long* poit;
}AddItem_Ret;

CWindowSC( ListBoxIcon_Data, CListBoxSC )
	HFONT hFont;
	int mMax;
	UINT focus;
	
	BOOL Create(DWORD dwStyle, const RECT& rect, HWND pParentWnd, UINT nID)
	{ bool res = CListBox::Create(dwStyle |= LBS_OWNERDRAWFIXED, rect, pParentWnd, nID);
	  if( res ) SubClass(_hwnd); return res; }
	void SubClass( HWND hwnd ){
		_hwnd = hwnd;
		SubClass( FL_DRAWITEM );
		hFont = (HFONT)SendMessage(WM_GETFONT, 0, 0);
		Clear();
	}
	
	void Clear(void){
		// iterate through all pointers and free memory
		int count = GetCount();
		while(--count >= 0)
			DeleteItem(count);
		ResetContent();
		mMax = 0;
		focus = -1;}
	
	long* GetData( int index ){
		int tmp = GetItemData(index);
		if( tmp == -1) return NULL;
		LbIconData_STR* poit = (LbIconData_STR*)tmp;
		int size = sizeof(HICON) + sizeof(int) + (poit->strSize+1)*sizeof(TCHAR);
		if(size & 1) size ++; if(size & 2) size += 2;
		return (long*)((char*)poit + size);}
		
	AddItem_Ret AddItem(HICON icon, TCHAR *str, int extraData){
		// do the scrollbar thing
		int TextHeight;
		{HDC hdc;
		HFONT hOldFont;
		RECT rc;
		hdc = ::GetDC(_hwnd);
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_CALCRECT);
		int m = (rc.right - rc.left) +19;
		TextHeight = rc.bottom - rc.top;
		SelectObject(hdc, hOldFont);
		::ReleaseDC(_hwnd, hdc);
		if(m > mMax){
			mMax = m;
			int scrollSize = 4;
			if(GetWindowLong(_hwnd, GWL_STYLE) & WS_VSCROLL)
				 scrollSize = GetSystemMetrics(SM_CXVSCROLL);
			SetHorizontalExtent(mMax + scrollSize);}}
		// calculate size of memory, and align to int
		int strSize = lstrlen(str)+1 * sizeof(TCHAR);
		int size = sizeof(HICON) + sizeof(int) + strSize;
		if(size & 1) size ++; if(size & 2) size += 2;
		LbIconData_STR* poit = (LbIconData_STR*)malloc(size + extraData);
		AddItem_Ret reta; reta.index = -1;
		if(poit == NULL) return reta;
		poit->icon = icon;
		poit->strSize = (strSize/sizeof(TCHAR))-1;
		lstrcpy( poit->str, str );
		int ret = AddString( (TCHAR*)poit );
		SetItemHeight(ret, (16 > TextHeight+2) ? 16 : TextHeight+2);
		if( ret == -1 ) free( poit );
		reta.index = ret;
		reta.poit = (long*)((char*)poit + size);
		return reta;
	}
	
	void DeleteItem(int index){
		void *poit = (void*)GetItemData(index);
		DeleteString(index);
		free(poit);
	}
	
	void OwnerDraw( LPARAM _ds ){
		DRAWITEMSTRUCT& ds = *(DRAWITEMSTRUCT*)_ds;
		int height = ds.rcItem.bottom - ds.rcItem.top;
		LbIconData_STR& poit = *(LbIconData_STR*)ds.itemData;
		if( ds.itemState & ODS_SELECTED ){
			SetTextColor( ds.hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			SetBkColor( ds.hDC, GetSysColor(COLOR_HIGHLIGHT));
			FillRect( ds.hDC, &ds.rcItem, GetSysColorBrush(COLOR_HIGHLIGHT));
		}else{
			SetTextColor( ds.hDC, GetSysColor(COLOR_WINDOWTEXT));
			SetBkColor( ds.hDC, GetSysColor(COLOR_WINDOW));
			FillRect( ds.hDC, &ds.rcItem, GetSysColorBrush(COLOR_WINDOW));
		}
		if(&poit != NULL ){
			DrawIconEx(ds.hDC, ds.rcItem.left+1, ds.rcItem.top + (height - 16) / 2, poit.icon, 16, 16, 0, NULL, DI_NORMAL);
			RECT tmp = ds.rcItem;
			tmp.left += 19; tmp.top;
			ExtTextOut( ds.hDC, tmp.left, tmp.top + 2, ETO_OPAQUE, &tmp, poit.str, poit.strSize, NULL);
		}
		//if(!(ds.itemState & ODS_SELECTED ))
		//	if(focus == ds.itemID ) DrawFocusRect( ds.hDC, &ds.rcItem);
		if(!( ds.itemState & ODS_FOCUS) )
			if(focus == ds.itemID ) DrawFocusRect( ds.hDC, &ds.rcItem);
		else
			focus = ds.itemID;
	}
	long _wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lresult){
		if( message == WM_DRAWITEM ){
			OwnerDraw( lParam ); return true;}
		//if( message == WM_DESTROY ){
		//	Clear(); _hwnd == NULL;}
		lresult = DefWindowProc(message, wParam, lParam);
		return true;
	}
_CWindowSC(ListBoxIcon_Data)

	