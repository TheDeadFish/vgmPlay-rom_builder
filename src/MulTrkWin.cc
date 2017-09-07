#include "stdafx.h"
#include "MultiTrack.h"
#include "FileList.h"
#include "WinOO\Extra\ListBox_LineNum.h"
#include "FirstGui.h"
#include "resource.h"

class _RomSizePrn
{
public:
	CStatic romSize;
	int used;
	void Init(HWND hwnd){
		romSize = Static(STC_ROMSIZE);
		used = 0;
	}
	void Set(int _used){
		used = _used;
		char usedtemp[64];
		sprintf(usedtemp, "RomUsed: %dKB", used / 1024);
		setWindowText(romSize, usedtemp);
	}
	INT_PTR ColorStatic( WPARAM wParam, LPARAM lParam ){
		if((used > 1024*4096)&&(lParam == (LPARAM)romSize._hwnd)){
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
	CStatic static1;
	CStatic static2;
	bool col1; bool line;
	void Init(HWND hwnd){
		static1 = Static(STC_RBLINE1);
		static2 = Static(STC_RBLINE2);
		col1 = 0; line = 0;}
	void Clear(bool mode){
		bool _line = line & mode;
		if(_line == 0){
			col1 = 0;
			static1.SetWindowTextA("");
		}else
			static2.SetWindowTextA("");
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
		if((col1)&&(lParam == (LPARAM)static1._hwnd)){
			HDC hDC = (HDC)wParam;
			SetTextColor(hDC, RGB(255, 0, 0));
			SetBkMode(hDC, TRANSPARENT);
			return (INT_PTR)GetSysColorBrush(COLOR_3DFACE);
		}
		return 0;
	}
};

#define MSGIF_CTLCOLORSTATIC( func )  		\
	if( message == WM_CTLCOLORSTATIC){ 			\
		INT_PTR tmp = func(wParam, lParam);		\
		if( tmp != 0 ){							\
			lresult = tmp;						\
			MSGTRUE();}}

CDialog(MultiTrackWinGui)

	MultiTrack mt;
	RECT& rc;
	MultiTrackWinGui(RECT& rcIn) : rc(rcIn){}
	
	// GUI variables
	_RomSizePrn RomSizePrn;
	_StaticPrn StaticPrn;
	ListBox_LineNum ListBox;

	// message handlers
	MsgReflecta();
	Resize_Macro(1, true);
	CInitDialogHandlr();
	CMainWndProcHandlr();
	void AddFile(FileNameList& fnl);

_CDialog(MultiTrackWinGui)

FInitDialogHandlr(MultiTrackWinGui)
{
	RomSizePrn.Init(hwnd);
	StaticPrn.Init(hwnd);
	ListBox.SubClass(Listbox(LST_RBLIST));
	SetMinSize(-1, -2);
	ResizeAdd(LST_RBLIST, HOR_BOTH, VER_BOTH);
	MtiRectCalc(hwnd, rc);
}

FMainWndProcHandlrM(MultiTrackWinGui)
{
	MSGIF_CTLCOLORSTATIC( RomSizePrn.ColorStatic );
	MSGIF_CTLCOLORSTATIC( StaticPrn.ColorStatic );

	MSGIF( SINGLE_MSG_TST( WM_DROPFILES ) )
	{
		StaticPrn.Clear(true);
		FileNameList fnl;
		fnl.FropHdrop((HDROP)wParam);
		this->AddFile(fnl);
	}

	MSGIF( WM_COMMAND_TST1( BTN_ADDTRK ) )
	{
		StaticPrn.Clear(true);
		FileNameList fnl;
		fnl.FromOfn(hwnd);
		this->AddFile(fnl);
	}
	
	MSGIF( WM_COMMAND_TST1( BTN_REMTRK ) )
	{
		int selection;
		while(ListBox.GetSelItems(1, &selection) > 0)
		{
			ListBox.DeleteString(selection);
			mt.remove(selection);
		}
		RomSizePrn.Set(mt.totalSize);
	}
	
	MSGIF(  WM_COMMAND_TST1( BTN_BUILDROM ) )
	{
		if(mt.nTracks != 0)
			EndDialog( mt.build() + 1);
		else
			StaticPrn.Write(0, 0, 0, "No Tracks Selected!");
	}
	
	MSGIF(  WM_COMMAND_TST1( BTN_RBOPTIONS ) )
	{
		showOptions(hwnd);
	}
	
	MSGIF(  WM_COMMAND_TST1( IDCANCEL ) )
	{
		EndDialog(1);
	}
			
	MSGIF(  WM_COMMAND_TST2( LST_RBLIST, LBN_SELCHANGE ) )
	{
		for(int i = 0; i < mt.nTracks; i++)
			mt.select(i, ListBox.GetSel(i));
		RomSizePrn.Set(mt.totalSize);
	}

}_FMainWndProcHandlrM()

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
			sendMessageU(ListBox, LB_ADDSTRING, filename);
			RomSizePrn.Set(mt.totalSize);
			StaticPrn.Clear(true);
		}
	}
}

bool MultiTrackWin(RECT& rc)
{
	MultiTrackWinGui mtw(rc);
	int result = (int)mtw.Create(NULL, _T("ROMBUILD_DIALOG"), true);
	if(result <= 0)
		fatalError("Cannot create GUI");
	return result-1;
}
