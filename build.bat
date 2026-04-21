@echo off
cd /d "%~dp0"
echo Compiling cat_clicker.c...
C:\msys64\ucrt64\bin\gcc.exe cat_clicker.c -o cat_clicker.exe -IC:\msys64\ucrt64\include\SDL2 -LC:\msys64\ucrt64\lib -lSDL2 -lSDL2_mixer -lm -Wl,-subsystem,console
if %ERRORLEVEL% EQU 0 (
    echo Build successful! Running the game...
    cat_clicker.exe
) else (
    echo Build failed!
)
pause
