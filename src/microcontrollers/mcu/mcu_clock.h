// Copyright (C) 2019-2023 Manuel Perez 
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

#ifndef __MCU_CLOCKS_H__
#define __MCU_CLOCKS_H__
/****************************************************************************
 *
 * DESCRIPCION
 *
 *  Concebimos un `Time_counter` como `Clock`. Para ello:
 *	(1) Elegimos la unidad del tick del `Time_counter` como milisegundo,
 *	    en el `Clock_ms`, o como segundo en el `Clock_s`.
 *	    (si no se puede elegir directamente lo emulamos)
 *
 *	(2) Ampliamos la resolución del `Time_counter`.
 *	    Ejemplo: 
 *		Clock_ms<Time_counter, uint32_t> clock;
 *
 *		La resolución del clock es `uint32_t.Time_counter::counter`
 *
 *  Ver dev_miniclocks.h para más documentación.
 *
 * CLOCKS
 *	+ Clock_s : reloj de sistema (similar a std::system_clock).
 *	+ Clock_ms: reloj que mide el tiempo en milisegundos.
 *
 *
 * HISTORIA
 *    Manuel Perez
 *    ??/??/2020 Escrito
 *    26/02/2021 Basado en Generic_timer
 *    11/04/2022 Reestructurado. Chronometer_ms
 *    06/01/2023 Clock_s/Clock_ms
 *
 ****************************************************************************/

#include <chrono>

#include <atd_time.h>
#include <atd_cast.h>

namespace mcu{

/***************************************************************************
 *				CLOCK_S
 ***************************************************************************/
// Si se quieren operar con los time_points hacer Rep_t signed.
// Voy a seguir el estilo de std::system_clock
//
// Observar que hay 2 tipos de Clock_s:
//  1) Un medidor de segundos: han pasado 300.000 segundos desde que
//     arrancaste el microcontrolador (o desde la hora que pusiste).
//
//  2) Un calendario, que nos da la fecha y la hora. 
//     En este caso 300.000 segundos no se muestra como 300.000 segundos sino
//     que se calcula la fecha correpondiente, cálculo para el que hay que
//     tener en cuenta los años bisiestos (respecto de la epoch) y que cada 
//     mes tiene un número diferente de días.
//
//  La diferencia entre un reloj y otro no es el funcionamiento interno sino
//  cómo se presentan los datos al usuario 
//
//		¿300 000 segundos vs 20/02/2022 13:52:47?
//
// Voy a suministrar un doble interfaz (manejar solo segundos, o manejar
// fechas) para que sea el usuario el que elija.
//
// Al principio solo lo implementé para funcionar con Timers que sean
// capaces de generar 1 tick cada segundo, como el Timer1.
// Pero ¿cómo implementarlo en el caso del Timer0 incapaz de dar 1 tick cada
// segundo?.
//
// EJEMPLO
//  Pensemos en un ejemplo concreto. Supongamos que el micro funciona a 1MHz,
//  el periodo del reloj es de 1 us.
//
//  Queremos generar 1 tick cada segundo. El reloj de los times del avr lo
//  podemos dividir entre 1, 8, 64, 256 ó 1024. Esto supone que los ticks que
//  generaremos con estos timers serán múltiplos de estos números.
//
//  Ejemplo: si divisor = 64, y el micro va a 1MHz el periodo del reloj del
//  Timer será de 64 us. Para generar 1 tick cada segundo debemos de contar
//	    1 seg = 1 000 000 us = 64 * 15625
//  15625. Cuando el timer alcance este valor sabremos que ha transcurrido 1
//  segundo (+- la incertidumbre del reloj). El Timer1 es de 16 bits capaz de
//  contar hasta 15625, pero el Timer0 es de 8 bits no pudiendo contar más de
//  255.
//
//  Si queremos usar el Timer0 descompongamos 15625 = 125 * 125, y llevemos la
//  cuenta de cuántos ticks genera Timer0 (genera un tick cada vez que lanza
//  una interrupción, cada vez que alcanza 125). Cuando el número de ticks sea
//  igual a 125 quiere decir que hemos contado hasta 15625, contando 1 segundo
//  más.
//
//  Para el Timer0 la función `tick()` debería de ser algo del tipo:
//	    if (nticks < 125)
//		++nticks;
//	    else {
//		nticks = 0;
//		++nseconds;
//	    }
//
//  Para el Timer1 basta con llevar la cuenta de nseconds, y una función
//  `tick` que se limita a `++nseconds`. El Timer0 necesita dos contadores
//  `nticks` y `nseconds` y una función `tick` un poco más complicada. 
//
//  ¿Qué diferencia hay entre Timer0 y Timer1? Que en el Timer0 hay que dar
//  125 ticks para alcanzar 1 segundo, mientras que en Timer1 hay que dar 1
//  tick para alcanzarlo. 
//
//  Si llamamos nticks_1s al número de ticks que hay en 1 segundo, el código
//  anterior quedaría generalizado:
//	    ++nticks;
//	    if (nticks >= nticks_of_1s){
//		nticks = 0;
//		++nseconds;
//	    }
//
//  El nticks_1s nos lo puede devolver la función Time_counter::turn_on();
//  Esta es una forma de hacerlo genérico y que sirva para Timer0 y Timer1.
//  Voy a probarla.
//	
template <typename Micro, typename Time_nticks_g>
class Clock_s
{
public:
    using Disable_interrupts = Micro::Disable_interrupts;
    using Time_counter	= Time_nticks_g;

