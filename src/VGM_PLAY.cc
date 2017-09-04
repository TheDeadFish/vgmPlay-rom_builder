#include "VGM_PLAY.h"
#include "FirstGui.h"
#include "MultiTrack.h"
#include <psapi.h>
#include "lib/mingw-unicode.c"
const char progName[] = "vgmPlay";

TCHAR* ExePath;
TCHAR* ExeName;
TCHAR* Config::EmuPath;
Config config;

TCHAR* ExePathCat(const char* name)
{
	TCHAR* dst = ExeName;
	while(*dst++ = *name++);
	return ExePath;
}

void Config::getScale(char* buff)
{
	buff[0] = '\0';
	if((scaleNum != 1)
	||(scaleDom != 1))
	{
		sprintf(buff, 
			(scaleDom != 1) ? "%g/%g" : "%g",
			scaleNum, config.scaleDom);	
	}
}

void Config::setScale(const char* buff)
{
	scaleNum = 1;
	scaleDom = 1;
	sscanf(buff, "%lf/%lf",	&scaleNum, &scaleDom);
}

void Config::load()
{
	// Get program directory
	TCHAR buff[MAX_PATH];
	if(GetModuleFileName(NULL, buff, MAX_PATH) == MAX_PATH)
		errorMaxPath();
	int pathLen = getPathLen(buff);
	ExePath = xmalloc(pathLen+20);
	ExeName = ExePath+pathLen;
	strcpy(ExePath, buff);

	// Load configuration
	FILE* fp = xfopen(
		ExePathCat("vgmPlay.cfg"), _T("rb"));
	if( fp != NULL )
	{
		int size = fsize(fp) - sizeof(Config);
		if(size >= 0)
		{
			size /= sizeof(TCHAR);
			EmuPath = xmalloc(size+1);
			EmuPath[size] = '\0';
			xfread(*this, fp);
			xfread(EmuPath, size, fp);
		}
		fclose(fp);
	}
	
	// Load defaults
	if(EmuPath == NULL)
	{
		RepeatLooped = 3;
		MS_Pause = 750;
		RomPad = 0;
		EmuPID = -1;
		scaleNum = 1;
		scaleDom = 1;
		bitOpts = EmuTerm | DacAvenc;
		dupRemove = 1;
		EmuPath = (TCHAR*)"";
	}
}

void Config::save()
{
	FILE* fp = xfopen(
		ExePathCat("vgmPlay.cfg"), "!wb");
	xfwrite(*this, fp);
	xfwrite(EmuPath, strlen(EmuPath)+1, fp);
	fclose(fp);
}

void callrom(void)
{
	// Terminate previous
	if((config.bitOpts & EmuTerm)&&(config.EmuPID != -1))
	{
		TCHAR procName[MAX_PATH] = _T("");
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | 
			PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, config.EmuPID);
		if(hProcess != NULL)
		{
			GetModuleFileNameEx(hProcess, NULL, procName, MAX_PATH);
			if(stricmp(procName, config.EmuPath) == 0)
				TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
	}
	
	// Call emulator
	if(config.bitOpts & EmuSpwn)
	{
		TCHAR cmdLine[560];
		snprintf(cmdLine, ARRAYSIZE(cmdLine), _T("\"%s\" \"%s\""),
			config.EmuPath, ExePathCat("vgmPlay.bin"));
		
		STARTUPINFO si = {0};
		PROCESS_INFORMATION pi = {0};
		if( !CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 
			DETACHED_PROCESS, NULL, NULL, &si, &pi))
		{
			contError(NULL, "Failed to call emulator, emulator calling will be disabled");
			config.bitOpts &= ~EmuSpwn;
		}
		else
		{
			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );
			config.EmuPID = pi.dwProcessId;
		}
		config.save();
	}
}

int _tmain(int argc, char *argv[])
{
	config.load();
	bool built = false;
	if(argc >= 2)
	{
		if(strEicmp(argv[1], _T(".EXE")) == 0)
		{
			config.EmuPath = xstrdup(argv[1]);
			config.bitOpts |= EmuSpwn;
			config.save();
			return 0;
		}
		built = MultiTrackOne(argv);
	}
	else
	{
		RECT rc;
		int options = greet(rc);
		if( options != 2 )
			return 0;
		if( config.bitOpts & ClsRombu )
			built = MultiTrackCmd(rc);
		else
			built = MultiTrackWin(rc);
	}
	if(built == true)
		callrom();
	return 0;
}
