### WARNING: unstable library. Some of the files may not compile.

Drivers for some devices.

Read the spanish description.

TODO: write this in english xD

Tested: avr-gcc 7.4.0

---

### ADVERTENCIA: Es inestable. Algunos de los ficheros puede que ni compilen.

Dispositivos a los que accedemos desde el microcontrolador.

La mejor forma de ver cómo funciona es mirar los test.

Como este paquete se basa en avr y avr lo he ido cambiando a medida que voy ganando
experiencia los primeros ficheros que hace tiempo que no uso puede que no compilen.
No los borro ya que con simples cambios seguramente se pueden actualizar.

Los dispositivos que fijo que funcionan (por lo menos a día de hoy) son:

### EEPROM 25LC256

Formado por varios ficheros:

* `dev_EEPROM_25LC256_tr.h`:   es el traductor de la EEPROM 25LC256. 
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




### LCD HD44780

Es el típico LCD 16 x 2 ó 20 x 4.

Formado por varios ficheros:

* `dev_LCD_HD44780_tr.h`: es el traductor del dispositivo.
* `dev_LCD_HD44780_screen.h`: concebimos el LCD como terminal.
   Un terminal es un terminal vulgar y corriente: vas escribiendo en él. 
   Cuando llegas a la última columna automáticamente pasa a la siguiente 
   fila, y si esta fuera la última fila tira toda lo escrito hacia arriba
   y continuas escribiendo.
   
* `dev_LCD_HD44780_ostream.h`: concebimos el LCD como ostream.
   Por defecto se suministran LCD de 16x2 y de 20x04: `LCD_HD44780_1602_ostream`
   y `LCD_HD44780_2004_ostream`.
   
   Ejemplo: (ver `test/HD44780`)
```
LCD_HD4470_1602_ostream lcd{LCD_H44780::DPin_RS{4}
                            , LCD_HD44780::DPin_RW{5}
                            , LCD_HD44780::DPin_E{6}
                            , LCD_HD44780::DPin_D4{11, 12, 13, 14}};
    
lcd.clear();
lcd << "hola\n"; // escribo cadenas
lcd << 25;       // escribo números
```



Probado con: avr-gcc 7.4.0
