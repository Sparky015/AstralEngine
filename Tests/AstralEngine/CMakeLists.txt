

#Enabling the tests for CMake
enable_testing()

set(TEST_FILES
        ExampleTest.cpp
        Core/Events/EventSystemTest.cpp
        Input/KeycodeConversionsTest.cpp
        Renderer/OrthographicCamera.cpp
        Core/Containers/VectorTest.cpp
        Core/Containers/ArrayTest.cpp
)


add_executable(AstralEngineTests
        ${TEST_FILES}
)

target_link_libraries(AstralEngineTests GTest::gtest_main)
target_link_libraries(AstralEngineTests AstralEngine)

target_include_directories(AstralEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/googletest-main/googletest/include/)
target_include_directories(AstralEngineTests PUBLIC ${CMAKE_SOURCE_DIR}/Source)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(AstralEngineTests PRIVATE
            -Wall
            -fsanitize=address,undefined
    )

    target_link_options(AstralEngineTests PRIVATE
            -fsanitize=address,undefined
    )
else()
    target_compile_options(AstralEngineTests PRIVATE -Wall)
endif()

include(GoogleTest)
gtest_discover_tests(AstralEngineTests)