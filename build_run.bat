@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Trip Management System - Build and Run
echo ========================================

set MINGW_PATH=D:\Apps\Qt\Tools\mingw1310_64\bin
set PATH=%MINGW_PATH%;%PATH%

:: Check if Qt is installed
echo Checking Qt installation...
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Qt/qmake not found in PATH!
    echo Please install Qt or run install_qt.bat first.
    echo.
    pause
    exit /b 1
)

echo Qt found! Building and running project...
echo.

:: Navigate to project directory
cd /d simpleQtApp

:: Clean previous build
echo Cleaning previous build...
if exist Makefile (
    if exist mingw32-make.exe (
        mingw32-make.exe clean >nul 2>&1
    ) else (
        make clean >nul 2>&1
    )
)

:: Remove old files
if exist "release\simpleQtApp.exe" del "release\simpleQtApp.exe" >nul 2>&1
if exist "debug\simpleQtApp.exe" del "debug\simpleQtApp.exe" >nul 2>&1

:: Generate Makefile
echo Generating Makefile with qmake...
qmake
if %errorlevel% neq 0 (
    echo ERROR: qmake failed!
    cd ..
    pause
    exit /b 1
)

:: Build the project
echo Building application...

:: Try to find the correct make command
where mingw32-make >nul 2>&1
if %errorlevel% equ 0 (
    set MAKE_CMD=mingw32-make
    echo Using mingw32-make...
) else (
    where make >nul 2>&1
    if %errorlevel% equ 0 (
        set MAKE_CMD=make
        echo Using make...
    ) else (
        echo ERROR: Neither mingw32-make nor make found in PATH!
        echo Please ensure MinGW is properly installed and in PATH.
        cd ..
        pause
        exit /b 1
    )
)

%MAKE_CMD% -f Makefile.Release
if %errorlevel% neq 0 (
    echo ERROR: Build failed!
    echo Check the error messages above.
    cd ..
    pause
    exit /b 1
)

:: Check if executable was created
if not exist "release\simpleQtApp.exe" (
    echo ERROR: Executable not found after build!
    echo Build may have failed silently.
    cd ..
    pause
    exit /b 1
)

:: Deploy Qt dependencies
echo Deploying Qt dependencies...
cd release
windeployqt simpleQtApp.exe >nul 2>&1
if %errorlevel% neq 0 (
    echo WARNING: windeployqt failed, trying manual deployment...
    :: Manual deployment fallback
    if not exist "platforms" mkdir platforms
    if exist "%QTDIR%\plugins\platforms\qwindows.dll" (
        copy "%QTDIR%\plugins\platforms\qwindows.dll" platforms\ >nul 2>&1
    )
)
cd ..

echo.
echo ========================================
echo Build successful! Starting application...
echo ========================================
echo.

:: Run the application
start "" "release\simpleQtApp.exe"

:: Wait a moment to see if the app starts successfully
timeout /t 2 >nul

:: Check if process is running
tasklist /fi "imagename eq simpleQtApp.exe" 2>nul | find /i "simpleQtApp.exe" >nul
if %errorlevel% equ 0 (
    echo Application started successfully!
    echo You can now test your changes.
    echo.
    echo Press any key to close this window...
    pause >nul
) else (
    echo WARNING: Application may not have started properly.
    echo Check if there are any error messages.
    echo.
    pause
)

:: Return to project root
cd ..

exit /b 0