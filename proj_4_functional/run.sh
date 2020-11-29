#!/bin/bash

set -e

g++ -std=c++17 -I./src test/test.cpp -o proj_4_functional
./proj_4_functional

echo All tests passed!