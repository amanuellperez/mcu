BIN = xx

SOURCES= main.cpp

#F_CPU = 8000000UL
# Configurar el LFUSE para sacar por CLKO la señal de reloj
#LFUSE=0xA2

# (RRR) ¿por qué usar un cristal de 12MHz?
#       A 3.3V es la máxima frecuencia a la que funciona el atmega32
#       (la máxima es 13.33MHz).
#       Si usamos el cristal de 16MHz el atmega hay que conectarlo a 5V.
#
# F_CPU = 16000000UL
F_CPU = 12000000UL
# Conectar cristal externo sacando la señal del reloj por CLKO:
LFUSE=0xB7

MCU = atmega328p

USER_CXXFLAGS=-DMCU=atmega

include $(HWD_GENRULES)




