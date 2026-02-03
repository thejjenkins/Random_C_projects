cmake_minimum_required(VERSION 3.20)
project(doublependulum C)

set(CMAKE_C_STANDARD 11)

add_executable(doublependulum
    doublependulum.c
)

# SDL3 include + lib paths (your manual install)
target_include_directories(doublependulum PRIVATE "C:/SDL3/include")
target_link_directories(doublependulum PRIVATE "C:/SDL3/lib")
target_link_libraries(doublependulum PRIVATE libSDL3)
