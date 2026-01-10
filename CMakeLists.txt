cmake_minimum_required(VERSION 3.16)
project(CChat C)

# 1. Configure SDL3 options before adding the directory
set(SDL_SHARED ON CACHE BOOL "Build shared library")
set(SDL_STATIC ON CACHE BOOL "Build static library")
set(SDL_TEST OFF CACHE BOOL "Disable SDL tests to save time")

# 2. Add the SDL directory - this makes the SDL3 targets available
add_subdirectory(deps/SDL)

# 3. Define your executable
add_executable(${PROJECT_NAME} src/main.c)

target_include_directories(${PROJECT_NAME} PRIVATE deps/SDL/include)

# 4. Link your project to SDL3
# Using SDL3::SDL3-shared (for .dll/.so) or SDL3::SDL3-static
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-shared)

# Automatically copy the SDL3.dll to your executable's folder
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3-shared>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()