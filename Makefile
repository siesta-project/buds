
# Define default VPATH
VPATH ?= $(shell pwd)


# the default target
.PHONY: default
default: lib


# SMEKASETTINGS (DO NOT DELETE)
# DO NOT CHANGE CONTENT IN THIS BLOCK
# IT MAY BE OVERWRITTEN WHEN REINSTALLING SMEKA
#
# This Makefile was created by smeka:
#  github.com/zerothi/smeka

# Top-directory of Makefile/source tree
# If need set, do so ABOVE this block!
TOP_DIR ?= .

# Directory of smeka default Makefiles
SMEKA_DIR = smeka

# Include the smeka settings!
include $(TOP_DIR)/$(SMEKA_DIR)/Makefile.smeka

# SMEKAENDSETTINGS (DO NOT DELETE)


# All routines require the special preprocessor files
# We skip this from the VPATH as it really is only INCLUDES
INCLUDES += -I$(TOP_DIR)/include

# Decide on the options for compilation

# Update VPATH with include files
VPATH := $(TOP_DIR)/include:$(VPATH)

#### MPI
MPI ?= 0
ifneq ($(MPI),0)

 # Define the mpi flag
 # the BUD_MPI should now retain the MPI standard
 # I.e. =1, =2, =3, etc.
 FPPFLAGS += -DBUD_MPI=$(MPI)

 CC := $(MPICC)
 CXX := $(MPICXX)
 FC := $(MPIFC)

endif

#### Object-Oriented
OO ?= 0
ifneq ($(OO),0)
 FPPFLAGS += -DBUD_FORTRAN=2003
endif


# Include the makefile in the source directories:
#    ./src
include $(TOP_DIR)/src/Makefile.inc
#    ./src/mpi
include $(TOP_DIR)/src/mpi/Makefile.inc


# This handy target copies from the SOURCES_DIR all sources
# to the current directory
# But ONLY if the current directory is not the top of the project
.PHONY: copy
ifeq ($(TOP_DIR),.)
copy:
	@echo ""
	@echo "make copy does not work when executed from the top BUDS directory"
	@echo "Please create an object directory with an appropriate Makefile"
	@echo ""
else
copy:
	cp $(SOURCES_DIR)/src/*.f90 .
	cp $(SOURCES_DIR)/src/mpi/*.f90 .
endif


# Create source target for creating _only_ the sources.
.PHONY: source-all
source-all:
	$(MAKE) OO=0 MPI=0 source
	$(MAKE) OO=1 MPI=0 source
	$(MAKE) OO=0 MPI=1 source
	$(MAKE) OO=1 MPI=1 source

.PHONY: source
source: source-src source-mpi

# Dependent on the option we can fake a VPATH to contain
# any pre-created sources, if they exist we can simply use those
SOURCES_DIR = $(TOP_DIR)/sources
ifneq ($(MPI),0)
 SOURCES_DIR := $(SOURCES_DIR)_mpi
endif
ifneq ($(OO),0)
 SOURCES_DIR := $(SOURCES_DIR)_oo
endif
# Ensure the creation of the sub-directory
source-dir:
	mkdir -p $(SOURCES_DIR)
source-src: source-dir


# We add all plugins that is defined in the
# setup.make file (or on the command-line)
ifdef PLUGINS
 $(eval orig_TOP_DIR:=$(TOP_DIR))\
 $(foreach plugin,$(PLUGINS),\
 $(eval TOP_DIR:=$(TOP_DIR)/plugins/$(plugin)/)\
 $(eval include $(TOP_DIR)/Makefile.plugin))
 $(eval TOP_DIR:=$(orig_TOP_DIR))
endif

# Define that this is *not* a plugin
IS_PLUGIN := 0


# The linker is a fortran compiler
LINK := $(FC)


$(BUDS_LIB_STATIC): $(OBJECTS)
$(BUDS_LIB_SHARED): $(OBJECTS)

# Libraries depend on the objects
lib: $(LIBRARIES)


# Include the makefile in the test source directories:
#    ./src/test
include $(TOP_DIR)/src/test/Makefile.inc
#    ./src/mpi/test
include $(TOP_DIR)/src/mpi/test/Makefile.inc
