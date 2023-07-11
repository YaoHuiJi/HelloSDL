find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_library (YEngine LTexture.cpp)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (YEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(YEngine PRIVATE SDL2::SDL2)
target_link_libraries(YEngine PRIVATE SDL2_image::SDL2_image)