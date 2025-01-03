# Copyright (C) 2019-2024 Manuel Perez <manuel2perez@proton.me>
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

#***************************************************************************
#
#  DESCRIPCIÓN: Reglas genéricas de compilación para AVRs
#
#     Para cargar: 
#     	> make flash
#
#	  Otras opciones:
#     	> make debug	# muestra info de lo que va a hacer
#	
#		Si se quiere ver el código asm que genera, compilar con la opción
#		-save-temps
#
#  HISTORIA:
# 		Manuel Perez
# 		21/07/17: Creado a partir del del libro de MAKE.
# 		01/03/21: tag HEADERS para poder generar automáticamente los menus.
# 		17/10/24: Soporte para varios micros (atmega328p y atmega4809)
#
#***************************************************************************
include $(MCU_FUSES)

# ----------------
# PROGRAMAS USADOS
# ----------------
CC 			?= avr-gcc
CXX 		?= avr-g++
MAKE_DEPEND	:= $(CXX) -MM -MG
MAKE_MENU   := make_menu

OBJCOPY 	:= avr-objcopy
OBJDUMP 	:= avr-objdump
AVRSIZE 	:= avr-size
AVRDUDE 	:= avrdude

CTAGS		:= ctags -R

# Programas del shell
MKDIR		:= mkdir -p
CP			:= cp
RM			:= rm -rf
SED			:= sed
PRINTF		:= /usr/bin/printf

# LOGS
# ----
AVRDUDE_LOG := avrdude.log


# -------------------
# VARIABLES GENÉRICAS
# -------------------
#  Ficheros con las DEPENDENCIAS
DEPENDENCIAS = $(subst .cpp,.d,$(SOURCES))

# Objects que construimos a partir de los .cpp
OBJECTS_CPP	 =$(subst .cpp,.o,$(SOURCES))
OBJECTS_ASM	 =$(subst .S,.o,$(ASM_SOURCES))
OBJECTS		 =$(OBJECTS_ASM) $(OBJECTS_CPP)
#OBJECTS		 = $(subst .S,.o,$(subst .cpp,.o,$(SOURCES)))

# Fichero que contiene las tags
FIC_TAGS 	:= tags


# Flags para el preprocesador
CPPFLAGS += -DMCU=$(MCU) 

# F_CPU es para el atmega328p, mientras que CLK_MAIN/CLK_PER para el
# atmega4809. En realidad:
# 	F_CPU: para avrs donde fijamos la frecuencia en tiempo de compilación
# 	CLK_MAIN/CLK_PER: para cuando la fijamos dinámicamente
# Estos flags los necesita mega0_clock_frequencies.h 
CPPFLAGS += -DCLK_MAIN=$(CLK_MAIN) -DCLK_PER=$(CLK_PER) -DF_CPU=$(F_CPU)
CPPFLAGS += $(PROJ_CPPFLAGS)


# ------------------------
# FLAGS PARA EL COMPILADOR
# ------------------------
#  De acuerdo a la nota, del punto 3 de la app "Internal High-Frequency
#  Oscillator Calibration Using the Auto-Tune Feature", la macro
#  _PROTECTED_WRITE necesita mínimo -O1 para ejecutarse correctamente.
#  La opción -Os incluye la mayoría de -O2 así que debería de compilar
#  correctamente la macro.
#
# 	-Os	: optimiza el size. (o usar -O3?) <-- no borrar -Os, ver nota anterior.
# 	-std: versión del estandar a usar
# 	-Wall: mostrar todos los warnings
# 	-g	: produce información para poder depurar.
#CXXFLAGS = -Os -std=$(CPP_STD) -Wall -pedantic $(PROJ_CXXFLAGS) $(USER_CXXFLAGS)
CXXFLAGS = -Os -std=$(CPP_STD) -Wall -pedantic 

# Use short (8-bit) data types 
# -funsigned-char	: hace que el tipo char sea unsigned
# -funsigned-bitfields:	los bitfields son unsigned
# -fpack-struct		: pack all structure members together without holes.
# -fshort-enums		: define el tipo de las enum lo más pequeño posible,
#  					  suficiente para que entren todos los datos de la enum.
#CXXFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
CXXFLAGS += -funsigned-char -funsigned-bitfields -fshort-enums 

# Splits up object files per function
CXXFLAGS += -ffunction-sections -fdata-sections 

# Añadimos los flags del proyecto
CXXFLAGS += $(PROJ_CXXFLAGS) $(USER_CXXFLAGS)

