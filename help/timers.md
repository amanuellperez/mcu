# TIMERS

De momento lo que sigue hace referencia a los Timers del atmega32. 


Lo primero que conviene distinguir es entre el hardware y cómo se usa dicho
hardware.

## Timer (hwd)

La clase `Timer` será el traductor del hardware correspondiente. Esta clase se
limita a traducir la datasheet a código, usando su misma notación.

## Usos del Timer

(Esta clasificación es temporal y la iré modificando a medida que vaya usando
el Timer y coja más experiencia)

El Timer lo podemos usar de varias formas. De momento las formas que he
identificado son:

1. Como contador de tiempo, `Time_counter`, que usaremos para medir el tiempo.
   Ejemplos: 

    * medir duración de pulsos (tanto recibidos como generados). En este caso
      usaremos la clase `Miniclock`

    * implementar un RTC. Usaremos `Clock`.

2. Como generador de ondas cuadradas: `Square_wave_generator`, que podríamos
   abreviar como `SWG`. Dos tipos:

   * Para generar tren de pulsos de corta duración necesitamos una clase
     eficiente en la generación. De momento llamo a esa clase
     `Square_wave_burst_generator`.

   * Cuando la eficiencia no sea necesaria es más sencilla de usar 
     `Square_wave_generator` (`SWG` mejor?


3. Como generador de PWM. 


