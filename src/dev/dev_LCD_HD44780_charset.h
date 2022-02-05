// Copyright (C) 2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#ifndef __DEV_LCD_HD44780_CHARSET_H__
#define __DEV_LCD_HD44780_CHARSET_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Map of charset of LCD.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    06/01/2020 v0.0
 *
 *		TODO: lcd << "España"; 
 *		      La 'ñ' no es ascii, luego no la muestra bien.
 *		      ¿Cómo convertir en tiempo de compilación la cadena utf8
 *		      "España" en la secuencia correspondientes de códigos que
 *		      usa el LCD para escribir "España" ???
 *
 ****************************************************************************/

namespace dev{
/*!
 *  Ejemplo de uso:
 *
 *  using symbol = dev::HD44780_charset_A00;
 *  lcd << symbol::of("→"));
 *
 */
struct HD44780_charset_A00{

static constexpr const char end_code[]  = "\x00\x00\x00";
static constexpr const char null_code[] = "\xFF\xFF\xFF"; 
static constexpr const int first_code  = 0x7E;

// Para escribir con 'vim' los símbolos usar: CTRL-V u 221a = escribe √
// Observar que los códigos utf8 que se van a usar son los que estoy
// escribiendo aquí.
static constexpr const char* codes[] = { 
"→", // '\u2192'
"←", // '\u2190'

null_code, null_code, null_code, null_code, null_code, null_code, null_code,
null_code, null_code, null_code, null_code, null_code, null_code, null_code,
null_code, null_code,

null_code, null_code, null_code, null_code, null_code, null_code, null_code,
null_code, null_code, null_code, null_code, null_code, null_code, null_code,
null_code, null_code, 

"\x30\x00", // '\u3000' ideographic space
"｡", // '\uFF61'
"｢", // '\uFF62'
"｣", // '\uFF63'
"､", // '\uFF64'
"･", // '\uFF65'
"ｦ", // '\uFF66'
"ｧ", // '\uFF67'
"ｨ", // '\uFF68'
"ｩ", // '\uFF69'
"ｪ", // '\uFF6A'
"ｫ", // '\uFF6B'
"ｬ", // '\uFF6C'
"ｭ", // '\uFF6D'
"ｮ", // '\uFF6E'
"ｯ", // '\uFF6F'

"ｰ", // '\uFF70'
"ｱ", // '\uFF71'
"ｲ", // '\uFF72'
"ｳ", // '\uFF73'
"ｴ", // '\uFF74'
"ｵ", // '\uFF75'
"ｶ", // '\uFF76'
"ｷ", // '\uFF77'
"ｸ", // '\uFF78'
"ｹ", // '\uFF79'
"ｺ", // '\uFF7A'
"ｻ", // '\uFF7B'
"ｼ", // '\uFF7C'
"ｽ", // '\uFF7D'
"ｾ", // '\uFF7E'
"ｿ", // '\uFF7F'

"ﾀ", // '\uFF80'
"ﾁ", // '\uFF81'
"ﾂ", // '\uFF82'
"ﾃ", // '\uFF83'
"ﾄ", // '\uFF84'
"ﾅ", // '\uFF85'
"ﾆ", // '\uFF86'
"ﾇ", // '\uFF87'
"ﾈ", // '\uFF88'
"ﾉ", // '\uFF89'
"ﾊ", // '\uFF8A'
"ﾋ", // '\uFF8B'
"ﾌ", // '\uFF8C'
"ﾍ", // '\uFF8D'
"ﾎ", // '\uFF8E'
"ﾏ", // '\uFF8F'

"ﾐ", // '\uFF90'
"ﾑ", // '\uFF91'
"ﾒ", // '\uFF92'
"ﾓ", // '\uFF93'
"ﾔ", // '\uFF94'
"ﾕ", // '\uFF95'
"ﾖ", // '\uFF96'
"ﾗ", // '\uFF97'
"ﾘ", // '\uFF98'
"ﾙ", // '\uFF99'
"ﾚ", // '\uFF9A'
"ﾛ", // '\uFF9B'
"ﾜ", // '\uFF9C'
"ﾝ", // '\uFF9D'
"ﾞ", // '\uFF9E'
//"ﾟ", // '\uFF9F'
"º", 

"α", // = "\xce\xb1" 
"ä",
"β",
"ε",
"μ",
"σ",
"ρ",
"gg", // 'g' larga
"√", // '\u221A'
"-1",// ¿cuál es el utf8 de ^-1?
"jj", // 'j' larga
"ˣ",
"¢", 
"£", // '\uC2A3'
"ñ",
"ö", 


"pp", // 'p' larga
"qq", // 'q' larga
"θ",
"\x22\x1e", // "∞" u221E (No puedo escribirlo en símbolo, lo confunde con √. ¿por qué?)
"Ω", //'\u03A9'
"ü",
"Σ",
"π", // '\u03C0'
"\x53\x43", // x bar (average(x)) 
"yy", // 'y' larga
"于", //'\u4E8E'
"万", //'\u4E07'
"円", //'\u5186'
"÷",
"■", //'\u25A0'
"█", //'\u2588'
end_code
};


static constexpr int _of(const char* str, int i) noexcept
{
    return ((codes[i][0] == str[0] and codes[i][1] == str[1] and codes[i][2] == str[2])
        or (codes[i][0] == end_code[0] and codes[i][1] == end_code[1] and codes[i][2] == end_code[2]))
             ? first_code
             : 1 + _of(str, ++i);
}

static constexpr int code_symbol_not_found()
{
    return first_code - 1 + sizeof(codes) / sizeof(const char*);
}

static constexpr char of(const char* str) noexcept
{
    return static_cast<char>((_of(str, 0) == code_symbol_not_found()
                ? '?'
                : _of(str, 0)));
}
};




/***************************************************************************
 *		ALGUNOS 'CARACTERES' QUE PUEDEN SER ÚTILES
 ***************************************************************************/
// DUDA: ¿Podríamos hablar de Glyph en general, sin mencionar el LCD? 
// Hay caracteres de 5 x 8 y también de 5 x 10. De momento para distinguir
// entre los dos uso el nombre.
using LCD_glyph8 = const char [8];

namespace character_glyphs8_basic{
constexpr LCD_glyph8 bell = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0001110,
			    0b0001110,
			    0b0011111,
			    0b0000100,
			    0b0000000 };

constexpr LCD_glyph8 arrow_up = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0000000 };


