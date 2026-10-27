# Empty target to set up header include path

add_library(NlohmannJson INTERFACE

)


target_include_directories(NlohmannJson INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")