# Flags para compilar asm
#ASMFLAGS =


# ------------------
# FLAGS PARA ENLAZAR
# ------------------
LDFLAGS = -Wl,-Map,$(BIN).map 

# Optional, but often ends up with smaller code
LDFLAGS += -Wl,--gc-sections 

# Relax shrinks code even more, but makes disassembly messy
# LDFLAGS += -Wl,--relax
# LDFLAGS += -Wl,-u,vfprintf -lprintf_flt -lm  # for floating-point printf
# LDFLAGS += -Wl,-u,vfprintf -lprintf_min      # for smaller printf

# Añadimos los flags del proyecto
LDFLAGS += $(PROJ_LDFLAGS) $(USER_LDFLAGS)

# Estos flags indican el micro para el que compilamos
# ¿por qué lo llama BIN_ARCH???
BIN_ARCH = -mmcu=$(MCU)



# ---------------
# Fase validación
# ---------------
ifdef BIN
	compile=1
endif

ifdef LIB
	compile=1
endif

ifdef DIST
	compile=1
endif

ifndef compile
	$(error You have define BIN, LIB or DIST variable)))
endif

#$(if $(BIN),,\
#	$(if $(LIB),,\
#		$(error No se ha definido la variable BIN ó la LIB)))



#***************************************************************************
#							TARGETS GENÉRICOS
#***************************************************************************
# La forma de compilar es:
# 	.cpp -> .o -> .elf -> .hex
#
# Lo que subimos al avr es el .hex
#
#***************************************************************************
# Elegimos condicionalmente si lo que se quiere es generar un programa
# o una librería. Cuando queramos compilar un programa, definimos BIN. Cuando
# queramos compilar una librería, definimos LIB
# ----------------------------------------
# Reglas genéricas para compilar programas
# ----------------------------------------
ifdef BIN
.PHONY: all
all: $(HEADERS) $(BIN).hex 
	@$(PRINTF) "-------------------------------------\n"
	@$(PRINTF) "Compilados: $(SOURCES) $(ASM_SOURCES)\n"
	@$(PRINTF) "MCU            = $(MCU)\n"
	@$(PRINTF) "Frequencies:\n"

ifneq ($(CLK_MAIN),0)
	@$(PRINTF) "      CLK_MAIN = $(CLK_MAIN)\n"
endif

ifneq ($(CLK_PER),$(F_CPU))
	@$(PRINTF) "      CLK_PER  = $(CLK_PER)\n"
endif

	@$(PRINTF) "      F_CPU    = $(F_CPU)\n"
	@$(PRINTF) "-------------------------------------\n\n"


# Para enlazar bibliotecas es fundamental que los LDFLAGS 
# (los -L... -l... vayan al final, después del -o $(BIN).elf)
# ¿¿¿Por qué???
$(BIN).elf: $(OBJECTS)
#	$(CXX) $(LDFLAGS) $(BIN_ARCH) $^ $(LDLIBS) -o $@ 
	$(CXX) $(BIN_ARCH) $^ $(LDLIBS) -o $@ $(LDFLAGS)

endif



# -----------------------------------------
# Reglas all y dist para compilar librarías
# -----------------------------------------
ifdef LIB

# cambio el nombre a LIB
LIB_NAME := lib$(LIB)_$(LIBSUFFIX).a

.PHONY: all
all: $(DIR_DEPENDS) $(DIR_OBJECTS) $(LIB_NAME)

.PHONY: dist
dist: $(INCS) $(LIB_NAME)
	@$(call distribuye,$(INCS),$(INSTALL_INC))
	@$(call distribuye,$(LIB_NAME),$(INSTALL_LIB))

endif	# LIB

# -----------------------------------
# Regla dist para distribuir archivos
# -----------------------------------
ifdef DIST

.PHONY: all
	@echo "Usage: make dist"

.PHONY: dist
dist: $(DIST) 
	@$(call distribuye,$(DIST),$(INSTALL_DIST))

endif	# LIB


# TODO: solo funciona (creo) con un .cpp. Que admita varios o pasarle 
#	el nombre al make
.PHONY: preprocesa
preprocesa:$(SOURCES)
	$(CXX) -E $(CXXFLAGS) $(CPPFLAGS) $(BIN_ARCH) -c $<


