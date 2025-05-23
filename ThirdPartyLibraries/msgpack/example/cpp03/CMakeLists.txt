FIND_PACKAGE (Threads REQUIRED)

FIND_PACKAGE (Boost COMPONENTS timer)

LIST (APPEND exec_PROGRAMS
        class_intrusive.cpp
        class_intrusive_map.cpp
        class_non_intrusive.cpp
        custom.cpp
        enum.cpp
        map_based_versionup.cpp
        protocol_new.cpp
        reuse_zone.cpp
        simple.cpp
)

IF (MSGPACK_DEFAULT_API_VERSION EQUAL 1)
    LIST (APPEND exec_PROGRAMS
            protocol.cpp
    )
ENDIF ()

IF (NOT MSVC)
    LIST (APPEND with_pthread_PROGRAMS
            stream.cpp
    )
ENDIF ()


IF (MSGPACK_USE_BOOST)
    IF (Boost_TIMER_LIBRARY)
        LIST (APPEND with_boost_lib_PROGRAMS
                speed_test.cpp
                speed_test_nested_array.cpp
        )
    ENDIF ()
ENDIF ()

FOREACH (source_file ${exec_PROGRAMS})
    GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
    ADD_EXECUTABLE (
        ${source_file_we}
        ${source_file}
    )
    TARGET_LINK_LIBRARIES (${source_file_we} PRIVATE
        msgpack-cxx
    )
    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wall -Wextra")
    ENDIF ()
    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        IF (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            STRING(REGEX REPLACE "/W[0-4]" "/W3 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        ELSE ()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
        ENDIF ()
    ENDIF ()
ENDFOREACH ()

FOREACH (source_file ${with_pthread_PROGRAMS})
    GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
    ADD_EXECUTABLE (
        ${source_file_we}
        ${source_file}
    )
    TARGET_LINK_LIBRARIES (${source_file_we} PRIVATE
        msgpack-cxx
        Threads::Threads
    )
    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wall -Wextra")
    ENDIF ()
    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        IF (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            STRING(REGEX REPLACE "/W[0-4]" "/W3 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        ELSE ()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
        ENDIF ()
    ENDIF ()
ENDFOREACH ()

FOREACH (source_file ${with_boost_lib_PROGRAMS})
    GET_FILENAME_COMPONENT (source_file_we ${source_file} NAME_WE)
    ADD_EXECUTABLE (
        ${source_file_we}
        ${source_file}
    )
    TARGET_LINK_LIBRARIES (${source_file_we} PRIVATE
        msgpack-cxx
        Boost::timer
    )
    IF (NOT MSVC AND NOT APPLE)
        TARGET_LINK_LIBRARIES (${source_file_we} PRIVATE
            rt
        )
    ENDIF ()

    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Wextra -Werror -O3")
    ENDIF ()

    IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        SET_PROPERTY (TARGET ${source_file_we} APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-mismatched-tags")
    ENDIF ()

    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        IF (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            STRING(REGEX REPLACE "/W[0-4]" "/W3 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        ELSE ()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
        ENDIF ()
    ENDIF ()
ENDFOREACH ()
