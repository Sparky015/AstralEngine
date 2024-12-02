REM Must be used from the root of the project.
mkdir build
pause
cd build || exit
cmake -G "MinGW Makefiles" ..
make
pause