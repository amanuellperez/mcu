# Copyright (C) 2019-2024 Manuel Perez
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

LIBSUFFIX=$(MCU)
MICROSUFFIX=$(MCU)_$(F_CPU)

# Variables genéricas de compilación del proyecto
PROJ_CXXFLAGS=

# TODO: hwd es una librería generica independiente del micro, luego no debe de
# enlazar con micro_$(LIBSUFFIX). Es el cliente, el que escribe el programa
# final, el que elige el micro y enlaza con micro_$(LIBSUFFIX). Borrar de aquí
# micro_$(LIBSUFFIX)
#
# Librerías con las que enlazar 
# Recordar el orden: 
# 	poner las librerías que dependen de otras antes.
# 	Ejemplo: A depende de B. Luego escribir '-lA -lB' y no al revés
# 	ya que no compilaría.

ifdef BIN
PROJ_LDFLAGS=-L$(MCU_LIB) -lmcu_$(MICROSUFFIX) -lmicro_$(MICROSUFFIX) \
			 			  -latd_$(MCU) -lstd_$(MCU)
else
PROJ_LDFLAGS=-L$(MCU_LIB) -latd_$(MCU) -lstd_$(MCU)
endif

include $(MCU_GENRULES)


