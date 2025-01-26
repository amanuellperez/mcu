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


# NO USAR atmega328p!!! SOLO TIENE 2kB de RAM!!! MCU = atmega328p
#F_CPU = 1000000UL

# El atmega4809 tiene 6 kB de RAM!!!
MCU = atmega4809
F_CPU = 3333333UL



include $(PRJ_GENRULES)

# meter esto en genrules como una regla genérica?
prj_strings.h: prj_strings.pgm
	str2rom prj_strings.pgm

dev_strings.h: dev_strings.pgm
	str2rom dev_strings.pgm

sdc_strings.h: sdc_strings.pgm
	str2rom sdc_strings.pgm

