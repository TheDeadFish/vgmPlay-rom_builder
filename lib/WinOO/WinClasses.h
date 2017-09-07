// these class defenitions and code are stolen from microsoft
// that is all
#ifndef _GdiClasses_H_
#define _GdiClasses_H_
#include <assert.h>
#ifdef _DEBUG
#define ASSERT(f) assert(f)
#else
#define ASSERT(f)
#endif
#define VERIFY(f) ASSERT(f)
#include "SimpClasses.h"
#include "WinMacroUndef.h"

/////////////////////////////////////////////////////////////////////////////
// Classes declared in this file
class CGdiObject;            // CDC drawing tool
	class CPen;              // a pen / HPEN wrapper
	class CBrush;            // a brush / HBRUSH wrapper
	class CFont;             // a font / HFONT wrapper
	class CBitmap;           // a bitmap / HBITMAP wrapper
	class CPalette;          // a palette / HPALLETE wrapper
	class CRgn;              // a region / HRGN wrapper
class CDC;
class CWnd;

/////////////////////////////////////////////////////////////////////////////
// Main Window Class
class CWNDCLASSEX
{
public:
	UINT      cbSize;
	UINT      style;
	WNDPROC   lpfnWndProc;
	int       cbClsExtra;
	int       cbWndExtra;
	HINSTANCE hInstance;
	HICON     hIcon;
	HCURSOR   hCursor;
	HBRUSH    hbrBackground;
	LPCTSTR   lpszMenuName;
	LPCTSTR   lpszClassName;
	HICON     hIconSm;
	ATOM      atom;
	CWNDCLASSEX(){
		ZeroMemory( this, sizeof(WNDCLASSEX) );
		cbSize = sizeof(WNDCLASSEX);
		hInstance = GetModuleHandle(NULL);
		hCursor = LoadCursor(NULL, IDC_ARROW);}
	ATOM Register(void){
		atom = RegisterClassEx( (WNDCLASSEX*)this );
		return atom;}
};

class CWnd_STR
{
public:
	DWORD dwExStyle;
	LPCTSTR lpClassName;
	LPCTSTR lpWindowName;
	DWORD dwStyle;
	int x;
	int y;
	int nWidth;
	int nHeight;
	HMENU hMenu;
	void Init(){
		ZeroMemory( this, sizeof(CWnd_STR) );
		dwStyle = WS_OVERLAPPEDWINDOW;
		x = CW_USEDEFAULT;
		y = CW_USEDEFAULT;
		nWidth = CW_USEDEFAULT;
		nHeight = CW_USEDEFAULT;
		}
	void SetClass( ATOM atom ){
		lpClassName = MAKEINTATOM(atom);}
	void SetClass( LPCTSTR name ){
		lpClassName = name;}
	CWnd_STR(){
		Init();}
	CWnd_STR( ATOM atom ){
		Init(); SetClass(atom);	}
	CWnd_STR( LPCTSTR name ){
		Init(); SetClass(name);}
};

class CWnd
{
public:
	HWND _hwnd;            // must be first data member
	operator HWND() const;
	BOOL operator==(const CWnd& wnd) const;
	BOOL operator!=(const CWnd& wnd) const;

	HWND GetSafeHwnd() const;
	DWORD GetStyle() const;
	DWORD GetExStyle() const;
	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd);
	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd);

// Constructors and other creation
	CWnd() {}
	CWnd(HWND hwnd) { _hwnd = hwnd; }
	CWnd( CWnd_STR& str, HWND hWndParent){
		CreateWindowEx( str, hWndParent ); }
		
	BOOL Create(LPCTSTR lpClass, LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd pParentWnd, UINT nID)
	{	_hwnd = ::CreateWindow(lpClass, lpszText, dwStyle | WS_CHILD, rect.left, rect.top, 
	rect.right-rect.left, rect.bottom-rect.top, pParentWnd, (HMENU)nID, GetModuleHandle(NULL), NULL);
	return (_hwnd != NULL);}
	
	// My own create routines come here
	#define CreateWindowExM( name, type ) HWND name( DWORD dwExStyle, type lpClassName, type lpWindowName, \
	DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance){ \
	_hwnd = ::name( dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, \
	hWndParent, hMenu, hInstance, (void*)this ); return _hwnd;}
	CreateWindowExM( CreateWindowExA, LPCSTR); 
	CreateWindowExM( CreateWindowExW, LPCWSTR);
	CreateWindowExM( CreateWindowEx, LPCTSTR);
	HWND CreateWindowEx( CWnd_STR& str, HWND hWndParent){
		return CreateWindowEx(str.dwExStyle, str.lpClassName, str.lpWindowName, str.dwStyle, str.x, str.y, 
		str.nWidth, str.nHeight, hWndParent, str.hMenu, GetModuleHandle(NULL) );}

	BOOL DestroyWindow() { return ::DestroyWindow(_hwnd); };
	enum AdjustType { adjustBorder = 0, adjustOutside = 1 };
	void CalcWindowRect(LPRECT lpClientRect,
		UINT nAdjustType = adjustBorder);

// Window tree access
	int GetDlgCtrlID() const;
	int SetDlgCtrlID(int nID);
		// get and set window ID, for child windows only
	CWnd GetDlgItem(int nID) const;
		// get immediate child with given ID
	void GetDlgItem(int nID, HWND* phWnd) const;
		// as above, but returns HWND

// Message Functions
	LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL SendNotifyMessage(UINT message, WPARAM wParam, LPARAM lParam);

// Message processing for modeless dialog-like windows
	BOOL IsDialogMessage(LPMSG lpMsg);

// Window Text Functions
	BOOL SetWindowTextA( LPCSTR lpString ) { return ::SetWindowTextA( _hwnd, lpString); }
	BOOL SetWindowTextW( LPWSTR lpString ) { return ::SetWindowTextW( _hwnd, lpString); }
	BOOL SetWindowText( LPCTSTR lpString ) { return ::SetWindowText( _hwnd, lpString); }
	int GetWindowTextA( LPSTR lpString, int nMaxCount ) { return ::GetWindowTextA( _hwnd, lpString, nMaxCount ); }
	int GetWindowTextW( LPWSTR lpString, int nMaxCount ) { return ::GetWindowTextW( _hwnd, lpString, nMaxCount ); }
	int GetWindowText( LPTSTR lpString, int nMaxCount ) { return ::GetWindowText( _hwnd, lpString, nMaxCount ); }
	//void GetWindowText(CString& rString) const { ASSERT(::IsWindow(_hwnd)); int nLen = ::GetWindowTextLength(_hwnd);
	//::GetWindowText(_hwnd, rString.GetBufferSetLength(nLen), nLen+1); rString.ReleaseBuffer(); }
	int GetWindowTextLength() const;
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CFont GetFont() const;

// CMenu Functions - non-Child windows only
	//CMenu GetMenu() const;
	//BOOL SetMenu(CMenu* pMenu);
	//void DrawMenuBar();
	//CMenu GetSystemMenu(BOOL bRevert) const;
	//BOOL HiliteMenuItem(CMenu* pMenu, UINT nIDHiliteItem, UINT nHilite);

// Window Size and Position Functions
	#define CWndRTX( rect ) rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top
	BOOL IsIconic() const;
	BOOL IsZoomed() const;
	void MoveWindow(int x, int y, int nWidth, int nHeight,
				BOOL bRepaint = TRUE);
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	int SetWindowRgn(HRGN hRgn, BOOL bRedraw);
	int GetWindowRgn(HRGN hRgn) const;

	BOOL SetWindowPos(const CWnd pWndInsertAfter, int x, int y,
				int cx, int cy, UINT nFlags);
	BOOL SetWindowPos( const CWnd pWndInsertAfter, RECT* rect, UINT uFlags) 
	{ return SetWindowPos( pWndInsertAfter, CWndRTX(rect), uFlags ); }
	UINT ArrangeIconicWindows();
	void BringWindowToTop();
	void GetWindowRect(LPRECT lpRect) const;
	void GetClientRect(LPRECT lpRect) const;
	BOOL GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) const;
	BOOL SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);

// Coordinate Mapping Functions
	void ClientToScreen(LPPOINT lpPoint) const;
	void ClientToScreen(LPRECT lpRect) const{
		ClientToScreen( (LPPOINT)lpRect );
		ClientToScreen( (LPPOINT)lpRect + 1 );}
	void ScreenToClient(LPPOINT lpPoint) const;
	void ScreenToClient(LPRECT lpRect) const;
	void MapWindowPoints(CWnd pwndTo, LPPOINT lpPoint, UINT nCount) const;
	void MapWindowPoints(CWnd pwndTo, LPRECT lpRect) const;

// Update/Painting Functions
	CDC BeginPaint(LPPAINTSTRUCT lpPaint);
	void EndPaint(LPPAINTSTRUCT lpPaint);
	CDC GetDC();
	CDC GetWindowDC();
	int ReleaseDC(CDC* pDC);
	void Print(CDC* pDC, DWORD dwFlags) const;
	void PrintClient(CDC* pDC, DWORD dwFlags) const;
	void UpdateWindow();
	void SetRedraw(BOOL bRedraw = TRUE);
	BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE);
	int GetUpdateRgn(CRgn* pRgn, BOOL bErase = FALSE);
	void Invalidate(BOOL bErase = TRUE);
	void InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE);
	void InvalidateRgn(CRgn* pRgn, BOOL bErase = TRUE);
	void ValidateRect(LPCRECT lpRect);
	void ValidateRgn(CRgn* pRgn);
	BOOL ShowWindow(int nCmdShow);
	BOOL IsWindowVisible() const;
	void ShowOwnedPopups(BOOL bShow = TRUE);
	CDC GetDCEx(CRgn* prgnClip, DWORD flags);
	BOOL LockWindowUpdate();    // for backward compatibility
	void UnlockWindowUpdate();
	BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL,
		CRgn* prgnUpdate = NULL,
		UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	BOOL EnableScrollBar(int nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);

// Timer Functions
	UINT SetTimer(UINT nIDEvent, UINT nElapse,
		void (CALLBACK* lpfnTimer)(HWND, UINT, UINT, DWORD));
	BOOL KillTimer(int nIDEvent);

// Window State Functions
	BOOL IsWindowEnabled() const;
	BOOL EnableWindow(BOOL bEnable = TRUE);

	// the active window applies only to top-level (frame windows)
	static CWnd PASCAL GetActiveWindow();
	CWnd SetActiveWindow();

	// the foreground window applies only to top-level windows (frame windows)
	BOOL SetForegroundWindow();
	static CWnd PASCAL GetForegroundWindow();

	// capture and focus apply to all windows
	static CWnd PASCAL GetCapture();
	CWnd SetCapture();
	static CWnd PASCAL GetFocus();
	CWnd SetFocus();

	static CWnd PASCAL GetDesktopWindow();

// Dialog-Box Item Functions
// (NOTE: Dialog-Box Items/Controls are not necessarily in dialog boxes!)
	void CheckDlgButton(int nIDButton, UINT nCheck);
	void CheckRadioButton(int nIDFirstButton, int nIDLastButton,
					int nIDCheckButton);
	int GetCheckedRadioButton(int nIDFirstButton, int nIDLastButton);
	int DlgDirListA( LPSTR lpPathSpec,int nIDListBox,int nIDStaticPath,UINT 
		nFileType);
	int DlgDirListW( LPWSTR lpPathSpec,int nIDListBox,int nIDStaticPath,UINT 
		nFileType);
	int DlgDirList( LPTSTR lpPathSpec,int nIDListBox,int nIDStaticPath,UINT 
		nFileType);
	int DlgDirListComboBoxA( LPSTR lpPathSpec,int nIDComboBox,int nIDStaticPath,UINT
		nFileType);
	int DlgDirListComboBoxW( LPWSTR lpPathSpec,int nIDComboBox,int nIDStaticPath,
		UINT nFileType);
	int DlgDirListComboBox( LPTSTR lpPathSpec,int nIDComboBox,int nIDStaticPath,UINT
		nFileType);
	BOOL DlgDirSelectA( LPSTR lpString,int nIDListBox);
	BOOL DlgDirSelectW( LPWSTR lpString,int nIDListBox);
	BOOL DlgDirSelect( LPTSTR lpString,int nIDListBox);
	BOOL DlgDirSelectComboBoxA( LPSTR lpString,int nIDComboBox);
	BOOL DlgDirSelectComboBoxW( LPWSTR lpString,int nIDComboBox);
	BOOL DlgDirSelectComboBox( LPTSTR lpString,int nIDComboBox);
	UINT GetDlgItemInt(int nID, BOOL* lpTrans = NULL,
					BOOL bSigned = TRUE) const;
	int GetDlgItemTextA (int nID,LPSTR lpStr,int nMaxCount)const;
	int GetDlgItemTextW (int nID,LPWSTR lpStr,int nMaxCount)const;
	int GetDlgItemText(int nID,LPTSTR lpStr,int nMaxCount)const;
	//int GetDlgItemText(int nID, CString& rString) const;
	CWnd GetNextDlgGroupItem(CWnd pWndCtl, BOOL bPrevious = FALSE) const;

	CWnd GetNextDlgTabItem(CWnd pWndCtl, BOOL bPrevious = FALSE) const;
	UINT IsDlgButtonChecked(int nIDButton) const;
	LRESULT SendDlgItemMessage(int nID, UINT message,
					WPARAM wParam = 0, LPARAM lParam = 0);
	void SetDlgItemInt(int nID, UINT nValue, BOOL bSigned = TRUE);
	void SetDlgItemTextA(int nID, LPCSTR lpszString){ ::SetDlgItemTextA(_hwnd, nID, lpszString); }
	void SetDlgItemTextW(int nID, LPCWSTR lpszString){ ::SetDlgItemTextW(_hwnd, nID, lpszString); }
	void SetDlgItemText(int nID, LPCTSTR lpszString){ ::SetDlgItemText(_hwnd, nID, lpszString); }

// Scrolling Functions
	/*
	int GetScrollPos(int nBar) const;
	void GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const;
	void ScrollWindow(int xAmount, int yAmount,
					LPCRECT lpRect = NULL,
					LPCRECT lpClipRect = NULL);
	int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE);
	void SetScrollRange(int nBar, int nMinPos, int nMaxPos,
			BOOL bRedraw = TRUE);
	void ShowScrollBar(UINT nBar, BOOL bShow = TRUE);
	void EnableScrollBarCtrl(int nBar, BOOL bEnable = TRUE);
	CScrollBar* GetScrollBarCtrl(int nBar) const;
			// return sibling scrollbar control (or NULL if none)
	int ScrollWindowEx(int dx, int dy,
				LPCRECT lpRectScroll, LPCRECT lpRectClip,
				CRgn* prgnUpdate, LPRECT lpRectUpdate, UINT flags);
	BOOL SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo,
		BOOL bRedraw = TRUE);
	BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
	int GetScrollLimit(int nBar);*/

// Window Access Functions
	CWnd ChildWindowFromPoint(POINT point) const;
	CWnd ChildWindowFromPoint(POINT point, UINT nFlags) const;
	static CWnd PASCAL FindWindowA(LPCSTR lpszClassName, LPCSTR lpszWindowName);
	static CWnd PASCAL FindWindowW(LPCWSTR lpszClassName, LPCWSTR lpszWindowName);
	static CWnd PASCAL FindWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName);
	CWnd GetNextWindow(UINT nFlag = GW_HWNDNEXT) const;
	CWnd GetTopWindow() const;

	CWnd GetWindow(UINT nCmd) const;
	CWnd GetLastActivePopup() const;

	BOOL IsChild(const CWnd pWnd) const;
	CWnd GetParent() const;
	CWnd SetParent(CWnd pWndNewParent);
	static CWnd PASCAL WindowFromPoint(POINT point);

// Alert Functions
	BOOL FlashWindow(BOOL bInvert);
	int MessageBoxA(LPCSTR lpszText, LPCSTR lpszCaption = NULL, UINT nType = MB_OK)
	{ return ::MessageBoxA(_hwnd, lpszText, lpszCaption, nType); }
	int MessageBoxW(LPCWSTR lpszText, LPCWSTR lpszCaption = NULL, UINT nType = MB_OK)
	{ return ::MessageBoxW(_hwnd, lpszText, lpszCaption, nType); }
	int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK)
	{ return ::MessageBox(_hwnd, lpszText, lpszCaption, nType); }

// Clipboard Functions
	BOOL ChangeClipboardChain(HWND hWndNext);
	HWND SetClipboardViewer();
	BOOL OpenClipboard();
	static CWnd PASCAL GetClipboardOwner();
	static CWnd PASCAL GetClipboardViewer();
	static CWnd PASCAL GetOpenClipboardWindow();

// Caret Functions
	void CreateCaret(CBitmap* pBitmap);
	void CreateSolidCaret(int nWidth, int nHeight);
	void CreateGrayCaret(int nWidth, int nHeight);
	static CPoint PASCAL GetCaretPos();
	static void PASCAL SetCaretPos(POINT point);
	void HideCaret();
	void ShowCaret();

// Shell Interaction Functions
	void DragAcceptFiles(BOOL bAccept = TRUE)
	{ ASSERT(::IsWindow(_hwnd)); ::DragAcceptFiles(_hwnd, bAccept);}

// Icon Functions
	HICON SetIcon(HICON hIcon, BOOL bBigIcon);
	HICON GetIcon(BOOL bBigIcon) const;

// Context Help Functions
	BOOL SetWindowContextHelpId(DWORD dwContextHelpId);
	DWORD GetWindowContextHelpId() const;

// Layout and other functions
	//void CenterWindow(CWnd* pAlternateOwner = NULL);
};

