MCU++ library
=============

Libraries and applications for microcontrollers (right now, only for
ATMEGA328).

#### Directories:

##### src: source code.

This directory contains the following:

* std: (Partial) C++ standard library, without exceptions nor memory allocations.
 
* atd: General utilities.
  
* avr: "Drivers" for avr microcontrollers (right now, only for ATMEGA328).
  
* dev: Devices (LED, LCD, ...)
  
* debug: Utilities for debug.
  
* app:           Applications.
  
#### mk: Compilation rules.

#### test and pc_test:
All the packages has a directory `test` or `pc_test`. The difference between
them is that `pc_test` are tests to do in the computer (you have to compile
them with `gcc`) but `test` are tests to do in the microcontroller.



---
Bibliotecas y aplicaciones para microcontroladores (en principio solo el
ATMEGA328).


### Estructura de directorios

#### src: Todo el código fuente. 

Este directorio lo descompongo de momento en los siguientes:

* [std](src/std/README.md): Biblioteca `std`, aquí incluyo todo lo de `std` que 
     no trae `avr-gcc`.
     `avr-gcc` no suministra ni excepciones ni los operadores new/delete. Por
     ello capan la `std` (supongo que para no tener que ir buscando qué código
     depende de excepciones y de new/delete).

   Mi `std` suministra todo (??? <-- ahora es mentira, suministra un poquito) el código standard que no depende de las
   excepciones ni hace uso de new/delete. Si algo depende de excepciones o lo
   elimino o no las suministro (ejemplo: `iostreams` no suministra
   excepciones).

* [atd](src/atd/README.md): Funciones genéricas que se basan en mi std. 
     No usan excepciones ni los operadores new/delete.

* [avr](src/avr/README.md): Traductores del avr. 
     En lugar de tener que recordar qué bit hay que escribir, usamos nombres.

* [dev](src/dev/README.md): Drivers de cualquier dispositivo (device) que conectamos 
     al microcontrolador.

* debug: Funciones para depurar los dispositivos. Compilarlo detrás de dev.

* [app](src/app/README.md): Aplicaciones.

#### mk: Reglas para compilar

#### test y pc_test:
En cada directorio suministro directorios `test` o `pc_test`. Los `pc_test`
son test automáticos para realizar en el ordenador. Se compilan con `gcc`
mientras que `test` son test para realizar en el microcontrolador, compilados
con `avr-gcc`.

