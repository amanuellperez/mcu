# Índice

* [Capas](#capas)
* [Notación](#notación)
* [SPI](#spi)

## <a name="capas"></a>Capas

### Problema

Programar un microcontrolador es muy costoso en tiempo ya que hay que dominar
la datasheet. 


### Solución: capa de hardware y capa de abstracción del hardware

La solución que le doy (que coincide con la que se lleva dando en el mundo de
los ordenadores desde hace decadas) es definir 2 capas:

1. Hardware layer (traductores)

   En esta capa se definen todos los traductores del microcontrolador. Un
   traductor se limita a traducir la datasheet a `C++`. Esta capa no introduce
   ineficiencias ni nada, lo único que hace es traducir el lenguaje críptico
   de la datasheet, que habla de registros y demás, a funciones con nombre.


2. Hardware abstraction layer (drivers)

    En esta capa definimos los drivers de los dispositivos. La idea es definir
    una API común para todos los Timers, todos los SPI, ... De esa forma se
    podrá independizar la aplicación final del hardware real que hay por
    debajo.

#### Hardware abstraction layer

La Hardware Abstraction Layer lo que hace es suministrar un interfaz común
para todos los dispositivos de hardware. Esta capa consigue:

1. Unificar los drivers de acceso a cada dispositivo real de hardware.

2. (opcional) Definir funciones un poco de más alto nivel que sean más
   sencillas de manejar.

3. Introducir ineficiencias (por culpa de 2)

4. Impedir acceder a todas las capacidades del hardware.

Cuando una aplicación pueda trabajar con estos dispositivos genéricos, el
sofwador podrá ignorar por completo la datasheet del microcontrolador
simplificando mucho su trabajo. 

Cuando se necesiten funciones que no suministre el dispositivo genérico
bastará con trabajar directamente con el traductor del dispositivo
correspondiente.



## <a name="notacion"></a>Notación

(Experimental) Hasta ahora llamaba `_basic` a los traductores. La idea del
`_basic` es que el harwador al escribir `prj_dev.h` supiera si está trabajando
con un traductor o con un driver. Sin embargo, puede que quede mejor meterlo
todo dentro de un `namespace hwd`. De esa forma queda claro que al escribir
`myu::hwd::Timer0` nos referimos al traductor del `Timer0`, mientras que al
escribir `myu::driver::Timer0` nos estamos refiriendo al driver del `Timer0`.
Al escribir explícitamente si usamos el traductor o el driver el cliente tiene
claro lo que está haciendo.

A su vez, como la API de los drivers es independiente del hardware, se pueden
escribir tests genéricos que valgan para todos los microcontroladores.







