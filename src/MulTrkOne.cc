// implements generation of a ROM file when specified as argument
#include "MultiTrack.h"
#include "FileList.h"

bool MultiTrackOne(TCHAR *argv[])
{
	MultiTrack mt;
	FileNameList fnl;
	fnl.FromArgv( argv+1 );
	
	while(TCHAR* fName = fnl.Get())
	{
		if(mt.add(fName))
			contError(NULL, "Error Loading File \"" TSTRPC "\"", fName);
	}
	
	return mt.build();
}
