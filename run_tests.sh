#!/bin/bash
# cmake -S . -B build
cmake --build build
cd build/test && ctest --output-on-failure && cd ../../