class CDlg : public CWnd
{
public:	// Modeless construct
	CDlg() {};
	BOOL Create(LPCTSTR lpszTemplateName, DLGPROC dlgProc, HWND pParentWnd = NULL){
		_hwnd = ::CreateDialogParam(GetModuleHandle(NULL), lpszTemplateName,
		pParentWnd, dlgProc, (LPARAM)this ); return (_hwnd != NULL);}
	BOOL Create(UINT nIDTemplate, DLGPROC dlgProc, HWND pParentWnd = NULL){
		return Create( MAKEINTRESOURCE( nIDTemplate ), dlgProc, pParentWnd);}
	// will implement the other functions at another time

public:	// Modal construct
	CDlg(LPCTSTR lpszTemplateName, DLGPROC dlgProc, HWND pParentWnd = NULL){
		_hwnd = (HWND)::DialogBox(GetModuleHandle(NULL), lpszTemplateName, pParentWnd, dlgProc);}
	CDlg(UINT nIDTemplate, DLGPROC dlgProc, HWND pParentWnd = NULL){
		_hwnd = (HWND)::DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(nIDTemplate), pParentWnd, dlgProc);}
	int GetDlgResult( void ) { return (int)_hwnd; }
	// will implement the other functions at another time

// Attributes
public:
	void MapDialogRect(LPRECT lpRect) const
	{ ASSERT(::IsWindow(_hwnd)); ::MapDialogRect(_hwnd, lpRect); }

// Operations
public:
	// support for passing on tab control - use 'PostMessage' if needed
	void NextDlgCtrl() const { ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_NEXTDLGCTL, 0, 0); }
	void PrevDlgCtrl() const { ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_NEXTDLGCTL, 1, 0); }
	void GotoDlgCtrl(HWND pWndCtrl) 
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_NEXTDLGCTL, (WPARAM)pWndCtrl, 1L); }

	// default button access
	void SetDefID(UINT nID) 
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, DM_SETDEFID, nID, 0); }
	DWORD GetDefID() const
	{ ASSERT(::IsWindow(_hwnd)); return ::SendMessage(_hwnd, DM_GETDEFID, 0, 0); }

	// termination
	void EndDialog(int nResult) { ::EndDialog(_hwnd, nResult); }
};

/////////////////////////////////////////////////////////////////////////////
// Standard Windows controls

class CStatic : public CWnd
{
// Constructors
public:
	CStatic() {}
	CStatic(HWND hwnd) { _hwnd = hwnd; }
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd pParentWnd, UINT nID = 0xffff){
		CWnd* pWnd = this; return pWnd->Create(TEXT("STATIC"), lpszText, dwStyle, rect, pParentWnd, nID);}

// Operations
	HICON SetIcon(HICON hIcon) 
	{ ASSERT(::IsWindow(_hwnd)); return (HICON)::SendMessage(_hwnd, STM_SETICON, (WPARAM)hIcon, 0L); }
	HICON GetIcon() const
	{ ASSERT(::IsWindow(_hwnd)); return (HICON)::SendMessage(_hwnd, STM_GETICON, 0, 0L); }
	HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile)
	{ ASSERT(::IsWindow(_hwnd)); return (HENHMETAFILE)::SendMessage(_hwnd, STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile); }
	HENHMETAFILE GetEnhMetaFile() const
	{ ASSERT(::IsWindow(_hwnd)); return (HENHMETAFILE)::SendMessage(_hwnd, STM_GETIMAGE, IMAGE_ENHMETAFILE, 0L); }
	HBITMAP SetBitmap(HBITMAP hBitmap)
	{ ASSERT(::IsWindow(_hwnd)); return (HBITMAP)::SendMessage(_hwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); }
	HBITMAP GetBitmap() const
	{ ASSERT(::IsWindow(_hwnd)); return (HBITMAP)::SendMessage(_hwnd, STM_GETIMAGE, IMAGE_BITMAP, 0L); }
	HCURSOR SetCursor(HCURSOR hCursor)
	{ ASSERT(::IsWindow(_hwnd)); return (HCURSOR)::SendMessage(_hwnd, STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor); }
	HCURSOR GetCursor()
	{ ASSERT(::IsWindow(_hwnd)); return (HCURSOR)::SendMessage(_hwnd, STM_GETIMAGE, IMAGE_CURSOR, 0L); }
};

class CButton : public CWnd
{
// Constructors
public:
	CButton() {}
	CButton(HWND hwnd) { _hwnd = hwnd; }
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd pParentWnd, UINT nID)
		{ CWnd* pWnd = this; return pWnd->Create(TEXT("BUTTON"), lpszCaption, dwStyle, rect, pParentWnd, nID);}
	
// Attributes
	UINT GetState() const
	{ ASSERT(::IsWindow(_hwnd)); return (UINT)::SendMessage(_hwnd, BM_GETSTATE, 0, 0); }
	void SetState(BOOL bHighlight)
	{ ::SendMessage(_hwnd, BM_SETSTATE, bHighlight, 0); }
	int GetCheck() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, BM_GETCHECK, 0, 0); }
	void SetCheck(int nCheck)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, BM_SETCHECK, nCheck, 0); }
	UINT GetButtonStyle() const
	{ ASSERT(::IsWindow(_hwnd)); return (UINT)GetWindowLong(_hwnd, GWL_STYLE) & 0xff; }
	void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, BM_SETSTYLE, nStyle, (LPARAM)bRedraw); }
	HICON SetIcon(HICON hIcon)
	{ ASSERT(::IsWindow(_hwnd)); return (HICON)::SendMessage(_hwnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon); }
	HICON GetIcon() const
	{ ASSERT(::IsWindow(_hwnd)); return (HICON)::SendMessage(_hwnd, BM_GETIMAGE, IMAGE_ICON, 0L); }
	HBITMAP SetBitmap(HBITMAP hBitmap)
	{ ASSERT(::IsWindow(_hwnd)); return (HBITMAP)::SendMessage(_hwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); }
	HBITMAP GetBitmap() const
	{ ASSERT(::IsWindow(_hwnd)); return (HBITMAP)::SendMessage(_hwnd, BM_GETIMAGE, IMAGE_BITMAP, 0L); }
	HCURSOR SetCursor(HCURSOR hCursor)
	{ ASSERT(::IsWindow(_hwnd)); return (HCURSOR)::SendMessage(_hwnd, BM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor); }
	HCURSOR GetCursor()
	{ ASSERT(::IsWindow(_hwnd)); return (HCURSOR)::SendMessage(_hwnd, BM_GETIMAGE, IMAGE_CURSOR, 0L); }
};

class CListBox : public CWnd
{
// Constructors
public:
	CListBox(){}
	CListBox(HWND hwnd) { _hwnd = hwnd; }
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd pParentWnd, UINT nID)
	{ CWnd* pWnd = this; return pWnd->Create(TEXT("LISTBOX"), NULL, dwStyle, rect, pParentWnd, nID); }

// Attributes

	// for entire listbox
	int GetCount() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETCOUNT, 0, 0); }
	int GetHorizontalExtent() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETHORIZONTALEXTENT,	0, 0); }
	void SetHorizontalExtent(int cxExtent)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, LB_SETHORIZONTALEXTENT, cxExtent, 0); }
	int GetTopIndex() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETTOPINDEX, 0, 0); }
	int SetTopIndex(int nIndex)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_SETTOPINDEX, nIndex, 0);}
	LCID GetLocale() const
	{ ASSERT(::IsWindow(_hwnd)); return (LCID)::SendMessage(_hwnd, LB_GETLOCALE, 0, 0); }
	LCID SetLocale(LCID nNewLocale)
	{ ASSERT(::IsWindow(_hwnd)); return (LCID)::SendMessage(_hwnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0); }
	int InitStorage(int nItems, UINT nBytes)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_INITSTORAGE, (WPARAM)nItems, nBytes); }
	UINT ItemFromPoint(CPoint pt, BOOL& bOutside) const 
	{ ASSERT(::IsWindow(_hwnd)); DWORD dw = (DWORD)::SendMessage(_hwnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
	bOutside = !!HIWORD(dw); return LOWORD(dw); }

	// for single-selection listboxes
	int GetCurSel() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETCURSEL, 0, 0); }
	int SetCurSel(int nSelect)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_SETCURSEL, nSelect, 0); }
	// for multiple-selection listboxes
	int GetSel(int nIndex) const           // also works for single-selection
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETSEL, nIndex, 0); }
	int SetSel(int nIndex, BOOL bSelect = TRUE)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_SETSEL, bSelect, nIndex); }
	int GetSelCount() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETSELCOUNT, 0, 0); }
	int GetSelItems(int nMaxItems, LPINT rgIndex) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETSELITEMS, nMaxItems, (LPARAM)rgIndex); }
	void SetAnchorIndex(int nIndex)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, LB_SETANCHORINDEX, nIndex, 0); }
	int GetAnchorIndex() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETANCHORINDEX, 0, 0); }

	// for listbox items
	DWORD GetItemData(int nIndex) const
	{ ASSERT(::IsWindow(_hwnd)); return ::SendMessage(_hwnd, LB_GETITEMDATA, nIndex, 0); }
	int SetItemData(int nIndex, DWORD dwItemData)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData); }
	void* GetItemDataPtr(int nIndex) const
	{ ASSERT(::IsWindow(_hwnd)); return (LPVOID)GetItemData(nIndex); }
	int SetItemDataPtr(int nIndex, void* pData)
	{ ASSERT(::IsWindow(_hwnd)); return SetItemData(nIndex, (DWORD)(LPVOID)pData); }
	int GetItemRect(int nIndex, LPRECT lpRect) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETITEMRECT, nIndex, (LPARAM)lpRect); }
	int GetTextA(int nIndex, LPSTR lpszBuffer) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageA(_hwnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer); }
	int GetTextW(int nIndex, LPWSTR lpszBuffer) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageW(_hwnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer); }
	int GetText(int nIndex, LPTSTR lpszBuffer) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer); }
	//void GetText(int nIndex, CString& rString) const
	//{ ASSERT(::IsWindow(_hwnd)); GetText(nIndex, rString.GetBufferSetLength(GetTextLen(nIndex)));
	//rString.ReleaseBuffer(); }
	int GetTextLen(int nIndex) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETTEXTLEN, nIndex, 0); }

	// Settable only attributes
	void SetColumnWidth(int cxWidth)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, LB_SETCOLUMNWIDTH, cxWidth, 0); }
	BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, LB_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops); }
	void SetTabStops()
	{ ASSERT(::IsWindow(_hwnd)); VERIFY(::SendMessage(_hwnd, LB_SETTABSTOPS, 0, 0)); }
	BOOL SetTabStops(const int& cxEachStop)    // takes an 'int'
		{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, LB_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop); }

	int SetItemHeight(int nIndex, UINT cyItemHeight)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0)); }
	int GetItemHeight(int nIndex) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETITEMHEIGHT, nIndex, 0L); }
	int FindStringExactA(int nIndexStart, LPCSTR lpszFind) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageA(_hwnd, LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind); }
	int FindStringExactW(int nIndexStart, LPCWSTR lpszFind) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageW(_hwnd, LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind); }
	int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind); }
	int GetCaretIndex() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_GETCARETINDEX, 0, 0L); }
	int SetCaretIndex(int nIndex, BOOL bScroll = TRUE)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_SETCARETINDEX, nIndex, MAKELONG(bScroll, 0)); }

// Operations
	// manipulating listbox items
	int AddStringA(LPCSTR lpszItem)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageA(_hwnd, LB_ADDSTRING, 0, (LPARAM)lpszItem); }
	int AddStringW(LPCWSTR lpszItem)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageW(_hwnd, LB_ADDSTRING, 0, (LPARAM)lpszItem); }
	int AddString(LPCTSTR lpszItem)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_ADDSTRING, 0, (LPARAM)lpszItem); }
	int DeleteString(UINT nIndex)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_DELETESTRING, nIndex, 0); }
	int InsertStringA(int nIndex, LPCSTR lpszItem)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageA(_hwnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem); }
	int InsertStringW(int nIndex, LPCWSTR lpszItem)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageW(_hwnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem); }
	int InsertString(int nIndex, LPCTSTR lpszItem)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem); }
	void ResetContent()
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, LB_RESETCONTENT, 0, 0); }
	int DirA(UINT attr, LPCSTR lpszWildCard)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageA(_hwnd, LB_DIR, attr, (LPARAM)lpszWildCard); }
	int DirW(UINT attr, LPCWSTR lpszWildCard)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageW(_hwnd, LB_DIR, attr, (LPARAM)lpszWildCard); }
	int Dir(UINT attr, LPCTSTR lpszWildCard)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_DIR, attr, (LPARAM)lpszWildCard); }

	// selection helpers
	int FindStringA(int nStartAfter, LPCSTR lpszItem) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageA(_hwnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem); }
	int FindStringW(int nStartAfter, LPCWSTR lpszItem) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessageW(_hwnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem); }
	int FindString(int nStartAfter, LPCTSTR lpszItem) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem); }
	int SelectString(int nStartAfter, LPCTSTR lpszItem)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem); }
	int SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
	{ ASSERT(::IsWindow(_hwnd)); return bSelect ?
		(int)::SendMessage(_hwnd, LB_SELITEMRANGEEX, nFirstItem, nLastItem) :
		(int)::SendMessage(_hwnd, LB_SELITEMRANGEEX, nLastItem, nFirstItem); }
};

/*class CCheckListBox : public CListBox
{
// Constructors
public:
	CCheckListBox(){}
	##BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Attributes
	void SetCheckStyle(UINT nStyle);
	UINT GetCheckStyle();
	void SetCheck(int nIndex, int nCheck);
	int GetCheck(int nIndex);
	void Enable(int nIndex, BOOL bEnabled = TRUE);
	BOOL IsEnabled(int nIndex);

	virtual CRect OnGetCheckPosition(CRect rectItem, CRect rectCheckBox);

// Overridables (must override draw, measure and compare for owner draw)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

// Implementation
protected:
	void PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void PreMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	int PreCompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	void PreDeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);

	virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);

   void SetSelectionCheck( int nCheck );


	int CalcMinimumItemHeight();
	void InvalidateCheck(int nIndex);
	void InvalidateItem(int nIndex);
	int CheckFromPoint(CPoint point, BOOL& bInCheck);
};*/


class CEdit : public CWnd
{
// Constructors
public:
	CEdit(){}
	CEdit(HWND hwnd) { _hwnd = hwnd; }
	BOOL Create(DWORD dwStyle, const RECT& rect, HWND pParentWnd, UINT nID)
	{CWnd* pWnd = this; return pWnd->Create(TEXT("EDIT"), NULL, dwStyle, rect, pParentWnd, nID);}

// Attributes
	BOOL CanUndo() const
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, EM_CANUNDO, 0, 0); }
	int GetLineCount() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, EM_GETLINECOUNT, 0, 0); }
	BOOL GetModify() const
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, EM_GETMODIFY, 0, 0); }
	void SetModify(BOOL bModified = TRUE)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETMODIFY, bModified, 0); }
	void GetRect(LPRECT lpRect) const
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_GETRECT, 0, (LPARAM)lpRect); }
	DWORD GetSel() const
	{ ASSERT(::IsWindow(_hwnd)); return ::SendMessage(_hwnd, EM_GETSEL, 0, 0); }
	void GetSel(int& nStartChar, int& nEndChar) const
		{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_GETSEL, (WPARAM)&nStartChar,(LPARAM)&nEndChar); }
	HLOCAL GetHandle() const
	{ ASSERT(::IsWindow(_hwnd)); return (HLOCAL)::SendMessage(_hwnd, EM_GETHANDLE, 0, 0); }
	void SetHandle(HLOCAL hBuffer)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETHANDLE, (WPARAM)hBuffer, 0); }
#if (WINVER >= 0x400)
	void SetMargins(UINT nLeft, UINT nRight)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, MAKELONG(nLeft, nRight)); }
	DWORD GetMargins() const
	{ ASSERT(::IsWindow(_hwnd)); return (DWORD)::SendMessage(_hwnd, EM_GETMARGINS, 0, 0); }
	void SetLimitText(UINT nMax)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETLIMITTEXT, nMax, 0); }
	UINT GetLimitText() const
	{ ASSERT(::IsWindow(_hwnd)); return (UINT)::SendMessage(_hwnd, EM_GETLIMITTEXT, 0, 0); }
	CPoint PosFromChar(UINT nChar) const
	{ ASSERT(::IsWindow(_hwnd)); return CPoint( (DWORD)::SendMessage(_hwnd, EM_POSFROMCHAR, nChar, 0)); }
	int CharFromPos(CPoint pt) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y)); }
#endif

	// NOTE: first word in lpszBuffer must contain the size of the buffer!
	int GetLine(int nIndex, LPTSTR lpszBuffer) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer); }
	int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
	{ ASSERT(::IsWindow(_hwnd)); *(LPWORD)lpszBuffer = (WORD)nMaxLength; return (int)::SendMessage(_hwnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer); }

// Operations
	void EmptyUndoBuffer()
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_EMPTYUNDOBUFFER, 0, 0); }
	BOOL FmtLines(BOOL bAddEOL)
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, EM_FMTLINES, bAddEOL, 0); }

	void LimitText(int nChars = 0)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_LIMITTEXT, nChars, 0); }
	int LineFromChar(int nIndex = -1) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, EM_LINEFROMCHAR, nIndex, 0); }
	int LineIndex(int nLine = -1) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, EM_LINEINDEX, nLine, 0); }
	int LineLength(int nLine = -1) const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, EM_LINELENGTH, nLine, 0); }
	void LineScroll(int nLines, int nChars = 0)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_LINESCROLL, nChars, nLines); }
	void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText); }
	void SetPasswordChar(TCHAR ch)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETPASSWORDCHAR, ch, 0); }
	void SetRect(LPCRECT lpRect)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETRECT, 0, (LPARAM)lpRect); }
	void SetRectNP(LPCRECT lpRect)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETRECTNP, 0, (LPARAM)lpRect); }
	void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
	  if (!bNoScroll) ::SendMessage(_hwnd, EM_SCROLLCARET, 0, 0); }
	void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, EM_SETSEL, nStartChar, nEndChar);
	  if (!bNoScroll) ::SendMessage(_hwnd, EM_SCROLLCARET, 0, 0); }
	BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, EM_SETTABSTOPS, nTabStops,	(LPARAM)rgTabStops); }
	void SetTabStops()
	{ ASSERT(::IsWindow(_hwnd)); VERIFY(::SendMessage(_hwnd, EM_SETTABSTOPS, 0, 0)); }
	BOOL SetTabStops(const int& cxEachStop)
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, EM_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop); }

	// Clipboard operations
	BOOL Undo()
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, EM_UNDO, 0, 0); }
	void Clear()
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_CLEAR, 0, 0); }
	void Copy()
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_COPY, 0, 0); }
	void Cut()
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_CUT, 0, 0); }
	void Paste()
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_PASTE, 0, 0); }

	BOOL SetReadOnly(BOOL bReadOnly = TRUE)
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::SendMessage(_hwnd, EM_SETREADONLY, bReadOnly, 0L); }
	int GetFirstVisibleLine() const
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SendMessage(_hwnd, EM_GETFIRSTVISIBLELINE, 0, 0L); }
	TCHAR GetPasswordChar() const
	{ ASSERT(::IsWindow(_hwnd)); return (TCHAR)::SendMessage(_hwnd, EM_GETPASSWORDCHAR, 0, 0L); }
};

