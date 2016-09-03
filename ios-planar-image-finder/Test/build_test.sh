#!/bin/bash
#g++ -DTEST_APP -O3 -ffast-math -ftree-vectorize -funroll-loops -ftree-loop-ivcanon -fgcse-sm -fgcse-las -funsafe-loop-optimizations -fsee -fomit-frame-pointer -o test test.cpp ../Classes/ImageFinder.cpp $(find ../nn -iname "*.cpp") -I../Classes -I.. -I../nn -I/opt/local/include $(pkg-config --cflags --libs opencv)
g++ -DTEST_APP -O0 -o test test.cpp ../Classes/ImageFinder.cpp $(find ../nn -iname "*.cpp") -I../Classes -I.. -I../nn -I/opt/local/include $(pkg-config --cflags --libs opencv)

