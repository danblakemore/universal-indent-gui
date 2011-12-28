@echo off
set callpath=%0
::echo %0
set exeBasePath=%callpath:~0,-13%
::echo %exeBasePath%
copy "%exeBasePath%\vbsbeaut_keywords.txt" .\keywords.txt > nul
copy "%exeBasePath%\vbsbeaut_keywords_indent.txt" .\keywords_indent.txt > nul
"%exeBasePath%\vbsbeaut.exe" %1 %2 %3 %4 %5 %6 %7