/////////////////////////////////////////////////////////////////////////////
// CGdiObject abstract class for CDC SelectObject
class CGdiObject
{
public:
	// Attributes
	HGDIOBJ m_hObject;                  // must be first data member
	operator HGDIOBJ() const;
	HGDIOBJ GetSafeHandle() const;

	// static CGdiObject* PASCAL FromHandle(HGDIOBJ hObject);
	// static void PASCAL DeleteTempMap();
	// BOOL Attach(HGDIOBJ hObject);
	// HGDIOBJ Detach();
	
	// Constructors
	CGdiObject(); // must Create a derived class object
	CGdiObject(HGDIOBJ h){ m_hObject = h; }
	BOOL DeleteObject();
	
	// Operations
	int GetObject(int nCount, LPVOID lpObject) const;
	UINT GetObjectType() const;
	BOOL CreateStockObject(int nIndex);
	BOOL UnrealizeObject();
	BOOL operator==(const CGdiObject& obj) const;
	BOOL operator!=(const CGdiObject& obj) const;
};

/////////////////////////////////////////////////////////////////////////////
// CGdiObject subclasses (drawing tools)
class CPen : public CGdiObject
{
public:
// Constructors
	CPen();
	CPen(HGDIOBJ h){ m_hObject = h; }
	CPen(int nPenStyle, int nWidth, COLORREF crColor);
	CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
		int nStyleCount = 0, const DWORD* lpStyle = NULL);
	BOOL CreatePen(int nPenStyle, int nWidth, COLORREF crColor);
	BOOL CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
		int nStyleCount = 0, const DWORD* lpStyle = NULL);
	BOOL CreatePenIndirect(LPLOGPEN lpLogPen);

// Attributes
	operator HPEN() const;
	int GetLogPen(LOGPEN* pLogPen);
	int GetExtLogPen(EXTLOGPEN* pLogPen);
};

class CBrush : public CGdiObject
{
public:
// Constructors
	CBrush();
	CBrush(HGDIOBJ h){ m_hObject = h; }
	CBrush(COLORREF crColor);             // CreateSolidBrush
	CBrush(int nIndex, COLORREF crColor); // CreateHatchBrush
	CBrush(CBitmap* pBitmap);          // CreatePatternBrush

	BOOL CreateSolidBrush(COLORREF crColor);
	BOOL CreateHatchBrush(int nIndex, COLORREF crColor);
	BOOL CreateBrushIndirect(const LOGBRUSH* lpLogBrush);
	BOOL CreatePatternBrush(CBitmap* pBitmap);
	BOOL CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
	BOOL CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage);
	BOOL CreateSysColorBrush(int nIndex);

// Attributes
	operator HBRUSH() const;
	int GetLogBrush(LOGBRUSH* pLogBrush);
};

class CFont : public CGdiObject
{
public:
// Constructors
	CFont();
	CFont(HGDIOBJ h){ m_hObject = h; }
	BOOL CreateFontIndirect(const LOGFONT* lpLogFont);
	BOOL CreateFontA (int nHeight,int nWidth,int nEscapement,int nOrientation,int 
		nWeight,BYTE bItalic,BYTE bUnderline,BYTE cStrikeOut,BYTE nCharSet,BYTE
		nOutPrecision,BYTE nClipPrecision,BYTE nQuality,BYTE nPitchAndFamily,
		LPCSTR lpszFacename);
	BOOL CreateFontW (int nHeight,int nWidth,int nEscapement,int nOrientation,int 
		nWeight,BYTE bItalic,BYTE bUnderline,BYTE cStrikeOut,BYTE nCharSet,BYTE
		nOutPrecision,BYTE nClipPrecision,BYTE nQuality,BYTE nPitchAndFamily,
		LPCWSTR lpszFacename);
	BOOL CreateFont (int nHeight,int nWidth,int nEscapement,int nOrientation,int 
		nWeight,BYTE bItalic,BYTE bUnderline,BYTE cStrikeOut,BYTE nCharSet,BYTE
		nOutPrecision,BYTE nClipPrecision,BYTE nQuality,BYTE nPitchAndFamily,
		LPCTSTR lpszFacename);
	BOOL CreatePointFontA (int nPointSize,LPCSTR lpszFaceName,CDC*pDC=NULL);
	BOOL CreatePointFontW (int nPointSize,LPCWSTR lpszFaceName,CDC*pDC=NULL);
	BOOL CreatePointFont (int nPointSize,LPCTSTR lpszFaceName,CDC*pDC=NULL);
	BOOL CreatePointFontIndirect(const LOGFONT* lpLogFont, CDC* pDC = NULL);

// Attributes
	operator HFONT() const;
	int GetLogFont(LOGFONT* pLogFont);
};

class CBitmap : public CGdiObject
{
public:
// Constructors
	CBitmap();
	CBitmap(HGDIOBJ h){ m_hObject = h; }
	BOOL LoadBitmapA (LPCSTR lpszResourceName);
	BOOL LoadBitmapW (LPCWSTR lpszResourceName);
	BOOL LoadBitmap (LPCTSTR lpszResourceName);
	BOOL LoadBitmap(UINT nIDResource);
	BOOL LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
	//BOOL LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0,
	//	void* lpColorMap = NULL, int nMapSize = 0);
	BOOL CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount,
			const void* lpBits);
	BOOL CreateBitmapIndirect(LPBITMAP lpBitmap);
	BOOL CreateCompatibleBitmap(CDC* pDC, int nWidth, int nHeight);
	BOOL CreateDiscardableBitmap(CDC* pDC, int nWidth, int nHeight);

// Attributes
	operator HBITMAP() const;
	int GetBitmap(BITMAP* pBitMap);

// Operations
	DWORD SetBitmapBits(DWORD dwCount, const void* lpBits);
	DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;
	CSize SetBitmapDimension(int nWidth, int nHeight);
	CSize GetBitmapDimension() const;
};

class CPalette : public CGdiObject
{
public:
// Constructors
	CPalette();
	CPalette(HGDIOBJ h){ m_hObject = h; }
	BOOL CreatePalette(LPLOGPALETTE lpLogPalette);
	BOOL CreateHalftonePalette(CDC* pDC);

// Attributes
	operator HPALETTE() const;
	int GetEntryCount();
	UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
			LPPALETTEENTRY lpPaletteColors) const;
	UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
			LPPALETTEENTRY lpPaletteColors);

// Operations
	void AnimatePalette(UINT nStartIndex, UINT nNumEntries,
			LPPALETTEENTRY lpPaletteColors);
	UINT GetNearestPaletteIndex(COLORREF crColor) const;
	BOOL ResizePalette(UINT nNumEntries);
};

class CRgn : public CGdiObject
{
public:
	operator HRGN() const;

// Constructors
	CRgn();
	CRgn(HGDIOBJ h){ m_hObject = h; }
	BOOL CreateRectRgn(int x1, int y1, int x2, int y2);
	BOOL CreateRectRgnIndirect(LPCRECT lpRect);
	BOOL CreateEllipticRgn(int x1, int y1, int x2, int y2);
	BOOL CreateEllipticRgnIndirect(LPCRECT lpRect);
	BOOL CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
	BOOL CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
			int nCount, int nPolyFillMode);
	BOOL CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
	BOOL CreateFromPath(CDC* pDC);
	BOOL CreateFromData(const XFORM* lpXForm, int nCount,
		const RGNDATA* pRgnData);

// Operations
	void SetRectRgn(int x1, int y1, int x2, int y2);
	void SetRectRgn(LPCRECT lpRect);
	int CombineRgn(CRgn* pRgn1, CRgn* pRgn2, int nCombineMode);
	int CopyRgn1(CRgn* pRgnSrc);
	BOOL EqualRgn(CRgn* pRgn) const;
	int OffsetRgn(int x, int y);
	int OffsetRgn(POINT point);
	int GetRgnBox(LPRECT lpRect) const;
	BOOL PtInRegion(int x, int y) const;
	BOOL PtInRegion(POINT point) const;
	BOOL RectInRegion(LPCRECT lpRect) const;
	int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;
};

/////////////////////////////////////////////////////////////////////////////
// The device context
class CDC
{
public:

// Attributes
	HDC m_hDC;          // The output DC (must be first data member)
	//HDC m_hDC;    // The Attribute DC
	operator HDC() const;
	HDC GetSafeHdc() const; // Always returns the Output DC
	CWnd GetWindow() const;

	CDC(HDC hDC) { m_hDC = hDC; }
	//static CDC* PASCAL FromHandle(HDC hDC);
	//static void PASCAL DeleteTempMap();
	//BOOL Attach(HDC hDC);   // Attach/Detach affects only the Output DC
	//HDC Detach();

	void SetAttribDC(HDC hDC);  // Set the Attribute DC
	void SetOutputDC(HDC hDC);  // Set the Output DC
	void ReleaseAttribDC();     // Release the Attribute DC
	void ReleaseOutputDC();     // Release the Output DC

	BOOL IsPrinting() const;            // TRUE if being used for printing

	CPen GetCurrentPen() const;
	CBrush GetCurrentBrush() const;
	CPalette GetCurrentPalette() const;
	CFont GetCurrentFont() const;
	CBitmap GetCurrentBitmap() const;

	// for bidi and mirrored localization
	DWORD GetLayout() const;
	DWORD SetLayout(DWORD dwLayout);

// Constructors
	CDC();
	BOOL CreateDCA (LPCSTR lpszDriverName,LPCSTR lpszDeviceName,LPCSTR lpszOutput,
		const void*lpInitData);
	BOOL CreateDCW (LPCWSTR lpszDriverName,LPCWSTR lpszDeviceName,LPCWSTR lpszOutput
		,const void*lpInitData);
	BOOL CreateDC (LPCTSTR lpszDriverName,LPCTSTR lpszDeviceName,LPCTSTR lpszOutput,
		const void*lpInitData);
	BOOL CreateICA (LPCSTR lpszDriverName,LPCSTR lpszDeviceName,LPCSTR lpszOutput,
		const void*lpInitData);
	BOOL CreateICW (LPCWSTR lpszDriverName,LPCWSTR lpszDeviceName,LPCWSTR lpszOutput
		,const void*lpInitData);
	BOOL CreateIC (LPCTSTR lpszDriverName,LPCTSTR lpszDeviceName,LPCTSTR lpszOutput,
		const void*lpInitData);
	BOOL CreateCompatibleDC(CDC* pDC);
	BOOL DeleteDC();

// Device-Context Functions
	 int SaveDC();
	 BOOL RestoreDC(int nSavedDC);
	int GetDeviceCaps(int nIndex) const;
	UINT SetBoundsRect(LPCRECT lpRectBounds, UINT flags);
	UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags);
	BOOL ResetDC(const DEVMODE* lpDevMode);

// Drawing-Tool Functions
	CPoint GetBrushOrg() const;
	CPoint SetBrushOrg(int x, int y);
	CPoint SetBrushOrg(POINT point);
	int EnumObjects(int nObjectType,
			int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData);

// Type-safe selection helpers
public:
	CGdiObject SelectStockObject(int nIndex);
	CPen SelectObject(CPen* pPen);
	CBrush SelectObject(CBrush* pBrush);
	CFont SelectObject(CFont* pFont);
	CBitmap SelectObject(CBitmap* pBitmap);
	int SelectObject(CRgn* pRgn);       // special return for regions
	CGdiObject SelectObject(CGdiObject* pObject);
		// CGdiObject* provided so compiler doesn't use SelectObject(HGDIOBJ)

// Color and Color Palette Functions
	COLORREF GetNearestColor(COLORREF crColor) const;
	CPalette SelectPalette(CPalette* pPalette, BOOL bForceBackground);
	UINT RealizePalette();
	void UpdateColors();

// Drawing-Attribute Functions
	COLORREF GetBkColor() const;
	int GetBkMode() const;
	int GetPolyFillMode() const;
	int GetROP2() const;
	int GetStretchBltMode() const;
	COLORREF GetTextColor() const;

	COLORREF SetBkColor(COLORREF crColor);
	int SetBkMode(int nBkMode);
	int SetPolyFillMode(int nPolyFillMode);
	int SetROP2(int nDrawMode);
	int SetStretchBltMode(int nStretchMode);
	COLORREF SetTextColor(COLORREF crColor);

	BOOL GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const;
	BOOL SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust);

// Mapping Functions
	int GetMapMode() const;
	CPoint GetViewportOrg() const;
	int SetMapMode(int nMapMode);
	// Viewport Origin
	CPoint SetViewportOrg(int x, int y);
	CPoint SetViewportOrg(POINT point);
	CPoint OffsetViewportOrg(int nWidth, int nHeight);

	// Viewport Extent
	CSize GetViewportExt() const;
	CSize SetViewportExt(int cx, int cy);
	CSize SetViewportExt(SIZE size);
	CSize ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);

	// Window Origin
	CPoint GetWindowOrg() const;
	CPoint SetWindowOrg(int x, int y);
	CPoint SetWindowOrg(POINT point);
	CPoint OffsetWindowOrg(int nWidth, int nHeight);

	// Window extent
	CSize GetWindowExt() const;
	CSize SetWindowExt(int cx, int cy);
	CSize SetWindowExt(SIZE size);
	CSize ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom);

// Coordinate Functions
	void DPtoLP(LPPOINT lpPoints, int nCount = 1) const;
	void DPtoLP(LPRECT lpRect) const;
	void DPtoLP(LPSIZE lpSize) const;
	void LPtoDP(LPPOINT lpPoints, int nCount = 1) const;
	void LPtoDP(LPRECT lpRect) const;
	void LPtoDP(LPSIZE lpSize) const;

// Special Coordinate Functions (useful for dealing with metafiles and OLE)
	void DPtoHIMETRIC(LPSIZE lpSize) const;
	void LPtoHIMETRIC(LPSIZE lpSize) const;
	void HIMETRICtoDP(LPSIZE lpSize) const;
	void HIMETRICtoLP(LPSIZE lpSize) const;

// Region Functions
	BOOL FillRgn(CRgn* pRgn, CBrush* pBrush);
	BOOL FrameRgn(CRgn* pRgn, CBrush* pBrush, int nWidth, int nHeight);
	BOOL InvertRgn(CRgn* pRgn);
	BOOL PaintRgn(CRgn* pRgn);

// Clipping Functions
	int GetClipBox(LPRECT lpRect) const;
	BOOL PtVisible(int x, int y) const;
	BOOL PtVisible(POINT point) const;
	BOOL RectVisible(LPCRECT lpRect) const;
	int SelectClipRgn(CRgn* pRgn);
	int ExcludeClipRect(int x1, int y1, int x2, int y2);
	int ExcludeClipRect(LPCRECT lpRect);
	int ExcludeUpdateRgn(CWnd pWnd);
	int IntersectClipRect(int x1, int y1, int x2, int y2);
	int IntersectClipRect(LPCRECT lpRect);
	int OffsetClipRgn(int x, int y);
	int OffsetClipRgn(SIZE size);
	int SelectClipRgn(CRgn* pRgn, int nMode);

// Line-Output Functions
	CPoint GetCurrentPosition() const;
	CPoint MoveTo(int x, int y);
	CPoint MoveTo(POINT point);
	BOOL LineTo(int x, int y);
	BOOL LineTo(POINT point);
	BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	BOOL Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
	BOOL Polyline(LPPOINT lpPoints, int nCount);

	BOOL AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle);
	BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	BOOL ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
	int GetArcDirection() const;
	int SetArcDirection(int nArcDirection);

	BOOL PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount);
	BOOL PolylineTo(const POINT* lpPoints, int nCount);
	BOOL PolyPolyline(const POINT* lpPoints,
		const DWORD* lpPolyPoints, int nCount);

	BOOL PolyBezier(const POINT* lpPoints, int nCount);
	BOOL PolyBezierTo(const POINT* lpPoints, int nCount);

// Simple Drawing Functions
	void FillRect(LPCRECT lpRect, CBrush* pBrush);
	void FrameRect(LPCRECT lpRect, CBrush* pBrush);
	void InvertRect(LPCRECT lpRect);
	BOOL DrawIcon(int x, int y, HICON hIcon);
	BOOL DrawIcon(POINT point, HICON hIcon);
#if (WINVER >= 0x400)
	BOOL DrawState(CPoint pt, CSize size, HBITMAP hBitmap, UINT nFlags,
		HBRUSH hBrush = NULL);
	BOOL DrawState(CPoint pt, CSize size, CBitmap* pBitmap, UINT nFlags,
		CBrush* pBrush = NULL);
	BOOL DrawState(CPoint pt, CSize size, HICON hIcon, UINT nFlags,
		HBRUSH hBrush = NULL);
	BOOL DrawState(CPoint pt, CSize size, HICON hIcon, UINT nFlags,
		CBrush* pBrush = NULL);
	BOOL DrawStateA(CPoint pt,CSize size,LPCSTR lpszText,UINT nFlags,BOOL 
		bPrefixText=TRUE,int nTextLen=0,HBRUSH hBrush=NULL);
	BOOL DrawStateW(CPoint pt,CSize size,LPCWSTR lpszText,UINT nFlags,BOOL 
		bPrefixText=TRUE,int nTextLen=0,HBRUSH hBrush=NULL);
	BOOL DrawState(CPoint pt,CSize size,LPCTSTR lpszText,UINT nFlags,BOOL 
		bPrefixText=TRUE,int nTextLen=0,HBRUSH hBrush=NULL);
	BOOL DrawStateA(CPoint pt,CSize size,LPCSTR lpszText,UINT nFlags,BOOL 
		bPrefixText=TRUE,int nTextLen=0,CBrush*pBrush=NULL);
	BOOL DrawStateW(CPoint pt,CSize size,LPCWSTR lpszText,UINT nFlags,BOOL 
		bPrefixText=TRUE,int nTextLen=0,CBrush*pBrush=NULL);
	BOOL DrawState(CPoint pt,CSize size,LPCTSTR lpszText,UINT nFlags,BOOL 
		bPrefixText=TRUE,int nTextLen=0,CBrush*pBrush=NULL);
	BOOL DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc,
		LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL);
	BOOL DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc,
		LPARAM lData, UINT nFlags, CBrush* pBrush = NULL);
