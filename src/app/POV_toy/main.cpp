// Es más eficiente y más sencillo de implementar usando los puertos
// (ver ejemplo de port/POV_toy). Sin embargo, es más fácil soldar con estas
// conexiones.
#include <avr_pin.h>
#include <avr_time.h>


using namespace avr;



class Main_app{
public:
    void run();

private:
    Pin_de_salida<15> p1_;
    Pin_de_salida<16> p2_;
    Pin_de_salida<17> p3_;
    Pin_de_salida<18> p4_;
    Pin_de_salida<19> p5_;
    Pin_de_salida<23> p6_;
    Pin_de_salida<24> p7_;
    Pin_de_salida<25> p8_;

    void POVDisplay(uint8_t fila_glyph);
    void clear();
};

void Main_app::POVDisplay(uint8_t fila_glyph)
{
    p1_.write(fila_glyph & 0b00000001);
    p2_.write(fila_glyph & 0b00000010);
    p3_.write(fila_glyph & 0b00000100);
    p4_.write(fila_glyph & 0b00001000);
    p5_.write(fila_glyph & 0b00010000);
    p6_.write(fila_glyph & 0b00100000);
    p7_.write(fila_glyph & 0b01000000);
    p8_.write(fila_glyph & 0b10000000);

    wait_ms(2);
}


void Main_app::clear()
{
    p1_.write_zero();
    p2_.write_zero();
    p3_.write_zero();
    p4_.write_zero();
    p5_.write_zero();
    p6_.write_zero();
    p7_.write_zero();
    p8_.write_zero();
}



void Main_app::run()
{
    while(1){
	POVDisplay(0b00001110);
	POVDisplay(0b00011000);
	POVDisplay(0b10111101);
	POVDisplay(0b01110110);
	POVDisplay(0b00111100);
	POVDisplay(0b00111100);
	POVDisplay(0b00111100);
	POVDisplay(0b01110110);
	POVDisplay(0b10111101);
	POVDisplay(0b00011000);
	POVDisplay(0b00001110);

	clear();
	wait_ms(10);
    }
}


int main()
{
    Main_app app;

    app.run();

}



