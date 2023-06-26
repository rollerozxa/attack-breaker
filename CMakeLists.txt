cmake_minimum_required(VERSION 3.0)
project(attack_breaker C)
set(CMAKE_C_STANDARD 99)

add_subdirectory(raylib)

include_directories(src/)

add_executable(${PROJECT_NAME}
	src/main.c
	src/snd_click.c
	src/snd_hit.c)

target_link_libraries(${PROJECT_NAME} raylib m)
