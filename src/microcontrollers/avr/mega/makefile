# Ejecutar: make.sh
# No defino aquí F_CPU, sino que make.sh usa LIST_F_CPU

SOURCES= mega_sleep.cpp			\
		 mega_debug.cpp			\
		 mega_ADC.cpp			\
		 mega_hwd_SPI.cpp 		\
		 mega_SPI.cpp 			\
		 mega_hwd_timer0.cpp	\
		 mega_timer0.cpp			\
		 mega_hwd_timer1.cpp	\
		 mega_timer1.cpp			\
		 mega_UART.cpp


INCS= \
	mega_hwd_ADC.h	\
	mega_ADC.h			\
	mega_atmega_dev.h	\
	mega_cfg.h			\
	mega_clock_frequencies.h\
	mega_debug.h			\
	mega_interrupt.h		\
	mega_micro.h			\
	mega_pin.h				\
	mega_hwd_port.h	\
	mega_import_avr.h		\
	mega_sleep.h			\
	mega_hwd_SPI.h		\
	mega_SPI.h	\
	mega_hwd_timern.h	\
	mega_hwd_timer0.h	\
	mega_timer0.h\
	mega_hwd_timer1.h	\
	mega_timer1.h\
	mega_hwd_timer2.h	\
	mega_timer2.h\
	mega_hwd_TWI.h		\
	mega_UART.h				\
	mega_hwd_USART.h		\
	mega_UART_baud_rate.h	\
	mega_watchdog.h			\
	mega.h	


MCU=atmega328p

LIB=micro

INSTALL_INC=$(MCU_INCLUDE)

include $(AVR_GENRULES)




