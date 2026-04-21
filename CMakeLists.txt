cmake_minimum_required(VERSION 3.10)
project(CatClicker C)

# C Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# SDL2
find_package(SDL2 REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})

# Main executable
add_executable(cat_clicker cat_clicker.c)

# Link libraries
target_link_libraries(cat_clicker ${SDL2_LIBRARIES} m)

# Compiler flags
if(MSVC)
    target_compile_options(cat_clicker PRIVATE /W4 /WX)
else()
    target_compile_options(cat_clicker PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

# Install
install(TARGETS cat_clicker DESTINATION bin)

# Print info
message(STATUS "Cat Clicker Build Configuration:")
message(STATUS "  SDL2_FOUND: ${SDL2_FOUND}")
message(STATUS "  SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message(STATUS "  C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "  BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
