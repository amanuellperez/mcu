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
* [Problemas al programar el atmega usando SPI](#conexionSPI)
* [Cristales externos](#cristalexterno)

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

     if (UART::is_ready_to_transmit()) { haz_algo(); }
     
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
los dispositivos del mismo tipo. 

Ejemplo: mi aplicación necesita un Timer, el cual es un dispositivo con el que
puedo realizar ciertas operaciones. El timer concreto puede ser
atmega::Timer0_g, o attiny::Timer0_g, o PIC::Timer0_g, 
o ESP::Timer0_g, pero sea cual sea el elegido todos representan el mismo
dispositivo lógico (el mismo concept de C++). La ventaja de esto es que mi
programa funcionará independientemente del Timer real elegido. La desventaja
es que al usar un interfaz genérico puede que no pueda incluir algunas
características particulares del Timer real que estoy usando. Pero si
necesitara usar esas características en lugar de usar atmega::Timer0_g
usaría atmega::Timer0_basic (el traductor que suministra todas las funciones
del dispositivo).

### Notación

De momento pruebo con la siguiente notación:

* `atmega_timer0_basic.h`: contiene el traductor del `Timer0` del `atmega`

* `atmega_timer0_g.h`: contiene el dispositivo genérico correspondiente.

Al dispositivo generic lo llamaré `atmega::Timer0_g`. El nombre del
traductor no lo tengo claro: ¿`atmega::Timer0` o `atmega::Timer0_basic`?

De momento optó más por el segundo estilo, ya que entre `atmega::Pin` y
`atmega::Pin_basic` el segundo no me gusta nada. Además que es bastante claro
que si escribes `atmega::Timer0` es porque vas a usar el `Timer0` y no un
dispositivo genérico.



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
    avr::cfg_basica(uart);      // esta es la configuración por defecto que siempre uso. Puedes usar otra.
    uart.on();
    
    uart << "Escribe un numero:\n";
    uint16_t x;
    uart >> x;
    uart << "Has escrito [" << x << "]\n";
 


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



Compilador: avr-gcc 11.3.0
 
