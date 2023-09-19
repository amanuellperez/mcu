# `basic_clock`
Este es el reloj más básico que se puede hacer. Es un micro más un terminal
que muestra la hora y tiene un teclado que permite ponerlo en hora.

## Terminales

Dos terminales básicos:

1. PC terminal:

   Consiste en usar el PC como terminal. Programar usando el PC como terminal
   simplifica el primer prototipo del programa ya que nos podemos centrar en
   la lógica y no en la presentación.

   Las versiones que se conecten al PC las llamaré `version_pc`

2. LCD + keyboard:

   En el caso del reloj con un teclado de 3 botones es suficiente para
   poner/cambiar la hora.

   Las versiones que se conecten al PC las llamaré `version_lcd`

3. Pantalla táctil:

   La ventaja de estas pantallas es que integran el teclado dentro de ellas.
   La desventaja es que son más caras que la versión LCD + keyboard.


### Problema

¿Podemos fusionar los dos programas anteriores? Estaría bien poder escribir el
programa para el PC y luego, simplemente cambiando el tipo de terminal que el
mismo programa sirva para el LCD + keyboard. 

¿Podemos definir `cout` y `cin`? En el PC es bastante sencillo, ya que tanto
`cout` como `cin` serían instancias de `uart`. En el keyboard `cin` se puede
definir (creo) que fácilmente, pero el problema está en cómo definir `cout`
para el LCD ya que son pantallas distintas: en `cout` vas escribiendo y cuando
se llega al final de la pantalla hace un scroll hacia arriba de toda la pantalla
para dejar espacio para la siguiente línea, pero en el LCD (para mostrar la
hora) usamos el LCD como una `screen`, una pantalla donde podemos elegir la
posición `(x, y)` donde escribir un caracter y que no hace scroll al escribir
el último caracter de la última línea de la pantalla. Son 2 ideas distintas la
de `cout` y la de `LCD`: `cout` es una especie de papiro, mientras que `LCD`
es una hoja de papel cuadriculada.




## Versión RTC timer1
Esta es la primera versión que se ocurre hacer: si quieres hacer un reloj lo
natural es coger el timer del microcontrolador y usarlo para hacer el reloj.

Lo conectas todo, lo programas, lo enciendes... y ¡ves que retrasa! (¡y en mi
caso retrasaba bastante!). Conclusión: reloj totalmente inútil.


### Problema
Al mirar los motivos por los que retrasa resulta que el "reloj" (= internal RC
oscillator) funciona de 7.3 a 8.1 Mhz, y por defecto, de fábrica viene
dividida la frecuencia entre 8 (ver "Fuse Low Byte", datasheet). Así que es
completamente normal que retrase y además, cada chip retrasará de una forma
diferente.

### Solución
¿Cómo conseguir que el reloj funcione practicamente a 1 MHz? En principio he
visto tres formas diferentes:
1. Parece ser que es posible calibrar el oscilador interno (registro OSCCAL?).
2. Se puede conectar a un cristal externo cambiando los fuses.
3. Usar el Timer2 en modo asíncrono, conectado a un cristal de 32kHz. De hecho
   parece ser que esta es la forma estandar de hacer un RTC con el atmega.

En la versión 0.1 optó por cambiar los fuses, mientras que en la versión 1.0
pruebo a usar el Timer2 en modo asíncrono.

## Versión 0.1
Para conseguir que el avr funcione prácticamente a 1 MHz basta con conectar un
cristal a los pines XTAL1, XTAL2. La datasheet llama a esto "Low Power Crystal
Oscillator". El cristal se conecta a tierra usando 2 condensadores cuyos
valores dependen de su frecuencia. Consultar la tabla 13-3 de la datasheet.
Además, en esta tabla junto con la 13-4 se indica cómo hay que cambiar los
fuses. 

Para que sea todo sencillo he modificado las reglas de compilación.
Escribiendo: `> make  set_external_crystal_8MHz` 
configura los fuses para que funcione con 8 MHz, dividiendo la frecuencia
entre 8. Como esto puede que varie en el futuro lo mejor es hacer `make help`y
ver qué opción es la correcta.

**Nota** Una vez que se configuran los fuses el avr usará el reloj externo. Si
no conectamos ningún cristal dejará de funcionar. Tampoco lo podremos
programar.

[Aquí](basic_clock.jpg) puedes encontrar una foto del resultado.

### Problemas

1. LCD: Mi objetivo es poder hacer un reloj vulgar y corriente, pero estos no
   usan los LCDs del tipo que usa todo el mundo en proyectos de arduino. Los
   LCDs de los relojes normales no emiten luz, sino que la reflejan.

2. Pilas: Usar 5 voltios para un reloj a día de hoy parece excesivo. Mi
   despertador usa una pila de 1'5 voltios. ¿Cómo alimentar el circuito solo
   con 1'5 voltios? 

   Para bajar el consumo del `avr` basta con dormir el microcontrolador y
   despertarlo con el Timer2 en modo asíncrono. El problema sigue en el LCD,
   que necesito un LCD que consuma poco.



## Versión RTC Timer2

El timer 2 del atmega está pensado para hacer un RTC. Tiene dos ventajas
frente al timer 0 y el timer 1:

1. Se puede conectar a un cristal externo de 32kHz consiguiendo de esta forma
   una medida más precisa del tiempo que con el timer 1.

2. Se puede dormir el micro usando el "power-save mode", lo que resuelve parte
   del problema que tenía con la versión del reloj anterior. 

Para hacer probar la lógica del programa es más rápido usar el PC como
terminal, comunicándonos a través del UART. 

### Problemas

1. En power-save mode hay que hacer un flush del UART sistemáticamente, sino
   te genera un montón de basura.

2. Ya que usamos el terminal del PC como interfaz de usuario parece lo más
   lógico pulsar una tecla para poner en hora el reloj. El problema es que en
   power-save mode no saltan las interrupciones de UART. Lo más sencillo es
   ponerle un pulsador que despierte el micro para ponerlo en hora.

### Evolución

A partir de este programa se puede escribir un data logger básico.

