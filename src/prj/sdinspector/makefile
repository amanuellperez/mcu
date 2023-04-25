BIN = xx

SOURCES= dev_print.cpp	\
		 sdc_print.cpp	\
		 prj_main.cpp	\
		 prj_run.cpp	\
		 prj_init.cpp	\
		 prj_write_sector.cpp	\
		 prj_read_sector.cpp	\
		 prj_load_sector.cpp	

# para probar
#F_CPU = 8000000UL

MCU = atmega328p



include $(DEV0_GENRULES)

# meter esto en genrules como una regla genérica?
prj_strings.h: prj_strings.pgm
	str2progmem prj_strings.pgm

dev_strings.h: dev_strings.pgm
	str2progmem dev_strings.pgm

sdc_strings.h: sdc_strings.pgm
	str2progmem sdc_strings.pgm

