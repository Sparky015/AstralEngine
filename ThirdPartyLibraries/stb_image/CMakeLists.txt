add_library(stb_image stb_image.h stb_image.cpp stb_image_write.h stb_image_write.cpp)

target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(stb_image PRIVATE AstralEngine)