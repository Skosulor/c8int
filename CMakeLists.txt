cmake_minimum_required (VERSION 3.7)
project(chip8-emu
        DESCRIPTION "Chip-8 e emulator"
        LANGUAGES   C)

set(PROJECT_DIR /home/ohman/project/chip8)
find_package(SDL2 REQUIRED)
SET(GCC_COVERAGE_LINK_FLAGS "-lSDL2")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_library (chip8 src/chip8.c include/chip8.h include/disp.h)
target_include_directories(chip8 PUBLIC include)

add_library (disp src/disp.c)
target_include_directories(disp PUBLIC include)
target_include_directories(disp PUBLIC ${SDL2_LIBRARIES})



add_executable(emu src/emu.c)
target_link_libraries(emu PUBLIC chip8)
target_link_libraries(emu PUBLIC disp)


