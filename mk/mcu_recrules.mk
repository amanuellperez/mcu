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


