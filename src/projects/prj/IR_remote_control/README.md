## Protocols
* [NEC protocol](https://techdocs.altium.com/display/FPGA/NEC%2bInfrared%2bTransmission%2bProtocol)
* RC5 protocol:
    1. [Altium](https://techdocs.altium.com/display/FPGA/Philips+RC5+Infrared+Transmission+Protocol)
    2. [Wikipedia](https://en.wikipedia.org/wiki/RC-5)


## Cosas básicas

* Según la wikipedia la frecuencia del LED es en torno a 870 nm y de 930-950
  nm, pero las referencias que da no son de fiar. 

* En internet la leyenda urbana que parece circular es que son 940 nm, sería
  interesante poder comprobarlo midiendo la de los mandos para ver cuál es la
  frecuencia real que emiten.

	    
## Driver del LED

* Algunos kits, como los de Elegoo, conectan el LED directamente al
  microcontrolador lo cual es una mala idea: el micro genera 5V mientras que
  el LED quiere estar a 1.1V.


* Por otra parte el microcontrolador suministra unos 20 mA lo cual es muy poca
  corriente para conseguir que el LED se comunique con el sensor. De hecho el
  primer intento que hice tenía que colocar el LED justo al lado del sensor
  para que leyera algo.

* El LED hay que conectarlo al micro via un driver. El primer driver que me ha
  funcionado es el indicado en el "Art of electronics", página 81, figura
  2.16. Se trata de un emitter-follower seguido de un switch. Para el
  transistor del emitter-follower he usado un 2N3904 y para el switch un
  2N2222. La resistencia del switch es de 40 ohmios. Como las resistencias que
  tengo son de 250 mW he conectado 4 de 200 ohmios en paralelo. Al medir el
  funcionamiento de este circuito no llego a generar ni siquiera 100 mA en el
  LED (a 38kHz, 50% duty cycle). He leído por ahi (<-- esto es, no tiene por
  qué ser de fiar) que estos LED se pulsan con PWM a 1% de duty con 1A de
  corriente (500 mA mínimo) para conseguir mucha distancia. Ya iré probando.


