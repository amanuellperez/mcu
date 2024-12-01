SOURCES= dev_BMP280_hwd.cpp	\
		 dev_BME280_hwd.cpp	\
		 dev_DHT_protocol.cpp	\
		 dev_DS1307_hwd.cpp	\
		 dev_DS18B20.cpp


INCS= 	dev_A4988.h				\
		dev_BME280_hwd.h		\
		dev_BMP280_hwd.h		\
		dev_CD4017B.h			\
		dev_DHT11.h				\
		dev_DHT22.h				\
		dev_DS1307_hwd.h		\
		dev_DS1307_clock.h		\
		dev_DS18B20.h			\
		dev_keyboard_analog.h	\
		dev_keyboard_code.h		\
		dev_keypad.h			\
		dev_keyrow.h			\
		dev_HCSR04.h			\
		dev_HD44780_hwd.h		\
		dev_HD44780_hwd.tcc	\
		dev_HD44780_charset.h	\
		dev_HD44780_hal.h		\
		dev_HD44780.h			\
		dev_L298N.h				\
		dev_led.h				\
		dev_LM75.h				\
		dev_MAX7219.h			\
		dev_MCP4231.h			\
		dev_push_button.h		\
		dev_SDD1306.h			\
		dev_sdcard.h			\
		dev_TMP36.h				\
		dev_25LC256_hwd.h		\
		dev_74HC595.h			\
		not_generic.h		


# Aunque esta librería es genérica, hay que compilarla para el target
# correspondiente.
MCU=${DEFAULT_MCU_FOR_LIBS}

LIB=hwd

INSTALL_INC=$(MCU_INCLUDE)

include $(HWD_GENRULES)




