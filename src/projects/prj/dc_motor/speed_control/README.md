## Primeros pensamientos

Para controlar la velocidad de un DC-motor lo que hacemos es aplicar una señal
PWM al pin ENABLE. La señal PWM viene caracterizada por el duty cycle.

Claramente tiene que haber una relación entre la velocidad en revoluciones por
minuto (rpm) de giro del motor y el duty cycle.

Para ver la relación real que existe, midámoslo: la opción del menu "Automatic
table (percentage, speed)" lo que hace es generar esos datos. Usando el
terminal myterm es fácil guardarlos en un fichero que luego podemos procesar.

### Aproximación lineal

Aunque claramente la gráfica que se obtiene no es lineal, podemos aproximarlo
linealmente.  Usando Gnuplot, Octave o cualquier otro programa podemos
calcular mínimos cuadrados y obtener la función correspondiente. En las
primeras pruebas he obtenido:

rpm = 1.61 * percentage - 4.7

Esto sugiere un primer método para controlar la velocidad al motor *concreto*
que estoy usando: aproximo la fórmula anterior a 

rpm = 1.5 * percentage

y cuando el usuario solicite que quiere que el motor gire a una velocidad
determinada, usando la fórmula anterior calculamos el percentage para que el
motor gire a una velocidad lo más probable más baja que la que queremos pero
próxima y luego ajustamos la velocidad poco a poco hasta alcanzar la que
buscamos.

El método es:

1. Calcular percentage = 0.67 * rpm
2. Hacer girar el motor a esa velocidad.
3. Ajustar poco a poco la velocidad hasta obtener la que queremos con un error
   del "err %"

### Método aproximar desde 0

Otra forma de conseguir que el motor gire a la velocidad deseada consiste en
hacer el punto 3 anterior pero empezando desde 0.

Empezamos en el 25% (este motor por debajo del 25% no se mueve, salvo que le
ayudes a empezar con la mano) y vamos aumentando un 5% hasta obtener la
velocidad deseada. 



