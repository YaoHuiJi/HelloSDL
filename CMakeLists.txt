cmake_minimum_required(VERSION 3.0.0)
project(HelloSDL VERSION 0.1.0)

# include(CTest)
# enable_testing()

#find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(HelloSDL src/main.cpp)

target_link_libraries(HelloSDL PRIVATE SDL2::SDL2)
target_link_libraries(HelloSDL PRIVATE SDL2_image::SDL2_image)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
