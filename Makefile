TOP_DIR ?= .

# the default target
.PHONY: default
.NOTPARALLEL: default
default: static

# the default target
.PHONY: all
.NOTPARALLEL: all
all: static shared

# Include user-defined setup
-include setup.make

# Common makefile stuff:
include $(TOP_DIR)/Makefile.common

include $(TOP_DIR)/Makefile.default

# Add the installation makefile
include $(TOP_DIR)/Makefile.install

# Add the documentation makefile
include $(TOP_DIR)/Makefile.doc

# Add the distribution creation tool
include $(TOP_DIR)/Makefile.dist

# Add the test directory make file
include $(TOP_DIR)/Makefile.test


# Be sure to include the default include
# directory:
INC += -I$(TOP_DIR)/include


# Include dir:
#    ./src
include $(TOP_DIR)/src/Makefile.inc


# Now create the actual compilation make file
.PHONY: lib libs
#.NOTPARALLEL: lib libs shared static
# Define default library creations
ifndef NO_SHARED
ifndef NO_STATIC
# No options given
NO_SHARED = 1
endif
endif

lib: libs
ifndef NO_STATIC
 ifndef NO_SHARED
libs: static shared
 else
libs: static
 endif
else
 ifndef NO_SHARED
libs: shared
 else
libs:
	@echo "No library build."
 endif
endif

# Static library compilation
$(BUDS_LIB_STATIC): $(OBJS)
	$(AR) $(ARFLAGS) $(BUDS_LIB_STATIC) $^
	$(RANLIB) $(BUDS_LIB_STATIC)

$(BUDS_LIB_SHARED): $(OBJS)
	$(CC) -shared -o $(BUDS_LIB_SHARED) $(CFLAGS) $^

.PHONY: static shared
static: $(BUDS_LIB_STATIC)
shared: $(BUDS_LIB_SHARED)


.PHONY: clean clean-lib clean-src clean-mod clean-test clean-tests
clean-lib:
	rm -f $(BUDS_LIB_STATIC) $(BUDS_LIB_SHARED)
clean-test:
	rm -f src/test/{*.o,*.mod,*.MOD} src/test/mbud*.f90
clean-tests: clean_test
clean-src:
	rm -f src/{*.o,*.mod,*.MOD} src/mbud*.f90
	rm -f src/mpi/{*.o,*.mod,*.MOD} src/mpi/mbud*.f90
clean-mod:
	rm -f $(BUDS_MODDIR)/*.mod $(BUDS_MODDIR)/*.MOD

clean: clean-lib clean-src clean-test clean-mod

