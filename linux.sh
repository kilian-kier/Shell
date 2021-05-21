#!/bin/bash

cd source
ld -r -b binary help.txt -o help.o
gcc alias.c basic_cmd.c command.c file_cmd.c functions.c help.o input.c main.c struct.c -o ../shell