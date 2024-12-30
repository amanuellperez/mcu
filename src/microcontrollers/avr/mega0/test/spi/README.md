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

[Esta foto](spi_test_breadboard.jpg) es un ejemplo de conexiones. Para depurar
he comprado un logic analyzer en Aliexpress (unos 5 euros) y, de momento, 
da la impresión de que funciona bien. Eso sí: al tener un dispositivo más que
conectar me ha generado el problema de que me faltan puertos USB en el
ordenador. [Esta otra foto](spi_test_breadboard02.jpg) muestra todo conectado.

Para averiguar el puerto USB al que está conectado ejecutar: `$ sudo dmesg -w`
y conectar el dispositivo. 

 



