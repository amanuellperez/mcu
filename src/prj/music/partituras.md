## WARNING

No soy músico y no se nada de música. Así que lo que está escrito aquí puede
ser totalmente incorrecto. Si lo lees, te lo aprendes, y está mal es tu problema. 

Lo explicado aquí corresponde a la *Chromatic scale* que es la que necesito. 

Esta sin acabar: son unas primeras notas para ir viendo cómo transformar la
partitura en la sucesión {frequencia, duración} que es la que tocamos. Ese es
el problema. 


## El problema

Quiero probar la función `play(Song)` con diferentes canciones. En la primera
prueba que hice leí la partitura y escribí las notas a mano, lo cual es
aburrido y muy propenso a error. El problema es automatizar esa tarea.

Para ello necesito:

1. Ver si existe algún formato standard con la información que contiene una
   partitura que este preparado para que lo lea un ordenador.

2. Ser capaz de leer yo una partitura, para usando 1 y 2:

3. escribir un programa que transforme el fichero de la partitura de 1 en mi
   header file.


Resulta que mínimo existe un standard: el formato `musicxml`. Es xml y viene
documentado en la página 
[musicxml.com](https://w3c.github.io/musicxml/)

El siguiente paso es aprender yo a leer una partitura. El problema cuando he
empezado a leer cosas en internet sobre "Teoría de la música" (<-- por favor,
poner voz grave y rimbombante al leer lo entrecomillado) es el problema que se
encuentra un vidente (el músico) cuando quiere explicarle a un ciego (yo) qué
es el color azul:

    * [ciego]   ¿qué es el color azul?
    * [vidente] El color del cielo.
    * [ciego]   Pero... ¿cuál es el color del cielo?
    * [vidente] Azul


Hay muchas definiciones circulares, cosas directamente que no hay quien las
entienda (bueno, que yo no las entiendo) y demás.

Por ello este documento: voy a intentar aclarar lo poco que he sacado en claro
de lo que he leído para que así dentro de unos meses (o años) cuando retome
este proyecto parta de aquí, no teniendo que empezar de cero.


## La sucesión de frecuencias musical (frecuencias que tocamos)
Un piano tiene un número finito de teclas, y una guitarra un número finito de
cuerdas. De hecho cualquier instrumento es capaz de generar un número FINITO
de sonidos. Lo que vamos a hacer es tocar sonidos de unas determinadas
frecuencias. A la sucesión de las frecuencias que puede tocar un músico las
llamaré sucesión de frecuencias musical (no he encontrado en internet ningún
nombre).

El primer problema que tenemos es elegir esta sucesión. Veamos cómo se ha
hecho.

### Frecuencia fundamental
Cuando tocamos una cuerda la cuerda vibra. Aunque la vibración está compuesta
por un montón de armónicos (y son estos, creo, los que hacen que suenen
completamente distinto un La de un piano de un La de un violín) la frecuencia
fundamental es la que usamos para caracterizar la nota. Así, el La típico son
440 Hz.

Lo que hacemos cuando tocamos una cuerda de un piano o de un violín es generar
la frecuencia determinada de esa cuerda. Y eso es lo que realmente hacemos:
generar un sonido de 440 Hz por una determinada duración de tiempo.

En una primera aproximación podemos decir que una nota es el par 
(frecuencia, duración).

Más adelante, a lo mejor, podríamos mejorar esta definición indicando que una
nota es el trio (instrumento, frecuencia, duración) pero eso complica mucho
las cosas ya que ¿cómo conseguir generar el sonido de un violín? 



### Octavas
Las notas que se enseñan en el colegio son: `Do`, `Re`, `Mi`, `Fa`, `Sol`, `La`, 
`Sí`, volviendo a empezar con `Do`, ... De `Do` a `Do` hay 8 notas así que
supongo que por eso se clasifican las notas en octavas. Una octava son las
notas de `Do` a `Do`, sin incluir el último `Do` supongo (???) 

Resulta que la frecuencia del segundo Do es el doble de la frecuencia del
primer `Do`. Si al primer `Do` lo llamamos `Do_1`, al segundo `Do_2`, ...
resulta que la sucesión de sus frecuencias es una sucesión geométrica de razón
2.

Ejemplo: Si tomamos `La_4= 440 Hz`, tendremos que `La_5= 880 Hz`, `La_6 =
1760 Hz`, o también que `La_3 = 220 Hz`, `La_2 = 110 Hz`, ...

### Número de notas en una octava
Antes he indicado que supongo que el nombre de octava viene de que de `Do` a
`Do` hay 8 notas, luego uno que es ingenuo por naturaleza, da por supuesto que
hay 8 notas en cada octava. Lo cual es FALSO!!! Resulta que en cada octava hay
12 notas.

*NOTA: Parece ser que hay un montón diferentes de escalas musicales basadas en
el número de notas que tienen: la Chromatic tiene 12 notas, la Nonatonic 9,
... pero sospecho (???) que todas estas diferentes escalas no son más que
subconjuntos de la escala cromática.*


### Sucesión de frecuencias en música
La frecuencia de las notas forman una sucesión geométrica de razón  `r`.
Llamando `f_n` a la frecuencia de la nota `n` el término general de la
sucesión es `f[n] = f[0] * r^n`.

Para calcular la razón basta imponer que de `Do` a `Do` se duplica la 
frecuencia. Como en cada octava hay 12 notas eso se traduce en que 
`f[n + 12] = f[n] * 2` de donde obtenemos que `r^{12} = 2` o que `r` es la
raíz doceaba de 2 (= 1.05956)

Ya conocemos la razón, necesitamos conocer un elemento de la sucesión para que
esté definida la sucesión. Se elige que la nota `La` tenga una frecuencia de
440 Hz. Con este dato y la razón ya podemos calcular todas las frecuencias que
podemos tocar con un instrumento.


### MIDI note number
Ya tenemos nuestra sucesión de frecuencias. ¿Qué frecuencia hacemos que sea la
primera `f[0]`?  No podemos elegir la nota `La` de 440 Hz ya que sino la
mayoría de las frecuencias tendrían índices negativos.

Parece ser que se eligió la nota `Do` más pequeña que se puede generar. La
nota `Do` correspondiente a la octava del `La` de 440 Hz tiene una frecuencia
de 262 Hz. Eso hace que las frecuencias de los `Do(e)s` anteriores sean
(basta con dividir entre 2): 131, 66, 33, 16, 8, 4, 2, 1. Las frecuencias
audibles van de 20 Hz a 20 kHz con lo que las octavas cuyos `Dos` son 1, 2, 4
y 8 fijo no se van a escuchar resultando inútiles en música. Algunas de las
notas de la octava del `Do` de 16 Hz se van a escuchar (en la práctica puede
que ni escuchemos las notas de esa octava). Si me hubieran preguntado a mí yo
habría elegido la nota de 16 Hz como la primera nota de nuestra sucesión:
`f[0] = 16`, pero como no me preguntaron (y supongo tendrían motivos para
hacerlo) optaron por elegir el `Do` de 8 Hz. 

Tomando `f[0] = 16 Hz` hace que la nota de `La` de 440 Hz ocupe la posición
`n = 69`.

A esta sistema de numeración de las frecuencias se llama `MIDI note number`
(<-- este es el nombre del número, supongo, ¿cómo se llama el sistema? MIDI
system note number? (???)). 

Podemos decir que el MIDI note number del `La` es 69.

En la página de la
[wikipedia](https://en.wikipedia.org/wiki/Scientific_pitch_notation)
se puede ver una tabla con todas las frecuencias clasificadas por octavas muy
visual.

Como solamente oímos de 20 Hz hasta 20 kHz la frecuencia mayor que podemos
escuchar es `f[135] = 20 kHz` no teniendo necesidad de números MIDI mayores de
135. De hecho, la página anterior de la wikipedia para estos números en 127
para que entren todos en 7 bits. 

A la hora práctica basta con almacenar la frecuencia como un número MIDI en un
`uint8_t` ahorrando espacio (en la primera implementación que he hecho de esto
usaba 2 `uint8_t` para representar el pitch = {octave, step}, lo cual es
desperdiciar un byte).


## De cómo los músicos hablan de las frecuencias

### Numeración de las octavas
A la primera octava que contiene notas audibles se le asigna el número 0; a la
siguiente octava el 1, la siguiente el 2, ... obteniendo las sucesión de las
frecuencias de los `Do` como: `Do_0 = 16 Hz`, `Do_1 = 32 Hz`, `Do_2 = 65
Hz`,...

Según este sistema la primera octava es la -1: `Do_{-1} = 8 Hz`. El -1 lo
podemos entender como "no lo uses, las personas no somos capaces de oir las
notas de la octava -1" (¿por qué empezaron en 8 Hz y no en 16 Hz? (???))

Clásicamente se han llamado:

| Núm. octava |    Nombre    |
|-------------|--------------|
|     2       | Great octave |
|     3       | Smal octave  |
|     4       | One line     |
|     5       | Two line     |
|     6       | Three line   |
    

Las octavas audibles son de la 1 a la 10, aunque es posible que tanto la
primera como la décima octava no se escuchen.

### Notación: semitonos, steps

* Cuando un músico quiere pasar de una frecuencia `f[n]` a la siguiente
  `f[n+1]` dice que *aumenta un semitono*. El símbolo usado para indicar
  *aumenta un semitono* es el *sostenido (sharp en inglés)* que es parecido a
  la almohadilla # pero con las líneas no horizontales sino inclinadas (buscar
  en internet: hashtag vs sharp, mirar las imágenes)

* Si en lugar de subir un semitono lo bajamos el músico lo indica con un
  *bemol (flat en inglés)*

* Un *step* son 2 semitonos.


### Escala musical

La sucesión de frecuencia sonoras `f[n]` la vamos a clasificar en octavas: las
agrupamos en grupos de 12 en 12 (y sí, todavía me chirría el hecho de que una
octava tenga 12 y no 8 notas). Cada grupo de octava lo numeramos empezando por
el 0 (realmente empieza en -1, pero esas son frecuencias no sonoras así que
podemos ignorarlas en música) y llegan hasta 10. En cada octava podemos
numerar las notas del 0 a la 11 (estos serían los índices relativos a la
octava).

Al hablar de las notas de cada octava un músico no usa los números del 0 al 11
sino que usa las notas de la escala musical. En español son

`Do`, `Do#`, `Re`, `Re#`, `Mi`, `Fa`, `Fa#`, `Sol`, `Sol#`, `La`, `La#`,
`Si`


Una de las cosas que llama mucho la atención al principio es observar que la
escala que se enseña en el colegio: 

`Do`, `Re`, `Mi`, `Fa`, `Sol`, `La`, `Si`

no corresponden a frecuencias seguidas `f[0]`, `f[1]`, `f[2]`, ... sino a las
frecuencias `f[0]`, `f[2]`, `f[4]`, `f[5]`, `f[7]`, `f[9]` y `f[11]` (y el
siguiente `Do` sería `f[12]`), donde los índices que uso aquí son los índices
relativos a la octava.

A esta escala la llaman *The major scale*. Básicamente es un subconjunto de
las 12 notas de una octava elegidas de una determinada manera. En internet
para caracterizar a esta escala se fijan en los steps o half-step (semitone)
que hay que dar para pasar de una nota a la siguiente. En esta escala sería:
step-step-semitone, step-step-step-semitone. 

En el teclado de un piano se ve
muy bien esta clasificación: las teclas blancas son las teclas que generan la
frecuencia de las notas sin sostenido (`Do`, `Re`, `Mi`, `Fa`, `Sol`, `La`, `Si`, ...)
mientras que las negras son las que tienen sostenido. 



### Notación: chromatic and heptatonic scale
La escala con 12 notas se llama *Chromatic scale* mientras que la de 7 notas
*Heptatonic scale*. Como se ve la segunda es un subconjunto de la primera
usando la misma sucesión de frecuencias `f[n]`.


### Escala en inglés

En inglés the major scale se escribe como:

`C`, `C#`, `D`, `D#`, `E`, `F`, `F#`, `G`, `G#`, `A`, `A#`, `B`

*Sí, para que sea sencillo al principio empieza en la letra C y después de la G
viene la A (todo muy lógico y natural).*

Se puede escribir usando bemoles en lugar de sostenidos. En este caso quedaría

`C`, `Db`, `D`, `Eb`, `E`, `F`, `Gb`, `Ab`, `A`, `Bb`




## Partituras

### La finalidad de la partitura
Un pianista lo que hace es tocar una o varias teclas durante un tiempo. Un
guitarrista toca una cuerda de una determinada forma (¿hay diferentes formas
de tocar una cuerda?) por un determinado tiempo.

Las partituras son instrucciones concretas que dicen al pianista qué teclas
del teclado tocar en un momento dado y por cuánto tiempo. Nada más, así de
sencillo (espero que no lea esto un músico).

Tenemos 2 problemas diferentes: 

1. ¿Cómo decirle al músico qué nota tiene que tocar? 

2. ¿Cómo decirle la duración de la nota?


### Pentagrama

Imagina un piano inmenso donde cada tecla genere una de las frecuencias de
nuestra sucesión `f[n]`. Coloquémoslo horizontalmente. Es un teclado que tiene
más de 100 teclas, algo imposible de manejar para una persona (aunque un
ordenador sí que podría manejarlo).

Como este teclado es demasiado grande tapémoslo todo quedándonos con una
pequeña parte quedándonos con un instrumento de un poco más de 12 teclas. Eso
básicamente es el pentagrama.

Desde un punto de vista de programación lo que hacemos es solo ver una ventana
de un poco más de 12 teclas. En lugar de tener la sucesión `f[n]`, nos
centramos en el intervalo `[f[n], f[n + t]]`.

Imagina ahora que para que se vean mejor las teclas de nuestro instrumento las
pintamos negro, blanco, negro, blanco... Las instrucciones para tocar una
canción consistiría en dibujar un dedo (un círculo, ovalo, ...) sobre la tecla
correspondiente. Hacerlo así tiene un problema: el dibujo del dedo no se vería
sobre la tecla negra, así que lo que hacemos es disminuir el grosor de las 
teclas negras hasta que se convierten en una línea. El  dibujo obtenido es lo
que llamamos pentagrama.


### Major scale
Una de las cosas que mas me ha llamado la atención al empezar a aprender sobre
esto es que en el pentagrama las notas no están seguidas. 

El orden que se enseña en el colegio de las notas en el pentagrama es: 

`Do`, `Re`, `Mi`, `Fa`, `Sol`, `La`, `Si`

que corresponden a frecuencias cuyas diferencias son

step, step, semitone, step, step, step, semitone

que en la wikipedia lo escriben como

whole, whole, half, whole, whole, whole, half

donde "whole" significa "whole step" y "half", "half step = semitone" (uno de
los problemas al principio al mirar esto es la cantidad de formas distintas
que usan para hablar de lo mismo).



### Selección de la ventana  (o de dónde colocar la mano)

Puede concebir el pentagrama como una ventana sobre la sucesión musical
`f[n]`. La forma de indicar dónde colocar la ventana es mediante la clave que
se escribe al principio.

TODO
[Ver esto](https://en.wikipedia.org/wiki/Key_signature) ir al apartado `Table`


### Accidentals: sharp, flat and natural

Una nota en el pentagrama representa la frecuencia `f[n]`. Como en un
pentagrama hay notas que no vamos a poder dibujar ya que en la major scale
cada vez que subimos un step en el pentagrama equivale a pasar de `f[n]` a
`f[n+2]` necesitamos alguna forma de indicarle al músico "toca `f[n+1]`".

Para ello se usan 3 modificadores ("alteradores" según los músicos, o 
accidentals en inglés):

* sharp (sostenido): la nota representa `f[n+1]` en lugar de `f[n]`
* flat (bemol): la nota representa `f[n-1]` en lugar de `f[n]`
* natural: cancela previos modificadores (accidentals). 


Parece ser que hay dos sistemas de aplicar los modificadores:

* Que sean *sticky* usando la forma de hablar de C++. El modificador modifica
  todas las notas de la misma measure. Al llegar al fin de la measure deja de
  aplicarse.

* Que no sean *sticky*, en cuyo caso solo afectan a la nota que preceden.

Ver: [la wikipedia](https://en.wikipedia.org/wiki/Accidental_(music))



*NOTA: el 'natural' es el nombre en inglés. En español, según la wikipedia,
se llama
'becuadro'. Este modificador lo que hace es que la nota suene de forma
NATURAL. ¿a quién se le ocurrió llamarlo becuadro? @_@*



### Notas

TODO
* Las partituras son una ventana sobre la sucesión de frecuencias, pero una
  ventana discontinua (se tapan algunas de las frecuencias)

* ¿cómo se especifica la ventana? La clave.

* Pentagrama: sistema posicional y simbólico. La posición nos indica la
  frecuencia, el dibujo (símbolo) la duración. Por supuesto: hay que
  especificar la clave y el tiempo.


### Duración
TODO

* ¿cómo se especifica la duración? El tiempo




## ¿Por qué es un lío?

* Se suele usar *nota* como sinónimo
  de *una de las frecuencias de mi sucesión de frecuencias musical*. Pero en
  otros sitios se usa nota como el par (frecuencia, duración). Las notas
  dibujadas en una partitura tienen una duración.

* Una octava en lugar de 8 notas tiene 12.

* Da la impresión (???) de que la frecuencia de sonidos definida aquí son las
  frecuencias que se usan en música sea cual sea el sistema musical (???). La
  diferencia entre los distintos sistemas es la selección de la escala, que no
  es más que un subconjunto de la escala cromática de 12 notas.

* La traducción de la escala Do, Re, Mi, Fa, Sol, La, Si en inglés es C, D, E,
  F, G, A, B. ¡Se empieza en la C y después de la G viene una A!

* La escala normal Do, Re, ... no corresponde al intervalo `[f[n], f[n+8])`,
  sino que son 8 frecuencias del intervalo `[f[n], f[n+12])` donde se omiten
  `f[n+1]`, `f[n+3]`, `f[n+6]`, `f[n+8]` y `f[n+10]`.


* En internet se encuentran muchas incorrecciones. Ejemplo: dicen que un step
  es la diferencia de frecuencia entre dos notas consecutivas lo cual es
  falso. Eso es el semitono. 

* La explicación de la wikipedia es un ejemplo de explicación circular. Un
  buen ejemplo de cómo no se debe de explicar algo.


## Alguna conclusión

Es interesante comparar la diferencia entre un ingeniero y un músico. Si yo
quiero saber cómo funciona un chip basta con mirar la datasheet. Ahí lo
encuentras todo (sí que es verdad que a veces la redacción de la datasheet
es... pero bueno, esta todo detallado). 

Las partituras es algo similar: son instrucciones muy precisas que tiene que
ejecutar el músico. Lo curioso es que no encuentras una "datasheet" o un
documento que te explique cómo funciona. Y mira que la música lleva siglos y
hay un montón de músicos. ¿Por qué no hay un standard de cómo escribir
/leer partituras? Si total, en una partitura lo único que está escrito es:
pulsa estas teclas del piano durante x tiempo; luego pulsa estas otras, ...





