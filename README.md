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

---
Bibliotecas y aplicaciones para microcontroladores (en principio solo el
ATMEGA328).


### Estructura de directorios

#### src: Todo el código fuente. 

Este directorio lo descompongo de momento en los siguientes:

* std: Librería std, aquí incluyo todo lo de std que no trae avr-gcc.
     avr-gcc no suministra ni excepciones ni los operadores new/delete. Por
     ello capan la std (supongo que para no tener que ir buscando qué código
     depende de excepciones y de new/delete).

   Mi std suministra todo (??? <-- ahora es mentira, suministra un poquito) el código standard que no depende de las
   excepciones ni hace uso de new/delete. Si algo depende de excepciones o lo
   elimino o no las suministro (ejemplo: iostreams no suministra
   excepciones).

* atd: Funciones genéricas que se basan en mi std. 
     No usan excepciones ni los operadores new/delete.

* avr: Traductores del avr. 
     En lugar de tener que recordar qué bit hay que escribir, usamos nombres.

* dev: Drivers de cualquier dispositivo (device) que conectamos 
     al microcontrolador.

* debug: Funciones para depurar los dispositivos. Compilarlo detrás de dev.

* app: Aplicaciones

#### mk: Reglas para compilar
