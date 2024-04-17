#!/bin/bash

g++ -Wall -Werror -Wpedantic -Wextra -O2 -c -fopenmp main.cpp
g++ -o app.exe -fopenmp main.o