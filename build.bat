@echo off
setlocal

REM Compiler and flags
set CC=cl
set CFLAGS=/EHsc /MD /std:c++17

REM Output executable
set OUT=Engine.exe

REM Source file
set SRC=Engine.cpp Loader.cpp

set INCLUDES=/I"C:/Program Files/Python313/include"^
            /I"./include"^
            /I"C:/Users/SomKrooz/Downloads/raylib-5.5_win64_msvc16/raylib-5.5_win64_msvc16/include"

set LIBPATHS=/LIBPATH:"C:/Program Files/Python313/libs" ^
             /LIBPATH:"C:/Users/SomKrooz/Downloads/raylib-5.5_win64_msvc16/raylib-5.5_win64_msvc16/lib"

REM Libraries to link
set LIBS=python313.lib ^
        raylib.lib ^
        legacy_stdio_definitions.lib ^
        msvcrt.lib ^
        vcruntime.lib ^
        ucrt.lib ^
        user32.lib ^
        gdi32.lib ^
        shell32.lib ^
        winmm.lib

REM Build the project
%CC% %CFLAGS% /Fe:%OUT% %SRC% %INCLUDES% /link %LIBPATHS% %LIBS%

echo Build Sucessfully Done...
