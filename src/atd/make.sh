#!/bin/bash

echo "Recordar que hay que tener cargado el entorno mcu para poder ejecutar
este script"


ATD_GENRULES=${ATD_GENRULES}
make super_clean
make dist

ATD_GENRULES=${ALP_COMPRULES}
make super_clean
make dist

make clean
