REM Must be used from the root of the project.
mkdir build-visual-studio
cd build-visual-studio || exit
cmake -G "Visual Studio 17 2022" -A x64 ..
exit