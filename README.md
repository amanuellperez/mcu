MCU++ library
=============

Libraries and applications for microcontrollers (right now, only for
ATMEGA328).

# Compiler
`avr-g++ 11.3.0`

# Directories

## src: source code.

This directory contains the following:

1. Generic things for microcontrolers and also for PC: (programming level?)

    * [std](src/std/README.md): (Partial) C++ standard library, 
      without exceptions nor memory allocations.
     
    * [atd](src/atd/README.md): General utilities.
      Depends on `std`.
 
2. Devices: (device level?)

    * [avr](src/avr/README.md): "Drivers" for avr microcontrollers 
	(right now, only for ATMEGA328). 
      
    * [dev0](src/dev0/README.md): Built-in devices.
      To program here you have to know your chip, 
      you need to read the datasheet.

    * [dev1](src/dev1/README.md): User-defined devices.
    

    * debug: Utilities for debug. (<--- what is this???)
  
3. Algorithms with meaning (application level?)
    * [app](src/app/README.md): Application libraries.
  

4. Projects: 
    * [prj](src/app/README.md): Projects.


# mk: Compilation rules.

# test and pc_test:
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

# Compilador
`avr-g++ 11.3.0`

Voy a empezar a probar las nuevas características de C++20 en este proyecto
según vaya aprendiendo a manejarlas. Por ello esta rama en unos meses
dejará de compilar en compiladores anteriores. 

Creo una `branch` en github llamada `gcc9.2.0` que compila con ese compilador.
No tengo intención de mantenerla.

Si nunca has compilado la toolchain puede que
[este](help/instalacion_toolchain.md) documento te pueda ayudar.


# Estructura de directorios

## src: Todo el código fuente. 

Se puede organizar el código en diferentes niveles:

1. Cosas genéricas tanto para micros como para PC (programming level?)

    * [std](src/std/README.md): Biblioteca `std`, aquí incluyo todo lo de `std` que 
	 no trae `avr-gcc`.
	 `avr-gcc` no suministra ni excepciones ni los operadores new/delete. 
	 Por ello capan la `std` (supongo que para no tener que ir buscando 
	 qué código depende de excepciones y de new/delete).

       Mi `std` suministra todo (??? <-- ahora es mentira, suministra un 
       poquito) el código standard que no depende de las excepciones ni 
       hace uso de new/delete. Si algo depende de excepciones o lo
       elimino o no las suministro (ejemplo: `iostreams` no suministra
       excepciones).

    * [atd](src/atd/README.md): Funciones genéricas que se basan en mi std. 
	 No usan excepciones ni los operadores new/delete.

2. Dispositivos: (device level)
    
    * [avr](src/avr/README.md): Traductores del avr. 
	 En lugar de tener que recordar qué bit hay que escribir, 
	 usamos nombres.

	 Quien programa este directorio tiene que conocer cómo funciona el
	 microcontrolador. Necesita leer la datasheet.

    * [dev0](src/dev0/README.md): Built-in devices
	Cada dispositivo físico, cada chip, necesita un driver. Aquí es donde
	guardo todos esos drivers. Cada dispositivo tendrá dos archivos: el
	`_basic` que contiene el traductor y el `_generic` que contiene el
	conceptuador.

	Quien programa aquí conoce los detalles del chip, necesitando leer la
	datasheet.

    * [dev1](src/dev1/README.md): User-defined devices
	Los dispositivos que aparecen aquí son dispositivos construidos a
	partir de concepts, no sabiendo el dispositivo real que hay por
	debajo. 

	El programador no conoce la datasheet ya que desconoce el chip real al
	que accede. Lo que conoce son los diferents concepts.

    * debug: Funciones para depurar los dispositivos. Esto no sé qué es. Hay
      que revisarlo y posiblemente recolocarlo.

3. Algoritmos con significado (application level?)
    * [app](src/app/README.md)

4. Proyectos:
    * [prj](src/prj/README.md): Proyectos.


## mk: Reglas para compilar

## test y pc_test:
En cada directorio suministro directorios `test` o `pc_test`. Los `pc_test`
son test automáticos para realizar en el ordenador. Se compilan con `gcc`
mientras que `test` son test para realizar en el microcontrolador, compilados
con `avr-gcc`.

Los `pc_test` necesitan varios ficheros de 
[alp](https://github.com/amanuellperez/alp/tree/master/src) (otro proyecto).
Basta con que descargarse `alp_test.h`, `alp_test.cpp`, 
`alp_string.h` y `alp_string.cpp` para que compilen.