# Incluimos todos los archivos de dependencias.
# make considera que tiene que actualizar todos los archivos que 
# se incluyen, con lo que marcará los .d como out of date, garantizando
# que se generen
# TODO: Si incluyo las dependencias da un error al intentar compilar los
# ficheros en ensamblador .S
-include $(DEPENDENCIAS)



# ----------------
# Reglas genéricas
# ----------------
# Generamos las DEPENDENCIAS
# Es necesario pasarle los flags de CPPFLAGS ya que si no puede dar errores
# el preprocesador (de que no tiene definido F_CPU...)
%.d: %.cpp
	@$(PRINTF) "Generando DEPENDENCIAS..."
	@$(MAKE_DEPEND) $(CXXFLAGS) $(CPPFLAGS) $(BIN_ARCH) $< > $@.$$$$;\
	$(SED) 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$
	@$(PRINTF) "\n"


# Compilamos los .c
# No voy a compilar en C. Lo deshabilito para garantizar que
# no se compila lo que no quiero.
#%.o: %.c
#	 $(CC) $(CFLAGS) $(CPPFLAGS) $(BIN_ARCH) -c -o $@ $<;


# Compilamos los .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(BIN_ARCH) -c -o $@ $<


# Compilamos los .S(.asm)
%.o: %.S
#	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(BIN_ARCH) -c -o $@ $<
#	$(CXX) $(ASMFLAGS) $(BIN_ARCH) -c -o $@ $<
	$(CXX) -mmcu=$(MCU) -c -o $@ $<


# Generación de librerías estáticas
%.a: $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $?


# Compilamos los .elf
%.hex: %.elf
	 $(OBJCOPY) -j .text -j .data -O ihex $< $@

# Generamos el .eeprom
%.eeprom: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ 


%.lst: %.elf
	$(OBJDUMP) -S $< > $@


%.h: %.mnu
	$(MAKE_MENU) $< > $@

# Optionally create listing file from .elf
# This creates approximate assembly-language equivalent of your code.
# Useful for debugging time-sensitive bits, 
# or making sure the compiler does what you want.
.PHONY: disassemble 
disassemble: $(BIN).lst

.PHONY: disasm
disasm: disassemble

# Optionally show how big the resulting program is 
.PHONY: size 
size:  $(BIN).elf
# avr-size tiene un error (ver https://bugs.archlinux.org/task/32504)
# supuestamente corregido que no tiene opciones -C y --mcu. Antes funcionaba,
# pero al instalar gcc 7 ó ubuntu 18 (no se cuál de los dos influyó o si es
# otro programa) dejó de funcionar la opción -C y --mcu
#	$(AVRSIZE) -C --mcu=$(MCU) $(BIN).elf
#	$(AVRSIZE) $(BIN).elf	# la berkeley ha dejado de funcionar
	$(AVRSIZE) -G $(BIN).elf


.PHONY: clean
clean:
	$(RM) $(BIN).elf $(BIN).hex  \
	$(BIN).d $(BIN).eep $(BIN).lst \
	$(BIN).lss $(BIN).sym $(BIN).map \
	$(BIN).eeprom $(FIC_TAGS) $(DEPENDENCIAS) \
	$(OBJECTS) $(BIN) $(LIB_NAME) $(FIC_TAGS) \
	$(AVRDUDE_LOG)

.PHONY: super_clean
super_clean: clean
	$(RM) *.elf *.hex *.d *.eep *.lst \
		  *.lss *.sym *.map *.eeprom \
		  *.o *.a tags *.tmp \
		  *.ii *.s



# Creamos las tags. 
$(FIC_TAGS): $(SOURCES)
	@$(CTAGS) .

# ----------------------------------
# Flashing code to AVR using avrdude   
# ----------------------------------
# Subimos el .hex al AVR
.PHONY: flash
# 30/10/2024
# Al intentar upload programas al atmega4809, falla porque no está subiendo la
# .rodata section. Si en lugar de subir el .hex se sube el .elf, que avrdude
# entiende, avrdude sube automáticamente la .rodata section. De momento no
# borro esto ya que lo llevo usando más de 7 años con el atmega328p. Cuando se
# haya probado con el atmega328p, borrarlo.
#flash: $(BIN).hex 
#	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PROGRAMMER_PORT) -b $(PROGRAMMER_BAUDRATE) -p $(MCU) $(PROGRAMMER_ARGS) -U flash:w:$<

flash: $(BIN).elf
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PROGRAMMER_PORT) -b $(PROGRAMMER_BAUDRATE) -p $(MCU) $(PROGRAMMER_ARGS) -U flash:w:$<:e

