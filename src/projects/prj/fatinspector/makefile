BIN = xx

SOURCES= atd_fat.cpp	\
		 dev_print.cpp	\
		 sdc_print.cpp	\
		 prj_dev.cpp	\
		 prj_fat.cpp	\
		 prj_print.cpp	\
		 prj_sector_driver_test.cpp	\
		 prj_reserved_area_test.cpp	\
		 prj_fat_area_test.cpp	\
		 prj_fat_area_list_test.cpp	\
		 prj_root_directory_test.cpp	\
		 prj_main.cpp	\
		 prj_run.cpp	\
		 prj_init.cpp	\
		 prj_print_sector.cpp


# NO USAR atmega328p!!! SOLO TIENE 2kB de RAM!!! MCU = atmega328p
#F_CPU = 1000000UL

# El atmega4809 tiene 6 kB de RAM!!!
MCU = atmega4809
F_CPU = 3333333UL

#USER_CXXFLAGS=-DTRACE_LEVEL=5
#USER_CXXFLAGS=-DTRACE_LEVEL=9
USER_CXXFLAGS=-DTRACE_LEVEL=100    # traza SDCard::read

include $(PRJ_GENRULES)

# meter esto en genrules como una regla genérica?
prj_strings.h: prj_strings.pgm
	str2rom prj_strings.pgm

dev_strings.h: dev_strings.pgm
	str2rom dev_strings.pgm

sdc_strings.h: sdc_strings.pgm
	str2rom sdc_strings.pgm

