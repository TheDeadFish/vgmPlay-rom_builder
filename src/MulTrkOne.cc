// implements generation of a ROM file when specified as argument
#include "stdafx.h"
#include "MultiTrack.h"
#include "FileList.h"

bool MultiTrackOne(char *argv[])
{
	MultiTrack mt;
	FileNameList fnl;
	fnl.FromArgv( argv+1 );
	
	while(char* fName = fnl.Get())
	{
		if(mt.add(fName))
			contError(NULL, "Error Loading File \"%s\"", fName);
	}
	
	return mt.build();
}
