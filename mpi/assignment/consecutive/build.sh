#!/bin/bash

clang++ -Wall -Wextra -Wpedantic -Wno-vla -O2 -c main.cpp
clang++ -o app.exe main.o