    using duration	= std::chrono::seconds;
    using rep		= duration::rep;
    using period	= duration::period;

    using time_point	= std::chrono::time_point<Clock_s, duration>;
    using Date_time     = atd::Date_time;

    static constexpr bool is_steady = true;

    // DUDA: como siempre la duda con los nombres:
    //	    on/off???, start/stop???, xxx/yyy???
    // En un cronómetro me suena mejor start/stop, pero en un reloj que lo
    // normal es que lo enciendas y no lo apagues, ahora me suena mejor on/off
    // (problema: apostamos a que en unos días me suena mejor otra cosa? @_@)
    /// on() enciende el reloj desde 0. Recordar ponerlo en hora con set()
    //
    // Devuelve true si se inicializa correctamente el reloj, y false en caso
    // de que haya algún error.
    static bool turn_on();

    /// Apaga el reloj.
    static void turn_off();

    /// Ponemos en hora el reloj, pasándo el número de segundos que marca el
    /// reloj.
    // (???) en lugar de set se podía llamar `now`. 
    static void set(const rep& nseconds);

    /// Ponemos en hora el reloj, pasándo la Date_time correspondiente.
    static void set(const Date_time& t);

    static time_point now() noexcept;

    static Date_time now_as_date_time();

    /// Damos un tick al clock.
    /// Obligatorio llamar a esta función dentro de la ISR correspondiente.
    static void tick();

    /// Convertimos el time_point a Date_time 
    static Date_time as_date_time(const time_point& t);

    /// En Timer0 se llama a la ISR un montón de veces antes de que haya
    /// transcurrido un nuevo segundo. ¿Cómo saber cúando ha transcurrido un
    /// nuevo segundo? Esta función nos lo dice. Devuelve true en caso de que
    /// haya transcurrido un nuevo segundo.
    static bool is_new_second();

private:
    using counter_type = Time_counter::counter_type;

    inline static volatile counter_type nticks_;
    inline static volatile counter_type nticks_of_1s_;
    inline static volatile rep nseconds_;

    static void reset();

};


template <typename M, typename TC>
void Clock_s<M, TC>::reset()
{
    nseconds_ = 0;
    nticks_   = 0;
}

template <typename M, typename TC>
bool Clock_s<M, TC>::turn_on()
{
    Disable_interrupts lock{};

    reset();

    nticks_of_1s_ = Time_counter::turn_on_with_overflow_to_count_1s();

    // (???) aqui enable_interrupts o que lo defina el cliente 
    // Micro::enable_interrupts();

    if (nticks_of_1s_ == Time_counter::minus_one)
	return false;

    return true;
}

template <typename M, typename TC>
inline void Clock_s<M, TC>::turn_off()
{ Time_counter::turn_off(); }


// (RRR) La hago inline por ser la función que se llama en la ISR
template <typename M, typename TC>
inline void Clock_s<M, TC>::tick() 
{
    nticks_ = nticks_ + 1;

    if (nticks_ >= nticks_of_1s_){
	nticks_ = 0;
	nseconds_ = nseconds_ + 1;
    }
}

template <typename M, typename TC>
inline bool Clock_s<M, TC>::is_new_second()
{ return nticks_ == 0; }

template <typename M, typename TC>
inline Clock_s<M, TC>::time_point Clock_s<M, TC>::now() noexcept
{
    Disable_interrupts lock{};

    return time_point{duration{nseconds_}};
}

template <typename M, typename TC>
inline Clock_s<M, TC>::Date_time Clock_s<M, TC>::now_as_date_time()
{
    return as_date_time(now());
}


template <typename M, typename TC>
void Clock_s<M, TC>::set(const rep& nseconds)
{
    Disable_interrupts lock{}; 
    nseconds_ = static_cast<volatile rep>(nseconds);
    nticks_   = 0;
}

template <typename M, typename TC>
inline void Clock_s<M, TC>::set(const Date_time& t)
{
    set(t.to_time_t());
}


template <typename M, typename TC>
Clock_s<M, TC>::Date_time Clock_s<M, TC>::as_date_time(const time_point& t)
{
    Date_time dt;
    dt.from_time_t(static_cast<std::time_t>(nseconds_));

    return dt;
}



/***************************************************************************
 *			    Clock_ms
 ***************************************************************************/
// TODO: modificar esta clase de forma similar a Clock_s para que pueda
// funcionar con Timer0 y Timer2 también. Ahora solo funciona con Timer1.
template <typename Micro, typename Time_counter_g>
class Clock_ms
{
public:
    using Disable_interrupts = Micro::Disable_interrupts;
    using Time_counter	= Time_counter_g;
    using duration	= std::chrono::milliseconds;
    using rep		= duration::rep;
    using period	= duration::period;

