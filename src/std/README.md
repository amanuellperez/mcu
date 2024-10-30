# The (partial) C++ standard library, without exceptions nor dynamic memory (new/delete).

gcc doesn't implement std for microcontrollers. So I decided to implement one to be
able to use it and also as a learning project.

I intend to write code as needed, little by little.

You can find this project in [github](https://github.com/amanuellperez/mcu).

Tested: `avr-g++ 11.3.0`

---

# La (parcial) biblioteca estandar de C++ sin excepciones ni memoria dinámica (new/delete).

`gcc` no suministra esta biblioteca para los microcontroladores. Al no encontrarla por ninguna parte, 
o encontrar copias de gcc, decidí comenzar a implementarla para poder usarla y como proyecto de aprendizaje.

En principio iré escribiendola poco a poco, según vaya necesitando cosas.

## ¿Dónde está estoy?

Este proyecto, de momento, lo tengo almacenado en github.
[Aquí](https://github.com/amanuellperez/mcu) puedes encontrar el original.


## Referencias

Se publica sistemáticamente un borrador del estandar.  
[Aquí](https://open-std.org/JTC1/SC22/WG21/docs/papers/) lo suelen publicar
(elegir el año correspondiente).

Por comodidad en este [enlace](doc/cpp_standard_2023_draft.pdf) se puede encontrar la
copia que estoy usando para implementarlo.

Aparte de la copia, [cppreference](https://en.cppreference.com/w/) suele ser
una buena referencia, y conviene cotillear el código de `gcc`, sobre todo
cuando el estandar no está nada claro.


## Sobre no usar memoria dinámica

¿Por qué no usar memoria dinámica?

A día de hoy solo programo el atmega32 que solo tiene 2kB de RAM, que es poca
memoria para introducir la gestión de memoria dinámica.

Pero la verdadera razón de por qué no implemento esta parte es simplemente
porque no lo necesito: mis programas, de momento, no la necesitan para nada.

(Bueno, inicialmente ni me la plantee implementarla porque he leído que los
gestores de memoria dinámica consumen ellos mismos memoria, y con solo 2kB de
RAM no quiero desperdiciarla en cosas no necesarias).


## Sobre no lanzar excepciones

¿Por qué no lanzarlas? 

El motivo es la leyenda urbana que dice de que el usar excepciones genera
mucho código. De hecho Herb Sutter tiene una charla proponiendo una futura
alternatima más eficiente. 

Al no haber hecho medidas desconozco si es verdad o no. De momento voy a
suponer que es cierto. 


### Problemas al implementar el estandar
Hay partes del estandar, como `string_view`, que dan la impresión de poder ser
útiles en microcontroladores. El problema es que algunas de sus funciones
lanzan excepciones. ¿Qué hacer con estas funciones? ¿No suministrar la clase
entera? ¿Suministrarla parcialmente?

Suministrarla parcialmente encaja bien con lo que estoy implementando:
implemento "la biblioteca estandar de C++ sin excepciones". El problema es que
es posible que si suministro parcialmente una clase, al empezarla usarla al
final quiera usar todo su interfaz. 

El problema es: ¿qué hacer con las funciones que lanzan excepciones?


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

3. Suministrar código:

   Hay partes del estandar (concepts, traits...) que están totalmente definidas.
   ¿Por qué no suministran archivos con el código también? Porque lo que estoy
   haciendo para escribir esas partes, básicamente, es copiar lo que pone el
   estandar. ¿Por qué cada implementación tiene que copiar lo mandado por el
   estandar? 

   Todo aquello que esté totalmente definido por el estandar tendrían que
   suministrarlo. Solamente las partes que admitan diferentes implementaciones
   tiene sentido que no las suministren, pero donde el implementador de la
   clase se tiene que limitar a copiar lo dicho por el estandar es una pérdida de
   tiempo no suministrarlo.

4. Test!!!:

   Unas bibliotecas tan importantes como las que suministra el estandar tiene que
   tener muy bien pensados los test que tienen que pasar aquellos que las
   implementen. Y sin embargo, los tests brillan por su ausencia. Sería
   interesante que se suministrase una batería de test que te las puedas
   descargar de un repositorio. Además, en ese repositorio podríamos contribuir
   todos los que implementamos el estandar, ya que al encontrar un error en
   nuestra implementación podríamos proponer un test que pasarían a tenerlo todas
   las implementaciones. De esa forma conseguiríamos que las implementaciones del
   estandar fueran más robustas.

   ¿Por qué no se suministran test? Oh, man, test is a difficult subject!!! (de
   hecho, muchas veces el problema es saber cómo probar el código escrito.
   Escribir código es fácil, escribir los test... puf! necesito que alguien me
   enseñe (ah, quiero una batería de test exhaustiva, que haga daño, que vaya a
   buscar todos los errores posibles; recordar que cuando quieres probar tu
   código tienes que ir a pillar, no basta con probar los casos donde sabes que
   funciona) (<-- lo triste es que esto es lo que se hace en la práctica).

### `uint8_t` no es un entero!!!

¿Por qué uso tanto `uint8_t`?

1. El atmega32 es de 8 bits: parece lógico manejar `uint8_t` en ese tipo de
   micros.

2. Al principio, para los bytes, empecé usando `std::byte` y resulto ser un
   dolor de xxx: había que estar todo el día haciendo castings, problemas al
   manejar arrays... Desconozco si es porque nunca lo había manejado o es que
   es incómodo de manejar. Resultado: cuando quiero usar un byte uso `uint8_t`

¿El problema?

Uno de los problemas más habituales al programar son todas las mentiras que
introducimos los programadores sin darnos cuenta (yo el primero). Con
`uint8_t` sucede eso: el nombre dice que es un `unsigned int` de 8 bits
¿verdad? ¡Mentira! ¡Es un char! (y eso lleva sistemáticamente a confusiones en
el código: tú piensas que es un entero, pero el compilador lo trata como un
char y ahí está el lío).

Más de una vez he estado tentado de definir `atd::Uint8_t` y el resto de
`Uint16_t`, ..., dentro de `atd` pero eso generaría confusión. Por eso no lo
defino. 

### ¿Heredar namespaces?

Al implementar el `atmega4809` me encuentro con código común para todos los
avrs. Quiero definir mi namespace `avr` donde meta el código dentro de otro
namespace `protected`, código que solo se podrá llamar dentro de namespaces
"heredados" de `avr`.

Además, sistemáticamente creo namespaces `impl_of` o `private_` que contienen
código que no quiero que sea llamado por los usuarios. (creo que esto se puede
hacer con los módulos de C++20, pero como todavía no están implementados del
todo en gcc  no he jugado con ellos).


## Test
`pc_test`: directorio con pruebas automáticas. Depende de `alp`.

Probado con: `avr-g++ 11.3.0`
