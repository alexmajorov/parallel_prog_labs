#!/bin/bash

g++ -Wall -Werror -Wextra -Wpedantic -O0 -g -c -fopenmp main.cpp
g++ -o app.exe -fopenmp main.o