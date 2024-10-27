SOURCES= 

INCS= 	dev_dc_motor.h			\
		dev_EEPROM_iostream.h	\
		dev_EEPROM_lineal.h		\
		dev_LCD_big_digits.h	\
		dev_LCD_ostream.h		\
		dev_LCD_screen.h		\
		dev_LCD_screen.tcc		\
		dev_LCD_terminal.h		\
		dev_LCD_terminal.tcc	\
		dev_square_wave.h


# Aunque esta librería es genérica, hay que compilarla para el target
# correspondiente.
MCU=${DEFAULT_MCU_FOR_LIBS}

LIB=logic

INSTALL_INC=$(MCU_INCLUDE)

include $(LOGIC_GENRULES)




