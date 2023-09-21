#include "prj_main.h"

void Main::print_result(std::ostream& out, Sensor::Result error)
{
    using Result = Sensor::Result;

    switch(error){
	break; case Result::ok: 
			out << "OK\n"; 

	break; case Result::not_found:
			out << "ERROR (device not found)\n";

	break; case Result::time_out:
			out << "TIMEOUT\n";

	break; case Result::wrong_CRC:
			out << "WRONG CRC\n";
    }
}

void Main::print_temperature(std::ostream& out)
{
    auto T = sensor_.read_temperature(timeout_ms);
    if (sensor_.is_ok()){
	print_time(out);
	out << ": " << T << " ºC\n";
    }

    else 
	print_result(out, sensor_.errno());

}


// DUDA: estoy pasando TL y TH como 0 y 100º. ¿Algún problema?
// A fin de cuentas no lo vamos a usar para nada, y si se usa el termómetro
// para otra cosa lo lógico es que lo primero que se haga es configurarse.
Sensor::Result Main::sensor_resolution(uint16_t ures)
{
    using Resolution = Sensor::Resolution;

    Resolution res{};

    switch(ures){
	break; case 9: res = Resolution::bits_9;
	break; case 10: res = Resolution::bits_10;
	break; case 11: res = Resolution::bits_11;
	break; case 12: res = Resolution::bits_12;
	break; default: uart << "Logic error: program can't be here\n";
    }

    return sensor_.write_scratchpad(0, 100, res);
}


