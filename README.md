MCU++ library
=============

Libraries and applications for microcontrollers (right now, only for
ATMEGA328).

#### Compiler
`avr-g++ 11.3.0`

#### Directories

##### src: source code.

This directory contains the following:

* [std](src/std/README.md): (Partial) C++ standard library, 
  without exceptions nor memory allocations.
 
* [atd](src/atd/README.md): General utilities.
  Depends on `std`.
  
* [avr](src/avr/README.md): "Drivers" for avr microcontrollers (right now, only for ATMEGA328).
  Depends on `atd`.
  
* [dev](src/dev/README.md): Devices (LED, LCD, ...)
  Depends on `avr`.
  
* debug: Utilities for debug.
  
* [app](src/app/README.md): Application libraries.
  Depends on `dev`.
  
* [prj](src/app/README.md): Projects.
  Depends on `app`.

#### mk: Compilation rules.

#### test and pc_test:
All the packages have a directory `test` or `pc_test`. The difference between
them is that `pc_test` are tests to do in the computer (you have to compile
them with `gcc`) but `test` are tests to do in the microcontroller.

If you want to compile `pc_test` you need some headers files of the 
[alp](https://github.com/amanuellperez/alp/tree/master/src) project (most
probably you need to download `alp_test.h`, `alp_test.cpp` and maybe
`alp_string.h` and `alp_string.cpp`).



---
Bibliotecas y aplicaciones para microcontroladores (en principio solo el
ATMEGA328).

### Compilador
`avr-g++ 11.3.0`

Voy a empezar a probar las nuevas características de C++20 en este proyecto
según vaya aprendiendo a manejarlas. Por ello esta rama en unos meses
dejará de compilar en compiladores anteriores. 

Creo una `branch` en github llamada `gcc9.2.0` que compila con ese compilador.
No tengo intención de mantenerla.

Si nunca has compilado la toolchain puede que
[este](help/instalacion_toolchain.md) documento te pueda ayudar.


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

* [app](src/app/README.md): Bibliotecas de aplicación.

* [prj](src/prj/README.md): Proyectos.


#### mk: Reglas para compilar

#### test y pc_test:
En cada directorio suministro directorios `test` o `pc_test`. Los `pc_test`
son test automáticos para realizar en el ordenador. Se compilan con `gcc`
mientras que `test` son test para realizar en el microcontrolador, compilados
con `avr-gcc`.

Los `pc_test` necesitan varios ficheros de 
[alp](https://github.com/amanuellperez/alp/tree/master/src) (otro proyecto).
Basta con que descargarse `alp_test.h`, `alp_test.cpp`, 
`alp_string.h` y `alp_string.cpp` para que compilen.