constexpr LCD_glyph8 arrow_down = 
			  { 0b0000000,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0010101,
			    0b0001110,
			    0b0000100,
			    0b0000000};

constexpr LCD_glyph8 heart_full = 
			  { 0b0000000,
			    0b0001010,
			    0b0011111,
			    0b0011111,
			    0b0001110,
			    0b0000100,
			    0b0000000,
			    0b0000000};

constexpr LCD_glyph8 heart_empty = 
			  { 0b0000000,
			    0b0001010,
			    0b0010101,
			    0b0010001,
			    0b0001010,
			    0b0000100,
			    0b0000000,
			    0b0000000};


constexpr LCD_glyph8 battery_empty = 
			  { 0b0001110,
			    0b0011011,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0010001,
			    0b0011111 };

constexpr LCD_glyph8 battery_half = 
			  { 0b0001110,
			    0b0011011,
			    0b0010001,
			    0b0010001,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111 };

constexpr LCD_glyph8 battery_full = 
			  { 0b0001110,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0011111 };

constexpr LCD_glyph8 musical_note = 
			  { 0b0000000,
			    0b0000011,
			    0b0001101,
			    0b0001001,
			    0b0001011,
			    0b0011011,
			    0b0011000,
			    0b0000000 };


constexpr LCD_glyph8 padlock_close = 
			  { 0b0000000,
			    0b0001110,
			    0b0010001,
			    0b0010001,
			    0b0011111,
			    0b0011011,
			    0b0011011,
			    0b0011111 };

constexpr LCD_glyph8 padlock_open = 
			  { 0b0000000,
			    0b0001110,
			    0b0010000,
			    0b0010000,
			    0b0011111,
			    0b0011011,
			    0b0011011,
			    0b0011111 };

constexpr LCD_glyph8 man = 
			  { 0b0001110,
			    0b0001110,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0001010,
			    0b0010001 };

