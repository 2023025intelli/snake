cmake_minimum_required(VERSION 3.19)
project(snake C)

set(CMAKE_C_STANDARD 99)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_library(MENU_LIBRARY menu)

add_executable(${PROJECT_NAME} main.c snake.c)

target_link_libraries(snake ${CURSES_LIBRARIES} ${MENU_LIBRARY})
