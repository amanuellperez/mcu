BIN = xx

SOURCES= main.cpp	\
		 print.cpp	

# para probar
#F_CPU = 8000000UL

MCU = atmega328p



include $(DEV0_GENRULES)

# meter esto en genrules como una regla genérica?
strings.h: strings.pgm
	str2progmem strings.pgm



