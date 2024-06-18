Some applications.

---

# Algunas aplicaciones.


## Principiantes
* [blink](blink): El "hola mundo" de los microcontroladores: hacer que un LED
  parpadee. 

* [POV toy](POV_toy): Es un POV display muy básico. De hecho es casi copia del
  ejemplo que trae el libro de MAKE. Es un buen proyecto para empezar amén de
  que el resultado es la mar de llamativo. 


## Útiles (directorio /util)
* [cost](util/cost): Programa que muestra los tamaños de los diferentes tipos
  usados por el avr. 

* [eeprom_read](util/eeprom_read): Hace un volcado de memoria de una EEPROM. Así
  podemos depurar y ver su contenido.

* [lcd_charset](util/lcd_charset): ¿Qué character set tiene un LCD? Esta
  aplicación muestra en la pantalla del LCD todos los caracteres que trae en
  la ROM, así se puede saber si es 'inglés/japonés', 'inglés/europeo' u otros.

* [TWI scan](util/TWI scan): ¿Qué dirección tiene un dispositivo TWI?
  Cuando se empieza a programar un nuevo TWI-device lo primero que hay que
  ver es que lo hayamos conectado correctamente y sepamos su dirección. Con
  esta aplicación podemos scanear el bus de TWI buscando el dispositivo.
  Incluso si no desconocemos la dirección del dispositivo podemos scanear todo
  el rango de direcciones posibles, averiguando su dirección.
  

## Un poco más elaboradas
* [cars](cars/README.md): Pruebas con coches de juguete.

* [clocks](clocks/README.md): Diferentes tipos de relojes.

* [calculator](calculator/README.md): Implementación de una calculadora
  básica.

* [datalogger](datalogger/README.md): Implementación de un datalogger.

* [IR_remote_control](IR_remote_control/README.md): Investigando el protocolo
  de los control remotos (por ejemplo, el mando de la tele).

* [music](music/README.md): jugando a tocar diferentes tonos en un altavoz. 

* [sdinspector](sdinspector/README.md): programa para ver el contenido de una
  SD-card (es un editor de bajo nivel de una SD-card)




# Estructura de los programas

El programa lo escriben dos personas: el hardwador, es quién sabe qué
dispositivos reales se van a usar, y el softwador que conoce qué dispositivos
hay pero desconoce cuáles son.


## Archivos que escribe el hardwador

1. `hwd_dev.h`

   Este archivo contiene las referencias a dispositivos reales
   que hay. Aquí es donde se encuentran cómo conectar el micro a los
   dispositivos. Se definen aquí también los dispositivos lógicos (o
   genéricos) que son los que usará el softwador.

   Este archivo sirve además para dejar documentado no solo los dispositivos
   usados sino cómo se conectan. 

2. `hwd_init.cpp`

   Los dispositivos y el micro hay que inicializarlos. El hardwador sabe si
   tiene que inicializar SPI, o llamar a `sleep_mode`, y cómo se inicializa
   cada dispositivo, ya que cada dispositivo real tiene su propia forma de
   inicializarse. 

3. `hwd_interrupts.cpp` (DUDA: ¿mejor `prj_isr.cpp`?)

   En este fichero se definen todas las ISRs que necesite el programa.


## Archivos que escribe el softwador

El softwador escribe el resto. 

1. 'prj_main.h`

   Aquí voy a definir la clase `Main`. Uso esta clase para tener dentro de
   ella todas las variables globales que se necesiten. Esta clase está
   definida a medias entre el hardwador y el softwador.

   ¿Es necesaria la clase `Main`? En principio no, pero tiene la ventaja de
   que me permite identificar código genérico del particular al proyecto. 

   Es habitual que cuando escribes un programa necesites funciones que
   genéricas que no necesitan saber nada del programa que se está escribiendo.
   Esas funciones no las meto dentro de la clase `Main`, metiendo sus
   prototipos en otro archivo de cabecera que no empieza por `prj_` sino por
   otro prefijo. De esa forma cuando se acaba el programa se puede revisar
   fácilmente qué código no depende del programa. Si más adelante ese código
   se necesita en otro programa es momento de generalizarlo, lo cual es
   sencillo de hacer. Ese es uno de los motivos por los que uso la clase
   `Main` aunque realmente no es necesaria.


2. `prj_strings.pgm`

   Contiene las cadenas que se van a guardar en la progmem. Observar que no es
   un `.h` ya que he creado un script que transforma ese fichero en el `.h`
   correspondiente. Para hacerlo basta con añadir la siguiente regla de
   compilación al `makefile`:
   ```
	prj_strings.h: prj_strings.pgm
		str2progmem prj_strings.pgm
   ```

   Si lo uso supongo que lo acabaré metiendo como una regla genérica de
   compilación. A día de hoy hay que escribirlo a mano.

   Usar estos archivos tiene (casi) una ventaja añadida: que si se quiere
   traducir el programa del español al inglés basta con traducir este fichero. 
   Digo "casi" ya que aquellas frases que lleven parámetros la traducción
   puede que falle por culpa del orden de los parámetros. Se podría mejorar
   esto haciendo que lo que se guarda en el `.pgm` sea el formato tipo
   `printf` de lo que se quiere mostrar. 

   Ejemplo (SIN IMPLEMENTAR):  en `strings.pgm` escribimos `msg0 "El coche %s"` en español,
   mientras que en inglés escribiríamos `msg0 "The %s car"`. El script tendría
   que generar no solo la cadena de progmem, tal como hace ahora, sino que
   además la función necesaria para escribir el mensaje. De esa forma
   evitaríamos errores. La salida en el caso del mensaje en español sería:
   ```
   constexpr mcu::Progmem_string<10> msg0
	PROGMEM{"El coche "};
   void print_msg0(std::ostream& out, const char* s)
   {
        atd::print(out, msg0);
        atd::print(out, s);
   }

   ```

   Mientras que en el caso del mensaje en inglés el script generaría el código
   siguiente:
   ```
   constexpr mcu::Progmem_string<5> msg0_0 PROGMEM{"The "};
   constexpr mcu::Progmem_string<4> msg0_1 PROGMEM{" car"};
   void print_msg0(std::ostream& out, const char* s)
   {
        atd::print(out, msg0_0);
        atd::print(out, s);
        atd::print(out, msg0_1);
   }

   ```

    De esta forma el softwador llamaría a `print_msg0(uart, color);` y
    quedaría escrito en el idioma que correspondería correctamente.




## ¿Por qué usar clase `Main`?

Desde el principio he estado usando una clase `Main` donde defino todas las
funciones necesarias para la aplicación. Esta clase realmente no es necesaria
y se podría eliminar. 

Motivos por los que la mantengo:

1. Permite identificar más fácilmente código genérico. Todas las funciones que
   no pertenezcan a la aplicación no las voy a escribir dentro de `Main`
   siendo más fácil luego localizarlas para generalizarlas.

2. Dentro de `Main` meto todos los dispositivos que necesito: tengo la noción
   de tener más control sobre la construcción e inicialización de los devices
   usados. (aunque esto realmente no es un motivo de peso por el que usar
   `Main`).


## Esto es experimental

Por supuesto, como esto es experimental puede que sean necesarios más o menos
archivos o que esta forma de concebir los proyectos no sea correcta. 

Y recordar que la programación está pensada por capas: si se necesita hacer
cosas especiales o raras usar directamente los traductores que permiten usar
al máximo las capacidades de los dispositivos. Los interfaces genéricos tienen
la ventaja de ser más sencillos de manejar y ser intercambiables pero el pago
a hacer es no tener acceso a todas las capacidades del dispositivo.


