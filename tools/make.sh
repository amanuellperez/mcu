#!/bin/bash

for f_cpu in ${LIST_F_CPU}
do
    F_CPU=${f_cpu}; export F_CPU
    make clean
    make dist
done

# en la practica siempre acabo así
make super_clean
