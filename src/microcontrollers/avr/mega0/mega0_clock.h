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

#ifndef __MEGA0_CLOCK_H__
#define __MEGA0_CLOCK_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del "Clock Controller"
 *
 * HISTORIA
 *    Manuel Perez
 *    26/10/2024 Implementación mínima para configurar el reloj.
 *
 ****************************************************************************/
#include <avr/io.h>

namespace mega0_{

class Clock_controller{
public:
// Static interface
    Clock_controller() = delete;

// MCLKCTRLA
    // System clock is output to the CLKOUT pin?
    static void enable_system_clock_out(); 
    static void disable_system_clock_out();

    // Clock select
    static void clock_source_internal_16_20MHz(); // OSC20M
    static void clock_source_internal_32kHz();
    static void clock_source_external_32kHz();
    static void clock_source_external();

// MCLKCTRLB
    static void clk_main_no_prescaler();
    static void clk_main_divided_by_1() {clk_main_no_prescaler();}
    static void clk_main_divided_by_2();
    static void clk_main_divided_by_4();
    static void clk_main_divided_by_6();
    static void clk_main_divided_by_8();
    static void clk_main_divided_by_10();
    static void clk_main_divided_by_12();
    static void clk_main_divided_by_16();
    static void clk_main_divided_by_24();
    static void clk_main_divided_by_32();
    static void clk_main_divided_by_64();
    static void clk_main_divided_by_48();

// TODO: faltan el resto de registros
// MCLKLOCK
// MCLKSTATUS
// OSC20MCTRLA
// OSC20MCALIBA
// OSC20MCALIBB
// OSC32KCTRLA
// XOSC32KCTRLA
    
};

// MCLKCTRLA
// ---------
inline void Clock_controller::enable_system_clock_out()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL.MCLKCTRLA | CLKCTRL_CLKOUT_bm);}

inline void Clock_controller::disable_system_clock_out()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL.MCLKCTRLA & ~CLKCTRL_CLKOUT_bm);}

// Esto parece chino ^_^:
//  1) _PROTECTED_WRITE: es necesario porque son bits protegidos. No se pueden
//			 acceder de forma norma. (ver CPU.CCP)
//  2) (CLKCTRL.MCLKCTRLA & ~CLKCTRL_CLKSEL_gm) | CLKCTRL_CLKSEL_OSC20M_gc)
//     esto es igual a:
//	escribe en CLKCTRL.MCLKCTRLA los bits CLKCTRL_CLKSEL_OSC20M_gc en la
//	posición de la máscara CLKCTRL_CLKSEL_OSC20M_gc
inline void Clock_controller::clock_source_internal_16_20MHz()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, 
	(CLKCTRL.MCLKCTRLA & ~CLKCTRL_CLKSEL_gm) | CLKCTRL_CLKSEL_OSC20M_gc);};

inline void Clock_controller::clock_source_internal_32kHz()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, 
	(CLKCTRL.MCLKCTRLA & ~CLKCTRL_CLKSEL_gm) | CLKCTRL_CLKSEL_OSCULP32K_gc);};

inline void Clock_controller::clock_source_external_32kHz()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, 
	(CLKCTRL.MCLKCTRLA & ~CLKCTRL_CLKSEL_gm) | CLKCTRL_CLKSEL_XOSC32K_gc);};

inline void Clock_controller::clock_source_external()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, 
	(CLKCTRL.MCLKCTRLA & ~CLKCTRL_CLKSEL_gm) | CLKCTRL_CLKSEL_EXTCLK_gc);};


// MCLKCTRLB (10.3.5: register under configuration change protection)
// ---------
inline void Clock_controller::clk_main_no_prescaler()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0); }

inline void Clock_controller::clk_main_divided_by_2()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_4()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_4X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_6()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_6X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_8()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_8X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_10()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_10X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_12()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_12X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_16()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_16X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_24()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_24X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_32()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_32X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_64()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_64X_gc | CLKCTRL_PEN_bm);}

inline void Clock_controller::clk_main_divided_by_48()
{_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_48X_gc | CLKCTRL_PEN_bm);}


}// namespace
 

#endif


