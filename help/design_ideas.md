# Ideas de diseño

1. El código tiene que ser legible en inglés.

   Que si tengo que modificar 1 año después un archivo entienda lo que está
   escrito sin necesidad de estar continuamente leyendo las datasheet
   correspondiente.

   La idea de usar inglés es que de esa forma es más asequible para todo el
   mundo. Además resulta más sencillo ya que todas las datasheets están en
   inglés. (Los comentarios los dejaré en español-inglés dependiendo del día.)
   

   *TODO: A día de hoy hay spanglish. Traducir todo el código al inglés.*
   

2. El hardware se conoce en tiempo de compilación. Configurar los dispositivos
   en tiempo de compilación (uso de `templates`).


## Traductores (paquete avr)

IDEA

Uno de los primeros problemas que te encuentras cuando empiezas a programar
un avr es que resulta casi imposible leer el código escrito por los demás: por
culpa de que hay que acceder a los bits de los registros del microcontrolador
y hay un montón o tienes una memoria increíble y además conoces la
arquitectura del micro a la perfección o te toca estar continuamente mirando
la datasheet para ver qué es lo que hace el programa. 

El código tiene que poderse leer, no tiene que ser críptico. De aquí surgen
los traductores (básicamente son los drivers más básicos que puedas escribir).
¿Qué es un traductor? Un traductor consiste en encapsular todos los accesos a
los bits de los registros del micro en funciones con nombre con significado.
Los llamo traductor porque para escribir uno basta con leer la datasheet e
irla traduciéndola a código.



DESCOMPONER LOS TRADUCTORES

Los dispositivos del avr son genéricos pudiendo funcionar de diferentes
formas. Así, por ejemplo, un Timer puede funcionar en "normal mode", en "CTC
mode", ... 

Si implemento un traductor literal basta con traducir las funciones de la
datasheet. Sin embargo eso puede dar lugar a mal usar el dispositivo. Por
ejemplo, si queremos operar en "normal mode" no tiene mucho sentido poder
definir como top el OCRA. Para evitar este mal uso opto por partir el Timer en
varias clases: 
* `Timer1`: clase base. Contiene la funcionalidad común a todos los timers 1.
* `Timer1_normal_mode`: El timer sólo funciona en "normal mode"
* `Timer1_CTC_mode`: El timer sólo  funciona en "CTC mode"
* `Timer1_fast_PWM`: El timer sólo  funciona en "fast PWM"
* ...




NOTACIÓN

Los archivos asociados a los traductores los llamaré `<dispositivo>_tr.h`.
Así, por ejemplo, tenemos:
* `avr_USART_tr.h`: traductor del USART.
* `avr_timer0_tr.h`: traductor del Timer0.
* `avr_SPI_tr.h`: traductor del SPI.



AÑADIENDO MÁS COSAS

Suministro funciones de más alto nivel construidas a partir del traductor.
Así, por ejemplo `avr_USART_iostream.h` es la implementación de UART como
flujo de entrada/salida. El fichero `avr_USART.h` incluye todos los archivos
de cabecera asociados con USART.






## Dispositivos (paquete dev)

Los dispositivos tienen que ir paramétrizados por el tipo de traductor usado.

Ejemplo:
```
using System_clock  = System_clock<avr::Timer1>;
usgin LCD = LCD_HD44780<avr::pin>;
```

De esta desvinculamos la implementación de los dispositivos del tipo de
microcontrolador que usamos.



NOTACIÓN

Una duda sobre la notación. En principio se me ocurren 2 posibilidades:

El traductor de cada dispositivo se encontrará en el archivo

1.  `<referencia_fabricante>_tr.h`

2.  ó `<tipo>_<referencia>_tr.h`


*Ejemplo: el archivo de cabecera del LCD HD44780 podemos llamarlo 
`HD44780_tr.h` ó `LCD_HD44780_tr.h`*

De los dos métodos parece más práctico el segundo por 2 motivos:

1. Los archivos de cabecera quedan ordenados por tipo. Todos los LCD empezarán
   por `LCD_<...>_tr.h`.

2. La referencia no indica nada del producto. Cuando al año de no usar un
   dispositivo lo busqué, ¿qué referencia tendrá? El nombre del archivo de
   cabecera me recuerda qué tipo de producto es.


DUDAS

(???) ¿Razones en contra? ¿Existe otra forma mejor de nombrar los archivos?

