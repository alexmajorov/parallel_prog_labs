#!/bin/bash

gcc -Wall -Werror -Wextra -Wpedantic -Wno-unused-parameter -O0 -g -c main.c
gcc -o app.exe -pthread main.o -lm