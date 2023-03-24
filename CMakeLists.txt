cmake_minimum_required(VERSION 3.0.0)
project(HelloSDL VERSION 0.1.0)

#include(CTest)
#enable_testing()

find_package(SDL2 REQUIRED COMPONENTS SDL2)

add_executable(HelloSDL src/main.cpp)

target_link_libraries(HelloSDL PRIVATE SDL2::SDL2)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
