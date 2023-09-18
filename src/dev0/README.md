# HARDWARE DEVICES
Dispositivos a los que accedemos desde el microcontrolador.

## Índice
* [Clasificación](#T-tipos)
* [Escritura de drivers](#T-drivers)
* [Drivers suministrados](#T-suministrados)

## <a name="T-tipos"></a>Clasificación
En principio, distingo los siguientes tipos:

* Built-in devices:

  Son los dispositivos reales: un LCD, una EEPROM, un ADC, un Timer, ...

  Este tipo de dispositivos pueden estar implementados dentro del
  microcontrolador, como por ejemplo los timers del avr, o ser externos al
  micro (un LCD, un DAC, ...).

  De estos dispositivos se suministra un driver para programarlos.

* User defined devices:
  
  Son devices que creamos a partir de los built-in devices. 

  Ejemplos:
 
  * Un LCD lo podemos ver como un ostream, o como una screen. 

  * Una EEPROM paginada la podemos ver como una EEPROM lineal, y esta a su vez
  la podemos ver como un iostream.

  El concebir un dispositivo de otra forma es lo que llamo user-defined
  devices. Estos devices de momento los guardo en `dev1`.



## <a name="T-drivers"></a>Escritura de drivers

¿Cómo escribir un driver? Mi propuesta (a día de hoy) es la siguiente:

### 1. Escribir el traductor

Cuando quieres programar un nuevo dispositivo lo primero que tienes que hacer
es leer la datasheet, y escribir un traductor. El traductor es una clase que
se limita a traducir la datasheet a código. No debe de tener más funcionalidad
que la suministrada que aparece en la datasheet y se usará la misma notación
de la datasheet.

Hacerlo de esta forma tiene la ventaja de que es muy sencillo de escribir y
mantener. Para escribir basta con escribir una función por cada función que
aparezca en la datasheet y limitarse a hacer lo que dice. Mantenerlo es igual
de sencillo: para ver si una función tiene un error basta con leer la
datasheet y ver si el código realmente hace lo que dice la datasheet.

#### Nombre
A falta de nombre he decidido poner el subfijo `_basic` a todas las clases que
representan traductores puros. Ejemplo: `DS18B20_basic` es el traductor del
sensor de temperatura `DS18B20`

#### Parámetros
Un device se conecta al micro usando un determinado protocolo (pins, SPI,
1-wire, 2-wire, ...). Todos los devices irán parametrizados por el Micro
(opcional???) y el protocolo usado.

Ejemplo: 
```
template <typename Micro, typename Protocol>
class Device_basic{
...
};
```

Cuando se tratan de protocolos es sencillo identificar qué parámetros pasar:

+ Si el protocolo es One Wire, parametrizamos la clase como `DS18B20<Micro,
   One_wire>`. 

+ Si es Two Wire, serán `BME280_basic<Micro, TWI>`.

+ Si es SPI, `25LC256_basic<Micro, SPI, SPI_selector>`.

Pero ¿qué parámetros pasar cuando el device se comunique con el micro usando
pines del microcontrolador? ¿Qué es mejor `Push_button<Micro, numero_de_pin>`,
o `Push_button<Pin>`?

La primera clase que escribí de `Push_button` la parametrizaba con el número
de pin y lo mismo hago con el LCD HD448870. El código se lee muy bien ya que
al escribir `Push_button<14>` queda claro que estamos conectando al pin número
14 del micro el push button. El problema es que la forma de implementar el
`Push_button` no es genérica, ya que las funciones que necesitamos llamar son
las funciones del `Pin<14>` correspondientes.

Un segundo intento es pasarle el `Pin<14>` como parámetro a `Push_button`:
conseguimos que el código quede completamente genérico. Da la impresión de que
está todo bien, hasta que revisamos el código de `KeyRow` y del LCD: al
definir `KeyRow` con los números de pines a los que se conecta este
dispositivo  `KeyRow<14, 15, 16>` queda claro cómo estamos conectando el
dispositivo, mientras que definirlo usando los pines correpondientes no queda
tan claro: `KeyRow<mcu::Pin<14>, mcu::Pin<15>, mcu::Pin<16>>` y peor queda el
LCD. 

Esto sugiere la tercera implementación: pasarle o bien Micro como parámetro y
que a partir de `Micro` use la clase `Pin` o pasarle directamente la clase `Pin`.
O bien `Push_button<Micro, 14>` o bien `Push_button<Pin, 14>`. A la primera
opción le veo 2 ventajas: 

1. El `Micro` se lo pasamos a todos los dispositivos. 

2. La implementación de `KeyRow` está hecho con números de pines. Cambiarla
   ahora a usar `Pin` como parámetro me obliga a repensarlo todo (y a día de
   hoy ya ando pensando en marcar esa clase como obsoleta y eliminarla).

Por ello de momento cuando un device se comunique directamente usando pines
con el micro los parámetros de plantilla a pasar serán `Device<Micro, pin0,
pin1, pin2, ...>` o mejor `Device<Micro, struct_with_npins>`.


#### Test
¿Cómo probar un traductor? Una forma que me resulta muy práctica es escribir
un programa que muestre un menu con todas (?) las funciones suministradas por
el dispositivo. Consigo varias cosas:

1. Pruebo todas las funciones de forma interactiva.

2. Aprendo a manejar el dispositivo.

3. Queda un programa escrito para probar si un dispositivo funciona o no. Por
   ejemplo, si mañana compro un nuevo sensor de temperatura `DS18B20`, y
   sospecho que no me funciona, tengo un programa que puedo usar para probar
   todas sus funciones.



### 2. Escribir el driver

El driver se encarga de ampliar la funcionalidad del traductor, simplificando
la vida del programador.

La idea es suministrar un interfaz más sencillo de usar, donde puedas
desconocer los detalles de la datasheet que si usas el traductor estas
obligado a conocer. 

Por ejemplo: idealmente el driver del sensor de temperatur `DS18B20` lo único
que debería de tener es una función `read_temperature()` o simplemente
`read()`. De esa forma quien usa este sensor en este programa no tiene que
recordar el protocolo marcado por la datasheet. 


### 3. Suministrar una interfaz genérica 

Cuando yo programo un proyecto (o aplicación) yo lo que quiero es definir en
un archivo `dev.h` los dispositivos de hardware que voy a usar y olvidarme por
completo en el resto del programa si se trata de un sensor de temperatura
`DS18B20`, o un `LM75A` o el que sea. Yo, como programador de aplicaciones, lo
único que quiero saber es que tengo un `sensor` y que puedo llamar a una
función `read()` que me devuelve la temperatura, independientemente del sensor
real que haya por debajo.

Observar que esto es lo que se hace al programar un ordenador: yo cuando
imprimo en pantalla no tengo ni idea de si la pantalla es la referencia xxxx
del fabricante xxxx, o es la yyyy del fabricante yyyy. Como programador lo que
tengo es una pantalla.

El interfaz genérico suministra precisamente eso: es un wrapper sobre el
el driver que traduce los nombres de las funciones concretas de ese
dispositivo a nombres genéricos que tienen todos los dispositivos de esa
clase. 

Lo ideal sería tener `concepts` definido para un sensor de temperatura, para
un LCD, ... y que ese interfaz sea el que se implemente en el interfaz
genérico.





## <a name="T-suministrados"></a>Drivers suministrados 

### EEPROMs
| Reference | I2C | SPI | datasheet |
|-----------|:---:|:---:|:---:|
| [25LC256](#25LC256)   |     |  x  |[datasheet](datasheet/EEPROM_25LC256.pdf)

### LCDs

Driver: [HD44780](#HD44780)

### Potentiometers (digital)
| Reference | I2C | SPI | datasheet |
|-----------|:---:|:---:|:---:|
|MCP4231    |     |  x  |[datasheet](datasheet/MCP4231.pdf)


### Registers
| Reference | Comments| datasheet |
|-----------| ---- |:---:|
| 74HC595   | 8-bit serial-in, parallel-out shift |[datasheet](datasheet/SN74HC595.pdf)


### RTC (Real-Time Clock)

| Reference | I2C | SPI | datasheet |
|-----------|:---:|:---:|:---:|
|DS1307	    |  x  |     |[datasheet](datasheet/DS1307.pdf)


### Sensors

| Reference |  T  |  P  |  H  |  G  | I2C | SPI | 1WIRE |datasheet |
|-----------|:---:|:---:|:---:|:---:|:---:|:---:|:-----:|:--------:|
|BMP280     |  x  |  x  |     |     |  x  |  x  |       |[datasheet](datasheet/BMP280.pdf)
|BME280     |  x  |  x  |  x  |     |  x  |  x  |       |[datasheet](datasheet/BME280.pdf)
|BME680     |  x  |  x  |  x  |  x  |  x  |  x  |       |[datasheet](datasheet/BME680.pdf)
|LM75A      |  x  |     |     |     |  x  |     |       |
|DS18B20    |  x  |     |     |     |     |     |   x   |[datasheet](datasheet/DS18B20.pdf)


T = temperature

P = pressure

H = humidity

G = gas

### Keyboards

#### Notación
Tengo 2 tipos de teclados:

1. Teclado básico: es un array o matriz de teclas. Las teclas quedan
   identificadas por sus coordenadas `(row, col)` o por la posición que ocupan
   (empezamos a numerar la primera tecla (0,0) como 0, luego 1, 2, 3, ...). La
   función básica que tendrá este teclado es `getkey()` que nos devolverá la
   tecla pulsada.

   Los llamaré `Keyrow` y `Keymatrix` (o `Keypad`).

2. Teclados: asignan un significado a cada tecla. La tecla `(0,0)`es la `a`.
   La tecla `(0,1)` la `b`, ... La función básica de este teclado es
   `getchar()` que nos devuelve el código ASCII (o el usado por la aplicación)
   con el significado de la tecla. De esta forma el programador al usar este
   teclado no sabe nada de la disposición de las teclas. 

   A estos teclados los llamaré `Keyboards`.



#### Tipos suministrados
De momento suministro dos tipos de teclado:
* `Keyrow`: Teclado formado por una fila de pulsadores.
  Está pensado para que sean 3-4 pulsadores como
  mucho. Cada pulsador está conectado a un pin del microcontrolador.

* `Keypad`: es un array de pulsadores colocados en forma de matriz. Lo llamo
  `Keypad` ya que los de Arduino lo llaman así, aunque quizás un nombre más
  sistemático habría sido `Matrix_key`, `Keymatrix` o algo parecido.

* `Keyboard_ADC`: no funciona. El problema con `keypad` es que si se quiere
  hacer un teclado para una calculadora científica, de 64 teclas mínimo, se
  necesitan demasiados pines. No se puede conectar un keypad tan grande y un
  LCD a la vez a un atmega32 (falta 1 pin!!! se podía usar el pin de reset
  pero no tengo programador en paralelo). Una forma de resolver es hacer 
  un keyboard
  analógico: pulsar una tecla se traduce en un cierto potencial que medimos
  con el ADC. Aunque da la impresión de que puede funcionar, a día de hoy
  todavía no he conseguido que funcione de forma segura 
  (garantizado su funcionamiento) ya que depende de la temperatura (basta con
  tocar con un dedo el transistor para que varien todos los potenciales). Lo
  dejo señalado aquí para tener presente mejorarlo para futuros proyectos.


### Clocks
Se tratan de dispositivos genéricos usados para medir tiempo, basados en
`Generic_timer`.

#### ¿Qué tipos de relojes podemos tener?

**Relojes que miden ticks = Tickers**

¿Cómo implementar un reloj? Para implementar un reloj se necesita un Timer (el
hardware). El problema del Timer es que tiene una resolución muy limitada: es
de 8 bits o de 16 bits. La forma de implementar un reloj es ampliar esta
resolución de tal manera que en lugar de 16 bits usemos 64 bits (ó realmente
16 bits del timer + 64 bits que ampliamos = 80 bits). 

Lo que hacemos es que cada vez que el Timer hace un overflow lanza una
interrupción que usamos para darle un tick a nuestro reloj. De esta forma si
programamos el timer para que genere 1 tick cada segundo, nuestro reloj medirá
segundos, si lo programamos para que mida 1 milisegundo, medirá milisegundos,
etc. 

Un reloj básicamente es un contador de ticks, que llamaré `Tickers` (podría
ser `Counter_ticks` pero es muy largo).

Aunque podría dar la impresión de que solo estamos interesados en medir tiempo
(x milisegundos, ...) no siempre es así. Por ejemplo, una forma de controlar
el bouncing de un teclado es midiendo ticks (donde 1 tick pueden ser 20 ms, 50
ms, o lo que sea). Así que puede (esto es experimental) que sean útiles los
tickers.




**Relojes que miden tiempo = clocks and watchs**

Una característica fundamental de un reloj es su resolución: en segundos, 
milisegundos, microsegundos, ... Además, pueden medir el tiempo hacia adelante
o hacia atrás. Algunos se pueden parar, otros no.

Ejemplos de relojes que podemos tener son:

* Reloj normal: 

  su resolución son segundos. Va hacia adelante. No se puede parar, 
  como mucho ponerlo en hora. Podríamos distinguir entre un reloj muy básico,
  que solo mide hasta horas, o un reloj normal que también lleva control de la
  fecha (esto serviría para implementar una alarma).

* Stopwatch:

  sirve para medir tiempo. Es el típico cronómetro. Dependiendo de lo que se
  quiera medir la resolución variará. Si quieres medir una carrera, con
  segundos valdrá. Si quieres hacer experimentos de física mejor milisegundos.
  ¿Merecerá hacer uno que mida microsegundos? ¿Se podrá hacer con el timer del
  avr?

  Los stopwatch se pueden: encender, parar. El tiempo va hacia adelante y
  empiezan en cero.

* Timer: 

  resolución en segundos. Va hacia atrás. Lo primero que hay que hacer es
  ponerlo en hora. Se enciende. Se para automáticamente al llegar a 0. Mide
  como mucho hasta horas. 


#### Implementación
  + `system_clock`: 

    reloj de sistema básico. Suministro este por consistencia con el standard `std`.


### A parte de los dispositivos incluyo interfaces genéricos:

* Dispositivos genéricos:
  + `signal_generator`


* user: interacción con el usuario a través de LCD y Keyboard.
  + `user_choose_number`
  + `user_choose_string`
  + `user_time`


La mejor forma de ver cómo funciona es mirar los test.

Como este paquete se basa en `avr` y `avr` lo he ido cambiando a medida que voy ganando
experiencia los primeros ficheros que hace tiempo que no uso puede que no compilen.
No los borro ya que con simples cambios seguramente se pueden actualizar.

Los dispositivos que fijo que funcionan (por lo menos a día de hoy) son:

#### <a name="25LC256"></a>EEPROM 25LC256

Formado por varios ficheros:

* `dev_EEPROM_25LC256_basic.h`:   es el traductor de la EEPROM 25LC256. 
* `dev_EEPROM_lineal.h`: concebimos la memoria como lineal.

* `dev_EEPROM_iostream.h`: concebimos la memoria como un flujo normal.
   
   Ejemplo: (ver `test/EEPROM_iostream/`)
```
EEPROM_ostream out;
out.open(address);
out << "Escribo esto en la memoria";
out.close();
```
  Hay que definir `EEPROM_ostream`. Consultar el test correspondiente (`EEPROM_iostream/interactivo`).




#### <a name="HD44780">LCD HD44780</a>

Es el típico LCD 16 x 2 ó 20 x 4.

Formado por varios ficheros:

* `dev_LCD_HD44780_basic.h`: es el traductor del dispositivo.
* `dev_LCD_screen.h`: concebimos el LCD como terminal.
   Un terminal es un terminal vulgar y corriente: vas escribiendo en él. 
   Cuando llegas a la última columna automáticamente pasa a la siguiente 
   fila, y si esta fuera la última fila tira todo lo escrito hacia arriba
   y continuas escribiendo.
   
* `dev_LCD_ostream.h`: concebimos el LCD como ostream.
   Por defecto se suministran LCD de 16x2 y de 20x04: `LCD_ostream_1602`
   y `LCD_ostream_2004`.
   
   Ejemplo: (ver `test/LCD`)
```
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;

using LCD_ostream = dev::LCD_ostream_1602<LCD_HD44780>;
 
LCD_ostream lcd;

lcd.clear();
lcd << "hola\n"; // escribo cadenas
lcd << 25;       // escribo números
```

##### Tipos de LCDs

La diferencia fundamental entre los LCDs son:
1. El driver que usan. De momento el único que tengo implementado es el HD44780.
2. El tamaño del display: 16 x 02 (2 filas x 16 columnas), 20 x 04, 40 x 04.
3. El potencial al que trabajan: a 5V, a 3'3 V.
4. El tipo de pantalla: puede estar iluminada o ser reflectante.
5. El color. La mayoría son monocolores pero se pueden encontrar multicolores.
   Además el fondo puede ser azul, verde...
6. El tipo de caracteres que trae: puede ser inglés/japonés; inglés/europeo y
   hay más (ruso por ejemplo).


##### Algunas referencias:
* 1602A: el típico que usa la gente de Arduino. Es de 16 x 02. Los hay a 5 V y
  también a 3'3 V.
* GFC1602M: de 16 x 02, a 5 V, pero reflectante.
* 1602O: de 16 x 02, de 3'3 a 5 V, multicolor. El driver es el AIP31068, es
  I2C (TWI).
* 2004A: de 20 x 04. A 5 V.
* 4004A: LCD de gran tamaño, de 40 x 04.

En aliexpress se pueden encontrar los de la marca Surenoo. Desconozco si serán
buenos, pero para aprender a manejarlos y jugar con ellos funcionan bien. Son
bastante más baratos que los que vende Amazon, además que en Amazon no te
indican ninguna característica con lo que no sabes lo que estas comprando. 

##### Vídeos
Estas son las pruebas de `LCD_screen` (los grabé con el móvil y han quedado la
mar de cutres, pero bueno son los primeros y no creo que nadie los vea):
* [test screen 16 x 02](https://youtu.be/Q0Fmtg7nzDE)
* [test screen 20 x 04](https://youtu.be/garxHAmSEDU) (este se ve fatal)
* [test screen 40 x 04](https://youtu.be/3GiaSXCdTn0)

Otros vídeos:
* [test big digits](https://youtu.be/GvXpkpzUbeE)

##### Documentación
* [Datasheet](datasheet/LCD_HD44780.pdf)
* app note AN658 de Microchip: LCD Fundamentals

Probado con: avr-gcc 9.2.0
