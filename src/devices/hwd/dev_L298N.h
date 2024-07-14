// Copyright (C) 2024 Manuel Perez 
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

#pragma once

#ifndef __DEV_L298N_H__
#define __DEV_L298N_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del L298N 
 *
 * HISTORIA
 *    Manuel Perez
 *    09/06/2024 Escrito
 *
 ****************************************************************************/
#include <cstdint>
#include <atd_math.h>	// Sign
#include <atd_percentage.h>

namespace dev{

// Si pin_IN2 == pin_IN1 se entiende que están conectados 
// usando una puerta NOT via hardware, solo necesitando escribirse
// pin_IN1 para controlar el signo del potencial
template <typename PWM_pin0, 
	  uint8_t Pin_IN1, uint8_t Pin_IN2 = Pin_IN1>
struct L298N_pin_cfg{
    using PWM_pin = PWM_pin0;
    static constexpr uint8_t IN1 = Pin_IN1;
    static constexpr uint8_t IN2 = Pin_IN2;
};


// (RRR) ¿por qué el interfaz habla de potencial y no de motores?
//	 El L298N son dos puentes H con capacidad de 2A por cada puente.
//	 Además, al meter una señal PWM en el pin enable la salida la podemos
//	 concebir como una señal de potencial variable desde -Vs hasta +Vs,
//	 siendo Vs el potencial analógico al que está conectado el L298N.
//	 
//	 CUIDADO: aunque se concibe como he escrito en el párrafo anterior la
//	 señal no va a ser una señal continua de potencial, sino una señal
//	 PWM. Para manejar un motor, aplicación típica, un primer modelo sería
//	 el anterior (pero es un modelo muy simple: ¿qué error se comete?)
template <typename Micro0,
	  typename Pin1_cfg, typename Pin2_cfg,
	  uint32_t L298N_frequency_in_Hz = 500> // DUDA: qué frecuencia es la mejor?
						//       funciona de 50Hz a 2KHz
class L298N_basic{
public:
// Constructors
    L298N_basic() = delete;	// static interface
    void init() {}

// port 1
// ------
    // Cambia el signo del potencial al indicado, sin cambiar el valor
    // absoluto del potencial de salida
    static void voltage1_sign(atd::Sign sign);

    // La salida del puerto1 será el p por cien de Vs. No cambia el signo del
    // potencial.
    static void voltage1_percentage(const atd::Percentage& p);

    // Cambia tanto el signo como el valor del potencial mostrado 
    static void voltage1(atd::Sign sign, const atd::Percentage& p);

    // Para la señal generada por PWM_pin1
    static void stop1();

// Info
    // Devuelve el duty cycle usado para generar el voltage1
    // El potencial medio de salida del port1 será Vs * voltage1_percentage().
    static atd::Percentage voltage1_percentage();

    // Signo del potencial de salida del port1
    static atd::Sign voltage1_sign();

// port 2
// ------
    // Cambia el signo del potencial al indicado, sin cambiar el valor
    // absoluto del potencial de salida
    static void voltage2_sign(atd::Sign sign);

    // La salida del puerto1 será el p por cien de Vs. No cambia el signo del
    // potencial.
    static void voltage2_percentage(const atd::Percentage& p);

    // Cambia tanto el signo como el valor del potencial mostrado 
    static void voltage2(atd::Sign sign, const atd::Percentage& p);

    // Para la señal generada por PWM_pin1
    static void stop2();

// Info
    // Devuelve el duty cycle usado para generar el voltage2
    // El potencial medio de salida del port2 será Vs * voltage2_percentage().
    static atd::Percentage voltage2_percentage();

    // Signo del potencial de salida del port2
    static atd::Sign voltage2_sign();

private:
// types
    using Micro = Micro0;

    // pins   
    using PWM_pin1 = Pin1_cfg::PWM_pin; // == EN_A
    using IN1 =  typename Micro::Pin<Pin1_cfg::IN1>;
    using IN2 =  typename Micro::Pin<Pin1_cfg::IN2>;

    using PWM_pin2 = Pin2_cfg::PWM_pin; // == EN_B
    using IN3 =  typename Micro::Pin<Pin2_cfg::IN1>;
    using IN4 =  typename Micro::Pin<Pin2_cfg::IN2>;

    using PWM_signal1 = PWM_pin1::PWM_signal;
    using PWM_signal2 = PWM_pin2::PWM_signal;

    using Frequency = PWM_signal1::Frequency;

