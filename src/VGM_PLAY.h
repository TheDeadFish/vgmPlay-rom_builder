#ifndef _VGM_PLAY_H_
#define _VGM_PLAY_H_

// global strings
extern char* ExePath;
extern char* ExeName;
char* ExePathCat(const char* name);

// configuration
struct Config
{
	int RepeatLooped;
	int MS_Pause;
	int RomPad;
	int EmuPID;
	int bitOpts;
	double scaleNum;
	double scaleDom;
	int dupRemove;
	char* EmuPath;
	
	void getScale(char* buff);
	void setScale(const char* buff);
	void load();
	void save();
};
extern Config config;
#define EmuSpwn 1
#define EmuTerm 2
#define DacAvenc 8
#define ClsRombu 16

#endif
