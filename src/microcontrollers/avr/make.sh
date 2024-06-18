#!/bin/bash

F_CPU=1000000UL	; export F_CPU
make clean
make dist

F_CPU=8000000UL	; export F_CPU
make clean
make dist

#F_CPU=12000000UL	; export F_CPU
#make clean
#make dist

make super_clean
