cmake_minimum_required(VERSION 2.6)

include_directories(~/gmock-1.6.0/include ~/gmock-1.6.0/gtest/include)
link_directories(~/gmock-1.6.0/mybuild ~/gmock-1.6.0/gtest/mybuild)
add_definitions(-std=c++0x)

set(sources 
   main.cpp 
   LineReaderTest.cpp LineReader.h)
add_executable(test ${sources})
target_link_libraries(test pthread)
target_link_libraries(test gmock)
target_link_libraries(test gtest)
target_link_libraries(test rt)

