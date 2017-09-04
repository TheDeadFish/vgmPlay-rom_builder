#include "VGM_PLAY.h"
#include "filelist.h"

#define OFN_FILTER "VGM Files\0*.VGM;*.VGZ;*.M3U\0All Files\0*.*\0\0"
#define OFN_OPENERR "GetOpenFileName failed"
#define OFN_BUFFERR "Too many files selected, Insufficient buffer"
#define OFN_MAXFILE 65535
#define OFN_FLAGS OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_EXPLORER | 0x02000000

void FileNameList::Free()
{
	if(mustFree)
	{	
		for(int i = 0; names[i]; i++)
			free(names[i]);
		free(names);
		mustFree = false;	
	}
	names = NULL;
	curPos = NULL;
}

TCHAR* FileNameList::Get()
{
	if(curPos == NULL)
		return NULL;
	if(*curPos == NULL)
		return NULL;
	return *curPos++;
}

void FileNameList::FromArgv(TCHAR *argv[])
{
	this->Free();
	
	if((argv[0] != NULL)
	&&(!FromM3u(argv[0])))
	{
		names = argv;
		curPos = argv;
	}
};

void FileNameList::FromOfn(HWND parent)
{
	this->Free();
	
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parent;
	ofn.lpstrFilter = _T( OFN_FILTER );
	ofn.lpstrFile = xmalloc( OFN_MAXFILE );
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = OFN_MAXFILE;
	ofn.Flags = OFN_FLAGS;
	
	if(GetOpenFileName(&ofn))
	{
		TCHAR* pathEnd = _tcschr(ofn.lpstrFile, '\0');
		if(pathEnd[1] == 0)
		{
			// single file-name
			if(!FromM3u(ofn.lpstrFile))
			{
				names = xmalloc(2);
				names[0] = xstrdup(ofn.lpstrFile);
				names[1] = NULL;
			}
		}
		else
		{
			// multi-name madness
			int nFiles = 0;
			*pathEnd++ = '\\';
			TCHAR* nameBgn = pathEnd;
			while(nameBgn[0])
			{
				TCHAR* nameEnd = _tcschr(nameBgn, '\0');
				_tcscpy(pathEnd, nameBgn);
				xrealloc(names, nFiles+2);
				names[nFiles++] = xstrdup(ofn.lpstrFile);
				nameBgn = nameEnd+1;				
			}
			names[nFiles] = NULL;
		}
		
		curPos = names;
		mustFree = true;
		free(ofn.lpstrFile);
		return;	
	}

	switch(CommDlgExtendedError())
	{
	default:
		fatalError(parent, OFN_OPENERR);
		break;
	case FNERR_BUFFERTOOSMALL:
		contError(parent, OFN_BUFFERR);
	case 0:
		free(ofn.lpstrFile);
	}
}

void FileNameList::FropHdrop(HDROP hDrop)
{
	this->Free();

	int argc = DragQueryFile(hDrop, (UINT)-1, NULL, 0);
	if(argc == 1)
	{
		int len = DragQueryFile(hDrop, 0, NULL, 0);
		TCHAR* fName = xmalloc(len + 1);
		DragQueryFile(hDrop, 0, fName, len+1);
		if(FromM3u(fName))
		{
			free(fName);
			return;
		}
		names = xmalloc(argc+1);
		names[0] = fName;
	}
	else
	{
		names = xmalloc(argc+1);
		for(int i = 0; i < argc; i++)
		{
			int len = DragQueryFile(hDrop, i, NULL, 0);
			names[i] = xmalloc(len + 1);
			DragQueryFile(hDrop, i, names[i], len+1);
		}
	}

	names[argc] = NULL;
	curPos = names;
	mustFree = true;
	DragFinish(hDrop);
}

bool FileNameList::FromM3u(TCHAR* fName)
{
	if(strEicmp(fName, _T(".m3u")))
		return false;

	// Prepare buffer
	int pathLen = getPathLen(fName);
	TCHAR* tmpBuff = xmalloc(pathLen + MAX_PATH);
	TCHAR* tmpName = tmpBuff + pathLen;
	strNcpy(tmpBuff, fName, pathLen);
	
	// Process file
	FILE* fp = xfopen(fName, _T("!r"));	
	names = xmalloc(1);
	int nFiles = 0;
	while(xfgets(tmpName, MAX_PATH, fp))
	{
		if((tmpName[0] <= ' ')
		||(tmpName[0] == '#'))
			continue;
		removeCrap(tmpName);
		
		names[nFiles++] = xstrdup(
			isFullPath(tmpName)	? tmpName : tmpBuff);
		xrealloc(names, nFiles+1);
	}
	
	free(tmpBuff);
	names[nFiles] = NULL;
	curPos = names;
	mustFree = true;
	return true;
}
