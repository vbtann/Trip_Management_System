@echo off
echo ========================================
echo Trip Management System - Clean Build
echo ========================================
echo.

cd /d "%~dp0\simpleQtApp"

echo Cleaning all build files...
if exist Makefile del Makefile
if exist Makefile.Debug del Makefile.Debug
if exist Makefile.Release del Makefile.Release
if exist .qmake.stash del .qmake.stash
if exist object_script.simpleQtApp.Debug del object_script.simpleQtApp.Debug
if exist object_script.simpleQtApp.Release del object_script.simpleQtApp.Release

if exist debug (
    echo Removing debug directory...
    rmdir /s /q debug
)

if exist release (
    echo Removing release directory...
    rmdir /s /q release
)

echo Clean completed!
pause
