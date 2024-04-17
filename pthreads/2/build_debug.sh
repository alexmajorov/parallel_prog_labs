#!/bin/bash

g++ -Wall -Werror -Wextra -Wpedantic -O0 -g -c main.cpp
g++ -o app.exe -pthread main.o