#endif

// Ellipse and Polygon Functions
	BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3,
		int x4, int y4);
	BOOL Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
	void DrawFocusRect(LPCRECT lpRect);
	BOOL Ellipse(int x1, int y1, int x2, int y2);
	BOOL Ellipse(LPCRECT lpRect);
	BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	BOOL Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
	BOOL Polygon(LPPOINT lpPoints, int nCount);
	BOOL PolyPolygon(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount);
	BOOL Rectangle(int x1, int y1, int x2, int y2);
	BOOL Rectangle(LPCRECT lpRect);
	BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
	BOOL RoundRect(LPCRECT lpRect, POINT point);

// Bitmap Functions
	BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop);
	BOOL BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
		int xSrc, int ySrc, DWORD dwRop);
	BOOL StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
		int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
	COLORREF GetPixel(int x, int y) const;
	COLORREF GetPixel(POINT point) const;
	COLORREF SetPixel(int x, int y, COLORREF crColor);
	COLORREF SetPixel(POINT point, COLORREF crColor);
	BOOL FloodFill(int x, int y, COLORREF crColor);
	BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType);
	BOOL MaskBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
		int xSrc, int ySrc, CBitmap& maskBitmap, int xMask, int yMask,
		DWORD dwRop);
	BOOL PlgBlt(LPPOINT lpPoint, CDC* pSrcDC, int xSrc, int ySrc,
		int nWidth, int nHeight, CBitmap& maskBitmap, int xMask, int yMask);
	BOOL SetPixelV(int x, int y, COLORREF crColor);
	BOOL SetPixelV(POINT point, COLORREF crColor);

// Text Functions
	BOOL TextOutA (int x,int y,LPCSTR lpszString,int nCount);
	BOOL TextOutW (int x,int y,LPCWSTR lpszString,int nCount);
	BOOL TextOut (int x,int y,LPCTSTR lpszString,int nCount);
	//BOOL TextOut(int x, int y, const CString& str);
	BOOL ExtTextOutA (int x,int y,UINT nOptions,LPCRECT lpRect,LPCSTR lpszString,
		UINT nCount,LPINT lpDxWidths);
	BOOL ExtTextOutW (int x,int y,UINT nOptions,LPCRECT lpRect,LPCWSTR lpszString,
		UINT nCount,LPINT lpDxWidths);
	BOOL ExtTextOut (int x,int y,UINT nOptions,LPCRECT lpRect,LPCTSTR lpszString,
		UINT nCount,LPINT lpDxWidths);
	//BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
	//	const CString& str, LPINT lpDxWidths);
	CSize TabbedTextOutA (int x,int y,LPCSTR lpszString,int nCount,int nTabPositions
		,LPINT lpnTabStopPositions,int nTabOrigin);
	CSize TabbedTextOutW (int x,int y,LPCWSTR lpszString,int nCount,int 
		nTabPositions,LPINT lpnTabStopPositions,int nTabOrigin);
	CSize TabbedTextOut (int x,int y,LPCTSTR lpszString,int nCount,int nTabPositions
		,LPINT lpnTabStopPositions,int nTabOrigin);
	//CSize TabbedTextOut(int x, int y, const CString& str,
	//	int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
	int DrawTextA (LPCSTR lpszString,int nCount,LPRECT lpRect,UINT nFormat);
	int DrawTextW (LPCWSTR lpszString,int nCount,LPRECT lpRect,UINT nFormat);
	int DrawText (LPCTSTR lpszString,int nCount,LPRECT lpRect,UINT nFormat);
	//int DrawText(const CString& str, LPRECT lpRect, UINT nFormat);
	CSize GetTextExtentA (LPCSTR lpszString,int nCount)const;
	CSize GetTextExtentW (LPCWSTR lpszString,int nCount)const;
	CSize GetTextExtent (LPCTSTR lpszString,int nCount)const;
	//CSize GetTextExtent(const CString& str) const;
	CSize GetOutputTextExtentA(LPCSTR lpszString,int nCount)const;
	CSize GetOutputTextExtentW(LPCWSTR lpszString,int nCount)const;
	CSize GetOutputTextExtent(LPCTSTR lpszString,int nCount)const;
	//CSize GetOutputTextExtent(const CString& str) const;
	CSize GetTabbedTextExtentA(LPCSTR lpszString,int nCount,int nTabPositions,LPINT
		lpnTabStopPositions)const;
	CSize GetTabbedTextExtentW(LPCWSTR lpszString,int nCount,int nTabPositions,
		LPINT lpnTabStopPositions)const;
	CSize GetTabbedTextExtent(LPCTSTR lpszString,int nCount,int nTabPositions,LPINT
		lpnTabStopPositions)const;
	//CSize GetTabbedTextExtent(const CString& str,
	//	int nTabPositions, LPINT lpnTabStopPositions) const;
	CSize GetOutputTabbedTextExtentA(LPCSTR lpszString,int nCount,int nTabPositions
		,LPINT lpnTabStopPositions)const;
	CSize GetOutputTabbedTextExtentW(LPCWSTR lpszString,int nCount,int 
		nTabPositions,LPINT lpnTabStopPositions)const;
	CSize GetOutputTabbedTextExtent(LPCTSTR lpszString,int nCount,int 
		nTabPositions,LPINT lpnTabStopPositions)const;
	//CSize GetOutputTabbedTextExtent(const CString& str,
	//	int nTabPositions, LPINT lpnTabStopPositions) const;
	BOOL GrayString(CBrush* pBrush, 
		BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData,
			int nCount, int x, int y, int nWidth, int nHeight);
	UINT GetTextAlign() const;
	UINT SetTextAlign(UINT nFlags);
	int GetTextFaceA( int nCount,LPSTR lpszFacename)const;
	int GetTextFaceW( int nCount,LPWSTR lpszFacename)const;
	int GetTextFace( int nCount,LPTSTR lpszFacename)const;
	//int GetTextFace(CString& rString) const;
	BOOL GetTextMetrics(LPTEXTMETRIC lpMetrics) const;
	BOOL GetOutputTextMetrics(LPTEXTMETRIC lpMetrics) const;
	int SetTextJustification(int nBreakExtra, int nBreakCount);
	int GetTextCharacterExtra() const;
	int SetTextCharacterExtra(int nCharExtra);

// Advanced Drawing
#if (WINVER >= 0x400)
	BOOL DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags);
	BOOL DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState);
#endif

// Scrolling Functions
	BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip,
		CRgn* pRgnUpdate, LPRECT lpRectUpdate);

// Font Functions
	BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
	BOOL GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
	DWORD SetMapperFlags(DWORD dwFlag);
	CSize GetAspectRatioFilter() const;

	BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const;
	DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData, DWORD cbData) const;
	int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const;
	UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const;
	DWORD GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
		DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const;

	BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
		LPABCFLOAT lpABCF) const;
	BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar,
		float* lpFloatBuffer) const;

// Printer/Device Escape Functions
	int Escape(int nEscape, int nCount,
		LPCSTR lpszInData, LPVOID lpOutData);
	int EscapeA( int nEscape,int nInputSize,LPCSTR lpszInputData,int nOutputSize,
		LPSTR lpszOutputData);
	int EscapeW( int nEscape,int nInputSize,LPCWSTR lpszInputData,int nOutputSize,
		LPWSTR lpszOutputData);
	int Escape( int nEscape,int nInputSize,LPCTSTR lpszInputData,int nOutputSize,
		LPTSTR lpszOutputData);
	int DrawEscape(int nEscape, int nInputSize, LPCSTR lpszInputData);

	// Escape helpers
	int StartDoc(LPCTSTR lpszDocName);  // old Win3.0 version
	int StartDoc(LPDOCINFO lpDocInfo);
	int StartPage();
	int EndPage();
	int SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int));
	int AbortDoc();
	int EndDoc();

// MetaFile Functions
	BOOL PlayMetaFile(HMETAFILE hMF);
	BOOL PlayMetaFile(HENHMETAFILE hEnhMetaFile, LPCRECT lpBounds);
	BOOL AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData);
		// can be used for enhanced metafiles only

// Path Functions
	BOOL AbortPath();
	BOOL BeginPath();
	BOOL CloseFigure();
	BOOL EndPath();
	BOOL FillPath();
	BOOL FlattenPath();
	BOOL StrokeAndFillPath();
	BOOL StrokePath();
	BOOL WidenPath();
	float GetMiterLimit() const;
	BOOL SetMiterLimit(float fMiterLimit);
	int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const;
	BOOL SelectClipPath(int nMode);

// Misc Helper Functions
	static CBrush PASCAL GetHalftoneBrush();
	void DrawDragRect(LPCRECT lpRect, SIZE size,
		LPCRECT lpRectLast, SIZE sizeLast,
		CBrush* pBrush = NULL, CBrush* pBrushLast = NULL);
	void FillSolidRect(LPCRECT lpRect, COLORREF clr);
	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
	void Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
	void Draw3dRect(int x, int y, int cx, int cy,
		COLORREF clrTopLeft, COLORREF clrBottomRight);
	HGDIOBJ SelectObject(HGDIOBJ);      // do not use for regions

	// used for implementation of non-virtual SelectObject calls
	 CGdiObject SelectGdiObject(HDC hDC, HGDIOBJ h);
};

// CGdiObject
inline CGdiObject::operator HGDIOBJ() const
	{ return this == NULL ? NULL : m_hObject; }
inline HGDIOBJ CGdiObject::GetSafeHandle() const
	{ return this == NULL ? NULL : m_hObject; }
inline CGdiObject::CGdiObject()
	{ m_hObject = NULL; }
inline BOOL CGdiObject::DeleteObject()
	{ return ::DeleteObject(m_hObject); }
inline int CGdiObject::GetObject(int nCount, LPVOID lpObject) const
	{ ASSERT(m_hObject != NULL); return ::GetObject(m_hObject, nCount, lpObject); }
inline BOOL CGdiObject::CreateStockObject(int nIndex)
	{ return (m_hObject = ::GetStockObject(nIndex)) != NULL; }
inline BOOL CGdiObject::UnrealizeObject()
	{ ASSERT(m_hObject != NULL); return ::UnrealizeObject(m_hObject); }
inline UINT CGdiObject::GetObjectType() const
	{ return (UINT)::GetObjectType(m_hObject); }
inline BOOL CGdiObject::operator==(const CGdiObject& obj) const
	{ return ((HGDIOBJ) obj) == m_hObject; }
inline BOOL CGdiObject::operator!=(const CGdiObject& obj) const
	{ return ((HGDIOBJ) obj) != m_hObject; }

// CPen
inline CPen::operator HPEN() const
	{ return (HPEN)(this == NULL ? NULL : m_hObject); }
inline CPen::CPen()
	{ }
inline CPen::CPen(int nPenStyle, int nWidth, COLORREF crColor)
	{ m_hObject = (HGDIOBJ)CreatePen(nPenStyle, nWidth, crColor); }
inline CPen::CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
	int nStyleCount, const DWORD* lpStyle)
	{ m_hObject = (HGDIOBJ)ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount,lpStyle); }
inline BOOL CPen::CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
	{ m_hObject = (HGDIOBJ)::CreatePen(nPenStyle, nWidth, crColor); return (m_hObject != NULL);}
inline BOOL CPen::CreatePenIndirect(LPLOGPEN lpLogPen)
	{  m_hObject = (HGDIOBJ)::CreatePenIndirect(lpLogPen); return (m_hObject != NULL);}
inline BOOL CPen::CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
		int nStyleCount, const DWORD* lpStyle)
	{  m_hObject = (HGDIOBJ)::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount,
		lpStyle); return (m_hObject != NULL);}
inline int CPen::GetExtLogPen(EXTLOGPEN* pLogPen)
	{ ASSERT(m_hObject != NULL);
		return ::GetObject(m_hObject, sizeof(EXTLOGPEN), pLogPen); }
inline int CPen::GetLogPen(LOGPEN* pLogPen)
	{ ASSERT(m_hObject != NULL);
		return ::GetObject(m_hObject, sizeof(LOGPEN), pLogPen); }

// CBrush
inline CBrush::operator HBRUSH() const
	{ return (HBRUSH)(this == NULL ? NULL : m_hObject); }
inline CBrush::CBrush()
	{ }
inline CBrush::CBrush(COLORREF crColor)
	{ m_hObject = (HGDIOBJ)::CreateSolidBrush(crColor);}
inline CBrush::CBrush(int nIndex, COLORREF crColor)
	{ m_hObject = (HGDIOBJ)::CreateHatchBrush(nIndex, crColor);}
inline CBrush::CBrush(CBitmap* pBitmap)
	{ m_hObject = (HGDIOBJ)::CreatePatternBrush((HBITMAP)pBitmap->m_hObject);}
inline BOOL CBrush::CreateSolidBrush(COLORREF crColor)
	{ m_hObject = (HGDIOBJ)::CreateSolidBrush(crColor); return (m_hObject != NULL);}
inline BOOL CBrush::CreateHatchBrush(int nIndex, COLORREF crColor)
	{ m_hObject = (HGDIOBJ)::CreateHatchBrush(nIndex, crColor); return (m_hObject != NULL);}
inline BOOL CBrush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
	{ m_hObject = (HGDIOBJ)::CreateBrushIndirect(lpLogBrush); return (m_hObject != NULL);}
inline BOOL CBrush::CreatePatternBrush(CBitmap* pBitmap)
	{ m_hObject = (HGDIOBJ)::CreatePatternBrush((HBITMAP)pBitmap->GetSafeHandle()); return (m_hObject != NULL);}
inline BOOL CBrush::CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage)
	{ m_hObject = (HGDIOBJ)::CreateDIBPatternBrushPt(lpPackedDIB, nUsage); return (m_hObject != NULL);}
inline BOOL CBrush::CreateSysColorBrush(int nIndex)
	{ m_hObject = (HGDIOBJ)::GetSysColorBrush(nIndex); return (m_hObject != NULL);}
inline BOOL CBrush::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
{ ASSERT(hPackedDIB != NULL); const void* lpPackedDIB = ::GlobalLock(hPackedDIB);
  ASSERT(lpPackedDIB != NULL); m_hObject = (HGDIOBJ)::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
  ::GlobalUnlock(hPackedDIB); return (m_hObject != NULL); }
inline int CBrush::GetLogBrush(LOGBRUSH* pLogBrush)
	{ ASSERT(m_hObject != NULL);
		return ::GetObject(m_hObject, sizeof(LOGBRUSH), pLogBrush); }

// CFont
inline CFont::operator HFONT() const
	{ return (HFONT)(this == NULL ? NULL : m_hObject); }
inline CFont::CFont()
	{ }
inline BOOL CFont::CreateFontIndirect(const LOGFONT* lpLogFont)
	{ m_hObject = (HGDIOBJ)::CreateFontIndirect(lpLogFont); return (m_hObject != NULL); }
inline BOOL CFont::CreateFont(int nHeight, int nWidth, int nEscapement,
		int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
		BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
		BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
		LPCTSTR lpszFacename)
	{ m_hObject = (HGDIOBJ)::CreateFont(nHeight, nWidth, nEscapement,
		nOrientation, nWeight, bItalic, bUnderline, cStrikeOut,
		nCharSet, nOutPrecision, nClipPrecision, nQuality,
		nPitchAndFamily, lpszFacename); return (m_hObject != NULL);}
inline int CFont::GetLogFont(LOGFONT* pLogFont)
	{ ASSERT(m_hObject != NULL);
		return ::GetObject(m_hObject, sizeof(LOGFONT), pLogFont); }

// CBitmap
inline CBitmap::operator HBITMAP() const
	{ return (HBITMAP)(this == NULL ? NULL : m_hObject); }
inline CBitmap::CBitmap()
	{ }
inline BOOL CBitmap::CreateBitmap(int nWidth, int nHeight, UINT nPlanes,
	 UINT nBitcount, const void* lpBits)
	{ m_hObject = (HGDIOBJ)::CreateBitmap(nWidth, nHeight, nPlanes, nBitcount, lpBits); return (m_hObject != NULL);}
inline BOOL CBitmap::CreateBitmapIndirect(LPBITMAP lpBitmap)
	{ m_hObject = (HGDIOBJ)::CreateBitmapIndirect(lpBitmap); return (m_hObject != NULL);}

inline DWORD CBitmap::SetBitmapBits(DWORD dwCount, const void* lpBits)
	{ return ::SetBitmapBits((HBITMAP)m_hObject, dwCount, lpBits); }
inline DWORD CBitmap::GetBitmapBits(DWORD dwCount, LPVOID lpBits) const
	{ return ::GetBitmapBits((HBITMAP)m_hObject, dwCount, lpBits); }
inline BOOL CBitmap::LoadBitmap(LPCTSTR lpszResourceName)
	{ m_hObject = (HGDIOBJ)::LoadBitmap(GetModuleHandle(NULL), lpszResourceName);
	return (m_hObject != NULL);}
