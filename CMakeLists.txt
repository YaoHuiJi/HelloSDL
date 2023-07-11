cmake_minimum_required(VERSION 3.0.0)
project(HelloSDL VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_subdirectory(src/common)

aux_source_directory(src DIR_SRCS)
add_executable(HelloSDL ${DIR_SRCS} )


target_link_libraries(HelloSDL PRIVATE Common)

target_link_libraries(HelloSDL PRIVATE SDL2::SDL2)
target_link_libraries(HelloSDL PRIVATE SDL2_image::SDL2_image)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#message("源资源目录：" ${CMAKE_CURRENT_SOURCE_DIR}/resources)
#message("可执行文件资源目录：" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
