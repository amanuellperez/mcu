BIN = xx

SOURCES= main.cpp 			\
		 lex.cpp			\
		 calc.tab.cpp	

# Fundamental para que sscanf trabaje con double!!!
USER_LDFLAGS=-Wl,-u,vfscanf -lscanf_flt


MCU = atmega328p

include $(APP_GENRULES)


calc.tab.hpp: calc.ypp
	bison -d calc.ypp

calc.tab.cpp: calc.ypp
	bison -d calc.ypp

sclean: clean
	rm calc.tab.hpp calc.tab.cpp
