@echo off
REM Must be used from the root of the project.
IF EXIST build-visual-studio (
rmdir build-visual-studio /S /Q    REM To replace existing build for iterating on the development of the build process and testing it
)
mkdir build-visual-studio
cd build-visual-studio || exit
cmake -G "Visual Studio 17 2022" -A x64 ..
pause
exit