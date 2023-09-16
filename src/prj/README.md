Some applications.

---

# Algunas aplicaciones.


## Principiantes
* [blink](blink): El "hola mundo" de los microcontroladores: hacer que un LED
  parpadee. 

* [POV toy](POV_toy): Es un POV display muy básico. De hecho es casi copia del
  ejemplo que trae el libro de MAKE. Es un buen proyecto para empezar amén de
  que el resultado es la mar de llamativo. 


## Útiles (directorio /util)
* [cost](util/cost): Programa que muestra los tamaños de los diferentes tipos
  usados por el avr. 

* [eeprom_read](util/eeprom_read): Hace un volcado de memoria de una EEPROM. Así
  podemos depurar y ver su contenido.

* [lcd_charset](util/lcd_charset): ¿Qué character set tiene un LCD? Esta
  aplicación muestra en la pantalla del LCD todos los caracteres que trae en
  la ROM, así se puede saber si es 'inglés/japonés', 'inglés/europeo' u otros.

* [oscilloscope](util/oscilloscope): Si quieres programar un mando de TV lo
  primero que tienes que hacer es aprender el protocolo con el que se
  comunican el emisor/receptor de IR. Si bien es fácil de ver la señal
  generada por el receptor en un osciloscopio ¿cómo verla en el
  microcontrolador? Este pequeño programa sirve para medir los pulsos
  generados por el receptor (o cualquier otro dispositivo).


## Un poco más elaboradas
* [clocks](clocks/README.md): Diferentes tipos de relojes.

* [calculator](calculator/README.md): Implementación de una calculadora
  básica.

* [IR_remote_control](IR_remote_control/README.md): Investigando el protocolo
  de los control remotos (por ejemplo, el mando de la tele).

* [music](music/README.md): jugando a tocar diferentes tonos en un altavoz. 

* [sdinspector](sdinspector/README.md): programa para ver el contenido de una
  SD-card (es un editor de bajo nivel de una SD-card)




# Estructura de los programas

El programa lo escriben dos personas: el hardwador, es quién sabe qué
dispositivos reales se van a usar, y el softwador que conoce qué dispositivos
ahi pero desconoce cuáles son.


## Archivos que escribe el hardwador

1. `prj_dev.h`

   Este archivo contiene las referencias a dispositivos reales
   que hay. Aquí es donde se encuentran cómo conectar el micro a los
   dispositivos. Se definen aquí también los dispositivos lógicos (o
   genéricos) que son los que usará el softwador.

2. `prj_init.cpp`

   Los dispositivos y el micro hay que inicializarlos. El hardwador sabe si
   tiene que inicializar SPI, o llamar a `sleep_mode`, y cómo se inicializa
   cada dispositivo, ya que cada dispositivo real tiene su propia forma de
   inicializarse. 

3. `prj_interrupts.cpp` (DUDA: ¿mejor `prj_isr.cpp`?)

   En este fichero se definen todas las ISRs que necesite el programa.


## Softwador

El softwador escribe el resto. 



## Esto es experimental

Por supuesto, como esto es experimental puede que sean necesarios más o menos
archivos o que esta forma de concebir los proyectos no sea correcta. 

Y recordar que la programación está pensada por capas: si se necesita hacer
cosas especiales o raras usar directamente los traductores que permiten usar
al máximo las capacidades de los dispositivos. Los interfaces genéricos tienen
la ventaja de ser más sencillos de manejar y ser intercambiables pero el pago
a hacer es no tener acceso a todas las capacidades del dispositivo.


