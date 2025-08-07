cmake_minimum_required(VERSION 3.20)

# set the project name and the language used
project(CHIP8Emulator C)

# specify the c standard
set(CMAKE_C_STANDARD 17)

# enable compiler warning flags
add_compile_options(-Wall -Wextra -Werror)

# request cmake to search for library and if not found, terminate with error
find_package(SDL2 REQUIRED)

# specify the source files
set(SOURCES
        src/chip8.c
        src/graphics.c
        src/main.c
)

# specify the executable
add_executable(chip8 ${SOURCES})


# linking the libraries needed for SDL2
target_link_libraries(chip8 PRIVATE -lmingw32 -lSDL2main -lSDL2)