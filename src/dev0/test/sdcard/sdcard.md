## Conexión
+ Conectar una resistencia de 10k (¿por qué de este valor? más? menos? motivos???)
en el pin MISO en serie entre la SD card. Si no se conecta esta resistencia da
un error al intentar programarlo (driver contention)


## Problemas encontrados
1. Al intentar programar el avr conectado a la SD card no programaba. Se
 trataba de 'driver contention'. La app note AVR910 resuelve el
 problema: basta con conectar resistencias en serie en los pines
 SCK/MISO/MOSI entre la sd card y el micro. De esa forma se desacopla el
 potencial en las lineas SCK/MISO/MOSI del micro leyendo correctamente
 los valores cuando se quiere programar.

2. El cmd0 ha funcionado desde el principio, pero el cmd8 devuelve:
	0x00 00 00 02 1F
 lo cual es incorrecto ya que le estoy enviando un pattern == AA y lo
 devuelve como 1F.

 La forma de resolver este problema es dar marcha atrás parcialmente al 
 punto 1. Al medir con el oscilloscopio veo que la señal de SCK/MOSI
 están muy amortiguadas por culpa de la resistencia que he puesto en
 (1). De hecho el reloj apenas parece reloj. Elimino las resistencias de
 SCK/MOSI manteniendo la de MISO y todo funciona bien.

 No puedo eliminar la de MISO sino el programador me da error.



## Leyendas urbanas:
Se suele enviar 0xFF antes y después de seleccionar el chip. En un
foro indicaba que la tarjeta necesita pulsos de reloj SCK para
terminar lo que estaba haciendo. Esto no aparece documentado en
ninguna parte pero está muy difundido en internet. ¿Es necesario?
¿aparece en la documentación?

