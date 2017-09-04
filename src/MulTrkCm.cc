#include "MultiTrack.h"
#include "filelist.h"
#include "lib/console.h"
#include <conio.h>

struct MultiTrackCmdLine
{
	MultiTrack mt;
	
	int linePos;
	bool dragMode;
	bool dragAway;
	bool showCaret;
	int dragOrgn;
	int Caret;
	int PTpage;
	int ScreenCaret;
	
	void PrintUsed(int used);
	void PrintTracks(char mode, bool dir);
	bool Get_Command(void);
	void AddFile();
};

BOOL WINAPI HandlerRoutine_MulTrkCm(DWORD dwCtrlType)
{
	(void)dwCtrlType;
	INPUT_RECORD ir;
	ir.EventType = KEY_EVENT;
	ZeroMemory(&ir.Event.KeyEvent, sizeof(KEY_EVENT_RECORD));
	ir.Event.KeyEvent.bKeyDown = TRUE;
	ir.Event.KeyEvent.wVirtualKeyCode = VK_ESCAPE;
	ir.Event.KeyEvent.uChar.AsciiChar = 27;
	WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &ir, 1, (LPDWORD)&ir);
	printf("conorot\n");
	return TRUE;
}

bool MultiTrackCmd(RECT& rect)
{
	MultiTrackCmdLine mti;
	mti.showCaret = false;
	mti.Caret = 0;
	mti.PTpage = 0;
	mti.ScreenCaret = 0;
	
	// Initialise console.c
	initcon(); // forget the error handling for now
	ClearBlock(0,24);
	
	// Select a pleasing window size*/	
	{	SMALL_RECT tmp = {0, 0, 79, 24};
		SetConsoleWindowInfo(hStdOut, TRUE, &tmp);
	}
	{	COORD tmp = {80, 25};
		SetConsoleScreenBufferSize(hStdOut, tmp);
	}
	SetConsoleCtrlHandler(HandlerRoutine_MulTrkCm, TRUE);
	
	// remaining window initialisation
	MtiRectCalc(conHwnd, rect);
	ShowWindow(conHwnd, SW_SHOW);
	SetForegroundWindow(conHwnd);

	// the console window is now initialised we must generate the interface
	locate(0,0); // forget error handling for now
	printf("Multitrack rom builder.");
	locate(0,2); // forget error handling for now
	printf("1. Add Track \n");
	printf("2. Remove Track \n");
	printf("3. Play Looped section (%d) \n", config.RepeatLooped);
	printf("4. Pause after none-looped tracks (%dms) \n", config.MS_Pause);
	printf("ENTER. Build rom \n");
	
	return mti.Get_Command();
}

bool MultiTrackCmdLine::Get_Command()
{
	while(1){
		PrintUsed(mt.totalSize);
		fflush(stdin);
		byte option = getch();
		switch(option)
		{
		case 0:
		case 224:
			continue;
		case 27:
			return false;
		case '1':
			this->AddFile();
			break;
		case '3':
			locate(0, 7); // forget error handling for now
			printf("Enter new value for Play Looped Section: ");
			scanf("%d", &config.RepeatLooped); 
			locate(0, 4);
			printf("3. Repeat Looped section (%d)        ", config.RepeatLooped);
			ClearLine(7);
			break;
		case '4':
			locate(0, 7);
			printf("Enter new value for Pause after none-looped tracks: ");
			scanf("%d",  &config.MS_Pause);
			locate(0, 5);
			printf("4. Pause after none-looped tracks (%dms)        ",  &config.MS_Pause);
			ClearLine(7);
		case 13:
			if(mt.nTracks != 0)
				return mt.build();
			locate(0, 7);
			printf("No Tracks Selected!");
			break;
		case '2': {
			// file delete code
			bool selected = false;
			for(int i = 0; i < mt.nTracks; i++)
				if(mt[i].selected){
					selected = true; break;}
			if(selected == false){
				locate(0,7);
				printf("Enter Track Number to remove: ");
				int Track;
				scanf("%d", &Track);
				ClearLine(7);
				if((Track>mt.nTracks)||(Track<1))
					break;
				Track--;
				mt.select(Track, true);
			}
			// remove the selected tracks
			for(int i = 0; i < mt.nTracks; i++)
				if(mt[i].selected){
					mt.remove(i);
					i--;
				}
			PrintTracks(0, 0);
			break; }
		// interface control]
		case 141:
		case 145:{
			if(showCaret == false)
				break;
			if(dragOrgn == -1){
				bool selected = mt[Caret].selected;
				mt.select(Caret, !selected);
				dragMode = !selected;
				dragOrgn = Caret;
				dragAway = false;
				PrintTracks(0, 0);
				continue;
			}
			bool dir = (option == 145);
			if((dragOrgn == Caret)&&(dragAway)){
				mt.select(Caret, !dragMode);
				dragOrgn = -1;
				PrintTracks(0, 0);
				continue;
			}
			dragAway = true;
			bool home = (dir) ? (dragOrgn > Caret) : (Caret > dragOrgn);
			if(home == false) PrintTracks(2, dir);
			mt.select(Caret, home^dragMode);
			PrintTracks((home) ? 2 : 0, dir);
			continue;}
		case '0':
		case 82:
			showCaret = true;
			for(int i = 0; i < mt.nTracks; i++)
				mt.select(i, false);
			PrintTracks(0, 0);
			break;
		case 81:
			PrintTracks(1, 1);
			break;
		case 73:
			PrintTracks(1, 0);
			break;
		case 80:
			PrintTracks(2, 1);
			break;
		case 72:
			PrintTracks(2, 0);
			break;
		case 32:
			if(showCaret)
			{
				mt.select(Caret, !mt[Caret].selected);
				PrintTracks(0, 0);
			}
			break;
		}
		dragOrgn = -1;
	}
}

