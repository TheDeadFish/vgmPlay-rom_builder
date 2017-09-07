#include "stdafx.h"
#include "MultiTrack.h"
#include <math.h>

void MtiRectCalc(HWND winHwnd, RECT &rect)
{
	RECT deskRect, winRect;
	HWND deskHwnd;
	rect.left -= 126;
	rect.top -= 189;
	deskHwnd = GetDesktopWindow();
	GetWindowRect(winHwnd, &winRect);
	GetWindowRect(deskHwnd, &deskRect);
	winRect.right -= winRect.left;
	winRect.bottom -= winRect.top;
	if(((deskRect.right - deskRect.left) < winRect.right)||
	((deskRect.bottom - deskRect.top) < winRect.bottom))
		return;
	if((rect.left + winRect.right) > deskRect.right)
		rect.left = deskRect.right - winRect.right;
	if(rect.left < deskRect.left)
		rect.left = deskRect.left;
	if((rect.top + winRect.bottom) > deskRect.bottom)
			rect.top = deskRect.bottom - winRect.bottom;
	if(rect.top < deskRect.top)
		rect.top = deskRect.top;
	MoveWindow(winHwnd, rect.left, rect.top, winRect.right, winRect.bottom, TRUE);
}

static
int getPadSize(int RomSize)
{
	int padMin = config.RomPad * 1024; 
	if(padMin == 0)
		return RomSize;
	if(RomSize < padMin)
		return padMin;
	return snapUpSize(RomSize);
}

MultiTrack::MultiTrack()
{
	// Load VGC player 
	FILE* fp = xfopen(ExePathCat("vgmPlay.dat"), "!rb");
	romSize = fsize(fp);
	romData = xmalloc(romSize);
	xfread(romData, romSize, fp);
	fclose(fp);

	totalSize = romSize;
	nTracks = 0;
	data = NULL;
}

char MultiTrack::add(char* filename)
{
	// Load VGM file
	auto& trk = xNextAlloc(data, nTracks);
	trk.vgmdata = loadvgm(
		filename, trk.filesize);
	if(trk.vgmdata == 0)
		return trk.filesize;

	totalSize += trk.filesize+4;
	trk.selected = false;
	trk.filename = NULL;
	return 0;
}

void MultiTrack::remove(int iTrack)
{
	select(iTrack, true);
	free(data[iTrack].vgmdata);
	free(data[iTrack].filename);
	nTracks--;
	for(int i = iTrack; i < nTracks; i++)
		data[i] = data[i+1];
}

void MultiTrack::select(int iTrack, bool select)
{
	if(data[iTrack].selected != select)
	{	
		data[iTrack].selected = select;
		if(select)
			totalSize -= data[iTrack].filesize+4;
		else
			totalSize += data[iTrack].filesize+4;
	}
}

bool MultiTrack::build()
{
	if(nTracks == 0)
		return false;
	FILE* fpout;
	uint BigOut;
	uint RomPos;

	// Write VGC player
	if(nTracks > 1)
		*(uint*)(romData+8) = bswap32(config.RepeatLooped);
	else
		*(uint*)(romData+8) = 0;
	*(uint*)(romData+12) = bswap32( lrint(config.MS_Pause*44.1) );
	fpout = xfopen(ExePathCat("vgmPlay.bin"), "!wb");
	printf("%X\n", fpout);
	
	xfwrite(romData, romSize, fpout);

	// write number of tracks to ROM
	BigOut = bswap32( nTracks );
	xfwrite(BigOut, fpout);
	RomPos = romSize + 4;
	RomPos += nTracks<<2;

	// write track start addresses to ROM
	for(int i = 0; i < nTracks; i++)
	{
		BigOut = bswap32(RomPos);
		xfwrite(BigOut, fpout);
		RomPos += data[i].filesize;
	}
			
	// write track data to ROM
	for(int i = 0; i < nTracks; i++)
		xfwrite(data[i].vgmdata, data[i].filesize, fpout);
	
	// pad out the ROM
	int padSize = getPadSize(RomPos);
	if(RomPos < padSize)
		xchsize(fpout, padSize);
	fclose(fpout);
	return true;
}
