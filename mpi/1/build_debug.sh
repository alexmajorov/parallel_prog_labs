#!/bin/bash

mpic++ -Wall -Werror -Wextra -Wpedantic -Wno-vla -O0 -g -c main.c
mpic++ -o app.exe main.o 