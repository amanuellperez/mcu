# AVR microcontrollers

De momento voy a probar a separar el código de los avr por familias: 

* [`atmega`](mega/README.md)
* `atmega-0`

etc. Cada una de estas familias tiene su propio directorio.

Al escribir por primera vez un programa para el `atmega4809` e intentar
enlazarlo con las librerías de `std` y `atd` descubrí que se tratan de
plataformas distintas, siendo obligatorio compilar todas las librerías para
cada target. Eso dió lugar a `libstd_atmega328p.a` y `libstd_atmega4809.a`.

## Familias de avrs

### tinyAVR 0-series

8 pins: ATtiny202, ATtiny402, 

14 pins: ATtiny204, ATtiny404, ATtiny804, ATtiny1604

20 pins: ATtiny406, ATtiny806, ATtiny1606

24 pins: ATtiny807, ATtiny1607


### tinyAVR 1-series

8 pins: ATtiny212, ATtiny412

14 pins: ATtiny214, ATtiny414, ATtiny814, ATtiny1614

20 pins: ATtiny416, ATtiny816, ATtiny1616, ATtiny3216

24 pins: ATtiny417, ATtiny817, ATtiny1617, ATtiny3217


### megaAVR 0-series

28 pins: ATmega808, ATmega1608, ATmega3208, ATmega4808

40 pins: ATmega4809

48 pins: ATmega809, ATmega1609, ATmega3209, ATmega4809


### megaAVR


### TODO: faltan más familias


## Documentación
[Ejemplo GPIO pins](https://github.com/microchip-pic-avr-examples/atmega4809-getting-started-with-gpio-mplab)


