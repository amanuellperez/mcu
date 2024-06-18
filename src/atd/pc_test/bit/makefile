SOURCES= main.cpp 
#SOURCES= main2.cpp 
#SOURCES= main3.cpp 

BIN = xx

# Para ver si atd::bit genera más código hay que eliminar excepciones y
# optimizar.
#USER_CXXFLAGS=-I$(MCU_INCLUDE) -O3 -fnothrow-opt -fno-rtti -save-temps
USER_CXXFLAGS=-I$(MCU_INCLUDE) -O3 -fnothrow-opt -fno-rtti

USER_LDFLAGS=-lalp

include $(CPP_COMPRULES)
