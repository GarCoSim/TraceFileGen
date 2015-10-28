cmake_minimum_required(VERSION 3.2)

project (TARCEGEN)

add_subdirectory (ObjectClasses)
add_subdirectory (Utils)

add_executable (targetGen main.cpp)

target_link_libraries (targetGen LINK_PUBLIC ObjectClasses Utils)
