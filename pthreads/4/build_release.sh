#!/bin/bash

gcc -Wall -Werror -Wpedantic -Wextra -Wno-unused-parameter -O2 -c main.c
gcc -o app.exe -pthread main.o -lm 