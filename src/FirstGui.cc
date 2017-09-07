#include "stdafx.h"
#include "VGM_PLAY.h"
#include "FileList.h"
#include "resource.h"
#include <windowsx.h>

const LPCSTR dupRemOpts[] = {"Dissabled", 
	"Safe Mode", "Emulator Mode", "Hardware Mode"};


CDialog(OptionsDialog)	
	OptionsDialog(){
		fishhdc = NULL;}
	HDC fishhdc;
	BITMAP fishbm;
	CMainWndProcHandlr()
	// non message members
	void ChkState(bool mode);
_CDialog(OptionsDialog)

void OptionsDialog::ChkState(bool mode)
{
	HWND hwnd = _hwnd;
	int bitCode[] = {EmuSpwn, EmuTerm, DacAvenc, ClsRombu};
	short ctrlList[] = {CHK_EMUSPAWN, CHK_EMUTERM, CHK_DACAVENC, CHK_CLASSICRB};
	if( mode == false) // initialise check boxes from config
		for(int i = 0; i < 4; i++)
			Button(ctrlList[i]).SetCheck( (config.bitOpts & bitCode[i]) ? BST_CHECKED : BST_UNCHECKED );
	else{ // get config from check boxes
		config.bitOpts = 0;
		for(int i = 0; i < 4; i++)
			config.bitOpts |= (Button(ctrlList[i]).GetCheck() == BST_CHECKED) ? bitCode[i] : 0;
	}
}

FMainWndProcHandlrM(OptionsDialog)
{
	MSGIF( SINGLE_MSG_TST(WM_INITDIALOG) ){
		// Emulator path
		setDlgItemText(hwnd, EDT_EMUPATH, config.EmuPath);
		// Play Looped Setion, pause after non-looped tracks, RomPad Multiple
		SetDlgItemInt(EDT_LOOPEDSECT, config.RepeatLooped, 0);
		SetDlgItemInt(EDT_TRACKPAUSE, config.MS_Pause, 0);
		SetDlgItemInt(EDT_ROMPAD, config.RomPad, 0);
		// Emualtor spawning, Emulator termination, Dac Average Encoding
		ChkState( false );
		// Duplicate Write Removal
		dlgCombo_addStrs(hwnd, IDC_COMBO1, dupRemOpts, 4);
		dlgCombo_setSel(hwnd, IDC_COMBO1, config.dupRemove);
		char buff[64]; config.getScale(buff);
		setDlgItemText(hwnd, EDT_SCALE, buff);
	}
	MSGIF( WM_COMMAND_TST1( BTN_EMUPATH ) ){
		OpenFileName ofn;
		ofn.lpstrFilter = "*.exe\0*.exe\0\0";
		if(ofn.doModal(hwnd)) {
			setDlgItemText(hwnd, EDT_EMUPATH, ofn.lpstrFile);
			Button(CHK_EMUSPAWN).SetCheck(1);	
		}
	}
	MSGIF(WM_COMMAND_TST1( IDOK )){
		// Emulator path
		free_repl(config.EmuPath, getDlgItemText(hwnd, EDT_EMUPATH));
		// Play Looped Section, pause after non-looped tracks, RomPad Multiple
		
		config.RepeatLooped = GetDlgItemInt(EDT_LOOPEDSECT, 0,0);
		config.MS_Pause = GetDlgItemInt(EDT_TRACKPAUSE, 0,0);
		config.RomPad = GetDlgItemInt(EDT_ROMPAD, 0,0);
		config.setScale(xstr(getDlgItemText(hwnd, EDT_SCALE)));
		ChkState( true );
		config.dupRemove = dlgCombo_getSel(hwnd, IDC_COMBO1);
		config.save();
		EndDialog(1);
	}
	MSGIF(SINGLE_MSG_TST( WM_CLOSE ))
		EndDialog(1);
}_FMainWndProcHandlrM()

void showOptions(HWND hwnd)
{	
	OptionsDialog od1;
	if((int)od1.Create(hwnd, _T("OPTIONS_DIALOG"), true) <= 0)
		fatalError(hwnd, "Cannot create options dialog");
}
		
CDialog(MainDialog)
	RECT* theRect;
	CMainWndProcHandlr()
_CDialog(MainDialog)

FMainWndProcHandlrM(MainDialog)
{
	MSGIF( WM_COMMAND_TST1( BTN_OPTIONS ) )
		showOptions(hwnd);
	MSGIF( SINGLE_MSG_TST( WM_CLOSE) || WM_COMMAND_TST1( IDCANCEL ) )
		EndDialog(1);
	MSGIF( WM_COMMAND_TST1( BTN_ROMBUILD ) )
		EndDialog(2);
	MSGIF( SINGLE_MSG_TST( WM_DESTROY ) )
		GetWindowRect(theRect);
}_FMainWndProcHandlrM()

int greet(RECT& mainRect)
{
	MainDialog md1;
	md1.theRect = &mainRect;
	int retVal = (int)md1.Create(NULL, _T("MAIN_DIALOG"), true);
	if(retVal <= 0)
		fatalError(NULL,  "Cannot create main dialog"); 
	return retVal;
}
