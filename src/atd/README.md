# Generic function based in std.

It is generic code. The only requirement to work with microcontrollers is:
* Can't throw exceptions.
* Can't use memory allocations.


---
# Funciones genéricas que se basan en std. 

Es código genérico que se puede compilar tanto con `gcc` como con `avr-gcc`.

Para que pueda compilar con los avrs es necesario imponer que:
* No lance excepciones.
* No use memoria dinámica.

## Estructura

Estas son funciones genéricas que quiero poder usar en programas para PC. Esto
me lleva a definir la siguiente estructura:

1. `std`: es el estandar. Idealmente lo suministraría el compilador. A falta
   de ello, voy escribiendo poco a poco lo que necesito para el avr.

2. `atd`: son funciones genéricas pensadas para ser usadas tanto en el avr
   como en los programas para PC. Dependen de `std` pero de nada más. Les
   impongo la reestricción de no poder lanzar excepciones ni manejar memoria
   dinámica.

3. [`alp`](https://github.com/amanuellperez/alp): son funciones genéricas 
   sin ningún tipo de restricción. Pueden lanzar excepciones y usar 
   `std::vector` y demás clases que manejan memoria
   dinámica. En principio están pensadas para ordenador. Aunque este proyecto
   no tiene nada que ver con `mcu` en la práctica uso algunas de sus funciones
   en los tests. Todos los directorios llamados `pc_test` contienen tests
   automáticos que ejecuto cargando el entorno de `alp` en lugar del de `mcu`.
   Es bastante práctico.


### `pc_test`
Todos los tests son automáticos para ser realizados en el ordenador. Se
compilan con `gcc` y no con `avr-gcc`.

Depende del paquete `alp` (creo que solo depende de `alp_test.h`, pero no he
revisado las dependencias, puede que alguna vez haya usado `alp_string.h`).



