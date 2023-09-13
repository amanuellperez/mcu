# PROTOCOLOS

Todo device se conecta con el microcontrolador a través de un protocolo. Los
protocolos que podemos tener son:

* Pins

  Como el LCD que se conecta al micro a través de pins. Es la forma
  clásica de conectar hardware.

* One wire

* Two wire

* SPI

Por ello, la forma típica de parametrizar un device será:
	`Device <Micro, Protocol>`

## DISCIPLINA

+ El hardwador es el que conoce qué dispositivos va a montar y qué protocolo va
a usar cada uno. Por ello, es responsabilidad del hardwador inicializar todos
los protocolos de la forma correcta para que los dispositivos se puedan
comunicar con el micro. Típicamente el hardwador es responsable de escribir
los archivos `prj_dev.h`, donde define los dispositivos reales que se conectan
así como las interfaces genéricas que usará el softwador, y `prj_init.cpp`
donde inicializa todos los dispositivos.

+ Como el softwador desconoce el protocolo usado por el dispositivo para
  comunicarse con el micro, es responsabilidad del dispositivo conectarse y
  desconectarse.

  Imponer esto es imponer que el programa tiene que ser secuencial y que en
  las interrupciones tienen que ser funciones que no se comuniquen con otros
  dispositivos. 

  Por ejemplo: supongamos que en el `main` un dispositivo se está comunicando
  via TWI con el micro. De repente salta una interrupción: si la interrupción
  intenta conectarse a otro dispositivo via TWI estará corrompiendo la
  comunicación que se estaba realizando en el `main`. 

  Para evitar este tipo de problemas impongo esta disciplina: siempre que sea
  posible evitar que el micro se comunique con dispositivos via algún
  protocolo, para evitar colision de datos. (Notar el "siempre que sea
  posible". Si no es posible, repensamos todo el programa para que sea posible
  hacerlo)



## SPI devices

+ ¿Quién es responsable de encender/apagar el SPI del micro?

  Cada dispositivo encenderá el SPI, mientras que el programa principal podrá
  apagarlo si ve que no lo necesita. 

+ ¿Por qué cada dispositivo es responsable de encenderlo? 

  Si por cualquier motivo el programa desconecta el SPI no quiero, como programador, tener que
  estar recordando que si quiero abrir un  fichero primero tengo que conectar
  SPI. 


+ ¿Quién configura el dispositivo SPI?

  El driver del dispositivo ya que es el driver el que sabe a qué frecuencia
  conectarlo, cuál es la polaridad y la fase que usa el dispositivo, si
  primero va el MSB o no... Todas esas cosas dependen del dispositivo.


+ ¿Quién es responsable de diseñar cómo se seleccionan los diferentes
  dispositivos de la aplicación? 

  El hardwador es el que sabe cómo va a
  conectarlos. Él será el responsable de escribir la clase `Select_SPI` y de
  inicializarla correctamente en la aplicación de tal manera que el
  programador pueda ignorar por completo el cómo realmente se conecta a una
  tarjeta SD, o a una EEPROM, ...