# Un sinónimo. ¿Mejor flash o upload?
.PHONY: upload
upload: flash


.PHONY: flash_eeprom
flash_eeprom: $(BIN).eeprom
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -U eeprom:w:$<

.PHONY: avrdude_terminal 
avrdude_terminal:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE)  -P $(PROGRAMMER_PORT) \
			   -b $(PROGRAMMER_BAUDRATE)  -p $(MCU) -t 

# help
.PHONY: help
help:
	@$(PRINTF) "\nHELP\n"
	@$(PRINTF) "----\n"
	@$(PRINTF) "Tres modos de funcionamiento dependiendo si se definen las siguientes variables:\n"
	@$(PRINTF) "\tBIN: compila un programa generando el ejecutable\n"
	@$(PRINTF) "\tLIB: compila una librería pudiendo distribuirla con 'make dist'\n"
	@$(PRINTF) "\tDIST: no compila nada, limitandose a distribuir archivos 'make dist'\n"
	@$(PRINTF) "\ntags\n"
	@$(PRINTF) "----\n"
	@$(PRINTF) "flash	: Compila y carga el programa en el avr\n"
	@$(PRINTF) "size 	: Muestra el tamaño del programa\n"
	@$(PRINTF) "          OJO: hay un problema con el avr-size que yo compilé,\n"
	@$(PRINTF) "          no funciona. Usar el que había instalado antes.\n"
	@$(PRINTF) "          PERO todo funciona si se quitan las opciones:\n"
	@$(PRINTF) "          avr-size xx.elf!!! PROBARLO\n"
	@$(PRINTF) "help	: Muestra esta ayuda.\n"
	@$(PRINTF) "debug	: Muestra las variables usadas para compilar.\n"
	@$(PRINTF) "          Si se quiere ver el código asm que genera,\n"
	@$(PRINTF) "          añadir 'USER_CXXFLAGS=-save-temps -fverbose-asm' al makefile.\n"
	@$(PRINTF) "\nFUSES\n"
	@$(PRINTF) "-----\n"
	@$(PRINTF) "read_fuses       : Muestra los fuses.\n"
	@$(PRINTF) "write_fuses      : Escribe los fuses. Hay que definir las variables\n"
	@$(PRINTF) "                   correspondientes dependiendo del avr:\n"
	@$(PRINTF) "                   (LFUSE, HFUSE y EFUSE) ó (FUSE0, FUSE1, ...),\n"
	@$(PRINTF) "set_default_fuses: Escribe los fuses como vienen de fábrica.\n"
	@$(PRINTF) "set_fast_fuse    : No dividimos la frecuencia de reloj entre 8,\n"
	@$(PRINTF) "                   dejando su máximo valor.\n"
	@$(PRINTF) "set_clock_output_fuse: No dividimos la frecuencia del reloj entre 8\n"
	@$(PRINTF) "                   y la sacamos por el pin CLKO para usarla o verla\n"
	@$(PRINTF) "                   en el osciloscopio.\n"
	@$(PRINTF) "set_clock_output_divide_by_8_fuse: Dividimos la frecuencia del reloj entre 8\n"
	@$(PRINTF) "                   y la sacamos por el pin CLKO para usarla o verla en\n"
	@$(PRINTF) "                   el osciloscopio.\n"
	@$(PRINTF) "set_external_crystal_8MHz:\n"
	@$(PRINTF) "                   Conectamos un cristal externo de 8MHz,\n"
	@$(PRINTF) "                   dividiendo la frecuencia del reloj entre 8,\n"
	@$(PRINTF) "                   y no sacando el reloj por el pin CLKO.\n"
	@$(PRINTF) "\n"
	

# Depuramos
.PHONY: debug
debug:
	@$(PRINTF) "Directorios de instalación\n"
	@$(PRINTF) "--------------------------\n"
	@$(PRINTF) "INSTALL_LIB = [$(INSTALL_LIB)]\n"
	@$(PRINTF) "INSTALL_INC = [$(INSTALL_INC)]\n"
	@$(PRINTF) "INSTALL_DIST= [$(INSTALL_DIST)]\n\n"

	@$(PRINTF) "Programas usados\n"
	@$(PRINTF) "----------------\n"
	@$(PRINTF) "CXX         = [$(CXX)]\n"
	@$(PRINTF) "MAKE_DEPEND = [$(MAKE_DEPEND)]\n"
