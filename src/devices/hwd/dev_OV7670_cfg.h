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

#ifndef __DEV_OV7670_CFG_H__
#define __DEV_OV7670_CFG_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Configuraciones básicas de la cámara.
 *	
 * DUDA
 *  Si no se van a usar todas las posibles configuraciones creo (???) que se
 *  está reservando en ROM memoria innecesaria, lo cual es un desperdicio.
 *
 * HISTORIA
 *    Manuel Perez
 *    06/12/2023 Escrito
 *
 ****************************************************************************/

namespace dev{
// Copia de los nombres dados en la datasheet (table 5)
struct OV7670_register_name{
static constexpr uint8_t GAIN = 0x00;
static constexpr uint8_t BLUE = 0x01;
static constexpr uint8_t RED = 0x02;
static constexpr uint8_t VREF = 0x03;
static constexpr uint8_t COM1 = 0x04;
static constexpr uint8_t BAVE = 0x05;
static constexpr uint8_t GbAVE = 0x06;
static constexpr uint8_t AECHH = 0x07;
static constexpr uint8_t RAVE = 0x08;
static constexpr uint8_t COM2 = 0x09;
static constexpr uint8_t PID = 0x0A;
static constexpr uint8_t VER = 0x0B;
static constexpr uint8_t COM3 = 0x0C;
static constexpr uint8_t COM4 = 0x0D;
static constexpr uint8_t COM5 = 0x0E;
static constexpr uint8_t COM6 = 0x0F;
static constexpr uint8_t AECH = 0x10;
static constexpr uint8_t CLKRC = 0x11;
static constexpr uint8_t COM7 = 0x12;
static constexpr uint8_t COM8 = 0x13;
static constexpr uint8_t COM9 = 0x14;
static constexpr uint8_t COM10 = 0x15;
// RSVD 16
static constexpr uint8_t HSTART = 0x17;
static constexpr uint8_t HSTOP = 0x18;
static constexpr uint8_t VSTART = 0x19; // la datasheet lo llama VSTRT???
static constexpr uint8_t VSTOP = 0x1A;
static constexpr uint8_t PSHFT = 0x1B;
static constexpr uint8_t MIDH = 0x1C;
static constexpr uint8_t MIDL = 0x1D;
static constexpr uint8_t MVFP = 0x1E;
static constexpr uint8_t LAEC = 0x1F;
static constexpr uint8_t ADCCTR0 = 0x20;
static constexpr uint8_t ADCCTR1 = 0x21;
static constexpr uint8_t ADCCTR2 = 0x22;
static constexpr uint8_t ADCCTR3 = 0x23;
static constexpr uint8_t AEW = 0x24;
static constexpr uint8_t AEB = 0x25;
static constexpr uint8_t VPT = 0x26;
static constexpr uint8_t BBIAS = 0x27;
static constexpr uint8_t GbBIAS = 0x28;
static constexpr uint8_t RSVD = 0x29;
static constexpr uint8_t EXHCH = 0x2A;
static constexpr uint8_t EXHCL = 0x2B;
static constexpr uint8_t RBIAS = 0x2C;
static constexpr uint8_t ADVFL = 0x2D;
static constexpr uint8_t ADVFH = 0x2E;
static constexpr uint8_t YAVE = 0x2F;
static constexpr uint8_t HSYST = 0x30;
static constexpr uint8_t HSYEN = 0x31;
static constexpr uint8_t HREF = 0x32;
static constexpr uint8_t CHLF = 0x33;
static constexpr uint8_t ARBLM = 0x34;
// RSVD 35-36
static constexpr uint8_t ADC = 0x37;
static constexpr uint8_t ACOM = 0x38;
static constexpr uint8_t OFON = 0x39;
static constexpr uint8_t TSLB = 0x3A;
static constexpr uint8_t COM11 = 0x3B;
static constexpr uint8_t COM12 = 0x3C;
static constexpr uint8_t COM13 = 0x3D;
static constexpr uint8_t COM14 = 0x3E;
static constexpr uint8_t EDGE = 0x3F;
static constexpr uint8_t COM15 = 0x40;
static constexpr uint8_t COM16 = 0x41;
static constexpr uint8_t COM17 = 0x42;
static constexpr uint8_t AWBC1 = 0x43;
static constexpr uint8_t AWBC2 = 0x44;
static constexpr uint8_t AWBC3 = 0x45;
static constexpr uint8_t AWBC4 = 0x46;
static constexpr uint8_t AWBC5 = 0x47;
static constexpr uint8_t AWBC6 = 0x48;
// RSVD 49-4A
static constexpr uint8_t REG4B = 0x4B;
static constexpr uint8_t DNSTH = 0x4C;
// RSVD 4D-4E
static constexpr uint8_t MTX1 = 0x4F;
static constexpr uint8_t MTX2 = 0x50;
static constexpr uint8_t MTX3 = 0x51;
static constexpr uint8_t MTX4 = 0x52;
static constexpr uint8_t MTX5 = 0x53;
static constexpr uint8_t MTX6 = 0x54;
static constexpr uint8_t BRIGHT = 0x55;
static constexpr uint8_t CONTRAS = 0x56;
static constexpr uint8_t CONTRAS_CENTER = 0x57;
static constexpr uint8_t MTXS = 0x58;
// RSVD 59-61
static constexpr uint8_t LCC1 = 0x62;
static constexpr uint8_t LCC2 = 0x63;
static constexpr uint8_t LCC3 = 0x64;
static constexpr uint8_t LCC4 = 0x65;
static constexpr uint8_t LCC5 = 0x66;
static constexpr uint8_t MANU = 0x67;
static constexpr uint8_t MANV = 0x68;
static constexpr uint8_t GFIX = 0x69;
static constexpr uint8_t GGAIN = 0x6A;
static constexpr uint8_t DBLV = 0x6B;
static constexpr uint8_t AWBCTR3 = 0x6C;
static constexpr uint8_t AWBCTR2 = 0x6D;
static constexpr uint8_t AWBCTR1 = 0x6E;
static constexpr uint8_t AWBCTR0 = 0x6F;
static constexpr uint8_t SCALING_XSC = 0x70;
static constexpr uint8_t SCALING_YSC = 0x71;
static constexpr uint8_t SCALING_DCWCTR = 0x72;
static constexpr uint8_t SCALING_PCLK_DIV = 0x73;
static constexpr uint8_t REG74 = 0x74;
static constexpr uint8_t REG75 = 0x75;
static constexpr uint8_t REG76 = 0x76;
static constexpr uint8_t REG77 = 0x77;
// RSVD 78-79
static constexpr uint8_t SLOP = 0x7A;
static constexpr uint8_t GAM1 = 0x7B;
static constexpr uint8_t GAM2 = 0x7C;
static constexpr uint8_t GAM3 = 0x7D;
static constexpr uint8_t GAM4 = 0x7E;
static constexpr uint8_t GAM5 = 0x7F;
static constexpr uint8_t GAM6 = 0x80 ;
static constexpr uint8_t GAM7 = 0x81 ;
static constexpr uint8_t GAM8 = 0x82;
static constexpr uint8_t GAM9 = 0x83;
static constexpr uint8_t GAM10 = 0x84;
static constexpr uint8_t GAM11 = 0x85;
static constexpr uint8_t GAM12 = 0x86;
static constexpr uint8_t GAM13 = 0x87;
static constexpr uint8_t GAM14 = 0x88;
static constexpr uint8_t GAM15 = 0x89;
// RSVD 8A-8B
static constexpr uint8_t RGB444 = 0x8C;
// RSVD 8D-91
static constexpr uint8_t DM_LNL = 0x92;
static constexpr uint8_t DM_LNH = 0x93;
static constexpr uint8_t LCC6 = 0x94;
static constexpr uint8_t LCC7 = 0x95;
// RSVD 96-9C
static constexpr uint8_t BD50ST = 0x9D;
static constexpr uint8_t BD60ST = 0x9E;
static constexpr uint8_t HAECC1 = 0x9F;
static constexpr uint8_t HAECC2 = 0xA0;
// RSVD A1
static constexpr uint8_t SCALING_PCLK_DELAY = 0xA2;
// RSVD A3
static constexpr uint8_t NT_CTRL = 0xA4;
static constexpr uint8_t BD50MAX = 0xA5;
static constexpr uint8_t HAECC3 = 0xA6;
static constexpr uint8_t HAECC4 = 0xA7;
static constexpr uint8_t HAECC5 = 0xA8;
static constexpr uint8_t HAECC6 = 0xA9;
static constexpr uint8_t HAECC7 = 0xAA;
static constexpr uint8_t BD60MAX = 0xAB;
static constexpr uint8_t STR_OPT = 0xAC;
static constexpr uint8_t STR_R = 0xAD;
static constexpr uint8_t STR_G = 0xAE;
static constexpr uint8_t STR_B = 0xAF;
// RSVD B0
static constexpr uint8_t ABLC1 = 0xB1;
// RSVD B2
static constexpr uint8_t THL_ST = 0xB3;
// RSVD B4
static constexpr uint8_t THL_DLT = 0xB5;
// RSVD B6-BD
static constexpr uint8_t AD_CHB = 0xBE;
static constexpr uint8_t AD_CHR = 0xBF;
static constexpr uint8_t AD_CHGb = 0xC0;
static constexpr uint8_t AD_CHGr = 0xC1;
// RSVD C2-C8
static constexpr uint8_t SATCTR = 0xC9;

}; // OV7670_register_name
 

} // namespace dev

#endif
