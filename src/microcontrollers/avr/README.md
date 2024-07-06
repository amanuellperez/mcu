# Translators of the avr's datasheet

(If you understand spanish, read the spanish version below. All the code is in
english, not so the comments, but my goal is that you can read the code
without reading the comments, so if you understand english I hope you can
understand the code).


One of the problems programming a microcontroller is that you need to know the architecture 
of the microcontroller. You have to read the datasheet to remember which bit you have to
write one to make the microcontroller to do something. 

To avoid it I use translators: instead of remember the datasheet is easier to write

     if (UART::is_ready_to_transmit()) { do_something(); }
     
everybody can understand this code. 

### `UART` as a stream
Besides translators I provide another classes. For instance, I treat UART as a normal
iostream:

    avr::UART_iostream uart;
    avr::cfg_basica(uart);  // This is my configuration. You can use another.
    uart.on();
    
    uart << "Write a number:\n";
    uint16_t x;
    uart >> x;
    uart << "x = [" << x << "]\n";
    

Compiler: avr-gcc 11.3.0
    
---
# Traductores de la datasheet

# Índice
* [¿Por qué esta biblioteca?](#problemas)
* [Static interface vs objects](#staticono)
* [Problemas al programar el atmega usando SPI](#conexionSPI)
* [Cristales externos](#cristalexterno)
* [Timers](#timers)

## <a name="problemas"></a>¿Por qué esta biblioteca? 

Cuando empecé a programar un microcontrolador me encontré los siguientes
problemas básicos:

### Dificultad de leer el código
Uno de los problemas que te encuentras cuando empiezas a programar el microcontrolador
es que tienes que conocer perfectamente la arquitectura del hardware. Continuamente tienes
que estar leyendo la datasheet para recordar que bit tienes que poner a 1 para hacer
algo. Ese estilo de programar es propenso a errores. 

Para evitarlo creo los traductores: se limitan a traducir la datasheet a
código, nada más. No añaden ni funcionalidad y no introducen ineficiencias.
Es un interfaz de funciones, cuyo nombre viene definido por la datasheet (no
lo elige el que escribe el traductor) y se limitan a hacer el set o clear de
un bit en el registro indicado por la datasheet. 

El código escrito usando los traductores es mucho más fácil de leer que el
código tradicional que se encuentra escrito en C.  En lugar de recordar que 
para mirar si UART está listo para transmitir tengo que mirar
si el bit `UDRE0` del registro `UCSR0A` es 1, basta con escribir 

```
     if (UART::is_ready_to_transmit()) { haz_algo(); }
```
     
Es mucho más legible, menos propenso a error y más fácil de portar. Además, los traductores
son muy sencillos de escribir: basta con leer la datasheet y escribir las funciones correspondientes.
La propia datasheet te da la mayoría de las veces el nombre de la función que tienes que escribir.


### Dependencia del microcontrolador
Si escribes código para un avr particular puede que no te sirva para otro
diferente, pero fijo no te va a servir para un PIC o cualquier otro
microcontrolador de otro fabricante.

Yo no quiero escribir aplicaciones que dependan de los dispositivos concretos
de hardware que estoy usando. Prefiero escribir mi código para unos
dispositivos genéricos. Solo la configuración de la aplicación elegirá qué
dispositivos concretos voy a usar. 

Esto lleva a querer definir todos los dispositivos genéricos. Mientras que el
traductor traduce la datasheet a código, el conceptuador (<-- mmm, es el
primer nombre que se me ha ocurrido, es porque escribe concepts de tipo C++)
traduce el traductor a un dispositivo genérico, un interfaz común para todos
los dispositivos del mismo tipo. (NOTA: últimamente al dispositivo genérico
cada vez más lo llamó driver que desconozco si se corresponde exactamente con
la definición de driver. Pero encaja bastante bien: a fin de cuentas, cuando
el ordenador quiere acceder a un dispositivo concreto de hardware lo hace a
través del driver que tiene que tener el mismo interfaz sea cual sea el
dispositivo real de hardware: esto es, es un dispositivo genérico).

#### Ejemplo

Mi aplicación necesita un Timer, el cual es un dispositivo con el que
puedo realizar ciertas operaciones. El timer concreto puede ser
`atmega::Time_counter`, o `attiny::Time_counter`, o `PIC::Time_counter`, 
o `ESP::Time_counter`, pero sea cual sea el elegido todos representan el mismo
dispositivo lógico (el mismo concept de C++). La ventaja de esto es que mi
programa funcionará independientemente del Timer real elegido. La desventaja
es que al usar un interfaz genérico puede que no pueda incluir algunas
características particulares del Timer real que estoy usando. Pero si
necesitara usar esas características en lugar de usar `atmega::Time_counter`
usaría `atmega::Timer0` (el traductor que suministra todas las funciones
del dispositivo).

### Otro ejemplo

Se puede ver un ejemplo de lo que digo en la aplicación datalogger. Estoy
implementando la misma aplicación para diferentes sensores: DHT11, DHT22,
DS18B20 y HCSR04 (de momento). 

La idea es que un datalogger se limita a tomar medidas de un sensor, *sea cual
sea el sensor*. Podemos escribir una aplicación genérica que no conozca el
sensor concreto, y cuando se quiera hacer un datalogger para un sensor nuevo
lo único que hay que hacer es escribir la parte correspondiente del sensor.
Nada más.




### Notación

De momento pruebo con la siguiente notación:

* `atmega_timer0_basic.h`: contiene el traductor del `Timer0` del `atmega`

* `atmega_timer0.h`: contiene el dispositivo genérico correspondiente.

A día de hoy es `avr_timer0_basic.h` ya que, de momento, voy a suponer que
todos los timer0 del avr funcionan igual (según se vayan programando otros
avr diferentes del atmega, supongo que habrá que volver a la notación
`atmega_timer0_basic.h`).


### ¿Qué implementar lo primero?

Para poder depurar un programa se necesita una forma de trazarlo. En el caso
del atmega conviene programar cuanto antes el traductor del UART ya que una
vez definido ese traductor automáticamente deberíamos de tener `UART_iostream`
(desconozco si se puede hacer pero quedaría bastante bien poder programar un
`UART_iostream` parametrizado por un `UART_generic`. Una vez programado el
traductor `UART_basic`, con escribir `UART_generic` podríamos definir `using
atmega::UART_iostream = generic::UART_iostream<UART_generic>`).


### Algunos ejemplos
#### `UART` como flujo
Además de los traductores suministro también algunas clases de más alto nivel. Por ejemplo, 
un flujo normal y corriente para acceder a UART. 

    avr::UART_iostream uart;
    avr::basic_cfg(uart);      // esta es la configuración por defecto que siempre uso. Puedes usar otra.
    uart.on();
    
    uart << "Escribe un numero:\n";
    uint16_t x;
    uart >> x;
    uart << "Has escrito [" << x << "]\n";
 

## <a name="staticono"></a>Static interface vs objects

Los traductores son traductores de dispositivos de hardware, no teniendo
estado. Por ello desde el principio he optado por implementarlos como meros
interfaces.

Los drivers (= traductor al que le he añadido más funcionalidad) pueden o no
tener estado. De momento la mayoría no lo tienen, definiendolos como
interfaces (y si tienen estado, como el dispositivo de hardware es único se
puede definir dicho estado como static no necesitando crear un objeto de esa
clase).

La duda desde el principio siempre ha sido si dejar al usuario crear o no
estos interfaces como objetos. Al carecer de experiencia programando
aplicaciones reales desconozco qué puede ser mejor.

Por el momento, basándome en mi poca experiencia (traducción: cuando tenga más
experiencia puede que esto lo cambie radicalmente) me resulta muy cómodo que
el harwador defina en `hwd_dev.h` los elementos de hardware que va a usar el
sofwador en la aplicación. Como son elementos de hardware solo va haber una
instancia de cada uno de ellos y por ello se pueden definir como interfaces. 


## <a name="conexionSPI"></a>Problemas al programar el atmega usando SPI

Al conectar por primera vez una SD card al SPI el programador falla. ¿Por qué?
El avr se programa usando SPI con lo que se puede generar un SPI driver
contention entre el programador y la SD card. La application note AVR910 da la
solución: para evitar este problema poner una resistencia en serie
entre las líneas SCK/MISO/MOSI de la SD card y el avr. Al probar con 10k
funciona.

Si se quiere dejar el producto final con la posibilidad de reprogramarlo hay
que dejar esas resistencias. 


## <a name="cristalexterno"></a>Cristales externos

### Usar cristal externo
Para seleccionar el reloj, tenemos que elegir el LFUSE. Opciones de interés:

* LFUSE = 0xF7: conectar cristal externo con 2 condensadores de 12 a 22 pF en
  los pines XTAL1/XTAL2. (full swing oscillator)

* LFUSE = 0xB7: misma opción que antes, pero además sacamos por el pin CLKO
  (pin número 14) la señal del reloj.

* LFUSE = 0x22: micro a 1 MHz sacando la señal de reloj por el pin CLKO.

* LFUSE = 0xA2: micro a 8 MHz sacando la señal de reloj por el pin CLKO.

No olvidar definir `F_CPU` a la frecuencia correspondiente del cristal usado.

La figura 32-1 de la datasheet muestra las frecuencias máximas a las que puede
operar el atmega32 dependiendo de Vcc:

+ A 3.3V la frecuencia máxima son 12MHz.
+ A 5V es de 20MHz.


### Errores iniciales

La primera vez que quise usar el "full swing oscillator" probé LFUSE = 0xE7,
luego me resultó imposible conectarme con el micro a través de avrdude. 


## <a name="timers"></a>Timers

### Hardware vs uso

Conviene distinguir entre el hardware que suministra el `avr` y el uso que
vamos a dar de dicho hardware. 

¿Cómo podemos usar el Timer del `avr`?

1. Pulse counter: el Timer 1 lo podemos conectar externamente y medir los
   pulsos. ¿Serviría para contar objetos? Probablemente, pero todavía no he
   jugado con esta opción.

2. Timer counter: podemos contar tiempo. En este caso lo que contamos son los
   pulsos de un reloj. Esto es lo que uso para luego implementar `Miniclock` y
   `Clock`.

3. SWG pin: podemos usarlo para generar una onda cuadrada en un pin.

4. PWM pin: o podemos generar una señal PWM.

### Restriciones impuestas por el hardware

*Timer0: PWM pin*

En este timer tenemos que elegir:

+ Generar 2 señales de dutys cycle diferentes en los pines A y B, pero
  imponiendo serias limitaciones a las frecuencias generadas. Solo podemos
  generar las frecuencias `f_clock / (N * 256)` ó `f_clock / (N * 510)`,
  siendo N el prescaler (1, 8, 64, 256 ó 1024).

+ Generar 1 señal PWM de "cualquier" frecuencia y cualquier duty cycle. La
  señal se generaría en el pin B. En el pin A se puede obtener una señal
  cuadrada.


*Timer1: PWM pin*

Podemos generar una señal PWM en los dos pines del timer de cualquier duty
cycle pero de igual frecuencia. El hardware no permite generar dos frecuencias 
diferentes en el pin A y B a la vez.


 
