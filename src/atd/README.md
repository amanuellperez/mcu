# Generic function based in std.

It is generic code. The only requirement to work with microcontrollers is:
* Can't throw exceptions.
* Can't use memory allocations.

Tested with: avr-gcc 9.2.0.


---
# Funciones genéricas que se basan en std. 

Es código genérico que se puede compilar tanto con `gcc` como con `avr-gcc`.

Para que pueda compilar con los avrs es necesario imponer que:
* No lance excepciones.
* No use memoria dinámica.

Probado con: avr-gcc 9.2.0

### Test
Al ser código genérico se pueden compilar en el ordenador y probarlo todo sin
necesidad de cargarlo en el microcontrolador, lo cual es más cómodo y permite
automatizar las pruebas automáticas.



