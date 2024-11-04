# Copyright (C) 2017-2024 Manuel Perez
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

# ----------------------
# Variables a configurar
# ----------------------
# Where is the project?
MCU_ROOT=${MY_HOME}/prj/git/mcu		; export MCU_ROOT

# Where is avr-gcc?
PATH_AVR_BINS=${MY_HOME}/../bin/avr/bin

# -------------------------
# Estructura de directorios
# -------------------------
#MCU_ROOT=${MY_HOME}/prj/git/mcu		; export MCU_ROOT
MCU_SRC=${MCU_ROOT}/src			; export MCU_SRC
MCU_MK=${MCU_ROOT}/mk			; export MCU_MK
MCU_TOOLS=${MCU_ROOT}/tools		; export MCU_TOOLS


# donde distribuimos los archivos
# -------------------------------
# de cabecera
MCU_INCLUDE=${MY_INCLUDE}/mcu		; export MCU_INCLUDE

# las librerías
MCU_LIB=${MY_LIB}/avrgcc		; export MCU_LIB

# los binarios
MCU_BIN=${MY_BIN}			; export MCU_BIN


# ---------------------
# Reglas de compilación
# ---------------------
# Reglas genéricas para compilar usando avr-g++
MCU_GENRULES=${MCU_MK}/mcu_genrules.mk  ; export MCU_GENRULES
MCU_RECRULES=${MCU_MK}/mcu_recrules.mk  ; export MCU_RECRULES
MCU_FUSES=${MCU_MK}/mcu_fuses.mk	; export MCU_FUSES
MCU_COMRULES=${MCU_MK}/com.mk		; export MCU_COMRULES

# Reglas particulares de cada entorno
STD_GENRULES=${MCU_MK}/std.mk	; export STD_GENRULES
ATD_GENRULES=${MCU_MK}/atd.mk	; export ATD_GENRULES
ROM_GENRULES=${MCU_MK}/rom.mk	; export ROM_GENRULES
AVR_GENRULES=${MCU_MK}/avr.mk	; export AVR_GENRULES
MCU_MCUGENRULES=${MCU_MK}/mcu.mk	; export MCU_MCUGENRULES

HWD_GENRULES=${MCU_MK}/hwd.mk	; export HWD_GENRULES
LOGIC_GENRULES=${MCU_MK}/logic.mk; export LOGIC_GENRULES

DEBUG_GENRULES=${MCU_MK}/debug.mk	; export DEBUG_GENRULES
PLI_GENRULES=${MCU_MK}/pli.mk	; export PLI_GENRULES
PRJ_GENRULES=${MCU_MK}/prj.mk	; export PRJ_GENRULES

# ------------------------------
# Variables usadas para compilar
# ------------------------------
# CPATH = es donde busca g++ los archivos .h (opción -I)
CPATH=${MCU_INCLUDE}
CPATH=${CPATH}:${MCU_INCLUDE}/atd
#CPATH=${CPATH}:${MCU_INCLUDE}/avr
#CPATH=${CPATH}:${MCU_INCLUDE}/dev
#CPATH=${CPATH}:${MCU_INCLUDE}/std <-- este lo añadimos en mcu_genrules!!!
CPATH=${CPATH}:${MCU_INCLUDE}/std  # ¿por qué estaba comentado?
export CPATH


# LIBRARY_PATH = donde busca g++ las librerias (opción -L)
LIBRARY_PATH=${MCU_LIB}	; export LIBRARY_PATH 

# ----
# PATH
# ----
# path del avr-g++
PATH=${PATH}:${PATH_AVR_BINS}
PATH=${PATH}:${MCU_TOOLS}
export PATH

# -------
# Targets
# -------


# LIST_F_CPU
# ----------
# Listado con las frecuencias F_CPU que compilará el script make.sh
LIST_F_CPU="1000000UL"
LIST_F_CPU=${LIST_F_CPU}" 8000000UL"	#  8 MHz
#LIST_F_CPU=${LIST_F_CPU}" 12000000UL"	# 12 MHz
#LIST_F_CPU=${LIST_F_CPU}" 16000000UL"	# 16 MHz
#LIST_F_CPU=${LIST_F_CPU}" 20000000UL"	# 20 MHz
export LIST_F_CPU


# MCU
# ---
# Target usado por defecto a la hora de compilar las librerías.
DEFAULT_MCU_FOR_LIBS=atmega328p
#DEFAULT_MCU_FOR_LIBS=atmega4809

export DEFAULT_MCU_FOR_LIBS


#--------------------
# Alias para compilar
# -------------------
alias mf='make flash'
alias md='make clean; make dist'
alias mc='make super_clean'

# Alias para navegar
# ------------------
alias s='cd ${MCU_SRC}'   
alias std='cd ${MCU_SRC}/std'
alias atd='cd ${MCU_SRC}/atd'
# DUDA: se podía definir únicamente avr como mega o mega0 dependiendo del
# micro con el que esté trabajando (DEFAULT_MCU_FOR_LIBS)
alias avr='cd ${MCU_SRC}/microcontrollers/avr/avr'
alias meg='cd ${MCU_SRC}/microcontrollers/avr/mega'
alias meg0='cd ${MCU_SRC}/microcontrollers/avr/mega0'
alias mcu='cd ${MCU_SRC}/microcontrollers/mcu'
alias hwd='cd ${MCU_SRC}/devices/hwd'
alias logic='cd ${MCU_SRC}/devices/logic'
alias pli='cd ${MCU_SRC}/projects/pli'
alias prj='cd ${MCU_SRC}/projects/prj'
alias rom='cd ${MCU_SRC}/rom'


cd ${MCU_SRC}
