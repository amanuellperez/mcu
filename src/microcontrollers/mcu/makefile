SOURCES= mcu_one_wire.cpp

INCS= 	mcu_clock.h					\
		mcu_concepts.h				\
		mcu_cycle.h					\
		mcu_miniclock.h				\
		mcu_one_wire.h				\
		mcu_pin.h					\
		mcu_PWM.h					\
		mcu_SPI.h					\
		mcu_time.h					\
		mcu_train_of_pulses.h		\
		mcu_TWI_master.h			\
		mcu_TWI_master_ioxtream.h	\
		mcu_TWI_memory_type.h		\
		mcu_TWI_slave.h				\
		mcu_UART_iostream.h


# Aunque esta librería es genérica, hay que compilarla para el target
# correspondiente.
MCU=${DEFAULT_MCU_FOR_LIBS}

LIB=mcu

INSTALL_INC=$(MCU_INCLUDE)

include $(MCU_MCUGENRULES)




