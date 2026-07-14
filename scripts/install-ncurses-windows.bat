@echo off
setlocal enabledelayedexpansion

echo === Installing ncurses for Windows (MSYS2) ===
echo.

REM Check for MSYS2 environment
if exist "C:\msys64\usr\bin\bash.exe" (
    echo Detected: MSYS2 (MINGW64)
    set MSYS2_DIR=C:\msys64
    goto :install
)
if exist "C:\msys2\usr\bin\bash.exe" (
    echo Detected: MSYS2 (MINGW64)
    set MSYS2_DIR=C:\msys2
    goto :install
)
if exist "%MSYS2_ROOT%\usr\bin\bash.exe" (
    echo Detected: MSYS2
    goto :install
)

echo.
echo Error: MSYS2 not found.
echo Please install MSYS2 from: https://msys2.org
echo Then run this script from a MSYS2 terminal (MINGW64).
echo.
pause
exit /b 1

:install
echo.
echo Updating package databases...
"%MSYS2_DIR%\usr\bin\bash.exe" -c "pacman -Sy --noconfirm"

echo.
echo Installing ncurses...
"%MSYS2_DIR%\usr\bin\bash.exe" -c "pacman -S --noconfirm mingw-w64-x86_64-ncurses"

echo.
echo Verifying installation...
if exist "%MSYS2_DIR%\mingw64\include\ncurses.h" (
    echo [OK] ncurses.h found
) else (
    echo [WARNING] ncurses.h not found in standard paths.
    exit /b 1
)

echo.
echo === Installation complete ===
echo Compile with: g++ -o hsedit.exe main.cpp -lncurses
echo.
pause
