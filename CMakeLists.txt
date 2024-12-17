cmake_minimum_required(VERSION 3.28)
project(Chess_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_subdirectory(ThirdPartyLibraries)
add_subdirectory(SozinEngine)
add_subdirectory(Chess)


