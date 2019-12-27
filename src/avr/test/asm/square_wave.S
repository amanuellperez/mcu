#include <avr/io.h> 

.global square_wave_fastest

; Esta es la onda cuadrada más rápida que puedo generar
; con 1 MHz de reloj genera una onda NO cuadrada de 4 us
; Está en on 1us, y en off 3 us (los 2 us a más supongo que serán 
; por culpa del rjmp repeat_fast)
square_wave_fastest:
    clr r16	; r16 = 0
;    ser r17	; r17 = 0xFF	; si se quieren encender todos los pines
    ldi r17, (1 << DDB0)	; 
    out _SFR_IO_ADDR(DDRB), r17	; todos (o solo el pin 0) los pines B de salida

repeat_fast:
    out _SFR_IO_ADDR(PORTB), r17 ; escribimos un 1
    out _SFR_IO_ADDR(PORTB), r16 ; escribimos un 0
    rjmp repeat_fast


; copiada de https://www.avrfreaks.net/forum/delay-function-assembly
.global square_wave
square_wave:
    ldi r17, (1 << DDB0)	; vamos a escribir en el pin PORTB0
    mov r2, r17			; r2 contiene el valor que escribimos
    out _SFR_IO_ADDR(DDRB), r17	; definimos el pin de salida


repeat:
    eor r2, r17			; r2 = r2^r17 // hacemos un flip-flop de r2
    out _SFR_IO_ADDR(PORTB), r2 ; escribimos su valor en el pin

;    ldi r16, 4			; si queremos esperar más tiempo

pierde_tiempo:			; ejecutamos 65536 ciclos
	sbiw r30, 1	    
	brne pierde_tiempo

;    dec r16
;    brne pierde_tiempo

    rjmp repeat			; volvemos a empezar



; Resultados de square_wave1.
; Llamo a esta función midiendo en el osciloscopio el periodo obtenido.
; Observar que el periodo viene a ser: T = 10 + 6.04t_on.
; Si pensamos que cada instrucción lleva 1 ciclo de reloj, el bucle donde
; genero el cambio del pin de 0->1 y viceversa tiene 6 instrucciones. Necesita 
; 6 ciclos, por eso el 6.04t. Todo parece cuadrar. 
; Por supuesto, estas mediciones solo funcionan con 1MHz de reloj. Pero si
; cambiamos la frecuencia del reloj es fácil ajustar las cuentas.
; t_on	|  T (us)
; ------+---------
;   1	| 16 us
;   2	| 22 us
;   3	| 28 us
;   4	| 34 us
;   5	| 40 us
;   10	| 71 us
;   100	| 618 us
;   200	| 1.224 us
;   255	| 1.556 us
;

.global square_wave1 ;(t_on, num_ciclos)
#define t_on  r16
#define num_ciclos r22	; 2º parámetro

square_wave1:
    mov t_on, r24		; copiamos el parámetro de square_wave1
    ldi r17, (1 << DDB0)	; vamos a escribir en el pin PORTB0
    mov r2, r17			; r2 contiene el valor que escribimos
    out _SFR_IO_ADDR(DDRB), r17	; definimos el pin de salida


repeat1:
    eor r2, r17			; r2 = r2^r17 // hacemos un flip-flop de r2
    out _SFR_IO_ADDR(PORTB), r2 ; escribimos su valor en el pin
 
; >>> delay(t_on) ---------------
    mov r3, t_on
pierde_tiempo1:			
    dec r3
    brne pierde_tiempo1
; <<< delay ---------------------


    dec num_ciclos
    brne repeat1

    ; dejamos en off el pin
;    clr r2
;    out _SFR_IO_ADDR(PORTB), r2 ; escribimos su valor en el pin
;    ret
    rjmp repeat1 

