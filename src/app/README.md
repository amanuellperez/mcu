## Biblioteca de aplicación

Todo el código escrito aquí tiene que ser genérico, no depender del hardware.
Aquí no hay ni un solo driver, ni traductor, ... 

Idealmente todo el código debería de basarse en dispositivos genéricos, para
que se pueda usar con cualquier tipo de microcontrolador, sensor, ...

En algunos casos, como en la biblioteca de trazas, hago ciertas elecciones:
uso un flujo UART
conectado con la configuración por defecto que estoy usando en todas las
aplicaciones. 