#	@$(PRINTF) "CTAGS       = [$(CTAGS)]\n"

	@$(PRINTF) "\nDirectorios de búsqueda\n"
	@$(PRINTF)   "-----------------------\n"
	@$(PRINTF) "CPATH=[$(CPATH)]\n"
	@$(PRINTF) "LIBRARY_PATH=[$(LIBRARY_PATH)]\n"

	@$(PRINTF) "\nTargets\n"
	@$(PRINTF)   "-------\n"
	@$(PRINTF) "BIN      = [$(BIN)]\n"
	@$(PRINTF) "LIB_NAME = [$(LIB_NAME)]\n"
	@$(PRINTF) "DIST     = [$(DIST)]\n\n"


	@$(PRINTF) "Flags usados para compilar\n"
	@$(PRINTF) "--------------------------\n"
	@$(PRINTF) "CPP_STD= [$(CPP_STD)]\n"
	@$(PRINTF) "PROJ_CXXFLAGS  = [$(PROJ_CXXFLAGS)]\n"
	@$(PRINTF) "PROJ_LDFLAGS = [$(PROJ_LDFLAGS)]\n"
	@$(PRINTF) "CPPFLAGS = [$(CPPFLAGS)]\n"
	@$(PRINTF) "CXXFLAGS = [$(CXXFLAGS)]\n"
	@$(PRINTF) "LDFLAGS  = [$(LDFLAGS)]\n"
	@$(PRINTF) "ARFLAGS  = [$(ARFLAGS)]\n"

	@$(PRINTF) "\nVariables\n"
	@$(PRINTF)   "---------\n"
	@$(PRINTF) "SOURCES = [$(SOURCES)]\n"
	@$(PRINTF) "OBJECTS = [$(OBJECTS)]\n"
	@$(PRINTF) "DEPENDENCIAS = [$(DEPENDENCIAS)]\n\n"

	@$(PRINTF) "MCU   = [$(MCU)]\n"
	@$(PRINTF) "F_CPU = [$(F_CPU)]\n"

#	@$(PRINTF) "LFUSE = [$(LFUSE)]\n"
#	@$(PRINTF) "HFUSE = [$(HFUSE)]\n"
#	@$(PRINTF) "EFUSE = [$(EFUSE)]\n"
	@$(PRINTF) "READ_FUSE_STRING  = [$(READ_FUSE_STRING)]\n"
	@$(PRINTF) "WRITE_FUSE_STRING = [$(WRITE_FUSE_STRING)]\n\n"

	@$(PRINTF) "Flags usados por el programador\n"
	@$(PRINTF) "-------------------------------\n"
	@$(PRINTF) "PROGRAMMER_TYPE     = [$(PROGRAMMER_TYPE)]\n"
	@$(PRINTF) "PROGRAMMER_PORT     = [$(PROGRAMMER_PORT)]\n" 
	@$(PRINTF) "PROGRAMMER_BAUDRATE = [$(PROGRAMMER_BAUDRATE)]\n" 
	@$(PRINTF) "PROGRAMMER_ARGS     = [$(PROGRAMMER_ARGS)]\n\n"




# -----------------------------------------------------
#				DISTINTOS PROGRAMADORES
# -----------------------------------------------------
# If you've got multiple programmers that you use, 
# you can define them here so that it's easy to switch.
# To invoke, use something like `make flash_arduinoISP`
# -----------------------------------------------------
.PHONY: flash_usbtiny
flash_usbtiny: PROGRAMMER_TYPE = usbtiny
flash_usbtiny: PROGRAMMER_ARGS =  # USBTiny works with no further arguments
flash_usbtiny: flash

.PHONY: flash_usbasp
flash_usbasp: PROGRAMMER_TYPE = usbasp
flash_usbasp: PROGRAMMER_ARGS =  # USBasp works with no further arguments
flash_usbasp: flash

.PHONY: flash_arduinoISP
flash_arduinoISP: PROGRAMMER_TYPE = avrisp
flash_arduinoISP: PROGRAMMER_ARGS = -b 19200 -P /dev/ttyACM0 
flash_arduinoISP: flash

.PHONY: flash_109
flash_109: PROGRAMMER_TYPE = avr109
flash_109: PROGRAMMER_ARGS = -b 9600 -P /dev/ttyUSB0
flash_109: flash


# -----
# Fuses
# -----
# Valores por defecto para atmega328
LFUSE ?= $(LFUSE_DEFAULT)
HFUSE ?= $(HFUSE_DEFAULT)
EFUSE ?= $(EFUSE_DEFAULT)


