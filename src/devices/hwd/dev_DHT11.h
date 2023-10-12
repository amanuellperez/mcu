// Copyright (C) 2023 Manuel Perez 
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

#ifndef __DEV_DHT11_H__
#define __DEV_DHT11_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del DHT11.
 *
 * HISTORIA
 *    Manuel Perez
 *    12/10/2023 Escrito
 *
 ****************************************************************************/
#include <dev_train_of_pulses.h>
#include <atd_concepts.h>
#include <atd_float.h>

namespace dev{

template <typename Micro0, uint8_t dht_pin>
class DHT11{
public:
// Types
    using Micro = Micro0;
    using Pin   = typename Micro::Pin<dht_pin>;

    enum class Result{
	ok,
	train_of_pulses_bad_polarity,
	train_of_pulses_bad_size,
	bad_check_sum
    };

// Preconditions
    // (RRR) Train_of_pulses_poll_receiver no es capaz de capturar bien 
    //	     el tren de pulsos enviado por el DHT11. Para eliminar este assert
    //	     es necesario cambiar la forma de leer el tren de pulsos.
    static_assert(Micro::frequency >= 8'000'000,
		  "Low frequency, change it to at least 8MHz");


    static bool basic_read(uint8_t data[5]);

    template <Type::Decimal Float>
    static bool read(Float& T, Float& RH);

    // En caso de que la última operación haya fallado, aquí se pueden
    // encontrar los detalles.
    static Result result_last_operation() {return result_;}

private:
// State
    inline static Result result_;

// Internal Cfg
    static constexpr uint8_t nmax_pulses = 42; // ACK + 5 bytes data + end
    static constexpr uint8_t sensor_pin = dht_pin;

// Internal types
    using Disable_interrupts = typename Micro::Disable_interrupts;
    using Cfg_train = dev::Train_of_pulses_poll_receiver_cfg<Micro,
					  sensor_pin, 
					  nmax_pulses>;

    using Train_of_pulses_receiver = 
				dev::Train_of_pulses_poll_receiver<Cfg_train>;

    using Train_of_pulses = dev::Train_of_pulses<nmax_pulses>;

    static void receive_polling(Train_of_pulses& pulse);
    static void pulse2data(const Train_of_pulses& pulse, uint8_t data[5]);
    static bool is_pulse_corrupted(const Train_of_pulses& pulse);
    static uint8_t check_sum(uint8_t data[5]);
    static bool is_check_sum_right(uint8_t data[5]);
    
};


// Functions
// ---------
template <typename M, uint8_t npin>
bool DHT11<M, npin>::is_pulse_corrupted(const Train_of_pulses& pulse)
{
    if (pulse.polarity() == 0){
	result_ = Result::train_of_pulses_bad_polarity;
	return true;
    }

    if (pulse.size() != nmax_pulses){
	result_ = Result::train_of_pulses_bad_size;
	return true;
    }

    return false;
}


template <typename M, uint8_t npin>
void DHT11<M, npin>::pulse2data(const Train_of_pulses& pulse, uint8_t data[5])
{
    // Datos a leer
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;

//  pulse[0] == pulse de 80us. Como no medimos tiempo, sino ticks, no se cómo
//  confirmar que se esté enviando realmente este pulso.

    // CUIDADO: los valores que contienen los datos son pulse[1..40], pero el
    // array es data[0...4]
    for (uint8_t i = 0; i < 40; ++i){
	data[i / 8] <<= 1;

	if (pulse[i + 1].time_high > pulse[i + 1].time_low)
	    //atd::write_bit<0>::to<1>::in(data[i / 8]); <-- en este caso esto
	    //lía más que aclara @_@
	    data[i / 8] |= 0x01;

    }
// pulse[41] == pulse end. El DHT11 mantiene por 50 us low y luego release the
// pin.

}

template <typename M, uint8_t npin>
void DHT11<M, npin>::receive_polling(Train_of_pulses& pulse)
{
    // Enviamos al DHT11 la señal para que empiece
    Pin::write_zero();
    Pin::as_output();
    Micro::wait_ms(20);
    Pin::as_input_without_pullup(); // release the bus
    Micro::wait_us(1); // tiempo para que pase de 0 -> 1 (tarda 0.7 us)

    Train_of_pulses_receiver::receive(pulse);
}

template <typename M, uint8_t npin>
bool DHT11<M, npin>::basic_read(uint8_t data[5])
{
    result_ = Result::ok;

    Disable_interrupts lock{};

    Train_of_pulses  pulse;
    receive_polling(pulse);

    if(is_pulse_corrupted(pulse))
	return false;

    pulse2data(pulse, data);

    if (!is_check_sum_right(data)){
	result_ = Result::bad_check_sum;
	return false;
    }

    return true;

}

template <typename M, uint8_t npin>
inline uint8_t DHT11<M, npin>::check_sum(uint8_t data[5])
{ return (data[0] + data[1] + data[2] + data[3]) & 0xFF; }

template <typename M, uint8_t npin>
inline bool DHT11<M, npin>::is_check_sum_right(uint8_t data[5])
{ return (check_sum(data) == data[4]); }



// Observar que estoy duplicando el ' result_ = Result::ok'
// ya que la función `basic_read` también lo hace, no siendo necesario hacerlo
// aquí. 
//
// Lo dejo aquí para que se vea bien la lógica. Si mañana cambio basic_read,
// no voy a acordarme de retocar esta función (si he eliminado el result_ =
// Result::ok)
template <typename M, uint8_t npin>
template <Type::Decimal Float>
bool DHT11<M, npin>::read(Float& T, Float& RH)
{
    uint8_t data[5];

    result_ = Result::ok;

    if (!basic_read(data))
	return false;

 

    atd::Integer_and_decimal_part{data[0], data[1]}.as(RH);
    atd::Integer_and_decimal_part{data[2], data[3]}.as(T);

    return true;
}


}// namespace dev
#endif

