// undef windows macros

#ifdef GetNextWindow
#undef GetNextWindow
inline HWND WINAPI GetNextWindow(HWND a, UINT b)
	{ return GetWindow( a, b ); }
#endif

#ifdef CreateWindowEx
#undef CreateWindowEx
inline HWND WINAPI CreateWindowEx(DWORD a, LPCTSTR b, LPCTSTR c, DWORD d,
int x, int y, int w, int h, HWND e, HMENU f, HINSTANCE g, LPVOID p)
#ifdef UNICODE
	{	return CreateWindowExW(a, b, c, d, x, y, w, h, e, f, g, p); }
#else
	{	return CreateWindowExA(a, b, c, d, x, y, w, h, e, f, g, p); }
#endif
#endif

#ifdef GetWindowText
#undef GetWindowText
inline int WINAPI GetWindowText(HWND a, LPTSTR b, int c)
#ifdef UNICODE
	{	return GetWindowTextW(a, b, c); }
#else
	{	return GetWindowTextA(a, b, c); }
#endif
#endif
	
#ifdef FindWindow
#undef FindWindow
inline HWND WINAPI FindWindow(LPCTSTR a, LPCTSTR b)
#ifdef UNICODE
	{	return FindWindowW(a, b); }
#else
	{	return FindWindowA(a, b); }
#endif
#endif

#ifdef TextOut
#undef TextOut
inline BOOL TextOut(HDC a, int b, int c, LPCTSTR d, int e)
#ifdef UNICODE
	{	return TextOutW(a, b, c, d, e); }
#else	
	{	return TextOutA(a, b, c, d, e); }
#endif
#endif

#ifdef DlgDirList
#undef DlgDirList
inline int DlgDirList(HWND a, LPTSTR b, int c, int d, UINT e)
#ifdef UNICODE
	{	return DlgDirListW(a, b, c, d, e); }
#else
	{	return DlgDirListA(a, b, c, d, e); }	
#endif
#endif

#ifdef DlgDirListComboBox
#undef DlgDirListComboBox
inline int DlgDirListComboBox(HWND a, LPTSTR b ,int c, int d, UINT e)
#ifdef UNICODE
	{	return DlgDirListComboBoxW(a, b, c, d, e); }
#else	
	{	return DlgDirListComboBoxA(a, b, c, d, e); }
#endif
#endif

#ifdef GetDlgItemText
#undef GetDlgItemText
inline UINT WINAPI GetDlgItemText(HWND a, int b, LPTSTR c, int d)
#ifdef UNICODE
	{	return GetDlgItemTextW(a, b, c, d); }
#else	
	{	return GetDlgItemTextA(a, b, c, d); }
#endif
#endif

#ifdef SetWindowText
#undef SetWindowText
inline BOOL WINAPI SetWindowText(HWND a, LPCTSTR b)
#ifdef UNICODE
	{	return SetWindowTextW(a, b); }
#else	
	{	return SetWindowTextA(a, b); }
#endif
#endif

#ifdef MessageBox
#undef MessageBox
inline int WINAPI MessageBox(HWND a, LPCTSTR b, LPCTSTR c, UINT d)
#ifdef UNICODE
	{	return MessageBoxW(a, b, c, d); }
#else	
	{	return MessageBoxA(a, b, c, d); }
#endif
#endif

#ifdef SetDlgItemText
#undef SetDlgItemText
inline BOOL WINAPI SetDlgItemText(HWND a, int b, LPCTSTR c)
#ifdef UNICODE
	{	SetDlgItemTextW(a, b, c); }
#else	
	{	SetDlgItemTextA(a, b, c); }
#endif
#endif

#ifdef CreateFont
#undef CreateFont
inline HFONT CreateFont(int a, int b, int c, int d, int e, DWORD f, DWORD g,
DWORD h, DWORD i, DWORD j, DWORD k, DWORD l, DWORD m, LPCTSTR n)
#ifdef UNICODE
	{	return CreateFontW(a, b, c, d, e, f, g, h, i, j, k, l, m, n); }
#else	
	{	return CreateFontA(a, b, c, d, e, f, g, h, i, j, k, l, m, n); }
#endif
#endif

#ifdef LoadBitmap
#undef LoadBitmap
inline HBITMAP LoadBitmap(HINSTANCE a, LPCTSTR b)
#ifdef UNICODE
	{	LoadBitmapW(a, b); }
#else	
	{	LoadBitmapA(a, b); }
#endif
#endif

#ifdef CreateDC
#undef CreateDC
inline HDC CreateDC(LPCSTR a, LPCTSTR b, LPCTSTR c, const DEVMODE *d)
#ifdef UNICODE
	{	CreateDCW(a, b, c, d); }
#else	
	{	CreateDCA(a, b, c, d); }
#endif
#endif

#ifdef CreateIC
#undef CreateIC
inline HDC CreateIC(LPCSTR a, LPCTSTR b, LPCSTR c, const DEVMODEA *d)
#ifdef UNICODE
	{	return CreateICW(a, b, c, d); }
#else	
	{	return CreateICA(a, b, c, d); }
#endif
#endif

#ifdef DrawState
#undef DrawState
inline BOOL DrawState(HDC a, HBRUSH b, DRAWSTATEPROC c, LPARAM d,
WPARAM e, int x, int y, int cx, int cy, UINT f)
#ifdef UNICODE
	{	return DrawStateW(a, b, c, d, e, x, y, cx, cy, f); }
#else
	{	return DrawStateA(a, b, c, d, e, x, y, cx, cy, f); }
#endif
#endif

#ifdef ExtTextOut
#undef ExtTextOut
inline BOOL ExtTextOut(HDC a, int X, int Y, UINT b, const RECT *c, LPCTSTR d, UINT e, const INT *f)
#ifdef UNICODE
	{	return ExtTextOutW(a, X, Y, b, c, d, e, f); }
#else
	{	return ExtTextOutA(a, X, Y, b, c, d, e, f); }
#endif
#endif

#ifdef TabbedTextOut
#undef TabbedTextOut
inline LONG TabbedTextOut(HDC a, int X, int Y, LPCTSTR b, int c, int d, const LPINT e, int f)
#ifdef UNICODE
	{	return TabbedTextOutW(a, X, Y, b, c, d, e, f); }
#else
	{	return TabbedTextOutA(a, X, Y, b, c, d, e, f); }
#endif
#endif

#ifdef DrawText
#undef DrawText
inline int DrawText(HDC a, LPCTSTR b, int c, LPRECT d, UINT e)
#ifdef UNICODE
	{	return DrawTextW(a, b, c, d, e); }
#else
	{	return DrawTextA(a, b, c, d, e); }
#endif
#endif

#ifdef GetTabbedTextExtent
#undef GetTabbedTextExtent
inline DWORD GetTabbedTextExtent(HDC a, LPCTSTR b, int c, int d, const LPINT e)
#ifdef UNICODE
	{	return GetTabbedTextExtentW(a, b, c, d, e); }
#else
	{	return GetTabbedTextExtentA(a, b, c, d, e); }
#endif
#endif

#ifdef GetTextFace
#undef GetTextFace
inline int GetTextFace(HDC a, int b, LPTSTR c)
#ifdef UNICODE
	{	return GetTextFaceW(a, b, c); }
#else
	{	return GetTextFaceA(a, b, c); }
#endif
#endif
