#!/bin/bash

nasm	-g -f elf register.s
nasm	-g -f elf instruction.s
nasm	-g -f elf main.s
gcc		main.o -o test -m32 -g

nasm	-g -f elf debug.s
gcc		debug.o -o debug -m32 -g