#include "stdafx.h"
#include "VGM_PLAY.h"
#include "FileList.h"
#include "resource.h"
#include <windowsx.h>

static const LPCSTR dupRemOpts[] = {"Dissabled", 
	"Safe Mode", "Emulator Mode", "Hardware Mode"};
static const int bitCode[] = { EmuSpwn, EmuTerm, DacAvenc, ClsRombu};
static const short ctrlList[] = {CHK_EMUSPAWN,
	CHK_EMUTERM, CHK_DACAVENC, CHK_CLASSICRB};

struct OptionsDialog
{
	void OnInit(HWND hwnd);
	void OnOk(HWND hwnd);
	
	MEMBER_DLGPROC(OptionsDialog, 
		DlgProc, This->OnInit(hwnd))
};

void OptionsDialog::OnInit(HWND hwnd)
{
	// Emulator path
	setDlgItemText(hwnd, EDT_EMUPATH, config.EmuPath);
	// Play Looped Setion, pause after non-looped tracks, RomPad Multiple
	SetDlgItemInt(hwnd, EDT_LOOPEDSECT, config.RepeatLooped, 0);
	SetDlgItemInt(hwnd, EDT_TRACKPAUSE, config.MS_Pause, 0);
	SetDlgItemInt(hwnd, EDT_ROMPAD, config.RomPad, 0);
	// Emualtor spawning, Emulator termination, Dac Average Encoding
	for(int i = 0; i < 4; i++) { CheckDlgButton(hwnd, 
		ctrlList[i], !!(config.bitOpts & bitCode[i])); }
	// Duplicate Write Removal
	dlgCombo_addStrs(hwnd, IDC_COMBO1, dupRemOpts, 4);
	dlgCombo_setSel(hwnd, IDC_COMBO1, config.dupRemove);
	char buff[64]; config.getScale(buff);
	setDlgItemText(hwnd, EDT_SCALE, buff);	
}

void OptionsDialog::OnOk(HWND hwnd)
{
	free_repl(config.EmuPath, getDlgItemText(hwnd, EDT_EMUPATH));
	config.RepeatLooped = GetDlgItemInt(hwnd, EDT_LOOPEDSECT, 0,0);
	config.RepeatLooped = GetDlgItemInt(hwnd, EDT_LOOPEDSECT, 0,0);
	config.MS_Pause = GetDlgItemInt(hwnd, EDT_TRACKPAUSE, 0,0);
	config.RomPad = GetDlgItemInt(hwnd, EDT_ROMPAD, 0,0);
	config.setScale(xstr(getDlgItemText(hwnd, EDT_SCALE)));
	
	config.bitOpts = 0;
	for(int i = 0; i < 4; i++) { if(IsDlgButtonChecked(hwnd,
		ctrlList[i])) config.bitOpts |= bitCode[i]; }

	config.dupRemove = dlgCombo_getSel(hwnd, IDC_COMBO1);
	config.save();
}

INT_PTR OptionsDialog::DlgProc(HWND hwnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DLGMSG_SWITCH(
	 ON_MESSAGE(WM_CLOSE, EndDialog(hwnd, 0))
	 CASE_COMMAND(
	  ON_COMMAND(IDOK, OnOk(hwnd); EndDialog(hwnd, 0))
	  ON_COMMAND(BTN_EMUPATH,
	   OpenFileName ofn;
	   ofn.lpstrFilter = "*.exe\0*.exe\0\0";
	   if(ofn.doModal(hwnd)) {
		setDlgItemText(hwnd, EDT_EMUPATH, ofn.lpstrFile);
		CheckDlgButton(hwnd, CHK_EMUSPAWN, 1); })
	 ,)
	,)
}

void showOptions(HWND hwnd)
{	
	OptionsDialog od1;
	DialogBoxParam(GetModuleHandle(0), 
		L"OPTIONS_DIALOG", 
		hwnd, OptionsDialog::cDlgProc, (LPARAM)&od1);
}

struct MainDialog
{
	RECT* theRect;
	
	MEMBER_DLGPROC(MainDialog, DlgProc,)
};

INT_PTR MainDialog::DlgProc(HWND hwnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DLGMSG_SWITCH(
	 ON_MESSAGE(WM_CLOSE, EndDialog(hwnd, 1))
	 ON_MESSAGE(WM_DESTROY, GetWindowRect(hwnd, theRect))
	 CASE_COMMAND(
	  ON_COMMAND(IDCANCEL,  EndDialog(hwnd, 1))
	  ON_COMMAND(BTN_OPTIONS, showOptions(hwnd))
	  ON_COMMAND(BTN_ROMBUILD, EndDialog(hwnd, 2))
	 ,)
	,)	
}

int greet(RECT& mainRect)
{
	MainDialog md1;
	md1.theRect = &mainRect;
	int retVal = DialogBoxParam(GetModuleHandle(0), 
		L"MAIN_DIALOG", NULL, 
		MainDialog::cDlgProc, (LPARAM)&md1);
	if(retVal <= 0)
		fatalError(NULL,  "Cannot create main dialog"); 
	return retVal;
}
