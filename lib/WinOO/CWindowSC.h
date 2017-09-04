// stuff required for making subclassed windows
#ifndef CWndSubCls_H
#define CWndSubCls_H
#include "CWindow.h"

#define classSC( _class )   						\
class _class##SC : public _class					\
{ public: 											\
	int scFlags; 									\
	_class##SC() { scFlags = 0; }};					
classSC( CWnd );
classSC( CStatic );
classSC( CButton );
classSC( CListBox );

#define CWindowSC(name, class) 												\
long CALLBACK name##Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);\
	CWindow_(name, class)	 												\
	WNDPROC oldProc;														\
	void SubClass( int flags ){												\
		scFlags |= flags;													\
		DWORD tmp = (DWORD)this; 											\
		SetWindowLong(_hwnd, GWL_USERDATA, tmp | 0xC0000000);				\
		WNDPROC tmp1 = (WNDPROC)GetWindowLongPtr(_hwnd, GWL_WNDPROC);		\
		if( tmp1 != (WNDPROC)name##Proc){									\
			oldProc = tmp1;													\
			SetWindowLongPtr(_hwnd, GWL_WNDPROC, (LONG_PTR)name##Proc);}}	\
		LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam){		\
			return ::CallWindowProc(oldProc, _hwnd, nMsg, wParam, lParam);}
#define _CWindowSC(name) }; \
	CWindowProcSC(name) 
#define CWindowProcSC(name) _CWindowProc_(name, object = (name*)((DWORD)object & 0x3FFFFFFF);)


#endif

