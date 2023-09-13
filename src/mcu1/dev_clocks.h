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

#ifndef __DEV_CLOCKS_H__
#define __DEV_CLOCKS_H__
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

namespace dev{

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
// TODO: De momento solo está pensado para funcionar con Timers que sean
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
//  Recordar si se implementa el Timer0 no penalizar la implementación de
//  Timer1 por culpa del Timer0.
template <typename Micro, typename Time_counter_g>
class Clock_s
{
public:
    using Disable_interrupts = Micro::Disable_interrupts;
    using Time_counter	= Time_counter_g;
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
    static void on();

    /// Apaga el reloj reiniciar el contador.
    static void off();

    /// Ponemos en hora el reloj, pasándo el número de segundos que marca el
    /// reloj.
    // (???) en lugar de set se podía llamar `now`. 
    static void set(const rep& nseconds);

    /// Ponemos en hora el reloj, pasándo la Date_time correspondiente.
    static void set(const Date_time& t);

    static time_point now() noexcept;

    static Date_time now_as_date_time();

    /// Damos un tick al clock.
    static void tick() {counter_ = counter_ + 1;}

    /// Convertimos el time_point a Date_time 
    static Date_time as_date_time(const time_point& t);

private:
    inline static volatile rep counter_;

};

template <typename M, typename TC>
void Clock_s<M, TC>::on()
{
    Disable_interrupts lock{};

    counter_ = 0;
    Time_counter::turn_on_with_overflow_every_1s();

    // (???) aqui enable_interrupts o que lo defina el cliente 
    // Micro::enable_interrupts();
}

template <typename M, typename TC>
inline void Clock_s<M, TC>::off()
{
    Time_counter::off();
}


template <typename M, typename TC>
inline Clock_s<M, TC>::time_point Clock_s<M, TC>::now() noexcept
{
    Disable_interrupts lock{};

    return time_point{duration{counter_}};
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
    counter_ = static_cast<volatile rep>(nseconds);
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
    dt.from_time_t(static_cast<std::time_t>(counter_));

    return dt;
}



/***************************************************************************
 *			    Clock_ms
 ***************************************************************************/
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
    static void on();

    // Lo dicho: ahora me suena mejor `start`. Motivo? `on` suena a
    // `enciendete` pero no refleja el hecho de que empezamos desde '0',
    // mientras que `start` indica 'empieza', lo cual si da una idea de que
    // empezamos desde 0.
    static void start() {on();}

    /// Enciende el reloj sin reinicializar el counter.
    static void resume();

    /// Apaga el reloj reiniciar el contador.
    static void off();

