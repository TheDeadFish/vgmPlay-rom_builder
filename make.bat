setlocal
call egcc.bat
pushd src
windres resource.rc -O coff -o ..\obj\resource.o  
set CFLAGS=%OCCFLG% -I.. -I..\LIB
set LFLAGS=%LFLAGS% ..\obj\*.o -lcomdlg32 -lgdi32 -lpsapi 
gcc *.cc ..\lib\*.cc %CFLAGS% %LFLAGS% -o ..\bin\vgmPlay.exe -mwindows
popd
endlocal
