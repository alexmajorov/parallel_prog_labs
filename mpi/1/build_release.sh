#!/bin/bash

mpic++ -Wall -Werror -Wextra -Wpedantic -Wno-vla -O2 -c main.c
mpic++ -o app.exe main.o 