# PRIMEROS PASOS CON AVR

¿Quieres aprender a programar un microcontrolador directamente y no sabes qué
hacer?  Aquí explico cosas básicas a nivel práctico. En principio lo explicado
es para programar avrs.

### Primero: aprende a programar.

Los avrs los puedes programar en C o en C++. Lo primero que hay que tener
claro es que estos son dos lenguajes diferentes: yo sé programar en C++ pero
no sé C, y alguien que programe en C dudo mucho que diga que sabe programar en
C++.

Antes de empezar a programar un microcontrolador aprende uno de los dos
lenguajes. Yo recomiendo C++ aunque la comunidad de avr programa
mayoritariamente en C. 


### Libros

* Elliot Williams, "Make:AVR Programming". ISBN: 978-1-449-35578-4 

  En inglés. Enseña poco a poco construyendo proyectos asequibles. Programa
  en C. Ideal para empezar.

### Hardware básico

- El microcontrolador. 

- Un programador: 

  Yo compré uno de Baite en amazon por unos 5 euros y es el que uso.  Luego
  [encontré el mismo](https://es.aliexpress.com/item/577097498.html) en
  aliexpress por 1.19 euros

- Un cable de FTDI para poder conectar el ordenador a el microcontrolador al
  PC (muy útil).

  El que yo tengo es un FTDI CHIP TTL-232D-5V.

  Primero conseguí uno chino por 5 euros y al no funcionar bien opté
  por comprar el original.  Cuesta unos 25 euros pero funciona bien. Lo tengo
  ya de hace tiempo y no ha dado ningún problema.

- Breadboards.
  En general, las que venden en Amazon están bien (aunque en una ocasión
  compré de una marca china desconocida y tuve que tirarlas a la basura). Las
  de Elegoo funcionan bien.

- Cable para conectar las breadboards.
  El mejor es el AWG22, solid core. Es cable caro pero ideal para conectar en
  las breadboards.

- Pelador de cables:
  Dependiendo de lo que vayas ha hacer necesitarás un buen pelador de cables o
  no. Yo tengo el de Stanley, el FatMax. Funciona fenomenalmente. Con todo
  como siempre depende de lo que vayas a hacer.

- Un multímetro:
  Al principio para empezar cualquier multímetro vale. Yo ahora uso el
  AM-510-EUR de AMPROBE. Los hay mejores y los hay peores (y el precio va de
  acuerdo con ello). 


### ¿Dónde comprar?

#### Distribuidores profesionales de electrónica
* [Farnell](https://es.farnell.com/)
  
  Te lo sirven en 24 horas. Facturan directamente en España.

* [Mouser](https://www.mouser.es/)

  Tardan una semana ya que lo envían desde los Estados Unidos.  Facturan
  directamente desde España.

* [Digikey](https://www.digikey.es/)

  Tardan una semana ya que lo envían desde los Estados Unidos.  No deben de
  tener distribuidora en España así que hay que pagar las tasas en la aduana.



#### Intermediarios

* [Amazon](https://www.amazon.es)

  Amazon distribuye todo el producto de Elegoo. 
  
  Aunque ya no suele aparecer en
  los primeros productos, merece la pena buscar la marca Sodial: te tarda 1
  mes en llegar pero es barato (y supongo que la calidad irá de acuerdo
  con el precio, pero para aprender electrónica está bien ya que funciona su
  producto).


* [Aliexpres](https://es.aliexpress.com/)

  Producto chino. Pedir las cosas con tiempo, ya que tarda bastante. Amazon
  distribuye productos de marcas que se pueden encontrar en Aliexpres bastante
  más caro. 

* [Ebay](https://www.ebay.com/)

  Merece la pena cotillear.


### Marcas que parece que funcionan bien.

* [Adafruit](https://www.adafruit.com/)

* [Sparkfun](https://www.sparkfun.com/)

* [Elegoo](https://www.elegoo.com/)


### Breadboards, programadores...

#### Breadboards
Una vez que tenemos las breadboards conviene conectarlas. Si se van a usar
dispositivos TWI y SPI queda mucho más ordenado dejando raíles específicos
para estos protocolos. Una primera versión de cómo conectar la da la siguiente
[imagen](breadboard_ic2_spi_v1.0.jpg).



#### Programadores
¿Cómo conectar el programador al dispositivo avr? 

El libro de Make trae algunas sugerencias. Evolucionándolas yo, a día de hoy,
uso los siguientes programadores:
* [v1](programador_v1.4.0.jpg)
* [v2](programador_v2.1.0.jpg)



---
Manuel Perez (https://github.com/amanuellperez/mcu)

