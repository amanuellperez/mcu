SOURCES= app_trace.cpp	\
		 pli_iostream.cpp


INCS= 	app_trace.h					\
		dev_signal_generator.h		\
		user_choose_number.h		\
		user_choose_string.h		\
		user_menu.h					\
		user_time.h	\
		pli_iostream.h


# Aunque esta librería es genérica, hay que compilarla para el target
# correspondiente.
MCU=${DEFAULT_MCU_FOR_LIBS}

LIB=pli

INSTALL_INC=$(MCU_INCLUDE)

include $(PLI_GENRULES)