    static void stop() {off();}


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
void Clock_ms<M, TC>::on()
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
inline void Clock_ms<M, TC>::off()
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



// >>> BORRAME
///***************************************************************************
// *			    CHRONOMETER_MS
// ***************************************************************************/
//
///// El tiempo lo podemos medir en milisegundos o escribirlo como
///// hh:mm::ss::ms.
///// Ejemplo: 2h 3min 4s 25ms = 7384025 ms
///// Esta clase es responsable de pasar de una representación a otra.
////
//// ¿Por qué devolver las horas? ¿Por qué no devolver también días?
//// La resolución la marca el tipo usado para representar los milisegundos.
//// Si usamos int32_t podemos representar hasta 2^31 - 1 ms = que vienen a ser
//// unas 600 horas. 
//// Si en lugar de int32_t se usara int64_t podríamos representar hastsa
//// 2^63 - 1 ms = con lo que se podría representar años perfectamente.
//// En principio la idea es hacer stopwatch y timers no necesitando en la
//// práctica  medir hasta las horas (usar un int16_t no serviría ya que solo se
//// podrían representar hasta 30 segundos, demasiado poco tiempo).
//struct _Sexagesimal_ms{
//    int16_t milliseconds; // 00-999
//    int8_t seconds;// 00-59
//    int8_t minutes;// 00-59
//    int8_t hours;  // 00-23
//
//    _Sexagesimal_ms() {}
//
//    // conversión de ms a sexagesimal
//    _Sexagesimal_ms(int32_t ms);
//
//    // conversión de sexagesimal a ms
//    int32_t to_milliseconds() const;
//
//    // std usa count(). La suministro por si en el futuro lo generalizo y lo
//    // podemos integrar con std::duration.
//    int32_t count() const {return to_milliseconds();}
//
//};
//
//
//
//inline bool operator==(const _Sexagesimal_ms& a,
//	               const _Sexagesimal_ms& b)
//{
//    return a.milliseconds == b.milliseconds
//	and a.seconds == b.seconds
//	and a.minutes == b.minutes
//	and a.hours == b.hours;
//}
//
//inline bool operator!=(const _Sexagesimal_ms& a,
//	               const _Sexagesimal_ms& b)
//{
//    return !(a == b);
//}
//
//
//
//
//
//
//// Tenemos 3 periodos al manejar timers (aunque escribo frecuencia son
//// periodos, suena mejor con frecuencia):
////  1. T_clock = frecuencia con la que opera el reloj del avr.
////  2. T_timer = frecuencia a la que opera el timer. Al timer le pasamos un
////	         preescaler, un divisor 'd' que hace que T_timer = d*T_clock.
////  3. T_interrupt = frecuencia a la que se genera la interrupción. 
////               DUDA: T_int = top * T_timer ó T_int = (top + 1)*T_timer???
//template <typename Time_counter, uint16_t timer_period_in_us>
//constexpr inline typename Time_counter::counter_type __Chronometer_ms_top()
//{
//    constexpr uint32_t one_millisecond_in_us = 1000u;
//    constexpr uint32_t top = one_millisecond_in_us/timer_period_in_us;
//
//    static_assert(top < Time_counter::max_top(),
//                  "Top too great for this timer. Try another period or choose "
//                  "a different F_CPU.");
//    return atd::safe_static_cast<typename Time_counter::counter_type, uint32_t, top>();
//}
//
//
//// * Restricciones: solo un Chronometer_ms por aplicación (culpa de que usa static
////   milliseconds_).
//// * (RRR) Voy a implementarlo siguiendo el formato de los relojes de std.
//// * tick_up = indica si cada tick incrementa o decrementa el contador.
////	     Esto es, si el tiempo va hacia adelante o hacia atras.
////
//// * mejoras (???): parametrizarlo con la representación y el ratio, de esta
////   forma se puede elegir que sea un chronometro que funcione en ms ó us.
////   ¿Merece la pena hacerlo? En lugar de contar milisegundos lo que cuenta
////   son ticks. Los ticks podrán ser ms o us.
//template <typename Time_counter,   // = Generic_timer
//	 uint16_t timer_period_in_us, 
//	 bool tick_up = true // up or down?
//	 > 
//struct Chronometer_ms {
//    using duration   = std::chrono::duration<int32_t, std::milli>;
//    using rep        = duration::rep;
//    using period     = duration::period;
//    using time_point = std::chrono::time_point<Chronometer_ms, duration>;
//
//    // milliseconds in sexagesimal representation
//    using Sexagesimal_ms = _Sexagesimal_ms;
//
//    // Para recordar que solo hay un Chronometer_ms por aplicación no permito
//    // construir objetos. 
//    Chronometer_ms() = delete;
//
//    /// init chronometer. Lo ponemos a 0.
//    constexpr static void init()
//    {
//	Time_counter::
//	    init(__Chronometer_ms_top<Time_counter, timer_period_in_us>());
//
//        reset();
//    }
//
//    /// Enciende el cronometro. Recordar definir el tiempo antes.
//    static void on()
//    { Time_counter::template on<timer_period_in_us>(); }
//
//    /// Para el cronómetro sin borrar el tiempo actual.
//    static void off() { Time_counter::off(); }
//
//
//// Lectura
//    // la incluyo para que sea similar a un clock std
//    static time_point now() noexcept 
//    {return time_point{duration{milliseconds_}};}
//
//    // en un chronometro parece más útil esta funcion que no now() (<-- la
//    // elimino?) 
//    static rep count() {return milliseconds_;}
//    static Sexagesimal_ms sexagesimal_count() 
//    { return Sexagesimal_ms{milliseconds_}; }
//
//
//// Escritura
//    static void reset() 
//    { 
//	milliseconds_ = 0; 
//	Time_counter::reset();
//    }
//
//    static void count(const Sexagesimal_ms& sexag)
//    { milliseconds_ = sexag.count(); }
//
//
//    // precondition: state == stop
//    static void add(duration incr_t0) 
//    {
//	duration incr_t{incr_t0};
//	// milliseconds_ += incr_t.count();
//	milliseconds_ = milliseconds_ + incr_t.count();
//    }
// 
//
//    // precondition: state == stop
//    static void substract(duration incr_t0) 
//    {
//	duration incr_t{incr_t0};
//	if (milliseconds_ > incr_t.count())
//	    // milliseconds_ -= incr_t.count();
//	    milliseconds_ = milliseconds_ - incr_t.count();
//	else 
//	    milliseconds_ = 0;
//    }
//
//
//    /// Damos un tick al clock. Esta función se llamará desde la interrupción
//    /// correspondiente.
//    static void tick() 
//    {
//	if constexpr (tick_up)
//	    //++milliseconds_;
//	    milliseconds_ = milliseconds_ + 1;
//	else
//	    // --milliseconds_;
//	    milliseconds_ = milliseconds_ - 1;
//    }
//
//
//private:
//    // por culpa de que tiene que ser volatile no se puede definir como
//    // duration (sus operators no son volatiles)
//    inline static volatile rep milliseconds_;
//
//};
// <<< BORRAME
}// dev

#endif


