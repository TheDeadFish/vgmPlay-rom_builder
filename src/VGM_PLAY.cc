#include "stdafx.h"
#include "VGM_PLAY.h"
#include "FirstGui.h"
#include "MultiTrack.h"
#include <psapi.h>

const char progName[] = "vgmPlay";

INI_DEFUCT(vgmPlayConf, Config, INI_DS8(RepeatLooped, Int)
	INI_DS8(MS_Pause, Int) INI_DS(RomPad, Int)
	INI_DSN(EmuPID, Int) INI_DS8(bitOpts, Int)
	INI_DS1(dupRemove, Int) INI_DS1(scaleNum, Double)
	INI_DS1(scaleDom, Double)INI_DS8(EmuPath, Str));
	
	

Config config;

char* ExePathCat(const char* name)
{
	static char* ExePath;
	free_repl(ExePath, pathCat(
		getProgramDir(), name));
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
	{ IniFile_Load ini;
	ini.load(ExePathCat("vgmPlay.cfg"));
	vgmPlayConf->readBlock(&ini, "config", this); }
	if(RepeatLooped < 0) RepeatLooped = 3;
	if(MS_Pause < 0) MS_Pause = 750;
	if(bitOpts < 0) bitOpts = EmuTerm | DacAvenc;
}

void Config::save()
{
	IniFile_Save ini;
	ini.create(ExePathCat("vgmPlay.cfg"));
	vgmPlayConf->writeBlock(&ini, "config", this);
	if(!ini.close()) contError(NULL,
		"failed to save config file");	
}

void callrom(void)
{
	// Terminate previous
	if((config.bitOpts & EmuTerm)&&(config.EmuPID != -1))
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | 
			PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, config.EmuPID);
		if(hProcess != NULL)
		{
			xstr procName = getModuleFileNameEx(hProcess, 0);
			if(procName && !stricmp(procName, config.EmuPath))
				TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
	}
	
	// Call emulator
	if(config.bitOpts & EmuSpwn)
	{
		xstr cmd = xstrfmt("\"%s\" \"%s\"", 
			config.EmuPath, ExePathCat("vgmPlay.bin"));
		
		STARTUPINFOA si = {0};
		PROCESS_INFORMATION pi = {0};
		if( !createProcess(NULL, cmd, NULL, NULL, FALSE, 
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

int main(int argc, char *argv[])
{
	config.load();
	bool built = false;
	if(argc >= 2)
	{
		if(strEicmp(argv[1], ".EXE") == 0)
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
