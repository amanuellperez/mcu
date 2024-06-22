// Copyright (C) 2020 Manuel Perez 
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

#include "../../dev_BMP280_basic.h"

#include <atd_cstddef.h>

#include <avr_atmega.h>	
#include <dev_TWI_master.h>

// Microcontroller
// ---------------
namespace my_mcu = atmega;
using Micro   = my_mcu::Micro;



// TWI Protocol
// ------------
constexpr uint8_t TWI_buffer_size = 100;
using TWI_master_cfg = dev::TWI_master_cfg<Micro, 
                                           my_mcu::TWI_basic,
					   TWI_buffer_size>;

using TWI_master  = dev::TWI_master<TWI_master_cfg>;
using TWI = dev::TWI_master_ioxtream<TWI_master>;

// 50 kHz es la unica frecuencia de TWI que va a 1MHz.
// 100 kHz a 8 MHz
static constexpr int TWI_frecuency = 50; // kHz


// Devices
// -------
static constexpr TWI::Address sensor_twi_address = 0x77;
using Sensor = dev::BMP280_TWI<TWI_master, sensor_twi_address>;


// En el breakout de adafruit la dirección la determina la conexión del pin
// SDO:
//	si SDO = GND	    ==> slave_address = 0x76
//	si SDO = flotante   ==> slave_address = 0x77


// para depurar
static constexpr uint8_t slave_address = 0x77;

void twi_print_state(TWI::iostate st)
{
    my_mcu::UART_iostream uart;
    uart << "state = ";

    switch(st){
	case TWI::iostate::ok:
	uart << "ok\n";
	break;

	case TWI::iostate::read_or_write:
	uart << "read_or_write\n";
	break;

	case TWI::iostate::sla_w:
	uart << "sla_w\n";
	break;

	case TWI::iostate::sla_r:
	uart << "sla_r\n";
	break;

	case TWI::iostate::no_response:
	uart << "no_response\n";
	break;

	case TWI::iostate::transmitting:
	uart << "transmitting\n";
	break;

	case TWI::iostate::eow:
	uart << "eow\n";
	break;

	case TWI::iostate::eow_data_nack:
	uart << "eow_data_nack\n";
	break;

	case TWI::iostate::error_buffer_size:
	uart << "error_buffer_size\n";
	break;

	case TWI::iostate::receiving:
	uart << "receiving\n";
	break;

	case TWI::iostate::eor_bf:
	uart << "eor_bf\n";
	break;

	case TWI::iostate::eor:
	uart << "eor\n";
	break;

	case TWI::iostate::bus_error:
	uart << "bus_error\n";
	break;

	case TWI::iostate::unknown_error:
	uart << "unknown_error\n";
	break;

	case TWI::iostate::prog_error:
	uart << "prog_error\n";
	break;

	default:
	uart << "desconocido\n";
	break;
    }
}
void bmp280_read_all_mem(uint8_t addr, uint8_t* mem, uint8_t n)
{
    TWI twi(slave_address);
    
    twi << addr;
//    twi.prepare_to_read(n);
    twi.read(mem, n);

    twi.close();

    if (twi.error()){
	my_mcu::UART_iostream uart;
	uart << "ERROR (bmp280_read_all_mem): ";
	twi_print_state(TWI::state());
    }

}

void print_int_as_hex(std::ostream& uart, uint8_t* p, uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i){
	atd::print_int_as_hex(uart, p[i]);
	uart << ' ';
    }
    uart << '\n';
}

void mem_dump_(std::ostream& uart, 
	       uint8_t addr, uint8_t* mem, uint8_t mem_size)
{
    bmp280_read_all_mem(addr, mem, mem_size);

    uart << "Memoria [";
    atd::print_int_as_hex(uart, addr);
    uart << "]:\n";

    print_int_as_hex(uart, mem, mem_size);
    uart << "\n";
}

