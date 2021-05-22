ld -r -b binary source\help.txt -o source\help.o
gcc source\alias.c source\basic_cmd.c source\command.c source\file_cmd.c source\functions.c source\help.o source\input.c source\main.c source\struct.c -o Shell.exe
cls
Shell.exe