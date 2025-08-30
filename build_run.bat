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

:: Enhanced deployment section
echo Deploying Qt dependencies...
cd release

echo Running windeployqt with verbose output...
windeployqt simpleQtApp.exe --debug --compiler-runtime --force --verbose 2
if %errorlevel% neq 0 (
    echo windeployqt failed with error %errorlevel%, performing manual deployment...
    
    :: Get Qt installation paths
    for /f "tokens=*" %%i in ('qmake -query QT_INSTALL_BINS') do set QT_BIN_DIR=%%i
    for /f "tokens=*" %%i in ('qmake -query QT_INSTALL_PLUGINS') do set QT_PLUGINS_DIR=%%i
    
    echo Qt bin directory: %QT_BIN_DIR%
    echo Qt plugins directory: %QT_PLUGINS_DIR%
    
    :: Ensure directories exist
    if not exist "platforms" mkdir platforms
    
    :: Copy essential Qt DLLs
    echo Copying Qt DLLs...
    if exist "%QT_BIN_DIR%\Qt6Core.dll" (
        copy "%QT_BIN_DIR%\Qt6Core.dll" . >nul 2>&1
        copy "%QT_BIN_DIR%\Qt6Gui.dll" . >nul 2>&1  
        copy "%QT_BIN_DIR%\Qt6Widgets.dll" . >nul 2>&1
    )
    
    :: Copy MinGW runtime DLLs (critical for standalone execution)
    echo Copying MinGW runtime DLLs...
    copy "%MINGW_PATH%\libgcc_s_seh-1.dll" . >nul 2>&1
    copy "%MINGW_PATH%\libstdc++-6.dll" . >nul 2>&1
    copy "%MINGW_PATH%\libwinpthread-1.dll" . >nul 2>&1
    
    :: Copy platform plugin (absolutely essential)
    echo Copying platform plugins...
    copy "%QT_PLUGINS_DIR%\platforms\qwindows.dll" platforms\ >nul 2>&1
    
    echo Manual deployment completed.
) else (
    echo windeployqt completed successfully!
)

:: Verify deployment
echo Verifying deployment...
set DEPLOYMENT_OK=1

if not exist "platforms\qwindows.dll" (
    echo ERROR: qwindows.dll missing from platforms directory!
    set DEPLOYMENT_OK=0
)

if not exist "Qt6Core.dll" (
    echo ERROR: Qt6Core.dll missing!
    set DEPLOYMENT_OK=0
)

if not exist "libgcc_s_seh-1.dll" (
    echo ERROR: libgcc_s_seh-1.dll missing!
    set DEPLOYMENT_OK=0
)

if %DEPLOYMENT_OK% equ 0 (
    echo.
    echo CRITICAL: Deployment incomplete!
    echo The application will not run independently.
    echo Current directory contents:
    dir /b *.dll *.exe
    echo.
    echo Platforms directory:
    if exist platforms (dir platforms /b) else (echo Directory missing!)
    cd ..
    cd ..
    pause
    exit /b 1
)

echo Deployment verification passed!

:: Copy cache directory if it exists (might be needed by your app)
if exist "..\cache" (
    echo Copying cache directory...
    if not exist "cache" mkdir cache
    xcopy "..\cache\*" "cache\" /E /Y >nul 2>&1
)

cd ..

echo.
echo ========================================
echo Build successful! Starting application...
echo ========================================
echo.

:: Test standalone execution first
echo Testing standalone execution...
cd release
echo Current working directory: %CD%
echo Running executable directly...
simpleQtApp.exe
if %errorlevel% neq 0 (
    echo ERROR: Application failed to start independently!
    echo Exit code: %errorlevel%
)
cd ..

:: Return to project root
cd ..

exit /b 0