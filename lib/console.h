#ifndef _CONSOLE_H_
#define _CONSOLE_H_

extern HANDLE hStdOut;
extern HWND conHwnd;

BOOL Clrscr(void);
BOOL locate(short x, short y);
BOOL color(short foreol);
BOOL initcon(void);
BOOL ClearLine(int row);
BOOL ClearBlock(int startrow, int endrow);
BOOL fullcolor(short color);
BOOL init_colmap(void);
BOOL set_colmap(int col);
#define COLOR_NORMAL 0
#define COLOR_HIGLIH 1
#define COLOR_WARNIH 2
#define COLOR_SELCTD 4

#endif
