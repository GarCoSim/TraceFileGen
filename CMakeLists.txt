cmake_minimum_required(VERSION 3.2)

project (TraceFileGen)

add_subdirectory (ObjectClasses)
add_subdirectory (Utils)

add_executable (TraceFileGen main.cpp)

target_link_libraries (TraceFileGen LINK_PUBLIC Utils ObjectClasses)
