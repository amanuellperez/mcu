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
 *	Traductor del DHT_protocol y DHT11 driver
 *	(¿separarlo en dos archivos de cabecera? `dht_protocol.h`y `dht11.h'?)
 *
 * HISTORIA
 *    Manuel Perez
 *    12/10/2023 Escrito
 *
 ****************************************************************************/
#include <dev_train_of_pulses.h>
#include <atd_concepts.h>
#include <atd_bit.h>
#include <atd_float.h>
#include <atd_magnitude.h>
#include <utility>

namespace dev{
/***************************************************************************
 *				DHT PROTOCOL
 ***************************************************************************/
// DHT11 y DHT22 tienen el mismo protocolo de comunicación (varían en el
// tiempo de espera para despertar al sensor: 1ms para el DHT22 y 20 ms para
// el DHT11)
class DHT_protocol{
public:
// Types
    enum class Result{
	ok,
	train_of_pulses_bad_polarity,
	train_of_pulses_bad_size,
	bad_check_sum
    };


    // Bloquea durante unos 4 ms la ejecución
    template <typename Micro, uint8_t npin, uint8_t start_time_ms>
    static Result basic_read(uint8_t data[5]);


private:

// Internal Cfg
    static constexpr uint8_t nmax_pulses = 42; // ACK + 5 bytes data + end

// Internal types
    using Train_of_pulses = dev::Train_of_pulses<nmax_pulses>;

    template <typename Micro, uint8_t npin, uint8_t start_time_ms>
    static void receive_polling(Train_of_pulses& pulse);

    static void pulse2data(const Train_of_pulses& pulse, uint8_t data[5]);
    static Result is_pulse_corrupted(const Train_of_pulses& pulse);
    static uint8_t check_sum(uint8_t data[5]);
    static bool is_check_sum_right(uint8_t data[5]);
    
};


template <typename Micro, uint8_t npin, uint8_t start_time_ms>
void DHT_protocol::receive_polling(Train_of_pulses& pulse)
{
    // (RRR) Train_of_pulses_poll_receiver no es capaz de capturar bien 
    //	     el tren de pulsos enviado por el DHT_protocol. Para eliminar este assert
    //	     es necesario cambiar la forma de leer el tren de pulsos.
    static_assert(Micro::clock_frequency_in_hz >= 8'000'000,
		  "Low frequency, change it to at least 8MHz");

    using Pin   = typename Micro::Pin<npin>;
    using Cfg_train = dev::Train_of_pulses_poll_receiver_cfg<Micro,
					  npin, 
					  nmax_pulses>;
    using Train_of_pulses_receiver = 
				dev::Train_of_pulses_poll_receiver<Cfg_train>;

    // Enviamos al DHT_protocol la señal para que empiece
    Pin::write_zero();
    Pin::as_output();
    Micro::wait_ms(start_time_ms);
    Pin::as_input_without_pullup(); // release the bus
    Micro::wait_us(1); // tiempo para que pase de 0 -> 1 (tarda 0.7 us)


    Train_of_pulses_receiver::receive(pulse);

}

template <typename Micro, uint8_t npin, uint8_t start_time_ms>
DHT_protocol::Result 
    DHT_protocol::basic_read(uint8_t data[5])
{
    typename Micro::Disable_interrupts lock{};

    Train_of_pulses  pulse;
    receive_polling<Micro, npin, start_time_ms>(pulse);

    Result result = (is_pulse_corrupted(pulse));
    if (result != Result::ok)
	return result;

    pulse2data(pulse, data);

    if (!is_check_sum_right(data))
	result = Result::bad_check_sum;

    return result;

}

inline uint8_t DHT_protocol::check_sum(uint8_t data[5])
{ return (data[0] + data[1] + data[2] + data[3]) & 0xFF; }

inline bool DHT_protocol::is_check_sum_right(uint8_t data[5])
{ return (check_sum(data) == data[4]); }





/***************************************************************************
 *				DHT11
 ***************************************************************************/
// Fundamental a tener en cuenta:
// + When power is supplied to sensor, don't send any instruction to the 
//   sensor within one second to pass unstable status
//
// + Collecting period should be : >1 second. (esperar 1 segundos entre
//   medidas).
// 
// + Todas las funciones son secuenciales, bloquean al micro hasta que 
//   acaban de ejecutarse.
//
template <typename Micro0, uint8_t dht_pin>
class DHT11{
public:
// Types
    using Micro = Micro0;
    using Result = DHT_protocol::Result;

    // Aumenta 1kB el código del datalogger al pasar de Decimal a Float16.
    // pero Float16 es experimental. Revisarla!!!
    //using Celsius  = atd::Celsius<atd::Decimal<int32_t, 1>>;
    using Celsius  = atd::Celsius<atd::Float16>;
    using Humidity = atd::Decimal<int32_t, 1>;

// Functions
    // El valor devuelto será válido si result_last_operation == ok
    static std::pair<Celsius, Humidity> read();


// Interfaz unificado para sensores
    static Celsius read_temperature();
    static Humidity read_humidity();


// Gestión de errores
    // En caso de que la última operación haya fallado, aquí se pueden
    // encontrar los detalles.
    static Result result_last_operation() {return result_;}
    static bool last_operation_is_ok() {return result_ == Result::ok;}
    static bool last_operation_fail() {return !last_operation_is_ok();}


private:
// Data
    inline static Result result_;

// Internal cfg
    static constexpr uint8_t start_time_ms = 20;

// Special values
    static constexpr Celsius null_temperature = Celsius{-1};
    static constexpr Humidity null_humidity   = Humidity{-1};
};


template <typename M, uint8_t npin>
std::pair<typename DHT11<M, npin>::Celsius, 
	  typename DHT11<M, npin>::Humidity> 
    DHT11<M, npin>::read()
{
    uint8_t data[5];

    result_ = DHT_protocol::basic_read<Micro, npin, start_time_ms>(data);
    if (result_ != Result::ok)
	return {null_temperature, null_humidity};

    Humidity H{data[0], data[1]};
    Celsius T{Celsius::Rep{data[2], data[3]}};


    return {T, H};
}

template <typename M, uint8_t npin>
inline DHT11<M, npin>::Celsius DHT11<M, npin>::read_temperature()
{
    auto [T, H] = read();
    
    return T;
}

template <typename M, uint8_t npin>
inline DHT11<M, npin>::Humidity DHT11<M, npin>::read_humidity()
{
    auto [T, H] = read();
    
    return H;
}


}// namespace dev
#endif

