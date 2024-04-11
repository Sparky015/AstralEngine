cmake_minimum_required(VERSION 3.28)
project(Ayla)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

        ### Ayla ###
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/classes)

        ### Google Test ###
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/googletest-main/)
include_directories(${CMAKE_SOURCE_DIR}/lib/googletest-main/googletest/include/)

        ### SDL2 ###
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/SDL)

include_directories(${CMAKE_SOURCE_DIR}/lib/SDL/include)
# ^^^ Fun fact I learned not while making this (for future reference), 'include_directories' tells cmake to also check a given directory
#     for header files (REMEMBER TO RELOAD CMAKE WHEN YOU MAKE A CHANGE THOUGH).

IF (APPLE)
    link_directories(${CMAKE_SOURCE_DIR}/lib/SDL/build/build/.libs)
# ^^^ Fun fact I learned while making this (for future reference), 'link_directories' tells cmake where to look for library files
#     so it can eventually link them to the executable later when you call the 'target_link_libraries' command.
endif ()
IF (WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/lib/SDL2-2.30.2/lib/x64/)
endif()


add_executable(Ayla src/main.cpp)

        ### Google Test ###
target_link_libraries(Ayla GTest::gtest_main)

        ### SDL2 ###
target_link_libraries(Ayla SDL2)
# ^^^ Fun fact I learned while making this (for future reference), the 'SDL2-2.0.0' comes from the file it finds that is always prefixed
#     with lib (libSDL2-2.0.0.dylib). That is how you know what the exact name of the library you are targeting is.





enable_testing()

add_executable(all_tests tests/hello_test.cpp)
add_executable(hello_test tests/hello_test.cpp)

target_link_libraries(all_tests GTest::gtest_main)
target_link_libraries(hello_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(all_tests)
gtest_discover_tests(hello_test)
