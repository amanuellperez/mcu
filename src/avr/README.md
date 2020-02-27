### WARNING: This library is unstable. Maybe version 0.1???

Translators for avr.

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

TWI includes:

* `avr_TWI_basic.h`: translator of the datasheet. Everything you can do with 
the hardware. 
		    

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


Tested: avr-gcc 9.2.0
    
---
### ADVERTENCIA: Esta biblioteca es inestable. ¿Podría ser la versión 0.1?

Traductores del avr.

Uno de los problemas que te encuentras cuando empiezas a programar el microcontrolador
es que tienes que conocer perfectamente la arquitectura del hardware. Continuamente tienes
que estar leyendo la datasheet para recordar que bit tienes que poner a 1 para hacer
algo. Ese estilo de programar es propenso a errores. 

Para evitarlo creo los traductores:
en lugar de recordar que para mirar si UART está listo para transmitir tengo que mirar
si el bit `UDRE0` del registro `UCSR0A` es 1, basta con escribir 

     if (UART::is_ready_to_transmit()) { haz_algo(); }
     
Es mucho más legible, menos propenso a error y más fácil de portar. Además, los traductores
son muy sencillos de escribir: basta con leer la datasheet y escribir las funciones correspondientes.
La propia datasheet te da la mayoría de las veces el nombre de la función que tienes que escribir.

### `UART` como flujo
Además de los traductores suministro también algunas clases de más alto nivel. Por ejemplo, 
un flujo normal y corriente para acceder a UART. 

    avr::UART_iostream uart;
    avr::cfg_basica(uart);      // esta es la configuración por defecto que siempre uso. Puedes usar otra.
    uart.on();
    
    uart << "Escribe un numero:\n";
    uint16_t x;
    uart >> x;
    uart << "Has escrito [" << x << "]\n";
 

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

#### ¿iostream vs ioxtream?
¿Cuál es la diferencia entre un `iostream` y un `ioxtream`? Los `iostream`
clásicos de C++ son flujos de caracteres, mientras que un `ioxtream` es un flujo
de bytes (en TWI, SPI... son precisamente estos flujos los que nos interesan).


 Probado con: avr-gcc 9.2.0
 
