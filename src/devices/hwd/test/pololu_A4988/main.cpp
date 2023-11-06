// Copyright (C) 2019-2020 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
//
// This file is part of the MCU++ Library.
//
// MCU++ Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// Primera prueba para ver si todo conecta bien
#include "../../avr_pin.h"
#include "../../avr_time.h"
#include "../../avr_UART.h"


using namespace avr;

struct Num_grados_n{};
struct Num_steps_n{};

// Al definir grados como int16_t podemos ir desde -32.768 hasta 32.767, que
// supone que podemos dar como mucho 91 vueltas (91*360 = 32.760)
// TODO: he eliminado Integral_type. Ahora está atd_magnitude. Crear Degree o
// algo así para manejar grados sexagesimales.
using Num_grados = alp::Integral_type<Num_grados_n, int16_t>;

// Al definir stes como uint16_t podemos dar hasta 327 vueltas en el sentido
// que se haya definido (al ser unsigned no permitimos pasar el sentido de
// giro en el Num_steps) (327 vueltas con la resolución de 1'8º = 200
// steps/vuelta)
using Num_steps  = alp::Integral_type<Num_steps_n, uint16_t>;

/*!
 *  \brief  Driver A4988, pololu.
 *
 *  Para definir el sentido de giro mirar el stepper motor desde arriba. El
 *  sentido + es cuando giramos en sentido antihorario (definición matemática
 *  habitual). El sentido - es cuando giramos en sentido horario.
 *
 */
class Pololu_A4988{
public:
    // Pines que tiene el A4988
    struct Pin_DIR{uint8_t v;};
    struct Pin_STEP{uint8_t v;};

    /// Definimos las conexiones del A4988
    Pololu_A4988(Pin_STEP step, Pin_DIR dir);

    /// Gira al stepper los grados indicados. 
    /// Si grados > 0, gira en sentido positivo
    /// Si grados < 0, gira en sentido negativo
    /// Toma el control del microcontrolador y hasta que no acabe de girar
    /// no devuelve el control.
    void gira(Num_grados grados);

    // -----------------------
    // Funciones de bajo nivel
    // -----------------------
    /// Fijamos el sentido de giro positivo.
    /// Esta función no afecta a gira(grados), ya que en 'grados' se incluye
    /// el sentido de giro.
    void gira_en_sentido_positivo() 
    {direccion_.write_one();}

    /// Fijamos el sentido de giro negativo.
    /// Esta función no afecta a gira(grados), ya que en 'grados' se incluye
    /// el sentido de giro.
    void gira_en_sentido_negativo() 
    {direccion_.write_zero();}

    /// Gira el número de steps indicado. 
    /// Esta función no define el sentido de giro, steps siempre es
    /// positivo!!! El sentido hay que definirlo antes con
    /// gira_en_sentido_positivo/negativo.
    void gira(Num_steps steps);

private:
    Pin direccion_;
    Pin step_;

    // número de milisegundos que vamos hacer el pulso
    // Para que funcione mínimo 350 us. Esto se traduce en la práctica en un
    // pulso de 420 us (debido a las llamadas a función y demás; la función
    // añade 140 microsegundos).
    static constexpr uint16_t step_us_low2high = 350;
};

Pololu_A4988::Pololu_A4988(Pin_STEP step, Pin_DIR dir)
    :direccion_{dir.v}, step_{step.v}
{
    // configuración de los pines
    direccion_.de_salida();
    step_.de_salida();
}


// Gira al stepper los grados indicados. 
// Si grados > 0 gira en sentido +, si es < 0, en sentido -.
void Pololu_A4988::gira(Num_grados grados)
{
    if (grados > Num_grados{0})
	direccion_.write_one();
    else {
	direccion_.write_zero();
	grados *= -1;
    }

    // Fundamental: 1.- Cuidado con usar uint y las conversiones implicitas.
    //		    2.- int32_t = garantizamos que no haya overflow; 
    //				(ojo: no puedo hacer la división primero, ya
    //				que perdería steps y no me movería todo lo que
    //				quisiera)
    // (200/360)*grados = 10*g/18;
    int16_t num_steps = (Num_grados{10}*grados)/Num_grados{18};

    // Con la resolución de 1'8º no hay ningún problema con el static_cast, ya
    // que grados < 2^15, y num_steps < 2^15, y Num_steps < 2^16
    gira(Num_steps{static_cast<uint16_t>(num_steps)});
}

void Pololu_A4988::gira(Num_steps num_steps)
{
    // minimo 15 de tiempo, a 60 ya empieza a sonar algo
    // Para ver lo que tarda, hacer num_steps muy grande (por ejemplo, 20000)
    // De esa forma se genera una señal que se ve muy bien en el osciloscopio.
    // Al hacerlo obtengo con 15 ciclos pulsos de 810 us. El while y el
    // num_steps-- ocupan unos 60 us.
    // Si pongo 10 ciclos, genera pulsos de 340 us y el motor ya no es capaz
    // de seguirlos.
    while (num_steps--){
	step_.square_wave(Num_ciclos{10}, Num_ciclos{10}, 1);
    }
}



int main() 
{
    Pololu_A4988 pololu{Pololu_A4988::Pin_STEP{12}, Pololu_A4988::Pin_DIR{13}};

    auto usart = UART::init();
    usart << "Iniciado correctamente\n\r";

    while(1){
//	usart << "45\n\r";
//	pololu.gira(Num_grados{45});
//	wait_ms(1000u);
//    
//	usart << "-45\n\r";
//	pololu.gira(Num_grados{-45});
//	wait_ms(1000u);

	usart << "180\n\r";
	pololu.gira(Num_grados{180});
	wait_ms(1000u);

	usart << "-180\n\r";
	pololu.gira(Num_grados{-180});
	wait_ms(1000u);

	usart << "270\n\r";
	pololu.gira(Num_grados{270});
	wait_ms(1000u);

	usart << "-270\n\r";
	pololu.gira(Num_grados{-270});
	wait_ms(1000u);

	usart << "360\n\r";
	pololu.gira(Num_grados{360});
	wait_ms(1000u);

	usart << "-360\n\r";
	pololu.gira(Num_grados{-360});
	wait_ms(1000u);

	usart << "steps = 20000\n\r";
	pololu.gira(Num_steps{20000});
	wait_ms(1000u);

//	usart << "giro en sentido positivo\n\r\n\r";
//	pololu.gira_en_sentido_positivo();
//
//	usart << "50\n\r";
//	pololu.gira(Num_steps{50u});
//	wait_ms(1000u);
//
//	usart << "100\n\r";
//	pololu.gira(Num_steps{100u});
//	wait_ms(1000u);
//
//	usart << "150\n\r";
//	pololu.gira(Num_steps{150u});
//	wait_ms(1000u);
//
//	usart << "200\n\r";
//	pololu.gira(Num_steps{200u});
//	wait_ms(1000u);
//
//	usart << "giro en sentido negativo\n\r\n\r";
//	pololu.gira_en_sentido_negativo();
//
//	usart << "50\n\r";
//	pololu.gira(Num_steps{50u});
//	wait_ms(1000u);
//
//	usart << "100\n\r";
//	pololu.gira(Num_steps{100u});
//	wait_ms(1000u);
//
//	usart << "150\n\r";
//	pololu.gira(Num_steps{150u});
//	wait_ms(1000u);
//
//	usart << "200\n\r";
//	pololu.gira(Num_steps{200u});
//	wait_ms(1000u);
    }


    return 0;
}


