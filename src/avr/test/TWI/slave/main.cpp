// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

// OJO: para que el TWI funcione a 100kHz es necesario operar
// a 8 MHz!!
// Conectar 2 termómetros LM75 con direcciónes 1 y 2. Muestra
// por USART la temperatura que marcan.
#include "../../../avr_TWI.h"
#include "../../../avr_USART.h"
#include "../../../avr_time.h"
#include "../../../avr_interrupt.h"

#include <atd_buffer.h>
#include <algorithm>
#include <array>


constexpr uint8_t buffer_size = 10;

inline void traza(const char* fname)
{
    auto tmp = TWCR;

    avr::UART_iostream uart;
    uart << ">>> " << fname << ": TWCR = " << static_cast<uint16_t>(tmp) << "; TWINT = ";

    if (atd::is_one_bit<TWINT>::of(TWCR))
        uart << "1\n";
    else
        uart << "0\n";
}

inline void traza_twcr()
{
    auto tmp = TWCR;
    avr::UART_iostream uart;
    uart << "\tTWCR = " << static_cast<uint16_t>(tmp) << "\n";
}

// Si fuera secuencial, ejecutaríamos:
//	    int ret = TWI::send_start();
//
//  Pero si es asíncrono, ejecutamos:
//	    TWI::send_start();
//	    ...
//	    if (TWI::return() != running) 
//		int ret = TWI::return();  // resultado de la última operación
//					  // pedida a TWI
enum class __TWI_slave_state {
    listening, // No tiene datos.
    data_nack,
    bus_error,

    // slave receiver mode
    // -------------------
    get_area_full, // se está recibiendo un mensaje pero el buffer de
                   // entrada se ha llenado.
    eor,           // end of reading: ha finalizado la recepción de un
                   // mensaje; quedan datos en el buffer de entrada
                   // pendientes de leer

    // slave transmitter mode
    // ----------------------
    put_area_empty, // La put area está vacía y nos piden datos.
                    // Hay que escribir en ella.

    eow,      // end of writting, everything ok
    eow_pane, // end of writting, put area not empty. No se han
              // enviado todos los datos de la put area.
    eow_pas   // end of writting, put area small. Se han enviado todos los bytes
              // pero el master quería más.
};

// TODO: La idea es que esto no sea más que una ampliacion del TWI con 1 byte
// de buffer. Le ponemos una put/get area a TWI en esta clase y ocultamos el
// protocolo al usuario.
// Responsabilidades de esta clase:
//	1.- Ponerle un buffer al hardware de TWI (que carece de el).
//	    Dos posibilidades:	
//		(1) put area = get area: mismo buffer de entrada/salida.
//		(2) put area != get area. Usamos dos buffers diferentes.
//
//	2.- Ocultar el protocolo al usuario. El usuario no tiene que saber
//	    nada de START/SLA+WR ... /STOP. 
//
// Realmente es un std::streambuf. De momento no lo voy a heredar, pero voy 
// a seguir el mismo planteamiento que con los streambuf, de esa forma las
// dos implementaciones (streambuf y TWI_slave) serán parecidas siendo más
// consistente todo y más sencillo de mantener.
class TWI_slave{
public:
    using TWI = avr::TWI_basic; // TODO: parametrizarlo con esto. De esta forma
				// queda independizado del microcontrolador.
    using iostate= __TWI_slave_state;
    using streamsize = uint8_t;

    // Tamaño de la put/get area.
    static constexpr uint8_t buffer_size = ::buffer_size; // TODO: parametro de template

    /// Enables TWI interface definiendo la frecuencia del SCL 
    /// a la que vamos a operar.
    /// f_scl = frecuencia en kilohercios de SCL (tipica: 100 y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = AVR_CLOCK_FREQUENCY_IN_HZ>
    struct on{
	// Enables as a slave in the address SLAVE_ADDRESS.
	template <uint8_t TWI_slave_address>
	static void in()
	{
	    TWI::SCL_frequency_in_kHz<f_scl, f_clock>();
	    avr::enable_all_interrupts();
	    TWI::init_slave<TWI_slave_address,1>(); // 1 = TWI::interrupt_enable()

	    reset();
	}
    };

    /// Lee del TWI como máximo N bytes.
    /// En caso de que el dispositivo esté ocupado o el buffer esté vacío
    /// devuelve 0.
    /// Postcondición: get_area() == empty()
    static streamsize read_buffer(std::byte* buffer, streamsize N);

