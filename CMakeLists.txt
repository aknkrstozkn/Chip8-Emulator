cmake_minimum_required(VERSION 3.25)
project(Chip8-Emulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chip8-Emulator main.cpp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Chip8-Emulator
        PRIVATE
        SDL2
)

target_compile_definitions(Chip8-Emulator PRIVATE SDL_MAIN_HANDLED)