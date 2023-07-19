cmake_minimum_required(VERSION 3.0.0)
project(HelloSDL VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_subdirectory(src/YEngine)
add_subdirectory(src/1.TicTacToe)

add_executable(HelloSDL src/main.cpp)

target_link_libraries(HelloSDL PRIVATE YEngine)

target_link_libraries(HelloSDL PRIVATE SDL2::SDL2)
target_link_libraries(HelloSDL PRIVATE SDL2_image::SDL2_image)
target_link_libraries(HelloSDL PRIVATE SDL2_mixer::SDL2_mixer)
target_link_libraries(HelloSDL PRIVATE SDL2_ttf::SDL2_ttf)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#message("源资源目录：" ${CMAKE_CURRENT_SOURCE_DIR}/resources)
#message("可执行文件资源目录：" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
