#!/bin/bash

g++ -Wall -Werror -Wpedantic -Wextra -O2 -c main.cpp
g++ -o app.exe -pthread main.o