    using time_point	= std::chrono::time_point<Clock_ms, duration>;
    using Time          = atd::Time_ms;

    static constexpr bool is_steady = true;

    // DUDA: como siempre la duda con los nombres:
    //	    on/off???, start/stop???, xxx/yyy???
    // En un cronómetro me suena mejor start/stop, pero en un reloj que lo
    // normal es que lo enciendas y no lo apagues, ahora me suena mejor on/off
    // (problema: apostamos a que en unos días me suena mejor otra cosa? @_@)
    /// on() enciende el reloj desde 0. 
    static void turn_on();

    // Lo dicho: ahora me suena mejor `start`. Motivo? `on` suena a
    // `enciendete` pero no refleja el hecho de que empezamos desde '0',
    // mientras que `start` indica 'empieza', lo cual si da una idea de que
    // empezamos desde 0.
    static void start() {turn_on();}

    /// Enciende el reloj sin reinicializar el counter.
    static void resume();

    /// Apaga el reloj reiniciar el contador.
    static void turn_off();

    static void stop() {turn_off();}


    // Todas las funciones set/reset no encienden/apagan el Clock_ms. Si
    // estaba encendido sigue encendido, si apagado, apagado. Lo que modifican
    // es el contador del Clock_ms.
    // Aunque tiene más sentido este set, pasarle el std::chrono::milliseconds
    // directamente, puede que sea más sencillo de usar el set(rep) ???
    static void set(const duration& nmilliseconds);

    template <typename Rep, typename Period>
    static void set(const std::chrono::duration<Rep, Period>& d)
    { set(std::chrono::duration_cast<duration>(d)); }

    /// Ponemos en hora el reloj, pasándo la Time correspondiente.
    static void set(const Time& t);

    /// Pone el contador a 0 (sin alterar su estado: si estaba encendido,
    /// sigue encendido)
    static void reset() {set(rep{0});}

    // TODO: Hay demasiadas funciones now():
    //	1) La del time_point está inspirada en std pero no la veo práctica
    //	   aquí.
    //	2) now_as_duration() da la impresión de ser más práctica que 1)
    //	3) now_as_time() es para mostrar la duración directamente en formato
    //	   humano que es algo que tienen que hacerse siempre.
    //	Conclusión: no tengo claro cómo hacer esto. Que el uso marque el
    //	camino.
    static time_point now() noexcept;

    static duration now_as_duration() noexcept;

    static Time now_as_time();

    /// Damos un tick al clock.
    static void tick() {nmilliseconds_ = nmilliseconds_ + 1;}

private:
    inline static volatile rep nmilliseconds_; // = nmilliseconds

    static rep now_() noexcept;

    /// Ponemos en hora el reloj, pasándo el número de milisegundos que marca el
    /// reloj.
    // (???) en lugar de set se podía llamar `now`. 
    static void set(const rep& nmilliseconds);
};

template <typename M, typename TC>
void Clock_ms<M, TC>::turn_on()
{
    Disable_interrupts lock{};

    nmilliseconds_ = 0;
    Time_counter::turn_on_with_overflow_every_1ms();

    resume();

    // (???) aqui enable_interrupts o que lo defina el cliente 
    // Micro::enable_interrupts();
}

template <typename M, typename TC>
inline void Clock_ms<M, TC>::resume()
{
    Time_counter::turn_on_with_overflow_every_1ms();
}


template <typename M, typename TC>
inline void Clock_ms<M, TC>::turn_off()
{
    Time_counter::turn_off();
}

template <typename M, typename TC>
inline Clock_ms<M, TC>::rep Clock_ms<M, TC>::now_() noexcept
{
    Disable_interrupts lock{};

    return nmilliseconds_;
}

template <typename M, typename TC>
inline Clock_ms<M, TC>::duration Clock_ms<M, TC>::now_as_duration() noexcept
{
    return duration{now_()};
}

template <typename M, typename TC>
inline Clock_ms<M, TC>::time_point Clock_ms<M, TC>::now() noexcept
{
    return time_point{now_as_duration()};
}

template <typename M, typename TC>
inline Clock_ms<M, TC>::Time Clock_ms<M, TC>::now_as_time()
{
    return Time{duration{now_()}};
}


template <typename M, typename TC>
void Clock_ms<M, TC>::set(const rep& nseconds)
{
    Disable_interrupts lock{}; 
    nmilliseconds_ = static_cast<volatile rep>(nseconds);
}

template <typename M, typename TC>
inline void Clock_ms<M, TC>::set(const duration& t)
{
    set(t.count());
}

template <typename M, typename TC>
inline void Clock_ms<M, TC>::set(const Time& t)
{
    set(t.as_milliseconds());
}


}// dev

#endif