void mem_dump(std::ostream& uart)
{
//    {
//    constexpr uint8_t mem_size = 24;
//    constexpr uint8_t addr{0x88};
//    uint8_t mem[mem_size];
//    uart << "Calibration params: ";
//
//    mem_dump_(uart, addr, mem, mem_size);
//    }

    // Hay que leerlo en dos partes. 'reset' es de solo escritura,
    // si se intenta leer falla la lectura del resto.
    {
    constexpr uint8_t mem_size = 1;
    constexpr uint8_t addr{0xD0};
    uint8_t mem[mem_size];

    mem_dump_(uart, addr, mem, mem_size);
    }
    {
    constexpr uint8_t mem_size = 3;
    constexpr uint8_t addr{0xF3};
    uint8_t mem[mem_size];
    mem_dump_(uart, addr, mem, mem_size);
    }
    {
    constexpr uint8_t mem_size = 6;
    constexpr uint8_t addr{0xF7};
    uint8_t mem[mem_size];
    mem_dump_(uart, addr, mem, mem_size);
    }


}



void print_mode(std::ostream& out, uint8_t mode)
{
    using Cfg = dev::__BMP280_config;
    if (mode == Cfg::sleep_mode)
	out << "sleep_mode\n";
    else if (mode == Cfg::force_mode)
	out << "force_mode\n";
    else if (mode == Cfg::normal_mode)
	out << "normal_mode\n";
    else
	out << "ERROR: mode desconocido\n\\nn";
}

void print_oversampling(std::ostream& out, uint8_t osr)
{
    using Cfg = dev::__BMP280_config;
    if (osr == Cfg::oversampling_none)
	out << "oversampling_none\n";

    else if (osr == Cfg::oversampling_x1)
	out << "oversampling_x1\n";

    else if (osr == Cfg::oversampling_x2)
	out << "oversampling_x2\n";

    else if (osr == Cfg::oversampling_x4)
	out << "oversampling_x4\n";

    else if (osr == Cfg::oversampling_x8)
	out << "oversampling_x8\n";

    else if (osr == Cfg::oversampling_x16)
	out << "oversampling_x16\n";

    else
	out << "ERROR: desconocido\n";

}

void print_t_sb(std::ostream& out, uint8_t t_sb)
{
    using Cfg = dev::__BMP280_config;

    if (t_sb == Cfg::t_sb_0_5_ms)
	out << "t_sb_0_5_ms\n";

    else if (t_sb == Cfg::t_sb_62_5_ms)
	out << "t_sb_62_5_ms\n";

    else if (t_sb == Cfg::t_sb_125_ms)
	out << "t_sb_125_ms\n";

    else if (t_sb == Cfg::t_sb_250_ms)
	out << "t_sb_250_ms\n";

    else if (t_sb == Cfg::t_sb_500_ms)
	out << "t_sb_500_ms\n";

    else if (t_sb == Cfg::t_sb_1000_ms)
	out << "t_sb_1000_ms\n";

    else if (t_sb == Cfg::t_sb_2000_ms)
	out << "t_sb_2000_ms\n";

    else if (t_sb == Cfg::t_sb_4000_ms)
	out << "t_sb_4000_ms\n";

    else
	out << "ERROR: desconocido\n";
}

void print_filter(std::ostream& out, uint8_t filter)
{
    using Cfg = dev::__BMP280_config;

    if (filter == Cfg::filter_off)
	out << "filter_off\n";

    else if (filter == Cfg::filter_coeff_2)
	out << "filter_coeff_2\n";

    else if (filter == Cfg::filter_coeff_4)
	out << "filter_coeff_4\n";

    else if (filter == Cfg::filter_coeff_8)
	out << "filter_coeff_8\n";

    else if (filter == Cfg::filter_coeff_16)
	out << "filter_coeff_16\n";

    else
	out << "ERROR: desconocido: [" << (int) filter << "]\n";
}

void print_spi3w_en(std::ostream& out, uint8_t spi3w_en)
{
    using Cfg = dev::__BMP280_config;

    if (spi3w_en == Cfg::spi3w_disable)
	out << "spi3w_disable\n";

    else if (spi3w_en == Cfg::spi3w_enable)
	out << "spi3w_enable\n";


    else
	out << "ERROR: desconocido\n";
}

void print_cfg(std::ostream& out, dev::__BMP280_config& cfg)
{
    out << "\n---------- Cfg ----------\n";
    print_mode(out, cfg.mode);
    out << "osrs_t ... ";
    print_oversampling(out, cfg.osrs_t);
    out << "osrs_p ... ";
    print_oversampling(out, cfg.osrs_p);
    out << "t_sb ... ";
    print_t_sb(out, cfg.t_sb);
    out << "filter_coeff ... ";
    print_filter(out, cfg.filter);
    out << "spi3w_en ... ";
    print_spi3w_en(out, cfg.spi3w_en);
    out << "-------------------------\n";
}


