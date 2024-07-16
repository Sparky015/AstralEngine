mkdir build
cd build || exit
cmake ..
MSBuild Ayla.sln /p:Configuration=Default
exit