# The (partial) C++ standard library, without exceptions nor dynamic memory (new/delete).

gcc doesn't implement std for microcontrollers. So I decided to implement one to be
able to use it and also as a learning project.

I intend to write code as needed, little by little.


Tested: avr-gcc 9.2.0

---

# La (parcial) biblioteca estandar de C++ sin excepciones ni memoria dinámica (new/delete).

`gcc` no suministra esta biblioteca para los microcontroladores. Al no encontrarla por ninguna parte, 
o encontrar copias de gcc, decidí comenzar a implementarla para poder usarla y como proyecto de aprendizaje.

En principio iré escribiendola poco a poco, según vaya necesitando cosas.

## Test
`pc_test`: directorio con pruebas automáticas. Depende de `alp`.

Probado con: avr-gcc 9.2.0
