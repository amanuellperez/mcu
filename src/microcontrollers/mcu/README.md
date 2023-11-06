# GENERIC MICROCONTROLLER

Read the spanish description or look the directory `test`.


### TWI_master_ioxtream

```
#include <avr_TWI_basic.h>
#include <avr_TWI_master_ioxtream.h>

constexpr uint8_t TWI_buffer_size = 100;
using TWI = avr::TWI_master_ioxtream<avr::TWI_basic, TWI_buffer_size>;
constexpr uint8_t slave_address = 0x10;

void service(const Data_in& in, Data_out& out)
{
    TWI twi;
    twi.open(slave_address);
    
    twi << in;

    if (twi.error()){
	uart << "Error: ";
	twi_print_error();
	return;
    }

    twi.read(Data::size()); // TODO: how to improve this???
    twi >> out;

    twi.close();

    if (twi.error())
	twi_print_error();
}

// Don't forget the ISR!!!
ISR(TWI_vect)
{
    TWI::handle_interrupt();
}
```

The problem with avr TWI is that its buffer is only of 1 byte. I change this
behaviour by software. 3 different configurations are given:

* `avr_TWI_master.h`: TWI with buffer. Only works as a master.
* `avr_TWI_slave.h`: TWI with buffer. Only works as a slave.
* `avr_TWI_multimaster.h`: TWI with buffer that can work as a master or slave.
(TODO).

But I don't want to remember the TWI protocol. It would be great if I can treat
TWI as a normal `iostream`:

* `avr_TWI_master_ioxtream.h`: TWI works as an `ioxtream`. Only works as a
  master.

#### ¿iostream vs ioxtream?
What is the difference between an `iostream` and an `ioxtream`? `iostreams` are
character streams, but `ioxtreams` are bytes streams. I want to send `uint16_t x
= 500` as two byts '0x01F4` and not as 3 characters '5', '0', '0'.


Compiler: avr-gcc 11.3.0

---

# Capa `mcu`

Esta capa es totalmente genérica NO PUDIENDO DEPENDER de ningún
microcontrolador concreto. Tiene que poder usarse tanto con `attiny`,
`atmega`, `pic`, `8052`, ...


Incluso algunas partes de esta capa, como `UART_iostream`, las voy a usar 
dentro de los tests de `avr`.



## Comentarios sobre algunas implementaciones

En principio implemento:

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


### Protocolos

* 1-wire de Maxim

* 2-wire (TWI o I2C).

  La implementación está pensada por capas. Cuanto más elaborada es la capa
  más sencilla de manejar y más robusta, pero también más ineficiente. Si
  buscas eficiencia usa directamente el traductor del microcontrolador.

  `TWI_master` añade un buffer al dispositivo TWI.
  [Aquí](doc/TWI_master_state_diagram_v0.0.jpg) se puede encontrar la versión
  0 del diagrama de estados que implementa esta clase (solo tiene interés si
  quieres ver la implementación de la clase, no se necesita para usarla).


### TWI_master_ioxtream
Si se quiere usar TWI como master concibiéndolo como un flujo usar
`avr::TWI_master_ioxtream`.

```
#include <avr_TWI_basic.h>
#include <avr_TWI_master_ioxtream.h>

constexpr uint8_t TWI_buffer_size = 100;
using TWI = avr::TWI_master_ioxtream<avr::TWI_basic, TWI_buffer_size>;
constexpr uint8_t slave_address = 0x10;

void service(const Data_in& in, Data_out& out)
{
    TWI twi;
    twi.open(slave_address);
    
    twi << in;

    if (twi.error()){
	uart << "Error: ";
	twi_print_error();
	return;
    }

    twi.read(Data::size()); // TODO: how to improve this???
    twi >> out;

    twi.close();

    if (twi.error())
	twi_print_error();
}

// No olvides definir la ISR
ISR(TWI_vect)
{
    TWI::handle_interrupt();
}

```

El paquete TWI incluye:

* `avr_TWI_basic.h`: traductor de la datasheet.  Son todas las funciones que 
  tiene el hardware de TWI.
		    

El problema que tiene el TWI del avr es que tiene un buffer interno de 1 byte. 
Por software modifico esto suministrando un TWI con un buffer interno más
grande. Aprovecho y separo TWI en 3 diferentes configuraciones:

* `avr_TWI_master.h`: TWI con buffer que funciona únicamente como master.
* `avr_TWI_slave.h`: TWI con buffer que funciona únicamente como slave.
* `avr_TWI_multimaster.h`: TWI con buffer que funciona como master o slave.
(TODO: no está implementado de momento)

Pero es un rollo tener que andar recordando el protocolo de TWI. ¿No es mejor
concebirlo como un `iostream` vulgar y corriente? De eso se encarga:
* `avr_TWI_master_ioxtream.h`: concebimos TWI como un `ioxtream`. Este solo
  funciona como master. 

##### ¿iostream vs ioxtream?
¿Cuál es la diferencia entre un `iostream` y un `ioxtream`? Los `iostream`
clásicos de C++ son flujos de caracteres, mientras que un `ioxtream` es un flujo
de bytes (en TWI, SPI... son precisamente estos flujos los que nos interesan).


Compilador: avr-gcc 11.3.0
