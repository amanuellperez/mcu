# The (partial) C++ standard library, without exceptions nor dynamic memory (new/delete).

gcc doesn't implement std for microcontrollers. So I decided to implement one to be
able to use it and also as a learning project.

I intend to write code as needed, little by little.


Tested: `avr-g++ 11.3.0`

---

# La (parcial) biblioteca estandar de C++ sin excepciones ni memoria dinámica (new/delete).

`gcc` no suministra esta biblioteca para los microcontroladores. Al no encontrarla por ninguna parte, 
o encontrar copias de gcc, decidí comenzar a implementarla para poder usarla y como proyecto de aprendizaje.

En principio iré escribiendola poco a poco, según vaya necesitando cosas.

## Problemas con C++20
NOTA: Los añadidos de C++20 introducen un montón de dependencias, lo cual hace
que o bien le dedicas un montón de tiempo para escribir el archivo de cabecera
que te interesa o bien dejas (muy) incompleto dicho archivo. 
Por ejemplo: `std::span` tiene un montón de `requires`, lo cual te obliga a
escribirlos todos junto con sus dependencias. Pero no solo eso: si quieres
escribir simplemente el constructor `span(It p0, size_type N)` necesitas
`to_address`, que este se basa en `pointer_traits`, que se basa en
`addressof`. Vamos que si quieres implementar una función del standar
necesitas tener un montón de cosas implementadas.

## Pensamientos sobre el standard de C++
Todo lo que son `traits`, `concepts`,..., son cosas genéricas que se pueden
implementar independientemente de la plataforma en la que trabajes. ¿Por qué
no crear un proyecto donde se creen todas esas funciones genéricas?

Propuestas:
1. Ordenar las dependencias:

De hecho es fundamental ordenar las dependencias del standard. Desconozco si
estará hecho. Yo quiero ir implementando poco a poco el standard, pero ¿por
dónde empezar? La forma de hacerlo es: necesito una función, la escribo; pero
para implementarla necesito otras cosas, y para estas otras, y así
sucesivamente. 

2. Diseñar el standard por capas:

Entre las capas básicas están todos los concepts y traits. Pero yo quiero
poder usar el standard de C++ pudiendo omitir las excepciones o la memoria
dinámica. ¿No podemos ordenarlo en capas? De hecho una de las capas
fundamentales que falta al estandar es la capa de usuario (o aplicación, o
como se quiera llamar): si programo una
aplicación de ventanas quiero tener un mecanismo standard de C++ para abrir
las ventanas.

Hacerlo por capas permitiría crear diferentes proyectos que tendrían definidas
unas dependencias muy claras.



## Test
`pc_test`: directorio con pruebas automáticas. Depende de `alp`.

Probado con: `avr-g++ 11.3.0`
