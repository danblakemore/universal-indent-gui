@echo off
set QTDIR=C:\QtSDK\Desktop\Qt\4.7.4\mingw
set PATH=%QTDIR%\bin;C:\QtSDK\mingw\bin;c:\cygwin\bin;%PATH%
c:\cygwin\bin\bash.exe buildRelease.sh win32 %QTDIR%