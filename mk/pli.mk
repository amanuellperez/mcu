# Copyright (C) 2022-2024 Manuel Perez 
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


# Reglas para compilar la librería pli

include $(MCU_COMRULES)

LIBSUFFIX=$(MCU)
MICROSUFFIX=$(MCU)_$(F_CPU)

# Variables genéricas de compilación del proyecto
PROJ_CXXFLAGS=

# Librerías con las que enlazar 
# Recordar el orden: 
# 	poner las librerías que dependen de otras antes.
# 	Ejemplo: A depende de B. Luego escribir '-lA -lB' y no al revés
# 	ya que no compilaría.
ifdef BIN
PROJ_LDFLAGS=-L$(MCU_LIB) \
			 -llogic_$(LIBSUFFIX) -lhwd_$(LIBSUFFIX)\
			 -lmcu_$(MICROSUFFIX) -lmicro_$(MICROSUFFIX) \
			 -latd_$(MCU) -lstd_$(MCU)
else
PROJ_LDFLAGS=-L$(MCU_LIB) \
			 -llogic_$(LIBSUFFIX) -lhwd_$(LIBSUFFIX)\
			 -latd_$(MCU) -lstd_$(MCU)
endif

include $(MCU_GENRULES)


