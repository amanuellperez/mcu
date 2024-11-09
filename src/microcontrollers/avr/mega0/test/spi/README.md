## ¿Cómo probar el traductor de SPI?

Se necesitan:

* 2 `atmega4809` de 40 pins (o el avr que se quiera probar).

* 2 cables FTDI para conectar cada micro a un puerto del ordenador.

* 1 programador, en mi caso el UPDI Friend de Adafruit.


Para controlar los puertos USB a los que se conecta cada cable conectarlos en
un orden. Por ejemplo: primero conecto el programador (va a `/dev/ttyUSB0`) y
luego los dos cables FTDI (`/dev/ttyUSB1` y `/dev/ttyUSB2`).

Alimento los microcontroladores a través del programador, dejando los cables
de alimentación de los FTDI sin conectar (no olvidar conectar los GND, sino no
funcionaría).

Abrir 3 terminales: 

* Uno para programar los micros via el programador.

* Y los otros dos para interaccionar con los micros. Para ello, ejecutar en
  cada terminal:
```
myterm --port /dev/ttyUSB1  
myterm --port /dev/ttyUSB2
```

[Esta foto](spi_test_breadboard.jpg) es un ejemplo de conexiones.

 



