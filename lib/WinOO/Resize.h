#ifndef Resize_H
#define Resize_H

#define HORSIZ(n,d) (((n<<4)|(d&15))<<0)
#define HORPOS(n,d) (((n<<4)|(d&15))<<8)
#define VERSIZ(n,d) (((n<<4)|(d&15))<<16)
#define VERPOS(n,d) (((n<<4)|(d&15))<<24)
#define HOR_LEFT HORSIZ(0,0) | HORPOS(0,0)
#define HOR_RIGH HORSIZ(0,0) | HORPOS(1,1)
#define HOR_BOTH HORSIZ(1,1) | HORPOS(0,0)
#define HOR(d, p) HORSIZ(1,d) | HORPOS((p-1),d)
#define VER_TOPP VERSIZ(0,0) | VERPOS(0,0)
#define VER_BOTT VERSIZ(0,0) | VERPOS(1,1)
#define VER_BOTH VERSIZ(1,1) | VERPOS(0,0)
#define VER(d, p) VERSIZ(1,d) | VERPOS((p-1),d)

typedef struct
{
	HWND hwnd;
	struct{
		short siz_parnt;
		short siz_child;
		short pos_child;
		char siz_scale;
		char pos_scale;
	}xy[2];
}Resize_Table;

typedef struct
{
	POINT minSize;
	int count;
	Resize_Table a[];
}Resize_Table2;

void Resize_Add(Resize_Table2 &d, HWND _hwnd, DWORD dlgItem, DWORD flags);
void Resize_WmSize(Resize_Table2 &d, HWND _hwnd);
void Resize_WmErase(Resize_Table2 &d, HWND _hwnd, HDC hDC);

template <int len>
class Resize
{
public:
	struct{
		POINT minSize;
		int count;
		Resize_Table a[len];
	}data;
	Resize(){ data.count = 0; }

	void Add(HWND _hwnd, DWORD dlgItem, DWORD flags)
	{
		Resize_Table2* tmp = (Resize_Table2*)&data;
		Resize_Add(*tmp, _hwnd, dlgItem, flags);
		//printf("%d\n", data.count);
	}
	
	void WmSize(HWND _hwnd)
	{
		Resize_Table2* tmp = (Resize_Table2*)&data;
		Resize_WmSize(*tmp, _hwnd);
	}
	
	void WmErase(HWND _hwnd, HDC hDC)
	{
		Resize_Table2* tmp = (Resize_Table2*)&data;
		Resize_WmErase(*tmp, _hwnd, hDC);
	}
};

// macros to tie in with the object based dialogs
#define Resize_Macro1() 							\
	template<class screb>							\
	bool Resize_WmSize(screb hwnd, UINT message, 	\
	WPARAM wParam,LPARAM lParam, int create){						\
		return false;}								
		
#define Resize_Macro2(var, create) \
	var |= object->Resize_WmSize(hwnd,message,wParam,lParam,create);
		
#define Resize_Macro(len, erase)					\
	Resize<len> resizeData;							\
	bool Resize_WmSize(HWND hwnd, UINT message, 	\
	WPARAM wParam, LPARAM lParam, int create){						\
		if(message == create){						\
			RECT rc;								\
			::GetWindowRect(hwnd, &rc);				\
			resizeData.data.minSize.x = rc.right-rc.left;\
			resizeData.data.minSize.y = rc.bottom-rc.top;\
			return true;}							\
		if(message == WM_GETMINMAXINFO){					\
			MINMAXINFO& mi = *(MINMAXINFO*)lParam;	\
			mi.ptMinTrackSize = resizeData.data.minSize;	\
			return true;}							\
		if(message == WM_SIZE){						\
			resizeData.WmSize(hwnd);				\
			return true;}							\
		if((erase)&&(message == WM_ERASEBKGND))		\
			resizeData.WmErase(hwnd, (HDC)wParam);	\
		return false;}

#define ResizeAdd(ctrl, hor, ver)			\
	resizeData.Add(hwnd, ctrl, hor | ver)
	
#define SetMinSize(_x, _y) \
	if( _x != -1 ) resizeData.data.minSize.x = _x;\
	if( _y != -1 ) resizeData.data.minSize.y = _y;

#endif