    static_assert(std::is_same_v<Frequency, typename PWM_signal2::Frequency>);


// cfg
    static constexpr 
	Frequency L298N_frequency = Frequency{L298N_frequency_in_Hz};

// Helpers
    static void write1_zero();
    static void write2_zero();
};


// port 1
// ------
template <typename M, typename P1, typename P2, uint32_t F>
void L298N_basic<M, P1, P2, F>::voltage1(atd::Sign sign, const atd::Percentage& p)
{
    voltage1_sign(sign); 
    voltage1_percentage(p);
}


template <typename M, typename P1, typename P2, uint32_t F>
void L298N_basic<M, P1, P2, F>::voltage1_percentage(const atd::Percentage& p)
{
    if (p == 0)
	write1_zero();

    else if (p == 1)
	PWM_pin1::write_one();

    else {
	PWM_signal1 pwm{L298N_frequency, p};
	PWM_pin1::generate(pwm);
    }
}

template <typename M, typename P1, typename P2, uint32_t F>
atd::Percentage L298N_basic<M, P1, P2, F>::voltage1_percentage()
{
    if (PWM_pin1::is_zero())
	return 0;

    if (PWM_pin1::is_one())
	return 100;

    return PWM_pin1::duty_cycle();
}

template <typename M, typename P1, typename P2, uint32_t F>
inline void L298N_basic<M, P1, P2, F>::voltage1_sign(atd::Sign sign) 
{
    if (sign == atd::Sign::positive){
	IN1::write_one();
	if constexpr (IN1::number != IN2::number)
	    IN2::write_zero();
    }

    else {
	IN1::write_zero();
	if constexpr (IN1::number != IN2::number)
	    IN2::write_one();
    }
}


template <typename M, typename P1, typename P2, uint32_t F>
inline atd::Sign L298N_basic<M, P1, P2, F>::voltage1_sign() 
{
    if constexpr (IN1::number != IN2::number){
	if (IN1::is_one() and IN2::is_zero()) 
	    return atd::Sign::positive;

	else
	    return atd::Sign::negative;
    }

    else {
	if (IN1::is_one()) 
	    return atd::Sign::positive;

	else
	    return atd::Sign::negative;

    }

}


template <typename M, typename P1, typename P2, uint32_t F>
inline void L298N_basic<M, P1, P2, F>::write1_zero() 
    { PWM_pin1::write_zero(); }


template <typename M, typename P1, typename P2, uint32_t F>
inline void L298N_basic<M, P1, P2, F>::stop1() 
{
    PWM_pin1::write_zero(); // garantizamos que genere 0
    PWM_pin1::stop();
}



// port 2
// ------
template <typename M, typename P1, typename P2, uint32_t F>
void 
L298N_basic<M, P1, P2, F>::voltage2(atd::Sign sign, const atd::Percentage& p)
{
    voltage2_sign(sign); 
    voltage2_percentage(p);
}


template <typename M, typename P1, typename P2, uint32_t F>
void L298N_basic<M, P1, P2, F>::voltage2_percentage(const atd::Percentage& p)
{
    if (p == 0)
	write2_zero();

    else if (p == 1)
	PWM_pin2::write_one();

    else {
	PWM_signal2 pwm{L298N_frequency, p};
	PWM_pin2::generate(pwm);
    }
}

template <typename M, typename P1, typename P2, uint32_t F>
atd::Percentage L298N_basic<M, P1, P2, F>::voltage2_percentage()
{
    if (PWM_pin2::is_zero())
	return 0;
   
    if (PWM_pin2::is_one())
	return 100;

    return PWM_pin2::duty_cycle();
}

template <typename M, typename P1, typename P2, uint32_t F>
inline void L298N_basic<M, P1, P2, F>::voltage2_sign(atd::Sign sign) 
{
    if (sign == atd::Sign::positive){
	IN3::write_one();
	if constexpr (IN3::number != IN4::number)
	    IN4::write_zero();
    }

    else {
	IN3::write_zero();
	if constexpr (IN3::number != IN4::number)
	    IN4::write_one();
    }
}

template <typename M, typename P1, typename P2, uint32_t F>
inline atd::Sign L298N_basic<M, P1, P2, F>::voltage2_sign() 
{
    if constexpr (IN3::number != IN4::number){
	if (IN3::is_one() and IN4::is_zero()) 
	    return atd::Sign::positive;

	else
	    return atd::Sign::negative;
    }

    else {
	if (IN3::is_one()) 
	    return atd::Sign::positive;

	else
	    return atd::Sign::negative;

    }
}


template <typename M, typename P1, typename P2, uint32_t F>
inline void L298N_basic<M, P1, P2, F>::write2_zero() 
    { PWM_pin2::write_zero(); }


template <typename M, typename P1, typename P2, uint32_t F>
inline void L298N_basic<M, P1, P2, F>::stop2() 
{
    PWM_pin2::write_zero(); // garantizamos que genere 0
    PWM_pin2::stop();
}
}// namespace



#endif


