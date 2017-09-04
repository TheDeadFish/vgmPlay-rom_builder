#include "VGM_PLAY.h"

#define error_success		0
#define error_fileBad		1
#define error_RopenEr		2
#define error_WopenEr		3
#define error_srcErr		4
#define error_dstErr		5
#define error_UrecTyp		6
#define error_MemErr		7
#define error_badArgs		8
#define error_noDst			9
#define error_noSrc			10
#define error_undef			11

byte* loadvgm(TCHAR* fName, int& fileSize)
{
	// Prepare command line
	TCHAR cmdLine[560];	
	char opts[64] = "-d0 -q -s";
	config.getScale(opts+9);
	opts[2] += config.dupRemove;
	if(config.bitOpts & DacAvenc) opts[5] = 'a';
	snprintf(cmdLine, ARRAYSIZE(cmdLine),
		_T("# -q -c -i %s \"%s\" \"%s\""),
		opts, ExePathCat("tmp.vgc"), fName);
	
	// Call vgmConv.exe with no console
	DWORD errCode;
	STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
	if( !CreateProcess( ExePathCat("vgmConv.exe"), cmdLine,
	NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL, &si, &pi))
		fatalError("failed to call vgmConv.exe");
	WaitForSingleObject( pi.hProcess, INFINITE );
	GetExitCodeProcess(pi.hProcess, &errCode);
	CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
	if(errCode != 0)
	{
		// handle vgmConv error
		switch(errCode-10)
		{
		case error_fileBad:
		case error_RopenEr:
			fileSize = '!';
			break;
		case error_srcErr:
			fileSize = 'X';
			break;
		case error_UrecTyp:
			fileSize = '?';
			break;
		default:
			fatalError("vgmConv.exe failed, error: %d", errCode-10);
		}
		return NULL;
	}
	
	// Open temporary VGC file
	int vgcHeader[6], tmpSize;
	FILE* fp = xfopen(ExePathCat("tmp.vgc"), _T("!rbD"));
	xfread(vgcHeader, fp);
	tmpSize = bswap32(vgcHeader[5]);
	fileSize = (tmpSize+1) & ~1;
	byte* data = xmalloc(fileSize);
	memcpy(data, vgcHeader, 24);
	xfread(data+24, tmpSize-24, fp);
	fclose(fp);
	return data;
}
