MCU++ library
=============
(If you understand spanish read better the spanish version of the readme
(scroll down this document))

Libraries and applications for microcontrollers (right now, only for
ATMEGA328).

# Toolchain
`binutils 2.42`
`avr-g++ 13.3.0`
`avr-libc 2.2.0`

# Directories

## src: source code.

This directory contains the following:

1. Generic things for microcontrolers and also for PC: (programming level?)

    * [std](src/std/README.md): (Partial) C++ standard library, 
      without exceptions nor memory allocations.
     
    * [atd](src/atd/README.md): General utilities.
      Depends on `std`.
 
2. [Microcontrollers](src/microcontrollers): (microcontroller level?)

    * [avr](src/microcontrollers/avr/README.md): "Drivers" for avr microcontrollers 
	(right now, only for ATMEGA328). 
      
    * [mcu](src/microcontrollers/mcu/README.md): Generic microcontrollers components.

3. [Devices](src/devices): (device level?)

    * [hwd](src/devices/hwd/README.md): Built-in devices.
      To program here you have to know your chip, 
      you need to read the datasheet.

    * [logic](src/devices/logic/README.md): User-defined devices.
    
  
4. [Projects](src/projects): 
    * [pli](src/projects/pli/README.md): Common libraries

    * [prj](src/projects/prj/README.md): Projects.


# mk: Compilation rules.

# test and pc_test:
All the packages have a directory `test` or `pc_test`. The difference between
them is that `pc_test` are tests to do in the computer (you have to compile
them with `gcc`) but `test` are tests to do in the microcontroller.

If you want to compile `pc_test` you need some headers files of the 
[alp](https://github.com/amanuellperez/alp/tree/master/src) project (most
probably you need to download `alp_test.h`, `alp_test.cpp` and maybe
`alp_string.h` and `alp_string.cpp`).



---
Bibliotecas y aplicaciones para microcontroladores (en principio solo el
ATMEGA328).

