
## Tocar música

Al intentar tocar una canción me encuentro con el problema de saber qué notas
tocar. ¿Cómo extraer esa información?

1. A mano: coges una partitura y la vas leyendo, anotando: corchea:Do,
   fusa:Mi, ... y con un script lo conviertes al formato adecuado.

   Así ha sido cómo he escrito la primera canción para probar mis funciones.
   Funciona pero es lento, aburrido y propenso a errores.

2. Automatizarlo: escanear una partitura y hacer un programa que traduzca la
   partitura en las notas correspondientes. Esto formaría parte de un proyecto
   de visión artificial y bastante interesante. El problema es que puede que
   lleve bastante tiempo hacerlo. Encaja con otro de los proyectos que ando
   haciendo de vez en cuando. Para no olvidar cómo puedo empezar esto debería
   de:
	* ¿Puedes identificar una línea? ¿En una hoja en blanco con una sola
	  línea? ¿Una hoja con muchas líneas? ¿una hoja con un paisaje y una
	  línea?

	* ¿Puedes identificar un pentagrama?

	* En una hoja con varios pentagramas, ¿puedes identificar cada uno por
	  separado?

	* En una hoja en blanco con solo una nota ¿puedes identificarla? ¿es
	  una negra? ¿una blanca? ¿una corchea? ...
	
    Si el programa es capaz de hacer esas cosas casi seguro que se pueda
    automatizar.

3. Usar MusicXML: parece ser que este es un formato para anotar partituras en
   el ordenador. Este camino puede que sea el más sencillo de seguir.

   [Aquí](https://www.w3.org/2021/06/musicxml40/) se puede encontrar el
   standard.

4. Extraer las notas a partir de un mp3???
    Aunque puede que esto sea bastante complicado a lo mejor es posible hacer
    una simplificación. Una primera idea sería hacer el análisis de Fourier de
    una canción e intentar simplificándolo para convertirlo en notas. ¿Se
    podrá hacer?


5. ¿Puedes hacerlo de otra forma? 


## Songs

En la carpeta songs voy a meter partituras descargadas de internet. Son sobre
todo para hacer pruebas. Los ficheros musicxml los he descargado de
[musicxml.com](https://www.musicxml.com/music-in-musicxml/example-set/). La música de
Beethoven y demás es de dominio público no teniendo derechos de autor. Lo
único que podría tener derecho de autor es el haberlo pasado al ordenador,
pero en la página dan a entender que lo ponen gratuitamente. Si tuviesen
derechos de autor y no se puediesen usar ruego me lo hagan saber para
borrarlos. Solo las quiero para probar el programa que funcione correctamente
y ver qué puedo hacer con él. 