constexpr LCD_glyph8 skull = 
			  { 0b0000000,
			    0b0001110,
			    0b0010101,
			    0b0011011,
			    0b0001110,
			    0b0001110,
			    0b0000000,
			    0b0000000 };


constexpr LCD_glyph8 speaker_right = 
			  { 0b0000001,
			    0b0000011,
			    0b0001111,
			    0b0001111,
			    0b0001111,
			    0b0000011,
			    0b0000001,
			    0b0000000 };


constexpr LCD_glyph8 plug = 
			  { 0b0001010,
			    0b0001010,
			    0b0011111,
			    0b0010001,
			    0b0010001,
			    0b0001110,
			    0b0000100,
			    0b0000100 };


// Al principio lo llamé 'square', pero eso entra en conflicto con la
// función square de C. Además, se pueden crear símbolos 3, 4, ... cuyos
// nombres serían exponent3, exponent4, ... 
constexpr LCD_glyph8 exponent2 = 
			  { 0b0000110,
			    0b0001001,
			    0b0000010,
			    0b0000100,
			    0b0001111,
			    0b0000000,
			    0b0000000,
			    0b0000000 };


constexpr LCD_glyph8 exponent3 = 
			  { 0b0000110,
			    0b0000001,
			    0b0000010,
			    0b0000001,
			    0b0000110,
			    0b0000000,
			    0b0000000,
			    0b0000000 };


constexpr LCD_glyph8 cuberoot = 
			  { 0b0011011,
			    0b0000110,
			    0b0001010,
			    0b0000110,
			    0b0011010,
			    0b0000010,
			    0b0000110,
			    0b0000010 };

constexpr LCD_glyph8 pacman = 
			  { 0b0000000,
			    0b0001110,
			    0b0011011,
			    0b0011110,
			    0b0011100,
			    0b0011110,
			    0b0011111,
			    0b0001110 };

constexpr LCD_glyph8 pacman_phantom = 
			  { 0b0000000,
			    0b0001110,
			    0b0011111,
			    0b0010101,
			    0b0011111,
			    0b0011111,
			    0b0011111,
			    0b0010101 };



}// namespace


// Busco generalidad, que el código funcione con cualquier tipo de LCD. Por
// eso uso templates.
// Escribe el glyph en la posición i.
// Paso i como segundo parámetro para que quede bien tabulado todo al llamar a
// esta función varias veces. Mirar test. (en general, esta función no debería
// de llamarse nunca, sino la equivalente new_glyphs)
template <typename LCD>
void new_glyph8(LCD& lcd, uint8_t i, LCD_glyph8 glyph)
{
    lcd.set_cgram_address(i*8);

    for (uint8_t j  = 0; j < 8; ++j)
	lcd.write_data_to_CG_or_DDRAM(glyph[j]);

    lcd.set_ddram_address(0x00); // ¿dejar el cursor donde estaba?
}

template <typename LCD>
void __new_glyph8(LCD& lcd, LCD_glyph8 glyph)
{
    for (uint8_t j  = 0; j < 8; ++j)
	lcd.write_data_to_CG_or_DDRAM(glyph[j]);
}

template <typename LCD>
void new_glyphs8(LCD& lcd,
	LCD_glyph8 g1, 
	LCD_glyph8 g2, 
	LCD_glyph8 g3, 
	LCD_glyph8 g4, 
	LCD_glyph8 g5, 
	LCD_glyph8 g6, 
	LCD_glyph8 g7, 
	LCD_glyph8 g8)
{
    lcd.set_cgram_address(0x00);
    __new_glyph8(lcd, g1);
    __new_glyph8(lcd, g2);
    __new_glyph8(lcd, g3);
    __new_glyph8(lcd, g4);
    __new_glyph8(lcd, g5);
    __new_glyph8(lcd, g6);
    __new_glyph8(lcd, g7);
    __new_glyph8(lcd, g8);
    lcd.set_ddram_address(0x00);
}


template <typename LCD>
inline void print_glyph8(LCD& lcd, char i)
{
    lcd.write_data_to_CG_or_DDRAM(i);
}


}// namespace



#endif