//inline BOOL CBitmap::LoadMappedBitmap(UINT nIDBitmap, UINT nFlags,
//	void* lpColorMap, int nMapSize)
//	{ m_hObject = (HGDIOBJ)::CreateMappedBitmap(GetModuleHandle(NULL), 
//	nIDBitmap, (WORD)nFlags, lpColorMap, nMapSize); return (m_hObject != NULL); }
inline CSize CBitmap::SetBitmapDimension(int nWidth, int nHeight)
	{
		SIZE size;
		VERIFY(::SetBitmapDimensionEx((HBITMAP)m_hObject, nWidth, nHeight, &size));
		return size;
	}
inline CSize CBitmap::GetBitmapDimension() const
	{
		SIZE size;
		VERIFY(::GetBitmapDimensionEx((HBITMAP)m_hObject, &size));
		return size;
	}

inline BOOL CBitmap::LoadBitmap(UINT nIDResource)
	{ m_hObject = (HGDIOBJ)::LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(nIDResource)); return (m_hObject != NULL);}
inline BOOL CBitmap::LoadOEMBitmap(UINT nIDBitmap)
	{ m_hObject = (HGDIOBJ)::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap)); return (m_hObject != NULL);}
inline BOOL CBitmap::CreateCompatibleBitmap(CDC* pDC, int nWidth, int nHeight)
	{ m_hObject = (HGDIOBJ)::CreateCompatibleBitmap(pDC->m_hDC, nWidth, nHeight); return (m_hObject != NULL);}
inline BOOL CBitmap::CreateDiscardableBitmap(CDC* pDC, int nWidth, int nHeight)
	{ m_hObject = (HGDIOBJ)::CreateDiscardableBitmap(pDC->m_hDC, nWidth, nHeight); return (m_hObject != NULL);}
inline int CBitmap::GetBitmap(BITMAP* pBitMap)
	{ ASSERT(m_hObject != NULL);
		return ::GetObject(m_hObject, sizeof(BITMAP), pBitMap); }

// CPalette
inline CPalette::operator HPALETTE() const
	{ return (HPALETTE)(this == NULL ? NULL : m_hObject); }
inline CPalette::CPalette()
	{ }
inline BOOL CPalette::CreatePalette(LPLOGPALETTE lpLogPalette)
	{ m_hObject = (HGDIOBJ)::CreatePalette(lpLogPalette); return (m_hObject != NULL);}
inline BOOL CPalette::CreateHalftonePalette(CDC* pDC)
	{ ASSERT(pDC != NULL && pDC->m_hDC != NULL); m_hObject = (HGDIOBJ)
		::CreateHalftonePalette(pDC->m_hDC); return (m_hObject != NULL);}
inline UINT CPalette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
		LPPALETTEENTRY lpPaletteColors) const
	{ ASSERT(m_hObject != NULL); return ::GetPaletteEntries((HPALETTE)m_hObject, nStartIndex,
		nNumEntries, lpPaletteColors); }
inline UINT CPalette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
		LPPALETTEENTRY lpPaletteColors)
	{ ASSERT(m_hObject != NULL); return ::SetPaletteEntries((HPALETTE)m_hObject, nStartIndex,
		nNumEntries, lpPaletteColors); }
inline void CPalette::AnimatePalette(UINT nStartIndex, UINT nNumEntries,
		LPPALETTEENTRY lpPaletteColors)
	{ ASSERT(m_hObject != NULL); ::AnimatePalette((HPALETTE)m_hObject, nStartIndex, nNumEntries,
			lpPaletteColors); }
inline UINT CPalette::GetNearestPaletteIndex(COLORREF crColor) const
	{ ASSERT(m_hObject != NULL); return ::GetNearestPaletteIndex((HPALETTE)m_hObject, crColor); }
inline BOOL CPalette::ResizePalette(UINT nNumEntries)
	{ ASSERT(m_hObject != NULL); return ::ResizePalette((HPALETTE)m_hObject, nNumEntries); }
inline int CPalette::GetEntryCount()
	{ ASSERT(m_hObject != NULL); WORD nEntries;
		 ::GetObject(m_hObject, sizeof(WORD), &nEntries); return (int)nEntries; }

// CRgn
inline CRgn::operator HRGN() const
	{ return (HRGN)(this == NULL ? NULL : m_hObject); }
inline CRgn::CRgn()
	{ }
inline BOOL CRgn::CreateRectRgn(int x1, int y1, int x2, int y2)
	{ m_hObject = (HGDIOBJ)::CreateRectRgn(x1, y1, x2, y2); return (m_hObject != NULL);}
inline BOOL CRgn::CreateRectRgnIndirect(LPCRECT lpRect)
	{ m_hObject = (HGDIOBJ)::CreateRectRgnIndirect(lpRect); return (m_hObject != NULL);}
inline BOOL CRgn::CreateEllipticRgn(int x1, int y1, int x2, int y2)
	{ m_hObject = (HGDIOBJ)::CreateEllipticRgn(x1, y1, x2, y2); return (m_hObject != NULL);}
inline BOOL CRgn::CreateEllipticRgnIndirect(LPCRECT lpRect)
	{ m_hObject = (HGDIOBJ)::CreateEllipticRgnIndirect(lpRect); return (m_hObject != NULL);}
inline BOOL CRgn::CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
	{ m_hObject = (HGDIOBJ)::CreatePolygonRgn(lpPoints, nCount, nMode); return (m_hObject != NULL);}
inline BOOL CRgn::CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode)
	{ m_hObject = (HGDIOBJ)::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode); return (m_hObject != NULL);}
inline BOOL CRgn::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
	{ m_hObject = (HGDIOBJ)::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3); return (m_hObject != NULL);}
inline BOOL CRgn::CreateFromPath(CDC* pDC)
	{ ASSERT(pDC != NULL); m_hObject = (HGDIOBJ)::PathToRegion(pDC->m_hDC); return (m_hObject != NULL);}
inline BOOL CRgn::CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
	{ m_hObject = (HGDIOBJ)::ExtCreateRegion(lpXForm, nCount, pRgnData); return (m_hObject != NULL);}
inline int CRgn::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
	{ ASSERT(m_hObject != NULL); return (int)::GetRegionData((HRGN)m_hObject, nDataSize, lpRgnData); }
inline void CRgn::SetRectRgn(int x1, int y1, int x2, int y2)
	{ ASSERT(m_hObject != NULL); ::SetRectRgn((HRGN)m_hObject, x1, y1, x2, y2); }
inline void CRgn::SetRectRgn(LPCRECT lpRect)
	{ ASSERT(m_hObject != NULL); ::SetRectRgn((HRGN)m_hObject, lpRect->left, lpRect->top,
		lpRect->right, lpRect->bottom); }
inline int CRgn::CombineRgn(CRgn* pRgn1, CRgn* pRgn2, int nCombineMode)
	{ ASSERT(m_hObject != NULL); return ::CombineRgn((HRGN)m_hObject, (HRGN)pRgn1->GetSafeHandle(),
		(HRGN)pRgn2->GetSafeHandle(), nCombineMode); }
inline int CRgn::CopyRgn1(CRgn* pRgnSrc)
	{ ASSERT(m_hObject != NULL); return ::CombineRgn((HRGN)m_hObject, (HRGN)pRgnSrc->GetSafeHandle(), NULL, RGN_COPY); }
inline BOOL CRgn::EqualRgn(CRgn* pRgn) const
	{ ASSERT(m_hObject != NULL); return ::EqualRgn((HRGN)m_hObject, (HRGN)pRgn->GetSafeHandle()); }
inline int CRgn::OffsetRgn(int x, int y)
	{ ASSERT(m_hObject != NULL); return ::OffsetRgn((HRGN)m_hObject, x, y); }
inline int CRgn::OffsetRgn(POINT point)
	{ ASSERT(m_hObject != NULL); return ::OffsetRgn((HRGN)m_hObject, point.x, point.y); }
inline int CRgn::GetRgnBox(LPRECT lpRect) const
	{ ASSERT(m_hObject != NULL); return ::GetRgnBox((HRGN)m_hObject, lpRect); }
inline BOOL CRgn::PtInRegion(int x, int y) const
	{ ASSERT(m_hObject != NULL); return ::PtInRegion((HRGN)m_hObject, x, y); }
inline BOOL CRgn::PtInRegion(POINT point) const
	{ ASSERT(m_hObject != NULL); return ::PtInRegion((HRGN)m_hObject, point.x, point.y); }
inline BOOL CRgn::RectInRegion(LPCRECT lpRect) const
	{ ASSERT(m_hObject != NULL); return ::RectInRegion((HRGN)m_hObject, lpRect); }

// CDC
inline CDC::operator HDC() const
	{ return this == NULL ? NULL : m_hDC; }
inline HDC CDC::GetSafeHdc() const
	{ return this == NULL ? NULL : m_hDC; }
inline CWnd CDC::GetWindow() const
	{ ASSERT(m_hDC != NULL); return CWnd(::WindowFromDC(m_hDC)); }
inline BOOL CDC::CreateDC(LPCTSTR lpszDriverName,
	LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData)
	{ m_hDC = ::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput,
	(const DEVMODE*)lpInitData); return (m_hDC != NULL);}
inline BOOL CDC::CreateIC(LPCTSTR lpszDriverName,
	LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData)
	{ m_hDC = ::CreateIC(lpszDriverName, lpszDeviceName, lpszOutput, 
	(const DEVMODE*) lpInitData); return (m_hDC != NULL);}
inline BOOL CDC::CreateCompatibleDC(CDC* pDC)
	{ m_hDC = ::CreateCompatibleDC(pDC->GetSafeHdc()); return (m_hDC != NULL); }
inline int CDC::ExcludeUpdateRgn(CWnd pWnd)
	{ ASSERT(m_hDC != NULL); return ::ExcludeUpdateRgn(m_hDC, pWnd._hwnd); }
inline int CDC::GetDeviceCaps(int nIndex) const
	{ ASSERT(m_hDC != NULL); return ::GetDeviceCaps(m_hDC, nIndex); }
inline CPoint CDC::GetBrushOrg() const
	{
		ASSERT(m_hDC != NULL);
		POINT point;
		VERIFY(::GetBrushOrgEx(m_hDC, &point));
		return point;
	}
inline CPoint CDC::SetBrushOrg(int x, int y)
	{
		ASSERT(m_hDC != NULL);
		POINT point;
		VERIFY(::SetBrushOrgEx(m_hDC, x, y, &point));
		return point;
	}
inline CPoint CDC::SetBrushOrg(POINT point)
	{
		ASSERT(m_hDC != NULL);
		VERIFY(::SetBrushOrgEx(m_hDC, point.x, point.y, &point));
		return point;
	}

inline int CDC::EnumObjects(int nObjectType,
		int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
	{ ASSERT(m_hDC != NULL); return ::EnumObjects(m_hDC, nObjectType, (GOBJENUMPROC)lpfn, lpData); }
inline CBitmap CDC::SelectObject(CBitmap* pBitmap)
	{ ASSERT(m_hDC != NULL); return (CBitmap)SelectGdiObject(m_hDC, pBitmap->GetSafeHandle()); }
inline CGdiObject CDC::SelectObject(CGdiObject* pObject)
	{ ASSERT(m_hDC != NULL); return SelectGdiObject(m_hDC, pObject->GetSafeHandle()); }
inline HGDIOBJ CDC::SelectObject(HGDIOBJ hObject) // Safe for NULL handles
	{ ASSERT(m_hDC == m_hDC); // ASSERT a simple CDC object
		return (hObject != NULL) ? ::SelectObject(m_hDC, hObject) : NULL; }
inline COLORREF CDC::GetNearestColor(COLORREF crColor) const
	{ ASSERT(m_hDC != NULL); return ::GetNearestColor(m_hDC, crColor); }
inline UINT CDC::RealizePalette()
	{ ASSERT(m_hDC != NULL); return ::RealizePalette(m_hDC); }
inline void CDC::UpdateColors()
	{ ASSERT(m_hDC != NULL); ::UpdateColors(m_hDC); }
inline COLORREF CDC::GetBkColor() const
	{ ASSERT(m_hDC != NULL); return ::GetBkColor(m_hDC); }
inline int CDC::GetBkMode() const
	{ ASSERT(m_hDC != NULL); return ::GetBkMode(m_hDC); }
inline int CDC::GetPolyFillMode() const
	{ ASSERT(m_hDC != NULL); return ::GetPolyFillMode(m_hDC); }
inline int CDC::GetROP2() const
	{ ASSERT(m_hDC != NULL); return ::GetROP2(m_hDC); }
inline int CDC::GetStretchBltMode() const
	{ ASSERT(m_hDC != NULL); return ::GetStretchBltMode(m_hDC); }
inline COLORREF CDC::GetTextColor() const
	{ ASSERT(m_hDC != NULL); return ::GetTextColor(m_hDC); }
inline int CDC::GetMapMode() const
	{ ASSERT(m_hDC != NULL); return ::GetMapMode(m_hDC); }

inline CPoint CDC::GetViewportOrg() const
	{
		ASSERT(m_hDC != NULL);
		POINT point;
		VERIFY(::GetViewportOrgEx(m_hDC, &point));
		return point;
	}
inline CSize CDC::GetViewportExt() const
	{
		ASSERT(m_hDC != NULL);
		SIZE size;
		VERIFY(::GetViewportExtEx(m_hDC, &size));
		return size;
	}
inline CPoint CDC::GetWindowOrg() const
	{
		ASSERT(m_hDC != NULL);
		POINT point;
		VERIFY(::GetWindowOrgEx(m_hDC, &point));
		return point;
	}
inline CSize CDC::GetWindowExt() const
	{
		ASSERT(m_hDC != NULL);
		SIZE size;
		VERIFY(::GetWindowExtEx(m_hDC, &size));
		return size;
	}

// non-virtual helpers calling virtual mapping functions
inline CPoint CDC::SetViewportOrg(POINT point)
	{ ASSERT(m_hDC != NULL); return SetViewportOrg(point.x, point.y); }
inline CSize CDC::SetViewportExt(SIZE size)
	{ ASSERT(m_hDC != NULL); return SetViewportExt(size.cx, size.cy); }
inline CPoint CDC::SetWindowOrg(POINT point)
	{ ASSERT(m_hDC != NULL); return SetWindowOrg(point.x, point.y); }
inline CSize CDC::SetWindowExt(SIZE size)
	{ ASSERT(m_hDC != NULL); return SetWindowExt(size.cx, size.cy); }

inline void CDC::DPtoLP(LPPOINT lpPoints, int nCount) const
	{ ASSERT(m_hDC != NULL); VERIFY(::DPtoLP(m_hDC, lpPoints, nCount)); }
inline void CDC::DPtoLP(LPRECT lpRect) const
	{ ASSERT(m_hDC != NULL); VERIFY(::DPtoLP(m_hDC, (LPPOINT)lpRect, 2)); }
inline void CDC::LPtoDP(LPPOINT lpPoints, int nCount) const
	{ ASSERT(m_hDC != NULL); VERIFY(::LPtoDP(m_hDC, lpPoints, nCount)); }
inline void CDC::LPtoDP(LPRECT lpRect) const
	{ ASSERT(m_hDC != NULL); VERIFY(::LPtoDP(m_hDC, (LPPOINT)lpRect, 2)); }

inline BOOL CDC::FillRgn(CRgn* pRgn, CBrush* pBrush)
	{ ASSERT(m_hDC != NULL); return ::FillRgn(m_hDC, (HRGN)pRgn->GetSafeHandle(), (HBRUSH)pBrush->GetSafeHandle()); }
inline BOOL CDC::FrameRgn(CRgn* pRgn, CBrush* pBrush, int nWidth, int nHeight)
	{ ASSERT(m_hDC != NULL); return ::FrameRgn(m_hDC, (HRGN)pRgn->GetSafeHandle(), (HBRUSH)pBrush->GetSafeHandle(),
		nWidth, nHeight); }
inline BOOL CDC::InvertRgn(CRgn* pRgn)
	{ ASSERT(m_hDC != NULL); return ::InvertRgn(m_hDC, (HRGN)pRgn->GetSafeHandle()); }
inline BOOL CDC::PaintRgn(CRgn* pRgn)
	{ ASSERT(m_hDC != NULL); return ::PaintRgn(m_hDC, (HRGN)pRgn->GetSafeHandle()); }
inline BOOL CDC::PtVisible(int x, int y) const
	{ ASSERT(m_hDC != NULL); return ::PtVisible(m_hDC, x, y); }
inline BOOL CDC::PtVisible(POINT point) const
	{ ASSERT(m_hDC != NULL); return PtVisible(point.x, point.y); } // call virtual
inline BOOL CDC::RectVisible(LPCRECT lpRect) const
	{ ASSERT(m_hDC != NULL); return ::RectVisible(m_hDC, lpRect); }
inline CPoint CDC::GetCurrentPosition() const
	{
		ASSERT(m_hDC != NULL);
		POINT point;
		VERIFY(::GetCurrentPositionEx(m_hDC, &point));
		return point;
	}

inline CPoint CDC::MoveTo(POINT point)
	{ ASSERT(m_hDC != NULL); return MoveTo(point.x, point.y); }
inline BOOL CDC::LineTo(POINT point)
	{ ASSERT(m_hDC != NULL); return LineTo(point.x, point.y); }
inline BOOL CDC::Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{ ASSERT(m_hDC != NULL); return ::Arc(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4); }
inline BOOL CDC::Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
	{ ASSERT(m_hDC != NULL); return ::Arc(m_hDC, lpRect->left, lpRect->top,
		lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
		ptEnd.x, ptEnd.y); }
inline BOOL CDC::Polyline(LPPOINT lpPoints, int nCount)
	{ ASSERT(m_hDC != NULL); return ::Polyline(m_hDC, lpPoints, nCount); }
inline void CDC::FillRect(LPCRECT lpRect, CBrush* pBrush)
	{ ASSERT(m_hDC != NULL); ::FillRect(m_hDC, lpRect, (HBRUSH)pBrush->GetSafeHandle()); }
inline void CDC::FrameRect(LPCRECT lpRect, CBrush* pBrush)
	{ ASSERT(m_hDC != NULL); ::FrameRect(m_hDC, lpRect, (HBRUSH)pBrush->GetSafeHandle()); }
inline void CDC::InvertRect(LPCRECT lpRect)
	{ ASSERT(m_hDC != NULL); ::InvertRect(m_hDC, lpRect); }
inline BOOL CDC::DrawIcon(int x, int y, HICON hIcon)
	{ ASSERT(m_hDC != NULL); return ::DrawIcon(m_hDC, x, y, hIcon); }
inline BOOL CDC::DrawIcon(POINT point, HICON hIcon)
	{ ASSERT(m_hDC != NULL); return ::DrawIcon(m_hDC, point.x, point.y, hIcon); }
#if (WINVER >= 0x400)
inline BOOL CDC::DrawState(CPoint pt, CSize size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush)
	{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, hBrush,
		NULL, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP); }
