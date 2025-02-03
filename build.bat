@echo off
setlocal

REM Compiler and flags
set CC=cl
set CFLAGS=/EHsc /MT /std:c++17

REM Output executable
set OUT=Engine.exe

REM Source file
set SRC=Engine.cpp Loader.cpp

REM Include directories
set INCLUDES=/I"./vendor/Python/include"^
            /I"./include"^
            /I"./vendor/raylib/include"

REM Library paths
set LIBPATHS=/LIBPATH:"./vendor/Python\lib" ^
             /LIBPATH:"./vendor/raylib/lib"

REM Libraries to link
set LIBS=python312.lib ^
        raylib.lib ^
        legacy_stdio_definitions.lib ^
        msvcrt.lib ^
        vcruntime.lib ^
        ucrt.lib ^
        user32.lib ^
        gdi32.lib ^
        shell32.lib ^
        winmm.lib ^
        /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT

REM Build the project /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup
%CC% %CFLAGS% /Fe:%OUT% %SRC% %INCLUDES% /link %LIBPATHS% %LIBS% 


echo Build Successfully Done...
