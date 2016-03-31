TEST_MPI_DIR = $(TOP_DIR)/src/mpi/test

# Ensure the include path gets updated
INC += -I$(TEST_MPI_DIR)

# Number of test processors
NPROC ?= 4


MPI_TESTS += dist
T_DIST_OBJS += dist.o
T_DIST_OBJS := $(addprefix $(TEST_MPI_DIR)/,$(T_DIST_OBJS))
TEST_MPI_OBJS += $(T_DIST_OBJS)
$(TEST_MPI_DIR)/bud_iDist.f90: $(TEST_MPI_DIR)/Dist.F90
	$(CPP) $(INC) $< > $@
$(TEST_MPI_DIR)/dist: $(TEST_COMMON_OBJS) $(TEST_MPI_DIR)/dist.o
	$(MPIFC) -o $@ $^ $(BUDS_LIB_STATIC) $(FCFLAGS) $(INC)
	@echo "#### TEST -- running: $@"
	-mpirun -np $(NPROC) ./$@


# Ensure correct compilation of sources
$(TEST_MPI_OBJS): FC := $(MPIFC)
$(TEST_MPI_OBJS): CC := $(MPICC)
$(TEST_MPI_OBJS): CXX := $(MPICXX)


TESTS_OBJS += $(addprefix $(TEST_MPI_DIR)/,$(TEST_MPI_OBJS))
MPI_TESTS := $(addprefix $(TEST_MPI_DIR)/,$(MPI_TESTS))

.PHONY: $(MPI_TESTS)
TESTS += $(MPI_TESTS)


# Local Variables:
#  mode: makefile-gmake
# End:
