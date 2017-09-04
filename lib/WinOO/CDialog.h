#ifndef CDialog_H
#define CDialog_H
#include "CWindow.h"

// DialogBoCDialog_Countx list
#define CDialog_MAX 256
extern int CDialog_Count;
extern HWND CDialog_Hwnd[CDialog_MAX];
HWND CDialog_Create( HWND hwnd, LPCTSTR lpTemplateName, DLGPROC dlgProc, LPARAM _this );
void CDialog_Destry( HWND hwnd );

// CDialog class macro
#define CDialog(name) \
BOOL CALLBACK name##Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);\
CWindow_(name, CDlg) 										\
	HWND Create(HWND hwnd, LPCTSTR lpTemplateName, bool mode){					\
	if(mode){																	\
			return (HWND)DialogBoxParam(GetModuleHandle(NULL),					\
			lpTemplateName,	hwnd, (DLGPROC)name##Proc, LPARAM(this));					\
	}else{																		\
		_hwnd = CDialog_Create(hwnd, lpTemplateName, 							\
		(DLGPROC)name##Proc, LPARAM(this)); return _hwnd;}}								\
	void Destroy(){																\
		CDialog_Destry(_hwnd);}													

// CDialog class macro end
#define _CDialog(name) }; \
	CDialogProc(name); 

// CDialog message handler members
#define CInitDialogHandlr() CInitWindowHandlr()
#define FInitDialogHandlr(class) FInitWindowHandlr(class)

// CDialog window procedure macro
#define CDialogProc(name) 															\
BOOL CALLBACK name##Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){ 	\
	name* object;																	\
	bool MessageHandled = false;													\
	if( message == WM_INITDIALOG ){													\
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)lParam);					\
		object = (name*)lParam;														\
		object->_hwnd = hwnd;														\
		MessageHandled |= true;														\
	}else																			\
		object = (name*)GetWindowLongPtr(hwnd, GWLP_USERDATA);						\
	Resize_Macro2( MessageHandled, WM_INITDIALOG)									\
	if( message == WM_INITDIALOG ){													\
		object->_initProc(hwnd, wParam, lParam); MessageHandled = true;}			\
	LRESULT lresult = 0;															\
	if((lresult = object->MessageReflect(hwnd, message, wParam, lParam))) return lresult;\
	MessageHandled |= object->_wndProc(hwnd, message, wParam, lParam, lresult); 	\
	MessageHandled |= object->MessageMap(hwnd, message, wParam, lParam, lresult);	\
	if(lresult != 0){																\
		SetWindowLong(hwnd, DWL_MSGRESULT, lresult);								\
		return lresult;}															\
	return MessageHandled;}

// Dialog processing message loop
int CMessageLoop(bool Quit);

#endif
