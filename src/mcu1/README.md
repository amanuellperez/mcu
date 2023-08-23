# GENERIC MICROCONTROLLER

Read the spanish description or look the directory `test`.

Tested: avr-gcc 9.2.0

---

# Microcontrolador genérico

De momento distingo dos capas para implementar el microcontrolador genérico:

* Capa `mcu0`: estará formada por los traductores de cada microcontrolador.
  Aquí se encontrarán los directorios `avr`, `pic`, ...

* Capa `mcu1`: aquí se implementarán dispositivos o protocolos que basados
  exclusivamente en hardware suministrado habitualmente por los micros
  (timers, pins, ...).

En principio implemento:

## Clocks
Se tratan de dispositivos genéricos usados para medir tiempo, basados en
`Generic_timer`.

### ¿Qué tipos de relojes podemos tener?

**Relojes que miden ticks = Tickers**

¿Cómo implementar un reloj? Para implementar un reloj se necesita un Timer (el
hardware). El problema del Timer es que tiene una resolución muy limitada: es
de 8 bits o de 16 bits. La forma de implementar un reloj es ampliar esta
resolución de tal manera que en lugar de 16 bits usemos 64 bits (ó realmente
16 bits del timer + 64 bits que ampliamos = 80 bits). 

Lo que hacemos es que cada vez que el Timer hace un overflow lanza una
interrupción que usamos para darle un tick a nuestro reloj. De esta forma si
programamos el timer para que genere 1 tick cada segundo, nuestro reloj medirá
segundos, si lo programamos para que mida 1 milisegundo, medirá milisegundos,
etc. 

Un reloj básicamente es un contador de ticks, que llamaré `Tickers` (podría
ser `Counter_ticks` pero es muy largo).

Aunque podría dar la impresión de que solo estamos interesados en medir tiempo
(x milisegundos, ...) no siempre es así. Por ejemplo, una forma de controlar
el bouncing de un teclado es midiendo ticks (donde 1 tick pueden ser 20 ms, 50
ms, o lo que sea). Así que puede (esto es experimental) que sean útiles los
tickers.




**Relojes que miden tiempo = clocks and watchs**

Una característica fundamental de un reloj es su resolución: en segundos, 
milisegundos, microsegundos, ... Además, pueden medir el tiempo hacia adelante
o hacia atrás. Algunos se pueden parar, otros no.

Ejemplos de relojes que podemos tener son:

* Reloj normal: 

  su resolución son segundos. Va hacia adelante. No se puede parar, 
  como mucho ponerlo en hora. Podríamos distinguir entre un reloj muy básico,
  que solo mide hasta horas, o un reloj normal que también lleva control de la
  fecha (esto serviría para implementar una alarma).

* Stopwatch:

  sirve para medir tiempo. Es el típico cronómetro. Dependiendo de lo que se
  quiera medir la resolución variará. Si quieres medir una carrera, con
  segundos valdrá. Si quieres hacer experimentos de física mejor milisegundos.
  ¿Merecerá hacer uno que mida microsegundos? ¿Se podrá hacer con el timer del
  avr?

  Los stopwatch se pueden: encender, parar. El tiempo va hacia adelante y
  empiezan en cero.

* Timer: 

  resolución en segundos. Va hacia atrás. Lo primero que hay que hacer es
  ponerlo en hora. Se enciende. Se para automáticamente al llegar a 0. Mide
  como mucho hasta horas. 


### Implementación
  + `system_clock`: 

    reloj de sistema básico. Suministro este por consistencia con el standard `std`.


## Protocolos

* 1-wire de Maxim


Probado con: avr-gcc 9.2.0
