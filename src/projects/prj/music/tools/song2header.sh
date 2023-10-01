#!/bin/bash
# Copyright (C) 2022 Manuel Perez 
#           mail: <manuel2perez@proton.me>
#           https://github.com/amanuellperez/mcu
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

# ------------------------------------------------------------------------
#
# DESCRIPCION
#   Para escribir las notas de una canción es más sencillo escribir
#   abreviaturas y luego usar este script para generar el .h correspondiente.
#
# HISTORIA
#    Manuel Perez
#    26/12/2022 Escrito
#   
# ------------------------------------------------------------------------
PATH=/usr/bin

file=$1

# Nombres en español de lo que dura una nota
duration64="redonda"	# = whole note
duration32="blanca"
duration24="negra_punto"
duration16="negra"
duration8="corchea"	# = quarter_note or crotchet
duration4="semicorchea"
duration2="fusa"
duration1="semifusa"

# Valores de las octavas
one_line="music::Octave::one_line"
two_line="music::Octave::two_line"
three_line="music::Octave::three_line"
great_octave="music::Octave::great"
small_octave="music::Octave::small"

# Nombres de las octavas
sed -e "s/#.*//g" \
    -e "/^[ \t]*$/d"\
    -e "s/^/\tmusic::Song_note{/"\
    -e "s/$/},/"\
    -e "s/Do/music::Note::Do, /"\
    -e "s/DoS/music::Note::DoS, /"\
    -e "s/Re/music::Note::Re, /"\
    -e "s/ReS/music::Note::ReS, /"\
    -e "s/Mi/music::Note::Mi, /"\
    -e "s/Fa/music::Note::Fa, /"\
    -e "s/FaS/music::Note::FaS, /"\
    -e "s/Sol/music::Note::Sol, /"\
    -e "s/SolS/music::Note::SolS, /"\
    -e "s/La/music::Note::La, /"\
    -e "s/LaS/music::Note::LaS, /"\
    -e "s/Si/music::Note::Si, /"\
    -e "s/${duration64}/64/g"\
    -e "s/${duration32}/32/g" \
    -e "s/${duration24}/24/g"\
    -e "s/${duration16}/16/g"\
    -e "s/${duration8}/8/g"\
    -e "s/${duration4}/4/g"\
    -e "s/${duration2}/2/g"\
    -e "s/${duration1}/1/g"\
    -e "s/one/${one_line}, /g"\
    -e "s/two/${two_line}, /g"\
    -e "s/three/${three_line}, /g"\
    -e "s/great/${great_octave}, /g"\
    -e "s/small/${small_octave}, /g"\
    ${file}
