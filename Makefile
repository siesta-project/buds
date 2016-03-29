TOP_DIR ?= .

# the default target
.PHONY: default
.NOTPARALLEL: default
default: libs

# the default target
.PHONY: all
.NOTPARALLEL: all
all: static shared

# Common makefile stuff:
include $(TOP_DIR)/Makefile.common

# Include user-defined setup
-include setup.make

# User-defined setup-file
ifdef SETUP

ifneq ("$(wildcard $(SETUP))","")
 include $(SETUP)
else ifneq ("$(wildcard setup.makes/$(SETUP))","")
 include setup.makes/$(SETUP)
else ifneq ("$(wildcard $(SETUP).make)","")
 include $(SETUP).make
else ifneq ("$(wildcard setup.makes/$(SETUP).make)","")
 include setup.makes/$(SETUP).make
else
 # This is a small information utility
 $(info Could not find: ./$(SETUP))
 $(info Could not find: ./setup.makes/$(SETUP))
 $(info Could not find: ./$(SETUP).make)
 $(info Could not find: ./setup.makes/$(SETUP).make)

 _TMP = $(subst .make,,$(subst ./setup.makes/,$(_NL)  ,$(wildcard ./setup.makes/*.make)))
 $(info Available arguments are: $(_TMP))
 $(error Custom setup (SETUP=<>) file could not be located)
endif

endif # only check if SETUP is defined

.PHONY: setup-list
setup-list:
	@echo
	$(info Available SETUP arguments are:$(_NL)$(subst .make,,$(subst ./setup.makes/,$(_NL)  ,$(wildcard ./setup.makes/*.make))))


include $(TOP_DIR)/Makefile.default

# Add the installation makefile
include $(TOP_DIR)/Makefile.install

# Add the documentation makefile
include $(TOP_DIR)/Makefile.doc

# Add the distribution creation tool
include $(TOP_DIR)/Makefile.dist

# Add the test directory make file
include $(TOP_DIR)/Makefile.test

# Add the clean make file
include $(TOP_DIR)/Makefile.clean


# Be sure to include the default include
# directory:
INC += -I$(TOP_DIR)/include


# Include dir:
#    ./src
include $(TOP_DIR)/src/Makefile.inc


ifeq ($(MPI),1)
# Include dir:
#    ./src/mpi
include $(TOP_DIR)/src/mpi/Makefile.inc

endif


# Now create the actual compilation make file
.PHONY: lib libs
#.NOTPARALLEL: lib libs shared static
lib: libs

# Define default library creations
STATIC ?= 1
SHARED ?= 0

# Create default targets for the libraries
ifeq ($(SHARED),1)
libs: shared
endif
ifeq ($(STATIC),1)
libs: static
endif

# Only in this case should we error out for the user
ifeq ($(STATIC)$(SHARED),00)
libs:
	@echo "No libraries build. (no static or shared)"
	@echo "This is probably an error on your part?"
endif

# Static library compilation
$(BUDS_LIB_STATIC): $(OBJS)
	$(AR) $(ARFLAGS) $(BUDS_LIB_STATIC) $^
	$(RANLIB) $(BUDS_LIB_STATIC)

$(BUDS_LIB_SHARED): $(OBJS)
	$(CC) -shared -o $(BUDS_LIB_SHARED) $(CFLAGS) $(LDFLAGS) $^

.PHONY: static shared
static: $(BUDS_LIB_STATIC)
shared: $(BUDS_LIB_SHARED)

