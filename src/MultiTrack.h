#ifndef _MultiTrk_H_
#define _MultiTrk_H_
#include "VGM_PLAY.h"
#include "image.h"

byte* loadvgm(char* fName, int& fileSize);
void MtiRectCalc(HWND hwnd, RECT &rect);
bool MultiTrackOne(char *argv[]);
bool MultiTrackCmd(RECT& rc);
bool MultiTrackWin(RECT& rc);

class MultiTrack
{
public:
	// methods
	MultiTrack();
	char add(char* fName);
	void remove(int iTrack);
	void select(int iTrack, bool);
	bool build();
	
	// properties
	BYTE uiFlags;
	int totalSize();
	int nTracks;
	struct Info
	{
		bool selected;
		char* filename;
		ImageInfo* img;
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
	
	
	void imgReset(void);
};

#endif
