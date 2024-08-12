#!/bin/bash
#
# Copyright (C) 2024 Manuel Perez <manuel2perez@proton.me>
# 
# This file is part of the MCU++ Library.
# 
# MCU++ Library is a free library: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
#****************************************************************************
#
#  DESCRIPCION
#   Para calcular los bytes asociados a un tipo de fuente:
#   1) Dibujar la fuente. La dibujamos en formato texto, con puntos y X.
#   2) Convertimos ese fichero con la fuente en los bytes correspondientes.
#
#   Este script se encarga de esa conversión.
#
#  HISTORIA
#    Manuel Perez
#    12/08/2024 Escrito
#
#****************************************************************************/
# TODO: elegir nombre
#
file=font_basic_5x8.txt

tmp_in=/tmp/fontdata1$$
tmp_line=/tmp/fontdata2$$
tmp3=/tmp/fontdata3$$

# Cada caracter primero lo coloco por lineas ...
#   + Borramos lineas vacias
#   + Borramos comentarios : empiezan con #
#   + Cambiamos . y X por 0 y 1
#   + Escribimos cada caracter en una linea
sed -e '/^[ \t]*$/d' \
    -e '/^#/d'	    \
    -e 's/\./0/g'   \
    -e 's/X/1/g' ${file} |\
awk '/^CHAR/ {printf "\n%s CHAR|", $0} \
     !/^CHAR/ {printf "%s|", $0} \
     END{print}'|\
sed '/^$/d' > ${tmp_in}

# ... para luego procesarlo por separado
while read line
do
    # guardo la linea en fichero para procesar
    echo ${line} > ${tmp_line}
    sed 's/|/\n/g' ${tmp_line} > ${tmp3}

    char_name=`head -n1 $tmp3`

    sed '1d' ${tmp3} |\
    transpose |\
    sed -e 's/ //g' \
	-e 's/^/0b/'|\
    tr '\n' ','

    # imprimimos el nombre de la letra
    echo "// ${char_name}" |\
    sed -e 's/CHAR://' \
	-e 's/CHAR//g'

done < ${tmp_in}

rm -f $tmp_in $tmp_line $tmp3

