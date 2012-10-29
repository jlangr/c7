project(chapter7)
cmake_minimum_required(VERSION 2.6)

add_subdirectory(library)
add_subdirectory(libraryTest)

install_programs(. FILES build/libraryTest/test)