.PHONY: write_fuses
write_fuses: 
	$(AVRDUDE) -c $(PROGRAMMER_TYPE)  -P $(PROGRAMMER_PORT) \
			   -b $(PROGRAMMER_BAUDRATE)  -p $(MCU) $(PROGRAMMER_ARGS) \
	           $(PROGRAMMER_ARGS) $(WRITE_FUSE_STRING)

.PHONY: read_fuses
read_fuses:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE)  -P $(PROGRAMMER_PORT) \
			   -b $(PROGRAMMER_BAUDRATE)  -p $(MCU) $(PROGRAMMER_ARGS) \
			   -l $(AVRDUDE_LOG) $(READ_FUSE_STRING)

#-nv

# TODO: lo siguiente solo funciona para avrs que tengan HFUSE/LFUSE/EFUSE.
# Generalizarlo para admitir otros micros (similar a lo hecho en fuses and 
# read_fuses)
ifeq ($(MCU), atmega328p)

# Called with no extra definitions, sets to defaults
.PHONY: set_default_fuses
set_default_fuses:  WRITE_FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m 
set_default_fuses:  fuses

# Set the fuse byte for full-speed mode
# Note: can also be set in firmware for modern chips
#set_fast_fuse: LFUSE = 0xE2
.PHONY: set_fast_fuse
set_fast_fuse: LFUSE = $(LFUSE_FAST_FUSE)
set_fast_fuse: WRITE_FUSE_STRING = -U lfuse:w:$(LFUSE):m 
set_fast_fuse: fuses

# Sacamos el reloj por el pin CLKO a la frecuencia del reloj del AVR
#set_clock_output_fuse: LFUSE = 0xA2
.PHONY: set_clock_output_fuse
set_clock_output_fuse: LFUSE = $(LFUSE_CLOCK_OUTPUT_FUSE)
set_clock_output_fuse: WRITE_FUSE_STRING = -U lfuse:w:$(LFUSE):m 
set_clock_output_fuse: fuses

# Sacamos el reloj por el pin CLKO, dividiendo la frecuencia del reloj entre 8
#set_clock_output_divide_by_8_fuse: LFUSE = 0x22
.PHONY: set_clock_output_divide_by_8_fuse
set_clock_output_divide_by_8_fuse: LFUSE = $(LFUSE_CLOCK_OUTPUT_DIVIDE_BY_8)
set_clock_output_divide_by_8_fuse: WRITE_FUSE_STRING = -U lfuse:w:$(LFUSE):m 
set_clock_output_divide_by_8_fuse: fuses

# Usamos un cristal externo de 8MHz, no dividiendo la frecuencia entre 8 
# TODO: el nombre es confuso, ¿cómo llamarlo?
.PHONY: set_external_crystal_8MHz
set_external_crystal_8MHz: LFUSE = $(LFUSE_SET_EXTERNAL_CRYSTAL_8MHZ)
set_external_crystal_8MHz: WRITE_FUSE_STRING = -U lfuse:w:$(LFUSE):m 
set_external_crystal_8MHz: fuses



# Set the EESAVE fuse byte to preserve EEPROM across flashes
#set_eeprom_save_fuse: HFUSE = 0xD7
.PHONY: set_eeprom_save_fuse
set_eeprom_save_fuse: HFUSE = $(HFUSE_EEPROM_SAVE_FUSE)
set_eeprom_save_fuse: WRITE_FUSE_STRING = -U hfuse:w:$(HFUSE):m
set_eeprom_save_fuse: fuses

# Clear the EESAVE fuse byte
.PHONY: clear_eeprom_save_fuse
clear_eeprom_save_fuse: WRITE_FUSE_STRING = -U hfuse:w:$(HFUSE):m
clear_eeprom_save_fuse: fuses

endif # if (MCU == atmega328p)





#***************************************************************************
#							FUNCIONES
#***************************************************************************
# Distribuye los ficheros en el directorio dado.
# En caso de que el directorio no exista lo crea.
# $(call distribuye,$(ficheros),$(directorio))
define distribuye
	@$(PRINTF) "-------------------------------------------------------------\n"
	@$(PRINTF) "Distribuyendo al directorio [$2]:\n"
	@$(PRINTF) "[$1]\n"
	@$(PRINTF) "-------------------------------------------------------------\n"
	$(MKDIR) $2
	for f in $1;		\
	do			\
		$(CP) $$f $2;	\
	done
endef

