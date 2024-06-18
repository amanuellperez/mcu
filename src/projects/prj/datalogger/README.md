# Data logger

## ¿Qué son?
Un data logger es un programa que se dedica a tomar medidas cada cierto
periodo de tiempo.

Las medidas pueden ser muy variadas: medir la temperatura, la presión, el
tanto por cien de humedad, la cantidad de polvo en la atmósfera, la cantidad
de CO2. ¿qué más? 

Pero en lugar de un sensor puedo conectar una cámara y hacer una foto
sistemáticamente lo cual permitiría hacer timelapses abriendo multitud de
posibilidades.



## Carpetas

### basic

Como RTC voy a usar el Timer2 del atmega conectado a un cristal externo.
Necesita poco hardware externo y da la impresión de que funciona bien. De
momento desarrollaré esta idea a ver hasta donde me lleva.

### experiments

Experimentos para ver diferentes formas de implementar un datalogger.

#### watchdog

Este es un experimento. Como RTC uso el watchdog del microcontrolador.
Funciona pero es poco preciso. Mucho mejor usar el Timer2 para medir el
tiempo.


## Basic datalogger

Un datalogger es un programa que se dedica a tomar medidas cada cierto tiempo.
¿Dónde guardar o mostrar las medidas?

1. PC datalogger:
   Conectamos el microcontrolador al PC y usamos el ordenador como interfaz de
   usuario.

2. Mini datalogger (nombre???):
   Guardamos todas las medidas en memoria, ya sea en una EEPROM o en una
   SD-card. 

   ¿Cómo configuramos y vemos los datos en este caso? En principio se me
   ocurren las siguientes alternativas:

   + Conectamos el dispositivo al PC para configurarlo y extraer los datos.

   + La configuración la hacemos por hardware, a través de switch pequeñitos.
     La lectura de datos usando el PC.

   Pero ¿cómo conectar el dispositivo al PC? La forma más sencilla, por estar
   implementada, es a través de UART. pero sería interesante conectarlo a
   través de wifi o internet. Eso permitiría poner múltiples dataloggers en
   un edificio y tomar las medidas sin moverse. 

3. Terminal datalogger:
   Suministramos un terminal junto con el datalogger para poder configurarlo y
   ver los datos en tiempo real sin necesidad de conectarlo al ordenador. Al
   tener que poner una pequeña pantalla y un miniteclado supongo que está
   opción hará que el dispositivo final sea el más voluminoso de todas las
   opciones dadas.


