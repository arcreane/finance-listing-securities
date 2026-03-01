@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
"C:\Qt\6.10.0\msvc2022_64\bin\qmake.exe" networkuser.pro CONFIG+=release
nmake /f Makefile.Release
echo Done.
