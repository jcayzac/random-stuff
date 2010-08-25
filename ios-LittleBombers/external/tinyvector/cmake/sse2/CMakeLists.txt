cmake_minimum_required(VERSION 2.4)
project(TV_SSE2 CXX C)
add_definitions(-msse2)
add_executable(sse2 sse2.cpp)