BIN = xx

SOURCES= main.cpp	\
		 init.cpp	\
		 print.cpp	\
		 read_block.cpp


#MCU = atmega328p
#F_CPU = 1000000UL

MCU = atmega4809
F_CPU = 3333333UL


include $(HWD_GENRULES)

# meter esto en genrules como una regla genérica?
strings.h: strings.pgm
	str2rom strings.pgm



