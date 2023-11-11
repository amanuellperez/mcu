BIN = xx

SOURCES= main.cpp	\
		 ../../../avr_UART.cpp 


MCU = atmega328p


#F_CPU=8000000UL
F_CPU=1000000UL

# LFUSE = 0xA2 (a 8MHz) ==> sacar por CLKO la señal de reloj
#LFUSE=0xA2

# Usar cristal externo de 16MHz
#F_CPU=16000000UL

# sin sacar la señal de reloj por CLKO (pin 14):
#LFUSE=0xF7

# sacando la señal del reloj por CLKO:
#LFUSE=0xB7

include $(AVR_GENRULES)