// --------------------------------------------------------------------------
//				MEDICIONES
// --------------------------------------------------------------------------
//
// 1.- Modificando num_ciclo1
//     ----------------------
//    num_ciclo0|num_ciclo1|      T
//    ----------+----------+-------------
//	1     |	   1	 |     20 us
//	1     |    2     |     26 us
//	1     |    3	 |     32 us
// 	1     |    4     |     38 us
//
// Suponiendo que T = A*num_ciclo1 + B, obtenemos que
//		    T = 6*num_ciclo1 + 14
//
// Predicción: si num_ciclo1 = 255 ==> T = 1544. Al mirarlo obtenemos T = 1566
// muy próximo al error predicho. El error procederá de la constante A = 6.
// Usando T(255) = 1566 = A*255 + 14 ==> A = 6.09:
//		    T = t.09*num_ciclo1 + 14
// que está de acuerdo con la tabla anterior.
// Según esta fórmula T(125) = 774, que coincide bastante bien con el valor
// medido experimentalmente.
//	
//
// 2.- Modificando num_ciclo0
//     ----------------------
//    num_ciclo0|num_ciclo1|      T
//    ----------+----------+-------------
//	1     |	   1	 |     20 us
//	2     |    1     |     32 us
//	3     |    1	 |     44 us
// 	4     |    1     |     57 us
//
// Suponiendo que T = A*num_ciclo0 + B, obtenemos que
//		    T = 12*num_ciclo1 + 8
// Predicción: T(255) = 3068. Medición experimental: T(255) = 3110, lo cual
// anda de acuerdo con nuestra predicción. Modificando la constante A para 
// que T(255) = A*255 + 8 ==> A = 12.16:
//		    T = 12.16*num_ciclo1 + 8
// que está de acuerdo con la tabla anterior. 
// Según esta fórmula T(125) = 1528, que coincide con el valor medido
// experimentalmente.
//
// 3.- Teóricamente
//     ------------
// Si suponemos que cada instrucción tarda 1 ciclo de reloj (casi cierto)
// el bucle pierde_tiempo2 lleva 4*num_ciclos0*(num_ciclos1 + 1), más las 4
// instrucciones de repeat2 obtenemos
//		    T = 4*num_ciclos0*(num_ciclos1 + 1) + 4
//
// Las fórmulas experimentales sugieren que sea
//		T = 6.08*num_ciclos0*(num_ciclos1 + 1) + B
//
// donde la constante B no queda bien determinada con las fórmulas anteriores.
// Hagamos unas cuantas predicciones:
//    num_ciclo0|num_ciclo1|T(experimental) |   B
//    ----------+----------+----------------+------
//	1       |    1	   |    20 us	    |  7.8
//	10	|   10	   |   678 us	    |  9.2
//	10	|   255	   |   15.60 ms	    |  ??? (no tengo resolución)
//	20	|   20	   |   2560 us	    |  6.4
//
//  Da la impresión de que B oscila. De todas formas, B no es tan importante
// cuando T es del orden de milisegundos (mi osciloscopio es incapaz de medir
// diferencias de 10 us), luego B solo es importante para pequeños valores de
// num_ciclos.
// 
//    num_ciclo0|num_ciclo1|      T     |   B
//    ----------+----------+------------+-------
//	1     |	   1	 |     20 us	|  7.8
//	2     |    2     |     45 us	|  8.5
//	3     |    3	 |     81 us	|  8.0
// 	4     |    4     |     130 us	|  8.4
//
//  Me voy a quedar con B = 9 (en medición 1 B era 8, en medición 2 valía 14,
// ¿por qué no sale en torno a 11?)
//
// Propuesta:
//	T(num_ciclos0, num_ciclos1) = 6.08*num_ciclos0*(num_ciclos1 + 1) + 9
// 
// Predicción: si quiero generar una onda cuadrada de 1 ms = 1000 us, tendré
// que num_ciclos0*(num_ciclos1 + 1) es +- 1000/6 = 166, cuya raiz cuadrada es
// 12 (truncada). Si cojo T(12, 12) = 957, mientras que T(12, 13) = 1030 us.
// Al generar esta señal oscila la medición entre 1028 y 1032, luego la
// predicción es muy buena


