# En este directorio solo puede haber archivos de cabecera
# No puede haber ningún .cpp ya que el código escrito aquí tiene
# que ser válido sea cual sea el avr, sea cual sea el target. 

# Ejecutar `make only_dist` para distribuir los archivos


# Archivos a distribuir (pueden ser .h, .tcc, ..., cualquier cosa que no se
# compile aquí)
DIST =\
		avr_constants.h	\
		avr_time.h		\
		avr_types.h


INSTALL_DIST=$(MCU_INCLUDE)

include $(AVR_GENRULES)




