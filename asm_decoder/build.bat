@echo off
cl.exe 2>nul >nul
IF %ERRORLEVEL% EQU 9009 (call setup.bat)
cl /FC /diagnostics:column /MP /nologo /Zi /Od main.c -o decode_to_asm.exe