void init(Sensor& sensor)
{
    my_mcu::UART_iostream uart;
    uart << "init ... ";
    sensor.init();
    if (sensor.error()){
	uart << "ERROR ... ";
	twi_print_state(sensor.state());
    }
    else
	uart << "OK\n";

    // Modos por defecto que propone Bosch.
    sensor.handheld_device_low_power();
    // sensor.handheld_device_dynamic();
    // sensor.weather_monitoring(); <-- este es forced mode
    // sensor.elevator_floor_change_detector();
    // sensor.drop_detection();
    // sensor.indoor_navigation();

// Configuración a bajo nivel:
//    using Cfg = Sensor::Config;
//    Cfg cfg;
//    cfg.mode   = normal_mode;
//    cfg.osrs_p = oversampling_x16;
//    cfg.osrs_t = oversampling_x2;
//    cfg.filter = filter_coeff_4;
//    cfg.t_sb   = t_sb_62_5_ms; // ODR (see table 14)
//    cfg.spi3w_en = Cfg::spi3w_disable; // es TWI

//   sensor.write(cfg); 


    if (sensor.error()){
	uart << "Error al configurar el sensor\n";
	twi_print_state(sensor.state());
    }
    else
	uart << "cfg OK\n";


// depurar configuración:
    Sensor::Config cfg2;
    sensor.read(cfg2);
    uart << "LEIDOS:\n";

    print_cfg(uart, cfg2);
}


void print_params(std::iostream& uart, Sensor& sensor)
{
    uart << "dig_T1 = [" << sensor.dig_T1() << "]\n";
    uart << "dig_T2 = [" << sensor.dig_T2() << "]\n";
    uart << "dig_T3 = [" << sensor.dig_T3() << "]\n";

    uart << "dig_P1 = [" << sensor.dig_P1() << "]\n";
    uart << "dig_P2 = [" << sensor.dig_P2() << "]\n";
    uart << "dig_P3 = [" << sensor.dig_P3() << "]\n";
    uart << "dig_P4 = [" << sensor.dig_P4() << "]\n";
    uart << "dig_P5 = [" << sensor.dig_P5() << "]\n";
    uart << "dig_P6 = [" << sensor.dig_P6() << "]\n";
    uart << "dig_P7 = [" << sensor.dig_P7() << "]\n";
    uart << "dig_P8 = [" << sensor.dig_P8() << "]\n";
    uart << "dig_P9 = [" << sensor.dig_P9() << "]\n";
}


void debug(std::ostream& out, Sensor& sensor)
{
    Sensor::Temp_and_press tp;
    sensor.read(tp);

    out << "\nsin comp: uT = " << tp.utemperature
	<< "; uP = " << tp.upressure << '\n';
//	<< "comp: T = " << sensor.compensate_T(tp.utemperature) << " ºC; ";
//    int32_t press_q248 = sensor.compensate_P_(tp.upressure);
//    out << "P comp. = " << press_q248 
//			<< " (" << press_q248/25600 << " hPa)\n";

//    auto press = sensor.compensate_P(tp.upressure);
//    out << "P = " << press << " Pa (" << Sensor::Hectopascal{press}
//        << " hPa)\n";

}

void test_bmp280()
{
// init_UART();
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();

    uart << "----------------------------------------\n"
	 << "BMP280\n"
	 << "----------------------------------------\n\n";

// init_TWI();
    TWI_master::turn_on<TWI_frecuency>();

// init_sensor();
    Sensor sensor;
    init(sensor);

    while (1){
	debug(uart, sensor);

	auto [T, P] = sensor.T_and_P();
	uart << "T_and_P = " << T << "; P = " << P << "\n";

	auto [T2, hP] = sensor.T_and_hP();
	uart << "T_and_hP= " << T2 << "; P = " << hP << "\n";

	uart << "T       = " << sensor.T() << "\n";

	// Probar a desconectar el sensor mientras está funcionando. Tiene que
	// generar el error correspondiente.
	if (sensor.no_response())
	    uart << "Error: no response\n";

	if (sensor.error())
	    uart << "Error: error en el sensor!!!\n";

	Micro::wait_ms(2000);
    }
}



int main()
{
    test_bmp280();
}




ISR(TWI_vect)
{
    TWI_master::handle_interrupt();
}


