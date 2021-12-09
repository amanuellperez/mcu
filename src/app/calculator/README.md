# Calculadora

Este es un proyecto básico para aprender a programar micros.

A nivel de hardware las calculadoras son muy sencillas: tienen un teclado y
una pantalla, nada más. Con todo esto da lugar a diferentes elecciones:

* ¿Qué tipo de teclado?
* ¿Qué tipo de pantalla? ¿Un LCD básico? ¿Una matriz de puntos?

O incluso podemos usar una de las pantallas táctiles modernas que incluyen
teclado y pantalla en un mismo dispositivo.

A su vez se pueden hacer diferentes tipos de calculadoras: básica, científica,
gráfica (algún tipo más?)

## Calculadora básica

En lugar de andar haciendo diferentes calculadoras, parece más práctico
programar una calculadora científica y luego dependiendo del teclado que tenga
la calculadora será básica o científica (si tiene tecla `sin` podrá calcular
senos, pero si no tiene esa tecla aunque la calculadora realmente pueda
calcularlos el usuario no podrá hacerlo).

Como es una prueba uso un teclado matricial de 5 x 5 (25 interruptores
dispuestos en 5 filas por 5 columnas), y como pantalla un LCD 16 x 2.

Para hacer el parser estoy usando `bison`. Al no haber usado nunca `bison` uso
como plantilla inicial la calculadora implementada en el libro "The UNIX
Programming Environment", de Brian W. Kernighan y Rob Pike (capítulo 8). Al
principio seguiré el libro bastante aunque según vaya aprendiendo lo iré
modificando a mi gusto.


## Vídeo
[Calculadora básica](https://youtu.be/5mxkGtWMyUA)

### Problemas encontrados:

1. La calculadora no sabe calcular `2^8`. En lugar de dar 256 da 255.9999.
   Resulta que el problema lo tiene la función `pow` que suministra `avrlibC`.
   He probado a enlazar con las bibliotecas para manejar `double` que trae
   `avrlibC`y sigue sin funcionar. ¿Es un error por mi parte o es error real
   de `pow`? ¿Cómo resolver el problema? Una calculadora que no sabe elevar 2
   a la 8 no sirve para nada. Esta NO FUNCIONA!!!

   Posibilidades:
   * Mirar a ver cómo poder usar las funciones existentes (esta solución
     parece la más práctica).

   * Escribir propias funciones `pow`. El problema es si sucede lo mismo con
     `sin`, `cos`, ... Habría que reescribir todas las funciones matemáticas
     lo cual lleva tiempo. 

   * Usar otro microcontrolador que tenga capacidad para operar con `double`
     directamente en hardware (FPU = floating point unit)


2. Para convertir un `double` en cadena y así poder mostrarlo en el LCD se
   puede usar la función `snprintf`. El problema es que hay que enlazar con
   las bibliotecas correspondientes (
   `USER_LDFLAGS=-Wl,-u,vfprintf -lprintf_flt -lm`) e incrementa el tamaño del
   programa en 2KB. Para evitarlo he probado a hacer mi propia función
   `to_string(double)`. Con esta función ahorro esos 2KB (el problema es que
   al no estar probada es poco robusta, aunque como esto es un proyecto de
   aprendizaje resulta interesante escribirla).

3. La idea era poder usar la misma calculadora básica y científica. Sin
   embargo, al intentar implementar la científica tengo problema con el
   teclado. ¿Cómo conectar un teclado de 64 teclas mínimo a un atmega32?

   Posibles soluciones:
   * No usar un atmega32 sino un micro con más pines. El problema es que creo
     (?) que no son DIP con lo que habría que soldarlos y prepararlos. Como
     esto es nuevo, esta opción de momento la dejamos para el futuro.

   * En lugar de conectar el keyboard como una matriz de teclas, usar un
     keyboard analógico. Solo se necesitaría 1 pin (en lugar de 16 para el
     teclado de 64 teclas). Esta opción promete, pero de momento no me ha
     funcionado (el transistor depende mucho de la temperatura y me varía la
     lectura). Hay que seguir pensándola.

   * Completar el teclado con más hardware para ahorrar pines: se puede usar
     un registro para ir haciendo el escáneado, puertas OR para detectar si
     hay alguna tecla pulsada, ... Esta opción tiene el problema de ser muy
     específica (más programación, más diseño de hardware) y al tener más
     componentes más cara (??? esto habría que calcularlo).

   * Usar otro micro usándolo como driver del teclado y comunicarlo con el
     micro de la calculadora via SPI, TWI, UART... Aunque esta opción al
     principio no parecía muy buena opción (usar otro micro para un
     teclado???) es posible que sea una de las más prácticas. Es económica:
     solo se necesita un micro, y sencilla de implementar (solo hay un
     componente más que soldar y fácil de programar ya que puedo usar
     `dev::Keypad`). Esta opción es fácil de ampliar y de dotar más
     funcionalidad al teclado.



