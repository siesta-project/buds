TOP_DIR = ..

# Include user-defined setup
-include $(TOP_DIR)/setup.make

# Common makefile stuff:
ifndef _INCLUDED_common
  include $(TOP_DIR)/Makefile.common
endif

ifndef _INCLUDED_default
  include $(TOP_DIR)/Makefile.default
endif

# Include the top-dir include directory
override INC += -I$(TOP_DIR)/include
override INC += -I$(TOP_DIR)/src

# The library
LIBS = $(TOP_DIR)/libbuds.a

COMMON_OBJS = test_utils.o

TESTS = array fs_array c2_array


.PHONY: test tests
test: $(TESTS)
tests: test

# All tests executables are being made phony
# to ensure their execution
.PHONY: $(TESTS)

$(TESTS): $(COMMON_OBJS)

array: $(COMMON_OBJS) array.o
	$(FC) -o $@ $^ $(LIBS) $(INC)
	./$@

fs_array: $(COMMON_OBJS) fs_array.o
	$(FC) -o $@ $^ $(LIBS) $(INC)
	./$@

c2_array: $(COMMON_OBJS) c2_array.o
	$(FC) -o $@ $^ $(LIBS) $(INC)
	./$@

.PHONY: clean
clean:
	@rm -rf *.o *.mod *.MOD $(TESTS)
