# Microcontrolador genérico

## El problema

### Parte 1: eliminar código críptico

Cuando empiezas a programar microcontroladores resulta muy llamativo que el
código que se encuentra en foros y tutoriales es totalmente ilegible: si me
escribieran en chino entendería más-menos lo mismo.

El problema es que los programas están hechos demasiado a nivel de hardware:
si quieres encender el timer 0 tienes que activar el bit 4 del registro XXXX,
y que no se te olvide poner el pin 23 como de salida, y eso escrito en código
queda todavía más ilegible.

Para evitar tener que escribir código totalmente críptico empecé a escribir
los traductores de un dispositivo: si quiero programar un Timer lo primero que
hago es leer la datasheet y traducir a código toda la funcionalidad que
suministra ese device. A nivel de eficiencia el código generado tiene que ser
el mismo que el código críptico, con la ventaja de que al darle a las
funciones nombres con sentido se puede leer el código.

Por ejemplo, para encender el Timer0 del atmega32 en CTC mode hay que escribir
en el registro TCCR0B el bit WGM02 a 0, y en el registro TCCR0A los bits WGM01
y WGM00 a 1 y 0 respectivamente. En lugar de tener que recordar los nombres
crípticos de los registros y qué bits y valores hay que escribir, encapsulo
todo ello dentro de la función `Timer0::CTC_mode()` que es mucho más fácil de
entender.

Los traductores ya llevo un tiempo usándolos y resultan muy sencillos de
implementar y sencillos de usar (aunque como es bajo nivel estas obligado a
conocer el hardware para poder usarlos).


### Parte 2: reutilizar código en diferentes microcontroladores

Cuando programo un ordenador desconozco completamente el tipo de hardware que
usa: sé que tiene un teclado, una pantalla, ... pero desconozco la marca, sus
características técnicas, ... Un teclado es un teclado: alguien aporrea teclas
y ya.


Yo quiero poder hacer algo parecido con los microcontroladores. ¿Por qué tengo
que saber que estoy programando un `atmega32`? ¿o un `pic`? Además, yo quiero
que simplemente cambiando un archivo (o varios, aunque uno solo sería ideal)
mi aplicación funcione INDEPENDIENTEMENTE del hardware conectado.


Esto me llevó a empezar a definir los dispositivos genéricos (lógicos,
virtuales, elige un nombre). Un dispositivo genérico es como el teclado: un
teclado tiene unas características básicas que comparten todos los teclados y
que son las que conoce el softwador.

Los dispositivos genéricos lo que hacen es quedarse con las características
comunes que deberían de tener todos los dispositivos de un tipo. Por ejemplo,
todos los pines, sean del microcontrolador que sean, tienen las mismas
características funcionales básicas: esa funcionalidad básica es la que define
el pin genérico.


Básicamente la idea es hacer un microcontrolador genérico: puede tener 2
timers, o 25 timers, pero cada timer funcionará exactamente igual. Ese
interfaz que me permite acceder al device es lo que llamo dispositivo
genérico.


¡Ey, pero el timer de mi micro tiene una característica que no tienen los
demás y yo quiero usarla! Si en tu programa necesitas acceder a todo el
hardware, usa los traductores directamente, pero si no usa los dispositivos
genéricos. Dos opciones:

1. Usar traductores: la ventaja es que accedes a todo el hardware del
   microcontrolador. Desventajas: tienes que conocer bien ese
   microcontrolador, con lo que te tienes que saber bien la datasheet, y no
   puedes reutilizar el código que escribas en ningún otro sitio.

2. Usar dispositivos genéricos: la ventaja es que puedes reutilizar el código
   que escribas, y a medida que los uses te aprenderás bien su funcionalidad.
   La desventaja es que no vas a poder acceder a todas las características del
   hardware que hay por debajo.

Elige.

La implementación del dispositivo genérico es sencilla: se limita a llamar a
las funciones del traductor por dentro.

El problema con los dispositivos genéricos es que hay que definir su interfaz
y para ello se necesita experiencia manejando muchos dispositivos.


### Parte 3: partes basadas en los dispositivos genéricos

Una vez que tengo un dispositivo genérico puedo empezar a construir código que
se base en él. 

Por ejemplo:
* si tengo un dispositivo para conectarme a TWI puedo crear `TWI_master`, 
  `TWI_slave` y `TWI_multimaster` que añaden un buffer interno y gestionan 
  el protocolo. 

* Puedo ponerle una capa a `UART` para poder usarlo como `iostream`. 

* O a los timers les puedo poner una capa encima para poder usarlos como
  `Miniclock` ó `Clock`.



## Capas generadas

A día de hoy la forma en que clasifico el código anterior es usando las
siguientes capas:

* Capa de traductores: estará formada por los traductores de cada 
  microcontrolador.  Aquí se encontrarán los directorios `avr`, `pic`, ...

* Capa de dispositivos genéricos: 
  Dentro del mismo directorio que el traductor ya que para implementarlos
  necesitas conocer la datasheet.
  

* Capa `mcu`: aquí se implementarán dispositivos o protocolos que basados
  exclusivamente en hardware suministrado habitualmente por los micros
  (timers, pins, ...).


