# -------------------------
# Estructura de directorios
# -------------------------
MCU_ROOT=${MY_HOME}/prj/git/mcu		; export MCU_ROOT
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
AVR_GENRULES=${MCU_MK}/avr.mk	; export AVR_GENRULES
MCU1_GENRULES=${MCU_MK}/mcu1.mk	; export MCU1_GENRULES

HWD_GENRULES=${MCU_MK}/hwd.mk	; export HWD_GENRULES
LOGIC_GENRULES=${MCU_MK}/logic.mk; export LOGIC_GENRULES

DEBUG_GENRULES=${MCU_MK}/debug.mk	; export DEBUG_GENRULES
APP_GENRULES=${MCU_MK}/app.mk	; export APP_GENRULES
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
PATH=${PATH}:${MY_HOME}/../bin/avr/bin
PATH=${PATH}:${MCU_TOOLS}
export PATH

# Alias 
alias mf='make flash'
alias mc='make super_clean'

# Alias para navegar
# ------------------
alias s='cd ${MCU_SRC}'   
alias std='cd ${MCU_SRC}/std'
alias atd='cd ${MCU_SRC}/atd'
alias avr='cd ${MCU_SRC}/microcontrollers/avr'
alias mcu='cd ${MCU_SRC}/microcontrollers/mcu'
alias hwd='cd ${MCU_SRC}/devices/hwd'
alias logic='cd ${MCU_SRC}/devices/logic'
alias pli='cd ${MCU_SRC}/projects/pli'
alias prj='cd ${MCU_SRC}/projects/prj'


cd ${MCU_SRC}
