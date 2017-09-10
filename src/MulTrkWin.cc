#include "stdafx.h"
#include "MultiTrack.h"
#include "FileList.h"
#include "FirstGui.h"
#include "resource.h"
#include "image.h"

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

static const WndResize::CtrlDef resizeList[] = 
{
	{IDC_LIST1, HVR_BOTH},

	// Track Info
	{GRP_TRKINFO, HOR_RIGH}, {IDC_MINUI_01, HOR_RIGH}, {IDC_MINUI_02, HOR_RIGH},
	{IDC_MINUI_04, HOR_RIGH}, {IDC_MINUI_08, HOR_RIGH}, {IDC_MINUI_10, HOR_RIGH},
	{IDC_MINUI_20, HOR_RIGH}, {IDC_MINUI_80, HOR_RIGH}, 
	// Image File
	{GRP_IMAGEFILE, HOR_RIGH}, {IDC_SELECTIMG, HOR_RIGH}, {BTN_ADDIMG, HOR_RIGH},
	{BTN_DELIMG, HOR_RIGH}, {IDC_YPOSEDT, HOR_RIGH}, {IDC_YPOS, HOR_RIGH},
	{IDC_YSIZEEDT, HOR_RIGH}, {IDC_YSIZE, HOR_RIGH}, 
	{IDC_SPIN1, HOR_RIGH}, {IDC_STATIC2, HOR_RIGH}, {IDC_STATIC3, HOR_RIGH},
};

static const WORD uiMaskList[] = {IDC_MINUI_01, IDC_MINUI_02, IDC_MINUI_04,
	IDC_MINUI_08, IDC_MINUI_10, IDC_MINUI_20, (WORD)-2, IDC_MINUI_80 };



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
	HWND hwnd, hList;
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
	void buildClick();
	MEMBER_DLGPROC(MultiTrackWinGui, 
		DlgProc, This->OnInit(hwnd))
		
		
		
	ImageInfo* selImg;
	RECT bkndRect, viewRect;
	
	
	void selTrack(void);
	
	
		
		
		
	// image loader
	void addImage(void);
	void selImage(bool);
	void onPaint(void);
	void onScroll(void);
	void onRemove(void);
	int lvUpdName(int, cch*);
	void lvUpdImage(int);
	
	void rectPaint(HDC, RECT& rc);
};

void MultiTrackWinGui::rectPaint(HDC hdc, RECT& rc)
{
	if(selImg) {
		SetDCBrushColor(hdc, selImg->img.palette[
			selImg->bkgndColor].colorRef());
		SelectObject(hdc, GetStockObject(DC_BRUSH));
	} else {  SelectObject(hdc, (HGDIOBJ)
		GetSysColorBrush(COLOR_3DFACE)); }
	Rectangle(hdc, RECT_LTRB(rc));
}

void MultiTrackWinGui::onPaint(void)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	rectPaint(hdc, bkndRect);
	rectPaint(hdc, viewRect);
	
	if(selImg) {
		IntersectClipRect(hdc, 
			viewRect.left+1, viewRect.top+1, 
			viewRect.right-1, viewRect.bottom-1);
		RECT rc = selImg->calcRect();
		OffsetRect(&rc, viewRect.left+1, viewRect.top+1);
		selImg->img.strBlt(hdc, rc);		
	}
	
	EndPaint(hwnd, &ps);
}

INT_PTR MultiTrackWinGui::DlgProc(HWND hwnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DLGMSG_SWITCH( 
	 REFLECT_DRAWITEM
	 ON_MESSAGE(WM_HSCROLL, onScroll());
	 ON_MESSAGE(WM_VSCROLL, onScroll());
	 ON_MESSAGE(WM_PAINT, onPaint())
	 ON_MESSAGE(WM_SIZE, wndResize.	resize(hwnd, wParam, lParam))
	 ON_MINMAXINFO(wndSetMin(hwnd, mi, wndResize.orgSize.X, 0))
	 ON_CTLCOLORSTATIC(
	   IFRET(RomSizePrn.ColorStatic(wParam, lParam));
	   IFRET(StaticPrn.ColorStatic(wParam, lParam)))
	 ON_MESSAGE(WM_DROPFILES, 
		StaticPrn.Clear(true); FileNameList fnl;
		fnl.FropHdrop((HDROP)wParam); this->AddFile(fnl))
	 CASE_COMMAND(
	  ON_COMMAND(IDCANCEL, EndDialog(hwnd, 1))
	  ON_COMMAND(BTN_ADDTRK,
		StaticPrn.Clear(true); FileNameList fnl;
		fnl.FromOfn(hwnd); this->AddFile(fnl);)
		
	  ON_COMMAND(BTN_REMTRK, onRemove())
	  ON_COMMAND(BTN_BUILDROM, buildClick())
	  ON_COMMAND(BTN_RBOPTIONS, showOptions(hwnd))
	  ON_COMMAND(BTN_ADDIMG, addImage());
	  
	  ON_CONTROL(CBN_SELCHANGE, IDC_SELECTIMG, selImage(true));
		



	 ,)
	 CASE_NOTIFY(
	  ON_LVN_STATECHG(IDC_LIST1, selTrack())
	 ,)
	,)




}

void MultiTrackWinGui::buildClick()
{
	BYTE uiFlags = 0;
	for(int i : Range(0, 8)) uiFlags |= 
		::IsDlgButtonChecked(hwnd, uiMaskList[i]) << i;
	mt.uiFlags = uiFlags;

	if(mt.nTracks != 0)
		EndDialog(hwnd, mt.build() + 1); else StaticPrn
		.Write(0, 0, 0, "No Tracks Selected!");
}

