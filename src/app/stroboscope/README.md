# Stroboscope

### basic
Para experimentar.

La idea va a ser generar un pulso de unos 500 us cada cierto tiempo. Estos
serán los flashes por minuto.

Si se programa el micro a 1 MHz se puede generar frecuencias desde 1 Hz hasta
1 KHz, mientras que si se programa a 8 MHz hasta 1KHz.

Para poder generar todo el rango de frecuencias es necesario usar 2 divisores
de la frecuencia del timer.

### v0
Primer intento. A 8MHz parece funcionar bien.

Le interfaz de 3 botones no parece muy cómodo para seleccionar la frecuencia.