inline BOOL CDC::DrawState(CPoint pt, CSize size, CBitmap* pBitmap, UINT nFlags, CBrush* pBrush)
	{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, (HBRUSH)pBrush->GetSafeHandle(),
		NULL, (LPARAM)pBitmap->GetSafeHandle(), 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP); }
inline BOOL CDC::DrawState(CPoint pt, CSize size, HICON hIcon, UINT nFlags, HBRUSH hBrush)
	{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, hBrush, NULL,
		(LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON); }
inline BOOL CDC::DrawState(CPoint pt, CSize size, HICON hIcon, UINT nFlags, CBrush* pBrush)
	{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, (HBRUSH)pBrush->GetSafeHandle(), NULL,
		(LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON); }
inline BOOL CDC::DrawState(CPoint pt, CSize size, LPCTSTR lpszText, UINT nFlags, BOOL bPrefixText, int nTextLen, HBRUSH hBrush)
	{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, hBrush,
		NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)); }
inline BOOL CDC::DrawState(CPoint pt, CSize size, LPCTSTR lpszText, UINT nFlags, BOOL bPrefixText, int nTextLen, CBrush* pBrush)
	{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, (HBRUSH)pBrush->GetSafeHandle(),
		NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)); }
inline BOOL CDC::DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush)
	{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, hBrush,
		lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX); }
inline BOOL CDC::DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, CBrush* pBrush)
	{ ASSERT(m_hDC != NULL); return ::DrawState(m_hDC, (HBRUSH)pBrush->GetSafeHandle(),
		lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX); }
inline BOOL CDC::DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags)
	{ ASSERT(m_hDC != NULL); return ::DrawEdge(m_hDC, lpRect, nEdge, nFlags); }
inline BOOL CDC::DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState)
	{ ASSERT(m_hDC != NULL); return ::DrawFrameControl(m_hDC, lpRect, nType, nState); }
#endif

inline BOOL CDC::Chord(int x1, int y1, int x2, int y2, int x3, int y3,
	int x4, int y4)
	{ ASSERT(m_hDC != NULL); return ::Chord(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4); }
inline BOOL CDC::Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
	{ ASSERT(m_hDC != NULL); return ::Chord(m_hDC, lpRect->left, lpRect->top,
		lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
		ptEnd.x, ptEnd.y); }
inline void CDC::DrawFocusRect(LPCRECT lpRect)
	{ ASSERT(m_hDC != NULL); ::DrawFocusRect(m_hDC, lpRect); }
inline BOOL CDC::Ellipse(int x1, int y1, int x2, int y2)
	{ ASSERT(m_hDC != NULL); return ::Ellipse(m_hDC, x1, y1, x2, y2); }
inline BOOL CDC::Ellipse(LPCRECT lpRect)
	{ ASSERT(m_hDC != NULL); return ::Ellipse(m_hDC, lpRect->left, lpRect->top,
		lpRect->right, lpRect->bottom); }
inline BOOL CDC::Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{ ASSERT(m_hDC != NULL); return ::Pie(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4); }
inline BOOL CDC::Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
	{ ASSERT(m_hDC != NULL); return ::Pie(m_hDC, lpRect->left, lpRect->top,
		lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
		ptEnd.x, ptEnd.y); }
inline BOOL CDC::Polygon(LPPOINT lpPoints, int nCount)
	{ ASSERT(m_hDC != NULL); return ::Polygon(m_hDC, lpPoints, nCount); }
inline BOOL CDC::PolyPolygon(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount)
	{ ASSERT(m_hDC != NULL); return ::PolyPolygon(m_hDC, lpPoints, lpPolyCounts, nCount); }
inline BOOL CDC::Rectangle(int x1, int y1, int x2, int y2)
	{ ASSERT(m_hDC != NULL); return ::Rectangle(m_hDC, x1, y1, x2, y2); }
inline BOOL CDC::Rectangle(LPCRECT lpRect)
	{ ASSERT(m_hDC != NULL); return ::Rectangle(m_hDC, lpRect->left, lpRect->top,
		lpRect->right, lpRect->bottom); }
inline BOOL CDC::RoundRect(int x1, int y1, int x2, int y2, int x3, int y3)
	{ ASSERT(m_hDC != NULL); return ::RoundRect(m_hDC, x1, y1, x2, y2, x3, y3); }
inline BOOL CDC::RoundRect(LPCRECT lpRect, POINT point)
	{ ASSERT(m_hDC != NULL); return ::RoundRect(m_hDC, lpRect->left, lpRect->top,
		lpRect->right, lpRect->bottom, point.x, point.y); }
inline BOOL CDC::PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop)
	{ ASSERT(m_hDC != NULL); return ::PatBlt(m_hDC, x, y, nWidth, nHeight, dwRop); }
inline BOOL CDC::BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
	int xSrc, int ySrc, DWORD dwRop)
	{ ASSERT(m_hDC != NULL); return ::BitBlt(m_hDC, x, y, nWidth, nHeight,
		pSrcDC->GetSafeHdc(), xSrc, ySrc, dwRop); }
inline BOOL CDC::StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
	int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop)
	{ ASSERT(m_hDC != NULL); return ::StretchBlt(m_hDC, x, y, nWidth, nHeight,
		pSrcDC->GetSafeHdc(), xSrc, ySrc, nSrcWidth, nSrcHeight,
		dwRop); }
inline COLORREF CDC::GetPixel(int x, int y) const
	{ ASSERT(m_hDC != NULL); return ::GetPixel(m_hDC, x, y); }
inline COLORREF CDC::GetPixel(POINT point) const
	{ ASSERT(m_hDC != NULL); return ::GetPixel(m_hDC, point.x, point.y); }
inline COLORREF CDC::SetPixel(int x, int y, COLORREF crColor)
	{ ASSERT(m_hDC != NULL); return ::SetPixel(m_hDC, x, y, crColor); }
inline COLORREF CDC::SetPixel(POINT point, COLORREF crColor)
	{ ASSERT(m_hDC != NULL); return ::SetPixel(m_hDC, point.x, point.y, crColor); }
inline BOOL CDC::FloodFill(int x, int y, COLORREF crColor)
	{ ASSERT(m_hDC != NULL); return ::FloodFill(m_hDC, x, y, crColor); }
inline BOOL CDC::ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType)
	{ ASSERT(m_hDC != NULL); return ::ExtFloodFill(m_hDC, x, y, crColor, nFillType); }
inline BOOL CDC::TextOut(int x, int y, LPCTSTR lpszString, int nCount)
	{ ASSERT(m_hDC != NULL); return ::TextOut(m_hDC, x, y, lpszString, nCount); }
//inline BOOL CDC::TextOut(int x, int y, const CString& str)
//	{ ASSERT(m_hDC != NULL); return TextOut(x, y, (LPCTSTR)str, str.GetLength()); } // call virtual
inline BOOL CDC::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
	LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths)
	{ ASSERT(m_hDC != NULL); return ::ExtTextOut(m_hDC, x, y, nOptions, lpRect,
		lpszString, nCount, lpDxWidths); }
//inline BOOL CDC::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
//	const CString& str, LPINT lpDxWidths)
//	{ ASSERT(m_hDC != NULL); return ::ExtTextOut(m_hDC, x, y, nOptions, lpRect,
//		str, str.GetLength(), lpDxWidths); }
inline CSize CDC::TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount,
	int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
	{ ASSERT(m_hDC != NULL); return ::TabbedTextOut(m_hDC, x, y, lpszString, nCount,
		nTabPositions, lpnTabStopPositions, nTabOrigin); }
//inline CSize CDC::TabbedTextOut(int x, int y, const CString& str,
//	int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
//	{ ASSERT(m_hDC != NULL); return ::TabbedTextOut(m_hDC, x, y, str, str.GetLength(),
//		nTabPositions, lpnTabStopPositions, nTabOrigin); }
inline int CDC::DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect,
		UINT nFormat)
	{ ASSERT(m_hDC != NULL);
		return ::DrawText(m_hDC, lpszString, nCount, lpRect, nFormat); }
//inline int CDC::DrawText(const CString& str, LPRECT lpRect, UINT nFormat)
//	{ ASSERT(m_hDC != NULL);
//		// these flags would modify the string
//		ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
//		ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
//		return DrawText((LPCTSTR)str, str.GetLength(), lpRect, nFormat); }
inline CSize CDC::GetTextExtent(LPCTSTR lpszString, int nCount) const
	{
		ASSERT(m_hDC != NULL);
		SIZE size;
		VERIFY(::GetTextExtentPoint32(m_hDC, lpszString, nCount, &size));
		return size;
	}
//inline CSize CDC::GetTextExtent(const CString& str) const
//	{
//		ASSERT(m_hDC != NULL);
//		SIZE size;
//		VERIFY(::GetTextExtentPoint32(m_hDC, str, str.GetLength(), &size));
//		return size;
//	}

inline CSize CDC::GetOutputTextExtent(LPCTSTR lpszString, int nCount) const
	{
		ASSERT(m_hDC != NULL);
		SIZE size;
		VERIFY(::GetTextExtentPoint32(m_hDC, lpszString, nCount, &size));
		return size;
	}
//inline CSize CDC::GetOutputTextExtent(const CString& str) const
//	{
//		ASSERT(m_hDC != NULL);
//		SIZE size;
//		VERIFY(::GetTextExtentPoint32(m_hDC, str, str.GetLength(), &size));
//		return size;
//	}

inline CSize CDC::GetTabbedTextExtent(LPCTSTR lpszString, int nCount,
	int nTabPositions, LPINT lpnTabStopPositions) const
	{ ASSERT(m_hDC != NULL); return ::GetTabbedTextExtent(m_hDC, lpszString, nCount,
		nTabPositions, lpnTabStopPositions); }
//inline  CSize CDC::GetTabbedTextExtent(const CString& str,
//		int nTabPositions, LPINT lpnTabStopPositions) const
//	{ ASSERT(m_hDC != NULL); return ::GetTabbedTextExtent(m_hDC,
//		str, str.GetLength(), nTabPositions, lpnTabStopPositions); }
inline CSize CDC::GetOutputTabbedTextExtent(LPCTSTR lpszString, int nCount,
	int nTabPositions, LPINT lpnTabStopPositions) const
	{ ASSERT(m_hDC != NULL); return ::GetTabbedTextExtent(m_hDC, lpszString, nCount,
		nTabPositions, lpnTabStopPositions); }
//inline  CSize CDC::GetOutputTabbedTextExtent(const CString& str,
//		int nTabPositions, LPINT lpnTabStopPositions) const
//	{ ASSERT(m_hDC != NULL); return ::GetTabbedTextExtent(m_hDC,
//		str, str.GetLength(), nTabPositions, lpnTabStopPositions); }
inline BOOL CDC::GrayString(CBrush* pBrush,
	BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int),
		LPARAM lpData, int nCount,
		int x, int y, int nWidth, int nHeight)
	{ ASSERT(m_hDC != NULL); return ::GrayString(m_hDC, (HBRUSH)pBrush->GetSafeHandle(),
		(GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight); }
inline UINT CDC::GetTextAlign() const
	{ ASSERT(m_hDC != NULL); return ::GetTextAlign(m_hDC); }
inline int CDC::GetTextFace(int nCount, LPTSTR lpszFacename) const
	{ ASSERT(m_hDC != NULL); return ::GetTextFace(m_hDC, nCount, lpszFacename); }
//inline  int CDC::GetTextFace(CString& rString) const
//	{ ASSERT(m_hDC != NULL); int nResult = ::GetTextFace(m_hDC,
//		256, rString.GetBuffer(256)); rString.ReleaseBuffer();
//		return nResult; }
inline BOOL CDC::GetTextMetrics(LPTEXTMETRIC lpMetrics) const
	{ ASSERT(m_hDC != NULL); return ::GetTextMetrics(m_hDC, lpMetrics); }
inline BOOL CDC::GetOutputTextMetrics(LPTEXTMETRIC lpMetrics) const
	{ ASSERT(m_hDC != NULL); return ::GetTextMetrics(m_hDC, lpMetrics); }
inline int CDC::GetTextCharacterExtra() const
	{ ASSERT(m_hDC != NULL); return ::GetTextCharacterExtra(m_hDC); }
inline BOOL CDC::GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
	{ ASSERT(m_hDC != NULL); return ::GetCharWidth(m_hDC, nFirstChar, nLastChar, lpBuffer); }
inline BOOL CDC::GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
	{ ASSERT(m_hDC != NULL); return ::GetCharWidth(m_hDC, nFirstChar, nLastChar, lpBuffer); }
inline CSize CDC::GetAspectRatioFilter() const
	{
		ASSERT(m_hDC != NULL);
		SIZE size;
		VERIFY(::GetAspectRatioFilterEx(m_hDC, &size));
		return size;
	}
inline BOOL CDC::ScrollDC(int dx, int dy,
		LPCRECT lpRectScroll, LPCRECT lpRectClip,
		CRgn* pRgnUpdate, LPRECT lpRectUpdate)
	{ ASSERT(m_hDC != NULL); return ::ScrollDC(m_hDC, dx, dy, lpRectScroll,
		lpRectClip, (HRGN)pRgnUpdate->GetSafeHandle(), lpRectUpdate); }

// Printer Escape Functions
inline int CDC::Escape(int nEscape, int nCount, LPCSTR lpszInData, LPVOID lpOutData)
	{ ASSERT(m_hDC != NULL); return ::Escape(m_hDC, nEscape, nCount, lpszInData, lpOutData);}

// CDC 3.1 Specific functions
inline UINT CDC::SetBoundsRect(LPCRECT lpRectBounds, UINT flags)
	{ ASSERT(m_hDC != NULL); return ::SetBoundsRect(m_hDC, lpRectBounds, flags); }
inline UINT CDC::GetBoundsRect(LPRECT lpRectBounds, UINT flags)
	{ ASSERT(m_hDC != NULL); return ::GetBoundsRect(m_hDC, lpRectBounds, flags); }
inline BOOL CDC::ResetDC(const DEVMODE* lpDevMode)
	{ ASSERT(m_hDC != NULL); return ::ResetDC(m_hDC, lpDevMode) != NULL; }
inline UINT CDC::GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const
	{ ASSERT(m_hDC != NULL); return ::GetOutlineTextMetrics(m_hDC, cbData, lpotm); }
inline BOOL CDC::GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const
	{ ASSERT(m_hDC != NULL); return ::GetCharABCWidths(m_hDC, nFirstChar, nLastChar, lpabc); }
inline DWORD CDC::GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData,
	DWORD cbData) const
	{ ASSERT(m_hDC != NULL); return ::GetFontData(m_hDC, dwTable, dwOffset, lpData, cbData); }
inline int CDC::GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const
	{ ASSERT(m_hDC != NULL); return ::GetKerningPairs(m_hDC, nPairs, lpkrnpair); }
inline DWORD CDC::GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
		DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const
	{ ASSERT(m_hDC != NULL); return ::GetGlyphOutline(m_hDC, nChar, nFormat,
			lpgm, cbBuffer, lpBuffer, lpmat2); }

// Document handling functions
inline int CDC::StartDoc(LPDOCINFO lpDocInfo)
	{ ASSERT(m_hDC != NULL); return ::StartDoc(m_hDC, lpDocInfo); }
inline int CDC::StartPage()
	{ ASSERT(m_hDC != NULL); return ::StartPage(m_hDC); }
inline int CDC::EndPage()
	{ ASSERT(m_hDC != NULL); return ::EndPage(m_hDC); }
inline int CDC::SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int))
	{ ASSERT(m_hDC != NULL); return ::SetAbortProc(m_hDC, (ABORTPROC)lpfn); }
inline int CDC::AbortDoc()
	{ ASSERT(m_hDC != NULL); return ::AbortDoc(m_hDC); }
inline int CDC::EndDoc()
	{ ASSERT(m_hDC != NULL); return ::EndDoc(m_hDC); }

inline BOOL CDC::MaskBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
		int xSrc, int ySrc, CBitmap& maskBitmap, int xMask, int yMask, DWORD dwRop)
	{ ASSERT(m_hDC != NULL); return ::MaskBlt(m_hDC, x, y, nWidth, nHeight, pSrcDC->GetSafeHdc(),
		xSrc, ySrc,  (HBITMAP)maskBitmap.m_hObject, xMask, yMask, dwRop); }
inline BOOL CDC::PlgBlt(LPPOINT lpPoint, CDC* pSrcDC, int xSrc, int ySrc,
		int nWidth, int nHeight, CBitmap& maskBitmap, int xMask, int yMask)
	{ ASSERT(m_hDC != NULL); return ::PlgBlt(m_hDC, lpPoint, pSrcDC->GetSafeHdc(), xSrc, ySrc, nWidth,
		nHeight, (HBITMAP)maskBitmap.m_hObject, xMask, yMask); }
inline BOOL CDC::SetPixelV(int x, int y, COLORREF crColor)
	{ ASSERT(m_hDC != NULL); return ::SetPixelV(m_hDC, x, y, crColor); }
inline BOOL CDC::SetPixelV(POINT point, COLORREF crColor)
	{ ASSERT(m_hDC != NULL); return ::SetPixelV(m_hDC, point.x, point.y, crColor); }
inline BOOL CDC::AngleArc(int x, int y, int nRadius,
		float fStartAngle, float fSweepAngle)
	{ ASSERT(m_hDC != NULL); return ::AngleArc(m_hDC, x, y, nRadius, fStartAngle, fSweepAngle); }
inline BOOL CDC::ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
	{ ASSERT(m_hDC != NULL); return ArcTo(lpRect->left, lpRect->top, lpRect->right,
		lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y); }
