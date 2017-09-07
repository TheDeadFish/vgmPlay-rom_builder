@setlocal
@call egcc.bat
@echo off
cd ..\obj
gcc ..\lib\stdafx.cc %CCFLAGS2% -c -g
gcc -x c++-header ..\lib\stdafx.h %CCFLAGS2% -g -o stdafx.h.gch 
