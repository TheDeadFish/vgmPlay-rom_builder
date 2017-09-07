#include "stdafx.h"
#include "MultiTrack.h"
#include "FileList.h"
#include "FirstGui.h"
#include "resource.h"

class _RomSizePrn
{
public:
	HWND romSize;
	int used;
	void Init(HWND hwnd){
		romSize = GetDlgItem(hwnd, STC_ROMSIZE);
		used = 0;
	}
	void Set(int _used){
		used = _used;
		char usedtemp[64];
		sprintf(usedtemp, "RomUsed: %dKB", used / 1024);
		setWindowText(romSize, usedtemp);
	}
	INT_PTR ColorStatic( WPARAM wParam, LPARAM lParam ){
		if((used > 1024*4096)&&(lParam == (LPARAM)romSize)){
			HDC hDC = (HDC)wParam;
			SetTextColor(hDC, RGB(255, 0, 0));
			SetBkMode(hDC, TRANSPARENT);
			return (INT_PTR)GetSysColorBrush(COLOR_3DFACE);
		}
		return 0;
	}
};

class _StaticPrn
{
public:
	HWND static1;
	HWND static2;
	bool col1; bool line;
	void Init(HWND hwnd){
		static1 = GetDlgItem(hwnd, STC_RBLINE1);
		static2 = GetDlgItem(hwnd, STC_RBLINE2);
		col1 = 0; line = 0;}
	void Clear(bool mode){
		bool _line = line & mode;
		if(_line == 0){
			col1 = 0;
			SetWindowTextA(static1, "");
		}else
			SetWindowTextA(static2, "");
	}
	void Write(bool mode, bool col, char symb, const char* text)
	{
		bool _line = line & mode;
		col1 = col;
		char tmp0[] = "  "; tmp0[0] = symb;
		xstr tmp = xstrfmt("%s%s", tmp0, text);
		if(_line)
			setWindowText(static2, tmp);
		else
			setWindowText(static1, tmp);
	}
	void ResetLine(void){
		line = 0;
	}
	void NextLine(void){
		line = 1;
	}
	INT_PTR ColorStatic( WPARAM wParam, LPARAM lParam ){
		if((col1)&&(lParam == (LPARAM)static1)){
			HDC hDC = (HDC)wParam;
			SetTextColor(hDC, RGB(255, 0, 0));
			SetBkMode(hDC, TRANSPARENT);
			return (INT_PTR)GetSysColorBrush(COLOR_3DFACE);
		}
		return 0;
	}
};

struct MultiTrackWinGui
{
	HWND hwnd;
	MultiTrack mt;
	RECT& rc;
	MultiTrackWinGui(RECT& rcIn) : rc(rcIn){}
	
	// GUI variables
	WndResize wndResize;
	_RomSizePrn RomSizePrn;
	_StaticPrn StaticPrn;

	// message handlers
	void AddFile(FileNameList& fnl);
	void OnInit(HWND hwnd);
	MEMBER_DLGPROC(MultiTrackWinGui, 
		DlgProc, This->OnInit(hwnd))
};

/* helper functions */
void WINAPI listBox_delStr(HWND h, DWORD c, int i) {
	sendDlgMsg(h, c, LB_DELETESTRING, i); }
#define ILREV(i,n,x,f) for(int i = n; --i >= 0; x) {f;}
#define ON_MINMAXINFO(f) ON_MESSAGE(WM_GETMINMAXINFO, \
	MINMAXINFO& mi = *(MINMAXINFO*)lParam; f)
SIZE wndFrmSize(HWND hwnd) { RECT wr, cr;
	GetWindowRect(hwnd, &wr); GetClientRect(hwnd, &cr);
	return {RECT_W(wr)-cr.right, RECT_H(wr)-cr.bottom}; }