# Índice
* [Compilador y toolchain](#compilador)
* [Estructura de directorios](#directorios)
* [Configurar entorno de trabajo](#entorno)
* [Reglas para compilar](#reglas_compilar)
* [Tests](#test)
* [Documentación y ayuda](#documentacion)
* [Licencia GPL](#licencia)
* [Vídeos](#videos)
* [Tags usadas en el código](#tags)

## <a name="compilador"></a>Compilador y toolchain
`binutils 2.42`
`avr-g++ 13.3.0`
`avr-libc 2.2.0`

Voy a empezar a probar las nuevas características de C++20 en este proyecto
según vaya aprendiendo a manejarlas. Por ello esta rama en unos meses
dejará de compilar en compiladores anteriores. 

Si nunca has compilado la toolchain puede que
[este](help/instalacion_toolchain.md) documento te pueda ayudar.


## <a name="directorios"></a>Estructura de directorios

### src: Todo el código fuente. 

Se puede organizar el código en diferentes niveles:

1. Cosas genéricas tanto para micros como para PC (programming level?)

    * [std](src/std/README.md): Biblioteca `std`, aquí incluyo todo lo de `std` que 
	 no trae `avr-gcc`.
	 `avr-gcc` no suministra ni excepciones ni los operadores new/delete. 
	 Por ello capan la `std` (supongo que para no tener que ir buscando 
	 qué código depende de excepciones y de new/delete).

       Mi `std` suministra todo (??? <-- ahora es mentira, suministra un 
       poquito) el código standard que no depende de las excepciones ni 
       hace uso de new/delete. Si algo depende de excepciones o lo
       elimino o no las suministro (ejemplo: `iostreams` no suministra
       excepciones).

    * [atd](src/atd/README.md): Funciones genéricas que se basan en mi std. 
	 No usan excepciones ni los operadores new/delete.

2. [Microcontroladores](src/microcontrollers): (microcontroller level)
    
    * [avr](src/microcontrollers/avr/README.md): Traductores del avr. 

      En lugar de tener que recordar qué bit hay que escribir, 
      usamos nombres.

      Quien programa este directorio tiene que conocer cómo funciona el
      microcontrolador. Necesita leer la datasheet.

    * [mcu](src/microcontrollers/mcu/README.md): Componentes genéricos de los
      microcontroladores.

      Son todos aquellos componentes que se implementan usando exclusivamente
      hardware encontrado en los microcontroladores (timers, pins, ...)

3. [Dispositivos](src/devices): (device level)

    * [hwd](src/devices/hwd/README.md): Built-in devices, dispositivos
      físicos, reales, de hardware.

      Cada dispositivo físico, cada chip, necesita un driver. Aquí es donde
      guardo todos esos drivers. Cada dispositivo tendrá dos archivos: el
      `_basic` que contiene el traductor y el `_generic` que contiene el
      conceptuador.

      Quien programa aquí conoce los detalles del chip, necesitando leer la
      datasheet.

    * [logic](src/devices/logic/README.md): User-defined devices

      Los dispositivos que aparecen aquí son dispositivos construidos a
      partir de concepts, no sabiendo el dispositivo real que hay por
      debajo. 

      El programador no conoce la datasheet ya que desconoce el chip real al
      que accede. Lo que conoce son los diferents concepts.

4. [Proyectos](src/projects):

    * [pli](src/projects/pli/README.md): Bibliotecas comunes a varios proyectos.

    * [prj](src/projects/prj/README.md): Proyectos.

## <a name="namespaces"></a>Namespaces definidos

* `std` es el namespace usado por el estandard. 
* `atd` incluye todas las funciones genéricas que no usan excepciones ni
   memoria dinámica.

* `mcu` contiene todo lo genérico a microcontroladores. 
* `atmega`, `attiny`, ... son funciones concretas de microcontroladores
   concretos.
* `dev` incluye todos los traductores y drivers de dispositivos. Observar que
   en este namespace estoy incluyendo dos cosas diferentes: por una parte
   traductores de dispositivos reales de hardware (como el L298N que es un
   chip real), mientras que también incluyo dentro drivers de dispositivos
   lógicos, dispositivos que no se corresponden realmente con un chip
   concreto (por ejemplo, `EEPROM_iostream` consiste en concebir una memoria
   EEPROM como un iostream).

* `pli` incluye código común a diferentes proyectos. (`pli` viene de PrjLib)


## <a name="entorno"></a>Configurar entorno de trabajo

Para configurar el entorno de trabajo basta con ejecutar [mcu_environment.sh](mcu_environment.sh).


## <a name="reglas_compilar"></a>Reglas para compilar

En el directorio [mk](mk) están todas las reglas de compilación necesarias para 
compilar cada bloque. 

En la mayoría de los directorios se puede encontrar un script `make.sh` que
compila la biblioteca con diferentes frecuencias.

## <a name="test"></a>test y pc_test
En cada directorio suministro directorios `test` o `pc_test`. Los `pc_test`
son test automáticos para realizar en el ordenador. Se compilan con `gcc`
mientras que `test` son test para realizar en el microcontrolador, compilados
con `avr-gcc`.

Los `pc_test` necesitan varios ficheros de 
[alp](https://github.com/amanuellperez/alp/tree/master/src) (otro proyecto).
Basta con que descargarse `alp_test.h`, `alp_test.cpp`, 
`alp_string.h` y `alp_string.cpp` para que compilen.

## <a name="documentacion"></a>Documentación y ayuda
Esto es un proyecto de aprendizaje y para experimentar, pudiendo haber partes
muy inestables. Aunque según voy escribiendo cosas algunas ya van siendo muy
estables, otras, las más nuevas o las que apenas he usado son todavía
inestables. Por eso no tengo ninguna intención de dedicar mi tiempo en
escribir documentación.

Con todo existen dos fuentes reales para entender el código:

1. El propio código: uno de los objetivos de todo el código es que se pueda
   leer. Si no se puede leer (salvo las partes de bajo nivel) está mal
   diseñado y hay que cambiarlo.

2. Los tests: son ejemplos de cómo tengo intención de usar la librería 
   correspondiente.



## <a name="licencia"></a>GPL

¿Qué licencia ponerle a este proyecto? Al mirar las diferentes licencias que
hay, y desconocer por completo el tema, uno se pierde. ¿Por qué elegí la GPL
3? Simplemente por ser última versión de la GPL (= traducción: no tengo ningún
criterio real de por qué elegir la GPL, básicamente la he puesto por moda y
desconocimiento. Espero que la decisión no haya sido un error).

¿Cuáles son las que (creo) que son las características de esta licencia?

1. El código que pongo es libre: lo puedes usar como quieras. Yo no tengo
   ningún problema en que lo uses en un proyecto particular, o incluso que lo
   uses en un proyecto comercial (aunque aquí no me ha quedado claro si la GPL
   3 prohibe esto).

2. Si usas mi código tienes que darme el crédito correspondiente (a mi o a
   quien lo desarrolle). Parece lo correcto, ¿no?

3. El código no tienen ningún tipo de garantía. Aunque por supuesto intento
   programarlo bien, muchas de las partes son realmente proyectos de
   aprendizaje y conviene recordar que siempre que se hace por primera vez una
   cosa lo más seguro es que no se haga de la mejor manera.

4. Al tratarse de un proyecto de aprendizaje puede ser inestable, sobre todo
   las partes nuevas. Salvo el estandar `std` que no lo defino yo y por tanto
   el interfaz es estable, lo demás puede que lo cambie según vaya ganando
   experiencia. Con todo, código que se vea que tiene años y se está usando
   sistemáticamente es de esperar que ya vaya siendo semiestable.

5. Si mejoras el código tienes que darme una copia de todas las mejoras, de
   esa forma yo también puedo beneficiarme.

6. Estoy subiendo datasheets o referencias que no tienen GPL ni tampoco he
   redactado yo. No tengo claro si puedo subirlas. Todas están públicas en 
   internet.
   Las subo para que sea más sencillo revisar las implementaciones de los
   traductores. Si por lo que sea alguno de los documentos tiene licencia y no
   se puede subir agradecería me lo indicasen para eliminarlo inmediatamente.






## <a name="videos"></a>Vídeos

Había creado un canal en YouTube para ir subiendo algún vídeo de algunos de los
circuitos que estaba implementando. Sin embargo voy a abandonarlo, ya que
voy a ir eliminando sistemáticamente el producto de Google de mi vida. Los
motivos los siguientes:

* En septiembre o así escribí un curso de arduino en español para
  principiantes y como quería que cada alumno fuese a su ritmo hice un montón
  de vídeos cortos (de 1 minuto como mucho) que subí en un nuevo canal de
  YouTube. Por motivos que desconozco uno de los vídeos en menos de 24 horas
  tenía más de 600 visitas. Ignoro si fue por culpa de este vídeo o por culpa
  de que estuve subiendo el máximo de vídeos que deja YouTube por día pero al
  cuarto-quinto día los de Google me dijeron que "mi canal violaba su
  política" y que o me identificaba dándoles mi teléfono o cerraban el
  canal. Hice lo que cualquier persona sensata haría: no darle mis datos.


* Llevo con otro canal desde hace más de 7 años, teniendo alguno de los videos
  del orden de 1 millón de visitas. Acabo de intentar entrar y me encuentro
  con un mensaje del tipo: "la reciente actividad del canal es muy sospechosa,
  identificate si quieres acceder al canal", pidiéndome de nuevo el número de
  teléfono. Por suerte nunca he monetizado dicho canal con lo que me trae sin
  cuidado si lo bloquean o no. 

  Es interesante observar el comportamiento de los de Google: 
  mientras que el canal de arduino, que no tenía seguidores, lo cerraron
  este último canal tiene un mínimo de 
  unas 50.000 visitas al mes por las que yo no saco nada de dinero mientras
  que los de YouTube sí. Y este canal no lo cierran sino que lo bloquean al
  que ha creado el contenido.

  Recuerda: Google, Meta, Amazon, ... se queda con todo tu trabajo. 



Si por
lo que fuera los de github deciden bloquearme migraré el proyecto a otro
sitio. El problema es que no podré poner aquí dónde (estaré bloqueado xD). Si
alguien quisiera localizarme que use el nuevo mail (manuel2perez@proton.me).


## <a name="tags"></a>Tags usadas en el código

+ (RRR): rationale, esto es, indica los motivos por los que he implementado
  (originalmente) la función de esa manera.

+ (impl): idea de implementación.

+ (???): duda. No tengo claro cómo hacerlo o si es verdad lo que digo.

