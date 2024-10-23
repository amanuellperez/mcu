# AVR microcontrollers

De momento voy a probar a separar el código de los avr por familias, creando
los siguientes directorios:

* [`avr`](avr)
  Contiene los archivos de cabecera comunes a todos los avrs.

* [`atmega`](mega/README.md)
  Familia megaAVR.

* `atmega-0`
  Familia mega0AVR.

etc. Cada una de estas familias tiene su propio directorio.

Más adelante puede que sea interesante crear directorios `avr8`, `avr32` para
los micros de 8 bits o los de 32 bits. 

## ¿Por qué separarlos por familias?

Al escribir por primera vez un programa para el `atmega4809` e intentar
enlazarlo con las librerías de `std` y `atd` descubrí que se tratan de
plataformas distintas, siendo obligatorio compilar todas las librerías para
cada target. Eso dió lugar a `libmicro_atmega328p.a` y `libmicro_atmega4809.a`.

### Namespaces

Suponiendo que el funcionamiento de todos los micros de una misma familia sea
el mismo, cosa que la experiencia lo dirá, voy a meter de momento cada familia
en un namespace interno: `mega_`, `mega0_`, ... Estos namespaces son internos
y los iré cambiando a medida que sea necesario.

Cada microcontrolador por su parte tendrá su propio namespace: `atmega328p`,
`atmega4809`, ... Estos namespaces son estables, no se cambiarán en el futuro.

### Nombres de archivo

¿Cómo llamar a los archivos de cabecera? `avr_mega_timer1.h` vs
`mega_timer1.h`. Como el segundo es más corto opto por este. Si en el futuro
se ve que puede entrar en conflicto con otros archivos bastaría con meter
todos estos archivos en un directorio llamado `avr`.




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
ATmega328p


### TODO: faltan más familias


## Documentación
[Ejemplo GPIO pins](https://github.com/microchip-pic-avr-examples/atmega4809-getting-started-with-gpio-mplab)


