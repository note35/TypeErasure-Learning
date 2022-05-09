#!/usr/bin/env sh
c++ -O3 -Wall -shared -std=c++17 -fPIC \
    $(./py3/bin/python3 -m pybind11 --includes) \
    -Wl,-undefined,dynamic_lookup \
    min_stack.cpp -o min_stack.so
