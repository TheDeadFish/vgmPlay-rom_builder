#ifndef CWindow_H
#define CWindow_H
#include "WinClasses.h"
#include "MessageMap.h"
#include "Resize.h"

// message reflection mechanism
#define FL_DRAWITEM 			(1<<0)
#define FL_MEASUREITEM			(1<<1)
#define FL_DELETEITEM			(1<<2)
#define FL_HSCROLL				(1<<3)
#define FL_VSCROLL				(1<<4)
#define FL_VKEYTOITEM			(1<<5)
#define FL_CHARTOITEM			(1<<6)
#define FL_COMPAREITEM			(1<<7)
#define FL_COMMAND				(1<<8)
#define FL_NOTIFY				(1<<9)
#define FL_CTLCOLORMSGBOX		(1<<10)
#define FL_CTLCOLOREDIT			(1<<11)
#define FL_CTLCOLORLISTBOX		(1<<12)
#define FL_CTLCOLORBTN			(1<<13)
#define FL_CTLCOLORDLG			(1<<14)
#define FL_CTLCOLORSCROLLBAR	(1<<15)
#define FL_CTLCOLORSTATIC		(1<<16)

#define MsgReflecta() \
	long MessageReflect(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){	\
		return ::MessageReflect(hwnd, message, wParam, lParam);}
LRESULT MessageReflect(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#define CWinCreate( winclass, winstr, class ) \
	CWNDCLASSEX winclass; CWnd_STR winstr;
#define _CWinCreate( winclass, winstr, class )	\
	winclass.lpfnWndProc = (WNDPROC)&class##Proc;\
	winclass.lpszClassName = "CWindow" #class;	\
	winclass.Register();						\
	winstr.SetClass( winclass.atom );			\
	CreateWindowEx( winstr, parent );

// CWindow class macro
#define CWindow(name) \
long CALLBACK name##Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); \
CWindow_(name, CWnd) \
	HWND Create( HWND parent );	

#define CWindow_(name, base) 													\
class name : public base														\
{																				\
public:																			\
	Resize_Macro1();															\
	template <class screb>														\
	void _initProc(screb hwnd, WPARAM wParam, LPARAM lParam){}\
	template <class screb>														\
	bool _wndProc(screb hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lresult){		\
		return false;}															\
	template <class screb>														\
	bool MessageMap(screb hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lresult){	\
		return false;}															\
	template <class screb>														\
	long MessageReflect(screb hwnd, UINT message, WPARAM wParam, LPARAM lParam){\
		return false;} 	
			

// CWindow class macro end, also has subclass extention
#define _CWindow(name) }; \
	CWindowProc(name); 
	
// CWindow message handler members
#define CInitWindowHandlr() \
	void initProc(HWND hwnd, WPARAM wParam, LPARAM lParam); \
	void _initProc(HWND hwnd, WPARAM wParam, LPARAM lParam){ \
	initProc(hwnd, wParam, lParam);} 
#define CMainWndProcHandlr() \
	bool wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lresult); \
	bool _wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lresult){	\
		return wndProc(hwnd, message, wParam, lParam, lresult);} 
#define FInitWindowHandlr(class) \
	void class::initProc(HWND hwnd, WPARAM wParam, LPARAM lParam) 
#define FMainWndProcHandlr(class) \
	bool class::wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lresult)
	
#define CSimpleMsgHandlr(name) \
	long name(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
#define FSimpleMsgHandlr(name, class) \
	inline long class::name(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
#define CSingleMsgHandlr(name) \
	void name(HWND hwnd, WPARAM wParam, LPARAM lParam)
#define FSingleMsgHandlr(name, class) \
	inline void class::name(HWND hwnd, WPARAM wParam, LPARAM lParam)
#define CMultiMsgHandlr(name) \
	void name(INT msgNum, HWND hwnd, WPARAM wParam, LPARAM lParam)
#define FMultiMsgHandlr(name, class) \
	inline void class::name(INT msgNum, HWND hwnd, WPARAM wParam, LPARAM lParam)
#define FMainWndProcHandlrM( class ) \
	FMainWndProcHandlr( class ) { \
	bool MessageHandled = false; 
#define _FMainWndProcHandlrM() return MessageHandled; }

// CWindow window procedure macro, also has subclass extention
#define CWindowProc(name) _CWindowProc_(name, )
#define _CWindowProc_(name, screb)													\
long CALLBACK name##Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){ 	\
	name* object;																	\
	long MessageHandled = false;													\
	if( message == WM_NCCREATE ){													\
		LPCREATESTRUCT tmp = (LPCREATESTRUCT)lParam;								\
		object = (name*)tmp->lpCreateParams;										\
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)object);					\
		object->_hwnd = hwnd;														\
	}else																			\
		object = (name*)GetWindowLongPtr(hwnd, GWLP_USERDATA);						\
	screb																			\
	Resize_Macro2( MessageHandled, WM_CREATE )										\
	if( message == WM_CREATE ){														\
		object->_initProc(hwnd, wParam, lParam); MessageHandled = true;}			\
	LRESULT lresult = 0;															\
	if((lresult = object->MessageReflect(hwnd, message, wParam, lParam))) return lresult;\
	MessageHandled |= object->_wndProc(hwnd, message, wParam, lParam, lresult); 	\
	MessageHandled |= object->MessageMap(hwnd, message, wParam, lParam, lresult);	\
	if( !MessageHandled )															\
		return DefWindowProc(hwnd, message, wParam, lParam);						\
	return lresult;}
	
// simple message loop
int CSimpleMsgLoop(void);

// macros to quickly get objects of child controls
#define Static(nID) CStatic(::GetDlgItem(hwnd, nID))
#define Button(nID) CButton(::GetDlgItem(hwnd, nID))
#define Listbox(nID) CListBox(::GetDlgItem(hwnd, nID))
#define Edit(nID) CEdit(::GetDlgItem(hwnd, nID))

#endif