    /// Escribe en el buffer de salida buf[0, n). 
    /// Devuelve el número de bytes escritos.
    /// A día de hoy si se n > buffer_size no hace nada, ya que esta primera
    /// versión está limitada a enviar el buffer completo de una sola vez (no
    /// se puede escribir poco a poco). Si en el futuro se usa, cambiarlo y
    /// darle más flexibilidad. Esta es una versión de aprendizaje.
    static streamsize write_buffer(const std::byte* buf, streamsize n);

    static void stop_transmission();


    /// Indica si el dispositivo está ocupado ejecutando una función o no.
    static bool is_busy() {return busy_;}

    // Función que va dentro de la ISR
    static void handle_interrupt();

    // estados: solo válidos cuando is_busy() == false!!!
    // --------------------------------------------------
    // TODO: creo que puedo fundier busy_ dentro de los states. Revisarlo.
    // Ahorraríamos 1 byte.
    static bool listening() {return state() == iostate::listening;}

    // de lectura
    static bool eor() {return state() == iostate::eor;}
    static bool get_area_full() { return state() == iostate::get_area_full;}




private:
    static inline iostate state_;
    static inline bool busy_;	

    static inline atd::TWI_iobuffer<buffer_size> buffer_;
    static inline std::byte* pa_;   // posición actual de la put/get area.

    // Estado en el que queda el slave después de ejecutar algo.
    // Sólo es válido cuando is_busy() == false!!!
    static iostate state() {return state_;}


    // Helper functions
    // ----------------
    static void reset()
    {
	state_ = iostate::listening;
        pa_ = nullptr;  // ni leyendo ni escribiendo, esto realmente sería para
                        // depurar
        busy_ = true;	// está escuchando
	TWI::reset_slave<1>(); // interrupt_enable = 1
    }

    static void set_busy()
    {
	busy_ = true;
	TWI::interrupt_enable();
    }

    static void set_no_busy()
    {
	TWI::interrupt_disable();  
	busy_ = false;
    }




// Respuesta a los estados de TWI en slave receiver mode
    static void srm_sla_w();
    static void srm_data_ack();
    static void srm_data_nack();
    static void srm_stop_or_repeated_start();


//// Respuesta a los estados de TWI en slave transmitter mode:
    static void stm_sla_r();
    static void stm_data_ack();
    static void stm_data_nack();

// Respuesta a miscellaneous states
    static void bus_error();
    static void unknown_error();
};


// DUDA: la datasheet admite poder responder a esto con nack, esto es, no
// reconocemos la recepción de datos. ¿Cuándo puede ser útil eso?
// ¿el slave está ocupado haciendo cosas y no puede responder y por eso
// responde con NACK? Pero basta con desconectarlo para que no responda a
// SLA+w. <-- pero esto sería como decirle al master: "error: dispositivo
// desconocido", mientras que si responde OK a SLA+W, pero luego te devuelve
// NACK al data le estas diciendo al master: "estoy ocupado, inténtalo más
// tarde".
inline void TWI_slave::srm_sla_w()
{
    buffer_.reset_as_input();
    TWI::slave_receive_data_with_ACK();
}


inline void TWI_slave::srm_data_ack()
{
    buffer_.in_write_one(TWI::data());
    if (buffer_.in_is_full()){
	set_no_busy();
	state_ = iostate::get_area_full;
    }
    else{
	TWI::slave_receive_data_with_ACK();
    }

}


inline void TWI_slave::srm_data_nack()
{
    set_no_busy();
    TWI::recover_from_bus_error();      
    state_ = iostate::data_nack;
}


inline void TWI_slave::srm_stop_or_repeated_start()
{
    set_no_busy();
    TWI::not_addressed_slave_mode();
    state_ = iostate::eor; 
}





inline void TWI_slave::stm_sla_r()
{
    set_no_busy();
    buffer_.reset_as_output();
    state_ = iostate::put_area_empty;
}


void TWI_slave::stm_data_ack()
{
    if (buffer_.out_is_empty()){
	state_ = iostate::eow_pas;
	reset();
	set_no_busy();
	return;
    }

    if (buffer_.out_size() == 1)
	TWI::slave_transmit_byte_received_NACK(buffer_.out_read_one());

    else
	TWI::slave_transmit_byte_received_ACK(buffer_.out_read_one());

}


