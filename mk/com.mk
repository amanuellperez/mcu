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


# Compilador y versión que usamos
CXX := avr-g++
CPP_STD ?= c++23

# ----------------------
# Programador que usamos
# ----------------------
ifeq ($(MCU), atmega4809)
	PROGRAMMER_TYPE     ?= serialupdi
	PROGRAMMER_BAUDRATE ?= 57600
else
	PROGRAMMER_TYPE     ?= usbasp
	PROGRAMMER_BAUDRATE ?= 9600
endif

PROGRAMMER_PORT ?= /dev/ttyUSB0
PROGRAMMER_ARGS ?= 


# -----------
# DIRECTORIOS
# -----------
# Directorio donde se instalan los binarios
INSTALL_BIN ?=$(MCU_BIN)

# Directorio donde se instalan las bibliotecas
INSTALL_LIB ?=$(MCU_LIB)

# Directorio donde se instalan los archivos de cabecera de las bibliotecas
#INSTALL_INC ?=$(MCU_INCLUDE)
INSTALL_INC ?=$(MCU_INCLUDE)/$(LIB)

# Directorio donde se instala mi std.
INSTALL_INC_STD = $(MCU_INCLUDE)/std


# ---
# std
# ---
# Como avr-gcc no suministra std tengo que suministrarlo yo. Para que puedan
# coexistir el std de g++ y mi std instalo mi std en INSTALL_INC_STD. Cuando
# compile con g++ no añado INSTALL_INC_STD a CPATH con lo que el compilador
# encuentra el std de g++. Pero al compilar con avr-g++ añado este directorio
# a CPATH encontrandolo en INSTALL_INC_STD.
# Se podría añadir este CPATH al entorno de mcu, pero eso genera un lío ya que
# al probar std, o hacer programas de prueba usando g++, g++ encuentra
# <iostream> en INSTALL_INC_STD en lugar de encontrar el estandar dando
# errores, ya que mi <iostream> no suministra cout (¿qué sentido tiene cout en
# un micro? No tiene por qué tener terminal de salida).
CPATH:=$(CPATH):$(INSTALL_INC_STD)


# --------------------
# Frecuencias de reloj
# --------------------
# Si no se definen explícitamente, valen 0
ifdef F_CPU
	CLK_PER = $(F_CPU)
else
	CLK_PER  ?= 0
endif 

F_CPU ?= 0
CLK_MAIN ?= 0



# -------------
# Preprocesador
# -------------
# Defino la variable IF_$(MCU) para poder compilar condicionalmente los
# programas. 
# Ejemplo: quiero probar los test de mcu con diferentes micros (atmega328p,
# atmega4809, ...) Este define permite poder compilar el programa para cada
# micro, simplemente cambiando la variable MCU.
CPPFLAGS += -DIF_$(MCU)
