# Calculadora

Este es un proyecto básico para aprender a programar micros.

A nivel de hardware las calculadoras son muy sencillas: tienen un teclado y
una pantalla, nada más. Con todo, esto da lugar a diferentes elecciones:

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


## Vídeos
* [Calculadora básica](https://youtu.be/5mxkGtWMyUA)
* [Calculadora científica](https://youtu.be/_x9jF84GR6A)
* [Desmontandolas](https://youtu.be/Ngfcy57okbw)

## Problemas encontrados:

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
   * No usar un atmega32 sino un micro con más pines. Cotilleando Mouser he
     encontrado el chip ATMEGA4809-PF, tiene 48 pins y es DIP (con lo que es
     sencillo jugar con él).

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

     La he implementado: es sencilla y fácil de ampliar. Serviría para hacer
     teclados más elaborados.


4. Problemas de memoria: me generó un stackoverflow la calculadora científica.
   Es culpa de `yacc` que usaba mucha memoria por defecto. 

   Posibles soluciones:
   * Configurar `yacc` para que consuma menos memoria. Lo hice y empezó a
     funcionar correctamente.

   * Meter todos las cadenas de texto en memoria flash en lugar de en RAM.
     Hice la prueba con las cadenas de abreviaturas. Al meter las abreviaturas
     en memoria flash el programa total aumentó 40 bytes, pero el data paso de
     970 a 912 bytes (ahorré unos 60 bytes de memoria RAM). ¿Merece la pena
     hacerlo? 
   

## TODO
De momento paro el proyecto. Faltan varias cosas básicas:

1. La tecla ANS no funciona. 

2. No uso las constantes (pi, ...)

3. El seno, coseno y tangente funciona con ángulos escritos solo en radianes.
   Tendría que poder funcionar en radianes y grados.

4. Aunque está implementado que se muestre la raíz cuadrada y otros símbolos,
   la calculadora da syntax error si se intentan usar. Una forma de
   implementarlo es que `Interfaz` traduzca estos símbolos a una cadena:
   convierte el código de la raíz cuadrada en la cadena sqrt(...) y eso es lo
   que procesa la calculadora. Esta solución sería genérica para cualquier
   símbolo.

5. No funcionan las teclas SHIFT/ALPHA. Basta con modificar el teclado.

6. Conviene mejorar facilidad de manejo. Por ejemplo, `sen(30` da error porque
   no se ha cerrado el paréntesis. Sin embargo, en muchas calculadoras el
   último paréntesis se puede omitir.

7. En la calculadora científica estoy duplicando symbols, ocupando más memoria
   de la necesaria. Convendría no hacerlo.

8. En la basic (y por tanto supongo también en la científica) no funciona si
   se intenta calcular sqrt(25) + 3.

9. Y, aunque ya lo he comentado, no funciona bien con floats. Hay que cambiar
   de micro para implementar una real.

## Conclusiones (de la primera versión, versión 0.0)
Es un buen proyecto de aprendizaje. Se manejan floats; interesante hacer un
teclado analógico (que no me funcionó), otro conectando 2 avrs vía SPI; se
maneja yacc (dando un stackoverflow); manejo por primera vez la memoria flash;
y supongo que alguna cosa más.



