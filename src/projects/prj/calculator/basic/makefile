BIN = xx

SOURCES= main.cpp 			\
		 lex.cpp			\
		 error.cpp			\
		 calc.tab.cpp	


# Fundamental para que sscanf trabaje con double!!!
# Aumenta 2K al enlazarlo con esta biblioteca
#USER_LDFLAGS=-Wl,-u,vfscanf -lscanf_flt -Wl,-u,vfprintf -lprint_flt -lm
#USER_LDFLAGS=-Wl,-u,vfprintf -lprintf_flt -lm
USER_LDFLAGS=-lm


MCU = atmega328p
F_CPU = 1000000UL

include $(PRJ_GENRULES)


calc.tab.hpp: calc.ypp
	bison -d calc.ypp

calc.tab.cpp: calc.ypp
	bison -d calc.ypp

sclean: clean
	rm calc.tab.hpp calc.tab.cpp
