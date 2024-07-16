REM Must be used from the root of the project.
mkdir build
cd build || exit
cmake ..
MSBuild Ayla.sln /p:Configuration=Default  REM This is not working sooo
exit