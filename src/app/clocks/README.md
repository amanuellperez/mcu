# Clocks

Como los relojes los tenemos por todas partes no nos damos cuenta de la
importancia que tienen. Además hacer uno es la mar de instructivo, sobre todo
porque se pueden hacer de muy diferentes formas.

## Digital
¿Eres capaz de hacer un reloj directamente con electrónica digital? ¿De
cuántas formas diferentes puedes hacerlos?

No tengo fotos del que hice en su día. Si lo vuelvo a hacer, ya subo alguna.


## `basic_clock`
Este es el reloj más básico que se puede hacer. Consta de:

* Un LCD que nos muestra la hora.
* Un teclado de 3 letras para poder poner/cambiar la hora.

### Versión 0.0
Lo conectas todo, lo programas, lo enciendes... y ¡ves que retrasa! (¡y en mi
caso retrasaba bastante!). Conclusión: reloj totalmente inútil.


#### Problema
Al mirar los motivos por los que retrasa resulta que el "reloj" (= internal RC
oscillator) funciona de 7.3 a 8.1 Mhz, y por defecto, de fábrica viene
dividida la frecuencia entre 8 (ver "Fuse Low Byte", datasheet). Así que es
completamente normal que retrase y además, cada chip retrasará de una forma
diferente.

#### Solución
¿Cómo conseguir que el reloj funcione practicamente a 1 MHz? En principio he
visto dos formas diferentes:
1. Parece ser que es posible calibrar el oscilador interno (registro OSCCAL?).
2. Se puede conectar a un cristal externo cambiando los fuses.

En la versión 0.1 optó por cambiar los fuses.

### Versión 0.1
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

[Aquí](basic_clock/basic_clock.jpg) puedes encontrar una foto del resultado.

#### Problemas

1. LCD: Mi objetivo es poder hacer un reloj vulgar y corriente, pero estos no
   usan los LCDs del tipo que usa todo el mundo en proyectos de arduino. Los
   LCDs de los relojes normales no emiten luz, sino que la reflejan.

2. Pilas: Usar 5 voltios para un reloj a día de hoy parece excesivo. Mi
   despertador usa una pila de 1'5 voltios. ¿Cómo alimentar el circuito solo
   con 1'5 voltios? 


## RTC clock

En lugar de usar el timer del avr, como en el `basic_clock`, podemos conectar
el avr a un RTC. Para hacer pruebas he elegido el DS1307 (¿por qué? ni idea,
supongo que sería uno de los primeros que apareció en internet cuando empecé a
buscar).

La ventaja de usar un RTC en lugar del timer es precisamente liberar al avr
del control del tiempo, que lo hace todo el RTC. Además, si se le conecta una
pila de botón al RTC se puede desconectar todo el dispositivo manteniendo el
RTC encendido.


### Versión 0.0

Da la impresión de funcionar bien (cuando lo tenga meses encendido
podré opinar, aunque el RTC es de esperar que funcione bien). Al implementar
este reloj ya se observa que en todo proyecto vamos a tener que tener que
hacer mínimo 2 prototipos:

1. [Prototipo breadboard](rtc_clock/0.0/rtc_clock_breadboard_v00.jpg):
   Conectamos todo en la breadboard todo y vamos probando y programando todo.

2. [Prototipo modular](rtc_clock/0.0/rtc_clock_proto_v00.jpg):
   La idea es hacer un prototipo con el que poder experimentar. En este caso,
   como es un reloj, lo que quiero es dejar el reloj días o meses funcionando 
   para ver si retrasa o adelante. Si fuera un cambio un sensor para medir la
   altura a la que me encuentro, quiero poder llevarlo en el bolsillo para
   poder probarlo. Pero no busco un prototipo definitivo ya que puede tener
   errores de diseño (total, todavía es la versión 0.0 = versión de
   aprendizaje). Por ello voy a construirlo con partes modulares que pueda
   reutilizar en otros proyectos (en la foto se ve que el LCD con los 3
   botones lo puedo usar en otros proyectos).


#### Problemas

A parte de los problemas que ya observé en el `basic_clock` (LCD radiante y
uso 7.5 voltios de alimentación) se ve que faltan cosas:
1. No aparece el día de la semana, pero el RTC lo suministra.
2. Al poner la hora no es posible pasar de 00 a 59. Si quieres poner 50
   minutos te toca ir desde 00 hasta 50, lo cual es incómodo.
3. Con el LCD radiante sería interesante poder apagar el LCD.


