#ifndef _FILENAMELIST_H_
#define _FILENAMELIST_H_
#include <windows.h>

class FileNameList
{
public:
	FileNameList() { ZINIT; }
	~FileNameList() { Free(); }
	void Free();
	char* Get();
	
	void FromArgv(char *argv[]);
	void FromOfn(HWND parent);
	void FropHdrop(HDROP hDrop);

private:
	void FromM3u(void);
		
	xarray<xstr> names;
	int curPos;
	int mustFree;
};

#endif
