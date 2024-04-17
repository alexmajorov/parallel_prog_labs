#!/bin/bash

g++ -std=c++17 -Wall -Werror -Wpedantic -Wextra -O2 -c -fopenmp main.cpp
g++ -std=c++17 -o app.exe -fopenmp main.o