void TWI_slave::stm_data_nack()
{
    if (buffer_.out_is_empty())
	state_ = iostate::eow;

    else 
	state_ = iostate::eow_pane;

    TWI::not_addressed_slave_mode();
    set_no_busy();
}



inline void TWI_slave::bus_error()
{
    set_no_busy();
    TWI::recover_from_bus_error();      
    state_ = iostate::bus_error;

}

// Este caso lo tienen así implementado en la app-note, pero
// ¿qué estatus puede ser este desconocido? ¿error de hardware?
// ¿protección contra error de software?
inline void TWI_slave::unknown_error()
{
    // set_no_busy(); (qué hacer???) Espero que el usuario haga algo?
    TWI::not_addressed_slave_mode();
    state_ = iostate::listening;
}



TWI_slave::streamsize TWI_slave::write_buffer(const std::byte* buf,
                                              streamsize n)
{
    if (is_busy())
	return 0;

    if (n > buffer_size) // Devuelvo el control para que el usuario sepa
	return 0;	 // que tiene que aumentar el tamaño del buffer.

    return buffer_.out_write_all_n(buf, n);
}

void TWI_slave::stop_transmission()
{
    avr::Interrupts_lock lock;   // garantizo control del flujo.
    reset();
}


// Mira a ver si hay datos que leer en el bufer del TWI. 
// En caso de que los haya los lee guardándolos en buffer.
// Si no hay datos a leer, devuelve inmediatamente el control.
// Devuelve el número de bytes leidos.
TWI_slave::streamsize TWI_slave::read_buffer(std::byte* buffer, streamsize N)
{
//    avr::Interrupts_lock lock;

    if (is_busy())
	return 0;
 

    streamsize n = buffer_.in_read_all_n(buffer, N);

    if (state_ == iostate::get_area_full){
	TWI::slave_receive_data_with_ACK();
	set_busy();
    }

    else{ // state_ == iostate::eor
	if (buffer_.in_is_empty()){
	    state_ = iostate::listening;
	    set_busy(); 
	}
    }

    return n;
}



// TODO: para poder independizarlo del microcontrolador es necesario
// darle un nombre a los estados que dependan del protocolo de TWI!!!
void TWI_slave::handle_interrupt()
{

    using SRM = TWI::iostate::slave_receiver_mode;
    using STM = TWI::iostate::slave_transmitter_mode;

    switch (TWI::status()){

    // slave receiver mode
    // -------------------
	case SRM::sla_w: 
	    srm_sla_w(); 
	    break;

	case SRM::sla_w_data_ack:
	case SRM::general_call_data_ack:
	    srm_data_ack();
	    break;

	case SRM::sla_w_data_nack:
	case SRM::general_call_data_nack:
	    srm_data_nack();
            break;


	case SRM::stop_or_repeated_start:
	    srm_stop_or_repeated_start();
            break;



    // slave transmitter mode
    // ----------------------
	case STM::sla_r:
	    stm_sla_r();
	    break;

	case STM::data_ack:
	    stm_data_ack();
	    break;

	case STM::data_nack:
	    stm_data_nack();
	    break;


    // miscellaneous states
    // --------------------
	case TWI::iostate::bus_error:
	    bus_error();
            break;

        default: // ¿Qué ha sucedido? ¡¡¡status desconocido!!!
	    unknown_error();
	    break;
    }
}













constexpr uint8_t slave_address = 0x10;

enum class Service{
    error   ,
    unknown ,
    service1, 
    service2
};

constexpr std::byte service1_name {0x34};
constexpr std::byte service2_name {0x87};

Service read_service_name(std::array<std::byte, buffer_size>& params_in)
{
    avr::UART_iostream uart;

    while (!TWI_slave::eor()){ // TODO: ojo: qué pasa si se envia algo diferente?
	if (TWI_slave::get_area_full()) {
	    uart << "Error: get_area_full!!! buffer muy pequeño!!!\n\n";
	    TWI_slave::stop_transmission();
	    return Service::error;
	}
    } // while

    if (!TWI_slave::eor()){
            uart << "Tendría que estar en eor, pero esta en otro estado\n";
	    TWI_slave::stop_transmission();
	    return Service::error;
    }

    TWI_slave::streamsize n = TWI_slave::read_buffer(params_in.data(), params_in.size());

    if (n == 0) {
	uart << "No se ha leído nada!\n";
	TWI_slave::stop_transmission();
	return Service::error;
    }

    if (params_in[0] == service1_name)
	return Service::service1;

    else if (params_in[0] == service2_name)
	return Service::service2;

    uart << "Servicio desconocido [" << static_cast<uint16_t>(params_in[0])
         << "]\n";
    return Service::unknown;

}

