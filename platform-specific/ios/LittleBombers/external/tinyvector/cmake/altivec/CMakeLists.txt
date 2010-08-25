cmake_minimum_required(VERSION 2.4)
project(TV_ALTIVEC CXX C)
add_definitions(-maltivec -mabi=altivec)
add_executable(altivec altivec.cpp)