inline int CDC::GetArcDirection() const
	{ ASSERT(m_hDC != NULL); return ::GetArcDirection(m_hDC); }
inline BOOL CDC::PolyPolyline(const POINT* lpPoints, const DWORD* lpPolyPoints,
		int nCount)
	{ ASSERT(m_hDC != NULL); return ::PolyPolyline(m_hDC, lpPoints, lpPolyPoints, nCount); }
inline BOOL CDC::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const
	{ ASSERT(m_hDC != NULL); return ::GetColorAdjustment(m_hDC, lpColorAdjust); }
inline CPen CDC::GetCurrentPen() const
	{ ASSERT(m_hDC != NULL); return CPen(::GetCurrentObject(m_hDC, OBJ_PEN)); }
inline CBrush CDC::GetCurrentBrush() const
	{ ASSERT(m_hDC != NULL); return CBrush(::GetCurrentObject(m_hDC, OBJ_BRUSH)); }
inline CPalette CDC::GetCurrentPalette() const
	{ ASSERT(m_hDC != NULL); return CPalette(::GetCurrentObject(m_hDC, OBJ_PAL)); }
inline CFont CDC::GetCurrentFont() const
	{ ASSERT(m_hDC != NULL); return CFont(::GetCurrentObject(m_hDC, OBJ_FONT)); }
inline CBitmap CDC::GetCurrentBitmap() const
	{ ASSERT(m_hDC != NULL); return CBitmap(GetCurrentObject(m_hDC, OBJ_BITMAP)); }
inline BOOL CDC::PolyBezier(const POINT* lpPoints, int nCount)
	{ ASSERT(m_hDC != NULL); return ::PolyBezier(m_hDC, lpPoints, nCount); }

inline int CDC::DrawEscape(int nEscape, int nInputSize, LPCSTR lpszInputData)
	{ ASSERT(m_hDC != NULL); return ::DrawEscape(m_hDC, nEscape, nInputSize, lpszInputData); }
//inline int CDC::Escape(int nEscape, int nInputSize, LPCSTR lpszInputData,
//		int nOutputSize, LPSTR lpszOutputData)
//	{ ASSERT(m_hDC != NULL); return ::ExtEscape(m_hDC, nEscape, nInputSize, lpszInputData,
//		nOutputSize, lpszOutputData); }

inline BOOL CDC::GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
		LPABCFLOAT lpABCF) const
	{ ASSERT(m_hDC != NULL); return ::GetCharABCWidthsFloat(m_hDC, nFirstChar, nLastChar, lpABCF); }
inline BOOL CDC::GetCharWidth(UINT nFirstChar, UINT nLastChar,
		float* lpFloatBuffer) const
	{ ASSERT(m_hDC != NULL); return ::GetCharWidthFloat(m_hDC, nFirstChar, nLastChar, lpFloatBuffer); }

inline BOOL CDC::AbortPath()
	{ ASSERT(m_hDC != NULL); return ::AbortPath(m_hDC); }
inline BOOL CDC::BeginPath()
	{ ASSERT(m_hDC != NULL); return ::BeginPath(m_hDC); }
inline BOOL CDC::CloseFigure()
	{ ASSERT(m_hDC != NULL); return ::CloseFigure(m_hDC); }
inline BOOL CDC::EndPath()
	{ ASSERT(m_hDC != NULL); return ::EndPath(m_hDC); }
inline BOOL CDC::FillPath()
	{ ASSERT(m_hDC != NULL); return ::FillPath(m_hDC); }
inline BOOL CDC::FlattenPath()
	{ ASSERT(m_hDC != NULL); return ::FlattenPath(m_hDC); }
inline float CDC::GetMiterLimit() const
	{ ASSERT(m_hDC != NULL); float fMiterLimit;
		VERIFY(::GetMiterLimit(m_hDC, &fMiterLimit)); return fMiterLimit; }
inline int CDC::GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const
	{ ASSERT(m_hDC != NULL); return ::GetPath(m_hDC, lpPoints, lpTypes, nCount); }
inline BOOL CDC::SetMiterLimit(float fMiterLimit)
	{ ASSERT(m_hDC != NULL); return ::SetMiterLimit(m_hDC, fMiterLimit, NULL); }
inline BOOL CDC::StrokeAndFillPath()
	{ ASSERT(m_hDC != NULL); return ::StrokeAndFillPath(m_hDC); }
inline BOOL CDC::StrokePath()
	{ ASSERT(m_hDC != NULL); return ::StrokePath(m_hDC); }
inline BOOL CDC::WidenPath()
	{ ASSERT(m_hDC != NULL); return ::WidenPath(m_hDC); }

inline BOOL CDC::AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData)
	{ ASSERT(m_hDC != NULL); return ::GdiComment(m_hDC, nDataSize, pCommentData); }
inline BOOL CDC::PlayMetaFile(HENHMETAFILE hEnhMF, LPCRECT lpBounds)
	{ ASSERT(m_hDC != NULL); return ::PlayEnhMetaFile(m_hDC, hEnhMF, lpBounds); }
inline BOOL CDC::DeleteDC()
	{ return ::DeleteDC(m_hDC); }
inline void CDC::SetAttribDC(HDC hDC)  // Set the Attribute DC
	{ m_hDC = hDC; }
inline void CDC::SetOutputDC(HDC hDC)  // Set the Output DC
	{	m_hDC = hDC; }
inline void CDC::ReleaseAttribDC()     // Release the Attribute DC
{	m_hDC = NULL; }
inline void CDC::ReleaseOutputDC()     // Release the Output DC
{	m_hDC = NULL; }
inline int CDC::SaveDC()
{	ASSERT(m_hDC != NULL);	return ::SaveDC(m_hDC); }
inline BOOL CDC::RestoreDC(int nSavedDC)
{	return ::RestoreDC(m_hDC, nSavedDC); }
inline CGdiObject CDC::SelectGdiObject(HDC hDC, HGDIOBJ h)
{	return CGdiObject(::SelectObject(hDC, h)); }
inline CGdiObject CDC::SelectStockObject(int nIndex)
{	ASSERT(m_hDC != NULL); 	HGDIOBJ hObject = ::GetStockObject(nIndex);
	ASSERT(hObject != NULL); return CGdiObject(::SelectObject(m_hDC, hObject));
}
inline CPen CDC::SelectObject(CPen* pPen)
{
	ASSERT(m_hDC != NULL); 
	return CPen(::SelectObject(m_hDC, pPen->GetSafeHandle()));
}
inline CBrush CDC::SelectObject(CBrush* pBrush)
{
	ASSERT(m_hDC != NULL); 
	return CBrush(::SelectObject(m_hDC, pBrush->GetSafeHandle()));
}
inline CFont CDC::SelectObject(CFont* pFont)
{
	ASSERT(m_hDC != NULL); 
	return CFont(::SelectObject(m_hDC, pFont->GetSafeHandle()));
}
inline int CDC::SelectObject(CRgn* pRgn)
{
	ASSERT(m_hDC != NULL);
	return (int)::SelectObject(m_hDC, pRgn->GetSafeHandle());
}
inline CPalette CDC::SelectPalette(CPalette* pPalette, BOOL bForceBackground)
{
	ASSERT(m_hDC != NULL);
	return CPalette(::SelectPalette(m_hDC,(HPALETTE)pPalette->GetSafeHandle(), bForceBackground));
}
inline COLORREF CDC::SetBkColor(COLORREF crColor)
{
	ASSERT(m_hDC != NULL);
	return ::SetBkColor(m_hDC, crColor);
}
inline int CDC::SetBkMode(int nBkMode)
{
	ASSERT(m_hDC != NULL);
	return ::SetBkMode(m_hDC, nBkMode);
}
inline int CDC::SetPolyFillMode(int nPolyFillMode)
{
	ASSERT(m_hDC != NULL);
	return ::SetPolyFillMode(m_hDC, nPolyFillMode);
}
inline int CDC::SetROP2(int nDrawMode)
{
	ASSERT(m_hDC != NULL);
	return ::SetROP2(m_hDC, nDrawMode);
}
inline int CDC::SetStretchBltMode(int nStretchMode)
{
	ASSERT(m_hDC != NULL);
	return ::SetStretchBltMode(m_hDC, nStretchMode);
}
inline COLORREF CDC::SetTextColor(COLORREF crColor)
{
	ASSERT(m_hDC != NULL);
	return ::SetTextColor(m_hDC, crColor);
}
inline int CDC::SetMapMode(int nMapMode)
{
	ASSERT(m_hDC != NULL);
	return ::SetMapMode(m_hDC, nMapMode);
}
inline CPoint CDC::SetViewportOrg(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	VERIFY(::SetViewportOrgEx(m_hDC, x, y, &point));
	return point;
}
inline CPoint CDC::OffsetViewportOrg(int nWidth, int nHeight)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	VERIFY(::OffsetViewportOrgEx(m_hDC, nWidth, nHeight, &point));
	return point;
}
inline CSize CDC::SetViewportExt(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CSize size;
	VERIFY(::SetViewportExtEx(m_hDC, x, y, &size));
	return size;
}
inline CSize CDC::ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom)
{
	ASSERT(m_hDC != NULL);
	CSize size;
	VERIFY(::ScaleViewportExtEx(m_hDC, xNum, xDenom, yNum, yDenom, &size));
	return size;
}
inline CPoint CDC::SetWindowOrg(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	VERIFY(::SetWindowOrgEx(m_hDC, x, y, &point));
	return point;
}
inline CPoint CDC::OffsetWindowOrg(int nWidth, int nHeight)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	VERIFY(::OffsetWindowOrgEx(m_hDC, nWidth, nHeight, &point));
	return point;
}
inline CSize CDC::SetWindowExt(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CSize size;
	VERIFY(::SetWindowExtEx(m_hDC, x, y, &size));
	return size;
}
inline CSize CDC::ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom)
{
	ASSERT(m_hDC != NULL);
	CSize size;
	VERIFY(::ScaleWindowExtEx(m_hDC, xNum, xDenom, yNum, yDenom, &size));
	return size;
}
inline int CDC::GetClipBox(LPRECT lpRect) const
{
	ASSERT(m_hDC != NULL);
	return ::GetClipBox(m_hDC, lpRect);
}
inline int CDC::SelectClipRgn(CRgn* pRgn)
{
	ASSERT(m_hDC != NULL);
	return ::SelectClipRgn(m_hDC, (HRGN)pRgn->GetSafeHandle());
}
inline int CDC::ExcludeClipRect(int x1, int y1, int x2, int y2)
{
	ASSERT(m_hDC != NULL);
	return ::ExcludeClipRect(m_hDC, x1, y1, x2, y2);
}
inline int CDC::ExcludeClipRect(LPCRECT lpRect)
{
	ASSERT(m_hDC != NULL);
	return ::ExcludeClipRect(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}
inline int CDC::IntersectClipRect(int x1, int y1, int x2, int y2)
{
	ASSERT(m_hDC != NULL);
	return ::IntersectClipRect(m_hDC, x1, y1, x2, y2);
}
inline int CDC::IntersectClipRect(LPCRECT lpRect)
{
	ASSERT(m_hDC != NULL);
	return ::IntersectClipRect(m_hDC, lpRect->left, lpRect->top,
			lpRect->right, lpRect->bottom);
}
inline int CDC::OffsetClipRgn(int x, int y)
{
	ASSERT(m_hDC != NULL);
	return ::OffsetClipRgn(m_hDC, x, y);
}
inline int CDC::OffsetClipRgn(SIZE size)
{
	ASSERT(m_hDC != NULL);
	return ::OffsetClipRgn(m_hDC, size.cx, size.cy);
}
inline CPoint CDC::MoveTo(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	VERIFY(::MoveToEx(m_hDC, x, y, &point));
	return point;
}
inline BOOL CDC::LineTo(int x, int y)
{
	ASSERT(m_hDC != NULL);
	return ::LineTo(m_hDC, x, y);
}

inline UINT CDC::SetTextAlign(UINT nFlags)
{
	ASSERT(m_hDC != NULL);
	return ::SetTextAlign(m_hDC, nFlags);
}
inline BOOL CDC::SetTextJustification(int nBreakExtra, int nBreakCount)
{
	ASSERT(m_hDC != NULL);
	return ::SetTextJustification(m_hDC, nBreakExtra, nBreakCount);
}
inline int CDC::SetTextCharacterExtra(int nCharExtra)
{
	ASSERT(m_hDC != NULL);
	return ::SetTextCharacterExtra(m_hDC, nCharExtra);
}
inline DWORD CDC::SetMapperFlags(DWORD dwFlag)
{
	ASSERT(m_hDC != NULL);
	return ::SetMapperFlags(m_hDC, dwFlag);
}
// this should probably not be inline
inline int CDC::StartDoc(LPCTSTR lpszDocName)
{
	DOCINFO di;
	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = lpszDocName;
	return StartDoc(&di);
}

// Window Class Members
inline BOOL ModifyStyle(HWND hWnd, int nStyleOffset, DWORD dwRemove, DWORD dwAdd)
	{ ASSERT(hWnd != NULL); DWORD dwStyle = ::GetWindowLong(hWnd, nStyleOffset);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd; if (dwStyle == dwNewStyle) return FALSE;
	::SetWindowLong(hWnd, nStyleOffset, dwNewStyle); return TRUE;}
inline CWnd::operator HWND() const
	{ return this == NULL ? NULL : _hwnd; }
inline BOOL CWnd::operator==(const CWnd& wnd) const
	{ return ((HWND) wnd) == _hwnd; }
inline BOOL CWnd::operator!=(const CWnd& wnd) const
	{ return ((HWND) wnd) != _hwnd; }
inline HWND CWnd::GetSafeHwnd() const
	{ return this == NULL ? NULL : _hwnd; }
inline DWORD CWnd::GetStyle() const
	{ ASSERT(::IsWindow(_hwnd)); return (DWORD)GetWindowLong(_hwnd, GWL_STYLE); }
inline DWORD CWnd::GetExStyle() const
	{ ASSERT(::IsWindow(_hwnd)); return (DWORD)GetWindowLong(_hwnd, GWL_EXSTYLE); }
inline BOOL CWnd::ModifyStyle(DWORD dwRemove, DWORD dwAdd)
	{ ASSERT(::IsWindow(_hwnd)); return ::ModifyStyle(_hwnd, GWL_STYLE, dwRemove, dwAdd); }
inline BOOL CWnd::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd)
	{ ASSERT(::IsWindow(_hwnd)); return ::ModifyStyle(_hwnd, GWL_EXSTYLE, dwRemove, dwAdd); }
inline LRESULT CWnd::SendMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{ ASSERT(::IsWindow(_hwnd)); return ::SendMessage(_hwnd, message, wParam, lParam); }
inline BOOL CWnd::PostMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{ ASSERT(::IsWindow(_hwnd)); return ::PostMessage(_hwnd, message, wParam, lParam); }
inline int CWnd::GetWindowTextLength() const
	{ ASSERT(::IsWindow(_hwnd)); return ::GetWindowTextLength(_hwnd); }
inline void CWnd::SetFont(CFont* pFont, BOOL bRedraw)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), bRedraw); }
inline CFont CWnd::GetFont() const
	{ ASSERT(::IsWindow(_hwnd)); return CFont(
		(HFONT)::SendMessage(_hwnd, WM_GETFONT, 0, 0)); }
//inline CMenu CWnd::GetMenu() const
//	{ ASSERT(::IsWindow(_hwnd)); return CMenu(::GetMenu(_hwnd)); }
//inline BOOL CWnd::SetMenu(CMenu* pMenu)
//	{ ASSERT(::IsWindow(_hwnd)); return ::SetMenu(_hwnd, pMenu->GetSafeHmenu()); }
//inline void CWnd::DrawMenuBar()
//	{ ASSERT(::IsWindow(_hwnd)); ::DrawMenuBar(_hwnd); }
//inline CMenu CWnd::GetSystemMenu(BOOL bRevert) const
//	{ ASSERT(::IsWindow(_hwnd)); return CMenu(::GetSystemMenu(_hwnd, bRevert)); }
//inline BOOL CWnd::HiliteMenuItem(CMenu* pMenu, UINT nIDHiliteItem, UINT nHilite)
//	{ ASSERT(::IsWindow(_hwnd)); return ::HiliteMenuItem(_hwnd, pMenu->m_hMenu, nIDHiliteItem, nHilite); }
inline int CWnd::GetDlgCtrlID() const
	{ ASSERT(::IsWindow(_hwnd)); return ::GetDlgCtrlID(_hwnd); }
inline int CWnd::SetDlgCtrlID(int nID)
	{ ASSERT(::IsWindow(_hwnd)); return (int)::SetWindowLong(_hwnd, GWL_ID, nID); }
inline BOOL CWnd::IsIconic() const
	{ ASSERT(::IsWindow(_hwnd)); return ::IsIconic(_hwnd); }
inline BOOL CWnd::IsZoomed() const
	{ ASSERT(::IsWindow(_hwnd)); return ::IsZoomed(_hwnd); }
inline void CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
	{ ASSERT(::IsWindow(_hwnd)); ::MoveWindow(_hwnd, x, y, nWidth, nHeight, bRepaint); }
inline void CWnd::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
	{ MoveWindow(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
		lpRect->bottom - lpRect->top, bRepaint); }
