# avr MEGA0 family

## Default clock frequency

Datasheet: 10.3.3 el prescaler por defecto es 6. No he encontrado el valor por
defecto que trae OSC20M de fábrica. En el chip que tengo son 16MHz en cuyo
caso por defecto funciona a 2 666 666 Hz.

El valor del reloj interno que se usa lo da el fuse2: si vale 0x01 funciona a
16 MHz; si 0x02 a 20 MHz

## `F_CPU`

### Tipos de relojes 
En los `atmega4809` se debe diferenciar claramente entre: 

1. El reloj que usamos: `CLK_MAIN`. Se puede elegir dinámicamente entre
   `OSC20M`, `OSCULP32K` y `XOSC32K` (ver diagrama punto 10 de la datasheet).

   Además, la frecuencia `OSC20M`puede ser 16 ó 20 MHz, configurada por el
   fuse2.

2. El prescaler que usamos, que da lugar a `CLK_CPU` y `CLK_PER`. Estos dos
   relojes funcionan a la misma frecuencia y es lo que en `atmega328p`
   llamamos `F_CPU`.


### Definición de variables

Tenemos 2 frecuencias de reloj:

1. `CLK_MAIN`: es la frecuencia del reloj conectado al `avr`.

2. `F_CPU`: es la frecuencia a la que funcionan la CPU (`CLK_CPU`) y los
   periféricos (`CLK_PER`).

   Si se define `F_CPU` se pueden usar las funciones `_delay_us/ms`.


### Formas de implementar una aplicación

Mínimo hay 3 formas diferentes de trabajar con el `atmega4809`:

1. Usar durante todo el programa una frecuencia única. 

    En este caso `CLK_MAIN` y `F_CPU` están definidas en tiempo de
    compilación. Al definir `F_CPU` se pueden usar las funciones
    `_delay_us/ms`.


2. Usar durante todo el programa el mismo reloj, pero cambiar dinámicamente el
   prescaler.

   En este caso `CLK_MAIN` está definido en tiempo de compilación, pero no así
   `F_CPU`. No se pueden usar las funciones `_delay_us/ms`.



3. Cambiar dinámicamente el reloj que estamos usando.

    En este caso ni `CLK_MAIN` ni `F_CPU` están definidas en tiempo de
    compilación. 


### Criterio de diseño de los traductores

* Los traductores no quitar funcionalidad a los dispositivos.

Los traductores se limitan a traducir la datasheet, nada más. Eso implica que
no puedo dar por supuesto la existencia de una frecuencia fija `F_CPU` ya que
sería limitar las capacidades del microcontrolador. 

Pero por otra parte si se define `F_CPU` puedo calcular cosas en tiempo de
compilación (como el valor de BAUD del USART). Sería interesante implementar
el código de tal manera que si está definida `F_CPU` se calcule todo lo que se
pueda en tiempo de compilación, pero si no está definida se haga en tiempo de
ejecución.







