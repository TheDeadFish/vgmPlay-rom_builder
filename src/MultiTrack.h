#ifndef _MultiTrk_H_
#define _MultiTrk_H_
#include "VGM_PLAY.h"

byte* loadvgm(TCHAR* fName, int& fileSize);
void MtiRectCalc(HWND hwnd, RECT &rect);
bool MultiTrackOne(TCHAR *argv[]);
bool MultiTrackCmd(RECT& rc);
bool MultiTrackWin(RECT& rc);

class MultiTrack
{
public:
	// methods
	MultiTrack();
	char add(TCHAR* fName);
	void remove(int iTrack);
	void select(int iTrack, bool);
	bool build();
	
	// properties
	int totalSize;
	int nTracks;
	struct Info
	{
		bool selected;
		char* filename;
	};
	Info& operator[](int iTrack)
		{	return data[iTrack]; }
	
private:
	int romSize;
	byte* romData;
	struct Data : Info
	{
		byte* vgmdata;
		int filesize;
	};
	Data* data;
};

#endif