inline BOOL CWnd::SetWindowPos(const CWnd pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
	{ ASSERT(::IsWindow(_hwnd)); return ::SetWindowPos(_hwnd, pWndInsertAfter.GetSafeHwnd(),
		x, y, cx, cy, nFlags); }
inline UINT CWnd::ArrangeIconicWindows()
	{ ASSERT(::IsWindow(_hwnd)); return ::ArrangeIconicWindows(_hwnd); }
inline int CWnd::SetWindowRgn(HRGN hRgn, BOOL bRedraw)
	{ ASSERT(::IsWindow(_hwnd)); return ::SetWindowRgn(_hwnd, hRgn, bRedraw); }
inline int CWnd::GetWindowRgn(HRGN hRgn) const
	{ ASSERT(::IsWindow(_hwnd) && hRgn != NULL); return ::GetWindowRgn(_hwnd, hRgn); }
inline void CWnd::BringWindowToTop()
	{ ASSERT(::IsWindow(_hwnd)); ::BringWindowToTop(_hwnd); }
inline void CWnd::GetWindowRect(LPRECT lpRect) const
	{ ASSERT(::IsWindow(_hwnd)); ::GetWindowRect(_hwnd, lpRect); }
inline void CWnd::GetClientRect(LPRECT lpRect) const
	{ ASSERT(::IsWindow(_hwnd)); ::GetClientRect(_hwnd, lpRect); }
inline void CWnd::MapWindowPoints(CWnd pwndTo, LPPOINT lpPoint, UINT nCount) const
	{ ASSERT(::IsWindow(_hwnd)); ::MapWindowPoints(_hwnd, pwndTo.GetSafeHwnd(), lpPoint, nCount); }
inline void CWnd::MapWindowPoints(CWnd pwndTo, LPRECT lpRect) const
	{ ASSERT(::IsWindow(_hwnd)); ::MapWindowPoints(_hwnd, pwndTo.GetSafeHwnd(), (LPPOINT)lpRect, 2); }
inline void CWnd::ClientToScreen(LPPOINT lpPoint) const
	{ ASSERT(::IsWindow(_hwnd)); ::ClientToScreen(_hwnd, lpPoint); }
inline void CWnd::ScreenToClient(LPPOINT lpPoint) const
	{ ASSERT(::IsWindow(_hwnd)); ::ScreenToClient(_hwnd, lpPoint); }
inline CDC CWnd::BeginPaint(LPPAINTSTRUCT lpPaint)
	{ ASSERT(::IsWindow(_hwnd)); return CDC(::BeginPaint(_hwnd, lpPaint)); }
inline void CWnd::EndPaint(LPPAINTSTRUCT lpPaint)
	{ ASSERT(::IsWindow(_hwnd)); ::EndPaint(_hwnd, lpPaint); }
inline CDC CWnd::GetDC()
	{ ASSERT(::IsWindow(_hwnd)); return CDC(::GetDC(_hwnd)); }
inline CDC CWnd::GetWindowDC()
	{ ASSERT(::IsWindow(_hwnd)); return CDC(::GetWindowDC(_hwnd)); }
inline int CWnd::ReleaseDC(CDC* pDC)
	{ ASSERT(::IsWindow(_hwnd)); return ::ReleaseDC(_hwnd, pDC->m_hDC); }
inline void CWnd::UpdateWindow()
	{ ASSERT(::IsWindow(_hwnd)); ::UpdateWindow(_hwnd); }
inline void CWnd::SetRedraw(BOOL bRedraw)
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_SETREDRAW, bRedraw, 0); }
inline BOOL CWnd::GetUpdateRect(LPRECT lpRect, BOOL bErase)
	{ ASSERT(::IsWindow(_hwnd)); return ::GetUpdateRect(_hwnd, lpRect, bErase); }
inline int CWnd::GetUpdateRgn(CRgn* pRgn, BOOL bErase)
	{ ASSERT(::IsWindow(_hwnd)); return ::GetUpdateRgn(_hwnd, (HRGN)pRgn->GetSafeHandle(), bErase); }
inline void CWnd::Invalidate(BOOL bErase)
	{ ASSERT(::IsWindow(_hwnd)); ::InvalidateRect(_hwnd, NULL, bErase); }
inline void CWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase)
	{ ASSERT(::IsWindow(_hwnd)); ::InvalidateRect(_hwnd, lpRect, bErase); }
inline void CWnd::InvalidateRgn(CRgn* pRgn, BOOL bErase)
	{ ASSERT(::IsWindow(_hwnd)); ::InvalidateRgn(_hwnd, (HRGN)pRgn->GetSafeHandle(), bErase); }
inline void CWnd::ValidateRect(LPCRECT lpRect)
	{ ASSERT(::IsWindow(_hwnd)); ::ValidateRect(_hwnd, lpRect); }
inline void CWnd::ValidateRgn(CRgn* pRgn)
	{ ASSERT(::IsWindow(_hwnd)); ::ValidateRgn(_hwnd, (HRGN)pRgn->GetSafeHandle()); }
inline BOOL CWnd::ShowWindow(int nCmdShow)
	{ ASSERT(::IsWindow(_hwnd)); return ::ShowWindow(_hwnd, nCmdShow); }
inline BOOL CWnd::IsWindowVisible() const
	{ ASSERT(::IsWindow(_hwnd)); return ::IsWindowVisible(_hwnd); }
inline void CWnd::ShowOwnedPopups(BOOL bShow)
	{ ASSERT(::IsWindow(_hwnd)); ::ShowOwnedPopups(_hwnd, bShow); }
inline BOOL CWnd::IsDialogMessage(LPMSG lpMsg)
	{ ASSERT(::IsWindow(_hwnd)); return ::IsDialogMessage(_hwnd, lpMsg); }

inline CDC CWnd::GetDCEx(CRgn* prgnClip, DWORD flags)
	{ ASSERT(::IsWindow(_hwnd)); return CDC(::GetDCEx(_hwnd, (HRGN)prgnClip->GetSafeHandle(), flags)); }
inline BOOL CWnd::LockWindowUpdate()
	{ ASSERT(::IsWindow(_hwnd)); return ::LockWindowUpdate(_hwnd); }
inline void CWnd::UnlockWindowUpdate()
	{ ASSERT(::IsWindow(_hwnd)); ::LockWindowUpdate(NULL); }
inline BOOL CWnd::RedrawWindow(LPCRECT lpRectUpdate, CRgn* prgnUpdate,
	UINT flags)
	{ ASSERT(::IsWindow(_hwnd)); return ::RedrawWindow(_hwnd, lpRectUpdate, (HRGN)prgnUpdate->GetSafeHandle(), flags); }
inline BOOL CWnd::EnableScrollBar(int nSBFlags, UINT nArrowFlags)
	{ ASSERT(::IsWindow(_hwnd)); return (BOOL)::EnableScrollBar(_hwnd, nSBFlags, nArrowFlags); }

inline UINT CWnd::SetTimer(UINT nIDEvent, UINT nElapse,
		void (CALLBACK* lpfnTimer)(HWND, UINT, UINT, DWORD))
	{ ASSERT(::IsWindow(_hwnd)); return ::SetTimer(_hwnd, nIDEvent, nElapse,
		(TIMERPROC)lpfnTimer); }
inline BOOL CWnd::KillTimer(int nIDEvent)
	{ ASSERT(::IsWindow(_hwnd)); return ::KillTimer(_hwnd, nIDEvent); }
inline BOOL CWnd::IsWindowEnabled() const
	{ ASSERT(::IsWindow(_hwnd)); return ::IsWindowEnabled(_hwnd); }
inline BOOL CWnd::EnableWindow(BOOL bEnable)
	{ ASSERT(::IsWindow(_hwnd)); return ::EnableWindow(_hwnd, bEnable); }
inline CWnd PASCAL CWnd::GetActiveWindow()
	{ return CWnd(::GetActiveWindow()); }
inline CWnd CWnd::SetActiveWindow()
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::SetActiveWindow(_hwnd)); }
inline CWnd PASCAL CWnd::GetCapture()
	{ return CWnd(::GetCapture()); }
inline CWnd CWnd::SetCapture()
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::SetCapture(_hwnd)); }
inline CWnd PASCAL CWnd::GetFocus()
	{ return CWnd(::GetFocus()); }
inline CWnd CWnd::SetFocus()
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::SetFocus(_hwnd)); }
inline CWnd PASCAL CWnd::GetDesktopWindow()
	{ return CWnd(::GetDesktopWindow()); }
inline void CWnd::CheckDlgButton(int nIDButton, UINT nCheck)
	{ ASSERT(::IsWindow(_hwnd)); ::CheckDlgButton(_hwnd, nIDButton, nCheck); }
inline void CWnd::CheckRadioButton(int nIDFirstButton, int nIDLastButton,
		int nIDCheckButton)
	{ ASSERT(::IsWindow(_hwnd)); ::CheckRadioButton(_hwnd, nIDFirstButton, nIDLastButton, nIDCheckButton); }
inline int CWnd::DlgDirList(LPTSTR lpPathSpec, int nIDListBox,
		int nIDStaticPath, UINT nFileType)
	{ ASSERT(::IsWindow(_hwnd)); return ::DlgDirList(_hwnd, lpPathSpec, nIDListBox,
			nIDStaticPath, nFileType); }
inline int CWnd::DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox,
		int nIDStaticPath, UINT nFileType)
	{ ASSERT(::IsWindow(_hwnd)); return ::DlgDirListComboBox(_hwnd, lpPathSpec,
			nIDComboBox, nIDStaticPath, nFileType); }
inline BOOL CWnd::DlgDirSelect(LPTSTR lpString, int nIDListBox)
	{ ASSERT(::IsWindow(_hwnd)); return ::DlgDirSelectEx(_hwnd, lpString, _MAX_PATH, nIDListBox); }
inline BOOL CWnd::DlgDirSelectComboBox(LPTSTR lpString, int nIDComboBox)
	{ ASSERT(::IsWindow(_hwnd)); return ::DlgDirSelectComboBoxEx(_hwnd, lpString, _MAX_PATH, nIDComboBox);}

inline CWnd CWnd::GetDlgItem(int nID) const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::GetDlgItem(_hwnd, nID)); }
inline void CWnd::GetDlgItem(int nID, HWND* phWnd) const
	{ ASSERT(::IsWindow(_hwnd)); ASSERT(phWnd != NULL); *phWnd = ::GetDlgItem(_hwnd, nID); }
inline UINT CWnd::GetDlgItemInt(int nID, BOOL* lpTrans,
		BOOL bSigned) const
	{ ASSERT(::IsWindow(_hwnd)); return ::GetDlgItemInt(_hwnd, nID, lpTrans, bSigned);}
inline int CWnd::GetDlgItemText(int nID, LPTSTR lpStr, int nMaxCount) const
	{ ASSERT(::IsWindow(_hwnd)); return ::GetDlgItemText(_hwnd, nID, lpStr, nMaxCount);}
inline CWnd CWnd::GetNextDlgGroupItem(CWnd pWndCtl, BOOL bPrevious) const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::GetNextDlgGroupItem(_hwnd,
			pWndCtl.GetSafeHwnd(), bPrevious)); }
inline CWnd CWnd::GetNextDlgTabItem(CWnd pWndCtl, BOOL bPrevious) const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::GetNextDlgTabItem(_hwnd,
			pWndCtl.GetSafeHwnd(), bPrevious)); }

inline UINT CWnd::IsDlgButtonChecked(int nIDButton) const
	{ ASSERT(::IsWindow(_hwnd)); return ::IsDlgButtonChecked(_hwnd, nIDButton); }
inline LPARAM CWnd::SendDlgItemMessage(int nID, UINT message, WPARAM wParam, LPARAM lParam)
	{ ASSERT(::IsWindow(_hwnd)); return ::SendDlgItemMessage(_hwnd, nID, message, wParam, lParam); }
inline void CWnd::SetDlgItemInt(int nID, UINT nValue, BOOL bSigned)
	{ ASSERT(::IsWindow(_hwnd)); ::SetDlgItemInt(_hwnd, nID, nValue, bSigned); }
/*inline int CWnd::ScrollWindowEx(int dx, int dy,
	LPCRECT lpRectScroll, LPCRECT lpRectClip,
	CRgn* prgnUpdate, LPRECT lpRectUpdate, UINT flags)
	{ ASSERT(::IsWindow(_hwnd)); return ::ScrollWindowEx(_hwnd, dx, dy, lpRectScroll, lpRectClip,
			(HRGN)prgnUpdate->GetSafeHandle(), lpRectUpdate, flags); }
inline void CWnd::ShowScrollBar(UINT nBar, BOOL bShow)
	{ ASSERT(::IsWindow(_hwnd)); ::ShowScrollBar(_hwnd, nBar, bShow); }*/
inline CWnd CWnd::ChildWindowFromPoint(POINT point) const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::ChildWindowFromPoint(_hwnd, point)); }
inline CWnd CWnd::ChildWindowFromPoint(POINT point, UINT nFlags) const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::ChildWindowFromPointEx(_hwnd, point, nFlags)); }
inline CWnd PASCAL CWnd::FindWindowA(LPCSTR lpszClassName, LPCSTR lpszWindowName)
	{ return CWnd(::FindWindowA(lpszClassName, lpszWindowName)); }
inline CWnd PASCAL CWnd::FindWindowW(LPCWSTR lpszClassName, LPCWSTR lpszWindowName)
	{ return CWnd(::FindWindowW(lpszClassName, lpszWindowName)); }
inline CWnd PASCAL CWnd::FindWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName)
	{ return CWnd(::FindWindow(lpszClassName, lpszWindowName)); }
inline CWnd CWnd::GetNextWindow(UINT nFlag) const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::GetNextWindow(_hwnd, nFlag)); }
inline CWnd CWnd::GetTopWindow() const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::GetTopWindow(_hwnd)); }
inline CWnd CWnd::GetWindow(UINT nCmd) const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::GetWindow(_hwnd, nCmd)); }
inline CWnd CWnd::GetLastActivePopup() const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::GetLastActivePopup(_hwnd)); }
inline BOOL CWnd::IsChild(const CWnd pWnd) const
	{ ASSERT(::IsWindow(_hwnd)); return ::IsChild(_hwnd, pWnd.GetSafeHwnd()); }
inline CWnd CWnd::GetParent() const
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::GetParent(_hwnd)); }
inline CWnd CWnd::SetParent(CWnd pWndNewParent)
	{ ASSERT(::IsWindow(_hwnd)); return CWnd(::SetParent(_hwnd,
			pWndNewParent.GetSafeHwnd())); }
inline CWnd PASCAL CWnd::WindowFromPoint(POINT point)
	{ return CWnd(::WindowFromPoint(point)); }
inline BOOL CWnd::FlashWindow(BOOL bInvert)
	{ ASSERT(::IsWindow(_hwnd)); return ::FlashWindow(_hwnd, bInvert); }
inline BOOL CWnd::ChangeClipboardChain(HWND hWndNext)
	{ ASSERT(::IsWindow(_hwnd)); return ::ChangeClipboardChain(_hwnd, hWndNext); }
inline HWND CWnd::SetClipboardViewer()
	{ ASSERT(::IsWindow(_hwnd)); return ::SetClipboardViewer(_hwnd); }
inline BOOL CWnd::OpenClipboard()
	{ ASSERT(::IsWindow(_hwnd)); return ::OpenClipboard(_hwnd); }
inline CWnd PASCAL CWnd::GetOpenClipboardWindow()
	{ return CWnd(::GetOpenClipboardWindow()); }
inline CWnd PASCAL CWnd::GetClipboardOwner()
	{ return CWnd(::GetClipboardOwner()); }
inline CWnd PASCAL CWnd::GetClipboardViewer()
	{ return CWnd(::GetClipboardViewer()); }
inline void CWnd::CreateCaret(CBitmap* pBitmap)
	{ ASSERT(::IsWindow(_hwnd)); ::CreateCaret(_hwnd, (HBITMAP)pBitmap->GetSafeHandle(), 0, 0); }
inline void CWnd::CreateSolidCaret(int nWidth, int nHeight)
	{ ASSERT(::IsWindow(_hwnd)); ::CreateCaret(_hwnd, (HBITMAP)0, nWidth, nHeight); }
inline void CWnd::CreateGrayCaret(int nWidth, int nHeight)
	{ ASSERT(::IsWindow(_hwnd)); ::CreateCaret(_hwnd, (HBITMAP)1, nWidth, nHeight); }
inline CPoint PASCAL CWnd::GetCaretPos()
	{ CPoint point; ::GetCaretPos((LPPOINT)&point); return point; }
inline void PASCAL CWnd::SetCaretPos(POINT point)
	{ ::SetCaretPos(point.x, point.y); }
inline void CWnd::HideCaret()
	{ ASSERT(::IsWindow(_hwnd)); ::HideCaret(_hwnd); }
inline void CWnd::ShowCaret()
	{ ASSERT(::IsWindow(_hwnd)); ::ShowCaret(_hwnd); }
inline BOOL CWnd::SetForegroundWindow()
	{ ASSERT(::IsWindow(_hwnd)); return ::SetForegroundWindow(_hwnd); }
inline CWnd PASCAL CWnd::GetForegroundWindow()
	{ return CWnd(::GetForegroundWindow()); }

inline BOOL CWnd::SendNotifyMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{ ASSERT(::IsWindow(_hwnd)); return ::SendNotifyMessage(_hwnd, message, wParam, lParam); }

// Win4
inline HICON CWnd::SetIcon(HICON hIcon, BOOL bBigIcon)
	{ ASSERT(::IsWindow(_hwnd)); return (HICON)::SendMessage(_hwnd, WM_SETICON, bBigIcon, (LPARAM)hIcon); }
inline HICON CWnd::GetIcon(BOOL bBigIcon) const
	{ ASSERT(::IsWindow(_hwnd)); return (HICON)::SendMessage(_hwnd, WM_GETICON, bBigIcon, 0); }
inline void CWnd::Print(CDC* pDC, DWORD dwFlags) const
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_PRINT, (WPARAM)pDC->GetSafeHdc(), dwFlags); }
inline void CWnd::PrintClient(CDC* pDC, DWORD dwFlags) const
	{ ASSERT(::IsWindow(_hwnd)); ::SendMessage(_hwnd, WM_PRINTCLIENT, (WPARAM)pDC->GetSafeHdc(), dwFlags); }
inline BOOL CWnd::SetWindowContextHelpId(DWORD dwContextHelpId)
	{ ASSERT(::IsWindow(_hwnd)); return ::SetWindowContextHelpId(_hwnd, dwContextHelpId); }
inline DWORD CWnd::GetWindowContextHelpId() const
	{ ASSERT(::IsWindow(_hwnd)); return ::GetWindowContextHelpId(_hwnd); }
inline void CWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
	{ 	DWORD dwExStyle = GetExStyle(); if (nAdjustType == 0) dwExStyle &= ~WS_EX_CLIENTEDGE;
	::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, dwExStyle); }
	
#endif
