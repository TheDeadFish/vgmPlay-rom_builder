#ifndef _FILENAMELIST_H_
#define _FILENAMELIST_H_
#include <windows.h>
#include <tchar.h>

class FileNameList
{
public:
	FileNameList() { ZINIT; }
	~FileNameList() { Free(); }
	void Free();
	TCHAR* Get();
	
	void FromArgv(TCHAR *argv[]);
	void FromOfn(HWND parent);
	void FropHdrop(HDROP hDrop);

private:
	bool FromM3u(TCHAR *name);

	TCHAR** names;
	TCHAR** curPos;
	int mustFree;
};

#endif
