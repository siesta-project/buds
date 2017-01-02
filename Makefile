
# Define VPATH
VPATH ?= $(shell pwd)

# the default target
.PHONY: default
default: lib

# the all target
.PHONY: all
all: static shared


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

# Libraries depend on the objects
$(LIBRARIES): $(OBJECTS)

# Create target
lib: $(LIBRARIES)

# Create target
source: source-src source-mpi

# Include the makefile in the test source directories:
#    ./src/test
include $(TOP_DIR)/src/test/Makefile.inc
#    ./src/mpi/test
include $(TOP_DIR)/src/mpi/test/Makefile.inc
