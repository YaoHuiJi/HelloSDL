
set(GAME_NAME TicTacToe)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(${GAME_NAME} main.cpp)

target_link_libraries(${GAME_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${GAME_NAME} PRIVATE SDL2_image::SDL2_image)
target_link_libraries(${GAME_NAME} PRIVATE SDL2_mixer::SDL2_mixer)
target_link_libraries(${GAME_NAME} PRIVATE SDL2_ttf::SDL2_ttf)