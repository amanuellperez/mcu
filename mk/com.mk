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


# -------------------------------------------------
# Datos del microcontrolador por defecto que usamos
# -------------------------------------------------
MCU   ?= atmega168a

# Frecuencia a la que funciona la CPU 
# OJO: el reloj del atmega328p funciona a 8 MHz, pero su CPU a 1 MHz!!!
# No confundir estas dos cosas.
F_CPU ?= 1000000UL

# BAUD = velocidad a la que queremos transmitir.
# Esta variable la necesitamos en <util/setbaud.h> para elegir la velocidad
# de transmisión.
# TODO: esto tiene que elegirse en el main del cliente:
# 		UART::baud_rate_9600(); ó baud_rate<9600>(); ó baud_rate(9600);
# Revisar, creo que esto ya no es necesario. Lo eliminé con mi baud_rate
BAUD  ?= 9600UL
# Also try BAUD = 19200 or 38400 if you're feeling lucky.

# Compilador y versión que usamos
CXX := avr-g++
STD ?= c++23

# ----------------------
# Programador que usamos
# ----------------------
PROGRAMMER_TYPE ?= usbasp
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
