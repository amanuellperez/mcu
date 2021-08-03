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

Para hacer el parser estoy usando `bison`.

## Vídeo
[Calculadora básica](https://youtu.be/5mxkGtWMyUA)

### Problemas encontrados:

1. La calculadora no sabe calcular `2^8`. En lugar de dar 256 da 255.9999.
   Resulta que el problema lo tiene la función `pow` que suministra `avrlibC`.
   He probado a enlazar con las bibliotecas para manejar `double` que trae
   `avrlibC`y sigue sin funcionar. ¿Es un error por mi parte o es error real
   de `pow`? ¿Cómo resolver el problema? Una calculadora que no sabe elevar 2
   a la 8 no sirve para nada.

2. Para convertir un `double` en cadena y así poder mostrarlo en el LCD se
   puede usar la función `snprintf`. El problema es que hay que enlazar con
   las bibliotecas correspondientes (
   `USER_LDFLAGS=-Wl,-u,vfprintf -lprintf_flt -lm`) e incrementa el tamaño del
   programa en 2KB. Para evitarlo he probado a hacer mi propia función
   `to_string(double)`. Con esta función ahorro esos 2KB (el problema es que
   al no estar probada es poco robusta, aunque como esto es un proyecto de
   aprendizaje resulta interesante escribirla).



