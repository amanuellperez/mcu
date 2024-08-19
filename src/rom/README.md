# Ficheros genéricos para guardar en la ROM

Aqui voy a meter todo lo que sea genérico que guardemos en la ROM (la progmem
del avr).

Ejemplos: tipos de letra, constantes matemáticas ...

Es importante (???) que solo se puedan incluir los tipos de letra usados y no
todo. Supuestamente el compilador debería de optimizar las variables no usadas
y no guardarlas en memoria, pero al no haber conseguido ninguna referencia
optó por intentar que el programa incluya solo aquello que tiene intención de
usar (<-- en la práctica parece que funciona correctamente y solo incluye
aquellas variables que se usan)


Todavía no se cómo generalizar la gestión de la memoria usando C++, así que
usemos macros:

1. Si el compilador cambia el nombre de la variable PROGMEM o no la usa en
   otros micros, basta con definirla en la biblioteca correspondiente del
   micro.

2. La única función que necesito no genérica es `avr::ROM_read` que depende
   del micro. Como en los makefiles estamos definiendo la variable `MCU` basta
   con dejarla como `MCU::ROM_read`.

Problema: ¿cómo generalizar esto sin necesidad de usar macros?


### Otra posible solución

En lugar de distribuir este archivo distribuimos un archivo de texto con 
los gliphs.

Ejemplo:
```
bell = 0b00000
       0b00010 ...
```

fichero que llamamos `glyphs.rom`.  A partir de este fichero usando un script
generamos este archivo de cabecera:

```
$ rom2cpp atmega32 glyphs.rom
```
que genera el fichero adecuado dependiendo de la arquitectura usada.

