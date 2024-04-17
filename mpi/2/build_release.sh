#!/bin/bash

mpic++ -Wall -Werror -Wextra -Wpedantic -O2 -c main.c
mpic++ -o app.exe main.o 