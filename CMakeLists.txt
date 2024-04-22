cmake_minimum_required (VERSION 3.7)
project(c8int
        DESCRIPTION "Chip-8 e interpreter"
        LANGUAGES   C)

find_package(SDL2 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

message ([STATUS] "SDL2_INCLUDE_DIR" ${SDL2_INCLUDE_DIRS})
message ([STATUS] "SDL2_LIBRARY" ${SDL2_LIBRARIES})

add_library (chip8 src/chip8.c include/chip8.h include/disp.h)
target_include_directories(chip8 PUBLIC include ${SDL2_INCLUDE_DIRS})

add_library (disp src/disp.c)
target_include_directories(disp PUBLIC ${SDL2_INCLUDE_DIRS} include)
target_link_libraries(disp ${SDL2_LIBRARIES})

add_executable(c8int src/c8int.c)
target_link_libraries(c8int PUBLIC chip8)
target_link_libraries(c8int PUBLIC disp)
