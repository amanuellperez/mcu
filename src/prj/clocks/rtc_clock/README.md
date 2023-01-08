# RTC clock

En lugar de usar el timer del avr, como en el `basic_clock`, podemos conectar
el avr a un RTC. Para hacer pruebas he elegido el DS1307 (¿por qué? ni idea,
supongo que sería uno de los primeros que apareció en internet cuando empecé a
buscar).

La ventaja de usar un RTC en lugar del timer es precisamente liberar al avr
del control del tiempo, que lo hace todo el RTC. Además, si se le conecta una
pila de botón al RTC se puede desconectar todo el dispositivo manteniendo el
RTC encendido.


## Versión 0.0.0

Da la impresión de funcionar bien (cuando lo tenga meses encendido
podré opinar, aunque el RTC es de esperar que funcione bien). Al implementar
este reloj ya se observa que en todo proyecto vamos a tener que tener que
hacer mínimo 2 prototipos:

1. [Prototipo breadboard](0.0/rtc_clock_breadboard_v00.jpg):
   Montamos todo en la breadboard y probamos que el código funcione bien.

2. [Prototipo modular](0.0/rtc_clock_proto_v00.jpg):
   La idea es hacer un prototipo con el que poder experimentar. En este caso,
   como es un reloj, lo que quiero es dejar el reloj días o meses funcionando 
   para ver si retrasa o adelante. Si fuera en cambio un sensor para medir la
   altura a la que me encuentro, quiero poder llevarlo en el bolsillo para
   poder probarlo. Pero no busco un prototipo definitivo ya que puede tener
   errores de diseño (todavía es la versión 0.0 = versión de
   aprendizaje). Por ello voy a construirlo con partes modulares que pueda
   reutilizar en otros proyectos (en la foto se ve que el LCD con los 3
   botones lo puedo usar en otros proyectos).


### Problemas

A parte de los problemas que ya observé en el `basic_clock` (LCD radiante y
uso 7.5 voltios de alimentación) se ve que faltan cosas:
1. (corregido) No aparece el día de la semana, pero el RTC lo suministra.
2. (corregido) Al poner la hora no es posible pasar de 00 a 59. Si quieres poner 50
   minutos te toca ir desde 00 hasta 50, lo cual es incómodo.
3. (corregido) Con el LCD radiante sería interesante poder apagar el LCD.
   Añado un interruptor para apagar/encender tanto LCD como avr. Por ello
   cuando se inicializa el avr al inicializar el RTC mira a ver si está
   encendido. Si está encendido no pide la hora.
4. (corregido) Al dejar el reloj funcionando, si se prueba a apagar y encender 
   hay veces se enciende pidiendo que se ponga en hora. Otras veces arranca
   correctamente. ¿Qué es lo que pasaba? Parece ser que el botón OK hace
   bouncing. De hecho si antes de encenderlo le das un golpe y lo enciendes
   siempre arranca pidiendo que se ponga en hora. Para evitar este problema
   en lugar de usar un único botón uso dos para ponerlo en hora (OK y DOWN).


## Versión 0.0.1

Corregidos los problemas de la versión 0.0.0. 

La forma de apagar el LCD es directamente desconectar de corriente tanto LCD
como avr: desconectamos todo salvo el RTC.

Además para que el consumo sea mínimo se puede conectar todo a 2 pilas de 1'5
voltios, funcionando perfectamente.


### Problemas

1. Cada vez que hay que poner en hora el reloj la hora que aparece por defecto
   es la que trae el RTC: 01/01/2000. ¿Merecería la pena guardar en la RAM del
   RTC la última hora que se puso y mostrar esa hora al usuario para que la
   modidfique? Creo que esto no merece la pena implementarlo. Total el reloj
   lo pondrás de año en año en hora.

2. Aunque ya el consumo es mínimo, ya que se puede apagar todo salvo el RTC,
   no serviría para hacer un reloj de pared. Un reloj de pared está
   continuamente encendido mostrando la hora. En mi pared tengo uno que
   funciona con 1 pila de 1'5 voltios (aunque también tengo otro que funciona
   con 4 pilas de 1'5 voltios). Ambos usan LCD reflectantes lo cual es
   bastante cómodo. 