int MultiTrackWinGui::lvUpdName(int i, cch* name)
{
	return lstView_iosText(hList, i, xstr(
		xstrfmt("%d: %s", i, name)));
}

void MultiTrackWinGui::lvUpdImage(int i)
{
	mt[i].img = selImg;
	lstView_iosText(hList, i, 1, 
		selImg ? selImg->name.data : "");
}

void MultiTrackWinGui::onRemove(void)
{
	// delete selected items
	uint firstDel = -1; ILREV(i, mt.nTracks,,
	if(listView_getSel(hList, i) > 0) { firstDel = i;
		ListView_DeleteItem(hList, i); mt.
		remove(i); }) RomSizePrn.Set(mt.totalSize());
		
	// renumber items
	for(;firstDel < mt.nTracks; firstDel++) {
		lvUpdName(firstDel, getName(
		mt[firstDel].filename)); }
}

void MultiTrackWinGui::addImage(void)
{
	// load the image
	OpenFileName ofn;
	ofn.lpstrFilter = "Image File\0*.png;*.gif;*.bmp\0All Files\0*.*\0\0";
	if(!ofn.doModal(hwnd)) return;
	ImageInfo* ii = new ImageInfo();
	if(ii->load(ofn.lpstrFile)) { delete ii;
		contError(hwnd, "failed to load image"); return; }
	
	// update controls
	int idx = dlgCombo_addStr(hwnd, 
		IDC_SELECTIMG, ii->name, (LPARAM)ii);
	dlgCombo_setSel(hwnd, IDC_SELECTIMG, idx);
	if(idx == 1) { for(WORD ctrlId : uiMaskList)
		if(ctrlId != IDC_MINUI_01) 
			CheckDlgButton(hwnd, ctrlId, FALSE); }
	this->selImage(true);
}

void MultiTrackWinGui::onScroll(void)
{
	if(!selImg) return;
	selImg->ypos = getDlgSpinValue(hwnd, IDC_YPOS);
	selImg->ysize = getDlgSpinValue(hwnd, IDC_YSIZE);
	selImg->bkgndColor = getDlgSpinValue(hwnd, IDC_SPIN1);
	InvalidateRect(hwnd, &bkndRect, FALSE);
	InvalidateRect(hwnd, &viewRect, FALSE);
	RomSizePrn.Set(mt.totalSize());
}


void MultiTrackWinGui::selImage(bool udtLV)
{
	// get selected image
	int idx = dlgCombo_getSel(hwnd, IDC_SELECTIMG);
	ImageInfo* ii = Void(dlgCombo_getData2(
		hwnd, IDC_SELECTIMG, idx));
	this->selImg = ii;
	InvalidateRect(hwnd, &bkndRect, FALSE);
	InvalidateRect(hwnd, &viewRect, FALSE);
		
	// update controls
	EnableDlgItems(hwnd, (const WORD[]){ IDC_YPOS, IDC_YSIZE,
		IDC_YSIZEEDT, IDC_YPOSEDT, IDC_SPIN1, BTN_DELIMG },6, !!ii);
	if(ii) setDlgSpinValue(hwnd, IDC_SPIN1, 0, 
		ii->img.palSize, ii->bkgndColor);
	setDlgSpinValue(hwnd, IDC_YPOS, -32768, 32767, ii ? ii->ypos : 0);
	setDlgSpinValue(hwnd, IDC_YSIZE, 0, 65535, ii ? ii->ysize : 0);

	// update listview
	if(udtLV == false) return;
	int curSelPos = -1; while((curSelPos =
	listView_nextSel(hList, curSelPos)) >= 0) {
		lvUpdImage(curSelPos); }
	RomSizePrn.Set(mt.totalSize());
}

void MultiTrackWinGui::selTrack(void)
{
	// find common image
	LPARAM ii = -1;
	for(int i = 0; i < mt.nTracks; i++) {
		int sel = listView_getSel(hList, i);
		if(sel > 0) { LPARAM tmp = (LPARAM)mt[i].img;
			if(ii != tmp) { ii = (ii == -1) ? tmp : -2; }}
	}
	
	// select common image
	int datum = dlgCombo_findData(hwnd, IDC_SELECTIMG, ii);
	dlgCombo_setSel(hwnd, IDC_SELECTIMG, 
		dlgCombo_findData(hwnd, IDC_SELECTIMG, ii));
	this->selImage(false);
}

void MultiTrackWinGui::OnInit(HWND hwnd)
{
	this->hwnd = hwnd;

	RomSizePrn.Init(hwnd);
	StaticPrn.Init(hwnd);	
	
	
	// 
	viewRect = MurderChild(hwnd, IDC_VIEW);
	bkndRect = MurderChild(hwnd, IDC_IMGBKG);
	
	wndResize.init(hwnd);
	wndResize.add(hwnd, resizeList, ARRAYSIZE(resizeList));
	wndResize.add(&viewRect, HOR_RIGH);
	wndResize.add(&bkndRect, HOR_RIGH);
	
	hList = GetDlgItem(hwnd, IDC_LIST1);
	lstView_insColumn(hList, 0, 200, "Track");
	lstView_insColumn(hList, 1, 0, "Image");
	ListView_SetColumnWidth(hList, 1,
		LVSCW_AUTOSIZE_USEHEADER);
		
		
	dlgCombo_addStr(hwnd, IDC_SELECTIMG, "None");
	
	for(WORD ctrlId : uiMaskList)
		::CheckDlgButton(hwnd, ctrlId, TRUE);
	
	MtiRectCalc(hwnd, rc);
	
	this->selImage(false);
}

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
			lvUpdImage(lvUpdName(mt.nTracks-1, filename));
			RomSizePrn.Set(mt.totalSize());
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
