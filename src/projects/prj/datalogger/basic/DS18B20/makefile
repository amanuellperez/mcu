BIN = xx

SOURCES= 	../prj_main.cpp 	\
			../prj_init.cpp		\
			../prj_menu.cpp		\
			../prj_print.cpp	\
			../prj_time.cpp		\
			../prj_locale.cpp	\
			../hwd_interrupts.cpp	\
			prj_menu_sensor.cpp \
			hwd_init.cpp		\
			hwd_ds18b20.cpp	


MCU = atmega328p

#F_CPU=8000000UL

# Incluimos el directorio actual de compilación dentro de la búsqueda
# de los archivos de cabecera para que pueda encontrar el compilador
# `hwd_dev.h` y demás.
USER_CXXFLAGS=-I.

include $(PRJ_GENRULES)




