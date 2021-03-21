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

### `pc_test`
Todos los tests son automáticos para ser realizados en el ordenador. Se
compilan con `gcc` y no con `avr-gcc`.

Depende del paquete `alp` (creo que solo depende de `alp_test.h`, pero no he
revisado las dependencias, puede que alguna vez haya usado `alp_string.h`).



