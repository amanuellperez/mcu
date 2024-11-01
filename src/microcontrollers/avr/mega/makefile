# Ejecutar: make.sh
# No defino aquí F_CPU, sino que make.sh usa LIST_F_CPU

SOURCES= mega_sleep.cpp			\
		 mega_debug.cpp			\
		 mega_ADC.cpp			\
		 mega_SPI_basic.cpp 		\
		 mega_SPI.cpp 			\
		 mega_timer0_basic.cpp	\
		 mega_timer0.cpp			\
		 mega_timer1_basic.cpp	\
		 mega_timer1.cpp			\
		 mega_UART.cpp


INCS= \
	avr_atmega.h		\
	mega_ADC_basic.h	\
	mega_ADC.h			\
	mega_atmega_dev.h	\
	mega_cfg.h			\
	mega_debug.h			\
	mega_interrupt.h		\
	mega_micro.h			\
	mega_pin.h				\
	mega_port_basic.h	\
	mega_import_avr.h		\
	mega_sleep.h			\
	mega_SPI_basic.h		\
	mega_SPI.h	\
	mega_timern_basic.h	\
	mega_timer0_basic.h	\
	mega_timer0.h\
	mega_timer1_basic.h	\
	mega_timer1.h\
	mega_timer2_basic.h	\
	mega_timer2.h\
	mega_TWI_basic.h		\
	mega_UART.h				\
	mega_UART_basic.h		\
	mega_UART_baud_rate.h	\
	mega_watchdog.h			\
	mega.h	


MCU=atmega328p

LIB=micro

INSTALL_INC=$(MCU_INCLUDE)

include $(AVR_GENRULES)