.global square_wave2 ;(num_ciclos0, num_ciclos1)
; Los registros que se pueden usar libremente dentro de una función en asm son
; los registros r18-r27 (ver nota de Atmel)
; pone que también se puede usar el r31. ¿y el r30? No dice nada
#define num_ciclos0	r24	// paramétro 1
#define num_ciclos1	r22	// paramétro 2
#define pin	    r18
#define valor	    r19	// valor que escribimos en el pin
#define counter0    r20
#define counter1    r21

square_wave2:
;    mov t_on, num_ciclos1	// copiamos el parámetro de square_wave1
    ldi pin, (1 << DDB0)	// vamos a escribir en el pin PORTB0
    mov valor, pin		// valor contiene el valor que escribimos
    out _SFR_IO_ADDR(DDRB), pin // definimos el pin de salida


repeat2:
    // pin = !pin;
    eor valor, pin		  // valor = valor^pin 
    out _SFR_IO_ADDR(PORTB), valor// escribimos su valor en el pin
 

; >>> delay(num_ciclos0, num_ciclos1)
    mov counter0, num_ciclos0
pierde_tiempo2:
;	>>> delay(num_ciclos1) --------
	mov counter1, num_ciclos1
pierde_tiempo_inner2:			
	    dec counter1
	    brne pierde_tiempo_inner2
;	<<< delay ---------------------
; <<< delay(num_ciclos0, num_ciclos1)
    
    dec counter0 
    brne pierde_tiempo2


    ; dejamos en off el pin
;    clr valor 
;    out _SFR_IO_ADDR(PORTB), valor // escribimos su valor en el pin
;    ret
    rjmp repeat2 



// Genera una onda cuadrada de periodo aproximado:
//		T = 6.08*num_ciclos0*(num_ciclos1 + 1) + 9
// Esta fórmula solo es válida frecuencia 1 MHz (esto es, T realmente nos da
// el número de ciclos que ejecuta esta función, de ahí el nombre).
// Genera num_pulsos.

.global square_wave_ciclos ;(num_ciclos0, num_ciclos1, num_pulsos)
#undef num_ciclos0
#undef num_ciclos1
#undef num_pulsos

#undef pin
#undef valor
#undef counter0
#undef counter1


; Los registros que se pueden usar libremente dentro de una función en asm son
; los registros r18-r27 (ver nota de Atmel)
; pone que también se puede usar el r31. ¿y el r30? No dice nada
#define num_ciclos0	r24	// paramétro 1
#define num_ciclos1	r22	// paramétro 2
#define num_pulsos r20	// paramétro 3

#define pin	    r18
#define valor	    r19	// valor que escribimos en el pin
#define counter0    r26
#define counter1    r27

square_wave_ciclos:
    // init();
;    mov t_on, num_ciclos1	// copiamos el parámetro de square_wave1
    ldi pin, (1 << DDB0)	// vamos a escribir en el pin PORTB0
;    mov valor, pin		// valor contiene el valor que escribimos
    in valor, _SFR_IO_ADDR(PORTB) // leemos el de PORTB. Solo  queremos
				// modificar el bit adecuado, y no tocar el resto
    out _SFR_IO_ADDR(DDRB), pin // definimos el pin de salida


repeat_us:
    // pin = !pin;
    eor valor, pin		  // valor = valor^pin 
    out _SFR_IO_ADDR(PORTB), valor// escribimos su valor en el pin
 

; >>> delay(num_ciclos0, num_ciclos1)
    mov counter0, num_ciclos0
pierde_tiempo_us:
;	>>> delay(num_ciclos1) --------
	mov counter1, num_ciclos1
pierde_tiempo_inner_us:			
	    dec counter1
	    brne pierde_tiempo_inner_us
;	<<< delay ---------------------
; <<< delay(num_ciclos0, num_ciclos1)

    dec counter0 
    brne pierde_tiempo_us


    dec num_pulsos
    brne repeat_us

    ; dejamos en off el pin
    clr valor 
    out _SFR_IO_ADDR(PORTB), valor // escribimos su valor en el pin
    ret



