# INSTALACIÓN Y CONFIGURACIÓN DE LA TOOLCHAIN Y DEMÁS

## ÍNDICE
* [Aviso](#aviso)
* [Instalar la toolchain](#instalar_toolchain)
* [Compilar la toolchain](#compilar_toolchain)
* [Configurar cable FTDI...](#configuracion)

## <a name="aviso">AVISO</a>

Estas son las notas que he ido tomando para saber cómo compilar e instalar la
toolchain, además de cómo configurar el cable de FTDI. Son meros apuntes que
no pretenden ni ser correctos ni exhaustivos. Los dejo públicos por si les
pueden resultar de ayuda a alguien.


## <a name="instalar_toolchain">INSTALAR LA TOOLCHAIN</a>
### Fuentes de fiar
El capítulo 2 del libro "Make: AVR Programming", de Elliot Williams, es una
buena referencia para saber cómo instalar la toolchain por defecto. Explica
cómo instalarlo en Linux, Windows y Mac.

La [página de `avrlibc`](https://avrdudes.github.io/avr-libc/avr-libc-user-manual-2.2.0/install_tools.html)
es la última referencia que he usado.

### Instalación
Para instalar la toolchain en linux (versión que suele estar muy anticuada,
así que mejor no la instales):

```
$ sudo apt install avrdude avrdude-doc binutils-avr avr-libc gcc-avr gdb-avr
```


## <a name="descargar_toolchain">DESCARGAR LA TOOLCHAIN</a>
* GNU Binutils: [Download](https://ftp.gnu.org/gnu/binutils/)
  [Página principal](https://www.gnu.org/software/binutils/)
* GCC: [Download](https://ftp.gnu.org/gnu/gcc/)
  [Página principal](https://gcc.gnu.org/)
* avr-libc: 
  [Download](https://github.com/avrdudes/avr-libc/releases)
  [Github](https://github.com/avrdudes/avr-libc)
  [Antigua página?](https://www.nongnu.org/avr-libc/)


### Verificar los ficheros descargados (método recomendado)

La [página de GNU](https://ftp.gnu.org/README) indica cómo hacerlo:

1. Descargar https://ftp.gnu.org/gnu/gnu-keyring.gpg
2. Verificar `binutils`:

```
$gpgv --keyring ./gnu-keyring.gpg binutils-2.42.tar.xz.sig binutils-2.42.tar.xz
gpgv: Signature made Mon 29 Jan 2024 04:20:06 PM CET
gpgv:                using RSA key 3A24BC1E8FB409FA9F14371813FCEF89DD9E3C4F
gpgv: Good signature from "Nick Clifton (Chief Binutils Maintainer) <nickc@redhat.com>"
```

3. Verificar `gcc`:
```
$ --keyring ./gnu-keyring.gpg gcc-13.3.0.tar.xz.sig gcc-13.3.0.tar.xz
gpgv: Signature made Tue 21 May 2024 10:49:50 AM CEST
gpgv:                using RSA key 6C35B99309B5FA62
gpgv: Good signature from "Jakub Jelinek <jakub@redhat.com>"
gpgv:                 aka "[invalid image]"
```

DUDA: ¿Qué es eso de [invalid image]? 

4. Verificar la avrlibc:

Ni idea de cómo hacerlo. No me vale el keyring de GNU y después de un buen 
rato no he conseguido encontrar quién firma el fichero. La versión que
descargo es la última, la subida en github. 

No verificarla por mi parte es un error (<-- es imperdonable!!!, en un
proyecto serio no se podría hacer esto), pero que los de `avrlibc` no tengan
documentado el cómo verificar el fichero descargado en la parte de instalación
de la toolchain es incómodo (tienen mucha documentación y en algún sitio puede
que este escrito).


## <a name="compilar_toolchain">COMPILAR LA TOOLCHAIN</a>

### Fuentes de fiar
La página de
[avr-libc](https://avrdudes.github.io/avr-libc/avr-libc-user-manual-2.2.0/install_tools.html)
explica cómo instalar la toolchain. 

Se puede consultar también la antigua página 
de [avr-libc](https://www.nongnu.org/avr-libc/user-manual/install_tools.html)
aunque supongo que esta irá quedando obsoleta.



### Pasos previos
1. **IMPORTANTE**. Intenté compilar solo el avr-gcc pero no quedó bien configurado. En la
página de ayuda indica que lo mejor es instalarlo todo en el mismo directorio
y al reutilizar la versión ya instalada no quería hacerlo. Supongo que por eso
me dió los errores. Para evitarlo, acabo por instalar toda la toolchain.

2. Si ya hay alguna versión instalada de avr desconfigurar todas las
variables de entorno. Un problema que tuve al compilar las binutils era que el
`configure` estaba localizando archivos de cabecera de la versión `avr`
instalada, los cuales claramente no compilan en ubuntu. Lo que hice fue no
cargar ningún entorno mio de compilación y automáticamente todo compiló
correctamente.

3. Antes que hacer nada:

   1. Definir dónde voy a instalarlo todo:
      ```
      $ PREFIX=$HOME/root/bin/avr
      $ export PREFIX
      ```

   2. Añadir al `PATH` el directorio de instalación de los bin: 
   
      Dejar el `PATH` completamente limpio para evitar que coja programas que
      no debe:

      ```
      $ PATH=$PREFIX/bin:/bin:/usr/bin
      $ export PATH 
      ```

      Notar que hago que busque primero en `$PREFIX/bin`.  

      La primera vez que compilé tenía instalada una versión de las binutils 
      en `/usr/lib`: al compilar `avr-gcc` quiero que use las `binutils` que 
      acabo de compilar, no las que pudieran estar instaladas en el ordenador,
      por eso necesito poner primero `$PREFIX/bin` para que encuentre la
      versión que yo acabo de compilar.

   3. Crear el directorio `$PREFIX`:

	    `$ mkdir $PREFIX`

   4. Guardar la salida del `configure` en un fichero, para poder revisar 
   lo que ha hecho (en caso de error de compilación puede ser necesario).


### Directorios de compilación

La página de
[avrlibc](https://avrdudes.github.io/avr-libc/avr-libc-user-manual-2.2.0/install_tools.html)
recomienda tener dos directorios diferentes: uno `source` donde descomprimimos
todo el código fuente, y otro `build` donde compilamos todo.


### binutils: linker, assembler...

1. Descargarlo de la página que indique el link de ayuda dado al principio.
    
   Verificar la versión descargada:

   ```
   $ gpg --verify --keyring ./gnu-keyring.gpg binutils-2.33.1.tar.xz.sig
   ```

   Descomprimirlo.

	
2. Configurarlo (versión 2.42):
    
    En el directorio `build` configurarlo:
   ```
   $ # in ./build
   $ ../../source/binutils-2.42/configure --prefix=$PREFIX 
                                      --target=avr --host=x86_64-linux-gnu \
          --disable-nls --disable-sim --disable-gdb --disable-werror > log 2>&1
   ```

   Notar que he añadido el `--host`. Sin el host daba un error de que no era
   capaz de reconocer el tamaño de un long long (¿por defecto considera que
   es una máquina de 32 bits?)


4. Compilarlo e instalarlo:
 
   ```
   $ # in ./build
   $ make
   $ make install
   ```

   Me ha dado un error. Resulta que el `configure` no ha definido la variable
   interna `prefix=`. Escribo el `Makefile` poniendo en lugar de `prefix=`
   --> `prefix=${PREFIX}`. Nota: en la última instalación me escribió
   correctamente el `prefix`.

6. Observar que deja instalados dos directorios bin:
   ```
   $PREFIX/bin	   # versiones: avr-as, ...
   $PREFIX/avr/bin # versiones: as, ld, ...
   ```
   Las dos parece ser necesarias. De hecho probé a eliminar la segunda
   y no podía configurar la compilación de `avr-libc`.

7. Se puede hacer una pequeña comprobación de que ha quedado instalado
   ejecutando
   ```
   $PREFIX/bin/avr-as --version
   ```

### avr-gcc: el compilador

Tener en cuenta que el `avr-gcc` no es más que el `gcc` compilado con `target =
avr`.

0. Garantizar tener en el PATH las avr-binutils instaladas antes (si no hemos
   salido del shell seguirá estando bien configurado).

1. Instalar prerequisitos (GMP, MPFR...)
   Se hace automático ejecutando `./contrib/download_prerequisites`:
   ```
   $ # in ./source
   $ cd source/gcc-13.3.0
   $ ./contrib/download_prerequisites
   ```

   Se necesita tener una conexión a internet. Este comando se ejecuta en el
   directorio del source de `gcc`.

2. Crear el directorio de compilación **FUERA** de `gcc` 
   (eso indica en la ayuda). 

   Si lo tengo descomprimido en `cd-rom/source/gcc-13.3`, creo
   `cd-rom/build/gcc-13.3`


3. Lo configuramos:(ver todas las posibles opciones 
   [avr-libc](https://avrdudes.github.io/avr-libc/avr-libc-user-manual-2.2.0/install_tools.html))
   [antiguo](https://gcc.gnu.org/install/configure.html))

   La vez que intenté compilarlo teniendo instalada la `toolchain` me 
   generó bastantes problemas compilarlo (fallaba en la biblioteca
   `libbacktrace`). Al final usé la configuración que tenía la toolchain de
   Atmel (se ve mirando la versión: `avr-g++ -v`) y compiló.

   La segunda vez, al compilarlo sin tener ninguna versión instalada de la
   toolchain, apenas ha generado problemas. 

   Configuro con las siguientes opciones:

   ```
   $ cd build/gcc-13.3
   $ ../../source/gcc-13.3/configure
	    --prefix=${PREFIX}			
	    --build=x86_64-linux-gnu 
	    --host=x86_64-linux-gnu 
	    --target=avr 
	    --program-prefix=avr-
	    --program-suffix=-13.3.0
	    --enable-languages=c,c++ 
	    --disable-nls 
	    --disable-libssp
        --disable-libcc1 # nueva en gcc-13.3
        --with-gnu-as    # nueva en gcc-13.3
        --with-gnu-ld    # nueva en gcc-13.3
	    # Elimino estas opciones que usé en la primera compilación.
	    # --enable-long-long  <-- esta no la he encontrado en 9.2.0
	    # --with-system-zlib <-- no encuentra esto
	    # --enable-shared  <-- avr soporta librerías dinámicas?
        # --with-dwarf2    <-- yo no voy a depurar
        # Opciones que desaparecen:
        # -v <-- desaparece en gcc-13.3
	    # --without-included-gettext	# esta opción no aparece en la web de gnu???
   ```

   *¿Qué hace cada opción?*

   * `-v`: ni idea. ¿qué hace?
   * `--with-system-zlib`: en la página de gcc indica que hay que incluirlo.

     La primera vez lo incluí y compiló la segunda vez no encontraba
     esta biblioteca, pero la trae el compilador así que no la incluí
     y compiló.

   * ¿dónde instalamos el compilador?: lo indica `--prefix=$PREFIX`.
   * ¿qué target?: `--target=avr`
   * ¿qué lenguajes usaremos?: `--enable-languages=c,c++`
   * ¿Los mensajes los damos en español o no?:  `--disable-nls`.

     De hecho, en general, nunca usar programas en español. Cuando dan
     un error si el mensaje está en español es casi imposible
     encontrarlo en internet. En inglés lo encuentras fácilmente.
   
   * `--with-gnu-as`: da por supuesto que `as` es el GNU assembler.
   * `--with-gnu-ld`: misma idea que `--with-gnu-as`
   * `--disable-libcc1`: ???

   * Usa DWARF 2 debugging information:	`--with-dwarf2`
     Aconsejan poner esto, pero no lo pongo. Es para depurar
     (yo no uso el depurador).

4. Lo compilamos e instalamos:
   ```
   $ # in ./build
   $ make > mk.log 2>&1 &
   $ tail -f mk.log	
   ``` 
   No es necesario guardar la salida de `make`. Sin embargo, en caso de
   error es más sencillo de revisar. Tarda más de 1 hora en compilar.

   Para compilar más rápido revisar el fichero html de la ayuda del `gcc`.
   Indica que `make -j 2` puede compilar en paralelo. No lo he probado.
	
   Para instalarlo: `$ make install`

5. Aunque encuentro en `mk.log` el siguiente warning

   ```
   libtool: install: 
       warning: remember to run 
	    'libtool --finish ~/bin/avr/libexec/gcc/avr/9.2.0'
   ```

   en internet pone que se puede ignorar. Lo ignoro y todo funciona.




POSIBLES ERRORES

1. Obsoleto:
   En ubuntu por defecto se instalan en `/usr/lib/avr/bin`, así que es este el
   path a añadir. En caso de no hacer esto al intentar compilar da el error de
   que no puede enlazar con `as` ya que le pasamos la opción `-mcu=avr5`.

2. Si ahora al intentar enlazar un programa del avr dice que no encuentra
   `crtatmega328p.o`, es normal: falta instalar las `avrlibc`!!!



### avr-libc

Este es el más sencillo de compilar. No genera problemas.

1. Descomprimirlo.
2. Averiguar el flag `--build`:
   ```
   $ # in ./source
   $ ./config.guess
   $ x86_64-pc-linux-gnu  # respuesta en mi caso
   ```

2. Ejecutar:

   ```
   $ # in ./build
   $ ../../source/avr-libc-2.2.0/configure --prefix=$PREFIX \
                                    --build=x86_64-pc-linux-gnu --host=avr
   $ make
   $ make install
   ```
   Por supuesto que la opción `build` se puede pasar como:
   `--build=`../../source/avr-libc-2.2.0/config.guess` 

   Puede que falle el `./configure` indicando que no encuentra `avr-gcc`. Es
   porque al compilar `avr-gcc` se habrá generado el fichero `avr-gcc-11.3.0`
   en vez de `avr-gcc`. Crear `avr-gcc` como link de `avr-gcc-11.3.0`.

   Al final de ejecutar `configure` se queda un ratito parado dando la
   impresión de haberse colgado: simplemente está acabando de ejecutarse (sin
   mostrar ningún mensaje nuevo).

### avrdude

`avrdude` es el programa que usamos para cargar nuestro programa en el `avr`.

Para instalarlo basta con escribir:

```
$ sudo apt install avrdude avrdude-doc
```

### Configuración de la toolchain

Añadir al path en el `.profile` el path donde ha quedado instalada la toolchain:
```
PATH=${PATH}:${PREFIX}/avr/bin
```


## <a name="configuracion">CONFIGURAR CABLE FTDI</a>

El cable que yo tengo es un FTDI TTL-232D-5V original. Funciona bien, no dando
ningún problema.

1. Para ver en que USB se encuentra ejecutar 
   `$ lsusb`
   seguramente esté en `/dev/ttyUSB0`.


2. Hay que meter nuestro usuario en el grupo cable, seguramente `dialout`.

   Para saber en qué grupo se encuentra hacer

   `$ ls -l /dev/tty*`
    
   Para darle los permisos correspondientes:

   `$ sudo usermod -a -G dialout <tu_userid>`

    (<tu_userid> es el resultado de ejecutar `whoami`)

   No olvidar reiniciar el ordendador.


3. Conectar el cable al AVR:

   Los cables son los siguientes:

   | color	| significado	|				|
   |-----------|---------------|-------------------------------|
   | negro	| tierra	|				|
   | amarillo	| RXD		| conectarlo al pin 3 del AVR	|
   | naranja	| TXD		| conectarlo al pin 2 del AVR	|



4. Para abrir un terminal de comunicación con el AVR, podemos:
   * Usar [`myterm`](https://github.com/amanuellperez/mcu_pc_tools/tree/main/src/myterm).
     : poco a poco voy a ir creando mi propio terminal ya que no
     he encontrado un terminal que encaje con todo lo que necesito. 
     `myterm`, como todo lo que hago, es experimental: si no funciona o no te gusta,
     ¿para qué lo usas?

   * Usar `screen`: (obsoleto; aunque funciona genera problemas ya que
     para escribir una nueva línea hay que enviar los dos caracteres "\r\n" y
     yo opto por enviar solo uno '\n'. A partir de ahora voy a empezar a usar
     mi terminal `myterm`)

     Para instalarlo ejecutar

     `$ sudo apt install screen`

     Para conectar el ordenador al avr via el cable de FTDI:
     ```
     $ screen /dev/ttyUSB0 9600
     (screen <serial_port> <baud_rate>)
     ```

     Todo lo que se escriba en este monitor serie se enviará al AVR
     a través del USB.
    
     Para salir de screen: `Ctrl-A` y luego `K` (kill).

   * Usar python (instalar primero `pyserial`):
	
     `$ python -m serial.tools.miniterm  /decv/ttyUSB0 9600`

     *OJO: si no se configura python hay que ejecutar esto desde
     el directorio donde está instalado pyserial.*

     Para ver qué envia y recibe:

     `$ python -m serial.tools.miniterm --filter debug /decv/ttyUSB0 9600`

     En la práctica yo uso `screen`.


---
Manuel Perez (https://github.com/amanuellperez/mcu)


