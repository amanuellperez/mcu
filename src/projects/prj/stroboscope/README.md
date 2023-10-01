# Stroboscope

### basic
Para experimentar.

La idea va a ser generar un pulso de unos 500 us cada cierto tiempo. Estos
serán los flashes por minuto.

Si se programa el micro a 1 MHz se puede generar frecuencias desde 1 Hz hasta
1 KHz, mientras que si se programa a 8 MHz hasta 3KHz.

Para poder generar todo el rango de frecuencias es necesario usar 2 divisores
de la frecuencia del timer.

#### Conclusiones:
Es un error intentar generar un pulso cada 500 us, es mejor generar un pulso
con un duty cycle del 10%. Para entender por qué imaginar una rueda girando.
Lo que buscamos es iluminar la rueda mientras gire un cierto ángulo, por
ejemplo, 10º. Esto supondría un duty cycle del 3%. Un 10% de duty supone que
tenemos iluminada la rueda en el tiempo que gira 36º. Da la impresión de que
con 10% funciona bien. El uso lo dirá.



### v0
Primer intento. A 8MHz parece funcionar bien.


