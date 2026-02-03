cmake_minimum_required(VERSION 3.20)
project(sdl_tv C)

set(CMAKE_C_STANDARD 11)

add_executable(sdl_tv
	sdl_tv.c
)

# SDL3 include + lib paths
target_include_directories(sdl_tv PRIVATE "C:/SDL3/include")
target_link_directories(sdl_tv PRIVATE "C:/SDL3/lib")
target_link_libraries(sdl_tv PRIVATE SDL3)

