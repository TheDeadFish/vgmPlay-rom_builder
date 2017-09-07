#include <windows.h>
#include <stdio.h>
#include "console.h"

HANDLE hStdOut = 0;
HWND conHwnd = 0;

BOOL Clrscr(void)
{
	DWORD count;
	COORD coord = {0, 0};
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi) == FALSE)
		return(FALSE);
	if(FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count))
		return(FALSE);
	if(SetConsoleCursorPosition(hStdOut, coord))
		return(FALSE);
	return(TRUE);
}

BOOL locate(short x, short y)
{
	 COORD point;
     point.X = x;
     point.Y = y;
	 if(SetConsoleCursorPosition(hStdOut,point) == FALSE)
		return(FALSE);
	return(TRUE);
}

BOOL color(short forecol)
{
	forecol &= 15;
	if(SetConsoleTextAttribute(hStdOut,forecol) == FALSE)
		return(FALSE);
	return(TRUE);
}

BOOL initcon(void)
{
	if(AllocConsole())
	{
		// Setup stdin and stdout
		_wfreopen(L"conin$", L"r", stdin);
		_wfreopen(L"conout$", L"w", stdout);
		_wfreopen(L"conout$", L"w", stderr);
		setvbuf( stdin, NULL, _IONBF, 0 );
		setvbuf( stdout, NULL, _IONBF, 0 );
		setvbuf( stderr, NULL, _IONBF, 0 );
	}
	
	TCHAR pszOldWindowTitle[1000];
	TCHAR pszNewWindowTitle[40];
	wsprintf(pszNewWindowTitle, TEXT("FindWindow:%d/%d"),
		GetTickCount(), GetCurrentProcessId());
	GetConsoleTitle(pszOldWindowTitle, 1000);
	SetConsoleTitle(pszNewWindowTitle);
	Sleep(40);
	conHwnd = FindWindow(NULL, pszNewWindowTitle);
	SetConsoleTitle(pszOldWindowTitle);
	
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hStdOut == INVALID_HANDLE_VALUE)
		return(FALSE);
	return init_colmap();
}

BOOL ClearLine(int row)
{
	char tempchar[81];
	if(!locate(0, row))
		return(FALSE);
	memset(tempchar, 0x20, 80);
	tempchar[80] = 0;
	printf("%s", tempchar);
	return(TRUE);
}

BOOL ClearBlock(int startrow, int endrow)
{
	int count;
	for(count=startrow;count<=endrow;count++)
		if(!ClearLine(count))
			return(FALSE);
	return(TRUE);
}

// a nice set of new additions
BOOL fullcolor(short color)
{
	color &= 255;
	if(SetConsoleTextAttribute(hStdOut,color) == FALSE)
		return(FALSE);
	return(TRUE);
}

WORD colmapDef;
BOOL init_colmap(void)
{
	CONSOLE_SCREEN_BUFFER_INFO cinfo;
	if(!GetConsoleScreenBufferInfo(hStdOut, &cinfo))
		return FALSE;
	colmapDef = cinfo.wAttributes;
	return TRUE;
}

BOOL set_colmap(int col)
{
	short newCol;
	char invert = col & COLOR_SELCTD;
	col &= ~COLOR_SELCTD;
	if( col == COLOR_NORMAL)
		newCol = colmapDef;
	if( col == COLOR_HIGLIH)
		newCol = colmapDef^0x08;
	if( col == COLOR_WARNIH )
		if( (colmapDef & 7) == 4 )
			newCol = 14;
		else
			newCol = 12;
	if(invert)
		newCol <<= 4;
	return fullcolor(newCol);
}
