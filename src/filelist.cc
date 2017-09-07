#include "stdafx.h"
#include "filelist.h"

#define OFN_FILTER "VGM Files\0*.VGM;*.VGZ;*.M3U\0All Files\0*.*\0\0"
#define OFN_OPENERR "GetOpenFileName failed"
#define OFN_BUFFERR "Too many files selected, Insufficient buffer"
#define OFN_MAXFILE 65535
#define OFN_FLAGS OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_EXPLORER | 0x02000000

void FileNameList::Free()
{
	if(mustFree) { freeLst(
		names.data, names.len); }
	ZINIT;
}

char* FileNameList::Get()
{
	if(curPos < names.size) {
		return names[curPos++]; }
	return NULL;
}

void FileNameList::FromArgv(char *argv[])
{
	this->Free();
	names.data = (xstr*)argv;
	while(*names.end()) names.len++;
	this->FromM3u();
};

void FileNameList::FromOfn(HWND parent)
{
	this->Free();
	
	OpenFileName ofn;
	ofn.lpstrFilter = OFN_FILTER;
	ofn.Flags = OFN_FLAGS;
	if(!ofn.doModal(parent)) return;
	names.init(ofn.lst());
	ofn.lst().data = 0;
	mustFree = true;
	this->FromM3u();
}

void FileNameList::FropHdrop(HDROP hDrop)
{
	this->Free();	
	auto list = hDropGet(hDrop);
	names.init(list);
	mustFree = true;
	this->FromM3u();
}

void FileNameList::FromM3u(void)
{
	if((names.len != 1)
	||(strEicmp(*names, ".m3u"))) return;
	xstr fName = release(*names);
	this->Free();

	// load .m3u file
	int nLines; char** lines = loadText(fName, nLines);
	if(nLines < 0) fatalError(
		"failed to load .m3u file");
	SCOPE_EXIT(free(*lines); free(lines));
		
	// parse .m3u file
	for(char* line : Range(lines, nLines)) {
		if((line[0] <= ' ')
		||(line[0] == '#')) continue;
		removeCrap(line);
		names.push_back(replName(fName, line));
	}

	mustFree = true;
}
