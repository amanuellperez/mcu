# avr MEGA0 family

## Default clock frequency

Datasheet: 10.3.3 el prescaler por defecto es 6. No he encontrado el valor por
defecto que trae OSC20M de fábrica. En el chip que tengo son 16MHz en cuyo
caso por defecto funciona a 2 666 666 Hz.

### `F_CPU`

El reloj del `atmega4809` se configura dinámicamente (el fuse2 permite elegir
el reloj interno de 16 ó 20 MHz), con lo que `F_CPU` no parece tener mucho
sentido. 

Por otra parte las funciones `_delay_us/ms` usan `F_CPU`, luego si se quiere
llamar a `Micro::wait_ms` es obligatorio (a día de hoy) tener definido `F_CPU`
y que este valor coincida con el valor programado en el micro.


