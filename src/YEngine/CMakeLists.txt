find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library (YEngine LTexture.cpp)

target_include_directories (YEngine PUBLIC include)

target_link_libraries(YEngine PRIVATE SDL2::SDL2)
target_link_libraries(YEngine PRIVATE SDL2_image::SDL2_image)