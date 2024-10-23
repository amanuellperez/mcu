# Copyright (C) 2019-2024 Manuel Perez
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

# *************************************************************************
# Reglas genéricas para compilación recursiva.
#
# Basado en:
# 	http://www.gnu.org/software/make/manual/make.html#Phony-Targets
# 	http://lackof.org/taggart/hacking/make-example/
#
# Definir DIRS y DIST_DIRS
#
# *************************************************************************

# Si no se definen directorios diferentes de instalar, usamos DIRS
DIST_DIRS ?= $(DIRS)

# El truco consiste en cambiar el nombre a los
# directorios para crear nuevas targets.
SUBDIRS  = $(DIRS:%=build-%)
CLEANDIRS= $(DIRS:%=clean-%)
SUPERCLEANDIRS= $(DIRS:%=super_clean-%)
TESTDIRS = $(DIRS:%=test-%)
DISTDIRS = $(DIST_DIRS:%=dist-%)

all: $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $(@:build-%=%) all


.PHONY: $(DISTDIRS)
dist: $(DISTDIRS)

$(DISTDIRS):
	@$(MAKE) -C $(@:dist-%=%) dist


.PHONY: $(TESTDIRS)
test: $(TESTDIRS)

$(TESTDIRS):
	@$(MAKE) -C $(@:test-%=%) test


.PHONY: $(CLEANDIRS)
clean: $(CLEANDIRS)

$(CLEANDIRS):
	@$(MAKE) -C $(@:clean-%=%) clean

.PHONY: $(SUPERCLEANDIRS)
super_clean: $(SUPERCLEANDIRS)

$(SUPERCLEANDIRS):
	@$(MAKE) -C $(@:super_clean-%=%) super_clean


.PHONY: $(DIRS)


