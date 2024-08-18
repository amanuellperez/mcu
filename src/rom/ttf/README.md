
## Tipos de letra

Parece ser que hay un montón de fuentes de libre uso que se pueden descargar y
usar. Por ello, en lugar de dedicarle tiempo a dibujarlas, opto por reutilizar
el trabajo de otros (máxime que de momento no estoy interesado en diseñar
letras, sino en programar).

¿De dónde descargar las fuentes? 

## FreeFont

[FreeFont](https://www.gnu.org/software/freefont/index.html) es un proyecto
donde se pueden encontrar un montón de fuentes gratuitas. La
[licencia](https://www.gnu.org/software/freefont/license.html) indica que son
letras que se pueden usar libremente.

El problema de las fuentes que he cotilleado es que no son tipo bitmap y
aunque jugando con el `font size` consigo generar fuentes para el display
decentes hay que retocarlas. 

## Dafont

La página de [dafont/bitmaps](https://www.dafont.com/bitmap.php) contiene un
montón de letras para descargar. Supuestamente muchas son gratuitas, pero hay
que ir leyendo las licencias. Las que he descargado indican ser gratuitas y
libres de uso.  Si alguien observa que alguna no es de libre uso agradecería lo
indicase para borrarlas. 

La ventaja de dafont sobre FreeFont es que aquí sí que se pueden encontrar
fuentes bitmap, ideales para convertirlas en matrices de bits.

Las siguientes fuentes están descargadas de la página de dafont. He descargado
las primeras que me han salido para hacer pruebas. 

Font name       | Autor             | font size | License
----------------|-------------------|-----------|--------
Minecraft       | Craftron Gaming   | 16px      | Free
VCR OSD Mono    | Riciery Leal      | 21px      | Free
Rainy Hearts    | Camelina          | 16px      | Free
Dogica          | Roberto Mocci     |  8px      | Public domain/GPL
Upheaval        | AEnigma           | 20px      | Free
Retro Gaming    | Daymarius         | 11px      | Free
Alagard         | Hewett Tsoi      | 16px      | Free
Home Video      | GGBotNet          | 20px      | Free
DePixel         | ingoFonts         |  9px      | Free
Mai 10          | J. Font           | 10px      | Free
Perfect DOS VGA 437| Zeh Fernando   | 16px      | Free

Voy a dejar en github tanto el `ttf` de la fuente como el `txt` generado con
el tamaño indicado. En el `txt` se ve perfectamente el tipo de letra y cómo va
a quedar (de hecho al mirar el `txt` en github se ve fenomenal el tipo de
letra que se generará).

Curiosamente lo que estoy haciendo se podría usar para ampliar el
tipo de letras para programas de linux tipo `banner`.


## Conversión de `ttf` a `cpp`
Para convertir las letras en el map correspondiente de bytes para usar en los
displays uso los programas `ttf2txt.py` y `ttf_txt2bin.py` del proyecto
`mcu_pc_tools`.

El primer programa `ttf2txt.py` convierte el fichero `.ttf` en un fichero de
texto con las letras. Este fichero se puede modificar a mano y luego pasar al
script `ttf_txt2bin.py` para convertirlo en código `.cpp`