void MultiTrackCmdLine::AddFile(void)
{
	FileNameList fnl;
	fnl.FromOfn(conHwnd);
	linePos = 7;
	while(TCHAR* name = fnl.Get())
	{
		TCHAR* filename = getName(name);
		locate(0, linePos);
		printf("#" TSTRPC, filename);
		char result = mt.add(name);
		if(result)
		{
			if(linePos == 8)
				ClearLine(7);
			else
				linePos = 8;
			locate(0, 7);
			set_colmap(COLOR_WARNIH);
			printf("%c" TSTRPC, result, filename);
			set_colmap(COLOR_NORMAL);
		}
		else
		{
			mt[mt.nTracks-1].filename = xstrdup(filename);
			PrintUsed(mt.totalSize);
		}
		ClearLine(linePos);
	}
	PrintTracks(0, 0);
}
		
// implementation functions
void MultiTrackCmdLine::PrintUsed(int used)
{
	char usedtemp[64];
	if(used > 1024*4096)
		set_colmap(COLOR_WARNIH);
	used /= 1024;
	sprintf(usedtemp, "        RomUsed: %dKB", used);
	locate(80-strlen(usedtemp), 0);
	printf("%s", usedtemp);
	set_colmap(COLOR_NORMAL);
}

void MultiTrackCmdLine::PrintTracks(char mode, bool dir)
{
	// calculate new position for the caret
	if(mode == 1) // page move
		Caret += (dir) ? 15 : -15;
	if((mode == 2)&&(showCaret)) // caret move
		Caret += (dir) ? 1 : -1;
	// check the bounds of the caret
	if(Caret >= mt.nTracks)
		Caret = mt.nTracks-1;
	if(Caret < 0)
		Caret = 0;
	// calculate the page position to match the Caret
	// fix the range of PTpage and the ScreenCaret
	locate(0, 7);
	if(mode == 1)
		PTpage = Caret - ScreenCaret;
	if(PTpage > Caret)
		PTpage = Caret;
	if(PTpage < Caret-14)
		PTpage = Caret-14;
	if(PTpage >= mt.nTracks-15)
		PTpage = mt.nTracks-15;
	if(PTpage < 0)
		PTpage = 0;
	ScreenCaret = Caret - PTpage;
	// display the lines
	int count, count_max;
	count_max = PTpage + 15;
	if(count_max > mt.nTracks)
		count_max = mt.nTracks;	
	ClearBlock(9, 23);
	locate(0,9);
	for(count=PTpage;count<count_max;count++){
		int colMode = COLOR_NORMAL;
		if((count == Caret)&&(showCaret))
			colMode = COLOR_HIGLIH;
		if( mt[count].selected )
			colMode |= COLOR_SELCTD;
		set_colmap(colMode);
		printf("%d: " TSTRPC "\n", count+1, mt[count].filename);	
	}
	set_colmap(COLOR_NORMAL);
}
