
// Esperamos un número determinado de ciclos
// La idea es tener entretenido al avr un número determinado de ciclos. Para
// ello nos limitamos a restar un contador un número determinado de veces.
//
// Copiada de https://www.avrfreaks.net/forum/delay-function-assembly
// 
// DUDA: uso el registro r30 porque así lo hacían en avrfreaks. Sin embargo, 
// en la nota de Atmel sobre mezclar asm y C, dicen que se pueden usar el 
// registro r31, dentro de una función, sin preocuparse de restaurar el valor
// que tenían inicialmente, pero no dicen nada del r30. ¿es correcto? Si no, 
// basta con usar otro registro, o 
.global delay_ciclos;(num_ciclos)

#define num_ciclos  r24	// parámetro 1
#define counter	    r30 // contador de 16 bits

delay_ciclos:
    mov counter, num_ciclos // counter = num_ciclos;

pierde_tiempo:		    // do{
	sbiw counter, 1	    // --counter;
	brne pierde_tiempo  // }while (counter > 0)

    ret


