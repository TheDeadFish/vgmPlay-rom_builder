#include "CWindow.h"

LRESULT MessageReflect(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	long clifTmp = 0;
	HWND child = NULL;
	int bitCode;
	switch(message){
		case WM_DRAWITEM:{
			bitCode = 0;
			DRAWITEMSTRUCT* is = (DRAWITEMSTRUCT*)lParam;
			child = is->hwndItem;
			break;}
		case WM_MEASUREITEM:{
			bitCode = 1;
			child = GetDlgItem( hwnd, wParam );
			break;}
		case WM_DELETEITEM:{
			bitCode = 2;
			DELETEITEMSTRUCT* is = (DELETEITEMSTRUCT*)lParam;
			child = is->hwndItem;
			break;}
		case WM_HSCROLL:;
			bitCode = 3;
		if(clifTmp)
		{case WM_VSCROLL:;
			bitCode = 4;}
		if(clifTmp)
		{case WM_VKEYTOITEM:
			bitCode = 5;}
		if(clifTmp)
		{case WM_CHARTOITEM:
			bitCode = 6;}
			child = (HWND)lParam;
			break;
		case WM_COMPAREITEM:{
			bitCode = 7;
			COMPAREITEMSTRUCT* is = (COMPAREITEMSTRUCT*)lParam;
			child = is->hwndItem;
			break;}
		case WM_COMMAND:
			bitCode = 8;
			child = (HWND)lParam;
			break;
		case WM_NOTIFY:{
			bitCode = 9;
			NMHDR *nm = (NMHDR*)lParam;
			child = nm->hwndFrom;
			break;}
		// other special cases (WM_CTLCOLOR family)
		default:
			if (message >= WM_CTLCOLORMSGBOX && message <= WM_CTLCOLORSTATIC){
				bitCode = 10 + (message - WM_CTLCOLORMSGBOX);
				child = (HWND)lParam;}
	}
	if( child == NULL )
		return false;
	DWORD tmp = GetWindowLong(child, GWL_USERDATA);
	if( (tmp & 0xC0000000) != 0xC0000000 )
		return false;
	tmp &= 0x3FFFFFFF;
	if( !(*(long*)(tmp+4) & (1<<bitCode)) )
		return false;
	return SendMessage(child, message, wParam, lParam);
}

// simple message loop
int CSimpleMsgLoop(void)
{
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
