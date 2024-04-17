#!/bin/bash

gcc -Wall -Werror -Wpedantic -Wextra -O2 -c main.c
gcc -o app.exe -pthread main.o -lm 