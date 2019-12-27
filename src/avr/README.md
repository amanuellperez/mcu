WARNING: This library is unstable. Maybe version 0.1???

Translators for avr.

One of the problems programming a microcontroller is that you need to know the architecture 
of the microcontroller. You have to read the datasheet to remember which bit you have to
write one to make the microcontroller to do something. 

To avoid it I use translators: instead of remember the datasheet is easier to write

     if (USART::is_ready_to_transmit()) { do_something(); }
     
everybody can understand this code. 

Besides translators I provide another classes. For instance, I treat UART as a normal
iostream:

    avr::UART_iostream uart;
    avr::cfg_basica(uart);  // This is my configuration. You can use another.
    uart.on();
    
    uart << "Write a number:\n";
    uint16_t x;
    uart >> x;
    uart << "x = [" << x << "]\n";
    
Tested: avr-gcc 7.4.0
    
---
ADVERTENCIA: Esta biblioteca es inestable. ¿Podría ser la versión 0.1?

Traductores del avr.

Uno de los problemas que te encuentras cuando empiezas a programar el microcontrolador
es que tienes que conocer perfectamente la arquitectura del hardware. Continuamente tienes
que estar leyendo la datasheet para recordar que bit tienes que poner a 1 para hacer
algo. Ese estilo de programar es propenso a errores. 

Para evitarlo creo los traductores:
en lugar de recordar que para mirar si USART está listo para transmitir tengo que mirar
si el bit UDRE0 del registro UCSR0A es 1, basta con escribir 

     if (USART::is_ready_to_transmit()) { haz_algo(); }
     
Es mucho más legible, menos propenso a error y más fácil de portar. Además, los traductores
son muy sencillos de escribir: basta con leer la datasheet y escribir las funciones correspondientes.
La propia datasheet te da la mayoría de las veces el nombre de la función que tienes que escribir.

Además de los traductores suministro también algunas clases de más alto nivel. Por ejemplo, 
un flujo normal y corriente para acceder a UART. 

    avr::UART_iostream uart;
    avr::cfg_basica(uart);      // esta es la configuración por defecto que siempre uso. Puedes usar otra.
    uart.on();
    
    uart << "Escribe un numero:\n";
    uint16_t x;
    uart >> x;
    uart << "Has escrito [" << x << "]\n";
    
 Probado con: avr-gcc 7.4.0
 
