# Copyright (C) 2022 A.Manuel L.Perez 
#           mail: <amanuel.lperez@gmail.com>
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
#    A.Manuel L.Perez
#    26/12/2022 Escrito
#   
# ------------------------------------------------------------------------

from bs4 import BeautifulSoup
import sys

# Args
# ----
usage = "Usage: " + sys.argv[0] + " file.xml"

if (len(sys.argv) != 2):
    print (usage)
    exit(1)

xml_name = sys.argv[1]


# main
# ----
with open(xml_name) as xml_doc:
    soup = BeautifulSoup(xml_doc, "xml")


#print(soup.work)

#print(soup.identification.creator.string)

part_name = []
for scpart in soup.find_all("score-part"):
    pn = scpart.find("part-name")
    part_name.append(pn.string)
#    print(scpart["id"], ":", pn.string)


id = 0
for part in soup.find_all("part"):
    print("PART ", part["id"], ":", part_name[id])
    id += 1     
    print("-----------------------------------------------")
    for note in part.find_all("note"):
        if (note.pitch != None):
            print( note.pitch.octave.string
                  , note.pitch.step.string
                  , note.type.string)

#        else:
#            print("NONE")
    print("\n")




