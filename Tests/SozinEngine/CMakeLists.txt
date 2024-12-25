

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        ExampleTest.cpp
        Core/Events/EventSystemTest.cpp
)


add_executable(SozinEngineTests
        ${TEST_FILES}
)

target_link_libraries(SozinEngineTests GTest::gtest_main)
target_link_libraries(SozinEngineTests SozinEngine)

target_include_directories(SozinEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(SozinEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/Source)

include(GoogleTest)
gtest_discover_tests(SozinEngineTests)