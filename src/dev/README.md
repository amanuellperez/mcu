# WARNING: unstable library. Some of the files may not compile.

Drivers for some devices. 

Read the spanish description or look the directory `test`.

TODO: write this in english xD

Tested: avr-gcc 9.2.0

---

# ADVERTENCIA: Es inestable. Algunos de los ficheros puede que ni compilen.

Dispositivos a los que accedemos desde el microcontrolador.

A día de hoy están implementados:
* EEPROM:
  25LC256

* Sensores:
  + BME280
  + BMP280
  + DHT22 (revisar)
  + TMP36 (revisar)

* RTC:
  + DS1307

* Counters:
  + CD4017B


* Pantallas:
  + LCD: HD44780
  + Matrix LED: MAX7219

* Potenciómetro digital:
  + MCP4231

* Registers:
  + 74HC595: 8-bit serial in, parallel-out shift.


A parte de los dispositivos incluyo interfaces genéricos:


* Dispositivos genéricos:
  + `signal_generator`
  + `system_clock`
  + `keyboard`


* user: interacción con el usuario a través de LCD y Keyboard.
  + `user_choose_number`
  + `user_choose_string`
  + `user_time`


La mejor forma de ver cómo funciona es mirar los test.

Como este paquete se basa en avr y avr lo he ido cambiando a medida que voy ganando
experiencia los primeros ficheros que hace tiempo que no uso puede que no compilen.
No los borro ya que con simples cambios seguramente se pueden actualizar.

Los dispositivos que fijo que funcionan (por lo menos a día de hoy) son:

## EEPROM 25LC256

Formado por varios ficheros:

* `dev_EEPROM_25LC256_basic.h`:   es el traductor de la EEPROM 25LC256. 
* `dev_EEPROM_lineal.h`: concebimos la memoria como lineal.

* `dev_EEPROM_iostream.h`: concebimos la memoria como un flujo normal.
   
   Ejemplo: (ver `test/EEPROM_iostream/`)
```
EEPROM_ostream out;
out.open(address);
out << "Escribo esto en la memoria";
out.close();
```
  Hay que definir `EEPROM_ostream`. Consultar el test correspondiente (`EEPROM_iostream/interactivo`).




## LCD HD44780

Es el típico LCD 16 x 2 ó 20 x 4.

Formado por varios ficheros:

* `dev_LCD_HD44780_basic.h`: es el traductor del dispositivo.
* `dev_LCD_screen.h`: concebimos el LCD como terminal.
   Un terminal es un terminal vulgar y corriente: vas escribiendo en él. 
   Cuando llegas a la última columna automáticamente pasa a la siguiente 
   fila, y si esta fuera la última fila tira todo lo escrito hacia arriba
   y continuas escribiendo.
   
* `dev_LCD_ostream.h`: concebimos el LCD como ostream.
   Por defecto se suministran LCD de 16x2 y de 20x04: `LCD_ostream_1602`
   y `LCD_ostream_2004`.
   
   Ejemplo: (ver `test/LCD`)
```
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;

using LCD_ostream = dev::LCD_ostream_1602<LCD_HD44780>;
 
LCD_ostream lcd;

lcd.clear();
lcd << "hola\n"; // escribo cadenas
lcd << 25;       // escribo números
```

### Tipos de LCDs

La diferencia fundamental entre los LCDs son:
1. El driver que usan. De momento el único que tengo implementado es el HD44780.
2. El tamaño del display: 16 x 02 (2 filas x 16 columnas), 20 x 04, 40 x 04.
3. El potencial al que trabajan: a 5V, a 3'3 V.
4. El tipo de pantalla: puede estar iluminada o ser reflectante.
5. El color. La mayoría son monocolores pero se pueden encontrar multicolores.
   Además el fondo puede ser azul, verde...
6. El tipo de caracteres que trae: puede ser inglés/japonés; inglés/europeo y
   hay más (ruso por ejemplo).


### Algunas referencias:
* 1602A: el típico que usa la gente de Arduino. Es de 16 x 02. Los hay a 5 V y
  también a 3'3 V.
* GFC1602M: de 16 x 02, a 5 V, pero reflectante.
* 1602O: de 16 x 02, de 3'3 a 5 V, multicolor. El driver es el AIP31068, es
  I2C (TWI).
* 2004A: de 20 x 04. A 5 V.
* 4004A: LCD de gran tamaño, de 40 x 04.

En aliexpress se pueden encontrar los de la marca Surenoo. Desconozco si serán
buenos, pero para aprender a manejarlos y jugar con ellos funcionan bien. Son
bastante más baratos que los que vende Amazon, además que en Amazon no te
indican ninguna característica con lo que no sabes lo que estas comprando. 

### Vídeos
Estas son las pruebas de `LCD_screen` (los grabé con el móvil y han quedado la
mar de cutres, pero bueno son los primeros y no creo que nadie los vea):
* [test screen 16 x 02](https://youtu.be/Q0Fmtg7nzDE)
* [test screen 20 x 04](https://youtu.be/garxHAmSEDU) (este se ve fatal)
* [test screen 40 x 04](https://youtu.be/3GiaSXCdTn0)

### Documentación
* app note AN658 de Microchip: [LCD
  Fundamentals](https://www.microchip.com/wwwAppNotes/AppNotes.aspx?appnote=en011075)

Probado con: avr-gcc 9.2.0
