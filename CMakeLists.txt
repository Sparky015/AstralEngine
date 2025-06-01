cmake_minimum_required(VERSION 3.28)
project(Astral-Runtime)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address")#-fsanitize=address
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()



# --- Git Submodule Update Logic ---
# Find Git executable
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # This block ensures submodules are updated when CMake is configured or built.
    # It's good practice to allow users to disable this if they manage submodules manually.
    option(UPDATE_GIT_SUBMODULES "Automatically update Git submodules during CMake configuration." ON)

    if(UPDATE_GIT_SUBMODULES)
        message(STATUS "Updating Git submodules...")
        execute_process( COMMAND ${GIT_EXECUTABLE} submodule update --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMODULE_RESULT
                OUTPUT_VARIABLE GIT_SUBMODULE_OUTPUT
                ERROR_VARIABLE GIT_SUBMODULE_ERROR
                #COMMAND_ERROR_IS_FATAL ANY
        )

        if(NOT GIT_SUBMODULE_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to update Git submodules:\n ${GIT_SUBMODULE_ERROR}")
        else()
            message(STATUS "Git submodules updated successfully.")
        endif()
    else()
        message(STATUS "Automatic Git submodule update disabled. Please update manually if needed.")
    endif()
else()
    message(WARNING "Git not found or not a Git repository. Submodules will not be updated automatically.")
endif()



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(ThirdPartyLibraries)
add_subdirectory(Astral-Runtime)
add_subdirectory(Astral-Editor)
add_subdirectory(Samples/Chess)
add_subdirectory(Samples/VulkanPlayground)

add_subdirectory(Tests)
