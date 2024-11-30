MCU++ library
=============
(If you understand spanish read better the spanish version of the readme
(scroll down this document))

Libraries and applications for microcontrollers (right now, only for
ATMEGA328 and atmega4809).

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
	(right now, only for ATMEGA328 and atmega4809). 
      
    * [mcu](src/microcontrollers/mcu/README.md): Generic microcontrollers components.

    * [rom](src/rom/README.md): Data to save in ROM.

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
ATMEGA328 y el atmega4809).

# Índice
* [Compilador y toolchain](#compilador)
* [Estructura de directorios](#directorios)
* [Namespaces definidos](#namespaces)
* [Entorno de trabajo](#entorno)
* [Compilar](#compilar)
* [Tests](#test)
* [Diseño](#discapas)
* [Programación](#programacion)
* [Documentación y ayuda](#documentacion)
* [Licencia GPL](#licencia)
* [Vídeos](#videos)
* [Tags usadas en el código](#tags)
* [Nombres de funciones](#namefunction)
* [Varios](#varios)

## <a name="compilador"></a>Compilador y toolchain
`binutils 2.42`
`avr-g++ 13.3.0`
`avr-libc 2.2.0`

El código usa cada vez más los concepts de C++20, con lo que no compilará con
versiones anticuadas del compilador. Aunque quiero empezar a usar módulos,
todavía no los uso ya que parece ser que no están completamente implementados 
en gcc. Cuando lo estén espero introducirlos de forma sistemática con lo que
ya no compilará nada de código en compiladores que no compilen C++20.

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

    * [rom](src/rom/README.md): Datos a guardar en la ROM (progmem del avr)

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
* `rom` contiene constantes guardadas en la ROM del microcontrolador.
* `atmega`, `attiny`, ... son funciones concretas de microcontroladores
   concretos.

   A su vez, dentro de cada micro, se encuentran definidos los namespaces
   `hwd` (traductores) y `hal` (hardware abstraction layer). `hal` tiene como
   sinónimo `driver`.

* `dev` incluye todos los traductores y drivers de dispositivos. Observar que
   en este namespace estoy incluyendo dos cosas diferentes: por una parte
   traductores de dispositivos reales de hardware (como el L298N que es un
   chip real), mientras que también incluyo dentro drivers de dispositivos
   lógicos, dispositivos que no se corresponden realmente con un chip
   concreto (por ejemplo, `EEPROM_iostream` consiste en concebir una memoria
   EEPROM como un iostream).

* `pli` incluye código común a diferentes proyectos. (`pli` viene de PrjLib)


## <a name="entorno"></a>Entorno de trabajo

* ¿Sistema operativo? Linux.

* ¿Mi entorno de trabajo? Un terminal (varios realmente) de linux. 

* ¿Mi editor? El mejor editor del mundo: ¡vim! No uso ningún tipo de IDE ni
  nada parecido. 

* `make` es el programa para controlar las dependencias y `gcc` el compilador.

Todo el software que uso es gratuito.


### Configurar entorno de trabajo
Para configurar el entorno de trabajo basta con ejecutar [mcu_environment.sh](mcu_environment.sh).

Para que el entorno se cargue correctamente es necesario configurar el primer
apartado de `mcu_environment.sh`: rellenar `MCU_ROOT`, que indica dónde se
encuentra el proyecto instalado, y la variable `PATH_AVR_BINS` que indica
dónde encontrar los compiladores.

## <a name="compilar"></a>Compilar

### Problema: crear bibliotecas con diferentes frecuencias

#### Generar la librería del traductor del micro a diferentes frecuencias
Las dos frecuencias más básicas a las que funciona el `atmega32` son a 1MHz y
a 8 MHz. Si se genera código en `.cpp` ese código solo funcionara a la
frecuencia `F_CPU` que se usó para compilarlo. 

Para poder generar bibliotecas para cada una de las diferentes frecuencias
usadas al nombre de la bilioteca generada le añado la frecuencia `F_CPU` que
se uso para compilarla. Así, se generan `libavr_1000000UL.a` y
`libavr_8000000UL.a`, la primera a 1MHz y la segunda a 8MHz. El usuario tendrá
que enlazar con la biblioteca que corresponda.


**make.sh**
Para compilar todas las bibliotecas a la vez, uso el script `make.sh` que se
puede encontrar en el directorio `tools`. Se limita a compilar todas las
frecuencias definidas en la variable de entorno `LIST_F_CPU`.

#### Resto de librerías

El resto de librerías, `hwd`, `logic`, ... son librerías genéricas que *no*
dependen del microcontrolador ni de la frecuencia a la que opere. Por ello,
las librerías no hacen referencia a `F_CPU`. Para compilarlas basta con
ejecutar `make dist`.



### Reglas para compilar

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



### Implementación de los test

Cuando implementas algo te acuerdas de todo. Si implementas un test guarro,
que es lo que se suele hacer al principio, resulta que 6 meses después cuando
tienes que modificar el driver o programa no recuerdas cómo funciona el test y
no solo tienes que perder tiempo en la modificación real sino también en
averiguar cómo funcionaba el test. ¿Qué pines estan conectados con quien?
¿Pongo un condensador? ¿Resistencia? ¿Cristal? ...

Por ello, es fundamental que los tests guien al que los ejecuta para probarlos. 
Cada vez voy desarrollando más el hábito de:

1. Poner un mensaje de presentación que indique cómo hay que conectar todo el
   hardware. (a día de hoy es la función `hello()`).

2. Estoy empezando a hacer algunas pruebas automáticas en el microcontrolador
   similares a las de `pc_test`. Estas pruebas dan mucha tranquilidad mental
   cuando se modifica un programa, el problema es que con el micro no tengo
   todavía claro cómo hacerlo. ¿Hacer un programa para probar todo lo del
   micro? Suena bien: experimentemos a crear un test llamado `automatic` a 
   ver si lo uso.


## <a name="discapas"></a>Diseño

### Diseño por capas
Todo el diseño está pensado por capas, donde cada capa se *parametriza* como
parámetro de template. Aunque por el momento no tengo intención de hacerlo, si
se escribe el equivalente de `avr` para `arduino` se podrían usar los drivers
definidos en la capa `devices` con arduino. Salvo por curiosidad y como
experimento, no creo (a día de hoy) que merezca mucho la pena escribir la 
capa de arduino ya que el estilo de arduino es muy diferente al estilo que 
uso aquí.

### Capas definidas

Las capas que (a día de hoy) hay definidas son:

1. Hardware layer (`hwd`): son los traductores.

2. Hardware abstraction layer (`hal` = `driver`): son interfaces genéricos de
   los traductores.

3. Logic layer: son devices a los que les amplio la capacidad por software o
   composición de diferentes dispositivos para formar uno mayor.

4. Application layer: son las aplicaciones (proyectos) del usuario final.


### Traductores vs drivers
La capa más básica son los traductores que se encuentran dentro del namespace
`hwd`. Quiero que el cliente explícitamente indique si quiere usar un
traductor o un driver, ya que el segundo puede que introduzca ineficiencias
que no tiene el primero. Por ello se distingue entre: `hwd::A4988` y
`driver::A4988`.


### Static interface vs objects

Desde el principio llevo con una duda:

1. Por una parte, si tengo un Timer0 en el microcontrolador solo tengo un
   dispositivo de hardware, *no* tiene sentido dar la opción a crear 2 Timer0.
   Para evitar que el programador se confunda definiendo dos Timer0 lo defino
   como static interface, borrando el constructor.

2. Por otra parte, los dispositivos hay que inicializarlos para que funcionen
   correctamente. Con el static interface defino la función `init` que el
   programador se tiene que acordar de llamar. Pero los constructores de C++
   precisamente evitan que el programador se olvide de inicializar el
   dispositivo haciendolo automáticamente y evitando error. 

Conclusión: quiero que el programador pueda crear objetos para que se llame al
constructor automáticamente, pero quiero que el compilador (?) genere un error
si el programador intenta definir dos objetos del mismo tipo a la vez.

Por ejemplo, supongamos que el programador quiere usar un `PWM1_pin` y asocia
un `Miniclock` al Timer1. Claramente esto es un error, porque el Timer1 o lo
usas para generar una señal PWM o lo usas para medir tiempos. A día de hoy, es
el hardwador el responsable de ello, definiendo en `hwd_dev.h` algo del tipo:

```
using PWM_pin      = myu::driver::PWM1_pin<15>;
using Miniclock_ms = mcu::Miniclock_ms<Micro, myu::driver::Time_counter1>;
```

Si, como hago ahora, uso las clases como static interface el hardwador llamará
en el `init` del programa a

```
PWM_pin::init();
Miniclock_ms::init();
```

El llamar a ambas funciones init debería de generar un error del compilador
diciendo "estas intentando usar el Timer1 de dos formas completamente
diferente". 

**PROBLEMA** ¿Cómo generar ese error? 

Observar que una vez que se genere ese error, se podrían usar objetos.
Bastaría con que los constructores se limitaran a llamar a `init`.

### Dispositivos lógicos

Podemos agrupar diferentes dispositivos de hardware para crear un nuevo
dispositivo. Por ejemplo, si quiero controlar la velocidad de un DC motor voy
a crear un nuevo dispositivo `Speed_control_motor`, o nombre parecido, que
está formado por dos componentes: un motor y un sensor de velocidad de giro.

Estos dos componentes tendrán sus drivers correspondientes `Motor` y
`Speedmeter`, pero es importante ser consciente de que el
`Speed_control_motor` *está formado por* incluye estos dos dispositivos dentro
de él. Eso hace que el `Speed_control_motor` sea responsable de la
creación/destrucción de esas clases.

Es importante tener en cuenta esto sobre todo cuando se quieren reutilizar
componentes como pueden ser los timers.

## <a name="programacion"></a>Programación

### Estilo

Uso snake case, definiendo las clases con la primera letra en mayúscula.

Uno de los objetivos propuestos es que todo el código sea legible, que se
pueda entender sin necesidad de comentarios. Me quedo con snake case porque
para mi es el que se parece más a la forma normal de escribir: en lugar de
espacios entre palabras escribes un underscore.

### Namespaces 

Cada vez es más frecuente el uso de ciertos espacios de nombre auxiliares.

1. Todos los namespaces que acaban en `_` son privados, reservandome el
   derecho a cambiar el nombre en el futuro. Incluyen detalles de
   implementación, como `avr_`, `mega_`, `mega0_`,... 

2. Hay algunos namespaces que poco a poco empiezo a usar:

   * `default_cfg`: incluyen configuraciones por defecto de templates (o
     cualquier otra cosa). 

   * `impl_of`: incluye la implementación de alguna función. Bajo ningún
     concepto el usuario debe llamar a las funciones definidas aquí.

   * `private_`: preferiría llamarlo simplemente `private` pero esa es una
     keyword de C++. Esto incluye detalles de implementación. No se deben de
     llamar.




## <a name="documentacion"></a>Documentación y ayuda
Esto es un proyecto de aprendizaje y para experimentar, pudiendo haber partes
muy inestables. Aunque según voy escribiendo cosas algunas ya van siendo muy
estables, otras, las más nuevas o las que apenas he usado son todavía
inestables. Por eso no tengo ninguna intención de dedicar mi tiempo en
escribir documentación. 

Con todo, para aclarar mis ideas y recordar lo que he
hecho voy escribiendo algo de documentación. El problema es que al no
mantenerla hay partes de la documentación completamente obsoletas, generando
una documentación inconsistente.

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

* Update: cosa de 2 años después de escribir lo anterior de repente Youtube me
  vuelve a dar acceso a mis canales. ¿Motivo? Ni idea, ya que yo no hice nada.

  Ando buscando una alternativa a Youtube. Probé con Odysee pero (antes?)
  tenía el problema de que solo te dejaba subir un número determinado de
  videos. Si tus videos se veían, te dejaban subir más. Si no, pues no. Y como
  yo no promociono mis videos, no me deja subir más que un número fijo de
  videos, no sirviéndome para documentar lo que voy haciendo aquí. 



Si por
lo que fuera los de github deciden bloquearme migraré el proyecto a otro
sitio. El problema es que no podré poner aquí dónde (estaré bloqueado xD). Si
alguien quisiera localizarme que use el nuevo mail (manuel2perez@proton.me).



## <a name="tags"></a>Tags usadas en el código

+ (RRR): rationale, esto es, indica los motivos por los que he implementado
  (originalmente) la función de esa manera.

+ (impl): idea de implementación.

+ (???): duda. No tengo claro cómo hacerlo o si es verdad lo que digo.

+ TORNOT: abreviatura de "TODO or not TODO".


## <a name="namefunction"></a>Nombres de funciones

C++ no admite la posibilidad de pasar parámetros en cualquier posición. Por
ejemplo, si quieres copiar dos números escribes `copy(x,y)` y siempre queda la
duda de si lo que se está haciendo es `x = y` o `x -> y`, con significados
completamente distintos.

Personalmente me gustaría que C++ admitiese notación del tipo
`copy(x)_into(y)`. Como esto es bastante práctico, de momento, tengo 2 métodos
para implementarlo:

1. Usando estructuras es fácil simular la notación `copy(x).into(y)`. El
   problema es que hay que escribir más código y siempre queda la duda de que
   pueda ser más ineficiente.

2. Una nueva notación experimental es escribir el nombre de la función como
   `copy___into___(x, y)` de esa forma los 3 guiones bajos indican dónde va
   cada parámetro. Esto es, `copy__into__(x,y)` es mi forma de escribir
   `copy(x)_into(y)`.

## <a name="varios"></a>Varios

* Estoy traduciendo `libraries` por librería. Sí, ya se que está mal
  traducido, pero qué quieres: cuando estudie mi profesor no debía saber nada
  de inglés y escuché tantas veces la palabra "librería" que ya me suena raro
  si digo otra cosa.


