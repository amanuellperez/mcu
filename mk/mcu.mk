# Copyright (C) 2019-2020 Manuel Perez
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

# Reglas para compilar la librería hwd

include $(MCU_COMRULES)

# Variables genéricas de compilación del proyecto
PROJ_CXXFLAGS=

# Librerías con las que enlazar 
# Recordar el orden: 
# 	poner las librerías que dependen de otras antes.
# 	Ejemplo: A depende de B. Luego escribir '-lA -lB' y no al revés
# 	ya que no compilaría.
PROJ_LDFLAGS=-L$(MCU_LIB) -lavr_$(F_CPU) -latd -lstd

include $(MCU_GENRULES)


