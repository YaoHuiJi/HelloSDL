find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library (YEngine LTexture.cpp)

#target_include_directories (YEngine PUBLIC .)

target_link_libraries(YEngine PRIVATE SDL2::SDL2)
target_link_libraries(YEngine PRIVATE SDL2_image::SDL2_image)
target_link_libraries(YEngine PRIVATE SDL2_mixer::SDL2_mixer)
target_link_libraries(YEngine PRIVATE SDL2_ttf::SDL2_ttf)