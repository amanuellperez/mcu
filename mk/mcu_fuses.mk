# Copyright (C) 2019-2024 Manuel Perez <manuel2perez@proton.me>
#
# This file is part of the MCU++ Library.
#
# MCU++ Library is a free library: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
# -----
# Fuses
# -----

# hfuse, lfuse and efuse
# -----------------------
READ_LHE_FUSE_STRING = -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h

ifdef HFUSE 
WRITE_LHE_FUSE_STRING= -U hfuse:w:$(HFUSE):m 
endif

ifdef LFUSE 
WRITE_LHE_FUSE_STRING += -U lfuse:w:$(LFUSE):m 
endif

ifdef EFUSE 
WRITE_LHE_FUSE_STRING += -U efuse:w:$(EFUSE):m 
endif


# fuse0, fuse1, ... fuse9
# -----------------------
READ_N_FUSE_STRING = -U fuse0:r:-:h \
					 -U fuse1:r:-:h \
					 -U fuse2:r:-:h \
					 -U fuse3:r:-:h \
					 -U fuse4:r:-:h \
					 -U fuse5:r:-:h \
					 -U fuse6:r:-:h \
					 -U fuse7:r:-:h \
					 -U fuse8:r:-:h \
					 -U fuse9:r:-:h 
# TODO: como se lee el fuse0x0A???

ifdef FUSE0
WRITE_N_FUSE_STRING = -U fuse0:w:$(FUSE0):m 
endif


ifdef FUSE1
WRITE_N_FUSE_STRING += -U fuse1:w:$(FUSE1):m 
endif

ifdef FUSE2
WRITE_N_FUSE_STRING += -U fuse2:w:$(FUSE2):m 
endif

ifdef FUSE3
WRITE_N_FUSE_STRING += -U fuse3:w:$(FUSE3):m 
endif

ifdef FUSE4
WRITE_N_FUSE_STRING += -U fuse4:w:$(FUSE4):m 
endif

ifdef FUSE5
WRITE_N_FUSE_STRING += -U fuse5:w:$(FUSE5):m 
endif

ifdef FUSE6
WRITE_N_FUSE_STRING += -U fuse6:w:$(FUSE6):m 
endif

ifdef FUSE7
WRITE_N_FUSE_STRING += -U fuse7:w:$(FUSE7):m 
endif

ifdef FUSE8
WRITE_N_FUSE_STRING += -U fuse8:w:$(FUSE8):m 
endif

ifdef FUSE9
WRITE_N_FUSE_STRING += -U fuse9:w:$(FUSE9):m 
endif



# atmega328p
# ----------
ifeq ($(MCU), atmega328p)
LFUSE_DEFAULT = 0x62
HFUSE_DEFAULT = 0xD9
EFUSE_DEFAULT = 0xFF

LFUSE_FAST_FUSE = 0xE2
LFUSE_CLOCK_OUTPUT_FUSE = 0xA2
LFUSE_CLOCK_OUTPUT_DIVIDE_BY_8 = 0x22
LFUSE_SET_EXTERNAL_CRYSTAL_8MHZ = 0x7F

HFUSE_EEPROM_SAVE_FUSE = 0xD7


FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m 
READ_FUSE_STRING=$(READ_LHE_FUSE_STRING)
WRITE_FUSE_STRING=$(WRITE_LHE_FUSE_STRING)
endif


# atmega4809
# ----------
ifeq ($(MCU), atmega4809)
READ_FUSE_STRING=$(READ_N_FUSE_STRING)
WRITE_FUSE_STRING=$(WRITE_N_FUSE_STRING)
endif


