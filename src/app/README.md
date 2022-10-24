## Biblioteca de aplicación

Aquí no hay ni un solo driver, ni traductor, ... Todo el código escrito aquí
se basa en los drivers `dev_` y en el microcontrolador `mcu_`.

Idealmente todo el código debería de basarse en dispositivos genéricos, para
que se pueda usar con cualquier tipo de microcontrolador, sensor, ...

En algunos casos, como en la biblioteca de trazas, hago ciertas elecciones:
uso un flujo UART
conectado con la configuración por defecto que estoy usando en todas las
aplicaciones. 

