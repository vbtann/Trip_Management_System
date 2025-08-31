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
cd /d TripManagementSystem

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
if exist "release\TripManagementSystem.exe" del "release\TripManagementSystem.exe" >nul 2>&1
if exist "debug\TripManagementSystem.exe" del "debug\TripManagementSystem.exe" >nul 2>&1

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
if not exist "release\TripManagementSystem.exe" (
    echo ERROR: Executable not found after build!
    echo Build may have failed silently.
    cd ..
    pause
    exit /b 1
)

:: Deploy Qt dependencies for standalone execution
echo Deploying Qt dependencies for standalone execution...
cd release

:: Try windeployqt first
echo Running windeployqt...
windeployqt TripManagementSystem.exe --compiler-runtime --force
if %errorlevel% neq 0 (
    echo windeployqt failed, performing complete manual deployment...
    
    :: Get Qt installation paths using qmake
    for /f "tokens=*" %%i in ('qmake -query QT_INSTALL_BINS') do set QT_BIN_DIR=%%i
    for /f "tokens=*" %%i in ('qmake -query QT_INSTALL_PLUGINS') do set QT_PLUGINS_DIR=%%i
    
    echo Qt bin directory: %QT_BIN_DIR%
    
    :: Create necessary directories
    if not exist "platforms" mkdir platforms
    if not exist "imageformats" mkdir imageformats
    if not exist "styles" mkdir styles
    
    :: Copy Qt DLLs (essential for Qt applications)
    echo Copying Qt runtime DLLs...
    if exist "%QT_BIN_DIR%\Qt6Core.dll" (
        copy "%QT_BIN_DIR%\Qt6Core.dll" . >nul 2>&1
        copy "%QT_BIN_DIR%\Qt6Gui.dll" . >nul 2>&1
        copy "%QT_BIN_DIR%\Qt6Widgets.dll" . >nul 2>&1
    ) else if exist "%QT_BIN_DIR%\Qt5Core.dll" (
        copy "%QT_BIN_DIR%\Qt5Core.dll" . >nul 2>&1
        copy "%QT_BIN_DIR%\Qt5Gui.dll" . >nul 2>&1
        copy "%QT_BIN_DIR%\Qt5Widgets.dll" . >nul 2>&1
    ) else (
        echo ERROR: Qt DLLs not found in %QT_BIN_DIR%
        cd ..
        cd ..
        pause
        exit /b 1
    )
    
    :: Copy MinGW runtime DLLs (critical for MinGW-compiled applications)
    echo Copying MinGW runtime DLLs...
    copy "%MINGW_PATH%\libgcc_s_seh-1.dll" . >nul 2>&1
    copy "%MINGW_PATH%\libstdc++-6.dll" . >nul 2>&1
    copy "%MINGW_PATH%\libwinpthread-1.dll" . >nul 2>&1
    
    :: Copy platform plugins (absolutely essential for Qt GUI applications)
    echo Copying platform plugins...
    if exist "%QT_PLUGINS_DIR%\platforms\qwindows.dll" (
        copy "%QT_PLUGINS_DIR%\platforms\qwindows.dll" platforms\ >nul 2>&1
    ) else (
        echo ERROR: qwindows.dll not found in %QT_PLUGINS_DIR%\platforms\
        cd ..
        cd ..
        pause
        exit /b 1
    )
    
    :: Copy additional plugins for better compatibility
    if exist "%QT_PLUGINS_DIR%\imageformats\qico.dll" (
        copy "%QT_PLUGINS_DIR%\imageformats\qico.dll" imageformats\ >nul 2>&1
    )
    if exist "%QT_PLUGINS_DIR%\styles\qwindowsvistastyle.dll" (
        copy "%QT_PLUGINS_DIR%\styles\qwindowsvistastyle.dll" styles\ >nul 2>&1
    )
    
    echo Manual deployment completed.
) else (
    echo windeployqt completed successfully!
)

:: Copy cache folder if it exists
if exist "..\cache" (
    echo Copying cache folder...
    xcopy "..\cache" "cache\" /E /I /Q /Y >nul 2>&1
    if %errorlevel% equ 0 (
        echo Cache folder copied successfully.
    ) else (
        echo Warning: Failed to copy cache folder.
    )
) else (
    echo Cache folder not found, skipping...
)

:: Verify critical dependencies for standalone execution
echo Verifying standalone deployment...
set DEPLOYMENT_FAILED=0

echo Checking for executable...
if not exist "TripManagementSystem.exe" (
    echo ERROR: TripManagementSystem.exe missing!
    set DEPLOYMENT_FAILED=1
)

echo Checking for Qt Core DLL...
if not exist "Qt6Core.dll" if not exist "Qt5Core.dll" (
    echo ERROR: Qt Core DLL missing!
    set DEPLOYMENT_FAILED=1
)

echo Checking for platform plugin...
if not exist "platforms\qwindows.dll" (
    echo ERROR: qwindows.dll missing from platforms directory!
    set DEPLOYMENT_FAILED=1
)

echo Checking for MinGW runtime...
if not exist "libgcc_s_seh-1.dll" (
    echo ERROR: libgcc_s_seh-1.dll missing!
    set DEPLOYMENT_FAILED=1
)

if not exist "libstdc++-6.dll" (
    echo ERROR: libstdc++-6.dll missing!
    set DEPLOYMENT_FAILED=1
)

if %DEPLOYMENT_FAILED% equ 1 (
    echo.
    echo DEPLOYMENT FAILED - Missing critical dependencies!
    echo The executable will not run independently.
    echo.
    echo Current release directory contents:
    dir /b *.dll *.exe
    echo.
    echo Platforms directory:
    if exist platforms (dir platforms /b) else (echo Missing!)
    echo.
    cd ..
    cd ..
    pause
    exit /b 1
)

echo Deployment verification passed - executable should run independently!

cd ..

echo.
echo ========================================
echo Build and deployment successful!
echo ========================================
echo.

:: Test standalone execution
echo Testing standalone execution...
start "" "release\TripManagementSystem.exe"

:: Wait a moment to see if the app starts successfully
timeout /t 3 >nul

:: Check if process is running
tasklist /fi "imagename eq TripManagementSystem.exe" 2>nul | find /i "TripManagementSystem.exe" >nul
if %errorlevel% equ 0 (
    echo SUCCESS: Application started and is running!
    echo You can now double-click the TripManagementSystem.exe file directly.
    echo.
    echo Executable location: %CD%\TripManagementSystem\release\TripManagementSystem.exe
    echo.
) else (
    echo WARNING: Application may not have started properly.
    echo Try running the executable manually to see any error messages.
    echo Location: %CD%\TripManagementSystem\release\TripManagementSystem.exe
    echo.
)

echo Press any key to close this window...
pause >nul

:: Return to project root
cd ..

exit /b 0