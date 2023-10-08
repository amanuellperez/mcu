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


