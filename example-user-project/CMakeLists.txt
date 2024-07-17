

add_executable(ExampleUserProject
        ${CMAKE_CURRENT_SOURCE_DIR}/src/start.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ExampleLayer.cpp)

target_link_libraries(ExampleUserProject Ayla)