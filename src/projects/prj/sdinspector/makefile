BIN = xx

SOURCES= dev_print.cpp	\
		 sdc_print.cpp	\
		 prj_main.cpp	\
		 prj_run.cpp	\
		 prj_init.cpp	\
		 prj_flush_sector.cpp	\
		 prj_edit_sector.cpp	\
		 prj_print_sector.cpp	\
		 prj_load_sector.cpp	

# para probar
#F_CPU = 8000000UL

MCU = atmega328p
F_CPU = 1000000UL


include $(PRJ_GENRULES)

# meter esto en genrules como una regla genérica?
prj_strings.h: prj_strings.pgm
	str2rom prj_strings.pgm

dev_strings.h: dev_strings.pgm
	str2rom dev_strings.pgm

sdc_strings.h: sdc_strings.pgm
	str2rom sdc_strings.pgm

