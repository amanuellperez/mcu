### Generic function based in std.

It is generic code. The only requirement to work with microcontrollers is:
* Can't throw exceptions.
* Can't use memory allocations.

Tested with: avr-gcc 7.4.0.


---
### Funciones genéricas que se basan en std. 

Es código genérico que se puede compilar tanto con `gcc` como con `avr-gcc`.

Para que pueda compilar con los avrs es necesario imponer que:
* No lance excepciones.
* No use memoria dinámica.


Probado con: avr-gcc 7.4.0
