#include "stdafx.h"
#include "MultiTrack.h"
#include <math.h>
#include "mdcode.h"

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

	uiFlags = 0xFF;
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

	trk.selected = false;
	trk.filename = getName(filename).xdup();
	trk.img = NULL; return 0;
}

void MultiTrack::imgReset(void)
{
	for(int i = 0; i < nTracks; i++) {
		if(data[i].img) data[i].img->fileOffset = 0; }
}

int MultiTrack::totalSize(void)
{
	this->imgReset();
	int size = romSize+3;
	for(int i = 0; i < nTracks; i++)
	if(!data[i].selected) { size += data[i].filesize;
		if((data[i].img)&&(!data[i].img->fileOffset)) {
			data[i].img->fileOffset = 1; 
			size += data[i].img->getSize();  }
	}
	
	return size;
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
	data[iTrack].selected = select;
}

bool MultiTrack::build()
{
	if(nTracks == 0)
		return false;
	FILE* fpout;
	uint BigOut;
	uint RomPos;

	// Write VGC player
	mdcode_setUiFlags(romData, uiFlags);
	mdcode_setRepLoop(romData, (nTracks > 1)
		? config.RepeatLooped : 0);
	mdcode_setPause(romData, lrint(config.MS_Pause*44.1));
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
	RomPos = ALIGN4(RomPos); this->imgReset();
	for(int i = 0; i < nTracks; i++) { DWORD offset = 0;
		if(data[i].img) { if(!data[i].img->fileOffset) { 
		data[i].img->fileOffset = release(RomPos,
			RomPos+data[i].img->getSize()); }
		offset = bswap32(data[i].img->fileOffset); }
		RI(data[i].vgmdata, 20) = offset; 
		xfwrite(data[i].vgmdata, data[i].filesize, fpout); }
		
	// write images to ROM
	for(int i = 0; i < nTracks; i++) if((data[i].img)
		&&(data[i].img->fileOffset)) { fseek(fpout, 
			release(data[i].img->fileOffset), SEEK_SET);
			data[i].img->write(fpout); }
	
	// pad out the ROM
	int padSize = getPadSize(RomPos);
	if(RomPos < padSize)
		xchsize(fpout, padSize);
	fclose(fpout);
	return true;
}
