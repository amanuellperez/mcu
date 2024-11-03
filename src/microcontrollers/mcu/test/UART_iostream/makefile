BIN = xx

SOURCES= main.cpp


MCU = atmega328p
F_CPU=1000000UL
#F_CPU=8000000UL

#MCU = atmega4809
#F_CPU = 3333333UL

# LFUSE = 0xA2 (a 8MHz) ==> sacar por CLKO la señal de reloj
#LFUSE=0xA2

# Usar cristal externo de 12MHz o 16MHz
#F_CPU=12000000UL
#F_CPU=16000000UL

# Conectar cristal externo sin sacar la señal de reloj por CLKO (pin 14):
#LFUSE=0xF7

# Conectar cristal externo sacando la señal del reloj por CLKO:
#LFUSE=0xB7

include $(HWD_GENRULES)
