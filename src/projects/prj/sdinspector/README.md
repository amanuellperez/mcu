## SD Inspector

## Finalidad inicial
¿Por qué esta aplicación?

1. Quiero implementar el FAT filesystem. Una forma sencilla de ver que voy
   entendiendo la documentación es ir comprobando que lo que leo lo veo
   realmente en una SD card real. En linux escribiré esa tarjeta, y con este
   programa comprobaré que se está escribiendo donde la documentación dice.

2. Este programa sirve para analizar a bajo nivel cualquier SD card. De hecho
   se podía evolucionar en un editor en hexadecimal (cosa que de momento no
   creo que haga; de momento no tengo demasiado interés en ello).


## Sobre la implementación

Posibles formas de implementarlo:

1. (lazy) Programo directamente todo en el avr. Problemas: el programa ocupa 
    mucho espacio en el micro; no puedo controlar el cursor ni la presentación
    de lo que ve el usuario de la aplicación.

2. Cliente-servidor: en el avr programamos un pequeño servidor para acceder a
   la SD Card, y es en el ordenador donde implementamos todas las funciones.
   Idealmente habría que escribir un driver para el ordenador que permitiría
   acceder a una aplicación genérica a esa tarjeta.

De momento lo implemento usando la opción 1, por ser la más rápida de
implementar (pero la menos portable, más dificil de mantener, ...)

