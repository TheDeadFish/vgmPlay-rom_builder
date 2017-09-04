#include "CDialog.h"

int CDialog_Count = 0;
HWND CDialog_Hwnd[CDialog_MAX];

HWND CDialog_Create( HWND hwnd, LPCTSTR lpTemplateName, DLGPROC dlgProc, LPARAM _this )
{
	HWND tmp = CreateDialogParam(GetModuleHandle(NULL),
	lpTemplateName,	hwnd, dlgProc, _this );
	if( CDialog_Count == CDialog_MAX){
		MessageBox(hwnd, "CDialog_Create: Out of Window Slots", "FATAL ERROR", MB_OK | MB_ICONERROR);
		ExitProcess(-1);}
	CDialog_Hwnd[CDialog_Count++] = tmp;
	return tmp;}
void CDialog_Destry( HWND hwnd ){
	DestroyWindow(hwnd);
	for(int i = 0; i < CDialog_Count; i++)
		if( hwnd == CDialog_Hwnd[i] )
			CDialog_Hwnd[i] = CDialog_Hwnd[--CDialog_Count];
	if( CDialog_Count == 0 )
		PostMessage(NULL, WM_NULL, 0, 0);
}

// Dialog processing message loop
int CMessageLoop(bool Quit)
{
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if((Quit)&&(CDialog_Count == 0))
			PostMessage(NULL, WM_QUIT, 0, 0);
		int i;
		for(i = 0; i < CDialog_Count; i++)
			if( IsDialogMessage( CDialog_Hwnd[i], &msg) )
				break;
		if( i == CDialog_Count ){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
