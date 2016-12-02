test_mpi_DIR = $(TOP_DIR)/src/mpi/test
# Where to create the output of tests
test_out_DIR = test

# Ensure the include path gets updated
INCLUDES += -I$(test_mpi_DIR)
# Ensure the linking to the library
LDFLAGS += -L.

# Number of test processors
NPROC ?= 4


mpi_TESTS += dist_sm_array
t_mpi_OBJECTS += dist_sm_array.o
$(test_out_DIR)/dist_sm_array: $(test_COMMON_OBJECTS) $(test_out_DIR)/dist_sm_array.o
	$(MPIFC) -o $@ $^ -l$(BUDS_LIB) $(FFLAGS) $(INCLUDES) $(LDFLAGS)
	@echo "#### TEST -- running: $@" >> run_tests.sh
	@echo "mpirun -np $(NPROC) ./$@" >> run_tests.sh


t_mpi_OBJECTS := $(addprefix $(test_out_DIR)/,$(t_mpi_OBJECTS))
mpi_TESTS := $(addprefix $(test_out_DIR)/,$(mpi_TESTS))

.PHONY: $(mpi_TESTS)
TESTS += $(mpi_TESTS)

$(t_mpi_OBJECTS): test-buds
$(t_mpi_OBJECTS): $(BUDS_LIB_SHARED)

# Ensure that the runned tests depend on these objects
test-run: $(mpi_TESTS)

clean-mpi-test:
	-rm -f $(t_mpi_OBJECTS) run_tests.sh
	-rm -f $(test_out_DIR)/*.mod
	-rm -f $(mpi_TESTS)

clean: clean-mpi-test


# Local Variables:
#  mode: makefile-gmake
# End:
