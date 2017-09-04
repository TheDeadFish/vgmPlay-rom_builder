#include "VGM_PLAY.h"
#include "WinOO\WinOO.h"
#include "FileList.h"
#include "resource.h"
#include <windowsx.h>

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
		Edit(EDT_EMUPATH).SetWindowText(config.EmuPath);
		// Play Looped Setion, pause after non-looped tracks, RomPad Multiple
		Edit(EDT_LOOPEDSECT).SetWindowText(ToString(config.RepeatLooped));
		Edit(EDT_TRACKPAUSE).SetWindowText(ToString(config.MS_Pause));
		Edit(EDT_ROMPAD).SetWindowText(ToString(config.RomPad));
		// Emualtor spawning, Emulator termination, Dac Average Encoding
		ChkState( false );
		// Duplicate Write Removal
		HWND hCombo = ::GetDlgItem(hwnd, IDC_COMBO1);
		ComboBox_AddString(hCombo, _T("Dissabled"));
		ComboBox_AddString(hCombo, _T("Safe Mode"));
		ComboBox_AddString(hCombo, _T("Emulator Mode"));
		ComboBox_AddString(hCombo, _T("Hardware Mode"));
		ComboBox_SetCurSel(hCombo, config.dupRemove);
		char buff[64];
		config.getScale(buff);
		Edit(EDT_SCALE).SetWindowText(buff);
	}
	MSGIF( WM_COMMAND_TST1( BTN_EMUPATH ) ){
		TCHAR fName[MAX_PATH] = _T("");
		OPENFILENAME ofn = {0};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFilter = _T("*.exe\0*.exe\0\0");
		ofn.lpstrFile = fName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER;
		if(GetOpenFileName(&ofn))
		{
			Edit(EDT_EMUPATH).SetWindowText(fName);
			Button(CHK_EMUSPAWN).SetCheck(1);
		}
	}
	MSGIF(WM_COMMAND_TST1( IDOK )){
		// Emulator path
		int EmuPathLen = Edit(EDT_EMUPATH).GetWindowTextLength()+1;
		config.EmuPath = xmalloc(EmuPathLen);
		Edit(EDT_EMUPATH).GetWindowText(config.EmuPath, EmuPathLen+1);
		// Play Looped Section, pause after non-looped tracks, RomPad Multiple
		CString tmp;
		Edit(EDT_LOOPEDSECT).GetWindowText(tmp); config.RepeatLooped = tmp;
		Edit(EDT_TRACKPAUSE).GetWindowText(tmp); config.MS_Pause = tmp;
		Edit(EDT_ROMPAD).GetWindowText(tmp); config.RomPad = tmp;
		Edit(EDT_SCALE).GetWindowText(tmp); config.setScale(tmp);
		ChkState( true );
		config.dupRemove = ComboBox_GetCurSel(
			::GetDlgItem(hwnd, IDC_COMBO1));
		config.save();
		EndDialog(1);
	}
	MSGIF(SINGLE_MSG_TST( WM_CLOSE ))
		EndDialog(1);
}_FMainWndProcHandlrM()

void showOptions(HWND hwnd)
{	
	OptionsDialog od1;
	if((int)od1.Create(hwnd, "OPTIONS_DIALOG", true) <= 0)
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
	int retVal = (int)md1.Create(NULL, "MAIN_DIALOG", true);
	if(retVal <= 0)
		fatalError(NULL,  "Cannot create main dialog"); 
	return retVal;
}
