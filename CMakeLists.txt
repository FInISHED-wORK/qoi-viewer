cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER ${CC})
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -pedantic")

project(qoiviewer C)

find_package(raylib 4.0.0 QUIET)
if (NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/4.0.0.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

add_executable(qoiviewer main.c qoi.h)

target_link_libraries(qoiviewer raylib)
