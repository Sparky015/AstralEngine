

add_library(GLAD STATIC glad.c)
target_include_directories(GLAD PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})