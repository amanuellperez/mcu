# Ejecutar: make.sh
# No defino aquí F_CPU, sino que make.sh usa LIST_F_CPU

SOURCES= mega_sleep_hwd.cpp			\
		 mega_debug_hwd.cpp			\
		 mega_ADC.cpp			\
		 mega_SPI_hwd.cpp 		\
		 mega_SPI.cpp 			\
		 mega_timer0_hwd.cpp	\
		 mega_timer0.cpp			\
		 mega_timer1_hwd.cpp	\
		 mega_timer1.cpp			\
		 mega_UART.cpp


INCS= \
	mega_ADC_hwd.h	\
	mega_ADC.h			\
	mega_atmega_dev.h	\
	mega_cfg_hwd.h			\
	mega_clock_frequencies.h\
	mega_debug_hwd.h			\
	mega_interrupt.h		\
	mega_micro.h			\
	mega_pin_hwd.h				\
	mega_port_hwd.h	\
	mega_import_avr.h		\
	mega_sleep_hwd.h			\
	mega_SPI_hwd.h		\
	mega_SPI.h	\
	mega_timern_hwd.h	\
	mega_timer0_hwd.h	\
	mega_timer0.h\
	mega_timer1_hwd.h	\
	mega_timer1.h\
	mega_timer2_hwd.h	\
	mega_timer2.h\
	mega_TWI_hwd.h		\
	mega_UART.h				\
	mega_USART_hwd.h		\
	mega_UART_baud_rate.h	\
	mega_watchdog_hwd.h			\
	mega.h	


MCU=atmega328p

LIB=micro

INSTALL_INC=$(MCU_INCLUDE)

include $(AVR_GENRULES)




