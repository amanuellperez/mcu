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
#   Los ficheros mlx o musicxml contienen la partitura. 
#   Esto es un intento de extraer las notas a partir de esos ficheros.
#
# HISTORIA
#    Manuel Perez
#    26/12/2022 Escrito
#   
# TODO
#   SIN ACABAR!!!
#
#   Este script solo funciona con partituras en las que se toque una nota cada
#   vez. Si hay que tocar varias notas a la vez (y da la impresión de ser lo
#   habitual) no funciona, amén de que habría que ver cómo implementar eso en
#   hardware. 
#   Es una primera prueba y solo sirve para partituras sencillas.
#   Con todo sienta la base para futuro trabajo y poder incluir más de una
#   nota a la vez.
# ------------------------------------------------------------------------

from bs4 import BeautifulSoup
import getopt, sys
import os   # path.basename

usage = "Usage: " + sys.argv[0] + " [-h] [-i] [-p part] file.xml\n"\
        "Extract or transform information from a musicxml file.\n"\
        "\n  -h\tshow this help\n"\
        "  -i\tshow information about the song\n"\
        "  -p part\tgenerate .h to include in the project\n"

main_options = "hip:"

file_header=\
"// This file has been generated by " + os.path.basename(sys.argv[0]) + "\n"\
"// Copyright (C) 2023 Manuel Perez \n"\
"//           mail: <manuel2perez@proton.me>\n"\
"//           https://github.com/amanuellperez/mcu\n"\
"//\n"\
"// This file is part of the MCU++ Library.\n"\
"//\n"\
"// MCU++ Library is a free library: you can redistribute it and/or modify\n"\
"// it under the terms of the GNU General Public License as published by\n"\
"// the Free Software Foundation, either version 3 of the License, or\n"\
"// (at your option) any later version.\n"\
"//\n"\
"// This library is distributed in the hope that it will be useful,\n"\
"// but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
"// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"\
"// GNU General Public License for more details.\n"\
"//\n"\
"// You should have received a copy of the GNU General Public License\n"\
"// along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"\
"#pragma once\n"\
"\n#ifndef __PRJ_SONGS_H__\n"\
"#define __PRJ_SONGS_H__\n"\
"\n#include <atd_memory.h>\n"\
"#include \"mus_musician.h\"\n"

file_tail="\n\n#endif"


def open_tree(xml_name):
    with open(xml_name) as xml_doc:
        return BeautifulSoup(xml_doc, "xml")

# -------------------------
# DUDA: con tanto replace seguramente sea más efectivo hacer un bucle for
def remove_spaces(str):
    return str.replace(' ', '_').replace('(', '').replace(')', '')

# ------------
def error(err):
    print(err)
    print(usage)
    sys.exit(1)


# -------------------------
def song_name(soup):
    movement_title = soup.find("movement-title")
    if (movement_title != None):
        return remove_spaces(movement_title.string)

    work_title = soup.work.find("work-title")
    if (work_title != None):
        return remove_spaces(work_title.string)

    return "unname_song"


# ------------
def args(argv):
    try:
        opts, args = getopt.getopt(argv[1:], main_options)

    except getopt.GetoptError as err:
        error(err)

    if (len(args) != 1 or ('-h', '') in opts):
        print (usage)
        sys.exit(2)

    return args[0], opts


# -------------------------
def generate_header_part(xml_name, part_number):

    soup = open_tree(xml_name)

    print(file_header)
    print_file_body(soup, part_number)
    print(file_tail)


# -------------------------
def print_file_body(soup, part_number):
    part_name = []
    for scpart in soup.find_all("score-part"):
        pn = scpart.find("part-name")
        part_name.append(pn.string)
    #    print(scpart["id"], ":", pn.string)
    id = 0
    for part in soup.find_all("part"):
        if (part_number == "0" or part_number == part["id"]):
            notes = part.find_all("note")
            notes = [note for note in notes if note.pitch != None]
            num_notes = len(notes)

            print("\n\n// PART ", part["id"], ":", part_name[id])
            print("constexpr const atd::Progmem_array<music::Note,"
                 , num_notes
                 , "> PROGMEM\n"
                 , song_name(soup) + "_" + part["id"]
                 , " =\n{")

            i = 1
            for note in notes:
                if (note.pitch != None):
                    print( "   music::Note{", note.pitch.octave.string
                         , ", music::Step::", note.pitch.step.string
                         , ", ", note.type.string
                         , "}", end = '')

                    if (i != num_notes):
                        print(",")

                    i += 1

            print("\n};")

        id += 1     




# -------------------------
def info(xml_name):
    soup = open_tree(xml_name)

    print("\nINFO")
    work_title = soup.work.find("work-title")
    if (work_title != None):
        print("   Work title\t:", work_title.string)

    work_number = soup.work.find("work-number")
    if (work_number != None):
        print("   Work number\t:", work_number.string)

    movement_title = soup.find("movement-title")
    if (movement_title!= None):
        print("   Song title\t:", movement_title.string)

    composer = soup.find(type = "composer")
    if (composer != None):
        print("   Composer\t:", composer.string)



    # parts
    # -----
    print("\nPARTS")
    for scpart in soup.find_all("score-part"):
        pn = scpart.find("part-name")
        print("   ", scpart["id"], ":", pn.string)


# ------------
def main():
    xml_name, opts = args(sys.argv)

    for opt, arg in opts:
        if opt == '-i':
            info(xml_name)
            return

        elif opt == '-p':
            generate_header_part(xml_name, arg)

    
    generate_header_part(xml_name, "0")


# program
# -------
if __name__ == "__main__":
    main()