// params_in[0] = nombre del servicio
// params_in[...] = resto de parámetros
void service1(const std::array<std::byte, buffer_size>& params_in,
		    std::array<std::byte, buffer_size>& params_out)
{
    avr::UART_iostream uart;
    uart << "Ejecutando service1\n";
    uart << "params_in: "
	 << static_cast<uint16_t>(params_in[1]) << ", "
	 << static_cast<uint16_t>(params_in[2]) << ", "
	 << static_cast<uint16_t>(params_in[3]) << '\n';

    // respondemos 1 byte con 45!!!
    params_out[0] = std::byte{45};
}


// params_in[0] = nombre del servicio
// params_in[...] = resto de parámetros
void service2(const std::array<std::byte, buffer_size>& params_in)
{
    avr::UART_iostream uart;
    uart << "Ejecutando service2\n";
}

void test_servidor()
{
    avr::UART_iostream uart;
    uart << "test_servidor\n"
         << "-------------\n"
         << "Elegir buffer_size de manera que se garantice que todo el mensaje "
            "entre en el buffer!!!\n\n";

    while (1){

	while (TWI_slave::listening()){
	    asm("nop");
	}

	std::array<std::byte, buffer_size> params_in;
	std::array<std::byte, buffer_size> params_out;

	// TODO: read_service_name miente! Lee tambien params_in!!! CAMBIARLO!
	Service srv_name = read_service_name(params_in);
	switch (srv_name){
	    case Service::service1:
		// Ejemplo: tiene parametros de entrada y salida
		service1(params_in, params_out);  // esta función 
			    // llama a: out = service1(in);
			    // transformando params_in y params_out en las
			    // structuras de datos correspondientes.
		// TODO: que params_out sea un "vector estatico". Esto es,
		// [begin, end) dentro de un array[begin, ...) <-- ¿esto es
		// std::span? De esa forma no tengo que pasarle la longitud!!!
//		write_params_out(params_out.data(), params_out.data() + 1);
		break;

	    case Service::service2:
		// Ejemplo: no tiene parametros de entrada, pero sí de salida
		service2(params_out); 
//		write_params_out(params_out);
		break;

	    case Service::unknown:
		TWI_slave::stop_transmission();
		break;

	    case Service::error:
		TWI_slave::stop_transmission();
		uart << "Transmission error\n";
		break;
	}

    }
}




void test_read()
{
    avr::UART_iostream uart;
    uart << "test_read\n"
	 << "---------\n"
	 << "Probar con distintos buffer_size\n\n";

    while (1){
	std::byte buffer[buffer_size];
	
	while (!TWI_slave::eor()){

            if (TWI_slave::get_area_full()) {
                uint8_t n = TWI_slave::read_buffer(buffer, buffer_size);
                uart << "\nXXX Recibidos " << static_cast<uint16_t>(n)
                     << " datos:\n";
                for (uint8_t i = 0; i < n; ++i)
                    uart << '\t' << std::to_integer<uint16_t>(buffer[i])
                         << '\n';
            }
        } // while

        while (TWI_slave::eor()){
	if (uint8_t n = TWI_slave::read_buffer(buffer, buffer_size)){
	    uart << "\nFFF Recibidos " << static_cast<uint16_t>(n)  << " datos:\n";
	    for (uint8_t i = 0; i < n; ++i)
		uart << '\t' << std::to_integer<uint16_t>(buffer[i]) << '\n';
	}
	}

	uart << "\n\n------------------------\n";
	
	wait_ms(100);

    }// while(1)

}



int main() 
{
    avr::UART_iostream uart;
    avr::cfg_basica(uart);
    uart.on();

    uart << "Empezando\n";

    TWI_slave::on<50>::in<slave_address>();

    uart << "TWI enable\n";
    
//    test_read();
    test_servidor();
}



ISR(TWI_vect)
{
    TWI_slave::handle_interrupt();
}// ISR



