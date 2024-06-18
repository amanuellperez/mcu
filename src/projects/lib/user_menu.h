// Copyright (C) 2021 Manuel Perez 
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

#ifndef __DEV_USER_MENU_H__
#define __DEV_USER_MENU_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Gestión automática de menus en LCDs.
 *
 *    Se trata de menús para mostrar en LCD y usando un teclado de 3 teclas:
 *    UP, DOWN y OK.
 *
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    28/02/2021 v0.0 Versión completamente experimental. Puede que la
 *		      reescriba por completo en el futuro cuando tenga más
 *		      experiencia.
 *
 ****************************************************************************/
#include "user_choose_string.h"

namespace dev{

// El menú es un conjunto de opciones
template <typename Main>
struct Option {
    int menu_id; // número de menu
    int str_id;  // id de la cadena
    using F = void (Main::*)();
    F action;	// acción a tomar en caso de que se elija esta opción.
		// Será nullptr si lo que se quiere es mostrar otro menú.
    int next_menu_id; // menú que mostrar en caso de que se elija esta opción.
                // Se ignora su valor si action != nullptr.
};

// Concibo el menu completo como un array de menus. 
// Cuando quiero mostrar un menu concreto en el LCD esta clase se encarga de
// localizarlo en memoria, para poder dar acceso a las distintas opciones de
// este menu concreto.
template <typename Options, typename Main>
class _Menu_number{
public:
    using value_type     = Options::value_type;
    using const_pointer  = Options::const_pointer;
    using const_iterator = Options::const_iterator;
    using size_type      = Options::size_type;
    using Option         = dev::Option<Main>;

    explicit constexpr _Menu_number(uint8_t menu_id)
	:p0_(init_p0(menu_id)), pe_(init_pe(menu_id)) {}

    constexpr size_type size() const { return pe_ - p0_;}
    
    // Devuelve la opción i dentro de este menu.
    constexpr value_type operator[](size_type i) const
    { return p0_[i]; }

    constexpr const_iterator begin() const {return p0_;}
    constexpr const_iterator end() const {return pe_;}


private:
// Data
// ¿Por qué no elegir [p0, size)? Si quiero que el constructor sea constexpr,
// necesito calcular directamente todos los datos en el constructor. Calcular
// size es más elaborado que calcular pe_. Por eso la elección.
    const_iterator p0_;
    const_iterator pe_;

// Helper functions
    constexpr const_iterator init_p0(uint8_t menu_id) const
    {
	return std::find_if(Options::begin(), Options::end(),
				[&](const Option& opt) {
				    return opt.menu_id == menu_id;
				});
    }

    constexpr const_iterator init_pe(uint8_t menu_id) const
    {
	return std::find_if(p0_, Options::end(),
			    [&](const Option& opt) {
				    return opt.menu_id != menu_id;
				});
    }

};




// No pasar menu_str de forma static, ya que si se mete el menu en memoria
// flash, habría que cargarla y esta clase cambiaría. 
// TODO: que el menú pueda estar guardado en memoria o en EEPROM. (hay que
// cambiar menu_str a un gestor de memoria).
template <typename Options, typename Main>
class _Options_as_array_of_strings{
public:
    using Menu = _Menu_number<Options, Main>;

    using size_type = std::size_t;

    _Options_as_array_of_strings(Menu menu, const char* const* menu_str)
	    :menu_{menu}, menu_str_{menu_str}{}

    constexpr const char* operator[](size_type i) const
    {return menu_str_[menu_[i].str_id];}

    constexpr size_type size() const {return menu_.size();}
    
private:
    Menu menu_;
    const char* const* menu_str_;
};



/*!
 *  \brief  Gestiona un menú por completo.
 *
 *  Ver el test correspondiente como ejemplo.
 *
 */
template <typename LCD,
          typename Keyrow3,
	  typename Options,
	  typename Main>
class LCD_Menu{
public:
    using Menu_number = _Menu_number<Options, Main>;
    using Options_as_array_of_strings =
				_Options_as_array_of_strings<Options, Main>;

    LCD_Menu(Main* main_app,
             LCD& lcd,
	     Keyrow3& keyrow,
             const char* const* menu_str)
        : main_{main_app}, lcd_{lcd}, menu_str_{menu_str}
    {}

    void show_menu(uint8_t menu_id);
  

private:
    Main* main_;
    LCD& lcd_;
    Keyrow3 keyrow_;
    const char* const* menu_str_; // <-- esto en realidad tiene que
                                  // ser un gestor de memoria,que da acceso a
                                  // memoria RAM o a flash.
};


template <typename LCD,
          typename Keyrow3,
	  typename Options,
	  typename Main>
void LCD_Menu<LCD, Keyrow3, Options, Main>::show_menu(uint8_t menu_id)
{
    Menu_number menu{menu_id};

    uint8_t res = 
	dev::user_choose_string_lineal<LCD::cols(), LCD::rows()>(
		lcd_, keyrow_,
		Options_as_array_of_strings(menu, menu_str_))
		      .pos(0, 0)
		      .show(0);

    if (menu[res].action == nullptr)
	show_menu(menu[res].next_menu_id);

    else{
	using F = void (Main::*)();
	F f = menu[res].action;
	(main_->*f)();
    }
}



// syntax sugar
// ------------
template <typename Options, typename Main, typename LCD, typename Keyrow3>
LCD_Menu<LCD, Keyrow3, Options, Main> lcd_menu(Main* main_app,
                                                 LCD& lcd,
                                                 Keyrow3 keyrow,
                                                 const char* const* menu_str)
{
    return LCD_Menu<LCD, Keyrow3, Options, Main>
				{main_app, lcd, keyrow, menu_str};
}


}// namespace


#endif


