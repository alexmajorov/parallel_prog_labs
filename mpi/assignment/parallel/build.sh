#!/bin/bash

mpic++ -Wall -Wextra -Wpedantic -Wno-vla -Wno-maybe-uninitialized -O2 -c main.cpp
mpic++ -o app.exe main.o