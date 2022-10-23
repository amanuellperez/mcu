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

### Instalación
Para instalar la toolchain en linux (versión que suele estar muy anticuada,
así que mejor no la instales):

```
$ sudo apt install avrdude avrdude-doc binutils-avr avr-libc gcc-avr gdb-avr
```


## <a name="descargar_toolchain">DESCARGAR LA TOOLCHAIN</a>
* [GNU Binutils](https://ftp.gnu.org/gnu/binutils/)
* [GCC](https://ftp.gnu.org/gnu/gcc/)
* [avr-libc](https://github.com/avrdudes/avr-libc)

Para verificar los downloads del FTP de GNU, descargar la
[key](https://ftp.gnu.org/gnu/gnu-keyring.gpg) y ejecutar
```
$ gpg --verify --keyring ./gnu-keyring.gpg gcc-11.3.0.tar.xz.sig
```

(solo tiene el problema de que si el FTP ha sido hackeado la key va a ser
consistente con el programa descargado ... )

## <a name="compilar_toolchain">COMPILAR LA TOOLCHAIN</a>

### Fuentes de fiar
La página de 
[avr-libc](https://www.nongnu.org/avr-libc/user-manual/install_tools.html)
explica cómo instalar la toolchain.


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
      $ PREFIX=$HOME/root/bin/avr/
      $ export PREFIX
      ```

   2. Añadir al `PATH` el directorio de instalación de los bin: 
   
      Dejar el `PATH` completamente limpio para evitar que coja programas que
      no debe:

      ```
      $ PATH=$PREFIX/bin:/bin:/usr/bin
      $ export PATH 
      ```

      Notar que hago que busque primero en `$PREFIX/bin`.  A día de hoy tengo
      instalada una versión de las binutils en `/usr/lib`.

      Al compilar avr-gcc quiero que use las binutils que acabo de compilar,
      por eso necesito poner primero `$PREFIX/bin` para que encuentre la
      versión que yo acabo de compilar.

   3. Crear el directorio `$PREFIX`:

	    `$ mkdir $PREFIX`

4. Guardar la salida del `configure` en un fichero, para poder revisar lo que
ha hecho (en caso de error de compilación puede ser necesario).



### binutils: linker, assembler...
1. Descargarlo de la página que indique el link de ayuda dado al principio.
    
   Verificar la versión descargada:

   ```
   $ gpg --verify --keyring ./gnu-keyring.gpg binutils-2.33.1.tar.xz.sig
   ```

   Descomprimirlo.

2. Crear el directorio obj-avr dentro:
   ```
   $ mkdir obj-avr
   $ cd obj-avr
   ```
	
3. Configurarlo:
   ```
   $ ../configure --prefix=$PREFIX --target=avr --host=x86_64-linux-gnu \
			    --disable-nls > log 2>&1
   ```

   Notar que he añadido el `--host`. Sin el host daba un error de que no era
   capaz de reconocer el tamaño de un long long (¿por defecto considera que
   es una máquina de 32 bits?)

4. Compilarlo e instalarlo:

   ```
   $ make
   $ make install
   ```

   Me ha dado un error. Resulta que el `configure` no ha definido la variable
   interna `prefix=`. Escribo el `Makefile` poniendo en lugar de `prefix=`
   --> `prefix=${PREFIX}`.

6. Observar que deja instalados dos directorios bin:
   ```
   $PREFIX/bin	   # versiones: avr-as, ...
   $PREFIX/avr/bin # versiones: as, ld, ...
   ```
   Las dos parece ser necesarias. De hecho probé a eliminar la segunda
   y no podía configurar la compilación de `avr-libc`.

### avr-gcc: el compilador

Tener en cuenta que el `avr-gcc` no es más que el `gcc` compilado con `target =
avr`.

1. Instalar prerequisitos (GMP, MPFR...)
   Se hace automático ejecutando `./contrib/download_prerequisites`.

2. Crear el directorio de compilación **FUERA** de `gcc` (eso indica en la ayuda).

   Si lo tengo descomprimido en `cd-rom/gcc-9.2.0`, creo `cd-rom/obj-avr`:
   ```
   $ mkdir obj-avr
   $ cd obj-avr
   ```

3. Lo configuramos:(ver todas las posibles opciones 
   [en la página de gcc](https://gcc.gnu.org/install/configure.html))

   La vez que intenté compilarlo teniendo instalada la `toolchain` me 
   generó bastantes problemas compilarlo (fallaba en la biblioteca
   `libbacktrace`). Al final usé la configuración que tenía la toolchain de
   Atmel (se ve mirando la versión: `avr-g++ -v`) y compiló.

   La segunda vez, al compilarlo sin tener ninguna versión instalada de la
   toolchain, apenas ha generado problemas. 

   Configuro con las siguientes opciones:

   ```
   $ ../gcc-9.2.0/configure -v 
	    --prefix=${PREFIX}			
	    --build=x86_64-linux-gnu 
	    --host=x86_64-linux-gnu 
	    --target=avr 
	    --enable-languages=c,c++ 
	    --disable-nls 
	    --program-prefix=avr-
	    --program-suffix=-9.2.0
	    --without-included-gettext	# esta opción no aparece en la web de gnu???
	    --disable-libssp
	    # Elimino estas opciones que usé en la primera compilación.
	    # --enable-long-long  <-- esta no la he encontrado en 9.2.0
	    # --with-system-zlib <-- no encuentra esto
	    # --enable-shared  <-- avr soporta librerías dinámicas?
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

   * Usa DWARF 2 debugging information:	`--with-dwarf2`
     Aconsejan poner esto, pero no lo pongo. Es para depurar
     (yo no uso el depurador).

4. Lo compilamos e instalamos:
   ```
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
	    'libtool --finish /home/alp/bin/avr/libexec/gcc/avr/9.2.0'
   ```

   en internet pone que se puede ignorar. Lo ignoro y todo funciona.




POSIBLES ERRORES

1. En ubuntu por defecto se instalan en `/usr/lib/avr/bin`, así que es este el
   path a añadir. En caso de no hacer esto al intentar compilar da el error de
   que no puede enlazar con `as` ya que le pasamos la opción `-mcu=avr5`.

2. Si ahora al intentar enlazar un programa del avr dice que no encuentra
   `crtatmega328p.o`, es normal: falta instalar las `avrlibc`!!!



### avr-libc

Este es el más sencillo de compilar. No genera problemas.

1. Descomprimirlo.
2. Ejecutar:

   ```
   $ ./configure --prefix=$PREFIX --build=`./config.guess` --host=avr
   $ make
   $ make install
   ```

   Puede que falle el `./configure` indicando que no encuentra `avr-gcc`. Es
   porque al compilar `avr-gcc` se habrá generado el fichero `avr-gcc-11.3.0`
   en vez de `avr-gcc`. Crear `avr-gcc` como link de `avr-gcc-11.3.0`.

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

   `$ sudo usermod -a -G dialout alp`

   No olvidar reiniciar el ordendador.


3. Conectar el cable al AVR:

   Los cables son los siguientes:

   | color	| significado	|				|
   |-----------|---------------|-------------------------------|
   | negro	| tierra	|				|
   | amarillo	| RXD		| conectarlo al pin 3 del AVR	|
   | naranja	| TXD		| conectarlo al pin 2 del AVR	|



4. Para abrir un terminal de comunicación con el AVR, podemos:
   * Usar `screen`:

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
A.Manuel L.Perez (https://github.com/amanuellperez/mcu)


