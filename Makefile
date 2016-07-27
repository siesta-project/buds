
# Define VPATH
VPATH ?= $(shell pwd)

# the default target
.PHONY: default
default: lib

# the default target
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


# Include the makefile in the src directory
include $(TOP_DIR)/src/Makefile.inc

ifneq ($(MPI),0)
 # Include dir:
 #    ./src/mpi
 include $(TOP_DIR)/src/mpi/Makefile.inc
endif


# Libraries depend on the objects
$(LIBRARIES): $(OBJECTS)

# Create target
lib: $(LIBRARIES)

# Include the makefile in the test directory
include $(TOP_DIR)/src/test/Makefile.inc