void wndSetMin(HWND hwnd, MINMAXINFO& mi, int mx, int my) {
	SIZE fsz = wndFrmSize(hwnd);
	if(mx > 0) mi.ptMinTrackSize.x = mx + fsz.cx;
	if(my > 0) mi.ptMinTrackSize.y = my + fsz.cy; }

INT_PTR CALLBACK MultiTrackWinGui::DlgProc(HWND hwnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DLGMSG_SWITCH( 
	 REFLECT_DRAWITEM
	 ON_MESSAGE(WM_SIZE, wndResize.	resize(hwnd, wParam, lParam))
	 ON_MINMAXINFO(wndSetMin(hwnd, mi, wndResize.orgSize.X, 0))
	 ON_MESSAGE(WM_CTLCOLORSTATIC,
	   RomSizePrn.ColorStatic(wParam, lParam);
	   RomSizePrn.ColorStatic(wParam, lParam))
	 ON_MESSAGE(WM_DROPFILES, 
		StaticPrn.Clear(true); FileNameList fnl;
		fnl.FropHdrop((HDROP)wParam); this->AddFile(fnl))
	 CASE_COMMAND(
	  ON_COMMAND(IDCANCEL, EndDialog(hwnd, 1))
	  ON_COMMAND(BTN_ADDTRK,
		StaticPrn.Clear(true); FileNameList fnl;
		fnl.FromOfn(hwnd); this->AddFile(fnl);)
	  ON_COMMAND(BTN_REMTRK, ILREV(i, mt.nTracks,,
		if(listBox_getSel(hwnd, LST_RBLIST, i) > 0) {
			listBox_delStr(hwnd, LST_RBLIST, i); mt.
			remove(i); }) RomSizePrn.Set(mt.totalSize))
	  ON_COMMAND(BTN_BUILDROM, if(mt.nTracks != 0)
		EndDialog(hwnd, mt.build() + 1); else StaticPrn
			.Write(0, 0, 0, "No Tracks Selected!"))
	  ON_COMMAND(BTN_RBOPTIONS, showOptions(hwnd))
	  ON_CONTROL(LBN_SELCHANGE, LST_RBLIST,
		for(int i = 0; i < mt.nTracks; i++) mt.select
			(i, listBox_getSel(hwnd, LST_RBLIST, i) > 0);
		RomSizePrn.Set(mt.totalSize))
	 ,)
	,)	




}

void MultiTrackWinGui::OnInit(HWND hwnd)
{
	this->hwnd = hwnd;

	RomSizePrn.Init(hwnd);
	StaticPrn.Init(hwnd);	
	
	wndResize.init(hwnd);
	wndResize.add(hwnd, LST_RBLIST, HVR_BOTH);
	
	LinNumListBoxInit(GetDlgItem(hwnd, LST_RBLIST));
	
	
	
	
	MtiRectCalc(hwnd, rc);
}


//FInitDialogHandlr(MultiTrackWinGui)
//{

	
	//SetMinSize(-1, -2);
	//ResizeAdd(LST_RBLIST, HOR_BOTH, VER_BOTH);
	
//}

void MultiTrackWinGui::AddFile(FileNameList& fnl)
{
	while(char* name = fnl.Get())
	{
		char* filename = getName(name);
		
		
		StaticPrn.Write(1, 0, '#', filename);
		char result = mt.add(name);
		if(result != 0)
		{
			StaticPrn.NextLine();
			StaticPrn.Write(0, 1, result, filename);	
		}
		else
		{
			listBox_addStr(hwnd, LST_RBLIST, filename);
			RomSizePrn.Set(mt.totalSize);
			StaticPrn.Clear(true);
		}
	}
}

bool MultiTrackWin(RECT& rc)
{
	MultiTrackWinGui mtw(rc);
	int result = DialogBoxParam(GetModuleHandle(0), 
		L"ROMBUILD_DIALOG", 
		NULL, MultiTrackWinGui::cDlgProc, (LPARAM)&mtw);
	if(result <= 0)
		fatalError("Cannot create GUI");
	return result-1;
}
